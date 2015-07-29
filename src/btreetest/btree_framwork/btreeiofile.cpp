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
	uint32_t nBlockSize, 
	_t_subnodeiter nNodeSize, 
	uint32_t nNumDataPools, 
	CBTreeIOperBlockPoolDesc_t *psDataPools
)
	:	CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype> (nBlockSize, nNodeSize, nNumDataPools, psDataPools)
{
	std::stringstream	aszFilename;
	LARGE_INTEGER		sTimeui64;
	bool				bRslt;
	uint32_t			nFails = 0;
	uint32_t			ui32;

	m_pClDataLayerProperties = new CBTreeIOpropertiesFile (rDataLayerProperties);

	BTREEDATA_ASSERT (m_pClDataLayerProperties != NULL, "CBTreeFileIO::CBTreeFileIO: insufficient memory!");

	m_ppPoolCaches = new uint8_t * [this->m_nNumDataPools];

	BTREEDATA_ASSERT (m_ppPoolCaches != NULL, "CBTreeFileIO::CBTreeFileIO: insufficient memory!");

	m_pnPerBlockPoolCacheMaskes = new uint32_t [this->m_nNumDataPools];

	BTREEDATA_ASSERT (m_pnPerBlockPoolCacheMaskes != NULL, "CBTreeFileIO::CBTreeFileIO: insufficient memory!");

	m_ppnPerBlockPoolCacheNode = new _t_nodeiter * [this->m_nNumDataPools];

	BTREEDATA_ASSERT (m_ppnPerBlockPoolCacheNode != NULL, "CBTreeFileIO::CBTreeFileIO: insufficient memory!");

	m_ppbPerBlockPoolCacheValid = new bool * [this->m_nNumDataPools];

	BTREEDATA_ASSERT (m_ppbPerBlockPoolCacheValid != NULL, "CBTreeFileIO::CBTreeFileIO: insufficient memory!");

	m_ppnPerBlockPoolCacheLen = new uint32_t * [this->m_nNumDataPools];

	BTREEDATA_ASSERT (m_ppnPerBlockPoolCacheLen != NULL, "CBTreeFileIO::CBTreeFileIO: insufficient memory!");

	for (ui32 = 0; ui32 < this->m_nNumDataPools; ui32++)
	{
		m_pnPerBlockPoolCacheMaskes[ui32] = (1 << this->m_psDataPools[ui32].nCacheVectorSize) - 1;

		m_ppnPerBlockPoolCacheNode[ui32] = new _t_nodeiter [get_perBlockPoolCacheSize (ui32)];

		BTREEDATA_ASSERT (m_ppnPerBlockPoolCacheNode[ui32] != NULL, "CBTreeFileIO::CBTreeFileIO: insufficient memory!");

		memset ((void *) m_ppnPerBlockPoolCacheNode[ui32], 0, sizeof (**m_ppnPerBlockPoolCacheNode) * get_perBlockPoolCacheSize (ui32));

		m_ppbPerBlockPoolCacheValid[ui32] = new bool [get_perBlockPoolCacheSize (ui32)];

		BTREEDATA_ASSERT (m_ppbPerBlockPoolCacheValid[ui32] != NULL, "CBTreeFileIO::CBTreeFileIO: insufficient memory!");

		memset ((void *) m_ppbPerBlockPoolCacheValid[ui32], 0, sizeof (**m_ppbPerBlockPoolCacheValid) * get_perBlockPoolCacheSize (ui32));

		m_ppnPerBlockPoolCacheLen[ui32] = new uint32_t [get_perBlockPoolCacheSize (ui32)];

		BTREEDATA_ASSERT (m_ppnPerBlockPoolCacheLen[ui32] != NULL, "CBTreeFileIO::CBTreeFileIO: insufficient memory!");

		m_ppPoolCaches[ui32] = new uint8_t [this->get_perBlockPoolTotalBlockSize (ui32) * get_perBlockPoolCacheSize (ui32)];

		BTREEDATA_ASSERT (m_ppPoolCaches[ui32] != NULL, "CBTreeFileIO::CBTreeFileIO: insufficient memory!");
	}

	if (nBlockSize == 0)
	{
#if defined (WIN32)

		uint32_t		nBytesPerSector, nSectorsPerCluster, dummy0, dummy1;

		GetDiskFreeSpaceA (m_pClDataLayerProperties->get_pathname (), (LPDWORD) &nSectorsPerCluster, (LPDWORD) &nBytesPerSector, (LPDWORD) &dummy0, (LPDWORD) &dummy1);

		nBlockSize = nSectorsPerCluster * nBytesPerSector;

#elif defined (LINUX)

		struct statfs		sFsParams;

		statfs (rDataLayerProperties.get_pathname (), &sFsParams);

		nBlockSize = sFsParams.f_bsize;

#else

		nBlockSize = 4096;

#endif
	}

	this->setup (nBlockSize);

	m_pFile = new CFileMem ();
	
	m_pFile->setMode (CFileMem::VIEW_RWCOPIED);
	
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

		bRslt = m_pFile->open ((char *) m_strTempFile.c_str (), CFileMem::modeCreate | CFileMem::modeReadWrite | CFileMem::osRandomAccess, true);

		nFails++;

		BTREEDATA_ASSERT (nFails <= 15, "CBTreeFileIO::CBTreeFileIO: Failed to create file!");
	} while (!bRslt);

	m_pFile->setCacheCfg (m_pClDataLayerProperties->get_num_of_log2_cache_lines (), m_pClDataLayerProperties->get_num_of_log2_bytes_per_cache_line ());
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::~CBTreeFileIO ()
{
	uint32_t		ui32;

	m_pFile->close ();

	delete (m_pFile);

	delete m_pClDataLayerProperties;

	delete [] m_pnPerBlockPoolCacheMaskes;
	
	for (ui32 = 0; ui32 < this->m_nNumDataPools; ui32++)
	{
		delete [] m_ppnPerBlockPoolCacheNode[ui32];
		delete [] m_ppbPerBlockPoolCacheValid[ui32];
		delete [] m_ppnPerBlockPoolCacheLen[ui32];
		delete [] m_ppPoolCaches[ui32];
	}

	delete [] m_ppnPerBlockPoolCacheNode;
	delete [] m_ppbPerBlockPoolCacheValid;
	delete [] m_ppnPerBlockPoolCacheLen;
	delete [] m_ppPoolCaches;
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

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
uint32_t CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_perBlockPoolCacheSize (uint32_t nPool)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeFileIO::get_perBlockPoolCacheSize: nPool exceeds available block pools!");
#endif

	return (m_pnPerBlockPoolCacheMaskes[nPool] + 1);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
uint32_t CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_perBlockPoolCacheMask (uint32_t nPool)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeFileIO::get_perBlockPoolCacheMask: nPool exceeds available block pools!");
#endif

	return (m_pnPerBlockPoolCacheMaskes[nPool]);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
uint8_t* CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_perBlockPoolCacheBaseAddress (uint32_t nPool)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeFileIO::get_perBlockPoolCacheBaseAddress: nPool exceeds available block pools!");
#endif

	return (m_ppPoolCaches[nPool]);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_nodeiter CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_perBlockPoolCacheNode (uint32_t nPool, uint32_t nLine)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeFileIO::get_perBlockPoolCacheNode: nPool exceeds available block pools!");
	BTREEDATA_ASSERT (nLine < get_perBlockPoolCacheSize (nPool), "CBTreeFileIO::get_perBlockPoolCacheNode: nLine exceeds cache size!");
#endif

	_t_nodeiter		*pnPerBlockPoolCacheNodePool = m_ppnPerBlockPoolCacheNode[nPool];

	return (pnPerBlockPoolCacheNodePool[nLine]);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
bool CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_perBlockPoolCacheValid (uint32_t nPool, uint32_t nLine)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeFileIO::get_perBlockPoolCacheValid: nPool exceeds available block pools!");
	BTREEDATA_ASSERT (nLine < get_perBlockPoolCacheSize (nPool), "CBTreeFileIO::get_perBlockPoolCacheValid: nLine exceeds cache size!");
#endif

	bool			*pbPerBlockPoolCacheValidPool = m_ppbPerBlockPoolCacheValid[nPool];

	return (pbPerBlockPoolCacheValidPool[nLine]);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_offsettype CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_perBlockPoolOffset (uint32_t nPool)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeFileIO::get_perBlockPoolOffset: nPool exceeds available block pools!");
#endif

	return (this->m_pnPerBlockPoolOffset[nPool]);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_addresstype CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_perBlockPoolAddr (uint32_t nPool, _t_nodeiter nNode)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeFileIO::get_perBlockPoolAddr: nPool exceeds available block pools!");
#endif

	_t_addresstype		nAddr = this->get_nodeAddr (nNode);

	nAddr += this->get_perBlockPoolOffset (nPool);

	return (nAddr);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
uint32_t CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_perBlockPoolCacheLength (uint32_t nPool, _t_nodeiter nNode)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeFileIO::set_perBlockPoolCacheLength: nPool exceeds available block pools!");
#endif

	const uint32_t				uCacheMask = get_perBlockPoolCacheMask (nPool);
	uint32_t					nLine = (uint32_t) nNode & uCacheMask;
	uint32_t					*pnPerBlockPoolCacheLen = m_ppnPerBlockPoolCacheLen[nPool];
	
	return (pnPerBlockPoolCacheLen[nLine]);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_perBlockPoolCacheNode (uint32_t nPool, uint32_t nLine, _t_nodeiter nNode)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeFileIO::set_perBlockPoolCacheLength: nPool exceeds available block pools!");
	BTREEDATA_ASSERT (nLine < get_perBlockPoolCacheSize (nPool), "CBTreeFileIO::get_perBlockPoolCacheNode: nLine exceeds cache size!");
#endif

	_t_nodeiter		*pnPerBlockPoolCacheNodePool = m_ppnPerBlockPoolCacheNode[nPool];

	pnPerBlockPoolCacheNodePool[nLine] = nNode;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_perBlockPoolCacheLength (uint32_t nPool, _t_nodeiter nNode, uint32_t nLen)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeFileIO::set_perBlockPoolCacheLength: nPool exceeds available block pools!");
	BTREEDATA_ASSERT (nLen <= this->get_perBlockPoolTotalBlockSize (nPool), "CBTreeFileIO::set_perBlockPoolCacheLength: nLen exceeds cache line size");
#endif

	const uint32_t				uCacheMask = get_perBlockPoolCacheMask (nPool);
	uint32_t					nLine = (uint32_t) nNode & uCacheMask;
	uint32_t					*pnPerBlockPoolCacheLen = m_ppnPerBlockPoolCacheLen[nPool];
	bool						*pbPerBlockPoolCacheValid = m_ppbPerBlockPoolCacheValid[nPool];
	
	pnPerBlockPoolCacheLen[nLine] = nLen;
	pbPerBlockPoolCacheValid[nLine] = true;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_perBlockPoolCacheToInvalid (uint32_t nPool, uint32_t nLine)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeFileIO::get_perBlockPoolCacheValid: nPool exceeds available block pools!");
	BTREEDATA_ASSERT (nLine < get_perBlockPoolCacheSize (nPool), "CBTreeFileIO::get_perBlockPoolCacheValid: nLine exceeds cache size!");
#endif

	bool			*pbPerBlockPoolCacheValidPool = m_ppbPerBlockPoolCacheValid[nPool];

	pbPerBlockPoolCacheValidPool[nLine] = false;
}

