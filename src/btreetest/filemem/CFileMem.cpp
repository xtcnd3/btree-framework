/************************************************************
**
** file:	CFileMem.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains a class presenting files as linearly
** addressable memory.
**
************************************************************/

#include "CFileMem.h"

CFileMem::CFileMem ()
{
#if defined (WIN32)
	SYSTEM_INFO			systemInfo;
#endif

#if defined (_HAS_MFC)
	m_pFile = NULL;
#elif defined (WIN32)
	m_hFile = INVALID_HANDLE_VALUE;
#elif defined (LINUX)
	m_filedes = -1;
#endif

	m_psCache = NULL;

	m_nLength = 0ULL;

#if defined (WIN32)
	GetSystemInfo (&systemInfo);

	m_nCacheLineGranularity = (uint64_t) systemInfo.dwAllocationGranularity;
#elif defined (LINUX)
	m_nCacheLineGranularity = (uint64_t) getpagesize ();
#endif

	m_nCacheLineBits = DEFAULT_LINEBITS;
	m_uCacheLineMask = (1ULL << m_nCacheLineBits) - 1ULL;
	m_nCacheLineSize = m_nCacheLineGranularity;

#if defined (WIN32)
	m_hFileMapping = INVALID_HANDLE_VALUE;
#endif

	m_eMode = VIEW_FILEMAPPED;

	m_aszFilename[0] = '\0';
	m_uOpenFlags = 0;

	m_bDeleteOnClose = false;
	m_bReadOnly = false;

	m_bCacheLineSizeBinary = false;
	m_uCacheLineSizeMask = 0x0ULL;
	m_nCacheLineSizeBits = 0x0UL;

	m_nFromSubscript = ~0;
	m_nToSubscript = ~0;
	m_nPageSubscript = ~0;

#if defined (USE_PERFORMANCE_COUNTERS)
	m_nHitCtr = 0;
	m_nMissCtr = 0;
#endif
}

CFileMem::~CFileMem ()
{
#if defined (_HAS_MFC)
	if (m_pFile != NULL)
#elif defined (WIN32)
	if (m_hFile != INVALID_HANDLE_VALUE)
#elif defined (LINUX)
	if (m_filedes != -1)
#endif
	{
		close ();
	}
}

bool CFileMem::init (bool bPrepare, bool bLoad)
{
	uint64_t			ui64;

#if defined (WIN32) && !defined (_HAS_MFC)
	DWORD				dwRead;
#endif

	if (bPrepare)
	{
		switch (m_eMode)
		{
#if defined (WIN32)
		case VIEW_FILEMAPPED:	if (m_hFileMapping == INVALID_HANDLE_VALUE)
								{
									if (m_nLength != 0ULL)
									{
#if defined (_HAS_MFC)
										m_hFileMapping = CreateFileMapping (m_pFile->m_hFile, NULL, (m_bReadOnly ? PAGE_WRITECOPY : PAGE_READWRITE) | SEC_COMMIT, 0, 0, NULL);
#else
										m_hFileMapping = CreateFileMapping (m_hFile, NULL, (m_bReadOnly ? PAGE_WRITECOPY : PAGE_READWRITE) | SEC_COMMIT, 0, 0, NULL);
#endif
									
										if ((m_hFileMapping == NULL) || (m_hFileMapping == INVALID_HANDLE_VALUE))
										{
											m_hFileMapping = INVALID_HANDLE_VALUE;

											return (false);
										}
									}
								}
#else
		case VIEW_FILEMAPPED:	
#endif
		case VIEW_RWCOPIED:		
		case VIEW_RCOPIED:		if (m_psCache == NULL)
								{
									m_psCache = (fileCache_t *) malloc ((size_t)get_numCacheLines () * sizeof (fileCache_t));

									if (m_psCache == NULL)
									{
										return (false);
									}

									for (ui64 = 0ULL; ui64 < get_numCacheLines (); ui64++)
									{
										m_psCache[ui64].nPage = ~0x0ULL;
										m_psCache[ui64].pMappedView = NULL;
										m_psCache[ui64].pMirrorView = NULL;
										m_psCache[ui64].uState = FILECACHE_STATE_INIT;
									} // for (ui64 = 0ULL; ui64 < get_numCacheLines (); ui64++)
								}

								break;

		case VIEW_RWLOAD:		
		case VIEW_RLOAD:		if (m_psCache == NULL)
								{
									m_psCache = (fileCache_t *) malloc (sizeof (fileCache_t));

									if (m_psCache == NULL)
									{
										return (false);
									}

									m_psCache[0].nPage = ~0x0ULL;
									m_psCache[0].pMappedView = NULL;
									m_psCache[0].pMirrorView = NULL;
									m_psCache[0].uState = FILECACHE_STATE_INIT;
								}

								break;

		default:				FILEMEM_ASSERT (false, "CFileMem::init: unknown mode during preparation");

								break;
		} // switch (m_eMode)
	} // if (init)

	if (bLoad)
	{
#if defined (_DEBUG)
		FILEMEM_ASSERT (m_psCache != NULL, "CFileMem::init: cache is invalid");
 #if defined (WIN32)
  #if defined (_HAS_MFC)
		FILEMEM_ASSERT (m_pFile != NULL, "CFileMem::init: file is invalid");
  #else
		FILEMEM_ASSERT (m_hFile != INVALID_HANDLE_VALUE, "CFileMem::init: file is invalid");
  #endif
 #elif defined (LINUX)
		FILEMEM_ASSERT (m_filedes != -1, "CFileMem::init: file is invalid");
 #endif
#endif

		switch (m_eMode)
		{
		case VIEW_FILEMAPPED:	
		case VIEW_RWCOPIED:		
		case VIEW_RCOPIED:		
								break;

		case VIEW_RWLOAD:		if (!(m_psCache[0].uState & FILECACHE_STATE_VALID))
								{
									m_psCache[0].pMirrorView = malloc ((size_t) getLength ());

									FILEMEM_ASSERT (m_psCache[0].pMirrorView != NULL, "CFileMem::init: insufficient memory");

#if defined (_HAS_MFC)
									m_pFile->SeekToBegin ();

									FILEMEM_ASSERT (m_pFile->Read (m_psCache[0].pMirrorView, (UINT) getLength ()) == (UINT) getLength (), "CFileMem::init: cannot read complete file");
#elif defined (WIN32)
									{
										LARGE_INTEGER		sPos;

										sPos.QuadPart = 0ULL;

										SetFilePointerEx (m_hFile, sPos, &sPos, FILE_BEGIN);

										FILEMEM_ASSERT (ReadFile (m_hFile, m_psCache[0].pMirrorView, (uint32_t) getLength (), &dwRead, NULL) != 0, "CFileMem::init: cannot read complete file");
										FILEMEM_ASSERT (dwRead == (DWORD) getLength (), "CFileMem::init: cannot read complete file");
									}
#elif defined (LINUX)
									lseek64 (m_filedes, 0LL, SEEK_SET);

									FILEMEM_ASSERT (read(m_filedes, m_psCache[0].pMirrorView, (size_t) getLength ()) == (size_t) getLength (), "cannot read complete file");
#endif
								}
								else
								{
									FILEMEM_ASSERT (m_psCache[0].nPage == 0ULL, "CFileMem::init: CFileMem panic: page fault!");
								}
								
		case VIEW_RLOAD:		if (!(m_psCache[0].uState & FILECACHE_STATE_VALID))
								{
									m_psCache[0].pMappedView = malloc ((size_t) getLength ());

									FILEMEM_ASSERT (m_psCache[0].pMappedView != NULL, "CFileMem::init: insufficient memory");

#if defined (_HAS_MFC)
									m_pFile->SeekToBegin ();

									FILEMEM_ASSERT (m_pFile->Read (m_psCache[0].pMappedView, (UINT) getLength ()) == (UINT) getLength (), "CFileMem::init: cannot read complete file");
#elif defined (WIN32)
									{
										LARGE_INTEGER	sPos;

										sPos.QuadPart = 0ULL;

										SetFilePointerEx (m_hFile, sPos, &sPos, FILE_BEGIN);

										FILEMEM_ASSERT (ReadFile (m_hFile, m_psCache[0].pMappedView, (uint32_t) getLength (), &dwRead, NULL) != 0, "CFileMem::init: cannot read complete file");
										FILEMEM_ASSERT (dwRead == (DWORD) getLength (), "CFileMem::init: cannot read complete file");
									}
#elif defined (LINUX)
									lseek64 (m_filedes, 0LL, SEEK_SET);

									FILEMEM_ASSERT (read(m_filedes, m_psCache[0].pMappedView, (size_t) getLength ()) == (size_t) getLength (), "CFileMem::init: cannot read complete file");
#endif


									m_psCache[0].nPage = 0ULL;
									m_psCache[0].uState |= FILECACHE_STATE_VALID;
								}
								else
								{
#if defined (_DEBUG)
									FILEMEM_ASSERT (m_psCache[0].nPage == 0ULL, "CFileMem::init: CFileMem panic: page fault!");
#endif
								}

								break;

		default:				FILEMEM_ASSERT (false, "CFileMem::init: unknown mode selected during load");

								break;
		} // switch (m_eMode)
	} // if (load)

	return (true);
}

