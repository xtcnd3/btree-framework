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
	_t_addresstype nBlockSize, 
	_t_subnodeiter nNodeSize, 
	uint32_t nNumDataPools, 
	CBTreeIOperBlockPoolDesc_t *psDataPools
)
	:	CBTreeLinearIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype> (nBlockSize, nNumDataPools, psDataPools)
{
	m_pClDataLayerProperties = new CBTreeIOpropertiesRAM (rDataLayerProperties);

	BTREEDATA_ASSERT (m_pClDataLayerProperties != NULL, "CBTreeRAMIO::CBTreeRAMIO: insufficient memory!");
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::~CBTreeRAMIO ()
{
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
nNode[in]	- specifies linear node address of data pool
nEntry[in]	- specifies entry to be returned

*/

template<class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
template<class _t_dl_data>
_t_dl_data* CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_pooledData (uint32_t nPool, _t_nodeiter nNode)
{
	_t_dl_data			*psData = (_t_dl_data *) this->get_node_base (nPool, nNode);

	return (psData);
}

/*

get_pooledData - get pooled data

nPool[in]	- specifies data pool ID
nNode[in]	- specifies linear node address of data pool
nEntry[in]	- specifies entry to be returned

*/

template<class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
template<class _t_dl_data>
_t_dl_data* CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_pooledData (uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nEntry)
{
	const uint8_t		*psPooledNode = this->template get_pooledData<uint8_t> (nPool, nNode);
	_t_dl_data			*psData = (_t_dl_data *) &(psPooledNode[nEntry * this->get_pool_entry_size (nPool)]);

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
void CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::insert_dataIntoPool (uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nNodeLen, _t_subnodeiter nOffset, _t_subnodeiter nDataLen, const _t_dl_data *pData)
{
#if defined (_DEBUG)

	BTREEDATA_ASSERT ((this->get_pool_entry_size (nPool) * nNodeLen) <= this->get_pool_total_size (nPool), "CBTreeRAMIO::insert_dataInDataBuffer: length exceeds node size");

#endif

	_t_dl_data		*psNodeData = this->template get_pooledData<_t_dl_data> (nPool, nNode);

	memmove ((void *) &(psNodeData[nOffset + nDataLen]), (void *) &(psNodeData[nOffset]), sizeof (*psNodeData) * (nNodeLen - nOffset));
	memcpy ((void *) &(psNodeData[nOffset]), (void *) pData, sizeof (*psNodeData) * nDataLen);
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

	_t_nodeiter			bNotInit;

#endif

	uint32_t			nPool;

	for (nPool = 0; nPool < this->m_nNumDataPools; nPool++)
	{
#if defined (_DEBUG)

		bNotInit = (this->m_ppsPools[nPool] == NULL) ? 0 : 1;

#endif

		nSize = (_t_addresstype) (this->get_pool_total_size (nPool) * (nMaxNodes + 1));

		this->m_ppsPools[nPool] = (uint8_t *) realloc ((void *) (this->m_ppsPools[nPool]), (size_t) nSize);

		BTREEDATA_ASSERT (this->m_ppsPools[nPool] != NULL, "CBTreeRAMIO::set_size: insufficient memory");

#if defined (_DEBUG)

		if (nMaxNodes > this->m_nMaxNodes)
		{
			nOrgSize = (_t_addresstype) (this->get_pool_total_size (nPool) * (this->m_nMaxNodes + bNotInit));

			memset ((void *) &(((this->m_ppsPools[nPool]))[nOrgSize]), 0xEE, (size_t) (nSize - nOrgSize));
		}

#endif
	}

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
void CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::showdump	(std::ofstream &ofs, _t_nodeiter nTreeSize, char *pAlloc)
{
	_t_nodeiter		ui64;
	uint32_t		ui32;

	this->set_cacheFreeze (true);
	{
		ofs << "<H1>data dump</H1>" << endl;
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
		
		ofs << "</table>" << endl;

		ofs << "<table>" << endl;

		for (ui64 = 0; ui64 < nTreeSize; ui64++)
		{
			ofs << "<tr>" << endl;
			ofs << "<td>" << endl;

			ofs << "<a name=\"node_dump_" << ui64 << "\">";
			ofs << "node: " << ui64;
			ofs << "</a>" << endl;

			ofs << "</td><td><a href=\"#node_" << ui64 << "\">tree<a/></td><td><a href=\"#node_list_" << ui64 << "\">list<a/></td>" << endl;
			ofs << "</tr>" << endl;

			{
				ofs << "<tr>" << endl;

				ofs << "<td></td><td>" << endl;

				uint32_t	uk32;

				for (uk32 = 0; uk32 < this->m_nNumDataPools; uk32++)
				{
					_t_addresstype	nAddr = (_t_addresstype) (this->get_pool_total_size (uk32) * ui64);
					bool			bIsCached = this->is_dataCached (uk32, ui64);

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

						pb = new uint8_t [nMaxPoolLen];

						BTREEDATA_ASSERT (pb != NULL, "CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::showdump: insufficient memory!");

						pb = this->template get_pooledData<uint8_t> (uk32, ui64);
					
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

						delete [] pb;

						ofs << "</font>";

						ofs << "<br>";
					}
					
					ofs << "</td>";
				}

				ofs << "</tr>" << endl;
			}
		}

		ofs << "</table>" << endl;
	}
	this->set_cacheFreeze (false);
}

#endif // BTREERAMIO_CPP