/*

get_pooledData - get pooled data

nPool[in]	- specifies data pool ID
node[in]	- specifies linear node address of data pool
len[in]		- specifies number of entries present in node
entry[in]	- specifies entry to be returned
pData[out]	- pointer to return value

*/

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_pooledData (uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, _t_subnodeiter nEntry, void *pData)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeFileIO::get_pooledData: nPool exceeds avilable block data pools");
	BTREEDATA_ASSERT (nLen <= (this->get_perBlockPoolTotalBlockSize (nPool) / this->get_perBlockPoolRawEntrySize (nPool)), "CBTreeFileIO::get_pooledData: nLen exceeds cache line size");
#endif

	uint64_t					nAddr;
	const uint32_t				uCacheMask = get_perBlockPoolCacheMask (nPool);
	uint32_t					nLine = (uint32_t) nNode & uCacheMask;
	uint8_t						*pBuffer;
	const uint32_t				nRawEntrySize = this->get_perBlockPoolRawEntrySize (nPool);

	nLen *= nRawEntrySize;
	nEntry *= nRawEntrySize;

	// get pool base address
	pBuffer = get_perBlockPoolCacheBaseAddress (nPool);
	
	// move pointer to block address within selected pool
	pBuffer = &(pBuffer[nLine * this->get_perBlockPoolTotalBlockSize (nPool)]);
	
	// if data is not cached ...
	if ((get_perBlockPoolCacheNode (nPool, nLine) != nNode) || !(get_perBlockPoolCacheValid (nPool, nLine)))
	{
		// ... then load data into cache
	
		// if cache freeze is effective ...
		if (this->m_bCacheFreeze)
		{
			// ... then directly read data from data layer
			nAddr = this->get_perBlockPoolAddr (nPool, nNode);
		
			m_pFile->memcpy (pData, nAddr + nEntry, this->get_perBlockPoolRawEntrySize (nPool));
	
			// and exit
			return;
		}
	
		// if cache already contains data ...
		if (get_perBlockPoolCacheValid (nPool, nLine))
		{
			// ... then save current data to mass memory
			nAddr = this->get_perBlockPoolAddr (nPool, get_perBlockPoolCacheNode (nPool, nLine));
	
			m_pFile->memcpy (nAddr, pBuffer, get_perBlockPoolCacheLength (nPool, nNode));
		}
	
		// update cache
		nAddr = this->get_perBlockPoolAddr (nPool, nNode);
		
		m_pFile->memcpy (pBuffer, nAddr, nLen);
	
		set_perBlockPoolCacheNode (nPool, nLine, nNode);
		set_perBlockPoolCacheLength (nPool, nNode, nLen);

#if defined (USE_PERFORMANCE_COUNTERS)
		m_anMissCtrs[PERFCTR_NODEDATA_DATA]++;
#endif
	}