void CFileMem::exit (bool bExit, bool bUnload, bool bInvalidate)
{
	uint64_t			ui64;

#if defined (WIN32) && !defined (_HAS_MFC)
	DWORD				dwLen;
#endif

	if (bUnload || bExit || bInvalidate)
	{
		switch (m_eMode)
		{
		case VIEW_FILEMAPPED:	
		case VIEW_RWCOPIED:		
		case VIEW_RCOPIED:		flush ();

#if defined (WIN32)
								if ((m_eMode == VIEW_FILEMAPPED) && (m_hFileMapping != INVALID_HANDLE_VALUE))
								{
									FILEMEM_ASSERT (CloseHandle (m_hFileMapping), "CFileMem::exit: failed to close file as part of the unmapping");
									
									m_hFileMapping = INVALID_HANDLE_VALUE;
								}
#endif

								break;

		case VIEW_RWLOAD:		if (bUnload || bExit)
								{
									if (m_psCache != NULL)
									{
										if ((m_psCache[0].pMirrorView != NULL) || (m_psCache[0].pMappedView != NULL))
										{
											if (::memcmp (m_psCache[0].pMappedView, m_psCache[0].pMirrorView, (size_t) getLength ()) != 0)
											{
												seek (0ULL);

#if defined (_HAS_MFC)
												m_pFile->Write (m_psCache[0].pMappedView, (UINT) getLength ());
#elif defined (WIN32)
												WriteFile (m_hFile, m_psCache[0].pMappedView, (DWORD) getLength (), &dwLen, NULL);
#elif defined (LINUX)
												write (m_filedes, m_psCache[0].pMappedView, (size_t) getLength ());
#endif
											} // if (::memcmp (m_psCache[0].pMappedView, m_psCache[0].pMirrorView, (size_t) getLength ()) != 0)
										} // if ((m_psCache[0].pMirrorView != NULL) && (m_psCache[0].pMappedView != NULL))

										if (m_psCache[0].pMirrorView != NULL)
										{
											free (m_psCache[0].pMirrorView);
										}
									}
								} // if (unload || exit)

		case VIEW_RLOAD:		if (bUnload || bExit)
								{
									if (m_psCache != NULL)
									{
										if ((m_psCache[0].pMappedView != NULL) && (m_psCache[0].uState & FILECACHE_STATE_VALID) && (m_psCache[0].nPage == 0ULL))
										{
											free (m_psCache[0].pMappedView);

											m_psCache[0].nPage = ~0x0ULL;
											m_psCache[0].uState = FILECACHE_STATE_INIT;
										}
									}
								} // if (unload || exit)

								break;

		default:				FILEMEM_ASSERT (false, "CFileMem::exit: unknown mode selected during unload");

								break;
		} // switch (m_eMode)
	} // if (unload || exit || invalidate)

	if (bInvalidate || bExit)
	{
		if (m_psCache != NULL)
		{
			for (ui64 = 0ULL; ui64 < get_numCacheLines (); ui64++)
			{
				switch (m_eMode)
				{
				case VIEW_FILEMAPPED:	if (m_psCache[ui64].pMappedView != NULL)
										{
#if defined (WIN32)
											FILEMEM_ASSERT (UnmapViewOfFile (m_psCache[ui64].pMappedView), "CFileMem::exit: cannot unmap cache line");
#elif defined (LINUX)
											FILEMEM_ASSERT (munmap (m_psCache[ui64].pMappedView, (size_t) get_cacheLineSize ()) == 0, "CFileMem::exit: cannot unmap cache line");
#endif
										} // if (m_psCache[ui64].pMappedView != NULL)

										break;

				case VIEW_RWCOPIED:		if (m_psCache[ui64].pMirrorView != NULL)
										{
											free ((void *) m_psCache[ui64].pMirrorView);
										}

				case VIEW_RCOPIED:		if (m_psCache[ui64].pMappedView != NULL)
										{
											free ((void *) m_psCache[ui64].pMappedView);
										}

										break;

				case VIEW_RWLOAD:		
				case VIEW_RLOAD:		
										break;

				default:				FILEMEM_ASSERT (false, "CFileMem::exit: unknown mode selected during de-allocation");

										break;
				} // switch (m_eMode)

				switch (m_eMode)
				{
				case VIEW_FILEMAPPED:	
				case VIEW_RWCOPIED:		
				case VIEW_RCOPIED:		m_psCache[ui64].nPage = ~0x0ULL;
										m_psCache[ui64].pMappedView = NULL;
										m_psCache[ui64].pMirrorView = NULL;
										m_psCache[ui64].uState = FILECACHE_STATE_INIT;

										break;

				case VIEW_RWLOAD:		
				case VIEW_RLOAD:		
										break;

				default:				FILEMEM_ASSERT (false, "CFileMem::exit: unknown mode selected during cache reset");

										break;	
										
				} // switch (m_eMode)
			} // for (ui64 = 0ULL; ui64 < get_numCacheLines (); ui64++)
		}
	} // if (invalidate || exit)

	if (bExit)
	{
#if defined (WIN32)
		if (m_hFileMapping != INVALID_HANDLE_VALUE)
		{
			FILEMEM_ASSERT (CloseHandle (m_hFileMapping), "CFileMem::exit: cannot close file");
		}

		m_hFileMapping = INVALID_HANDLE_VALUE;
#endif

		if (m_psCache != NULL)
		{
			free ((void *) m_psCache);
		}

		m_psCache = NULL;
	} // if (exit)
}

bool CFileMem::open (const char *pFName, uint32_t nFlags, bool bDeleteOnClose)
{
	bool		bRtrn = false;

#if defined (_HAS_MFC)
	if (m_pFile != NULL)
#elif defined (WIN32)
	if (m_hFile != INVALID_HANDLE_VALUE)
#elif defined (LINUX)
	if (m_filedes != -1)
#endif
	{
		close ();
	}

	m_bDeleteOnClose = bDeleteOnClose;

	m_bReadOnly = !(nFlags & CFileMem::modeWrite);

#if defined (WIN32)

	DWORD				access = 0;
	DWORD				share = 0;
	DWORD				creation = OPEN_EXISTING;
	DWORD				attr = FILE_ATTRIBUTE_NORMAL;
	struct _finddata_t	fileinfo;

	if ((nFlags & CFileMem::modeReadWrite) == 0)
	{
		access = GENERIC_READ;
	}

	access |= (nFlags & CFileMem::modeRead) ? GENERIC_READ : 0;
	access |= (nFlags & CFileMem::modeWrite) ? GENERIC_WRITE : 0;

	if (nFlags & CFileMem::shareDenyNone)
	{
		share = FILE_SHARE_READ | FILE_SHARE_WRITE;
	}

	if (nFlags & CFileMem::shareDenyRead)
	{
		share &= ~FILE_SHARE_READ;
	}

	if (nFlags & CFileMem::shareDenyWrite)
	{
		share &= ~FILE_SHARE_WRITE;
	}

	if (nFlags & CFileMem::shareExclusive)
	{
		share = 0;
	}

	if (nFlags & CFileMem::modeCreate)
	{
		if (_findfirst (pFName, &fileinfo) != -1)
		{
			creation = TRUNCATE_EXISTING;
		}
		else
		{
			creation = CREATE_NEW;
		}
	}

	if (nFlags & CFileMem::modeNoTruncate)
	{
		creation = OPEN_ALWAYS;
	}

	if (m_bDeleteOnClose)
	{
		attr |= FILE_FLAG_DELETE_ON_CLOSE;
	}

	if (m_bReadOnly)
	{
		attr |= FILE_ATTRIBUTE_READONLY;
	}

	if (nFlags & CFileMem::osNoBuffer)
	{
		attr |= FILE_FLAG_NO_BUFFERING;
	}

	if (nFlags & CFileMem::osWriteThrough)
	{
		attr |= FILE_FLAG_WRITE_THROUGH;
	}

	if (nFlags & CFileMem::osRandomAccess)
	{
		attr |= FILE_FLAG_RANDOM_ACCESS;
	}

	if (nFlags & CFileMem::osSequentialScan)
	{
		attr |= FILE_FLAG_SEQUENTIAL_SCAN;
	}

	m_hFile = CreateFileA (pFName, access, share, NULL, creation, attr, 0);

	bRtrn = (m_hFile != INVALID_HANDLE_VALUE);

#if defined (_HAS_MFC)
	m_pFile = bRtrn ? new CFile (m_hFile) : NULL;

	bRtrn = (m_pFile != NULL);
#endif
	
#elif defined (LINUX)

	int		openFlags = O_TRUNC;
	mode_t	createMode = S_IRUSR;

	if (nFlags & modeCreate)
	{
		openFlags |= O_CREAT;
	}

	if (nFlags & modeNoTruncate)
	{
		openFlags &= ~O_TRUNC;
	}

	if ((nFlags & modeReadWrite) == modeReadWrite)
	{
		openFlags |= O_RDWR;

		createMode |= S_IWUSR;
	}
	else
	{
		if (nFlags & modeRead)
		{
			openFlags |= O_RDONLY;
		}

		if (nFlags & modeWrite)
		{
			openFlags |= O_WRONLY;

			createMode |= S_IWUSR;
		}
	}

	m_filedes = ::open (pFName, openFlags, createMode);

	if (m_filedes > 0)
	{
		bRtrn = true;

		if (m_bDeleteOnClose)
		{
			unlink (pFName);
		}
	}

#endif

	if (bRtrn)
	{
#if defined (_HAS_MFC)
		m_nLength = m_pFile->GetLength ();
#elif defined (WIN32)
		LARGE_INTEGER		sFileSize;

		GetFileSizeEx (m_hFile, &sFileSize);

		m_nLength = sFileSize.QuadPart;
#elif defined (LINUX)
		m_nLength = lseek64 (m_filedes, 0LL, SEEK_END);

		lseek64 (m_filedes, 0LL, SEEK_SET);
#endif

#if defined (LINUX)
		strcpy (m_aszFilename, pFName);
#else
		strcpy_s (m_aszFilename, sizeof (m_aszFilename), pFName);
#endif

		m_uOpenFlags = nFlags;

		if (!init (true, false))
		{
			close ();

			bRtrn = false;
		} // if (!init (true, true))
	} // if (rtrn)

	return (bRtrn);	
}

