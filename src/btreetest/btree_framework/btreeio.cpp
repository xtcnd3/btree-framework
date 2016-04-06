/************************************************************
**
** file:	btreeio.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** abstract data layer class.
**
************************************************************/

#ifndef	BTREEIO_CPP
#define	BTREEIO_CPP

#include "btreeio.h"

using namespace std;

// block [0]
// |- node desciption[0]         -|- maintainence vector-|-algnm-|--- data[0][] ---|-algnm-|--- sub node numbers[0][] ---|-algnm-|--- serVector[0][] ---|-algnm-|	<-- node[0]
// |- node desciption[1]         -|- maintainence vector-|-algnm-|--- data[1][] ---|-algnm-|--- sub node numbers[1][] ---|-algnm-|--- serVector[1][] ---|-algnm-|	<-- node[1]
// |- node desciption[2]         -|- maintainence vector-|...
// ...
// |- node desciption[(2^n) - 1] -|- maintainence vector-|...
// data layer block alignment
// block [1]
// |- node desciption[2^n]       -|- maintainence vector-|-algnm-|--- data[0][] ---|-algnm-|--- sub node numbers[0][] ---|-algnm-|--- serVector[0][] ---|-algnm-|	<-- node[0]

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeIO
(
	_t_addresstype nBlockSize, 
	uint32_t nNumDataPools, 
	CBTreeIOperBlockPoolDesc_t *psDataPools
)
	:	m_psDataPools (NULL)
	,	m_nNumDataPools (nNumDataPools)
	,	m_bCacheFreeze (false)
	,	m_nMaxNodes (0ULL)
{
	uint32_t			ui32;

	m_psDataPools = new CBTreeIOperBlockPoolDesc_t [m_nNumDataPools];

	BTREEDATA_ASSERT (m_psDataPools != NULL, "CBTreeIO::CBTreeIO: insufficient memory!");

	for (ui32 = 0; ui32 < m_nNumDataPools; ui32++)
	{
		m_psDataPools[ui32] = psDataPools[ui32];
	}

#if defined (USE_PERFORMANCE_COUNTERS)
	memset ((void *) &m_anHitCtrs, 0, sizeof (m_anHitCtrs));
	memset ((void *) &m_anMissCtrs, 0, sizeof (m_anMissCtrs));
#endif

#if defined (_DEBUG)

#endif
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::~CBTreeIO ()
{
	if (m_psDataPools != NULL)
	{
		delete[] m_psDataPools;
	}
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_nodeiter CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_maxNodes ()
{
	return (m_nMaxNodes);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_offsettype CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_dataAlignment ()
{
	// The keyword 'long' has been purposely choosen, since it will make the compiler to return 4 bytes on 32 bit builds and 8 bytes on 64 bit builds, as oppose to uint32_t and uint64_t.
	return ((_t_offsettype) (sizeof (long)));
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_offsettype CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_alignedOffset (_t_offsettype nOffset)
{
	nOffset += get_dataAlignment () - 1;
	nOffset /= get_dataAlignment ();
	nOffset *= get_dataAlignment ();

	return (nOffset);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_performance_counters (uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR])
{
#if defined (USE_PERFORMANCE_COUNTERS)
	memcpy ((void *) &rHitCtrs, (void *) m_anHitCtrs, sizeof (m_anHitCtrs));
	memcpy ((void *) &rMissCtrs, (void *) m_anMissCtrs, sizeof (m_anMissCtrs));

	switch (m_ePresentationMode)
	{
	case presentationMODE_FILE		:	m_pFile->getPerfCtrs (rHitCtrs[PERFCTR_FILE_CACHE], rMissCtrs[PERFCTR_FILE_CACHE]);

										break;
	case presentationMODE_MEMORY	:	
										break;

	default							:	BTREEDATA_ASSERT (false, "CBTreeIO::get_performance_counters: bayer tree presentation mode not set or corrupted");

										break;
	}

#else
	memset ((void *) &rHitCtrs, 0, sizeof (rHitCtrs));
	memset ((void *) &rMissCtrs, 0, sizeof (rMissCtrs));
#endif
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
uint32_t CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_pool_entry_size (uint32_t nPool)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < m_nNumDataPools, "CBTreeIO::get_pool_entry_size: nPool exceeds available block pools!");
#endif

	return (m_psDataPools[nPool].nEntrySize);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_root_node (_t_nodeiter nRootNode)
{
	nRootNode;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::terminate_access ()
{

}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_cacheFreeze (bool bEnabled)
{
	m_bCacheFreeze = bEnabled;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
uint32_t CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_pool_total_size (uint32_t nPool)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < m_nNumDataPools, "CBTreeIO::get_pool_total_size: nPool exceeds available block pools!");
#endif

	return (m_psDataPools[nPool].nTotalSize);
}

#endif // BTREEIO_CPP