#if defined (USE_PERFORMANCE_COUNTERS)
	else
	{
		m_anHitCtrs[PERFCTR_NODEDATA_DATA]++;
	}
#endif
										
	// copy cached entry to return location
	memcpy (pData, (void *) &(((char *)pBuffer)[nEntry]), this->get_perBlockPoolRawEntrySize (nPool));
}

/*

set_pooledData - set pooled data

nPool[in]	- specifies data pool ID
node[in]	- specifies linear node address of data pool
len[in]		- specifies length of node
entry[in]	- specifies linear entry of node
pData[in]	- pointer to input data

*/

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_pooledData (uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, _t_subnodeiter nEntry, void *pData)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeFileIO::set_pooledData: nPool exceeds avilable block data pools");
	BTREEDATA_ASSERT (nLen <= (this->get_perBlockPoolTotalBlockSize (nPool) / this->get_perBlockPoolRawEntrySize (nPool)), "CBTreeFileIO::set_pooledData: nLen exceeds cache line size");
#endif

	BTREEDATA_ASSERT (this->m_bCacheFreeze == false, "CBTreeFileIO::set_pooledData: data cannot be modified while cache freeze is in effect");

	uint64_t					nAddr;
	const uint32_t				uCacheMask = this->get_perBlockPoolCacheMask (nPool);
	uint32_t					nLine = (uint32_t) nNode & uCacheMask;
	uint8_t						*pBuffer;
	const uint32_t				nRawEntrySize = this->get_perBlockPoolRawEntrySize (nPool);

	nLen *= nRawEntrySize;
	nEntry *= nRawEntrySize;

										// get pool base address
										pBuffer = get_perBlockPoolCacheBaseAddress (nPool);

										// move pointer to block address within selected pool
										pBuffer = &(pBuffer[nLine * this->get_perBlockPoolTotalBlockSize (nPool)]);

										// if cache contains data of a different node or invalid data ...
										if (!(get_perBlockPoolCacheValid (nPool, nLine)) || (get_perBlockPoolCacheNode (nPool, nLine) != nNode))
										{
											// ... then load node in question

											// if cache contains valid data ...
											if (get_perBlockPoolCacheValid (nPool, nLine))
											{
												// ... then save cached data to mass memory
												nAddr = this->get_perBlockPoolAddr (nPool, get_perBlockPoolCacheNode (nPool, nLine));

												m_pFile->memcpy (nAddr, pBuffer, get_perBlockPoolCacheLength (nPool, nNode));
											}

											// load node to cache
											nAddr = this->get_perBlockPoolAddr (nPool, nNode);
											
											m_pFile->memcpy (pBuffer, nAddr, nLen);

											set_perBlockPoolCacheNode (nPool, nLine, nNode);

#if defined (USE_PERFORMANCE_COUNTERS)
											m_anMissCtrs[PERFCTR_NODEDATA_DATA]++;
#endif
										}