bool CFileMem::close ()
{
	return (close (true));
}

bool CFileMem::close (bool bReset)
{
	bool				bRtrn = true;

	exit (true, true, true);

#if defined (_HAS_MFC)
	if (m_pFile != NULL)
	{
		m_pFile->Close ();

		delete m_pFile;

		m_pFile = NULL;
	} // if (m_pFile != NULL)
#elif defined (WIN32)
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle (m_hFile);

		m_hFile = INVALID_HANDLE_VALUE;
	}
#elif defined (LINUX)
	if (m_filedes != -1)
	{
		::close (m_filedes);

		m_filedes = -1;
	} // if (m_filedes != -1)
#endif

	if (bReset)
	{
		m_nLength = 0ULL;

		m_aszFilename[0] = '\0';
		m_uOpenFlags = 0;
	}

	return (bRtrn);
}

bool CFileMem::flush ()
{
	uint64_t			ui64;

#if defined (_HAS_MFC)
	if ((m_pFile != NULL) && (!m_bReadOnly))
#elif defined (WIN32)
	if ((m_hFile != INVALID_HANDLE_VALUE) && (!m_bReadOnly))
#elif defined (LINUX)
	if ((m_filedes != -1) && (!m_bReadOnly))
#endif
	{
		for (ui64 = 0ULL; ui64 < get_numCacheLines (); ui64++)
		{
			flushLine (ui64);
		}

#if defined (_HAS_MFC)
		m_pFile->Flush ();
#elif defined (WIN32)
		FlushFileBuffers (m_hFile);
#elif defined (LINUX)
		ioctl (m_filedes, BLKFLSBUF, 0);
#endif

		return (true);
	}

	return (false);
}

void CFileMem::unload ()
{
	exit (true, true, true);
}

bool CFileMem::autoSetup (uint32_t nCacheEffP, uint32_t nCacheEffQ, uint32_t nMaxEffFileSizeP, uint32_t nMaxEffFileSizeQ, bool bReadOnly)
{
	bool					bRetval = true;
	uint64_t				nFreePhysMem;
	uint64_t				nMaxPhysMem;
	uint64_t				nMaxCacheLines;
	uint32_t				nMaxCacheLineBits;
	uint32_t				nCacheLineBits = 0;
#if defined (WIN32)
	MEMORYSTATUSEX			sMemStatus;
#elif defined (LINUX)
	FILE					*pfMeminfo;
	char					acszBuffer[1024];
	char					*pszPos;
#endif
	view_e					eAutoMode;

	if ((nCacheEffP > nCacheEffQ) || (nCacheEffQ == 0UL))
	{
		nCacheEffP = 4UL;
		nCacheEffQ = 5UL;

		bRetval = false;
	}

	if ((nMaxEffFileSizeP > nMaxEffFileSizeQ) || (nMaxEffFileSizeQ == 0UL))
	{
		nMaxEffFileSizeP = 1UL;
		nMaxEffFileSizeQ = 8UL;

		bRetval = false;
	}

#if defined (WIN32)
	sMemStatus.dwLength = sizeof (sMemStatus);

	GlobalMemoryStatusEx (&sMemStatus);

	nFreePhysMem = sMemStatus.ullAvailPhys;
#elif defined (LINUX)
	pfMeminfo = fopen ("/proc/meminfo", "r");

	FILEMEM_ASSERT (pfMeminfo != NULL, "CFileMem::autoSetup: cannot open: //proc//meminfo");

	nFreePhysMem = ~0x0LLU;

	while (!(feof (pfMeminfo)))
	{
		if (NULL == fgets (acszBuffer, sizeof (acszBuffer) / sizeof (*acszBuffer) - 1, pfMeminfo))
		{
			break;
		}

		if (strncmp ("MemFree:", acszBuffer, strlen ("MemFree:")) == 0)
		{
			pszPos = acszBuffer;

			pszPos += strlen ("MemFree:");

			pszPos += strspn (pszPos, " \t");

			sscanf (pszPos, "%llu", &nFreePhysMem);

			break;
		}
	}

	FILEMEM_ASSERT (nFreePhysMem != ~0x0LLU, "CFileMem::autoSetup: unable to determine available physical memory");

	fclose (pfMeminfo);
#endif

	nMaxPhysMem = (nFreePhysMem * nMaxEffFileSizeP) / nMaxEffFileSizeQ;

	if (getLength () <= nMaxPhysMem)
	{
		if (m_bReadOnly || bReadOnly)
		{
			eAutoMode = VIEW_RLOAD;
		}
		else
		{
			eAutoMode = VIEW_RWLOAD;
		}

		setMode (eAutoMode);
	}
	else
	{
		nMaxCacheLines = nMaxPhysMem;
		nMaxCacheLines /= m_nCacheLineGranularity;

		for (nMaxCacheLineBits = 0UL; nMaxCacheLineBits < 32UL; nMaxCacheLineBits++)
		{
			if (nMaxCacheLines < (1ULL << nMaxCacheLineBits))
			{
				using namespace std;
				
				nMaxCacheLineBits = min (nMaxCacheLineBits, nMaxCacheLineBits - 1);

				break;
			}
		}

		bool				bUseMappedView = false;
		uint64_t			nCacheSize;
		uint64_t			nCacheLines;
		uint64_t			nEffCacheSize;

		if (!bReadOnly)
		{
			for (nCacheLineBits = nMaxCacheLineBits; nCacheLineBits > 0UL; nCacheLineBits--)
			{
				nCacheSize = ((1ULL << nCacheLineBits) * m_nCacheLineGranularity);

				if (nCacheSize <= getLength ())
				{
					nCacheLines = (getLength () + (m_nCacheLineGranularity - 1ULL)) / m_nCacheLineGranularity;
					nEffCacheSize = nCacheLines * m_nCacheLineGranularity;

					if (nEffCacheSize <= ((getLength () * nCacheEffQ) / nCacheEffP))
					{
						bUseMappedView = true;

						break;
					}
				}
			}
		}

		if (bUseMappedView)
		{
			setMode (VIEW_FILEMAPPED);
			setCacheCfg (nCacheLineBits, m_nCacheLineGranularity);
		}
		else
		{
			uint64_t	nEffDiff = ~0x0ULL, nDiff;
			uint32_t	nBestCacheLineBits = nMaxCacheLineBits;
			uint64_t	nBestCacheLineSize = getLength () / (1ULL << nBestCacheLineBits);
			uint64_t	nCacheLineSize;

			for (nCacheLineBits = nMaxCacheLineBits; nCacheLineBits > 0UL; nCacheLineBits--)
			{
				nCacheLineSize = nMaxPhysMem / (1ULL << nCacheLineBits);
				
				nCacheLines = (getLength () + (nCacheLineSize - 1ULL)) / nCacheLineSize;
				
				nEffCacheSize = nCacheLines * nCacheLineSize;
				
				if (nEffCacheSize > getLength ())
				{
					nDiff = nEffCacheSize - getLength ();
				}
				else
				{
					nDiff = getLength () - nEffCacheSize;
				}

				if (nDiff < nEffDiff)
				{
					nEffDiff = nDiff;

					nBestCacheLineBits = nCacheLineBits;
					nBestCacheLineSize = nCacheLineSize;
				}
			}

			if (m_bReadOnly || bReadOnly)
			{
				setMode (VIEW_RCOPIED);
			}
			else
			{
				setMode (VIEW_RWCOPIED);
			}

			setCacheCfg (nBestCacheLineBits, nBestCacheLineSize);
		}
	}

	return (bRetval);
}

bool CFileMem::setMode (CFileMem::view_e eMode)
{
	bool	bRtrn = true;

	if (eMode == m_eMode)
	{
		return (true);
	}

#if defined (_HAS_MFC)
	if (m_pFile != NULL)
#elif defined (WIN32)
	if (m_hFile != INVALID_HANDLE_VALUE)
#elif defined (LINUX)
	if (m_filedes != -1)
#endif
	{
		exit (true, true, true);
	}

	if ((eMode == VIEW_RWCOPIED) && (m_bReadOnly))
	{
		bRtrn = false;

		eMode = VIEW_RCOPIED;
	} // if ((mode == VIEW_RWCOPIED) && (m_bReadOnly))

	if ((eMode == VIEW_RWLOAD) && (m_bReadOnly))
	{
		bRtrn = false;

		eMode = VIEW_RLOAD;
	} // if ((mode == VIEW_RWCOPIED) && (m_bReadOnly))

	m_eMode = eMode;

#if defined (_HAS_MFC)
	if (m_pFile != NULL)
#elif defined (WIN32)
	if (m_hFile != INVALID_HANDLE_VALUE)
#elif defined (LINUX)
	if (m_filedes != -1)
#endif
	{
		init (true, false);
	}

	return (bRtrn);
}

CFileMem::view_e CFileMem::getMode ()
{
	return (m_eMode);
}

