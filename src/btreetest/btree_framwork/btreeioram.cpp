/************************************************************
**
** file:	btreeioram.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** RAM data layer class.
**
************************************************************/

#ifndef	BTREERAMIO_CPP
#define	BTREERAMIO_CPP

#include "btreeioram.h"

using namespace std;

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeRAMIO
(
	CBTreeIOpropertiesRAM &rDataLayerProperties, 
	uint32_t nBlockSize, 
	_t_subnodeiter nNodeSize, 
	uint32_t nNumDataPools, 
	CBTreeIOperBlockPoolDesc_t *psDataPools
)
	:	CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype> (nBlockSize, nNodeSize, nNumDataPools, psDataPools)
{
	m_pClDataLayerProperties = new CBTreeIOpropertiesRAM (rDataLayerProperties);

	BTREEDATA_ASSERT (m_pClDataLayerProperties != NULL, "CBTreeRAMIO::CBTreeRAMIO: insufficient memory!");

	if (nBlockSize == 0)
	{
#if defined (WIN32)
		{
			DWORD									nBufferSize = 0;
			uint32_t								i;
			uint32_t								nEntries;
			SYSTEM_LOGICAL_PROCESSOR_INFORMATION	*psBuffer;

			GetLogicalProcessorInformation (NULL, &nBufferSize);

			nEntries = nBufferSize / sizeof (SYSTEM_LOGICAL_PROCESSOR_INFORMATION);

			psBuffer = new SYSTEM_LOGICAL_PROCESSOR_INFORMATION [nEntries];

			BTREEDATA_ASSERT (psBuffer != NULL, "CBTreeRAMIO::CBTreeRAMIO: insufficient memory!");

			GetLogicalProcessorInformation(psBuffer, &nBufferSize);

			for (i = 0; i < nEntries; i++)
			{
			    if ((psBuffer[i].Relationship == RelationCache) && 
					(psBuffer[i].Cache.Level == 1))
				{
			        nBlockSize = psBuffer[i].Cache.LineSize;

			        break;
			    }
			}

			delete [] psBuffer;
		}
#elif defined (LINUX)
		{
			const char	*pszCPUcacheInfoFileName = "/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size";
			std::string	aszMsg;
			FILE		*pf;
			
			pf = fopen(pszCPUcacheInfoFileName, "r");

			aszMsg = "CBTreeRAMIO::CBTreeRAMIO: Failed to open file: ";
			aszMsg += pszCPUcacheInfoFileName;

			BTREEDATA_ASSERT (pf != NULL, aszMsg.c_str ());
			
			fscanf (pf, "%u", &nBlockSize);
			
			fclose (pf);
		}
#else

		nBlockSize = get_alignedDataOffsets ();

#endif
	}

	this->setup (nBlockSize);

	m_pMemory = NULL;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::~CBTreeRAMIO ()
{
	if (m_pMemory != NULL)
	{
		free (m_pMemory);
	}

	delete m_pClDataLayerProperties;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_performance_counters (uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR])
{
	memset ((void *) &rHitCtrs, 0, sizeof (rHitCtrs));
	memset ((void *) &rMissCtrs, 0, sizeof (rMissCtrs));
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
void CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_pooledData (uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, _t_subnodeiter nEntry, void *pData)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeRAMIO::get_pooledData: nPool exceeds avilable block data pools");
	BTREEDATA_ASSERT (nLen <= (this->get_perBlockPoolTotalBlockSize (nPool) / this->get_perBlockPoolRawEntrySize (nPool)), "CBTreeRAMIO::get_pooledData: nLen exceeds cache line size");
#endif

	uint64_t					nAddr;

	nLen *= this->get_perBlockPoolRawEntrySize (nPool);
	nEntry *= this->get_perBlockPoolRawEntrySize (nPool);

	nAddr = this->get_perBlockPoolAddr (nPool, nNode);

	// copy data to return location
	memcpy (pData, (void *) &(((char *)m_pMemory)[nAddr + nEntry]), this->get_perBlockPoolRawEntrySize (nPool));
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
void CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_pooledData (uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, _t_subnodeiter nEntry, void *pData)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeRAMIO::set_pooledData: nPool exceeds avilable block data pools");
	BTREEDATA_ASSERT (nLen <= (this->get_perBlockPoolTotalBlockSize (nPool) / this->get_perBlockPoolRawEntrySize (nPool)), "CBTreeRAMIO::set_pooledData: nLen exceeds cache line size");
#endif

	BTREEDATA_ASSERT (this->m_bCacheFreeze == false, "CBTreeRAMIO::set_pooledData: data cannot be modified while cache freeze is in effect");

	uint64_t					nAddr;

	nLen *= this->get_perBlockPoolRawEntrySize (nPool);
	nEntry *= this->get_perBlockPoolRawEntrySize (nPool);

	nAddr = this->get_perBlockPoolAddr (nPool, nNode);

	// copy data to memory
	memcpy ((void *) &(((char *)m_pMemory)[nAddr + nEntry]), pData, this->get_perBlockPoolRawEntrySize (nPool));
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_pooledData (uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, void *pData)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeRAMIO::get_pooledData (no entry): nPool exceeds avilable block data pools");
	BTREEDATA_ASSERT (nLen <= (this->get_perBlockPoolTotalBlockSize (nPool) / this->get_perBlockPoolRawEntrySize (nPool)), "CBTreeRAMIO::get_pooledData (no entry): nLen exceeds cache line size");
#endif

	uint64_t					nAddr;

	nLen *= this->get_perBlockPoolRawEntrySize (nPool);
	
	nAddr = this->get_perBlockPoolAddr (nPool, nNode);

	// copy data to return location
	memcpy (pData, (void *) &(((char *)m_pMemory)[nAddr]), (size_t) nLen);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_pooledData (uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, void *pData)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeRAMIO::set_pooledData (no entry): nPool exceeds avilable block data pools");
	BTREEDATA_ASSERT (nLen <= (this->get_perBlockPoolTotalBlockSize (nPool) / this->get_perBlockPoolRawEntrySize (nPool)), "CBTreeRAMIO::set_pooledData (no entry): nLen exceeds cache line size");
#endif

	BTREEDATA_ASSERT (this->m_bCacheFreeze == false, "CBTreeRAMIO::set_pooledData (no entry): data cannot be modified while cache freeze is in effect");

	uint64_t					nAddr;

	nLen *= this->get_perBlockPoolRawEntrySize (nPool);
	
	nAddr = this->get_perBlockPoolAddr (nPool, nNode);

	// copy data to memory
	memcpy ((void *) &(((char *)m_pMemory)[nAddr]), pData, (size_t) nLen);
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
void CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::insert_dataIntoPool (uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nNodeLen, _t_subnodeiter nOffset, _t_subnodeiter nDataLen, void *pData)
{
	_t_addresstype				nAddr;
	
	nOffset *= this->get_perBlockPoolRawEntrySize (nPool);
	nNodeLen *= this->get_perBlockPoolRawEntrySize (nPool);
	nDataLen *= this->get_perBlockPoolRawEntrySize (nPool);

	BTREEDATA_ASSERT (nNodeLen <= this->get_perBlockPoolTotalBlockSize (nPool), "CBTreeRAMIO::insert_dataInDataBuffer: length exceeds node size");

	nAddr = this->get_perBlockPoolAddr (nPool, nNode);

	memmove ((void *) &(((char *)m_pMemory)[nAddr + nOffset + nDataLen]), &(((char *)m_pMemory)[nAddr + nOffset]), (size_t)(nNodeLen - nOffset));
	memcpy ((void *) &(((char *)m_pMemory)[nAddr + nOffset]), pData, (size_t)nDataLen);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_size (_t_nodeiter nMaxNodes)
{
	if (nMaxNodes == this->m_nMaxNodes)
	{
		return;
	}

	_t_addresstype		nSize;

#if defined (_DEBUG)
	_t_addresstype		nOrgSize;

	_t_nodeiter			bNotInit = (m_pMemory == NULL) ? 0 : 1;
#endif

	nSize = this->get_nodeAddr (nMaxNodes + 1);

	m_pMemory = realloc (m_pMemory, (size_t) nSize);

	BTREEDATA_ASSERT (m_pMemory != NULL, "CBTreeRAMIO::set_size: insufficient memory");

#if defined (_DEBUG)

	if (nMaxNodes > this->m_nMaxNodes)
	{
		nOrgSize = this->get_nodeAddr (this->m_nMaxNodes + bNotInit);

		memset ((void *) &(((uint8_t *) (m_pMemory))[nOrgSize]), 0xEE, (size_t) (nSize - nOrgSize));
	}

#endif

	this->m_nMaxNodes = nMaxNodes;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::unload ()
{
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::unload_from_cache (_t_nodeiter nNode)
{
	nNode;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
bool CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::is_dataCached (uint32_t nPool, _t_nodeiter nNode)
{
	bool	bRetval = false;

	return (bRetval);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
uint32_t CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_node_buffer_cache_size ()
{
	return (0);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::showdump	(std::ofstream &ofs, _t_nodeiter nTreeSize, char *pAlloc)
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
			for (ui32 = 0; ui32 < (nTreeSize / this->m_nNodesPerBlock); ui32++)
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
							bool			bIsCached = this->is_dataCached (uk32, ui64);

							if (m_pMemory != NULL)
							{
								bIsCached = true;
							}

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

								BTREEDATA_ASSERT (pb != NULL, "CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::showdump: insufficient memory!");

								this->get_pooledData (uk32, ui64, nMaxPoolLen / this->get_perBlockPoolRawEntrySize (uk32), (void *) pb);
							
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
_t_addresstype CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_blockAddr (_t_nodeiter nNode)
{
	_t_addresstype		nRetval;

	nRetval = (_t_addresstype) ((nNode >> this->m_nNodesPerBlockVectorSize) * this->m_nBlockSize);

	return (nRetval);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_offsettype CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_poolOffset ()
{
	return ((_t_offsettype) 0);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_addresstype CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_nodeAddr (_t_nodeiter nNode)
{
	_t_addresstype		nRetval;

	nRetval = this->get_blockAddr (nNode);
	nRetval += (nNode & (this->m_nNodesPerBlock - 1)) * this->m_nAlignedNodeSize;

	return (nRetval);
}

#endif // BTREERAMIO_CPP