#if defined (USE_PERFORMANCE_COUNTERS)
										else
										{
											m_anHitCtrs[PERFCTR_NODEDATA_DATA]++;
										}
#endif
										// copy data to cache
										memcpy ((void *) &(((char *)pBuffer)[nEntry]), pData, this->get_perBlockPoolRawEntrySize (nPool));

										set_perBlockPoolCacheLength (nPool, nNode, nLen);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_pooledData (uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, void *pData)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeFileIO::get_pooledData (no entry): nPool exceeds avilable block data pools");
	BTREEDATA_ASSERT (nLen <= (this->get_perBlockPoolTotalBlockSize (nPool) / this->get_perBlockPoolRawEntrySize (nPool)), "CBTreeFileIO::get_pooledData (no entry): nLen exceeds cache line size");
#endif

	uint64_t					nAddr;
	const uint32_t				uCacheMask = this->get_perBlockPoolCacheMask (nPool);
	uint32_t					nLine = (uint32_t) nNode & uCacheMask;
	uint8_t						*pBuffer;

	nLen *= this->get_perBlockPoolRawEntrySize (nPool);
	
	// get pool base address
	pBuffer = get_perBlockPoolCacheBaseAddress (nPool);

	// move pointer to block address within selected pool
	pBuffer = &(pBuffer[nLine * this->get_perBlockPoolTotalBlockSize (nPool)]);

	// if data is not cached ...
	if ((get_perBlockPoolCacheNode (nPool, nLine) != nNode) || !(get_perBlockPoolCacheValid (nPool, nLine)))
	{
		// ... then load data into cache

		// if cache freeze is effective ...
		if (this->m_bCacheFreeze)
		{
			// ... then directly read data from data layer
			nAddr = this->get_perBlockPoolAddr (nPool, nNode);
		
			m_pFile->memcpy (pData, nAddr, nLen);

			// and exit
			return;
		}

		// if cache already contains data ...
		if (get_perBlockPoolCacheValid (nPool, nLine))
		{
			// ... then save current data to mass memory
			nAddr = this->get_perBlockPoolAddr (nPool, get_perBlockPoolCacheNode (nPool, nLine));

			m_pFile->memcpy (nAddr, pBuffer, get_perBlockPoolCacheLength (nPool, nNode));
		}

		// update cache
		nAddr = this->get_perBlockPoolAddr (nPool, nNode);
		
		m_pFile->memcpy (pBuffer, nAddr, nLen);

		set_perBlockPoolCacheNode (nPool, nLine, nNode);
		set_perBlockPoolCacheLength (nPool, nNode, nLen);

#if defined (USE_PERFORMANCE_COUNTERS)
		m_anMissCtrs[PERFCTR_NODEDATA_DATA]++;
#endif
	}