CFileMem::cache_e CFileMem::getIsCached (uint64_t nOffset, uint64_t nLen)
{
	cache_e				eRetval = CFileMem::CACHE_NOT_FOUND;
	uint64_t			ui64, nHits, nMiss;
	uint64_t			nPage;
	uint64_t			nLine;
	uint64_t			nStep;

	switch (m_eMode)
	{
	case VIEW_RLOAD		:	
	case VIEW_RWLOAD	:	eRetval = CFileMem::CACHE_FOUND;

							break;

	default				:	nHits = 0ULL;

							nMiss = 0ULL;

							nPage = nOffset / get_cacheLineSize ();
							nLine = nPage / get_numCacheLines ();
							
							for (ui64 = nOffset; ui64 < (nOffset + nLen); nPage++, nLine++, ui64 += nStep)
							{
								if ((m_psCache[nLine].uState & FILECACHE_STATE_VALID) && m_psCache[nLine].nPage == nPage)
								{
									nHits++;
								}
								else
								{
									nMiss++;
								}

								using namespace std;

								nStep = min ((nOffset + nLen) - ui64, get_cacheLineSize () - (ui64 % get_cacheLineSize ()));
							}

							if ((nHits != 0ULL) && (nMiss != 0ULL))
							{
								eRetval = CFileMem::CACHE_HALF;
							}
							else if (nMiss == 0ULL)
							{
								eRetval = CFileMem::CACHE_FOUND;
							}

							break;
	}

	return (eRetval);
}

uint64_t CFileMem::getLength ()
{
	return (m_nLength);
}

bool CFileMem::setLength (uint64_t nLen)
{
#if defined (_DEBUG)
	FILEMEM_ASSERT (m_bReadOnly == false, "CFileMem::setLength: file size can only be changed when m_eMode is set to VIEW_RWCOPIED");
#endif

	switch (m_eMode)
	{
	case VIEW_FILEMAPPED	:	
	case VIEW_RWCOPIED		:	
	case VIEW_RWLOAD		:	if (nLen < getLength ())
								{
									exit (false, true, true);
								}

								break;

	case VIEW_RCOPIED		:	
	case VIEW_RLOAD			:	

#if defined (_DEBUG)
								FILEMEM_ASSERT (false, "CFileMem::setLength: m_eMode contradicted m_bReadOnly during read cache invalidation <- this shouldn't happen");
#endif
								break;

	default					:	FILEMEM_ASSERT (false, "CFileMem::setLength: unknown mode selected during read cache invalidation");

								break;
	}

#if defined (_HAS_MFC)
	
	try
	{
		m_pFile->SetLength (nLen);
	}
	catch (CMemoryException *pCME)
	{
		pCME;

		FILEMEM_ASSERT (false, "CFileMem::setLength: SetLength has failed!");
	}

#elif defined (WIN32)

	LARGE_INTEGER		sNewLen;
	BOOL				bRslt;

	sNewLen.QuadPart = nLen;

	bRslt = SetFilePointerEx (m_hFile, sNewLen, NULL, FILE_BEGIN);

	FILEMEM_ASSERT (bRslt != FALSE, "CFileMem::setLength: SetFilePointerEx has failed!");

	bRslt = SetEndOfFile (m_hFile);

	FILEMEM_ASSERT (bRslt != FALSE, "CFileMem::setLength: SetEndOfFile has failed!");

#elif defined (LINUX)

	int		nRslt;
	
	nRslt = ftruncate64 (m_filedes, nLen);

	FILEMEM_ASSERT (nRslt == 0, "CFileMem::setLength: ftruncate64 has failed!");

#endif

	m_nLength = nLen;

	switch (m_eMode)
	{
	case VIEW_FILEMAPPED	:	
	case VIEW_RWCOPIED		:	
	case VIEW_RWLOAD		:	init (true, true);

								break;

	case VIEW_RCOPIED		:	
	case VIEW_RLOAD			:	

#if defined (_DEBUG)
								FILEMEM_ASSERT (false, "CFileMem::setLength: m_eMode contradicts m_bReadOnly during read cache re-initialization <- this shouldn't happen");
#endif

								break;

	default					:	FILEMEM_ASSERT (false, "CFileMem::setLength: unknown mode selected during read cache re-initialization");

								break;
	}

#if defined (_HAS_MFC)
	return (m_pFile->GetLength () == nLen);
#elif defined (WIN32)
	{
		LARGE_INTEGER		ui64len;

		FILEMEM_ASSERT (GetFileSizeEx (m_hFile, &ui64len) != false, "CFileMem::setLength: Cannot determine file size!");

		return (ui64len.QuadPart == nLen);
	}
#elif defined (LINUX)
	uint64_t		n64curPos = lseek64 (m_filedes, 0, SEEK_CUR);

	nLen -= lseek64 (m_filedes, 0, SEEK_END);

	lseek64 (m_filedes, n64curPos, SEEK_SET);

	return (nLen == 0ULL);
#endif
}

bool CFileMem::setCacheCfg (uint32_t nLineBits, uint64_t nDesiredCacheLineSize)
{
	uint64_t				nCacheLineSize;
	bool					bRtrn;

	if (m_eMode == VIEW_FILEMAPPED)
	{
		nCacheLineSize = (nDesiredCacheLineSize + m_nCacheLineGranularity - 1ULL) / m_nCacheLineGranularity;
		nCacheLineSize *= m_nCacheLineGranularity;
	}
	else
	{
		nCacheLineSize = nDesiredCacheLineSize;

#if defined (_HAS_MFC)
#elif defined (WIN32)
		if (nCacheLineSize % 512)
		{
			nCacheLineSize >>= 9;
			nCacheLineSize++;
			nCacheLineSize <<= 9;
		}
#elif defined (LINUX)
#endif
	}

	bRtrn = nDesiredCacheLineSize == nCacheLineSize;

#if defined (_HAS_MFC)
	if (m_pFile != NULL)
#elif defined (WIN32)
	if (m_hFile != INVALID_HANDLE_VALUE)
#elif defined (LINUX)
	if (m_filedes != -1)
#endif
	{
		exit (true, true, true);
	}

	m_nCacheLineBits = nLineBits;
	m_uCacheLineMask = (1ULL << m_nCacheLineBits) - 1ULL;
	m_nCacheLineSize = nCacheLineSize;

	m_bCacheLineSizeBinary = false;
	m_uCacheLineSizeMask = 0x0ULL;
	m_nCacheLineSizeBits = 0x0UL;	

	while (nCacheLineSize != 0ULL)
	{
		if ((nCacheLineSize & 0x1ULL) == 0x1ULL)
		{
			if ((1ULL << m_nCacheLineSizeBits) == m_nCacheLineSize)
			{
				m_bCacheLineSizeBinary = true;
				m_uCacheLineSizeMask = m_nCacheLineSize - 1ULL;

				break;
			}
		}

		m_nCacheLineSizeBits++;

		nCacheLineSize >>= 1ULL;
	}

#if defined (_HAS_MFC)
	if (m_pFile != NULL)
#elif defined (WIN32)
	if (m_hFile != INVALID_HANDLE_VALUE)
#elif defined (LINUX)
	if (m_filedes != -1)
#endif
	{
		bRtrn = init (true, false);
	}
	else
	{
		bRtrn = false;
	}

	return (bRtrn);
}

void CFileMem::getCacheCfg (uint32_t *pnLineBits, uint64_t *pnCacheLineSize)
{
	if (pnLineBits != NULL)
	{
		*pnLineBits = this->m_nCacheLineBits;
	}

	if (pnCacheLineSize != NULL)
	{
		*pnCacheLineSize = this->m_nCacheLineSize;
	}
}

void CFileMem::getPerfCtrs (uint64_t &rHitCtr, uint64_t &rMissCtr)
{
#if defined (USE_PERFORMANCE_COUNTERS)
	rHitCtr = m_nHitCtr;
	rMissCtr = m_nMissCtr;
#else
	rHitCtr = 0;
	rMissCtr = 0;
#endif
}

void CFileMem::memcpy (uint64_t nDest, void *pSrc, uint64_t nLen)
{
#if defined (_HAS_MFC)
	if (m_pFile == NULL)
#elif defined (WIN32)
	if (m_hFile == INVALID_HANDLE_VALUE)
#elif defined (LINUX)
	if (m_filedes == -1)
#endif
	{
		return;
	}

	unsigned char		*pcSrc = (unsigned char *)pSrc;
	uint64_t			nPage = nDest / get_cacheLineSize ();
	uint64_t			nOffset;
	uint64_t			nLine;
	uint64_t			ui64, nChunk;

#if defined (_DEBUG)
	FILEMEM_ASSERT ((nDest + nLen) <= m_nLength, "CFileMem::memcpy (uint64_t, void *, uint64_t): subscript exceeds file size");
#endif

	switch (m_eMode)
	{
	case VIEW_FILEMAPPED:	
	case VIEW_RWCOPIED:		
	case VIEW_RCOPIED:		for (ui64 = 0ULL; ui64 < nLen; nPage++)
							{
								nOffset = (nDest + ui64) % get_cacheLineSize ();
								nLine = nPage & get_cacheLineMask ();

								loadPage (nPage);

#if defined (_DEBUG)
								FILEMEM_ASSERT (m_psCache[nLine].uState & FILECACHE_STATE_VALID ? true : false, "CFileMem::memcpy (uint64_t, void *, uint64_t): invalid line");
								FILEMEM_ASSERT (m_psCache[nLine].nPage == nPage, "CFileMem::memcpy (uint64_t, void *, uint64_t): invalid page");
#endif

								using namespace std;
								
								nChunk = min (get_cacheLineSize () - nOffset, nLen - ui64);

								::memcpy (
											(void *) &(((unsigned char *)m_psCache[nLine].pMappedView)[nOffset]), 
											(void *) &(pcSrc[ui64]), 
											(size_t) nChunk);

								ui64 += nChunk;
							}

							break;

	case VIEW_RWLOAD:		
	case VIEW_RLOAD:		init (true, true);
							
							::memcpy ((void *) &(((unsigned char *)m_psCache[0].pMappedView)[nDest]), pSrc, (size_t)nLen);

							break;

	default:				FILEMEM_ASSERT (false, "CFileMem::memcpy (uint64_t, void *, uint64_t): unknown mode");		// pretty insecure

							break;
	} // switch (m_eMode)
}

