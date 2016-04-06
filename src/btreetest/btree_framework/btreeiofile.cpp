/************************************************************
**
** file:	btreeiofile.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** file data layer class.
**
************************************************************/

#ifndef	BTREEFILEIO_CPP
#define	BTREEFILEIO_CPP

#include "btreeiofile.h"

using namespace std;

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeFileIO
(
	CBTreeIOpropertiesFile &rDataLayerProperties, 
	_t_addresstype nBlockSize, 
	_t_subnodeiter nNodeSize, 
	uint32_t nNumDataPools, 
	CBTreeIOperBlockPoolDesc_t *psDataPools
)
	:	CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype> (nBlockSize, nNodeSize, nNumDataPools, psDataPools)
#if defined (WIN32)
	,	m_hFile (INVALID_HANDLE_VALUE)
	,	m_hFileMapping (INVALID_HANDLE_VALUE)
#elif defined (LINUX)
	,	m_nFileDesc (-1)
#endif
	,	m_nTotalAddressSpace (0)
{
#if defined (WIN32)

	typedef DWORD		pagesize_t;

#elif defined (LINUX)

	typedef int			pagesize_t;

#endif

	std::stringstream	aszFilename;
	LARGE_INTEGER		sTimeui64;
	uint32_t			nFails = 0;
	bool				bOpenedFile = false;
	uint32_t			nAttempts = 0;
	pagesize_t			nPageSize;
	char				*pszFileName;

#if defined (WIN32)

	DWORD				nAccess = GENERIC_READ | GENERIC_WRITE;
	DWORD				nShare = 0;
	DWORD				nCreation = OPEN_EXISTING;
	DWORD				nAttr = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_DELETE_ON_CLOSE | FILE_FLAG_RANDOM_ACCESS;
	struct _finddata_t	sFileinfo;
	SYSTEM_INFO			sSystemInfo;
	
#elif defined (LINUX)

	uint32_t			ui32;
	int					nOpenFlags = O_TRUNC | O_CREAT | O_RDWR | O_NDELAY | O_NOATIME;
	mode_t				nCreateMode = S_IRUSR | S_IWUSR;

#endif

	m_pClDataLayerProperties = new CBTreeIOpropertiesFile (rDataLayerProperties);

	BTREEDATA_ASSERT (m_pClDataLayerProperties != NULL, "CBTreeFileIO::CBTreeFileIO: insufficient memory!");

#if defined (WIN32)

	GetSystemInfo (&sSystemInfo);

	nPageSize = sSystemInfo.dwAllocationGranularity;

#elif defined (LINUX)

//	nPageSize = getpagesize ();

	nPageSize = sysconf(_SC_PAGE_SIZE);

#endif

	if (nBlockSize == 0)
	{
		nBlockSize = (_t_addresstype) nPageSize;

		// these asserts usually flag if the type of _t_addresstype is set to a too small type (ie. _t_addresstype = uint16 and HW page size is 64k)
		BTREE_ASSERT (nBlockSize == (_t_addresstype) nPageSize, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeFileIO: variable nBlockSize cannot display page size!");
		BTREE_ASSERT (nPageSize == (pagesize_t) nBlockSize, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeFileIO: variable nBlockSize cannot display page size!");
	}
	else
	{
		nBlockSize = (_t_addresstype) ((nBlockSize + nPageSize - 1) / nPageSize);

		nBlockSize *= (_t_addresstype) nPageSize;

		// these asserts usually flag if the type of _t_addresstype is set to a too small type (ie. _t_addresstype = uint16 and HW page size is 64k)
		BTREE_ASSERT (nBlockSize >= nPageSize, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeFileIO: variable nBlockSize cannot display page size!");
		BTREE_ASSERT ((nBlockSize % nPageSize) == 0, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeFileIO: variable nBlockSize cannot display page size!");
	}

	this->setup (nBlockSize);

	do
	{
		m_strTempFile = m_pClDataLayerProperties->get_pathname ();
	
		QueryPerformanceCounter (&sTimeui64);
	
		aszFilename.clear ();
		aszFilename << hex << setfill ('0') << setw (16) << sTimeui64.QuadPart << setw (2) << nFails << dec;

		if ((m_strTempFile[m_strTempFile.size () - 1] != '/') && (m_strTempFile[m_strTempFile.size () - 1] != '\\'))
		{
			m_strTempFile += string ("/");
		}
	
		m_strTempFile += aszFilename.str ();
		m_strTempFile += string (".bttmp");

#if defined (LINUX)
		for (ui32 = 0; ui32 < m_strTempFile.length (); ui32++)
		{
			if (m_strTempFile[ui32] == '\\')
			{
				m_strTempFile[ui32] = '/';
			}
		}
#endif

		pszFileName = (char *) m_strTempFile.c_str ();

#if defined (WIN32)

		if (_findfirst (pszFileName, &sFileinfo) != -1)
		{
			nCreation = TRUNCATE_EXISTING;
		}
		else
		{
			nCreation = CREATE_NEW;
		}
	
		m_hFile = CreateFileA (pszFileName, nAccess, nShare, NULL, nCreation, nAttr, 0);

		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			bOpenedFile = true;
		}
		
#elif defined (LINUX)

		int		openFlags = O_TRUNC | O_CREAT | O_RDWR;
		mode_t	createMode = S_IRUSR | S_IWUSR;
	
		m_nFileDesc = ::open (pszFileName, openFlags, createMode);
	
		if (m_nFileDesc > 0)
		{
			unlink (pszFileName);

			bOpenedFile = true;
		}

#endif

		nAttempts++;

	} while ((!bOpenedFile) && (nAttempts <= 15));

	BTREE_ASSERT (bOpenedFile, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeFileIO: failed to open temporary file!");

	// init_mapping () fails by throwing an exception when a mapping handle on a zero length file has to be called into existance.
	// to work around that, the file size is set to the size of one hardware page. to do so set_size () is used, since it has that
	// code to perform the operation already in it. the problem is, that for set_size () to work correctly the mapping handle has to
	// be in place, which is not the case here. An extra parameter is not an option, as this method is present in other data layer
	// classes and would render CBTreeFileIO::set_size () incompatible and then would fail to compile.
	// The solution was to have a member variable (m_bNoMapHandling), to control whether or not the mapping handle code in
	// CBTreeFileIO::set_size () is active. To disable any mapping handle code during construction m_bNoMapHandling is asserted
	// here, but has to be left alone anywhere else.
	m_bNoMapHandling = true;
	{
		set_size (1);
	}
	m_bNoMapHandling = false;

	init_mapping ();
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::~CBTreeFileIO ()
{
	unmap_all_descriptors (false);

	exit_mapping ();

#if defined (WIN32)

	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle (m_hFile);

		m_hFile = INVALID_HANDLE_VALUE;
	}
		
#elif defined (LINUX)

	if (m_nFileDesc > 0)
	{
		::close (m_nFileDesc);

		m_nFileDesc = -1;
	}

#endif

	delete m_pClDataLayerProperties;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_performance_counters (uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR])
{
#if defined (USE_PERFORMANCE_COUNTERS)
	memcpy ((void *) &rHitCtrs, (void *) m_anHitCtrs, sizeof (m_anHitCtrs));
	memcpy ((void *) &rMissCtrs, (void *) m_anMissCtrs, sizeof (m_anMissCtrs));

										m_pFile->getPerfCtrs (rHitCtrs[PERFCTR_FILE_CACHE], rMissCtrs[PERFCTR_FILE_CACHE]);

										break;

#else
	memset ((void *) &rHitCtrs, 0, sizeof (rHitCtrs));
	memset ((void *) &rMissCtrs, 0, sizeof (rMissCtrs));
#endif
}

/*

get_pooledData - get pooled data

nPool[in]	- specifies data pool ID
nNode[in]	- specifies linear node address of data pool

*/

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
template<class _t_dl_data>
_t_dl_data* CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_pooledData (uint32_t nPool, _t_nodeiter nNode)
{
	uint32_t		nDescriptor = this->convert_node_to_descriptor (nNode);
	_t_addresstype	nOffset;

	if (this->m_psDescriptorVector[nDescriptor].pBlockData == NULL)
	{
		this->map_descriptor (nDescriptor);
	}

	this->increment_access_counter (nDescriptor);

	nOffset = this->get_node_offset (nNode);
	nOffset += this->get_per_node_pool_offset (nPool);

	return ((_t_dl_data *) &(this->m_psDescriptorVector[nDescriptor].pBlockData[nOffset]));
}

/*

get_pooledData - get pooled data

nPool[in]	- specifies data pool ID
nNode[in]	- specifies linear node address of data pool
nEntry[in]	- specifies entry to be returned

*/

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
template<class _t_dl_data>
_t_dl_data* CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_pooledData (uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nEntry)
{
	uint8_t			*pClData = this->template get_pooledData<uint8_t> (nPool, nNode);
	_t_dl_data		*psData = (_t_dl_data *) &(pClData[nEntry * this->get_pool_entry_size (nPool)]);

	return (psData);
}

/*

insert_dataIntoPool

node	- node ID
nodeLen	- total entry number
offset	- entry where new data is to be inserted
dataLen	- number of new entries
pData	- pointer to new data

*/

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
template <class _t_dl_data>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::insert_dataIntoPool (uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nNodeLen, _t_subnodeiter nOffset, _t_subnodeiter nDataLen, const _t_dl_data *pData)
{
#if defined (_DEBUG)

	BTREEDATA_ASSERT ((this->get_pool_entry_size (nPool) * nNodeLen) <= this->get_pool_total_size (nPool), "CBTreeRAMIO::insert_dataInDataBuffer: length exceeds node size");

#endif

	_t_dl_data		*psNodeData = this->template get_pooledData<_t_dl_data> (nPool, nNode);

	memmove ((void *) &(psNodeData[nOffset + nDataLen]), (void *) &(psNodeData[nOffset]), sizeof (*psNodeData) * (nNodeLen - nOffset));
	memcpy ((void *) &(psNodeData[nOffset]), (void *) pData, sizeof (*psNodeData) * nDataLen);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_size (_t_nodeiter nMaxNodes)
{
	if (nMaxNodes == this->m_nMaxNodes)
	{
		return;
	}

	_t_addresstype		nSize;
	_t_addresstype		nOldSize;

	nSize = this->get_nodeAddr (nMaxNodes + 1);

	nOldSize = this->get_nodeAddr (this->m_nMaxNodes + 1);

#if defined (_DEBUG)

	if (nMaxNodes > this->m_nMaxNodes)
	{
		BTREE_ASSERT (nSize >= nOldSize, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_size: ERROR: While a larger space was requested, a smaller size was calculated! This means that, variables of type _t_addresstype cannot display an address space necessary for this application!");
	}

	if (nMaxNodes < this->m_nMaxNodes)
	{
		BTREE_ASSERT (nSize <= nOldSize, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_size: ERROR: While a smaller space was requested, a larger size was calculated! This means that, variables of type _t_addresstype cannot display an address space necessary for this application!");
	}

#endif

	nSize = (nSize + this->m_nBlockSize - 1) / this->m_nBlockSize;
	nSize *= this->m_nBlockSize;

	nOldSize = (nOldSize + this->m_nBlockSize - 1) / this->m_nBlockSize;
	nOldSize *= this->m_nBlockSize;
		
	if ((nSize != nOldSize) || (this->m_nMaxNodes == 0))
	{
		if (!m_bNoMapHandling)
		{
			unmap_all_descriptors (false);

			exit_mapping ();
		}

#if defined (WIN32)

		LARGE_INTEGER		sNewLen;
		BOOL				bRslt;

		sNewLen.QuadPart = (LONGLONG) nSize;

		bRslt = SetFilePointerEx (m_hFile, sNewLen, NULL, FILE_BEGIN);

		BTREE_ASSERT (bRslt != FALSE, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_size: failed to set file pointer!");

		bRslt = SetEndOfFile (m_hFile);

		BTREE_ASSERT (bRslt != FALSE, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_size: failed to set end of file marker!");

#if defined (_DEBUG)
		{
			LARGE_INTEGER		ui64len;

			BTREE_ASSERT (GetFileSizeEx (m_hFile, &ui64len) != false, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_size: Failed to confirm new file size!");

			BTREE_ASSERT (ui64len.QuadPart == nSize, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_size: : Failed to set new file size!");
		}
#endif

#elif defined (LINUX)

		int		nRslt;

		nRslt = ftruncate64 (m_nFileDesc, (off_t) nSize);

		BTREE_ASSERT (nRslt == 0, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_size: failed to truncate file to new length!");

#if defined (_DEBUG)

		uint64_t		n64curPos = lseek64 (m_nFileDesc, 0, SEEK_CUR);

		nSize -= lseek64 (m_nFileDesc, 0, SEEK_END);

		lseek64 (m_nFileDesc, n64curPos, SEEK_SET);

		BTREE_ASSERT (nSize == 0ULL, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_size: Failed to set new file size!");

#endif

#endif

		if (!m_bNoMapHandling)
		{
			init_mapping ();
		}

		this->realloc_descriptor_vector (nMaxNodes + 1);
	}

	this->m_nMaxNodes = nMaxNodes;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::unload ()
{
	unmap_all_descriptors (true);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::unload_from_cache (_t_nodeiter nNode)
{
	nNode;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
bool CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::is_dataCached (uint32_t nPool, _t_nodeiter nNode)
{
	nPool;
	nNode;

	return (false);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::terminate_access ()
{
	uint32_t		nMinAccessCtr;
	uint32_t		ui32;
	uint32_t		nNumUnmapped;
	uint32_t		nDescRoot;
	_t_addresstype	nTotalAddressSpace = m_nTotalAddressSpace;

	if (this->m_nPrevRootNode == ~0)
	{
		nDescRoot = this->m_nDescriptorVectorSize;
	}
	else
	{
		nDescRoot = this->convert_node_to_descriptor (this->m_nPrevRootNode);
	}

	while (nTotalAddressSpace > (_t_addresstype) m_pClDataLayerProperties->get_address_space_soft_limit ())
	{
		// for first active descriptor and therewith initialise minimum access counter
		for (ui32 = 0; ui32 < this->m_nDescriptorVectorSize; ui32++)
		{
			if (this->m_psDescriptorVector[ui32].pBlockData != NULL)
			{
				if (!this->m_psDescriptorVector[ui32].bMarkedForUnmap)
				{
					if (ui32 != nDescRoot)
					{
						nMinAccessCtr = this->m_psDescriptorVector[ui32].nAccessCtr;

						break;
					}
				}
			}
		}

		nNumUnmapped = 0;

		// if minimum access counter was initialised ... (otherwise stop)
		if (ui32 < this->m_nDescriptorVectorSize)
		{
			// ... find smallest access counter
			for (ui32++; ui32 < this->m_nDescriptorVectorSize; ui32++)
			{
				if (this->m_psDescriptorVector[ui32].pBlockData != NULL)
				{
					if (!this->m_psDescriptorVector[ui32].bMarkedForUnmap)
					{
						if (ui32 != nDescRoot)
						{
							if (this->m_psDescriptorVector[ui32].nAccessCtr < nMinAccessCtr)
							{
								nMinAccessCtr = this->m_psDescriptorVector[ui32].nAccessCtr;
							}
						}
					}
				}
			}

			// any descriptor having the smallest access counter will get unmapped
			for (ui32 = 0; ui32 < this->m_nDescriptorVectorSize; ui32++)
			{
				if (this->m_psDescriptorVector[ui32].pBlockData != NULL)
				{
					if (!this->m_psDescriptorVector[ui32].bMarkedForUnmap)
					{
						if (ui32 != nDescRoot)
						{
							if (this->m_psDescriptorVector[ui32].nAccessCtr == nMinAccessCtr)
							{
								sync_descriptor (ui32);

								nTotalAddressSpace -= this->m_nBlockSize;

								nNumUnmapped++;
							}
						}
					}
				}
			}
		}

		if (nNumUnmapped == 0)
		{
			break;
		}
	}

	for (ui32 = 0; ui32 < this->m_nDescriptorVectorSize; ui32++)
	{
		if (this->m_psDescriptorVector[ui32].bMarkedForUnmap)
		{
			unmap_descriptor (ui32);
		}
	}
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::showdump	(std::ofstream &ofs, _t_nodeiter nTreeSize, char *pAlloc)
{
	_t_nodeiter		ui64;
	uint32_t		ui32, uj32;

	this->set_cacheFreeze (true);
	{
		ofs << "<H1>data dump</H1>" << endl;
		ofs << "<br>" << endl;

		ofs << "m_nBlockSize: " << this->m_nBlockSize << " (0x" << hex << this->m_nBlockSize << dec << ")<br>" << endl;
		ofs << "m_nNodeSize: " << this->m_nNodeSize << " (0x" << hex << this->m_nNodeSize << dec << ")<br>" << endl;
		ofs << "m_nNodesPerBlock: " << this->m_nNodesPerBlock << " (0x" << hex << this->m_nNodesPerBlock << dec << ")<br>" << endl;
		ofs << "m_nNodesPerBlockVectorSize: " << this->m_nNodesPerBlockVectorSize << " (0x" << hex << this->m_nNodesPerBlockVectorSize << dec << ")<br>" << endl;
		ofs << "m_nAlignedNodeSize: " << this->m_nAlignedNodeSize << " (0x" << hex << this->m_nAlignedNodeSize << dec << ")<br>" << endl;
		
		ofs << "<br>" << endl;
		
		ofs << "m_nNumDataPools: " << this->m_nNumDataPools << "<br>" << endl;
		
		ofs << "<table>" << endl;
		
		ofs << "<tr>" << endl;
		ofs << "<td></td>" << endl;

		for (ui32 = 0; ui32 < this->m_nNumDataPools; ui32++)
		{
			ofs << "<td>Pool " << ui32 << "</td>" << endl;
		}

		ofs << "</tr>" << endl;

		ofs << "<tr>" << endl;
		ofs << "<td>m_psDataPools->nEntrySize</td>" << endl;

		for (ui32 = 0; ui32 < this->m_nNumDataPools; ui32++)
		{
			ofs << "<td>" << this->m_psDataPools[ui32].nEntrySize << "</td>" << endl;
		}

		ofs << "</tr>" << endl;

		ofs << "<tr>" << endl;
		ofs << "<td>m_psDataPools->nTotalSize</td>" << endl;

		for (ui32 = 0; ui32 < this->m_nNumDataPools; ui32++)
		{
			ofs << "<td>" << this->m_psDataPools[ui32].nTotalSize << "</td>" << endl;
		}

		ofs << "</tr>" << endl;
		
		ofs << "<tr>" << endl;
		ofs << "<td>m_pnPerNodePoolOffset</td>" << endl;

		for (ui32 = 0; ui32 < this->m_nNumDataPools; ui32++)
		{
			ofs << "<td>" << this->m_pnPerNodePoolOffset[ui32] << "</td>" << endl;
		}

		ofs << "</tr>" << endl;
		
		ofs << "</table>" << endl;

		ofs << "<table>" << endl;

		for (ui64 = 0; ui64 < nTreeSize; )
		{
			for (ui32 = 0; ui32 < ((nTreeSize + this->m_nNodesPerBlock - 1) / this->m_nNodesPerBlock); ui32++)
			{
				ofs << "<tr>" << endl;
				ofs << "<td>" << endl;

				ofs << "block: " << ui32 << endl;

				ofs << "</td><td></td>" << endl;
				ofs << "</tr>" << endl;

				for (uj32 = 0; uj32 < (1U << this->m_nNodesPerBlockVectorSize); uj32++, ui64++)
				{
					ofs << "<tr>" << endl;
					ofs << "<td></td><td>" << endl;

					ofs << "<a name=\"node_dump_" << ui64 << "\">";
					ofs << "node: " << ui64;
					ofs << "</a>" << endl;

					ofs << "</td><td><a href=\"#node_" << ui64 << "\">tree<a/></td><td><a href=\"#node_list_" << ui64 << "\">list<a/></td>" << endl;
					ofs << "</tr>" << endl;

					{
						ofs << "<tr>" << endl;

						ofs << "<td></td><td></td><td>" << endl;

						uint32_t	uk32;

						for (uk32 = 0; uk32 < this->m_nNumDataPools; uk32++)
						{
							_t_addresstype	nAddr = this->get_pool_address (uk32, ui64);
							bool			bIsCached = is_dataCached (uk32, ui64);

							ofs << "<td>" << endl;
							
							if (!bIsCached)
							{
								ofs << "<font color=\"#999999\">";
							}

							ofs << nAddr << ":" << endl;
							
							if (!bIsCached)
							{
								ofs << "</font>";
							}

							ofs << "</td>" << endl;

							ofs << "<td>" << endl;

							if (bIsCached)
							{
								uint32_t	ul32;
								uint8_t		*pb;
								uint32_t	nMaxPoolLen;

								ofs << "<font color=\"#4040FF\">";

								nMaxPoolLen = this->get_pool_total_size (uk32);

								pb = get_pooledData<uint8_t> (uk32, ui64, nMaxPoolLen / this->get_pool_entry_size (uk32));
							
								for (ul32 = 0; ul32 < nMaxPoolLen; ul32++)
								{
									ofs << hex << setfill ('0') << setw (2) << (uint32_t) pb[ul32] << dec;

									if (((ul32 + 1) % this->get_pool_entry_size (uk32)) == 0)
									{
										ofs << "<br>" << endl;
									}
									else if (((ul32 + 1) % 4) == 0)
									{
										ofs << " ";
									}
								}

								ofs << "</font>";

								ofs << "<br>";
							}
							
//							uint32_t	ul32;
//							uint8_t		b;

//							if (m_pFile != NULL)
//							{
//								ofs << "<font color=\"#999999\">";
//								
//								for (ul32 = 0; ul32 < this->get_pool_total_size (uk32); ul32++)
//								{
//									m_pFile->memcpy ((void *) &b, nAddr + ul32, 1ULL);
//
//									ofs << hex << setfill ('0') << setw (2) << (uint32_t) b << dec;
//
//									if (((ul32 + 1) % this->get_pool_entry_size (uk32)) == 0)
//									{
//										ofs << "<br>";
//									}
//									else if (((ul32 + 1) % 4) == 0)
//									{
//										ofs << " ";
//									}
//								}
//
//								ofs << "</font>";
//							}
//							
							ofs << "</td>";
						}

						ofs << "</tr>" << endl;
					}
				}
			}
		}

		ofs << "</table>" << endl;
	}
	this->set_cacheFreeze (false);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::init_mapping ()
{
#if defined (WIN32)

	BTREE_ASSERT (m_hFile != INVALID_HANDLE_VALUE, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::init_mapping: file handle not initialised!");
	BTREE_ASSERT (m_hFileMapping == INVALID_HANDLE_VALUE, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::init_mapping: map handle was previously initialised");

	m_hFileMapping = CreateFileMapping (m_hFile, NULL, PAGE_READWRITE, 0, 0, NULL);

	BTREE_ASSERT (m_hFileMapping != NULL, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::init_mapping: failed to initialise map handle!");

#endif
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::exit_mapping ()
{
#if defined (WIN32)

	BTREE_ASSERT (m_hFileMapping != INVALID_HANDLE_VALUE, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::exit_mapping: map handle was not initialised!");

	DWORD	nErrCode = GetLastError ();

	BTREE_ASSERT (nErrCode == ERROR_SUCCESS, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::exit_mapping: error code was not reset!");

	HRESULT		sRslt = CloseHandle (m_hFileMapping);

	if (sRslt != S_OK)
	{
		// it looks like CloseHandle sometimes returns with an error on file mapping handles, although everything seems to be fine
		nErrCode = GetLastError ();

		// in those cases the code is double checking by using GetLastError ()
		BTREE_ASSERT (nErrCode == ERROR_SUCCESS, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::exit_mapping: map handle was not de-initialised!");
	}
									
	m_hFileMapping = INVALID_HANDLE_VALUE;

#endif
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor (uint32_t nDescriptor)
{
	uint64_t	nFileOffset;

#if defined (WIN32)

	DWORD				nHighOffset, nLowOffset;

#elif defined (LINUX)

	int		prot;
	int		nFlags;

#endif

	size_t				nLineLen;

#if defined (_DEBUG)

	BTREE_ASSERT (nDescriptor < this->m_nDescriptorVectorSize, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: requested descriptor exceeds descriptor vector size!");
	BTREE_ASSERT (this->m_psDescriptorVector[nDescriptor].pBlockData == NULL, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: requested mapping already in place!");

#endif

	nFileOffset = this->m_nBlockSize * nDescriptor;

	nLineLen = (size_t) this->m_nBlockSize;

#if defined (WIN32)

#if defined (_DEBUG)

	BTREE_ASSERT (m_hFileMapping != INVALID_HANDLE_VALUE, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: map handle was not initialised!");

#endif

	nHighOffset = (DWORD)((nFileOffset >> 32ULL) & 0xFFFFFFFFUL);
	nLowOffset = (DWORD)(nFileOffset & 0xFFFFFFFFUL);

	this->m_psDescriptorVector[nDescriptor].pBlockData = (uint8_t *) MapViewOfFile (m_hFileMapping, FILE_MAP_ALL_ACCESS, nHighOffset, nLowOffset, nLineLen);

	BTREE_ASSERT (this->m_psDescriptorVector[nDescriptor].pBlockData != NULL, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: address mapping failed!");

#elif defined (LINUX)

#if defined (_DEBUG)

	BTREE_ASSERT (m_nFileDesc > 0, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: file descriptor was not initialised!");

#endif

	prot = PROT_READ | PROT_WRITE;
	nFlags = MAP_SHARED;

	this->m_psDescriptorVector[nDescriptor].pBlockData = (uint8_t *) mmap64 (NULL, (size_t) nLineLen, prot, nFlags, m_nFileDesc, nFileOffset);

	if (this->m_psDescriptorVector[nDescriptor].pBlockData == ((void *) -1))
	{
		char		*pszMessage;

		switch (errno)
		{
		case EACCES	:	pszMessage = (char *) "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: mmap64 failed to map file to memory! error code = EACCES";

						break;

		case EAGAIN	:	pszMessage = (char *) "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: mmap64 failed to map file to memory! error code = EAGAIN";

						break;

		case EBADF	:	pszMessage = (char *) "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: mmap64 failed to map file to memory! error code = EBADF";

						break;

		case EINVAL	:	pszMessage = (char *) "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: mmap64 failed to map file to memory! error code = EINVAL";

						break;

		case ENFILE	:	pszMessage = (char *) "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: mmap64 failed to map file to memory! error code = ENFILE";

						break;

		case ENODEV	:	pszMessage = (char *) "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: mmap64 failed to map file to memory! error code = ENODEV";

						break;

		case ENOMEM	:	pszMessage = (char *) "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: mmap64 failed to map file to memory! error code = ENOMEM";

						break;

		case EPERM	:	pszMessage = (char *) "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: mmap64 failed to map file to memory! error code = EPERM";

						break;

		case ETXTBSY:	pszMessage = (char *) "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: mmap64 failed to map file to memory! error code = ETXTBSY";

						break;

		case EOVERFLOW:	pszMessage = (char *) "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: mmap64 failed to map file to memory! error code = EOVERFLOW";

						break;

		default		:	pszMessage = (char *) "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::map_descriptor: mmap64 failed to map file to memory!";

						break;
		}

		BTREE_ASSERT (this->m_psDescriptorVector[nDescriptor].pBlockData != ((uint8_t *) -1), pszMessage);
	}

#endif

	this->m_psDescriptorVector[nDescriptor].bMarkedForUnmap = false;

	m_nTotalAddressSpace += this->m_nBlockSize;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::sync_descriptor (uint32_t nDescriptor)
{
#if defined (_DEBUG)

	BTREE_ASSERT (nDescriptor < this->m_nDescriptorVectorSize, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::sync_descriptor: requested descriptor exceeds descriptor vector size!");
	BTREE_ASSERT (this->m_psDescriptorVector[nDescriptor].pBlockData != NULL, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::sync_descriptor: requested unmapping already done!");

#endif

#if defined (WIN32)
	
	BOOL	bRslt = FlushViewOfFile ((void *) this->m_psDescriptorVector[nDescriptor].pBlockData, (size_t) this->m_nBlockSize);

	BTREE_ASSERT (bRslt == TRUE, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::sync_descriptor: failed to flush view of file!");

#elif defined (LINUX)

	int		nRslt = msync ((void *) this->m_psDescriptorVector[nDescriptor].pBlockData, (size_t) this->m_nBlockSize, MS_ASYNC);

	BTREE_ASSERT (nRslt == 0, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::sync_descriptor: failed to flush view of file!");

#endif

	this->m_psDescriptorVector[nDescriptor].bMarkedForUnmap = true;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::unmap_descriptor (uint32_t nDescriptor)
{
#if defined (_DEBUG)

	BTREE_ASSERT (nDescriptor < this->m_nDescriptorVectorSize, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::unmap_descriptor: requested descriptor exceeds descriptor vector size!");
	BTREE_ASSERT (this->m_psDescriptorVector[nDescriptor].pBlockData != NULL, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::unmap_descriptor: requested unmapping already done!");

#endif

#if defined (WIN32)
	
	BOOL	bRslt = UnmapViewOfFile ((void *) this->m_psDescriptorVector[nDescriptor].pBlockData);

	BTREE_ASSERT (bRslt == TRUE, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::unmap_descriptor: failed to unmap view of file!");

#elif defined (LINUX)

	int		nRslt = munmap ((void *) this->m_psDescriptorVector[nDescriptor].pBlockData, (size_t) this->m_nBlockSize);

	BTREE_ASSERT (nRslt == 0, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::unmap_descriptor: failed to unmap view of file!");

#endif

	if (this->m_nPrevRootNode != ~0)
	{
		if (nDescriptor == this->convert_node_to_descriptor (this->m_nPrevRootNode))
		{
			this->m_nPrevRootNode = ~0;
		}
	}

	this->m_psDescriptorVector[nDescriptor].pBlockData = NULL;
	this->m_psDescriptorVector[nDescriptor].bMarkedForUnmap = false;

	m_nTotalAddressSpace -= this->m_nBlockSize;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::unmap_all_descriptors (bool bExceptRoot)
{
	uint32_t	u;
	uint32_t	nDescRoot;

	if (this->m_nPrevRootNode == ~0)
	{
		nDescRoot = this->m_nDescriptorVectorSize;
	}
	else
	{
		nDescRoot = this->convert_node_to_descriptor (this->m_nPrevRootNode);
	}

	for (u = 0; u < this->m_nDescriptorVectorSize; u++)
	{
		if (!bExceptRoot || (bExceptRoot && (u != nDescRoot)))
		{
			if (this->m_psDescriptorVector[u].pBlockData != NULL)
			{
				sync_descriptor (u);
			}
		}
	}

	for (u = 0; u < this->m_nDescriptorVectorSize; u++)
	{
		if (this->m_psDescriptorVector[u].bMarkedForUnmap)
		{
			unmap_descriptor (u);
		}
	}
}

#endif // BTREEFILEIO_CPP