#if defined (USE_PERFORMANCE_COUNTERS)
	else
	{
		m_anHitCtrs[PERFCTR_NODEDATA_DATA]++;
	}
#endif
										
	// copy cached entry to return location
	memcpy (pData, (void *) pBuffer, (size_t) nLen);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_pooledData (uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, void *pData)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeFileIO::set_pooledData (no entry): nPool exceeds avilable block data pools");
	BTREEDATA_ASSERT (nLen <= (this->get_perBlockPoolTotalBlockSize (nPool) / this->get_perBlockPoolRawEntrySize (nPool)), "CBTreeFileIO::set_pooledData (no entry): nLen exceeds cache line size");
#endif

	BTREEDATA_ASSERT (this->m_bCacheFreeze == false, "CBTreeFileIO::set_pooledData (no entry): data cannot be modified while cache freeze is in effect");

	uint64_t					nAddr;
	const uint32_t				uCacheMask = this->get_perBlockPoolCacheMask (nPool);
	uint32_t					nLine = (uint32_t) nNode & uCacheMask;
	uint8_t						*pBuffer;

	nLen *= this->get_perBlockPoolRawEntrySize (nPool);
	
	// get pool base address
	pBuffer = get_perBlockPoolCacheBaseAddress (nPool);

	// move pointer to block address within selected pool
	pBuffer = &(pBuffer[nLine * this->get_perBlockPoolTotalBlockSize (nPool)]);

	// if cache contains data of a different node or invalid data ...
	if (!(get_perBlockPoolCacheValid (nPool, nLine)) || (get_perBlockPoolCacheNode (nPool, nLine) != nNode))
	{
		// ... then load node in question

		// if cache contains valid data ...
		if (get_perBlockPoolCacheValid (nPool, nLine))
		{
			// ... then save cached data to mass memory
			nAddr = this->get_perBlockPoolAddr (nPool, get_perBlockPoolCacheNode (nPool, nLine));

			m_pFile->memcpy (nAddr, pBuffer, get_perBlockPoolCacheLength (nPool, nNode));
		}

		// load node to cache
		nAddr = this->get_perBlockPoolAddr (nPool, nNode);
		
		m_pFile->memcpy (pBuffer, nAddr, nLen);

		set_perBlockPoolCacheNode (nPool, nLine, nNode);

#if defined (USE_PERFORMANCE_COUNTERS)
		m_anMissCtrs[PERFCTR_NODEDATA_DATA]++;
#endif
	}