void CFileMem::memcpy (void *pDest, uint64_t nSrc, uint64_t nLen)
{
#if defined (_HAS_MFC)
	if (m_pFile == NULL)
#elif defined (WIN32)
	if (m_hFile == INVALID_HANDLE_VALUE)
#elif defined (LINUX)
	if (m_filedes == -1)
#endif
	{
		return;
	}

	if (m_bCacheLineSizeBinary)
	{
#if defined (WIN32)
		__declspec(align(16)) uint64_t			arr[2];
		__declspec(align(8)) uint64_t			nChunk;
#elif defined (LINUX)
		uint64_t								arr[2];
		uint64_t								nChunk;
#endif

		unsigned char							*pcDest = (unsigned char *)pDest;
		uint64_t								nPage = nSrc >> m_nCacheLineSizeBits;
		uint64_t								&nOffs = arr[0], &ui64 = arr[1];
		uint64_t								nLine;

#if defined (_DEBUG)
		FILEMEM_ASSERT ((nSrc + nLen) <= m_nLength, "CFileMem::memcpy (void *, uint64_t, uint64_t): subscript exceeds file size");
#endif

		switch (m_eMode)
		{
		case VIEW_FILEMAPPED:	
		case VIEW_RWCOPIED:		
		case VIEW_RCOPIED:		nOffs = nSrc & m_uCacheLineSizeMask;

								for (ui64 = 0ULL; nLen > 0ULL; nPage++)
								{
									nLine = nPage & get_cacheLineMask ();

									loadPage (nPage);

#if defined (_DEBUG)
									FILEMEM_ASSERT (m_psCache[nLine].uState & FILECACHE_STATE_VALID ? true : false, "CFileMem::memcpy (void *, uint64_t, uint64_t): invalid line");
									FILEMEM_ASSERT (m_psCache[nLine].nPage == nPage, "CFileMem::memcpy (void *, uint64_t, uint64_t): invalid page");
#endif

									nChunk = get_cacheLineSize () - nOffs;

									if (nLen < nChunk)
									{
										nChunk = nLen;
									}

									::memcpy (
												(void *) &(pcDest[ui64]), 
												(void *) &(((unsigned char *)m_psCache[nLine].pMappedView)[nOffs]), 
												(size_t) nChunk);

									ui64 += nChunk;
									nOffs += nChunk;
									nLen -= nChunk;

									nOffs &= m_uCacheLineSizeMask;
								}

								break;

		case VIEW_RWLOAD:		
		case VIEW_RLOAD:		init (true, true);
			
								::memcpy (pDest, (void *) &(((unsigned char *)m_psCache[0].pMappedView)[nSrc]), (size_t)nLen);

								break;

		default:				FILEMEM_ASSERT (false, "CFileMem::memcpy (void *, uint64_t, uint64_t): unknown mode");

								break;
		} // switch (m_eMode)
	}
	else
	{
#if defined (WIN32)
		__declspec(align(16)) uint64_t	arr[2];
		__declspec(align(8)) uint64_t	nChunk;
#elif defined (LINUX)
		uint64_t						arr[2];
		uint64_t						nChunk;
#endif
		uint8_t							*pcDest = (uint8_t *)pDest;
		uint64_t						nPage = nSrc / get_cacheLineSize ();
		uint64_t						&nOffs = arr[0], &ui64 = arr[1];
		uint64_t						nLine;

#if defined (_DEBUG)
		FILEMEM_ASSERT ((nSrc + nLen) <= m_nLength, "CFileMem::memcpy (void *, uint64_t, uint64_t): subscript exceeds file size");
#endif

		switch (m_eMode)
		{
		case VIEW_FILEMAPPED:	
		case VIEW_RWCOPIED:		
		case VIEW_RCOPIED:		nOffs = nSrc % get_cacheLineSize ();

								for (ui64 = 0ULL; nLen > 0ULL; nPage++)
								{
									nLine = nPage & get_cacheLineMask ();

									loadPage (nPage);

#if defined (_DEBUG)
									FILEMEM_ASSERT (m_psCache[nLine].uState & FILECACHE_STATE_VALID ? true : false, "CFileMem::memcpy (void *, uint64_t, uint64_t): invalid line");
									FILEMEM_ASSERT (m_psCache[nLine].nPage == nPage, "CFileMem::memcpy (void *, uint64_t, uint64_t): invalid page");
#endif

									nChunk = get_cacheLineSize () - nOffs;

									if (nLen < nChunk)
									{
										nChunk = nLen;
									}

									::memcpy (
												(void *) &(pcDest[ui64]), 
												(void *) &(((unsigned char *)m_psCache[nLine].pMappedView)[nOffs]), 
												(size_t) nChunk);

									ui64 += nChunk;
									nOffs += nChunk;
									nLen -= nChunk;

									if (nOffs >= get_cacheLineSize ())
									{
										nOffs -= get_cacheLineSize ();
									}
								}
								break;

		case VIEW_RWLOAD:		
		case VIEW_RLOAD:		init (true, true);
			
								::memcpy (pDest, (void *) &(((unsigned char *)m_psCache[0].pMappedView)[nSrc]), (size_t)nLen);

								break;

		default:				FILEMEM_ASSERT (false, "CFileMem::memcpy (void *, uint64_t, uint64_t): unknown mode");

								break;
		} // switch (m_eMode)
	}
}

