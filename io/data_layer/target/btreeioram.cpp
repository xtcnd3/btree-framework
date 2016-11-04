/************************************************************
**
** file:	btreeioram.cpp
** author:	Andreas Steffens
** license:	LGPL v3
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

#include "io/properties/btreeramioprop.h"

template<class _t_datalayerproperties>
CBTreeRAMIO<_t_datalayerproperties>::CBTreeRAMIO
(
	_t_datalayerproperties &rDataLayerProperties, 
	typename _t_datalayerproperties::sub_node_iter_type nNodeSize, 
	uint32_t nNumDataPools, 
	CBTreeIOperBlockPoolDesc_t *psDataPools
)
	:	CBTreeLinearIO<_t_datalayerproperties> (nNumDataPools, psDataPools)
{
	m_pClDataLayerProperties = new _t_datalayerproperties (rDataLayerProperties);

	BTREEDATA_ASSERT (m_pClDataLayerProperties != NULL, "CBTreeRAMIO::CBTreeRAMIO: insufficient memory!");
}

template<class _t_datalayerproperties>
CBTreeRAMIO<_t_datalayerproperties>::~CBTreeRAMIO ()
{
	delete m_pClDataLayerProperties;
}

template<class _t_datalayerproperties>
void CBTreeRAMIO<_t_datalayerproperties>::get_performance_counters (uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR])
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

template<class _t_datalayerproperties>
template<class _t_dl_data>
_t_dl_data* CBTreeRAMIO<_t_datalayerproperties>::get_pooledData (uint32_t nPool, typename _t_datalayerproperties::node_iter_type nNode)
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

template<class _t_datalayerproperties>
template<class _t_dl_data>
_t_dl_data* CBTreeRAMIO<_t_datalayerproperties>::get_pooledData (uint32_t nPool, typename _t_datalayerproperties::node_iter_type nNode, typename _t_datalayerproperties::sub_node_iter_type nEntry)
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

template<class _t_datalayerproperties>
template<class _t_dl_data>
void CBTreeRAMIO<_t_datalayerproperties>::insert_dataIntoPool (uint32_t nPool, typename _t_datalayerproperties::node_iter_type nNode, typename _t_datalayerproperties::sub_node_iter_type nNodeLen, typename _t_datalayerproperties::sub_node_iter_type nOffset, typename _t_datalayerproperties::sub_node_iter_type nDataLen)
{
#if defined (_DEBUG)

	BTREEDATA_ASSERT ((this->get_pool_entry_size (nPool) * nNodeLen) <= this->get_pool_total_size (nPool), "CBTreeRAMIO::insert_dataInDataBuffer: length exceeds node size");

#endif

	_t_dl_data		*psNodeData = this->template get_pooledData<_t_dl_data> (nPool, nNode);

	memmove ((void *) &(psNodeData[nOffset + nDataLen]), (void *) &(psNodeData[nOffset]), sizeof (*psNodeData) * (nNodeLen - nOffset));
//	memcpy ((void *) &(psNodeData[nOffset]), (void *) pData, sizeof (*psNodeData) * nDataLen);
}

template<class _t_datalayerproperties>
void CBTreeRAMIO<_t_datalayerproperties>::set_size (typename _t_datalayerproperties::node_iter_type nMaxNodes)
{
	if (nMaxNodes == this->m_nMaxNodes)
	{
		return;
	}

	address_type		nSize;

#if defined (_DEBUG)
	
	address_type		nOrgSize;

	node_iter_type		bNotInit;

#endif

	uint32_t			nPool;

	for (nPool = 0; nPool < this->m_nNumDataPools; nPool++)
	{
#if defined (_DEBUG)

		bNotInit = (this->m_ppsPools[nPool] == NULL) ? 0 : 1;

#endif

		nSize = (address_type) (this->get_pool_total_size (nPool) * (nMaxNodes + 1));

		this->m_ppsPools[nPool] = (uint8_t *) realloc ((void *) (this->m_ppsPools[nPool]), (size_t) nSize);

		BTREEDATA_ASSERT (this->m_ppsPools[nPool] != NULL, "CBTreeRAMIO::set_size: insufficient memory");

#if defined (_DEBUG)

		if (nMaxNodes > this->m_nMaxNodes)
		{
			nOrgSize = (address_type) (this->get_pool_total_size (nPool) * (this->m_nMaxNodes + bNotInit));

			memset ((void *) &(((this->m_ppsPools[nPool]))[nOrgSize]), 0xEE, (size_t) (nSize - nOrgSize));
		}

#endif
	}

	this->m_nMaxNodes = nMaxNodes;
}

template<class _t_datalayerproperties>
void CBTreeRAMIO<_t_datalayerproperties>::unload ()
{
}

template<class _t_datalayerproperties>
void CBTreeRAMIO<_t_datalayerproperties>::unload_from_cache (typename _t_datalayerproperties::node_iter_type nNode)
{
	nNode;
}

template<class _t_datalayerproperties>
bool CBTreeRAMIO<_t_datalayerproperties>::is_dataCached (uint32_t nPool, typename _t_datalayerproperties::node_iter_type nNode)
{
	bool	bRetval = false;

	return (bRetval);
}

template<class _t_datalayerproperties>
void CBTreeRAMIO<_t_datalayerproperties>::showdump	(std::ofstream &ofs, typename _t_datalayerproperties::node_iter_type nTreeSize, char *pAlloc)
{
	node_iter_type	ui64;
	uint32_t		ui32;

	this->set_cacheFreeze (true);
	{
		ofs << "<H1>data dump</H1>" << ::std::endl;
		ofs << "<br>" << ::std::endl;

		ofs << "m_nNumDataPools: " << this->m_nNumDataPools << "<br>" << ::std::endl;
		
		ofs << "<table>" << ::std::endl;
		
		ofs << "<tr>" << ::std::endl;
		ofs << "<td></td>" << ::std::endl;

		for (ui32 = 0; ui32 < this->m_nNumDataPools; ui32++)
		{
			ofs << "<td>Pool " << ui32 << "</td>" << ::std::endl;
		}

		ofs << "</tr>" << ::std::endl;

		ofs << "<tr>" << ::std::endl;
		ofs << "<td>m_psDataPools->nEntrySize</td>" << ::std::endl;

		for (ui32 = 0; ui32 < this->m_nNumDataPools; ui32++)
		{
			ofs << "<td>" << this->m_psDataPools[ui32].nEntrySize << "</td>" << ::std::endl;
		}

		ofs << "</tr>" << ::std::endl;

		ofs << "<tr>" << ::std::endl;
		ofs << "<td>m_psDataPools->nTotalSize</td>" << ::std::endl;

		for (ui32 = 0; ui32 < this->m_nNumDataPools; ui32++)
		{
			ofs << "<td>" << this->m_psDataPools[ui32].nTotalSize << "</td>" << ::std::endl;
		}

		ofs << "</tr>" << ::std::endl;
		
		ofs << "</table>" << ::std::endl;

		ofs << "<table>" << ::std::endl;

		for (ui64 = 0; ui64 < nTreeSize; ui64++)
		{
			ofs << "<tr>" << ::std::endl;
			ofs << "<td>" << ::std::endl;

			ofs << "<a name=\"node_dump_" << ui64 << "\">";
			ofs << "node: " << ui64;
			ofs << "</a>" << ::std::endl;

			ofs << "</td><td><a href=\"#node_" << ui64 << "\">tree<a/></td><td><a href=\"#node_list_" << ui64 << "\">list<a/></td>" << ::std::endl;
			ofs << "</tr>" << ::std::endl;

			{
				ofs << "<tr>" << ::std::endl;

				ofs << "<td></td><td>" << ::std::endl;

				uint32_t	uk32;

				for (uk32 = 0; uk32 < this->m_nNumDataPools; uk32++)
				{
					address_type	nAddr = (address_type) (this->get_pool_total_size (uk32) * ui64);
					bool			bIsCached = this->is_dataCached (uk32, ui64);

					ofs << "<td>" << ::std::endl;
					
					if (!bIsCached)
					{
						ofs << "<font color=\"#999999\">";
					}

					ofs << nAddr << ":" << ::std::endl;
					
					if (!bIsCached)
					{
						ofs << "</font>";
					}

					ofs << "</td>" << ::std::endl;

					ofs << "<td>" << ::std::endl;

					uint32_t	ul32;
					uint8_t		*pb;
					uint32_t	nMaxPoolLen;

					if (bIsCached)
					{
						ofs << "<font color=\"#4040FF\">";
					}

					nMaxPoolLen = this->get_pool_total_size (uk32);

					pb = this->template get_pooledData<uint8_t> (uk32, ui64);
					
					for (ul32 = 0; ul32 < nMaxPoolLen; ul32++)
					{
						ofs << ::std::hex << ::std::setfill ('0') << ::std::setw (2) << (uint32_t) pb[ul32] << ::std::dec;

						if (((ul32 + 1) % this->get_pool_entry_size (uk32)) == 0)
						{
							ofs << "<br>" << ::std::endl;
						}
						else if (((ul32 + 1) % 4) == 0)
						{
							ofs << " ";
						}
					}

					if (bIsCached)
					{
						ofs << "</font>";
					}

					ofs << "<br>";
					
					ofs << "</td>";
				}

				ofs << "</tr>" << ::std::endl;
			}
		}

		ofs << "</table>" << ::std::endl;
	}
	this->set_cacheFreeze (false);
}

#endif // BTREERAMIO_CPP