#if defined (USE_PERFORMANCE_COUNTERS)
	else
	{
		m_anHitCtrs[PERFCTR_NODEDATA_DATA]++;
	}
#endif

	if (pBuffer != (void *) pData)
	{
		// copy data to cache
		memcpy ((void *) pBuffer, pData, (size_t) nLen);
	}

	if (this->m_bWriteThrough)
	{
		m_pFile->memcpy (this->get_perBlockPoolAddr (nPool, nNode), pBuffer, (uint64_t) nLen);
	}

	set_perBlockPoolCacheLength (nPool, nNode, nLen);
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
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::insert_dataIntoPool (uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nNodeLen, _t_subnodeiter nOffset, _t_subnodeiter nDataLen, void *pData)
{
	_t_addresstype				nAddr;
	const uint32_t				uCacheMask = get_perBlockPoolCacheMask (nPool);
	uint32_t					nLine = (uint32_t)nNode & uCacheMask;
	uint8_t						*pBuffer = get_perBlockPoolCacheBaseAddress (nPool);
	const uint32_t				nRawEntrySize = this->get_perBlockPoolRawEntrySize (nPool);

	pBuffer = &(pBuffer[nLine * this->get_perBlockPoolTotalBlockSize (nPool)]);

	nOffset *= nRawEntrySize;
	nNodeLen *= nRawEntrySize;
	nDataLen *= nRawEntrySize;

	BTREEDATA_ASSERT (nNodeLen <= this->get_perBlockPoolTotalBlockSize (nPool), "CBTreeFileIO::insert_dataInDataBuffer: length exceeds node size");

	if (!(get_perBlockPoolCacheValid (nPool, nLine)) || (get_perBlockPoolCacheNode (nPool, nLine) != nNode))
	{
		if (get_perBlockPoolCacheValid (nPool, nLine))
		{
			uint64_t	nCacheNode = get_perBlockPoolCacheNode (nPool, nLine);
			
			nAddr = this->get_perBlockPoolAddr (nPool, (_t_nodeiter) nCacheNode);

			m_pFile->memcpy (nAddr, pBuffer, get_perBlockPoolCacheLength (nPool, (_t_nodeiter) nCacheNode));
		}

		nAddr = this->get_perBlockPoolAddr (nPool, nNode);
		
		m_pFile->memcpy (pBuffer, nAddr, nNodeLen);

		set_perBlockPoolCacheNode (nPool, nLine, nNode);

#if defined (USE_PERFORMANCE_COUNTERS)
		m_anMissCtrs[PERFCTR_NODEDATA_DATA]++;
#endif
	}
#if defined (USE_PERFORMANCE_COUNTERS)
	else
	{
		m_anHitCtrs[PERFCTR_NODEDATA_DATA]++;
	}
#endif

	set_perBlockPoolCacheLength (nPool, nNode, nNodeLen + nDataLen);

#if defined (_DEBUG)

	BTREE_ASSERT (nOffset <= nNodeLen, "CBTreeFileIO<>::insert_dataIntoPool: ERROR: offset exceeds node size!");

#endif

	memmove ((void *) &(((char *)pBuffer)[nOffset + nDataLen]), &(((char *)pBuffer)[nOffset]), (size_t)(nNodeLen - nOffset));
	memcpy ((void *) &(((char *)pBuffer)[nOffset]), pData, (size_t)nDataLen);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_size (_t_nodeiter nMaxNodes)
{
	if (nMaxNodes == this->m_nMaxNodes)
	{
		return;
	}

	_t_addresstype		nSize;
	bool				bRslt;

	nSize = this->get_nodeAddr (nMaxNodes + 1);

	bRslt = m_pFile->setLength (nSize);

	BTREEDATA_ASSERT (bRslt != false, "CBTreeFileIO::set_size: setLength failed!");

	this->m_nMaxNodes = nMaxNodes;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::unload ()
{
	uint32_t			ui32;
	uint8_t				*pBuffer;
	uint32_t			nCacheSize;
	uint32_t			nPool;
	_t_nodeiter			nNode;

	this->set_writeThrough (true);
	
	for (nPool = 0; nPool < this->m_nNumDataPools; nPool++)
	{
		nCacheSize = get_perBlockPoolCacheSize (nPool);

		for (ui32 = 0UL; ui32 < nCacheSize; ui32++)
		{
			if (get_perBlockPoolCacheValid (nPool, ui32))
			{
				pBuffer = get_perBlockPoolCacheBaseAddress (nPool);
				pBuffer = &(pBuffer[ui32 * this->get_perBlockPoolTotalBlockSize (nPool)]);

				nNode = get_perBlockPoolCacheNode (nPool, ui32);

				set_pooledData (nPool, nNode, get_perBlockPoolCacheLength (nPool, nNode) / this->get_perBlockPoolRawEntrySize (nPool), (void *) pBuffer);

				set_perBlockPoolCacheToInvalid (nPool, ui32);
			}
		}
	}

	this->set_writeThrough (false);

	m_pFile->unload ();
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::unload_from_cache (_t_nodeiter nNode)
{
	uint32_t		ui32;

	for (ui32 = 0; ui32 < this->m_nNumDataPools; ui32++)
	{
		const uint32_t				uCacheMask = this->get_perBlockPoolCacheMask (ui32);
		uint32_t					nLine = (uint32_t) nNode & uCacheMask;
		
		if ((get_perBlockPoolCacheValid (ui32, nLine)) && (get_perBlockPoolCacheNode (ui32, nLine) == nNode))
		{
			set_perBlockPoolCacheToInvalid (ui32, nLine);
		}
	}
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
bool CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::is_dataCached (uint32_t nPool, _t_nodeiter nNode)
{
	const uint32_t	uCacheMask = get_perBlockPoolCacheMask (nPool);
	bool			bRetval = false;
	uint32_t		nLine = (uint32_t) nNode & uCacheMask;
	
	bRetval = (get_perBlockPoolCacheNode (nPool, nLine) == nNode);

	return (bRetval);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
uint32_t CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_node_buffer_cache_size ()
{
	return (get_perBlockPoolCacheSize (0));
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
		ofs << "<td>m_psDataPools->nCacheVectorSize</td>" << endl;

		for (ui32 = 0; ui32 < this->m_nNumDataPools; ui32++)
		{
			ofs << "<td>" << this->m_psDataPools[ui32].nCacheVectorSize << "</td>" << endl;
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
		ofs << "<td>m_pnPerBlockPoolCacheMaskes</td>" << endl;

		for (ui32 = 0; ui32 < this->m_nNumDataPools; ui32++)
		{
			ofs << "<td>0x" << hex << m_pnPerBlockPoolCacheMaskes[ui32] << dec << "</td>" << endl;
		}

		ofs << "</tr>" << endl;

		ofs << "<tr>" << endl;
		ofs << "<td>m_pnPerBlockPoolOffset</td>" << endl;

		for (ui32 = 0; ui32 < this->m_nNumDataPools; ui32++)
		{
			ofs << "<td>" << this->m_pnPerBlockPoolOffset[ui32] << "</td>" << endl;
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
							_t_addresstype	nAddr = this->get_perBlockPoolAddr (uk32, ui64);
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

								nMaxPoolLen = this->get_perBlockPoolTotalBlockSize (uk32);

								pb = new uint8_t [nMaxPoolLen];

								BTREEDATA_ASSERT (pb != NULL, "CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::showdump: insufficient memory!");

								get_pooledData (uk32, ui64, nMaxPoolLen / this->get_perBlockPoolRawEntrySize (uk32), (void *) pb);
							
								for (ul32 = 0; ul32 < nMaxPoolLen; ul32++)
								{
									ofs << hex << setfill ('0') << setw (2) << (uint32_t) pb[ul32] << dec;

									if (((ul32 + 1) % this->get_perBlockPoolRawEntrySize (uk32)) == 0)
									{
										ofs << "<br>" << endl;
									}
									else if (((ul32 + 1) % 4) == 0)
									{
										ofs << " ";
									}
								}

								delete [] pb;

								ofs << "</font>";

								ofs << "<br>";
							}
							
							uint32_t	ul32;
							uint8_t		b;

							if (m_pFile != NULL)
							{
								ofs << "<font color=\"#999999\">";
								
								for (ul32 = 0; ul32 < this->get_perBlockPoolTotalBlockSize (uk32); ul32++)
								{
									m_pFile->memcpy ((void *) &b, nAddr + ul32, 1ULL);

									ofs << hex << setfill ('0') << setw (2) << (uint32_t) b << dec;

									if (((ul32 + 1) % this->get_perBlockPoolRawEntrySize (uk32)) == 0)
									{
										ofs << "<br>";
									}
									else if (((ul32 + 1) % 4) == 0)
									{
										ofs << " ";
									}
								}

								ofs << "</font>";
							}
							
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
_t_addresstype CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_blockAddr (_t_nodeiter nNode)
{
	_t_addresstype		nRetval;

	nRetval = (_t_addresstype) ((nNode >> this->m_nNodesPerBlockVectorSize) * this->m_nBlockSize);

	return (nRetval);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_offsettype CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_poolOffset ()
{
	return ((_t_offsettype) 0);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_addresstype CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_nodeAddr (_t_nodeiter nNode)
{
	_t_addresstype		nRetval;

	nRetval = this->get_blockAddr (nNode);
	nRetval += (nNode & (this->m_nNodesPerBlock - 1)) * this->m_nAlignedNodeSize;

	return (nRetval);
}

#endif // BTREEFILEIO_CPP