void CFileMem::memcpy (uint64_t nDest, CFileMem *pSrcFile, uint64_t nSrc, uint64_t nLen)
{
#if defined (WIN32)
	__declspec(align(16)) uint64_t			arrpage[2];
	__declspec(align(16)) uint64_t			arrline[2];
	__declspec(align(16)) uint64_t			arrpos[2];
	__declspec(align(16)) uint64_t			arrfrag[2];
	__declspec(align(16)) uint64_t			&spage = arrpage[0], &dpage = arrpage[1];
	__declspec(align(16)) uint64_t			&sline = arrline[0], &dline = arrline[1];
	__declspec(align(16)) uint64_t			&spos = arrpos[0], &dpos = arrpos[1];
	__declspec(align(16)) uint64_t			&sfrag = arrfrag[0], &dfrag = arrfrag[1];
#elif defined (LINUX)
	uint64_t								arrpage[2];
	uint64_t								arrline[2];
	uint64_t								arrpos[2];
	uint64_t								arrfrag[2];
	uint64_t								&spage = arrpage[0], &dpage = arrpage[1];
	uint64_t								&sline = arrline[0], &dline = arrline[1];
	uint64_t								&spos = arrpos[0], &dpos = arrpos[1];
	uint64_t								&sfrag = arrfrag[0], &dfrag = arrfrag[1];
#endif
	bool									conflict, srcFragmented = true, dstFragmented = true;
	uint64_t								fragmentSize, ui64;
	unsigned char							buf[8192];
	void									*pSrcBuffer, *pDestBuffer;

#if defined (CF_SSE2) && defined (WIN32)
	__declspec(align(16)) const uint64_t	arrone[2] = {1ULL, 1ULL};
	__declspec(align(16)) const uint64_t	arrset[2] = {~0x0ULL, ~0x0ULL};
	__declspec(align(16)) uint64_t			arrcacheLineSize[2];

	__asm
	{
		movdqa		xmm6, arrset				// load all bits set
		movdqa		xmm7, arrone				// load ones
	}
#endif

	switch (pSrcFile->m_eMode)
	{
	case VIEW_FILEMAPPED:	
	case VIEW_RWCOPIED:		
	case VIEW_RCOPIED:		srcFragmented = true;
							break;
	case VIEW_RWLOAD:
	case VIEW_RLOAD:		pSrcFile->init (true, true);
		
							srcFragmented = false;
							break;
	default:				FILEMEM_ASSERT (false, "CFileMem::memcpy (uint64_t, CFileMem *, uint64_t, uint64_t): unknown mode selected during source file cache re-initialization");
							break;
	}

	switch (m_eMode)
	{
	case VIEW_FILEMAPPED:	
	case VIEW_RWCOPIED:		dstFragmented = true;
							break;
	case VIEW_RWLOAD:		init (true, true);
		
							dstFragmented = false;
							break;
	case VIEW_RLOAD:		
	case VIEW_RCOPIED:		FILEMEM_ASSERT (false, "CFileMem::memcpy (uint64_t, CFileMem *, uint64_t, uint64_t): CFileMem is write protected");
							break;
	default:				FILEMEM_ASSERT (false, "CFileMem::memcpy (uint64_t, CFileMem *, uint64_t, uint64_t): unknown mode selected during destination file cache re-initialization");
							break;
	} // switch (m_eMode)

	dpage = nDest / get_cacheLineSize ();
	spage = nSrc / pSrcFile->get_cacheLineSize ();

	dfrag = nDest % get_cacheLineSize ();
	sfrag = nSrc % pSrcFile->get_cacheLineSize ();

#if defined (CF_SSE2) && defined (WIN32)
	arrcacheLineSize[0] = get_cacheLineSize ();
	arrcacheLineSize[1] = pSrcFile->get_cacheLineSize ();

	__asm
	{
		// dpos = nDest;
		// spos = nSrc;
		movq		xmm0, nDest
		pslldq		xmm0, 8
		movq		xmm2, nSrc
		por			xmm0, xmm2
		movdqa		arrpos, xmm0
	}
#else
	dpos = nDest;
	spos = nSrc;
#endif

	for (ui64 = 0ULL; ui64 < nLen; )
	{
#if defined (CF_SSE2) && defined (WIN32)
		__asm
		{
			movdqa		xmm0, arrfrag				// load frag
			movdqa		xmm1, xmm0					// copy frag
			movdqa		xmm2, arrcacheLineSize		// load cache line sizes
			paddq		xmm1, xmm7					// increment fragments by one
			movdqa		xmm3, xmm2					// copy cache line sizes
			psubq		xmm3, xmm1					// subtract incremented fragments from cache line sizes
			psrlq		xmm3, 63					// right shift result of subtraction by 63 bits
			psubq		xmm3, xmm7					// mask all fragments smaller than cache line size
			movdqa		xmm5, arrpage				// load page numbers
			pxor		xmm3, xmm6					// invert masks
			movdqa		xmm4, xmm7					// copy ones vector
			pand		xmm2, xmm3					// mask all cache line sizes that will be subtracted from fragments
			pand		xmm4, xmm3					// mask ones vector
			psubq		xmm0, xmm2					// subtract cache line sizes from fragments
			movdqa		arrfrag, xmm0				// store fragments
			paddq		xmm5, xmm4					// increment page numbers
			movdqa		arrpage, xmm5				// store page numbers
		}
#else
		if (dfrag >= get_cacheLineSize ())
		{
			dpage++;

			dfrag -= get_cacheLineSize ();
		}

		if (sfrag >= pSrcFile->get_cacheLineSize ())
		{
			spage++;

			sfrag -= pSrcFile->get_cacheLineSize ();
		}
#endif

		dline = dpage & get_cacheLineMask ();
		sline = spage & pSrcFile->get_cacheLineMask ();

		conflict = ((pSrcFile == this) && (dline == sline));

		if (dstFragmented)
		{
			using namespace std;

			//fragmentSize = std::min (nLen - ui64, (dpos - (dpos % get_cacheLineSize ()) + get_cacheLineSize ()) - dpos);
			fragmentSize = min (nLen - ui64, get_cacheLineSize () - dfrag);
		}
		else
		{
			fragmentSize = nLen - ui64;
		}

		if (srcFragmented)
		{
			using namespace std;

			//fragmentSize = std::min (fragmentSize, (spos - (spos % pSrcFile->get_cacheLineSize ()) + pSrcFile->get_cacheLineSize ()) - spos);
			fragmentSize = min (fragmentSize, pSrcFile->get_cacheLineSize () - sfrag);
		}

		if (conflict)
		{
			using namespace std;

			fragmentSize = min (fragmentSize, (uint64_t) (sizeof (buf) / sizeof (*buf)));

			pSrcFile->memcpy ((void *) buf, spos, fragmentSize);
			memcpy (dpos, (void *) buf, fragmentSize);
		}
		else
		{
			if (srcFragmented)
			{
				pSrcFile->loadPage (spage);

				pSrcBuffer = (void *) &(((unsigned char *)pSrcFile->m_psCache[sline].pMappedView)[sfrag]);
			}
			else
			{
				pSrcBuffer = (void *) &(((unsigned char *)pSrcFile->m_psCache[0].pMappedView)[spos]);
			}

			if (dstFragmented)
			{
				loadPage (dpage);

				pDestBuffer = (void *) &(((unsigned char *)m_psCache[dline].pMappedView)[dfrag]);
			}
			else
			{
				pDestBuffer = (void *) &(((unsigned char *)m_psCache[0].pMappedView)[dpos]);
			}

			::memcpy	(
							pDestBuffer, 
							pSrcBuffer, 
							(size_t) fragmentSize
						);
		}

#if defined (CF_SSE2) && defined (WIN32)
		__asm
		{
			movq		xmm5, fragmentSize
			movq		xmm0, ui64
			paddq       xmm0, xmm5
			movdqa		xmm1, arrfrag
			punpcklqdq	xmm5, xmm5
			movq		ui64, xmm0
			paddq       xmm1, xmm5
			movdqa		xmm2, arrpos
			paddq       xmm2, xmm5
			movdqa		arrfrag, xmm1
			movdqa		arrpos, xmm2
		}
#else
		ui64 += fragmentSize;
		dfrag += fragmentSize;
		sfrag += fragmentSize;
		dpos += fragmentSize;
		spos += fragmentSize;
#endif
	}

#if defined (CF_SSE2) && defined (WIN32)
	__asm
	{
		emms
	}
#endif
}

#if defined (_HAS_MFC)
void CFileMem::memcpy (uint64_t nDest, CFile *pSrcFile, uint64_t nPos, uint64_t nLen)
{
#define	BUFSIZE			(8192ULL)

	if (m_pFile == NULL)
	{
		return;
	}

	uint64_t	ui64, nBlockSize;
	uint8_t		buf[BUFSIZE];

	seek (nPos, pSrcFile);

	while (nLen != 0ULL)
	{
		nBlockSize = nLen;

		if (nBlockSize > BUFSIZE)
		{
			nBlockSize = BUFSIZE;
		}

		pSrcFile->Read ((void *) buf, (unsigned int) nBlockSize);

		for (ui64 = 0ULL; ui64 < nBlockSize; ui64++)
		{
			(*this)[nDest + ui64] = buf[ui64];
		}

		nLen -= nBlockSize;
	} // while (nLen != 0ULL)
}
#endif

bool CFileMem::memcmp (uint64_t nD0, void *pD1, uint64_t nLen)
{
#if defined (_HAS_MFC)
	if (m_pFile == NULL)
#elif defined (WIN32)
	if (m_hFile == INVALID_HANDLE_VALUE)
#elif defined (LINUX)
	if (m_filedes == -1)
#endif
	{
		return (false);
	}

	uint8_t				*pcD1 = (uint8_t *)pD1;
	uint64_t			nFragmentSize;
	uint64_t			ui64;
	uint64_t			nDpos;
	uint64_t			nDpage;
	uint64_t			nDline;
	bool				bDstFragmented;
	void				*pDestBuffer;

	switch (m_eMode)
	{
	case VIEW_FILEMAPPED:	
	case VIEW_RCOPIED:		
	case VIEW_RWCOPIED:		bDstFragmented = true;

							break;

	case VIEW_RWLOAD:		
	case VIEW_RLOAD:		bDstFragmented = false;

							break;

	default:				FILEMEM_ASSERT (false, "CFileMem::memcmp (uint64_t, void *, uint64_t): unknown mode");

							break;
	} // switch (m_eMode)

	seek (nD0);

	for (ui64 = 0ULL; ui64 < nLen; ui64 += nFragmentSize)
	{
		nDpos = nD0 + ui64;

		nDpage = nDpos / get_cacheLineSize ();
		
		nDline = nDpage & get_cacheLineMask ();

		if (bDstFragmented)
		{
			using namespace std;

			nFragmentSize = min (nLen - ui64, (nDpos - (nDpos % get_cacheLineSize ()) + get_cacheLineSize ()) - nDpos);

			nDpos %= get_cacheLineSize ();

			loadPage (nDpage);

			pDestBuffer = (void *) &(((uint8_t *)m_psCache[nDline].pMappedView)[nDpos]);
		}
		else
		{
			nFragmentSize = nLen - ui64;

			pDestBuffer = (void *) &(((unsigned char *)m_psCache[0].pMappedView)[nDpos]);
		}

		if (::memcmp (pDestBuffer, (void *) pcD1, (size_t) nFragmentSize) != 0)
		{
			return (false);
		}

		pcD1 += nFragmentSize;
	}

	return (true);
}

bool CFileMem::memcmp (uint64_t nThisPos, CFileMem *pFDcmp, uint64_t nCmpPos, uint64_t nLen)
{
#define	BUFSIZE		(8192ULL)

#if defined (_HAS_MFC)
	if (m_pFile == NULL)
#elif defined (WIN32)
	if (m_hFile == INVALID_HANDLE_VALUE)
#elif defined (LINUX)
	if (m_filedes == -1)
#endif
	{
		return (false);
	}

	uint64_t		nRemain;
	uint8_t			buf[BUFSIZE];
	bool			bDstFragmented;
	int				triRslt;

	switch (m_eMode)
	{
	case VIEW_FILEMAPPED:	
	case VIEW_RCOPIED:		
	case VIEW_RWCOPIED:		bDstFragmented = true;

							break;

	case VIEW_RWLOAD:		
	case VIEW_RLOAD:		bDstFragmented = false;

							if (m_psCache == NULL)
							{
								init (true, true);
							}

							break;

	default:				FILEMEM_ASSERT (false, "CFileMem::memcmp (uint64_t, CFileMem *, uint64_t, uint64_t): unknown mode");

							break;
	} // switch (m_eMode)

	for (nRemain = nLen; nRemain > 0; )
	{
		using namespace std;

		uint64_t	nChunk = BUFSIZE;
		uint64_t	nPage;
		uint64_t	nOffset;
		uint64_t	nLine;

		if (m_bCacheLineSizeBinary)
		{
			nPage = nThisPos >> m_nCacheLineSizeBits;
			nOffset = nThisPos & m_uCacheLineSizeMask;
		}
		else
		{
			nPage = nThisPos / get_cacheLineSize ();
			nOffset = nThisPos % get_cacheLineSize ();
		}

		nChunk = min (nChunk, nRemain);

		pFDcmp->memcpy ((void *) buf, nCmpPos, nChunk);

		if (bDstFragmented)
		{
			nChunk = min (nChunk, get_cacheLineSize () - nOffset);

			loadPage (nPage);

			nLine = nPage & get_cacheLineMask ();

			triRslt = ::memcmp ((void *) buf, (void *) &(((uint8_t *)m_psCache[nLine].pMappedView)[nOffset]), (size_t) nChunk);
		}
		else
		{
			triRslt = ::memcmp ((void *) buf, (void *) &(((unsigned char *)m_psCache[0].pMappedView)[nThisPos]), (size_t) nChunk);
		}

		if (triRslt != 0)
		{
			return (false);
		}

		nThisPos += nChunk;
		nCmpPos += nChunk;
		nRemain -= nChunk;
	}

	return (true);
}

