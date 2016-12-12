/************************************************************
**
** file:	btreeio.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** abstract data layer class.
**
************************************************************/

#ifndef	BTREEIO_CPP
#define	BTREEIO_CPP

#include "io/data_layer/base/btreeio.h"

template<class _t_datalayerproperties>
CBTreeIO<_t_datalayerproperties>::CBTreeIO
(
	const sub_node_iter_type nNodeSize, 
	const uint32_t nNumDataPools, 
	const CBTreeIOperBlockPoolDesc_t *psDataPools
)
	:	m_psDataPools (NULL)
	,	m_nNumDataPools (nNumDataPools)
	,	m_bCacheFreeze (false)
	,	m_nNodeSize (nNodeSize)
	,	m_nMaxNodes (0)
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

template<class _t_datalayerproperties>
CBTreeIO<_t_datalayerproperties>::~CBTreeIO ()
{
	if (m_psDataPools != NULL)
	{
		delete[] m_psDataPools;
	}
}

template<class _t_datalayerproperties>
typename _t_datalayerproperties::node_iter_type CBTreeIO<_t_datalayerproperties>::get_maxNodes () const
{
	return (m_nMaxNodes);
}

template<class _t_datalayerproperties>
typename CBTreeIO<_t_datalayerproperties>::offset_type CBTreeIO<_t_datalayerproperties>::get_dataAlignment () const
{
	// The keyword 'long' has been purposely choosen, since it will make the compiler to return 4 bytes on 32 bit builds and 8 bytes on 64 bit builds, as oppose to uint32_t and uint64_t.
	return ((offset_type) (sizeof (long)));
}

template<class _t_datalayerproperties>
typename CBTreeIO<_t_datalayerproperties>::offset_type CBTreeIO<_t_datalayerproperties>::get_alignedOffset (const typename _t_datalayerproperties::offset_type nOffset) const
{
	offset_type		nRetval = nOffset;

	nRetval += get_dataAlignment () - 1;
	nRetval /= get_dataAlignment ();
	nRetval *= get_dataAlignment ();

	return (nRetval);
}

template<class _t_datalayerproperties>
void CBTreeIO<_t_datalayerproperties>::get_performance_counters (uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR])
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

template<class _t_datalayerproperties>
uint32_t CBTreeIO<_t_datalayerproperties>::get_pool_entry_size (const uint32_t nPool) const
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < m_nNumDataPools, "CBTreeIO::get_pool_entry_size: nPool exceeds available block pools!");
#endif

	return (m_psDataPools[nPool].nEntrySize);
}

template<class _t_datalayerproperties>
void CBTreeIO<_t_datalayerproperties>::set_root_node (const typename _t_datalayerproperties::node_iter_type nRootNode)
{
	nRootNode;
}

template<class _t_datalayerproperties>
void CBTreeIO<_t_datalayerproperties>::terminate_access ()
{

}

template<class _t_datalayerproperties>
void CBTreeIO<_t_datalayerproperties>::set_cacheFreeze (const bool bEnabled)
{
	m_bCacheFreeze = bEnabled;
}

template<class _t_datalayerproperties>
uint32_t CBTreeIO<_t_datalayerproperties>::get_pool_total_size (const uint32_t nPool) const
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < m_nNumDataPools, "CBTreeIO::get_pool_total_size: nPool exceeds available block pools!");
#endif

	return (m_psDataPools[nPool].nTotalSize);
}

#endif // BTREEIO_CPP
