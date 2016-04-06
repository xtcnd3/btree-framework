/************************************************************
**
** file:	btreeiolinear.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** block IO data layer class.
**
************************************************************/

#ifndef	BTREEIOLINEAR_CPP
#define	BTREEIOLINEAR_CPP

#include "btreeiolinear.h"

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeLinearIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeLinearIO
(
	_t_addresstype nBlockSize, 
	uint32_t nNumDataPools, 
	CBTreeIOperBlockPoolDesc_t *psDataPools
)
	:	CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype> (nBlockSize, nNumDataPools, psDataPools)
	,	m_ppsPools (NULL)
{
	m_ppsPools = new uint8_t * [this->m_nNumDataPools];

	BTREEDATA_ASSERT (m_ppsPools != NULL, "CBTreeLinearIO::CBTreeLinearIO: insufficient memory!");

	uint32_t	nPool;

	for (nPool = 0; nPool < this->m_nNumDataPools; nPool++)
	{
		m_ppsPools[nPool] = NULL;
	}
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeLinearIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::~CBTreeLinearIO ()
{
	if (m_ppsPools != NULL)
	{
		uint32_t	nPool;

		for (nPool = 0; nPool < this->m_nNumDataPools; nPool++)
		{
			if (m_ppsPools[nPool] != NULL)
			{
				free ((void *) m_ppsPools[nPool]);
			}
		}

		delete [] m_ppsPools;
	}
}

template<class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
const uint8_t* CBTreeLinearIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_node_base (uint32_t nPool, _t_nodeiter nNode)
{
#if defined (_DEBUG)

	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeLinearIO::get_node_base: nPool exceeds available pools");

#endif

	const uint8_t		*psPool = this->m_ppsPools[nPool];
	const uint8_t		*psPooledNode = &(psPool[nNode * this->get_pool_total_size (nPool)]);

	return (psPooledNode);
}

#endif // BTREEIOLINEAR_CPP