#if defined (_HAS_MFC)
bool CFileMem::memcmp (uint64_t nD0, CFile *pFD0, uint64_t nPos, uint64_t nLen)
{
#define	BUFSIZE		(8192ULL)

	if (m_pFile == NULL)
	{
		return (false);
	}

	uint8_t				buf0[BUFSIZE], buf1[BUFSIZE];
	uint64_t			nBlocksize;

	while (nLen != 0ULL)
	{
		nBlocksize = nLen;

		if (nBlocksize > BUFSIZE)
		{
			nBlocksize = BUFSIZE;
		}

		seek (nD0);
		m_pFile->Read ((void *) buf0, (uint32_t) nBlocksize);

		seek (nPos, pFD0);
		pFD0->Read ((void *) buf1, (uint32_t) nBlocksize);

		if (::memcmp ((void *) buf0, (void *) buf1, (size_t) nBlocksize) != 0)
		{
			return (false);
		}

		nD0 += nBlocksize;
		nPos += nBlocksize;

		nLen -= nBlocksize;
	} // while (nLen != 0ULL)

	return (true);
}
#endif

void CFileMem::memmove (uint64_t nDest, CFileMem *pSrcFile, uint64_t nPos, uint64_t nLen)
{
#if defined (_HAS_MFC)
	if (m_pFile == NULL)
#elif defined (WIN32)
	if (m_hFile == INVALID_HANDLE_VALUE)
#elif defined (LINUX)
	if (m_filedes == -1)
#endif
	{
		return;
	}

	if (this != pSrcFile)
	{
		memcpy (nDest, pSrcFile, nPos, nLen);

		return;
	}

	if (nDest == nPos)
	{
		return;
	}

	if (((nPos > nDest) && (nPos < (nDest + nLen))) || ((nDest > nPos) && (nDest < (nPos + nLen))))
	{
		if (nDest < nPos)
		{
			memcpy (nDest, pSrcFile, nPos, nLen);
		}
		else
		{
			uint64_t	nBlocksize, nDiff, ui64;

			for (nDiff = nPos > nDest ? nPos - nDest : nDest - nPos, ui64 = 0ULL; ui64 < nLen; ui64 += nBlocksize)
			{
				using namespace std;

				nBlocksize = min (nDiff, nLen - ui64);

				memcpy (nDest + nLen - ui64 - nBlocksize, pSrcFile, nPos + nLen - ui64 - nBlocksize, nBlocksize);
			}
		}
	}
	else
	{
		memcpy (nDest, pSrcFile, nPos, nLen);
	}
}

#if defined (_HAS_MFC)
void CFileMem::memmove (uint64_t nDest, CFile *pSrcFile, uint64_t nPos, uint64_t nLen)
{
	if (m_pFile == this->m_pFile)
	{
		memmove (nDest, this, nPos, nLen);

		flush ();
	}
	else
	{
		memcpy (nDest, pSrcFile, nPos, nLen);
	}
}
#endif

void CFileMem::memset (uint64_t nPos, unsigned char nVal, uint64_t nLen)
{
#if defined (_HAS_MFC)
	if (m_pFile == NULL)
#elif defined (WIN32)
	if (m_hFile == INVALID_HANDLE_VALUE)
#elif defined (LINUX)
	if (m_filedes == -1)
#endif
	{
		return;
	}

	uint64_t		ui64;

	for (ui64 = 0ULL; ui64 < nLen; ui64++)
	{
		(*this)[nPos + ui64] = nVal;
	}
}

unsigned char& CFileMem::operator[] (uint64_t nSubscript)
{
	uint64_t	nPage;
	uint64_t	nOffset;
	uint64_t	nLine;

#if defined (_DEBUG)
	FILEMEM_ASSERT (nSubscript < m_nLength, "CFileMem::operator[]: subscript exceeds file size");
#endif

	switch (m_eMode)
	{
	case VIEW_FILEMAPPED:	
	case VIEW_RWCOPIED:		
	case VIEW_RCOPIED:		if (m_bCacheLineSizeBinary)
							{
								nPage = nSubscript >> m_nCacheLineSizeBits;
								nOffset = nSubscript & m_uCacheLineSizeMask;
							}
							else
							{
								if ((nSubscript >= m_nFromSubscript) && (nSubscript < m_nToSubscript))
								{
									nPage = m_nPageSubscript;
									nOffset = nSubscript - m_nFromSubscript;
								}
								else
								{
									nPage = nSubscript / get_cacheLineSize ();
									nOffset = nSubscript % get_cacheLineSize ();

									m_nFromSubscript = nPage * get_cacheLineSize ();
									m_nToSubscript = m_nFromSubscript + get_cacheLineSize ();

									m_nPageSubscript = nPage;
								}
							}

							nLine = nPage & get_cacheLineMask ();

							loadPage (nPage);

							return (((unsigned char *)m_psCache[nLine].pMappedView)[nOffset]);

							break;

	case VIEW_RWLOAD:		
	case VIEW_RLOAD:		init (true, true);

							return (((unsigned char *)m_psCache[0].pMappedView)[nSubscript]);

							break;

	default:				FILEMEM_ASSERT (false, "CFileMem::operator[]: unknown mode");

							return (((unsigned char *)m_psCache[0].pMappedView)[0]);

							break;
	} // switch (m_eMode)
}

bool CFileMem::flushLine (uint64_t nLine)
{
	if (!m_bReadOnly)
	{
#if defined (_DEBUG)
		FILEMEM_ASSERT (nLine < get_numCacheLines (), "CFileMem::flushLine: cache line number exceeds number of cache lines");
#endif
		
		if (m_psCache != NULL)
		{
			if (m_psCache[nLine].uState & FILECACHE_STATE_VALID)
			{
				switch (m_eMode)
				{
#if defined (WIN32)
				case VIEW_FILEMAPPED:	return (FlushViewOfFile (m_psCache[nLine].pMappedView, (size_t) get_cacheLineSize ()) == TRUE ? true : false);
#elif defined (LINUX)
				case VIEW_FILEMAPPED:	return (0 == msync (m_psCache[nLine].pMappedView, (size_t) get_cacheLineSize (), m_uOpenFlags & osNoBuffer ? MS_SYNC : MS_ASYNC));
#endif
										break;
				case VIEW_RWCOPIED:		{
											bool	bWriteBack = false;
											
											if ((size_t) get_cacheLineSize () == sizeof (uint32_t))
											{
												uint32_t	d, m;

												d = *((uint32_t *) (m_psCache[nLine].pMappedView));
												m = *((uint32_t *) (m_psCache[nLine].pMirrorView));

												bWriteBack = d != m;
											}
											else
											{
												bWriteBack = (::memcmp (m_psCache[nLine].pMappedView, m_psCache[nLine].pMirrorView, (size_t) get_cacheLineSize ()) != 0) ? true : false;
											}

											if (bWriteBack)
											{
												seek (m_psCache[nLine].nPage * get_cacheLineSize ());

#if defined (_HAS_MFC)
												m_pFile->Write (m_psCache[nLine].pMappedView, (UINT)get_cacheLineSize ());
#elif defined (WIN32)
												{
													DWORD		dwLen;

													WriteFile (m_hFile, m_psCache[nLine].pMappedView, (DWORD)get_cacheLineSize (), &dwLen, NULL);

													FILEMEM_ASSERT (dwLen == (DWORD)get_cacheLineSize (), "CFileMem::flushLine: write error");
												}
#elif defined (LINUX)
												write (m_filedes, m_psCache[nLine].pMappedView, (size_t) get_cacheLineSize ());
#endif
											}
										}
				case VIEW_RCOPIED:		
				case VIEW_RWLOAD:		
				case VIEW_RLOAD:		return (true);
										break;

				default:				FILEMEM_ASSERT (false, "CFileMem::flushLine: unknown mode");

										break;
				} // switch (m_eMode)
			}
		}
	} // if (!m_bReadOnly)

	return (false);
}

