/************************************************************
**
** file:	CFileMem.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains a class presenting files as linearly
** addressable memory.
**
************************************************************/

#ifndef	CFILEMEM_H
#define	CFILEMEM_H

#if defined (WIN32)

 #include <io.h>

 #if !defined (_HAS_MFC)
  #include <windows.h>
 #else
  #include <afx.h>
 #endif

#elif defined (LINUX)

 #include <unistd.h>
 #include <sys/mman.h>
 #include <string.h>
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <sys/ioctl.h>
 #include <linux/fs.h>
 #include <errno.h>

#endif

//#include <emmintrin.h>
#include <stdint.h>

#include <string>
#include <stdexcept>
#include <algorithm>

#define FILEMEM_ASSERT(c,m)		if (!(c)) throw (new std::runtime_error (m))

#define	DEFAULT_LINEBITS		4ULL
#define	DEFAULT_CACHELINESIZE	65536ULL

#if defined (WIN32)

#elif defined (LINUX)

 #include <stdio.h>
 #include <limits.h>

 #if !defined (_MAX_PATH)
  #define	_MAX_PATH				(PATH_MAX)
 #endif

#endif

class CFileMem
{
public:
	// enums
	enum	fileAccessFlags_e
	{
		modeRead = 0x00000001,
		modeWrite = 0x00000002,
		modeReadWrite = 0x00000003,
		shareDenyNone = 0x00000004,
		shareDenyRead = 0x00000008,
		shareDenyWrite = 0x00000010,
		shareExclusive = 0x00000020,
		modeCreate = 0x00000040,
		modeNoTruncate = 0x00000080,
		osNoBuffer = 0x00000100,
		osWriteThrough = 0x00000200,
		osRandomAccess = 0x00000400,
		osSequentialScan = 0x00000800,
	};

	enum	view_e							{VIEW_FILEMAPPED, VIEW_RWCOPIED, VIEW_RCOPIED, VIEW_RWLOAD, VIEW_RLOAD};
	enum	cache_e							{CACHE_NOT_FOUND, CACHE_HALF, CACHE_FOUND};
	
#define	FILECACHE_STATE_INIT				0x00
#define	FILECACHE_STATE_VALID				0x01
#define	FILECACHE_STATE_NOT_MAPPED			0x02

	// typedefs
	typedef struct
	{
		void				*pMappedView;
		void				*pMirrorView;
		uint64_t			nPage;
		uint32_t			uState;
	} fileCache_t;

	// constructors
								CFileMem			();
								~CFileMem			();

	bool						open				(const char *pFName, uint32_t nFlags, bool bDeleteOnClose = false);
	bool						close				();
	bool						flush				();
	void						unload				();

	bool						autoSetup			(uint32_t nCacheEffP, uint32_t nCacheEffQ, uint32_t nMaxEffFileSizeP, uint32_t nMaxEffFileSizeQ, bool bReadOnly);

	bool						setMode				(view_e eMode);
	view_e						getMode				();

	cache_e						getIsCached			(uint64_t nOffset, uint64_t nLen);

	uint64_t					getLength			();
	bool						setLength			(uint64_t nLen);

	bool						setCacheCfg			(uint32_t nLineBits = DEFAULT_LINEBITS, uint64_t nDesiredCacheLineSize = DEFAULT_CACHELINESIZE);
	void						getCacheCfg			(uint32_t *pnLineBits = NULL, uint64_t *pnCacheLineSize = NULL);

	void						getPerfCtrs			(uint64_t &rHitCtr, uint64_t &rMissCtr);

	void						memcpy				(uint64_t nDest, void *pSrc, uint64_t nLen);
	void						memcpy				(void *pDest, uint64_t nSrc, uint64_t nLen);
	void						memcpy				(uint64_t nDest, CFileMem *pSrcFile, uint64_t nSrc, uint64_t nLen);
#if defined (_HAS_MFC)
	void						memcpy				(uint64_t nDest, CFile *pSrcFile, uint64_t nPos, uint64_t nLen);
#endif

	bool						memcmp				(uint64_t nD0, void *pD1, uint64_t nLen);
	bool						memcmp				(uint64_t nD0, CFileMem *pFD0, uint64_t nPos, uint64_t nLen);
#if defined (_HAS_MFC)
	bool						memcmp				(uint64_t nD0, CFile *pFD0, uint64_t nPos, uint64_t nLen);
#endif

	void						memmove				(uint64_t nDest, CFileMem *pSrcFile, uint64_t nPos, uint64_t nLen);
#if defined (_HAS_MFC)
	void						memmove				(uint64_t nDest, CFile *pSrcFile, uint64_t nPos, uint64_t nLen);
#endif

	void						memset				(uint64_t nPos, uint8_t nVal, uint64_t nLen);

	unsigned char&				operator[]			(uint64_t nSubscript);

protected:
	bool						init				(bool bPrepare, bool bLoad);
	void						exit				(bool bExit, bool bUnload, bool bInvalidate);

	bool						flushLine			(uint64_t nLine);
	bool						loadPage			(uint64_t nPage);
	bool						seek				(uint64_t nPos);
#if defined (_HAS_MFC)
	static bool					seek				(uint64_t nPos, CFile *pf);
#endif
	inline uint64_t				get_cacheLineSize	();
	uint64_t					get_numCacheLines	();
	uint64_t					get_cacheLineMask	();
	bool						close				(bool bReset);

#if defined (_HAS_MFC)
	CFile						*m_pFile;
	HANDLE						m_hFile;
#elif defined (WIN32)
	HANDLE						m_hFile;
#else
	int							m_filedes;
#endif

	bool						m_bReadOnly;
	bool						m_bDeleteOnClose;
	view_e						m_eMode;

	uint32_t					m_nCacheLineBits;
	uint64_t					m_uCacheLineMask;
	uint64_t					m_nCacheLineSize;
	uint64_t					m_nCacheLineGranularity;
	bool						m_bCacheLineSizeBinary;
	uint64_t					m_uCacheLineSizeMask;
	uint32_t					m_nCacheLineSizeBits;
	uint64_t					m_nLength;

	uint64_t					m_nFromSubscript;
	uint64_t					m_nToSubscript;
	uint64_t					m_nPageSubscript;

#if defined (WIN32)
	HANDLE						m_hFileMapping;
#endif

	char						m_aszFilename[_MAX_PATH];
	uint32_t					m_uOpenFlags;

	fileCache_t					*m_psCache;

#if defined (USE_PERFORMANCE_COUNTERS)
	uint64_t					m_nHitCtr;
	uint64_t					m_nMissCtr;
#endif
};

#endif	// CFILEMEM_H