bool CFileMem::loadPage (uint64_t nPage)
{
	uint64_t	nLine = nPage & get_cacheLineMask ();
	uint64_t	nFileOffset;

#if defined (WIN32)
	DWORD				nHighOffset, nLowOffset;
#elif defined (LINUX)
	int		prot;
	int		nFlags;
#endif

	size_t				nLineLen;

	if (m_psCache == NULL)
	{
		init (true, false);
	}

	if (m_psCache[nLine].uState & FILECACHE_STATE_VALID)
	{
		if (m_psCache[nLine].nPage == nPage)
		{
#if defined (USE_PERFORMANCE_COUNTERS)
			m_nHitCtr++;
#endif

			return (true);
		}

#if defined (USE_PERFORMANCE_COUNTERS)
			m_nMissCtr++;
#endif

		flushLine (nLine);

		if (m_eMode == VIEW_FILEMAPPED)
		{
#if defined (WIN32)
			UnmapViewOfFile (m_psCache[nLine].pMappedView);
#elif defined (LINUX)
			munmap (m_psCache[nLine].pMappedView, (size_t) get_cacheLineSize ());
#endif
		}

		m_psCache[nLine].uState = FILECACHE_STATE_INIT;
	}

	nFileOffset = nPage * get_cacheLineSize ();

#if defined (_DEBUG)
	FILEMEM_ASSERT (nFileOffset < getLength (), "CFileMem::loadPage: file offset exceeds file size");
#endif

	switch (m_eMode)
	{
		case VIEW_FILEMAPPED:	nLineLen = (size_t) get_cacheLineSize ();

								if ((nFileOffset + nLineLen) > getLength ())
								{
									nLineLen = (size_t)(getLength () - nFileOffset);
								}
#if defined (WIN32)
								nHighOffset = (DWORD)((nFileOffset >> 32ULL) & 0xFFFFFFFFUL);
								nLowOffset = (DWORD)(nFileOffset & 0xFFFFFFFFUL);

								m_psCache[nLine].pMappedView = MapViewOfFile (m_hFileMapping, (m_bReadOnly ? FILE_MAP_COPY : FILE_MAP_ALL_ACCESS), nHighOffset, nLowOffset, nLineLen);
#elif defined (LINUX)
								prot = PROT_READ;
								nFlags = MAP_PRIVATE;

								if (!m_bReadOnly)
								{
									prot |= PROT_WRITE;
								}

								if (m_uOpenFlags & shareDenyNone)
								{
									nFlags &= ~MAP_PRIVATE;
									nFlags |= MAP_SHARED;
								}

								m_psCache[nLine].pMappedView = mmap64 (NULL, (size_t) nLineLen, prot, nFlags, m_filedes, nFileOffset);

								if (m_psCache[nLine].pMappedView == ((void *) -1))
								{
									char		*pszMessage;

									switch (errno)
									{
									case EACCES	:	pszMessage = (char *) "CFileMem::loadPage: mmap64 failed to map file to memory! error code = EACCES";

												break;

									case EAGAIN	:	pszMessage = (char *) "CFileMem::loadPage: mmap64 failed to map file to memory! error code = EAGAIN";

												break;


									case EBADF	:	pszMessage = (char *) "CFileMem::loadPage: mmap64 failed to map file to memory! error code = EBADF";

												break;


									case EINVAL	:	pszMessage = (char *) "CFileMem::loadPage: mmap64 failed to map file to memory! error code = EINVAL";


												break;


									case ENFILE	:	pszMessage = (char *) "CFileMem::loadPage: mmap64 failed to map file to memory! error code = ENFILE";

												break;


									case ENODEV	:	pszMessage = (char *) "CFileMem::loadPage: mmap64 failed to map file to memory! error code = ENODEV";

												break;


									case ENOMEM	:	pszMessage = (char *) "CFileMem::loadPage: mmap64 failed to map file to memory! error code = ENOMEM";

												break;


									case EPERM	:	pszMessage = (char *) "CFileMem::loadPage: mmap64 failed to map file to memory! error code = EPERM";

												break;


									case ETXTBSY	:	pszMessage = (char *) "CFileMem::loadPage: mmap64 failed to map file to memory! error code = ETXTBSY";

												break;


									case EOVERFLOW	:	pszMessage = (char *) "CFileMem::loadPage: mmap64 failed to map file to memory! error code = EOVERFLOW";

												break;


/*									case SIGSEGV	:	pszMessage = (char *) "CFileMem::loadPage: mmap64 failed to map file to memory! error code = SIGSEGV";

												break;


									case SIGBUS	:	pszMessage = (char *) "CFileMem::loadPage: mmap64 failed to map file to memory! error code = SIGBUS";

												break;*/

									default		:	pszMessage = (char *) "CFileMem::loadPage: mmap64 failed to map file to memory!";

												break;
}

									FILEMEM_ASSERT (m_psCache[nLine].pMappedView != ((void *) -1), pszMessage);
								}
#endif

								break;

		case VIEW_RWCOPIED:		if (m_psCache[nLine].pMirrorView == NULL)
								{
									m_psCache[nLine].pMirrorView = malloc ((size_t) get_cacheLineSize ());

									FILEMEM_ASSERT (m_psCache[nLine].pMirrorView != NULL, "CFileMem::loadPage: insufficient memory");
								}

		case VIEW_RCOPIED:		if (m_psCache[nLine].pMappedView == NULL)
								{
									m_psCache[nLine].pMappedView = malloc ((size_t) get_cacheLineSize ());

									FILEMEM_ASSERT (m_psCache[nLine].pMappedView != NULL, "CFileMem::loadPage: insufficient memory");
								}

								seek (nFileOffset);

								{
									uint64_t		nReadBytes;
#if defined (_HAS_MFC)
									{
										UINT		nLen;

										nLen = m_pFile->Read (m_psCache[nLine].pMappedView, (UINT) get_cacheLineSize ());

										nReadBytes = (uint64_t) nLen;
									}
#elif defined (WIN32)
									{
										DWORD		nLen;
										BOOL		bRslt;

										bRslt = ReadFile (m_hFile, m_psCache[nLine].pMappedView, (DWORD) get_cacheLineSize (), &nLen, NULL);

										FILEMEM_ASSERT (bRslt != FALSE, "CFileMem::loadPage: ReadFile failed!");

										nReadBytes = (uint64_t) nLen;
									}
#elif defined (LINUX)
									ssize_t		nLen;
									
									nLen = read (m_filedes, m_psCache[nLine].pMappedView, (size_t) get_cacheLineSize ());

									if (nLen == -1)
									{
										char	buffer[1024];

										sprintf (buffer, "%u\n%u\n%u\n%u\n%8X\n%u\nCFileMem::loadPage: read failed!", m_eMode, errno, m_filedes, nLine, m_psCache[nLine].pMirrorView, (size_t) get_cacheLineSize ());

										FILEMEM_ASSERT (nLen != -1, buffer);
									}

									nReadBytes = (uint64_t) nLen;
#endif
									if (nReadBytes < get_cacheLineSize ())
									{
										uint64_t	nPadBytes = get_cacheLineSize () - nReadBytes;
										size_t		nPadSize = (size_t) nPadBytes;
										size_t		nReadSize = (size_t) nReadBytes;
										uint8_t		*pPadArea = &(((uint8_t *)(m_psCache[nLine].pMappedView))[nReadSize]);

										::memset ((void *) pPadArea, 0xED, nPadSize);
									}
								}

								if (m_eMode == VIEW_RWCOPIED)
								{
									::memcpy (m_psCache[nLine].pMirrorView, m_psCache[nLine].pMappedView, (size_t) get_cacheLineSize ());
								}

								break;

		default:				FILEMEM_ASSERT (false, "CFileMem::loadPage: unknown mode");

								break;
	} // switch (m_eMode)

	if (m_psCache[nLine].pMappedView != NULL)
	{
		m_psCache[nLine].uState |= FILECACHE_STATE_VALID;
		m_psCache[nLine].nPage = nPage;

		return (true);
	}

	return (false);
}

bool CFileMem::seek (uint64_t nPos)
{
#if defined (_HAS_MFC)
	return (seek (nPos, this->m_pFile));
#elif defined (WIN32)
	LARGE_INTEGER	sPos;

	sPos.QuadPart = nPos;

	return (SetFilePointer (m_hFile, sPos.LowPart, &sPos.HighPart, FILE_BEGIN) == INVALID_SET_FILE_POINTER ? false : true);
#elif defined (LINUX)
	return (nPos == lseek64 (m_filedes, nPos, SEEK_SET));
#endif
}

#if defined (_HAS_MFC)
bool CFileMem::seek (uint64_t nPos, CFile *pf)
{
	try
	{
		uint64_t	udiff;
		int64_t		idiff;

		while (nPos != pf->GetPosition ())
		{
			if (nPos < pf->GetPosition ())
			{
				udiff = pf->GetPosition () - nPos;

				if (udiff > 0x7FFFFFFFFFFFFFFFULL)
				{
					pf->Seek (0x7FFFFFFFFFFFFFFFLL, pf->current);
				}
			}
			else
			{
				udiff = nPos - pf->GetPosition ();

				if (udiff > 0x7FFFFFFFFFFFFFFFULL)
				{
					pf->Seek (0x8000000000000001LL, pf->current);
				}
			}

			if (udiff <= 0x7FFFFFFFFFFFFFFFULL)
			{
				idiff = pf->GetPosition () - nPos;

				pf->Seek (-idiff, pf->current);
			}

#if defined (CF_SSE2)
			__asm prefetcht0  [esp] 
#endif
		}
	}
	catch (CFileException *e)
	{
		e;

		return (false);
	}

	return (true);
}
#endif

inline uint64_t CFileMem::get_cacheLineSize ()
{
	return (m_nCacheLineSize);
}

uint64_t CFileMem::get_numCacheLines ()
{
	return (1ULL << this->m_nCacheLineBits);
}

uint64_t CFileMem::get_cacheLineMask ()
{
	return (m_uCacheLineMask);
}
