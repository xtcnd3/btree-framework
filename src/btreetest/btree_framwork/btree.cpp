/************************************************************
**
** file:	btree.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree frameworks base class.
**
************************************************************/

#ifndef	BTREE_CPP
#define	BTREE_CPP

#if defined (_OPENMP)
 #include <omp.h>
#endif

#include <string>

#if defined (WIN32)

 #if defined (_HAS_MFC)
  #include <afxtempl.h>
 #endif

 #if !defined (_HAS_MFC)
  #include <windows.h>
 #endif

 #include <io.h>

#elif defined (LINUX)

 #include <stdlib.h>

#endif

#include <memory.h>

#include "btree.h"

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize)
	:	CBTreeSuper ()
	,	CBTreeIf<_t_sizetype> ()
	,	m_nRootNode (0ULL)
	,	m_nTreeSize (0ULL)
	,	m_pData (NULL)
	,	m_sCacheDescription (*psCacheDescription)
	,	m_nNodeSize (nNodeSize)
	,	m_nGrowBy (64UL)
	,	m_nAutoShrinkThreshold (nNodeSize * 2)
	,	m_nAutoShrinkCounter (0)
	,	m_pSerVector (NULL)
{
#if defined (BTREE_ITERATOR_REGISTRATION)
	m_psIterRegister = new typename ::std::set<const_iterator *> ();

	BTREE_ASSERT (m_psIterRegister != NULL, "ERROR: CBTreeBase<>::CBTreeBase (): insufficient memory!");
#endif

	m_nRebuildFIFOSize = 0UL;
	m_pRebuildFIFO = NULL;
	m_nRebuildFIFOreadPos = 0ULL;
	m_nRebuildFIFOwritePos = 0ULL;

	m_pSubNodeBuffer0 = new _t_nodeiter [(size_t) get_node_max_sub_nodes ()];
	m_pSubNodeBuffer1 = new _t_nodeiter [(size_t) get_node_max_sub_nodes ()];
	m_pSubNodeBuffer2 = new _t_nodeiter [(size_t) get_node_max_sub_nodes ()];

	m_pData_tobj0 = new _t_data [(size_t) get_node_max_data_size ()];
	m_pData_tobj1 = new _t_data [(size_t) get_node_max_data_size ()];
	
	m_pData_tobj_u0 = new _t_data [(size_t) get_node_max_data_size ()];
	m_pData_tobj_u1 = new _t_data [(size_t) get_node_max_data_size ()];
	m_pData_tobj_u2 = new _t_data [(size_t) get_node_max_data_size ()];

	BTREE_ASSERT (m_pSubNodeBuffer0 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (bool, char *, bayerTreeCacheDescription_t *, uint64_t, sortMode_e, peekMode_e, void *, uint64_t, unsigned int, unsigned int): insufficient memory");
	BTREE_ASSERT (m_pSubNodeBuffer1 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (bool, char *, bayerTreeCacheDescription_t *, uint64_t, sortMode_e, peekMode_e, void *, uint64_t, unsigned int, unsigned int): insufficient memory");
	BTREE_ASSERT (m_pSubNodeBuffer2 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (bool, char *, bayerTreeCacheDescription_t *, uint64_t, sortMode_e, peekMode_e, void *, uint64_t, unsigned int, unsigned int): insufficient memory");
	BTREE_ASSERT (m_pData_tobj0 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (bool, char *, bayerTreeCacheDescription_t *, uint64_t, sortMode_e, peekMode_e, void *, uint64_t, unsigned int, unsigned int): insufficient memory");
	BTREE_ASSERT (m_pData_tobj1 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (bool, char *, bayerTreeCacheDescription_t *, uint64_t, sortMode_e, peekMode_e, void *, uint64_t, unsigned int, unsigned int): insufficient memory");
	BTREE_ASSERT (m_pData_tobj_u0 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (bool, char *, bayerTreeCacheDescription_t *, uint64_t, sortMode_e, peekMode_e, void *, uint64_t, unsigned int, unsigned int): insufficient memory");
	BTREE_ASSERT (m_pData_tobj_u1 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (bool, char *, bayerTreeCacheDescription_t *, uint64_t, sortMode_e, peekMode_e, void *, uint64_t, unsigned int, unsigned int): insufficient memory");
	BTREE_ASSERT (m_pData_tobj_u2 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (bool, char *, bayerTreeCacheDescription_t *, uint64_t, sortMode_e, peekMode_e, void *, uint64_t, unsigned int, unsigned int): insufficient memory");

//	m_bRootCacheValid = false;
//	m_nRootCacheRoot = 0ULL;

	m_pKeyBuffer = NULL;
	m_nKeyBufferSize = 0;
	m_nKeyBufferFillState = 0;
	m_bKeyBufferWasTooSmall = false;

	m_nNextAlloc = 0ULL;

#if defined (_DEBUG)
	m_pNodeTrace = NULL;
	m_nNodeTraceDepth = 0UL;
	m_nNodeTraceDepthMax = 0UL;
#endif

#if defined (USE_PERFORMANCE_COUNTERS)
	m_nHitCtr = 0;
	m_nMissCtr = 0;
#endif

	m_pSerVector = new _t_sizetype [this->get_node_max_data_size ()];

	BTREE_ASSERT (m_pSerVector != NULL, "CBTreeBase<>::CBTreeBase (_t_datalayerproperties &, bayerTreeCacheDescription_t *, _t_subnodeiter): insufficient memory!");

	m_pClDataLayerProperties = new _t_datalayerproperties (rDataLayerProperties);

	BTREE_ASSERT (m_pClDataLayerProperties != NULL, "CBTreeBase<>::CBTreeBase (_t_datalayerproperties &, bayerTreeCacheDescription_t *, _t_subnodeiter): insufficient memory!");

	setup_pools ();
}

/*

Copy Constructor

rBT		-	reference to CBTreeBase instance to be copied

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT)
	:	CBTreeSuper ()
	,	CBTreeIf<_t_sizetype> ()
	,	m_nAutoShrinkThreshold (rBT.m_nAutoShrinkThreshold)
	,	m_nAutoShrinkCounter (0)
	,	m_pSerVector (NULL)
{
#if defined (BTREE_ITERATOR_REGISTRATION)
	m_psIterRegister = new typename ::std::set<const_iterator *> ();

	BTREE_ASSERT (m_psIterRegister != NULL, "ERROR: CBTreeBase<>::CBTreeBase (): insufficient memory!");
#endif

	m_nRootNode = 0ULL;
	m_nTreeSize = 0ULL;
	m_pData = NULL;
	m_sCacheDescription = rBT.m_sCacheDescription;

	m_nNodeSize = rBT.m_nNodeSize;

	m_nGrowBy = rBT.m_nGrowBy;

	m_nRebuildFIFOSize = 0UL;
	m_pRebuildFIFO = NULL;
	m_nRebuildFIFOreadPos = 0ULL;
	m_nRebuildFIFOwritePos = 0ULL;

	m_pSubNodeBuffer0 = new _t_nodeiter [(size_t) get_node_max_sub_nodes ()];
	m_pSubNodeBuffer1 = new _t_nodeiter [(size_t) get_node_max_sub_nodes ()];
	m_pSubNodeBuffer2 = new _t_nodeiter [(size_t) get_node_max_sub_nodes ()];

	m_pData_tobj0 = new _t_data [(size_t) get_node_max_data_size ()];
	m_pData_tobj1 = new _t_data [(size_t) get_node_max_data_size ()];
	
	m_pData_tobj_u0 = new _t_data [sizeof (_t_data) * (size_t) get_node_max_data_size ()];
	m_pData_tobj_u1 = new _t_data [sizeof (_t_data) * (size_t) get_node_max_data_size ()];
	m_pData_tobj_u2 = new _t_data [sizeof (_t_data) * (size_t) get_node_max_data_size ()];

	BTREE_ASSERT (m_pSubNodeBuffer0 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (CBTreeBase<_t_data> &)insufficient memory");
	BTREE_ASSERT (m_pSubNodeBuffer1 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (CBTreeBase<_t_data> &)insufficient memory");
	BTREE_ASSERT (m_pSubNodeBuffer2 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (CBTreeBase<_t_data> &)insufficient memory");
	BTREE_ASSERT (m_pData_tobj0 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (CBTreeBase<_t_data> &)insufficient memory");
	BTREE_ASSERT (m_pData_tobj1 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (CBTreeBase<_t_data> &)insufficient memory");
	BTREE_ASSERT (m_pData_tobj_u0 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (CBTreeBase<_t_data> &)insufficient memory");
	BTREE_ASSERT (m_pData_tobj_u1 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (CBTreeBase<_t_data> &)insufficient memory");
	BTREE_ASSERT (m_pData_tobj_u2 != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBase (CBTreeBase<_t_data> &): insufficient memory");

//	m_bRootCacheValid = false;
//	m_nRootCacheRoot = 0ULL;

	m_pKeyBuffer = NULL;
	m_nKeyBufferSize = 0;
	m_nKeyBufferFillState = 0;
	m_bKeyBufferWasTooSmall = false;

	m_nNextAlloc = 0ULL;

#if defined (_DEBUG)
	m_pNodeTrace = NULL;
	m_nNodeTraceDepth = 0UL;
	m_nNodeTraceDepthMax = 0UL;
#endif

#if defined (USE_PERFORMANCE_COUNTERS)
	m_nHitCtr = 0;
	m_nMissCtr = 0;
#endif

	m_pSerVector = new _t_sizetype [this->get_node_max_data_size ()];

	BTREE_ASSERT (m_pSerVector != NULL, "CBTreeBase<>::CBTreeBase (CBTreeBase<> &): insufficient memory!");

	m_pClDataLayerProperties = new _t_datalayerproperties (*(rBT.m_pClDataLayerProperties));

	BTREE_ASSERT (m_pClDataLayerProperties != NULL, "CBTreeBase<>::CBTreeBase (CBTreeBase<> &): insufficient memory!");

	setup_pools ();
}

/*

Destructor

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeBase ()
{
	clear ();

	delete m_pData;

	delete m_pClDataLayerProperties;

	delete [] m_pSubNodeBuffer0;
	delete [] m_pSubNodeBuffer1;
	delete [] m_pSubNodeBuffer2;

	delete [] m_pData_tobj0;
	delete [] m_pData_tobj1;
	
	delete [] m_pData_tobj_u0;
	delete [] m_pData_tobj_u1;
	delete [] m_pData_tobj_u2;

	if (m_pRebuildFIFO != NULL)
	{
		delete [] m_pRebuildFIFO;
	}

	if (m_pKeyBuffer != NULL)
	{
		free ((void *) m_pKeyBuffer);
	}

#if defined (_DEBUG)
	if (m_pNodeTrace != NULL)
	{
		delete [] m_pNodeTrace;
	}
#endif

#if defined (BTREE_ITERATOR_REGISTRATION)

	typename ::std::set<const_iterator *>::iterator		sIter = m_psIterRegister->begin ();

	for (; sIter != m_psIterRegister->end (); sIter++)
	{
		(*sIter)->detach ();
	}

	delete m_psIterRegister;

#endif

	if (m_pSerVector != NULL)
	{
		delete [] m_pSerVector;
	}
}

/*

get_performance_counters - get performance counters

The information released by this routine can be used to identify performance lags in a CBTreeBase instance.

rHitCtrs	- is a refernce to an array of cache hit counters
rMissCtrs	- is a refernce to an array of cache missed counters

Each array contains the following elements:
PERFCTR_SUPER_BLOCK_ADDR		- super block address generator TLB
PERFCTR_BLOCK_ADDR				- block address generator TLB
PERFCTR_RESERVATION_VECTOR_ADDR	- reservation vector address generator TLB
PERFCTR_NODE_ADDR				- node address generator TLB
PERFCTR_SUBNODE_ADDR			- sub-node data address generator TLB
PERFCTR_SERIAL_VECTOR_ADDR		- serial information vector address generator TLB
PERFCTR_DATA_ADDR				- data address generator TLB
PERFCTR_FILE_CACHE				- file cache of internal CFileMem instance
PERFCTR_NODEDATA_DATA			- node information data cache
PERFCTR_SUBNODE_DATA			- sub-node information data cache
PERFCTR_DATA					- top level data cache

Note: TLB means translation look aside buffer.

NB: The defined USE_PERFORMANCE_COUNTERS needs to be defined, otherwise only unset values are returned.

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_performance_counters (uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR])
{
#if defined (USE_PERFORMANCE_COUNTERS)
	if (m_pData != NULL)
	{
		m_pData->get_performance_counters (rHitCtrs, rMissCtrs);

		rHitCtrs[PERFCTR_DATA] = m_nHitCtr;
		rMissCtrs[PERFCTR_DATA] = m_nMissCtr;
	}
	else
	{
		memset ((void *) &rHitCtrs, 0, sizeof (rHitCtrs));
		memset ((void *) &rMissCtrs, 0, sizeof (rMissCtrs));
	}
#else
	memset ((void *) &rHitCtrs, 0, sizeof (rHitCtrs));
	memset ((void *) &rMissCtrs, 0, sizeof (rMissCtrs));
#endif
}

/*

is_emtpy - is b-tree empty

This method returns true if this b-tree instance contains no data, otherwise false.

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::empty ()
{
	bool	bRetval = CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::size () == 0ULL;

	return (bRetval);
}

/*

size - get count

This method returns the number of data entries present in this b-tree instance.

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::size ()
{
	if (m_pData == NULL)
	{
		return (0ULL);
	}

	return (get_max_index (m_nRootNode));
}

/*

clear - remove all data

This method removes all data from the b-tree at once.

If successful the method returns true, otherwise false.

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ()
{
	if (m_pData != NULL)
	{
		delete m_pData;

		m_pData = NULL;
	}

	m_nTreeSize = 0ULL;
//	m_bRootCacheValid = false;
	m_nNextAlloc = 0ULL;
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::cbegin ()
{
	typename CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator			sIter (this, 0, false);

	return (sIter);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator  CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::cend ()
{
	typename CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator			sIter (this, this->size (), false);

	return (sIter);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::crbegin ()
{
	typename CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator			sIter (this, this->size (), false);
	typename CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator	sRIter (sIter);

	return (sRIter);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::crend ()
{
	typename CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator			sIter (this, 0, false);
	typename CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator	sRIter (sIter);

	return (sRIter);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap
	(
		CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT
	)
{
	if (this != &rBT)
	{
		fast_swap<_t_nodeiter> (this->m_nRootNode, rBT.m_nRootNode);
		fast_swap<_t_nodeiter> (this->m_nTreeSize, rBT.m_nTreeSize);
		fast_swap<_t_datalayer *> (this->m_pData, rBT.m_pData);
		fast_swap<bayerTreeCacheDescription_t> (this->m_sCacheDescription, rBT.m_sCacheDescription);
		fast_swap<_t_datalayerproperties *> (this->m_pClDataLayerProperties, rBT.m_pClDataLayerProperties);
		fast_swap<_t_subnodeiter> (this->m_nNodeSize, rBT.m_nNodeSize);
		fast_swap<uint32_t> (this->m_nGrowBy, rBT.m_nGrowBy);
		fast_swap<uint32_t> (this->m_nPoolIDnodeDesc, rBT.m_nPoolIDnodeDesc);
		fast_swap<uint32_t> (this->m_nPoolIDnodeMaintenance, rBT.m_nPoolIDnodeMaintenance);
		fast_swap<uint32_t> (this->m_nPoolIDdata, rBT.m_nPoolIDdata);
		fast_swap<uint32_t> (this->m_nPoolIDsubNodes, rBT.m_nPoolIDsubNodes);
//		fast_swap<bool> (this->m_bRootCacheValid, rBT.m_bRootCacheValid);
//		fast_swap<_t_nodeiter> (this->m_nRootCacheRoot, rBT.m_nRootCacheRoot);
//		fast_swap<node_t> (this->m_sRootCacheNode, rBT.m_sRootCacheNode);
		fast_swap<_t_nodeiter *> (this->m_pSubNodeBuffer0, rBT.m_pSubNodeBuffer0);
		fast_swap<_t_nodeiter *> (this->m_pSubNodeBuffer1, rBT.m_pSubNodeBuffer1);
		fast_swap<_t_nodeiter *> (this->m_pSubNodeBuffer2, rBT.m_pSubNodeBuffer2);
		fast_swap<_t_data *> (this->m_pData_tobj0, rBT.m_pData_tobj0);
		fast_swap<_t_data *> (this->m_pData_tobj1, rBT.m_pData_tobj1);
		fast_swap<_t_data *> (this->m_pData_tobj_u0, rBT.m_pData_tobj_u0);
		fast_swap<_t_data *> (this->m_pData_tobj_u1, rBT.m_pData_tobj_u1);
		fast_swap<_t_data *> (this->m_pData_tobj_u2, rBT.m_pData_tobj_u2);
		fast_swap<_t_nodeiter *> (this->m_pRebuildFIFO, rBT.m_pRebuildFIFO);
		fast_swap<uint32_t> (this->m_nRebuildFIFOSize, rBT.m_nRebuildFIFOSize);
		fast_swap<uint32_t> (this->m_nRebuildFIFOreadPos, rBT.m_nRebuildFIFOreadPos);
		fast_swap<uint32_t> (this->m_nRebuildFIFOwritePos, rBT.m_nRebuildFIFOwritePos);
		fast_swap<uint32_t> (this->m_nAutoShrinkThreshold, rBT.m_nAutoShrinkThreshold);
		fast_swap<uint32_t> (this->m_nAutoShrinkCounter, rBT.m_nAutoShrinkCounter);

#if defined (_DEBUG)

		fast_swap<_t_nodeiter *> (this->m_pNodeTrace, rBT.m_pNodeTrace);
		fast_swap<uint32_t> (this->m_nNodeTraceDepth, rBT.m_nNodeTraceDepth);
		fast_swap<uint32_t> (this->m_nNodeTraceDepthMax, rBT.m_nNodeTraceDepthMax);

#endif

		fast_swap<uint8_t *> (this->m_pKeyBuffer, rBT.m_pKeyBuffer);
		fast_swap<uint32_t> (this->m_nKeyBufferSize, rBT.m_nKeyBufferSize);
		fast_swap<uint32_t> (this->m_nKeyBufferFillState, rBT.m_nKeyBufferFillState);
		fast_swap<bool> (this->m_bKeyBufferWasTooSmall, rBT.m_bKeyBufferWasTooSmall);
		fast_swap<_t_nodeiter> (this->m_nNextAlloc, rBT.m_nNextAlloc);

#if defined (USE_PERFORMANCE_COUNTERS)

		fast_swap<uint64_t> (this->m_nHitCtr, rBT.m_nHitCtr);
		fast_swap<uint64_t> (this->m_nMissCtr, rBT.m_nMissCtr);

#endif

		fast_swap<_t_sizetype *> (this->m_pSerVector, rBT.m_pSerVector);

//#if defined (BTREE_ITERATOR_REGISTRATION)
//
//		fast_swap<typename ::std::set<const_iterator *> *> (this->m_psIterRegister, rBT.m_psIterRegister);
//
//#endif

		CBTreeSuper	&rThisSuper = dynamic_cast <CBTreeSuper &> (*this);
		CBTreeSuper	&rArraySuper = dynamic_cast <CBTreeSuper &> (rBT);

		rThisSuper.swap (rArraySuper);
	}
}

/*

unload - unload all data to disc

This method stores all data in this instance associated temporary file and frees
all temporary buffers and data caches. This is useful if the resources on a system are
scarce and it is known that this instance is not needed access to for a long time.
It is not required to reload the data, since this is done by any call trying to access
any data. However, even a simple call to size will cause the data to be reloaded.
Hence, if you do not want this instance to be realoded, then do not call any of its
methods.
Also do not frequently call the unload method. It will result in significant
performance drops.

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::unload ()
{
/*	if (m_bRootCacheValid)
	{
		// flush root cache

		// read root node from root cache
		node_t		sNodeDesc;
			
		get_node (m_nRootNode, sNodeDesc);

		// disable root cache
		m_bRootCacheValid = false;

		// write root node to normal cache
		set_node (m_nRootNode, sNodeDesc);
	}*/

	// unload data container
	if (m_pData != NULL)
	{
		m_pData->unload ();
	}

	// destroy rebuild FIFO
	if (m_pRebuildFIFO != NULL)
	{
		delete [] m_pRebuildFIFO;
	}

	m_pRebuildFIFO = NULL;

	m_nRebuildFIFOwritePos = 0ULL;
	m_nRebuildFIFOreadPos = 0ULL;
	m_nRebuildFIFOSize = 0ULL;

#if defined (_DEBUG)
	// destroy node trace buffer
	if (m_pNodeTrace != NULL)
	{
		delete [] m_pNodeTrace;
	}

	m_pNodeTrace = NULL;

	m_nNodeTraceDepthMax = 1;
#endif
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::test_integrity ()
{
	if (m_pData == NULL)
	{
		return (true);
	}

	char				*pAlloc = new char[(size_t) m_nTreeSize];
	bool				retval = false;
	std::ofstream		ofs ("");

	ofs.close ();
	
	if (pAlloc != NULL)
	{
		memset ((void *) pAlloc, 0, sizeof (*pAlloc) * (size_t) m_nTreeSize);

		try
		{
			retval = shownode (ofs, m_nRootNode, m_nRootNode, pAlloc);
		}
		catch (exception *pE)
		{
			cerr << endl << "CRITICAL ERROR!" << endl;
			cerr << "exception has not been caught during integrity test" << endl;
			cerr << "message: " << pE->what () << endl;

			retval = false;
		}

		delete [] pAlloc;
	}
	else
	{
		cerr << "ERROR: CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::test_integrity: insufficient memory!" << endl;

		retval = false;
	}

	return (retval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::show_integrity (const char *pFilename)
{
	char				*pAlloc = new char[(size_t) m_nTreeSize];
	std::ofstream		ofs ((const char *) pFilename, std::ofstream::out | std::ofstream::trunc);

	BTREE_ASSERT (pAlloc != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::show_integrity: insufficient memory to generate integrty page");

	memset ((void *) pAlloc, 0, sizeof (*pAlloc) * (size_t) m_nTreeSize);

	ofs << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">" << endl;
	ofs << "<HTML>" << endl;
	ofs << "<head>" << endl;
	ofs << "<title>bayer tree integrity</title>" << endl;
	ofs << "<h1>bayer tree integrity</h1>" << endl;
	ofs << "</head>" << endl;
	ofs << "<body>" << endl;

	m_pData->set_cacheFreeze (true);
	{
		try
		{
			shownode (ofs, m_nRootNode, m_nRootNode, pAlloc);
		}
		catch (exception *pE)
		{
			cerr << endl << "CRITICAL ERROR!" << endl;
			cerr << "exception has not been caught during integrity display" << endl;
			cerr << "message: " << pE->what () << endl;
		}

		try
		{
			shownodelist (ofs, pAlloc);
		}
		catch (exception *pE)
		{
			cerr << endl << "CRITICAL ERROR!" << endl;
			cerr << "exception has not been caught during node list display" << endl;
			cerr << "message: " << pE->what () << endl;
		}
	}
	m_pData->set_cacheFreeze (false);

	try
	{
		m_pData->showdump (ofs, m_nTreeSize, pAlloc);
	}
	catch (exception *pE)
	{
		cerr << endl << "CRITICAL ERROR!" << endl;
		cerr << "exception has not been caught during data dump display" << endl;
		cerr << "message: " << pE->what () << endl;
	}

	ofs << "</body>" << endl;
	ofs << "</HTML>" << endl;

	ofs.close ();

	delete [] pAlloc;
}

/*

create_root

- creates the root node on creation of the tree or if split_node needs to generate a new root

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::create_root ()
{
	// if the tree structure has not been generated ...
	if (m_pData == NULL)
	{
		// ... then create the data layer
		
		m_pData = new _t_datalayer
								(
									*m_pClDataLayerProperties,
									m_sCacheDescription.nMinNumberOfBytesPerSuperBlock, 
									sizeof (node_t), 
									m_nNumPools, 
									m_pPoolDesc
								);

		// test if bring up was successful
		BTREE_ASSERT (m_pData != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::create_root: data object not created");

//		BTREE_ASSERT (m_bRootCacheValid == false, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::create_root: root cache had invalid state while creating root node");

		// allocate root node
		m_nRootNode = create_node ((_t_nodeiter) ~0x0);

		// set up root node
//		get_node (m_nRootNode, m_sRootCacheNode);
//		m_nRootCacheRoot = m_nRootNode;
//		m_bRootCacheValid = true;
	}
}

/*

create_node

- allocates a new node and returns that nodes identifier number
- it also updates the node allocation table 

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_nodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::create_node (_t_nodeiter nParent)
{
	node_t					sNodeDesc;
	_t_nodeiter				ui64, retval;
	_t_nodeiter				uj64;
	uint32_t				mask;
	bool					foundFirst = false;

	// if tree structure has not been generated ...
	if (m_pData == NULL)
	{
		// ... then create root node
		create_root ();

		return (m_nRootNode);
	}

	// obtain node data cache size
	/*
	**	This mask will be used to avoid creating a child node on the same
	**	cache line as its parent, in an afford to optimise for speed.
	*/
	mask = m_pData->get_node_buffer_cache_size () - 1UL;

	// try to find next available free node from next position to probe for allocation on ...
	for (ui64 = m_nNextAlloc;;)
	{
		// ... until the currently allocated maximum nodes has been hit
		for (; ui64 < m_nTreeSize; ui64++)
		{
			// if node is available ...
			if (!get_reservation (ui64))
			{
				// ... then test if node is suitable

				// if this is the first availbable node ...
				if (!foundFirst)
				{
					// ... then update next allocation position
					m_nNextAlloc = ui64;

					foundFirst = true;
				}

				// if parent node has not been determined (ie. root node is being created) or
				// cache size is too small to allow any optimization ...
				if ((nParent == (_t_nodeiter) ~0x0) || (mask == 0))
				{
					// ... then allocate regradsless
					set_reservation (ui64);

					// update parent parameter to new root
					nParent = ui64;

					break;
				}
				else
				{
					// ... otherwise test if the new node would share the same cache line as its parent node
					if (((uint32_t)ui64 & mask) != ((uint32_t)nParent & mask))
					{
						// if not, allocate and terminate loop
						set_reservation (ui64);

						break;
					}
				}
			}
		}

		// if searching where to allocate the new node was not successful ...
		if (ui64 >= m_nTreeSize)
		{
			// ... then extend available number of nodes
			m_nTreeSize += (_t_nodeiter) m_nGrowBy;

			// determine and set new file size
			m_pData->set_size (m_nTreeSize);
			
			// unset allocation vector in newly created space
			for (uj64 = m_nTreeSize - (_t_nodeiter)m_nGrowBy; uj64 < m_nTreeSize; uj64++)
			{
				reset_reservation (uj64);
			}
		}
		else
		{
			// ... otherwise set allocated node as return value
			retval = ui64;

			// if next allocation id is the same as the id of the node that has just been created ...
			if (m_nNextAlloc == ui64)
			{
				// ... then move next allocation id by one
				m_nNextAlloc++;
			}

			break;
		}
	}

	// set up new node description
	sNodeDesc.nMaxIdx = 0;
	sNodeDesc.nNumData = 0;
	sNodeDesc.nParent = nParent;

	// copy new node description to data layer
	set_node (retval, sNodeDesc);

	return (retval);
}

/*

add_to_node

- recursively traces into the tree and adds one entry in a leave
- what will be added is determined the sort mode that has been set
- the method also splits nodes and traces back if need be
- if nodes had been split they also be rebuild

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::add_to_node (_ti_pos sPos, const _t_data &rData, _t_nodeiter nNode, uint32_t nDepth, _t_sizetype *pnPos)
{
	_t_sizetype				retval = 0ULL;
	_t_nodeiter				nSubNode;
	_t_subnodeiter			nSubPos;
	node_t					sNodeDesc;
		
	get_node (nNode, sNodeDesc);

	// store node identifier on rebuild stack
	attache_traced_node (nNode);

	// identify next sub node's position
	nSubPos = find_next_sub_pos (nNode, sPos);

	// if node is not a leave ...
	if (!is_leaf (nNode))
	{
		// ... then search for next node to trace into

		if (pnPos != NULL)
		{
			if (nSubPos > 0)
			{
				this->get_serVector (nNode, this->m_pSerVector);

				(*pnPos) += this->m_pSerVector[nSubPos - 1] + 1;
			}
		}

		// obtain next sub node's id number
		nSubNode = get_sub_node (nNode, nSubPos);

		// trace into next sub node
		retval += add_to_node (sPos, rData, nSubNode, nDepth + 1, pnPos);
		// sNodeDesc is not valid after this point, since it is a reference
		// and the content it was referring is cached and may have
		// been modified during the last call
		
		// if rebuild FIFO ran out of entries ...
		if (rebuild_FIFO_fillstate () == 0ULL)
		{
			// ... then quick auto-update this node by increasing amount of data contained in this tree branch by one
			rebuild_node (nNode, 1, nSubPos);
		}
	}
	else
	{
		// ... otherwise add data to leave
		insert_data_into_leaf (nNode, nSubPos, rData);

		retval++;

		auto_shrink_dec ();

		if (pnPos != NULL)
		{
			(*pnPos) += (_t_sizetype) nSubPos;
		}
	}

	// if rebuild FIFO is not empty ...
	if (rebuild_FIFO_fillstate () > 1ULL)
	{
		// ... then run a full update on both this node and the newly created node on the same level
		rebuild_node (rebuild_FIFO_get ());
		rebuild_node (rebuild_FIFO_get ());
	}

	// if a new root node has been created ...
	if ((nDepth == 0) && (nNode != m_nRootNode))
	{
#if defined (_DEBUG)
		BTREE_ASSERT (rebuild_FIFO_fillstate () == 1ULL, "CBTreeBase<_t_data, _t_datalayerproperties, _t_datalayer, _t_sizetype, _t_nodeiter, _t_subnodeiter>::add_to_node: FIFO corruption");
#endif

		// ... then update new root node
		rebuild_node (rebuild_FIFO_get ());
	}

#if defined (_DEBUG)
	if (nDepth == 0)
	{
		BTREE_ASSERT (rebuild_FIFO_fillstate () == 0UL, "CBTreeBase<_t_data, _t_datalayerproperties, _t_datalayer, _t_sizetype, _t_nodeiter, _t_subnodeiter>::add_to_node: FIFO corruption");
	}

	BTREE_ASSERT (get_data_count (nNode) <= get_node_max_data_size (), "CBTreeBase<_t_data, _t_datalayerproperties, _t_datalayer, _t_sizetype, _t_nodeiter, _t_subnodeiter>::add_to_node: node size exceeded 2 * t - 1 --> tree invalid");
#endif

	detache_traced_node ();

	return (retval);
}

/*

split_node

- tests if a new root node must be created
	+ if yes creates a new root node
- creates a new node on the same level
- copies all relevant information from upper half of node to be split to bottom of new node
- inserts middle data set into parent node, which can be the new root node

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_nodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::split_node (_t_nodeiter nNode)
{
	_t_nodeiter					nRetval, nParent;
	_t_data						sData;
	bool						bNewRoot;
	_t_subnodeiter				nSubPos;
	node_t						sNodeDesc;
	uint32_t					i;

	get_node (nNode, sNodeDesc);

#if defined (_DEBUG)
	BTREE_ASSERT (get_data_count (sNodeDesc) == get_node_max_data_size (), "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::split_node: split node called with inconsistent node");
#endif

	bNewRoot = sNodeDesc.nParent == nNode;

	// if the root node has to be split ...
	if (bNewRoot)
	{
		// ... then prepare a new root node

		// allocate new node
		nParent = create_node ((_t_nodeiter) ~0);

		// update root node cache

		// read old root node from root cache
		node_t tempNode2;
		
//		get_node (m_nRootNode, tempNode2);

		// disable root cache
//		m_bRootCacheValid = false;

		// write old node data to normal cache
//		set_node (m_nRootNode, tempNode2);

		// set new root
		m_nRootNode = nParent;

		// read new root from normal cache
		get_node (m_nRootNode, tempNode2);

		// set new root cache node number
//		m_nRootCacheRoot = m_nRootNode;

		// enable root cache
//		m_bRootCacheValid = true;

		// set new root description data
		tempNode2.nParent = m_nRootNode;
		tempNode2.nMaxIdx = sNodeDesc.nMaxIdx;

		// write new root to root cache
		set_node (m_nRootNode, tempNode2);

		// set new childs parent to root
		sNodeDesc.nParent = m_nRootNode;

		// maintain integrity by adding child to new root
		set_sub_node (nParent, 0, nNode);
	}
	else
	{
		// otherwise create a new node on the same level as the existing node to be split
		nParent = sNodeDesc.nParent;
	}

	// allocate space for the new node
	nRetval = create_node (sNodeDesc.nParent);

	// append node to be split and new node to rebuild FIFO
	rebuild_FIFO_put (nRetval);
	rebuild_FIFO_put (nNode);

	// if a new root node is to be added ...
	if (bNewRoot)
	{
		// ... then append the new root to the rebuild FIFO
		rebuild_FIFO_put (m_nRootNode);
	}

	{
		node_t		tempNewNode;
		_t_nodeiter	*pSubNodeBase, *pNewSubNodeBase;

		// load newly created node
		get_node (nRetval, tempNewNode);

		if (!is_leaf (nNode))
		{
			// copying generic subnode buffer points to clear text named buffer pointers
			pSubNodeBase = m_pSubNodeBuffer0;
			pNewSubNodeBase = m_pSubNodeBuffer1;

			// load sub node vector of node to be split
			get_sub_node_buffer (nNode, pSubNodeBase);
					
			// copy sub node vector upper half from node to be split to bottom of new node
			memcpy ((void *) pNewSubNodeBase, (void *) &(pSubNodeBase[m_nNodeSize]), sizeof (*pSubNodeBase) * (size_t)m_nNodeSize);

			// write-back new node's sub node vector
			set_sub_node_buffer (nRetval, pNewSubNodeBase, (uint32_t) m_nNodeSize);
		}

		_t_data		*pNodeData = m_pData_tobj0;
		_t_data		*pNewNodeData = m_pData_tobj1;

		// data from node to be split
		get_data_buffer (nNode, pNodeData);
		
		// copy data of upper half from node to be split to bottom of new node
		memcpy ((void *) pNewNodeData, (void *) &(pNodeData[m_nNodeSize]), sizeof (*pNodeData) * (size_t)(m_nNodeSize - 1ULL));

		// copy data at middle position to object to be moved to parent node
		sData = pNodeData[m_nNodeSize - 1ULL];

		// write-back new node's data
		set_data_buffer (nRetval, pNewNodeData, (uint32_t)m_nNodeSize - 1ULL);

		// if split happened on leaf node level ...
		if (is_leaf (nNode))
		{
			// ... then only update the data count of the node that was split as well as the new node
			sNodeDesc.nNumData = (0 - (m_nNodeSize - 1));
			tempNewNode.nNumData = (0 - (m_nNodeSize - 1));
		}
		else
		{
			// ... otherwise update the data count of the node that was split as well as the new node
			sNodeDesc.nNumData = m_nNodeSize - 1;
			tempNewNode.nNumData = m_nNodeSize - 1;

			// and update all sub nodes parent information which are addressed by the new node
			for (i = 0; i < m_nNodeSize; i++)
			{
				node_t		sNodeDesc;
				
				get_node (pNewSubNodeBase[i], sNodeDesc);

				sNodeDesc.nParent = nRetval;

				set_node (pNewSubNodeBase[i], sNodeDesc);
			}
		}

		// write-back new node and node that was split
		set_node (nNode, sNodeDesc);
		set_node (nRetval, tempNewNode);
	}

	// if a new root node is to be created ...
	if (bNewRoot)
	{
		// ... then the first data set written is always at the initial position
		nSubPos = 0;
	}
	else
	{
		// determine the splitted node's position in its parent node
		nSubPos = find_sub_node_offset (nParent, nNode);

#if defined (_DEBUG)
		BTREE_ASSERT (nSubPos >= 0, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::split_node: subPos not found in parent node");
#endif
	}

	// insert former middle positioned data into parent node
	insert_data_into_node (nParent, nSubPos, sData, nRetval, 1);

	// return newly created node identifier
	return (nRetval);
}

/*

remove_from_node

- 

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_from_node (_ti_pos sPos, _t_nodeiter nNode, uint32_t nDepth)
{
	_t_sizetype				nRetval = 0;
	_t_subnodeiter			nSub;
	_t_subnodeiter			nSubData;
	_ti_pos					sNewPos;
	bool					bFound;
	
	// store node identifier on rebuild stack
	attache_traced_node (nNode);

	// determine position or sub node position of data set in question and copy position to variable "nSub"
	sNewPos = determine_position (sPos, nNode, nSub, nSubData, bFound);

	// if node is not a leaf ...
	if (!is_leaf (nNode))
	{
		// ... then determine what to do in order to remove the data set in question
		_t_sizetype			nLeftSize, nRightSize;
		_t_nodeiter			nLeftNode, nRightNode, nNextNode;
		_t_subnodeiter		nNewSub;
		bool				bMergeNodes, bRotateNodes;
	
		do
		{
			bRotateNodes = false;
			bMergeNodes = false;
			
			nLeftNode = get_sub_node (nNode, nSubData);
			nRightNode = get_sub_node (nNode, nSubData + 1);

			nLeftSize = get_data_count (nLeftNode);
			nRightSize = get_data_count (nRightNode);

			// if left and right node have size t - 1 ...
			if ((nLeftSize == (m_nNodeSize - 1ULL)) && (nRightSize == (m_nNodeSize - 1ULL)))
			{
				// ... then mark them for a merge
				bMergeNodes = true;
			}
			// if one of the nodes has size t - 1 and the other is larger ...
			else if (((nLeftSize == (m_nNodeSize - 1ULL)) && (nRightSize >= m_nNodeSize)) ||
					((nRightSize == (m_nNodeSize - 1ULL)) && (nLeftSize >= m_nNodeSize)))
			{
				// ... then mark the for a rotate
				bRotateNodes = true;

				// - the following code covers a corner case where a rotate is potentially counter productive
				// - that is the case if data is rotated away from a leaf node in which the actual remove will 
				// happen in and would leave the tree in an undefined state
				
				// determine if data in question is in left node
				bool	bDataIsInLeftNode = nLeftNode == get_sub_node (nNode, nSub);

				// if data is in left node ...
				if (bDataIsInLeftNode)
				{
					// ... and left the left node larger than t - 1 ...
					if (nLeftSize >= m_nNodeSize)
					{
						// ... then a rotate is counter productive
						bRotateNodes = false;
					}
				}
				else
				{
					// if data is in right node and right node is larger ...
					if (nRightSize >= m_nNodeSize)
					{
						// ... then a rotate is also counter productive
						bRotateNodes = false;
					}
				}
			}

			// perform any previously determined operation
			if (bMergeNodes)
			{
				merge_node (nNode, nSubData);
			}
			else if (bRotateNodes)
			{
				rotate (nNode, nSubData, nNewSub, nLeftSize > nRightSize);
			}
			
			// if any operation took place ...
			if (bMergeNodes || bRotateNodes)
			{
				// ... then re-determine position
				sNewPos = determine_position (sPos, nNode, nSub, nSubData, bFound);
			}
		// it has to be checked if node (nNode) has become a leaf node, since merge_node can do that
		} while ((bMergeNodes || bRotateNodes) && !is_leaf (nNode));

		// if data in question has been found ...
		if (bFound)
		{
			// ... then remove data

			// if data is in a leaf node ...
			if (is_leaf (nNode))
			{
				// ... then remove from leaf
				nRetval = 1;

				remove_data_from_leaf (nNode, nSubData);
			}
			else
			{
				// ... otherwise take a copy from the nearest adjacent position
				// and remove the source of the copy

				nLeftSize = get_max_index (nLeftNode);
				nRightSize = get_max_index (nRightNode);

				// replace data
				replace_with_adjacent_entry (nNode, nSub, nLeftSize > nRightSize);

				// if left sub-tree is larger then right sub-tree ...
				if (nLeftSize > nRightSize)
				{
					// ... then determine nearest position on the left
					sNewPos = generate_prev_position (nNode, nSubData, sPos);
				}
				else
				{
					// ... otherwise determine nearest position on the right
					sNewPos = generate_next_position (nNode, nSubData, sPos);
				}

				// remove data from newly determined position, which is the source location of the copy from before
				nRetval = remove_from_node (sNewPos, nNode, nDepth + 1);

				rebuild_node (nNode);
			}
		}
		else
		{
			// ... otherwise determine sub-tree
			nNextNode = get_sub_node (nNode, nSub);

			// and remove data from sub-tree
			nRetval = remove_from_node (sNewPos, nNextNode, nDepth + 1);

			rebuild_node (nNode);
		}
	}
	else
	{
		// if data is on a leaf node, then remove it from leaf
		nRetval = 1;

		remove_data_from_leaf (nNode, nSubData);
	}

	// remove node identifier from rebuild stack
	detache_traced_node ();

	return (nRetval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_nodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node (_t_nodeiter nNode, _t_subnodeiter nSub)
{
	node_t	sNodeDesc;
	
	get_node (nNode, sNodeDesc);

#if defined (_DEBUG)
	BTREE_ASSERT (!is_leaf (sNodeDesc), "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: cannot merge from leaf node");
	BTREE_ASSERT (nSub < get_data_count (sNodeDesc), "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: cannot merge sub nodes beyond parents data size");
#endif

	_t_nodeiter				nRetval;
	_t_nodeiter				nLeftNode, nRightNode;
	_t_subnodeiter			nLeftSize, nRightSize;
	_t_data					sData;

	nLeftNode = get_sub_node (nNode, nSub);
	nRightNode = get_sub_node (nNode, nSub + 1);

	nLeftSize = get_data_count (nLeftNode);
	nRightSize = get_data_count (nRightNode);

	// if node (node must be root) is not a leaf node and has only one entry left ...
	if (get_data_count (sNodeDesc) == 1)
	{
		// ... then (special case applies) merge the two remaining sub-nodes and this node
		_t_data			*pLeftBuffer = m_pData_tobj_u0;
		_t_data			*pMergeBuffer = m_pData_tobj_u1;
		_t_data			*pRightBuffer = m_pData_tobj_u2;

#if defined (_DEBUG)
		BTREE_ASSERT (pLeftBuffer != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: insufficient memory");
		BTREE_ASSERT (pMergeBuffer != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: insufficient memory");
		BTREE_ASSERT (pRightBuffer != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: insufficient memory");
		BTREE_ASSERT (nNode == m_nRootNode, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: special case merge applies for root node only");
#endif

		// the merge result will be root
		nRetval = m_nRootNode;

		get_data_buffer (nLeftNode, pLeftBuffer);
		get_data_buffer (nNode, pMergeBuffer);
		get_data_buffer (nRightNode, pRightBuffer);

		// move data from initial entry to 'leftSize'-th entry in merge node
		memcpy ((void *) &(pMergeBuffer[nLeftSize]), (void *) pMergeBuffer, sizeof (*pMergeBuffer));

		// copy all left node data to the base of the merge node
		memcpy ((void *) pMergeBuffer, (void *) pLeftBuffer, sizeof (*pLeftBuffer) * nLeftSize);

		// copy all right node data to the top of the merge node
		memcpy ((void *) &(pMergeBuffer[nLeftSize + 1]), (void *) pRightBuffer, sizeof (*pRightBuffer) * nRightSize);

		// update merge node's data
		set_data_buffer (nNode, pMergeBuffer, nLeftSize + nRightSize + 1);

		// if the nodes being merged from were not leaf nodes ...
		if (!is_leaf (nLeftNode))
		{
			// ... then transfer sub node data as well
			_t_nodeiter			*pLeftSub = m_pSubNodeBuffer0;
			_t_nodeiter			*pMergeSub = m_pSubNodeBuffer1;
			_t_nodeiter			*pRightSub = m_pSubNodeBuffer2;
			_t_subnodeiter		ui32;

#if defined (_DEBUG)
			BTREE_ASSERT (pLeftSub != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: insufficient memory");
			BTREE_ASSERT (pMergeSub != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: insufficient memory");
			BTREE_ASSERT (pRightSub != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: insufficient memory");
#endif

			// get sub node data left, merge and right node
			get_sub_node_buffer (nLeftNode, pLeftSub);
			get_sub_node_buffer (nNode, pMergeSub);
			get_sub_node_buffer (nRightNode, pRightSub);

			// copy all left node sub node data to bottom half of merge node
			memcpy ((void *) pMergeSub, (void *) pLeftSub, sizeof (_t_nodeiter) * (nLeftSize + 1));

			// copy all right node sub node data to top half of merge node
			memcpy ((void *) &(pMergeSub[nLeftSize + 1]), (void *) pRightSub, sizeof (_t_nodeiter) * (nRightSize + 1));

			// store merged left and right sub node data in merge node
			set_sub_node_buffer (nNode, pMergeSub, nLeftSize + nRightSize + 2);

			// determine new size of merged node
			sNodeDesc.nNumData = nLeftSize + nRightSize + 1;

			// update merge node descriptor
			set_node (nNode, sNodeDesc);

			// every node that was previously refering to the left or right node as their parent, will now refer to the merge node
			for (ui32 = 0UL; ui32 < (nLeftSize + nRightSize + 2); ui32++)
			{
				// load node descriptor
				node_t sNodeDesc;
				
				get_node (pMergeSub[ui32], sNodeDesc);

				// update node descriptor
				sNodeDesc.nParent = nNode;

				// store node descriptor
				set_node (pMergeSub[ui32], sNodeDesc);
			}

			// update merge node
			rebuild_node (nNode);
		}
		else // if the nodes being merged from were leaf nodes ...
		{
			// ... then determine new size of merge node 
			sNodeDesc.nNumData = 0 - (_t_subnodeiter) sNodeDesc.nMaxIdx;
			
			// and update merge node descriptor
			set_node (nNode, sNodeDesc);
		}

		// remove left and right node from reservation list
		unset_reservation (nLeftNode);
		unset_reservation (nRightNode);

		// if the left node was smaller than the next node to allocate ...
		if (nLeftNode < m_nNextAlloc)
		{
			// ... then the former left node is the next to allocate from
			m_nNextAlloc = nLeftNode;
		}

		// if the right node was smaller than the next node to allocate ...
		if (nRightNode < m_nNextAlloc)
		{
			// ... then the former right node is the next to allocate from
			m_nNextAlloc = nRightNode;
		}

		return (nRetval);
	}

	// if node is a leaf node or has more than one entry then this code path is executed

	// mark left node as return value
	nRetval = nLeftNode;

	// create a copy of the entry to be removed during the merge and remove it from the merge node
	sData = get_data (nNode, nSub);
	remove_data_from_node (nNode, nSub, nSub + 1);

	nLeftSize = get_data_count (nLeftNode);
	nRightSize = get_data_count (nRightNode);

	_t_data		*pBaseLeft = m_pData_tobj0;
	_t_data		*pBaseRight = m_pData_tobj1;

#if defined (_DEBUG)
	BTREE_ASSERT (pBaseLeft != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: insufficient memory");
	BTREE_ASSERT (pBaseRight != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: insufficient memory");
#endif

	// load left and right node's data
	get_data_buffer (nLeftNode, pBaseLeft);
	get_data_buffer (nRightNode, pBaseRight);

	// append previously created entry copy that has been removed from the merge node to left node's data
	pBaseLeft[nLeftSize] = sData;

	// append right node's data to left node's data
	memcpy ((void *) &(pBaseLeft[nLeftSize + 1]), (void *) pBaseRight, sizeof (*pBaseLeft) * nRightSize);

	// store left node's data including newly appended data
	set_data_buffer (nLeftNode, pBaseLeft, nLeftSize + nRightSize + 1);

	// if the node being merged to is not a leaf node ...
	if (!is_leaf (nLeftNode))
	{
		// ... then merge sub node data as well
		_t_nodeiter			*pLeftSub = m_pSubNodeBuffer0;
		_t_nodeiter			*pRightSub = m_pSubNodeBuffer1;
		_t_subnodeiter		ui32;

#if defined (_DEBUG)
		BTREE_ASSERT (pLeftSub != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: insufficient memory");
		BTREE_ASSERT (pRightSub != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: insufficient memory");
#endif

		// load left and right sub node's data
		get_sub_node_buffer (nLeftNode, pLeftSub);
		get_sub_node_buffer (nRightNode, pRightSub);

		// append right node's sub node data to left node's sub node data
		memcpy ((void *) &(pLeftSub[nLeftSize + 1]), (void *) pRightSub, sizeof (*pLeftSub) * (nRightSize + 1));

		// set right node's sub nodes to refer to the left node as their parent
		for (ui32 = 0UL; ui32 < (nRightSize + 1); ui32++)
		{
			// load next right node's sub node
			node_t sNodeDesc;
			
			get_node (pRightSub[ui32], sNodeDesc);

#if defined (_DEBUG)
			BTREE_ASSERT (sNodeDesc.nParent == nRightNode, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: unexpected parent node found while merging");
#endif

			// update sub node to left node as its parent
			sNodeDesc.nParent = nLeftNode;

			// store right node's sub node
			set_node (pRightSub[ui32], sNodeDesc);
		}

		// load left node's descriptor
		node_t sNodeDesc;
		
		get_node (nLeftNode, sNodeDesc);

		// update left node's size after the merge
		sNodeDesc.nNumData = nLeftSize + nRightSize + 1;

		// store left node's descriptor
		set_node (nLeftNode, sNodeDesc);

		// store left node's sub node data
		set_sub_node_buffer (nLeftNode, pLeftSub, get_data_count (sNodeDesc) + 1);
	}
	else // if the node being merged to is a leaf node ...
	{
		// load left node's descriptor
		node_t sNodeDesc;
		
		get_node (nLeftNode, sNodeDesc);

		// update left node's size after the merge
		sNodeDesc.nNumData = 0 - (nLeftSize + nRightSize + 1);

		// store left node's descriptor
		set_node (nLeftNode, sNodeDesc);
	}

	// remove right node from reservation list
	unset_reservation (nRightNode);

	// if the right node was smaller than the next node to allocate ...
	if (nRightNode < m_nNextAlloc)
	{
		// ... then the former right node is the next to allocate from
		m_nNextAlloc = nRightNode;
	}

	// update left and merge node
	rebuild_node (nLeftNode);
	rebuild_node (nNode);

	return (nRetval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rotate (_t_nodeiter nNode, _t_subnodeiter nSub, _t_subnodeiter &newSub, bool bLeftToRight)
{
	_t_nodeiter				nLeftNode, nRightNode, nSubNode;
	_t_nodeiter				nSrcNode, nDstNode;
	_t_subnodeiter				nSrcSub, nDstSub;
	_t_subnodeiter				nSrcData;
	_t_data					sData;
	node_t					sDescLeft, sDescRight;
	int					triMod;

	// determine left and right sub node
	nLeftNode = get_sub_node (nNode, nSub);
	nRightNode = get_sub_node (nNode, nSub + 1);

	// get left and right sub node's description
	get_node (nLeftNode, sDescLeft);
	get_node (nRightNode, sDescRight);

	// if rotate is set from right to left ...
	if (!bLeftToRight)
	{
		// ... then set up left as destination and right as source
		nDstNode = nLeftNode;
		nDstSub = get_data_count (sDescLeft);
		nSrcNode = nRightNode;
		nSrcData = 0;
		nSrcSub = 0;
		triMod = 1;
	}
	else
	{
		// ... otherwise set up right as destination and left as source
		nDstNode = nRightNode;
		nDstSub = 0;
		nSrcNode = nLeftNode;
		nSrcData = get_data_count (nLeftNode);
		nSrcSub = nSrcData - 1;
		triMod = 0;
	}

	// set newSub return value
	newSub = nDstSub;

	// get data from rotate over position
	sData = get_data (nNode, nSub);

	// if next level is leaf layer ...
	if ((is_leaf (nLeftNode)) && (is_leaf (nRightNode)))
	{
		// ... then copy data into destination leaf node
		insert_data_into_leaf (nDstNode, nDstSub, sData);
	}
	else if ((!is_leaf (nLeftNode)) && (!is_leaf (nRightNode)))
	{
		// ... otherwise copy data into destination node
		nSubNode = get_sub_node (nSrcNode, nSrcData);

		insert_data_into_node (nDstNode, nDstSub, sData, nSubNode, triMod);
	}
	else
	{
		BTREE_ASSERT (false, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rotate: two nodes which have the same parent are not on the same level");
	}

	// get data from source position
	sData = get_data (nSrcNode, nSrcSub);

	// copy data into rotate over position
	replace_data (nNode, nSub, sData);

	// if next level is leaf layer ...
	if ((is_leaf (nLeftNode)) && (is_leaf (nRightNode)))
	{
		// ... then remove data from source leaf node
		remove_data_from_leaf (nSrcNode, nSrcSub);
	}
	else if ((!is_leaf (nLeftNode)) && (!is_leaf (nRightNode)))
	{
		// ... otherwise remove data from source node
		remove_data_from_node (nSrcNode, nSrcSub, nSrcData);
	}
	else
	{
		BTREE_ASSERT (false, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rotate: two nodes which have the same parent are not on the same level");
	}

	// destination node and rotate over node need to have their indexing updated
	// the source node was already updated during the remove
	rebuild_node (nDstNode);
	
	rebuild_node (nNode);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::replace_with_adjacent_entry (_t_nodeiter nNode, _t_subnodeiter nSub, bool bFromLeft)
{
#if defined (_DEBUG)
	BTREE_ASSERT (!is_leaf (nNode), "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::replace_with_adjacent_entry: method has been used on a leaf node!");
#endif

	_t_data				sData;
	_t_nodeiter			nAdjNode;
	_t_subnodeiter		nAdjSub;
	bool				bBounce;

	// if entry has to be copied from the nearest previous position ...
	if (bFromLeft)
	{
		// ... then determine nearest previous position
		move_prev (nNode, nSub, nAdjNode, nAdjSub, bBounce);
	}
	else
	{
		// ... otherwise determine nearest following position
		move_next (nNode, nSub, nAdjNode, nAdjSub, bBounce);
	}

#if defined (_DEBUG)
	BTREE_ASSERT (bBounce == false, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::replace_with_adjacent_entry: While looking for an adjacent entry, the method unexpectedly hit a termination!"); 

	BTREE_ASSERT (is_leaf (nAdjNode), "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::replace_with_adjacent_entry: Adjacent node must be a leaf node!");
#endif
	
	// get data from previously determined nearest position
	sData = get_data (nAdjNode, nAdjSub);

	// copy data to input location
	replace_data (nNode, nSub, sData);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::serialize (const _ti_pos nFrom, const _t_sizetype nLen, _t_data *pData, bool bReadOpr)
{
	_t_sizetype						nRslt = 0;
	bool							bFound;
	bool							bBounce;
	_t_nodeiter						nNode = this->m_nRootNode;
	_t_subnodeiter					nSubPos, nSubData;
	_t_nodeiter						nNextNode;
	_t_subnodeiter					nNextSubPos;
	_ti_pos							sPos;
	_t_sizetype						i;

	create_root ();

	sPos = nFrom;

	// translate start position to node / sub-position
	do
	{
		sPos = determine_position (sPos, nNode, nSubPos, nSubData, bFound);

		if (!bFound)
		{
			nNode = this->get_sub_node (nNode, nSubPos);
		}
	} while (!bFound);

	// serialize data
	for (i = 0; i < nLen; )
	{
		if (this->is_leaf (nNode))
		{
			_t_subnodeiter	nRem = (_t_subnodeiter) (nLen - i);

			if ((nSubData == 0) && (nRem >= (_t_sizetype) this->get_data_count (nNode)))
			{
				nRem = this->get_data_count (nNode);

				if (bReadOpr)
				{
					this->get_data_buffer (nNode, &(pData[i]));
				}
				else
				{
					this->set_data_buffer (nNode, &(pData[i]), nRem);
				}
			}
			else
			{
				_t_data			*pDataBuf = new _t_data [this->get_data_count (nNode)];
				
				BTREE_ASSERT (pDataBuf != NULL, "CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::serialize (nFrom, nTo, *pData): insufficient memory!");

				this->get_data_buffer (nNode, pDataBuf);

				if ((this->get_data_count (nNode) - nSubData) < nRem)
				{
					nRem = this->get_data_count (nNode) - nSubData;
				}

				if (bReadOpr)
				{
					memcpy ((void *) &(pData[i]), (void *) &(pDataBuf[nSubData]), sizeof (_t_data) * nRem);
				}
				else
				{
					memcpy ((void *) &(pDataBuf[nSubData]), (void *) &(pData[i]), sizeof (_t_data) * nRem);

					this->set_data_buffer (nNode, pDataBuf, this->get_data_count (nNode));
				}

				delete [] pDataBuf;
			}

			nSubPos = this->get_data_count (nNode);

			this->move_next (nNode, nSubPos, nNextNode, nNextSubPos, bBounce);

			i += (_t_sizetype) nRem;
			nRslt += (_t_sizetype) nRem;
		}
		else
		{
			if (bReadOpr)
			{
				pData[i] = this->get_data (nNode, nSubPos);
			}
			else
			{
				this->set_data (nNode, nSubPos, pData[i]);
			}

			this->move_next (nNode, nSubPos, nNextNode, nNextSubPos, bBounce);

			i++;
			nRslt++;
		}

		if (bBounce)
		{
			break;
		}

		nSubData = 0;

		nNode = nNextNode;
		nSubPos = nNextSubPos;
	}

	return (nRslt);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert_data_into_leaf (_t_nodeiter nNode, _t_subnodeiter nSubPos, const _t_data &rData)
{
	return (insert_data_into_node (nNode, nSubPos, rData, (_t_nodeiter) ~0x0, -1));
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert_data_into_node (_t_nodeiter nNode, _t_subnodeiter nSubPos, const _t_data &rData, _t_nodeiter nSubNode, int triMod)
{
	node_t					sNodeDesc;
	_t_nodeiter				nNewNode;
	_t_subnodeiter			nNumData;
	
#if defined (_DEBUG)
	BTREE_ASSERT (get_data_count (nNode) <= get_node_max_data_size (), "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert_data_into_leaf: node exceeds possible data entries");
#endif

	// load node descriptor
	get_node (nNode, sNodeDesc);

	// determine node size
	nNumData = get_data_count (sNodeDesc);

	// if the node's size has reached its maximum ...
	if (nNumData >= get_node_max_data_size ())
	{
		// ... then split node and divide it to node and nodeG
		// the bottom half will remain in the node referred by the variable 'node'
		// the top half will be copied to the node referred by the variable 'newNode'
		nNewNode = split_node (nNode);

		// if entry to be added is beyond the maximum node size half ...
		if (nSubPos >= m_nNodeSize)
		{
			// ... then add entry to the node newly created 'newNode'
			nNode = nNewNode;

			// load newNode descriptor
			get_node (nNode, sNodeDesc);

			// update sub position due to new node generated
			nSubPos -= (uint32_t)m_nNodeSize;
		}
		else // if entry to be added is not beyond the maximum node size half ...
		{
			// ... then load node descriptor
			get_node (nNode, sNodeDesc);
		}

		// re-determine node size after node split
		nNumData = get_data_count (sNodeDesc);
	}

	_t_nodeiter		*pSubNodes = m_pSubNodeBuffer0;

#if defined (_DEBUG)
	BTREE_ASSERT (pSubNodes != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert_data_into_leaf: insufficient memory");
#endif

	/*
	** triMod - three state modifier
	**
	** -1	- do not insert new sub node id
	** 0	- insert new sub node id prior to nSubPos
	** 1	- insert new sub node id subsequent to nSubPos
	*/

	// if new sub node id has to be inserted ...
	if (triMod != -1)
	{
		// ... then get sub node buffer
		get_sub_node_buffer (nNode, pSubNodes);

		node_t sSubNodeDesc;
		
		// update parent of inserted node
		get_node (nSubNode, sSubNodeDesc);
		sSubNodeDesc.nParent = nNode;
		set_node (nSubNode, sSubNodeDesc);

		// insert node into sub node buffer
		memmove ((void *) &(pSubNodes[nSubPos + triMod + 1]), (void *) &(pSubNodes[nSubPos + triMod]), sizeof (*pSubNodes) * ((nNumData + 1) - (nSubPos + triMod)));
		pSubNodes[nSubPos + triMod] = nSubNode;

		// write sub node buffer back
		set_sub_node_buffer (nNode, pSubNodes, nNumData + 2);
	}
#if defined (_DEBUG)
	else
	{
		BTREE_ASSERT (is_leaf (sNodeDesc), "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert_data_into_leaf: insert_data_into_node expects either subNodeBeforeData or subNodeAfterData to contain a valid node index when calling no leaf node");
	}
#endif

	// insert one data entry into a node's buffer
	m_pData->insert_dataIntoPool (m_nPoolIDdata, nNode, get_data_count (sNodeDesc), nSubPos, 1, (void *) &rData);

	// if this node is not a leaf node ...
	if (triMod != -1)
	{
		// if this node used to be a leaf node ...
		if (is_leaf (sNodeDesc))
		{
			// ... then convert to inner node
			sNodeDesc.nNumData = 0 - sNodeDesc.nNumData;
		}

		// increment node size
		sNodeDesc.nNumData++;
	}
	else // if this node is a leaf node ...
	{
		// absolute number contains number of data entries - if zero or negativ then leaf node

		// increment leaf node size
		sNodeDesc.nNumData--;
		sNodeDesc.nMaxIdx++;
	}

	// store node descriptor
	set_node (nNode, sNodeDesc);

	this->update_time_stamp ();

	return (nSubPos);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_leaf (_t_nodeiter nNode, _t_subnodeiter nSub)
{
#if defined (_DEBUG)
	node_t		sNodeDesc;

	get_node (nNode, sNodeDesc);

	BTREE_ASSERT (is_leaf (sNodeDesc), "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_leaf: remove_data_from_leaf ought to be called on leaf nodes");
	BTREE_ASSERT (get_data_count (sNodeDesc) > 0, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_leaf: remove_data_from_leaf ought not to be called on empty nodes");
#endif

	// remove data entry from node
	remove_data_from_node (nNode, nSub, ~0x0);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_node (_t_nodeiter nNode, _t_subnodeiter nSubData, _t_subnodeiter nSubNode)
{
	node_t					sNodeDesc;
	_t_subnodeiter			nNumData;

	get_node (nNode, sNodeDesc);

	// determine node's data size
	nNumData = get_data_count (sNodeDesc);

#if defined (_DEBUG)
	BTREE_ASSERT (nSubData < nNumData, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_node: subData exceeds nodes data size");
	BTREE_ASSERT (nNumData > 0, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_node: cannot remove in an empty node");
#endif

	_t_data		*pBase = m_pData_tobj0;

#if defined (_DEBUG)
	BTREE_ASSERT (pBase != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_node: insufficient memory");
#endif
	// load all data from current node
	get_data_buffer (nNode, pBase);

	// remove entry
	memmove ((void *) &(pBase[nSubData]), (void *) &(pBase[nSubData + 1L]), sizeof (*pBase) * (nNumData - nSubData - 1L));

	// store all data from current node
	set_data_buffer (nNode, pBase, nNumData - 1ULL);

	// if node is not a leaf node ...
	if (!is_leaf (nNode))
	{
		// ... then remove sub-node entry
#if defined (_DEBUG)
		BTREE_ASSERT (nSubNode <= nNumData, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_node: subNode exceeds nodes sub entry size");
#endif

		_t_nodeiter		*pSubNodes = m_pSubNodeBuffer0;

#if defined (_DEBUG)
		BTREE_ASSERT (pSubNodes != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_node: insufficient memory");
#endif
		
		// load all sub-node data from current node
		get_sub_node_buffer (nNode, pSubNodes);

		// remove sub-node entry
		memmove ((void *) &(pSubNodes[nSubNode]), (void *) &(pSubNodes[nSubNode + 1L]), sizeof (*pSubNodes) * (nNumData - nSubNode));

		// store all data from current node
		set_sub_node_buffer (nNode, pSubNodes, nNumData);

		// decrement node size
		sNodeDesc.nNumData--;
	}
	else
	{
		// ... otherwise only decrement node size
		sNodeDesc.nNumData++;
	}

	// write-back node descriptor
	set_node (nNode, sNodeDesc);

	// update current node
	rebuild_node (nNode);

	auto_shrink_inc ();

	this->update_time_stamp ();
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::replace_data (_t_nodeiter nNode, _t_subnodeiter nSub, const _t_data &rData)
{
	// replace data by overwriting old data
	set_data (nNode, nSub, rData);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find_sub_node_offset (_t_nodeiter nNode, _t_nodeiter nSubNode)
{
	_t_subnodeiter		nRetval = ~0;
	_t_subnodeiter		i;
	_t_subnodeiter		nNumSub;
	_t_nodeiter			*pSubNodes = m_pSubNodeBuffer0;

#if defined (_DEBUG)
	BTREE_ASSERT (pSubNodes != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find_sub_node_offset: insufficient memory");
	BTREE_ASSERT (!is_leaf (nNode), "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find_sub_node_offset: method was used on leaf node!");
#endif

	// if node is not a leaf node ...
	if (!is_leaf (nNode))
	{
		// ... then seek for sub node in sub node list

		// load sub node list
		get_sub_node_buffer (nNode, pSubNodes);

		// determine size of sub node list
		nNumSub = get_sub_node_count (nNode);
		
		for (i = 0; i < nNumSub; i++)
		{
			// if node was found ...
			if (pSubNodes[i] == nSubNode)
			{
				// ... then return with position
				nRetval = i;

				break;
			}
		}
	}

	return (nRetval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::convert_pos_to_container_pos (_t_nodeiter nNode, _t_sizetype nPos, _t_nodeiter &rRsltNode, _t_subnodeiter &rRsltSubPos)
{
//	_t_subnodeiter							ui32;
	_t_subnodeiter							nMax;
	_t_subnodeiter							nMin;
	_t_subnodeiter							nMid;
	bool									isLeaf = is_leaf (nNode);
	_t_sizetype								nMaxIdx;

	// if nNode is inner node ...
	if (!isLeaf)
	{
		// ... then search for the next sub node or find data in current node
		this->get_serVector (nNode, this->m_pSerVector);

		nMax = this->get_data_count (nNode);
		nMin = 0;

		while (nMax != nMin)
		{
			nMid = (nMax + nMin) / 2;

			if (nMid >= this->get_data_count (nNode))
			{
				nMaxIdx = this->get_max_index (nNode);
			}
			else
			{
				nMaxIdx = this->m_pSerVector[nMid];
			}

			if (nPos == nMaxIdx)
			{ // if data is in current node, then return with result immediately

				rRsltNode = nNode;
				rRsltSubPos = nMid;
				
				return;
			}

			if (nPos < nMaxIdx)
			{
				nMax = nMid;
			}
			else
			{
				nMin = nMid + 1;
			}
		}

		if (nMin > 0)
		{
			if (nMin >= this->get_data_count (nNode))
			{
				nPos -= this->m_pSerVector[this->get_data_count (nNode) - 1] + 1;
			}
			else
			{
				nPos -= this->m_pSerVector[nMin - 1] + 1;
			}
		}

		convert_pos_to_container_pos (get_sub_node (nNode, nMin), nPos, rRsltNode, rRsltSubPos);

/*		for (ui32 = 0UL; ui32 < nNumSubNodes; ui32++)
		{
			// get sub tree size
			nMaxIdx = get_max_index (get_sub_node (nNode, ui32));

			// if linear position is within the current sub tree ...
			if (nPos < nMaxIdx)
			{
				// ... then seek for entry inside that sub tree
				convert_pos_to_container_pos (get_sub_node (nNode, ui32), nPos, rRsltNode, rRsltSubPos);

				return;
			}
			else if (nPos == nMaxIdx)
			{ // if data is in current node, then return with result immediately

				rRsltNode = nNode;
				rRsltSubPos = ui32;
				
				return;
			}

			// advance to next sub tree
			nPos -= nMaxIdx + 1ULL;
		}*/
	}
	else
	{
#if defined (_DEBUG)
		BTREE_ASSERT (nPos < 0x80000000ULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::convert_pos_to_container_pos: nodes sub element addressing exceeds possible node size");
#endif
		// ... otherwise return with result here

		rRsltNode = nNode;
		rRsltSubPos = (_t_subnodeiter) (nPos);
	}
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_reservation (_t_nodeiter nNode)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_reservation: requested reservation exceeds tree size");
#endif

	node_maintainence_t		sMaintenance;

	// read maintenancy vector of node
	m_pData->get_pooledData (m_nPoolIDnodeMaintenance, nNode, 1, (void *) &sMaintenance);

	// set reservation bit
	sMaintenance.uVector |= BAYERTREE_NODE_MAINTAINANCE_ALLOCATION;

	// write back maintenancy vector
	m_pData->set_pooledData (m_nPoolIDnodeMaintenance, nNode, 1, (void *) &sMaintenance);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_reservation (_t_nodeiter nNode)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_reservation: requested reservation information beyond tree size");
#endif

	node_maintainence_t		sMaintenance;

	// read maintenancy vector of node
	m_pData->get_pooledData (m_nPoolIDnodeMaintenance, nNode, 1, (void *) &sMaintenance);

	// return true if node is flagged as reserved, otherwise false
	return ((sMaintenance.uVector & BAYERTREE_NODE_MAINTAINANCE_ALLOCATION) != 0 ? true : false);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::unset_reservation (_t_nodeiter nNode)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::unset_reservation: requested reservation release exceeds tree size");
#endif

	node_maintainence_t		sMaintenance;

	// read maintenancy vector of node
	m_pData->get_pooledData (m_nPoolIDnodeMaintenance, nNode, 1, (void *) &sMaintenance);

	// unset reservation bit
	sMaintenance.uVector &= ~BAYERTREE_NODE_MAINTAINANCE_ALLOCATION;

	// write back maintenancy vector
	m_pData->set_pooledData (m_nPoolIDnodeMaintenance, nNode, 1, (void *) &sMaintenance);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reset_reservation (_t_nodeiter nNode)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reset_reservation: requested reservation release exceeds tree size");
#endif

	node_maintainence_t		sMaintenance;

	// read maintenancy vector of node
	m_pData->get_pooledData (m_nPoolIDnodeMaintenance, nNode, 1, (void *) &sMaintenance);

	// unset reservation bit
	sMaintenance.uVector  = 0;

	// write back maintenancy vector
	m_pData->set_pooledData (m_nPoolIDnodeMaintenance, nNode, 1, (void *) &sMaintenance);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_node_max_sub_nodes ()
{// returns the maximum number of sub node entries per node
	return (m_nNodeSize * 2);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_node_max_data_size ()
{// returns the maximum number of data entries per node
	return ((m_nNodeSize * 2) - 1);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::register_iterator (const_iterator *pIter)
{
#if defined (BTREE_ITERATOR_REGISTRATION)

	if (m_psIterRegister->count (pIter) == 0)
	{
		m_psIterRegister->insert (pIter);
	}

#endif
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::unregister_iterator (const_iterator *pIter)
{
#if defined (BTREE_ITERATOR_REGISTRATION)

	m_psIterRegister->erase (pIter);

#endif
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data_count (const _t_nodeiter nNode)
{// returns the present number of entries in a node
	node_t		sNodeDesc;

	get_node (nNode, sNodeDesc);

	return (get_data_count (sNodeDesc));
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data_count (const node_t &sNodeDesc)
{// returns the present number of entries from a node descriptor
	_t_subnodeiter		nRetval;

	// if node descriptor is displaying a leaf node ...
	if (is_leaf (sNodeDesc))
	{
		// ... then return the inverted number of data entries, since leaf nodes store the their fill state as a 2's complement
		nRetval = (_t_subnodeiter) (0 - sNodeDesc.nNumData);
	}
	else
	{
		// ... otherwise return fill state
		nRetval = (_t_subnodeiter) sNodeDesc.nNumData;
	}

	return (nRetval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node_count (const _t_nodeiter nNode) 
{// returns the present number of sub nodes
	node_t		sNodeDesc;

	get_node (nNode, sNodeDesc);

	return (get_sub_node_count (sNodeDesc));
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node_count (const node_t &sNodeDesc)
{
	_t_subnodeiter		nRetval;

	// if node descriptor is displaying a leaf node ...
	if (is_leaf (sNodeDesc))
	{
		// ... then return zero sub nodes
		nRetval = (_t_subnodeiter) 0;
	}
	else
	{
		// ... otherwise return number of sub nodes
		nRetval = (_t_subnodeiter) (sNodeDesc.nNumData + 1);
	}

	return (nRetval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_max_index (const _t_nodeiter nNode)
{// returns number of data entries in sub tree contain node (nNode)
	node_t		sNodeDesc;

	get_node (nNode, sNodeDesc);

	return (get_max_index (sNodeDesc));
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype  CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_max_index (const node_t &sNodeDesc)
{// returns number of data entries in sub tree displayed by node descriptor
	return (sNodeDesc.nMaxIdx);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_nodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_parent (const _t_nodeiter nNode)
{
	node_t		sNodeDesc;

	get_node (nNode, sNodeDesc);

	return (get_parent (sNodeDesc));

}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_nodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_parent (const node_t &sNodeDesc)
{
	return (sNodeDesc.nParent);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::is_root (const _t_nodeiter nNode)
{
	return (nNode == this->m_nRootNode);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::is_leaf (const _t_nodeiter nNode)
{// returns if node (nNode) is a leaf node or an inner node
	node_t		sNodeDesc;

	get_node (nNode, sNodeDesc);

	return (is_leaf (sNodeDesc));
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::is_leaf (const node_t &nodeDesc)
{
	const _t_subnodeiter		nMask = ~(((_t_subnodeiter) ~0) >> 1);
	bool						bRslt = nodeDesc.nNumData == 0;

	bRslt |= ((nMask & nodeDesc.nNumData) != 0x0);

	// returns if true if node descriptor displays a leaf node, otherwise false
	return (bRslt);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_node (const _t_nodeiter nNode, node_t &sDesc)
{
#if defined (_DEBUG)

	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_node: requested node exceeds tree size");

#endif

	// if root node is present in root node cache and the node to be returned is the root node ...
/*	if (m_bRootCacheValid && (m_nRootCacheRoot == nNode))
	{
		// ...  then return root cache
		sDesc = m_sRootCacheNode;

		return;
	}*/

	// otherwise read node from IO layer
	m_pData->get_pooledData (m_nPoolIDnodeDesc, nNode, 1, (void *) &sDesc);

	// if root node is not present in root node cache and the node to be returned is the root node ...
/*	if (!m_bRootCacheValid && (m_nRootCacheRoot == nNode))
	{
		// ... then set root node cache to node description returned from IO layer
		m_sRootCacheNode = sDesc;

		// assert root node cache valid
		m_bRootCacheValid = true;
	}*/
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_node (_t_nodeiter nNode, node_t &sDesc)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_node: requested node exceeds tree size");
#endif

	// if root node is present in root node cache and the node to be returned is the root node ...
/*	if (m_bRootCacheValid && (m_nRootCacheRoot == nNode))
	{
		// ... then set root node cache to input node description
		m_sRootCacheNode = sDesc;

		return;
	}*/

	// otherwise write node descriptor to IO layer
	m_pData->set_pooledData (m_nPoolIDnodeDesc, nNode, 1, (void *) &sDesc);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_nodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node (_t_nodeiter nNode, _t_subnodeiter nEntry)
{
	_t_nodeiter		nRetval;

#if defined (_DEBUG)
	node_t	sNodeDesc;
	
	get_node (nNode, sNodeDesc);

	BTREE_ASSERT (!is_leaf (nNode), "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node: get_sub_node cannot be used with leaf nodes");
	BTREE_ASSERT (nEntry <= get_data_count (sNodeDesc), "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node: requested sub position exceeds node size");
	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node: requested node exceeds tree size");
#endif

	// read sub node ID from IO layer
	m_pData->get_pooledData (m_nPoolIDsubNodes, nNode, get_data_count (nNode) + 1, nEntry, (void *) &nRetval);

	return (nRetval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_sub_node (_t_nodeiter nNode, _t_subnodeiter nEntry, _t_nodeiter nSubNode)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_sub_node: requested node exceeds tree size");
#endif

	// write sub node ID to IO layer
	m_pData->set_pooledData (m_nPoolIDsubNodes, nNode, get_data_count (nNode) + 1, nEntry, (void *) &nSubNode);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node_buffer (_t_nodeiter nNode, _t_nodeiter *pBuffer)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node_buffer: requested node exceeds tree size");
#endif

	node_t	sNodeDesc;
	
	get_node (nNode, sNodeDesc);

	// read sub node buffer from IO layer
	m_pData->get_pooledData (m_nPoolIDsubNodes, nNode, get_data_count (nNode) + 1, (void *) pBuffer);

	// in the above call get_data_count is in use instead of get_sub_node_count
	// In the event a new node is created it has size zero and would therefore be deemed
	// as a leaf node. In that case get_sub_node_count would always return zero, which is incorrect here.
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_sub_node_buffer (_t_nodeiter nNode, _t_nodeiter *pBuffer, _t_subnodeiter nNumEntries)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_sub_node_buffer: requested node exceeds tree size");
#endif

	// write sub node buffer to IO layer
	m_pData->set_pooledData (m_nPoolIDsubNodes, nNode, nNumEntries, (void *) pBuffer);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_data CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data (_t_nodeiter nNode, _t_subnodeiter nEntry)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data: requested node exceeds tree size");

	BTREE_ASSERT (nEntry < get_node_max_data_size (), "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data: requested entry exceeds possible node size");
#endif

	_t_data			sRetval;

	// read data entry from IO layer
	m_pData->get_pooledData (m_nPoolIDdata, nNode, get_data_count (nNode), nEntry, (void *) &sRetval);

	return (sRetval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_data (_t_nodeiter nNode, _t_subnodeiter nEntry, _t_data sData)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_data: requested node exceeds tree size");
#endif

	// write data entry to IO layer
	m_pData->set_pooledData (m_nPoolIDdata, nNode, get_data_count (nNode), nEntry, (void *) &sData);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data_buffer (const _t_nodeiter nNode, _t_data *pBuffer)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data_buffer: requested node exceeds tree size");
#endif

	// read all data entries of one node from IO layer
	m_pData->get_pooledData (m_nPoolIDdata, nNode, get_data_count (nNode), pBuffer);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_data_buffer (const _t_nodeiter nNode, _t_data *pBuffer, const _t_subnodeiter nSize)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_data_buffer: requested node exceeds tree size");
#endif

	// write all data entries of one node to IO layer
	m_pData->set_pooledData (m_nPoolIDdata, nNode, nSize, pBuffer);
}

/*

get_serVector - get serial vector

This method retrieves the serial vector of one node and writes the result to the location
pVector is pointing at.

nNode		- specifies the node of which to retrieve the serial vector of
pVector		- specifies the pointer of the returned serial vector

Note:	The method assumes that the location pVector is pointing is big enough to
		contain all returned information.

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_serVector (_t_nodeiter nNode, _t_sizetype *pVector)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeBase<>::get_serVector: requested node exceeds tree size");
#endif

	this->m_pData->get_pooledData (m_nPoolIDserialVector, nNode, this->get_data_count (nNode), (void *) pVector);
}

/*

set_serVector - set serial vector

This method writes back the serial vector of one node, from the location
pVector is pointing at.

nNode		- specifies the node of which the serial vector has to be written back
pVector		- specifies the pointer of the serial vector to be written

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_serVector (_t_nodeiter nNode, _t_sizetype *pVector)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeBase<>::set_serVector: requested node exceeds tree size");
#endif

	this->m_pData->set_pooledData (m_nPoolIDserialVector, nNode, this->get_data_count (nNode), (void *) pVector);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::move_prev (_t_nodeiter nNode, _t_subnodeiter nSub, _t_nodeiter &nPrevNode, _t_subnodeiter &nPrevSubPos, bool &bBounce)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::move_prev: requested node exceeds tree size");
#endif

	// search for nearest left or previous entry
	// and return result in nPrevNode, nPrevSubPos and bBounce

	node_t		sNodeDesc;
	
	get_node (nNode, sNodeDesc);
	
	nPrevNode = nNode;
	nPrevSubPos = nSub;
	bBounce = false;

	// if leaf node ...
	if (is_leaf (sNodeDesc))
	{
		// ... and the left most position has not been reached ...
		if (nSub > 0)
		{
			// ... then decrement sub position and return
			nSub--;

			nPrevNode = nNode;
			nPrevSubPos = nSub;
		}
		else
		{
			// ... otherwise if leaf node and left most position has been reached, then seek for the one parent's parent node that contains the previous data entry
			do
			{
				// if during the search the root node is reached ...
				if (sNodeDesc.nParent == nNode)
				{
					// ... then assert that the beginning of the list has been found and return
					bBounce = true;

					return;
				}

				// find this node's position in the parent node
				nPrevSubPos = find_sub_node_offset (sNodeDesc.nParent, nNode);
				nNode = sNodeDesc.nParent;

				get_node (nNode, sNodeDesc);

			// continue until a parent node has been found, where the current node's position is not on the left
			} while (nPrevSubPos == 0);

			// the found parent node becomes the return node value
			nPrevNode = nNode;

			// and the position before the found sub position beecomes the return sub position
			nPrevSubPos--;
		}
	}
	else
	{
		// ... otherwise if this is not a leaf node, then seek for the leaf node, which contains the nearest previous data entry
		do
		{
			// get next sub node
			nNode = get_sub_node (nNode, nSub);

			get_node (nNode, sNodeDesc);

			// determine most right position within node
			nSub = get_data_count (sNodeDesc);
		} while (!is_leaf (nNode));

		// the found leaf node becomes the return node value
		nPrevNode = nNode;

		// the most right position within that leaf node becomes the return sub node position
		nPrevSubPos = get_data_count (sNodeDesc) - 1;
	}
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::move_next (_t_nodeiter nNode, _t_subnodeiter nSub, _t_nodeiter &nNextNode, _t_subnodeiter &nNextSubPos, bool &bBounce)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < m_nTreeSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::move_next: requested node exceeds tree size");
#endif

	// search for nearest left or previous entry
	// and return result in nNextNode, nNextSubPos and bBounce

	node_t		sNodeDesc;
	
	get_node (nNode, sNodeDesc);

	nNextNode = nNode;
	nNextSubPos = nSub;
	bBounce = false;

	// if leaf node ...
	if (is_leaf (sNodeDesc))
	{
		// ... and the right most position has not been reached ...
		if (nSub < (get_data_count (sNodeDesc) - 1))
		{
			// ... then increment sub position and return
			nSub++;

			nNextNode = nNode;
			nNextSubPos = nSub;
		}
		else
		{
			// ... otherwise if leaf node and right most position has been reached, then seek for the one parent's parent node that contains the next data entry
			do
			{
				// if during the search the root node is reached ...
				if (sNodeDesc.nParent == nNode)
				{
					// ... then assert that the beginning of the list has been found and return
					bBounce = true;

					return;
				}

				// find this node's position in the parent node - this also becomes the return sub position
				nNextSubPos = find_sub_node_offset (sNodeDesc.nParent, nNode);
				nNode = sNodeDesc.nParent;

				get_node (nNode, sNodeDesc);

				BTREE_ASSERT (nNextSubPos <= get_data_count (sNodeDesc), "move_next went beyond nodes max data entries");

			// continue until a parent node has been found, where the current node's position is not on the right
			} while (nNextSubPos == get_data_count (sNodeDesc));

			// the found parent node becomes the return node value
			nNextNode = nNode;
		}
	}
	else
	{
		// ... otherwise if this is not a leaf node, then increment to the next position and seek for the leaf node, which contains the nearest subsequent data entry
		nSub++;

		do
		{
			// get next sub node
			nNode = get_sub_node (nNode, nSub);

			get_node (nNode, sNodeDesc);

			// set sub position to left most position
			nSub = 0;
		} while (!is_leaf (nNode));

		// the found leaf node becomes the return node value
		nNextNode = nNode;

		// that leaf node's most left sub position becomes the return sub position value
		nNextSubPos = 0;
	}
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rebuild_FIFO_put (_t_nodeiter nNode)
{
#if defined (_DEBUG)
	BTREE_ASSERT (rebuild_FIFO_fillstate () <= m_nRebuildFIFOSize, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rebuild_FIFO_put: FIFO overflow");
#endif

	// if rebuild FIFO is full ...
	if (rebuild_FIFO_fillstate () == m_nRebuildFIFOSize)
	{
		// ... then allocate more space, copy current content to new space and destroy old space
		uint32_t		u;
		uint32_t		oldFillState = rebuild_FIFO_fillstate ();
		uint32_t		newRebuildFIFOsize = (m_nRebuildFIFOSize == 0) ? 2 : m_nRebuildFIFOSize << 1;
		_t_nodeiter		*pNew = new _t_nodeiter [newRebuildFIFOsize];

		BTREE_ASSERT (pNew != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rebuild_FIFO_put: insufficient memory");

		// copy rebuild FIFO content to new space
		for (u = 0UL; rebuild_FIFO_fillstate () > 0UL; u++)
		{
			pNew[u] = rebuild_FIFO_get ();
		}

		// update FIFO state according to new space
		m_nRebuildFIFOwritePos = oldFillState;
		m_nRebuildFIFOreadPos = 0UL;

		// if old space is valid ...
		if (m_pRebuildFIFO != NULL)
		{
			// ... then delete old space
			delete [] m_pRebuildFIFO;
		}

		// assign new space and maximum size to member variables
		m_pRebuildFIFO = pNew;
		m_nRebuildFIFOSize = newRebuildFIFOsize;
	}

	// put node into FIFO and increment write pointer
	m_pRebuildFIFO[(m_nRebuildFIFOwritePos++ & (m_nRebuildFIFOSize - 1UL))] = nNode;
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_nodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rebuild_FIFO_get ()
{
#if defined (_DEBUG)
	BTREE_ASSERT (m_nRebuildFIFOreadPos < m_nRebuildFIFOwritePos, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rebuild_FIFO_get: rebuild FIFO underrun");
#endif

	// if both the read and write pointer are exceeding the FIFO space ...
	while ((m_nRebuildFIFOreadPos > m_nRebuildFIFOSize) && (m_nRebuildFIFOwritePos > m_nRebuildFIFOSize))
	{
		// ... then rewind both until at least one is within FIFO space again
		m_nRebuildFIFOreadPos -= m_nRebuildFIFOSize;
		m_nRebuildFIFOwritePos -= m_nRebuildFIFOSize;
	}

	// return current node and increment read pointer
	return (m_pRebuildFIFO[m_nRebuildFIFOreadPos++ & (m_nRebuildFIFOSize - 1UL)]);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_nodeiter CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rebuild_FIFO_peek ()
{
#if defined (_DEBUG)
	BTREE_ASSERT (m_nRebuildFIFOreadPos < m_nRebuildFIFOwritePos, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rebuild_FIFO_peek: rebuild FIFO underrun");
#endif
	
	// return current node
	return (m_pRebuildFIFO[m_nRebuildFIFOreadPos & (m_nRebuildFIFOSize - 1UL)]);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
uint32_t CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rebuild_FIFO_fillstate ()
{
	// return number of items in rebuild FIFO
	// for this to work the write position must always be bigger than the read position
	return (m_nRebuildFIFOwritePos - m_nRebuildFIFOreadPos);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::attache_traced_node (_t_nodeiter nNode)
{
#if defined (_DEBUG)
	// only valid in debug

	// if node trace buffer not allocate ...
	if (m_pNodeTrace == NULL)
	{
		// ... then set minimum size and allocate trace buffer space
		m_nNodeTraceDepthMax = 1UL;

		m_pNodeTrace = new _t_nodeiter [m_nNodeTraceDepthMax];

		BTREE_ASSERT (m_pNodeTrace != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::attache_traced_node: insufficient memory!");
	}

	// if trace buffer is full ...
	if (m_nNodeTraceDepth >= m_nNodeTraceDepthMax)
	{
		// ... then allocate more a larger space, copy current buffer to larger space and de-allocte current buffer
		_t_nodeiter			*pNodeTraceTemp;
		uint32_t			nNodeTraceDepthTemp = m_nNodeTraceDepthMax;
		uint32_t			ui32;

		// allocte space twice as large than current space
		m_nNodeTraceDepthMax <<= 1UL;

		pNodeTraceTemp = new _t_nodeiter [m_nNodeTraceDepthMax];

		BTREE_ASSERT (m_pNodeTrace != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::attache_traced_node: insufficient memory!");

		// copy content to new space
		for (ui32 = 0; ui32 < nNodeTraceDepthTemp; ui32++)
		{
			pNodeTraceTemp[ui32] = m_pNodeTrace[ui32];
		}

		// de-allocate current trace buffer
		delete [] m_pNodeTrace;

		// assign new buffer as current buffer
		m_pNodeTrace = pNodeTraceTemp;
	}

	// put node into trace buffer and update pointer
	m_pNodeTrace[m_nNodeTraceDepth] = nNode;

	m_nNodeTraceDepth++;
#endif
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::detache_traced_node ()
{
#if defined (_DEBUG)
	// if trace buffer present ...
	if (m_pNodeTrace != NULL)
	{
		// ... then detach current node
		m_nNodeTraceDepth--;
	}
#endif
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::auto_shrink_inc ()
{
	// if auto shrink is due ...
	if (m_nAutoShrinkCounter >= m_nAutoShrinkThreshold)
	{
		// ... then set back auto shrink counter and run auto shrink
		m_nAutoShrinkCounter -= m_nAutoShrinkThreshold;

		auto_shrink ();
	}
	else
	{
		// ... otherwise, increment auto shrink counter
		m_nAutoShrinkCounter++;
	}
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::auto_shrink_dec ()
{
	// if auto shrink counter won't wrap on next decrement ...
	if (m_nAutoShrinkCounter > 0)
	{
		// ... then decrement counter
		m_nAutoShrinkCounter--;
	}
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::auto_shrink ()
{
	_t_nodeiter		i;
	_t_nodeiter		nNewSize;

	// find node with highest id
	for (i = m_nTreeSize; i > 0; )
	{
		i--;

		if (get_reservation (i))
		{
			break;
		}
	}

	nNewSize = i;

	// unload any remaining data still present in cache
	for (i++; i < m_nTreeSize; i++)
	{
		m_pData->unload_from_cache (i);
	}

	// the previous step is required, since although if a node is de-allocted its data in the cache is still marked as valid
	// if another node is requested, which shares the same cache line, then the write back to of the de-allocated node's data
	// is an attempt to write to a location via the IO layer that is no longer there and will most likely result in a seg-fault
	// or hit an assert

	// update IO layer and tree size
	m_pData->set_size (nNewSize);

	m_nTreeSize = nNewSize + 1;
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
float CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_average_access_depth ()
{
	if (this->empty ())
	{
		return (0.0f);
	}

	if (this->is_leaf (this->m_nRootNode))
	{
		return (1.0f);
	}

	float		fRetval = 1.0f;
	float		fRangeIncPerDepth;
	float		fSize = (float) this->size ();
	float		fRange = 1.0;
	float		fPrevRange;

	fRangeIncPerDepth = (float) this->get_node_max_sub_nodes ();
	fRangeIncPerDepth *= 0.75f;

	do
	{
		fRetval += 1.0f;

		fPrevRange = fRange;

		fRange *= fRangeIncPerDepth;
	} while (fRange < fSize);

	fRetval -= (fRange - fSize) / (fRange - fPrevRange);

	return (fRetval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::shownode (std::ofstream &ofs, _t_nodeiter nNode, _t_nodeiter nParent, char *pAlloc)
{
	node_t					sNodeDesc, sSubNodeDesc;
	uint64_t				ntab = 0ULL, notab = 0ULL, nNumIdx;
	_t_subnodeiter			i;
	bool					bError = false;
	std::string				strError;
	_t_nodeiter				nSerVectorTest;

	/*
	**	If argument ofs is a stream that is not open, then this method will
	**	operate in integrity test mode and exit with false if anything is
	**	incorrect or an exception occures. The integrity test mode exits
	**	with true if everything is correct.
	**	If argument ofs is an open stream, then the node's data is output in
	**	HTML format to that stream and the result is always true. If an error
	**	happens in the process, then the data this method "thinks" is correct
	**	as well as a description of what went wrong is output to the stream.
	*/

	try
	{
		get_node (nNode, sNodeDesc);
	}
	catch (exception *pE)
	{
		strError = pE->what ();

		bError = true;
	}

	if (ofs.is_open ())
	{
		ofs << "<table cellspacing=\"0\"><tr><td>";
		ofs << "<table border=\"1\" cellspacing=\"0\">";
		ofs << "<tr align=center>" << endl;
		ofs << "<td>" << endl;
	}

	if (bError)
	{
		if (ofs.is_open ())
		{
			ofs << "<b><font color=\"#FF0000\">get_node failed on node " << nNode << "<br>Reason: " << strError << "<br><br></font></b>" << endl;
		}
		else
		{
			return (false);
		}
	}
	else if ((nNode == nParent) && (nNode != m_nRootNode))
	{	// if a node claims to be its own parent, then it also must be the root node, otherwise the node's state is invalid
		if (ofs.is_open ())
		{
			ofs << "<b><font color=\"#FF0000\">node claims to be root<br><br></font></b>" << endl;
		}
		else
		{
			return (false);
		}
	}
	else if (sNodeDesc.nParent != nParent)
	{	// if the node's parent state is not equal to the node it was recursively called into, then the node's state is invalid
		if (ofs.is_open ())
		{
			ofs << "<b><font color=\"#FF0000\">nodes parent state invalid<br><br></font></b>" << endl;
		}
		else
		{
			return (false);
		}
	}
	else if (pAlloc[nNode])
	{	// if this node was processed before, then the tree state is invalid
		if (ofs.is_open ())
		{
			ofs << "<b><font color=\"#FF0000\">node is linked to more than once<br><br></font></b>" << endl;
		}
		else
		{
			return (false);
		}
	}
	
	// if it was possible to call this node without any assertion (invalid states that may have been detected are being ignored) ...
	if (!bError)
	{
		// ... then process this node

		// flag this node being in use
		pAlloc[nNode] = 0x01;

		// test if nMaxIdx state is valid
		nNumIdx = get_data_count (nNode);

		if (!is_leaf (nNode))
		{
			for (i = 0L; i < get_sub_node_count (nNode); i++)
			{
				get_node (get_sub_node (nNode, i), sSubNodeDesc);
				nNumIdx += sSubNodeDesc.nMaxIdx;
			}
		}

		// this node's number of data entries plus the nMaxIdx of all sub nodes combined must be equal to this node's maxIdx state
		if (nNumIdx != sNodeDesc.nMaxIdx)
		{
			if (ofs.is_open ())
			{
				ofs << "<b><font color=\"#FF0000\">nMaxIdx is inconsistent<br></font></b><br>" << endl;
			}
			else
			{
				return (false);
			}
		}
		
		if (ofs.is_open ())
		{
			ofs << "<a name=\"node_" << nNode << "\"></a>" << endl;
			ofs << "nMaxIdx: " << sNodeDesc.nMaxIdx << "<br>node: " << nNode << "<br>";
			ofs << "<a href=\"#node_" << sNodeDesc.nParent << "\">parent: " << sNodeDesc.nParent << "</a>";
		}
	}

	if (ofs.is_open ())
	{
		ofs << "</td>" << endl;
		ofs << "</tr>" << endl;
		ofs << "<tr align=center>" << endl;
		ofs << "<td>" << endl;
		ofs << "<table border=\"1\" cellspacing=\"0\">";
	}

	// only the root node is valid as an empty node
	if (get_data_count (sNodeDesc) == 0L)
	{
		// hence, if this code path has been entered, then the current node must be the root node

		// size uses the root node's nMaxIdx to deteremine the tree size
		// if that is not zero, then the current node not the root node, which contradicts the above assumption
		// or the root node descriptor has been corrupted
		// both cases lead to the conclusion that the tree's state is invalid
		if (ofs.is_open ())
		{
			if (CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::size () != 0)
			{
				ofs << "<tr align=center><td><b><font color=\"#FF0000\">no data</font></b></td></tr>" << endl;
			}
			else
			{
				ofs << "<tr align=center><td>no data</td></tr>" << endl;
			}
		}
		else
		{
			if (CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::size () != 0)
			{
				return (false);
			}
		}
	}
	else
	{
		// ... otherwise this node's data must be displayed
		if (ofs.is_open ())
		{
			ofs << "<tr align=center>";

			if (!is_leaf (nNode))
			{
				ofs << "<td><font color=\"#CCCCCC\">linear offset:<br> 0 </font></td>" << endl;
			}
		}

		this->get_serVector (nNode, this->m_pSerVector);

		nSerVectorTest = 0;

		// test or displayer node's data
		for (i = 0UL; i < get_data_count (nNode); i++)
		{
			if (ofs.is_open ())
			{
				ofs << "<td>" << endl;
			}
			
			// test or display one data item
			bool bPassed = showdata (ofs, nNode, i);

			if (ofs.is_open ())
			{
				// if showdata failed for any reason ...
				if (!bPassed)
				{
					// ... then the error message is already logged in the output stream

					// and this call returns immediately
					return (false);
				}
			}

			if (!is_leaf (nNode))
			{
				nSerVectorTest += (_t_nodeiter) this->get_max_index (this->get_sub_node (nNode, i));

				bPassed = (nSerVectorTest + i) == (this->m_pSerVector)[i];
			}
			else
			{
				bPassed = true;
			}

			if (ofs.is_open ())
			{
				ofs << "</td>" << endl;

				if (!is_leaf (nNode))
				{
					ofs << "<td>";

					if (!bPassed) ofs << "<font color=\"#FF0000\">";

					ofs << "linear offset:<br>" << (this->m_pSerVector)[i];

					if (!bPassed) ofs << "</font>";
					
					ofs << "</td>" << endl;
				}
			}
			else if (!bPassed)
			{
				return (false);
			}
		}

		if (ofs.is_open ())
		{
			ofs << "</tr>" << endl;
		}

		// if this is not a leaf node ...
		if (!is_leaf (nNode))
		{
			// ... then all sub nodes must be test or displayed
			if (ofs.is_open ())
			{
				ofs << "<tr align=center>" << endl;
			}

			// test or display all sub nodes
			for (i = 0L; i < get_sub_node_count (nNode); i++)
			{
				if (ofs.is_open ())
				{
					ofs << "<td>" << endl;
				}

				// test or display one sub node
				bool bPassed = shownode (ofs, get_sub_node (nNode, i), nNode, pAlloc);

				if (ofs.is_open ())
				{
					// if any of the sub nodes encountered any error ...
					if (!bPassed)
					{
						// ... then the error message is already logged in the output stream

						// and this call returns immediately
						return (false);
					}
				}
			
				if (ofs.is_open ())
				{
					ofs << "</td>" << endl;

					ofs << "<td></td>" << endl;
				}
			}

			if (ofs.is_open ())
			{
				ofs << "</tr>" << endl;
			}
		}
	}

	if (ofs.is_open ())
	{
		ofs << "</table>" << endl;
		ofs << "</td>" << endl;
		ofs << "</tr>" << endl;
		ofs << "</table>" << endl;
		ofs << "</table>" << endl;
	}

	// if this call made it down here then it is either in display mode or did not find any error
	return (true);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::shownodelist (std::ofstream &ofs, char *pAlloc)
{
	_t_nodeiter	ui64;
	node_t		sNodeDesc;

	ofs << "<H1>node list</H1>" << endl;
	ofs << "<br>" << endl;
	ofs << "<table>" << endl;

	// display the entire node list - regradless if a node is in use or not
	for (ui64 = 0; ui64 < m_nTreeSize; ui64++)
	{
		bool	bInUse = get_reservation (ui64);
		
		get_node (ui64, sNodeDesc);
		
		ofs << "<tr>" << endl;
		ofs << "<td>" << endl;
		ofs << "<a name=\"node_list_" << ui64 << "\">";
		ofs << "node: " << ui64;
		ofs << "</a>" << endl;
		ofs << "</td>" << endl;
		ofs << "<td><a href=\"#node_" << ui64 << "\">tree</a></td>" << endl;
		ofs << "<td><a href=\"#node_dump_" << ui64 << "\">dump</a></td>" << endl;
		ofs << "</tr>" << endl;
		ofs << "<tr>" << endl;
		ofs << "<td></td><td>" << endl;

		if (!bInUse)
		{
			ofs << "<font color=\"#CCCCCC\">";
		}

		ofs << "<a href=\"#node_list_" << sNodeDesc.nParent << "\">";
		ofs << "parent: " << sNodeDesc.nParent;
		ofs << "</a>" << endl;
		
		if (!bInUse)
		{
			ofs << "</font>";
		}

		ofs << "</td>" << endl;
		ofs << "<td>" << endl;
		
		if (!bInUse)
		{
			ofs << "<font color=\"#CCCCCC\">";
		}

		ofs << "nNumData: " << sNodeDesc.nNumData << endl;
		
		if (!bInUse)
		{
			ofs << "</font>";
		}

		ofs << "</td>" << endl;

		ofs << "<td>" << endl;
		
		if (!bInUse)
		{
			ofs << "<font color=\"#CCCCCC\">";
		}

		ofs << "nMaxIdx: " << sNodeDesc.nMaxIdx << endl;
		
		if (!bInUse)
		{
			ofs << "</font>";
		}

		ofs << "</td>" << endl;

		ofs << "</tr>" << endl;
	}

	ofs << "</table>" << endl;
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::setup_pools ()
{
	CBTreeIOperBlockPoolDesc_t		sPoolDesc;

	sPoolDesc.nTotalSize = sizeof (node_t);
	sPoolDesc.nEntrySize = sizeof (node_t);
	sPoolDesc.nCacheVectorSize = m_sCacheDescription.nNodeDescriptorNumOfLog2Lines;

	m_nPoolIDnodeDesc = create_dataPool (&sPoolDesc);

	sPoolDesc.nTotalSize = sizeof (node_maintainence_t);
	sPoolDesc.nEntrySize = sizeof (node_maintainence_t);
	sPoolDesc.nCacheVectorSize = m_sCacheDescription.nNodeDescriptorNumOfLog2Lines;

	m_nPoolIDnodeMaintenance = create_dataPool (&sPoolDesc);

	sPoolDesc.nTotalSize = sizeof (_t_data) * get_node_max_data_size ();
	sPoolDesc.nEntrySize = sizeof (_t_data);
	sPoolDesc.nCacheVectorSize = m_sCacheDescription.nDataCacheNumOfLog2Lines;

	m_nPoolIDdata = create_dataPool (&sPoolDesc);

	sPoolDesc.nTotalSize = (uint32_t)(sizeof (_t_nodeiter) * get_node_max_sub_nodes ());
	sPoolDesc.nEntrySize = (uint32_t)(sizeof (_t_nodeiter));
	sPoolDesc.nCacheVectorSize = m_sCacheDescription.nSubNodeCacheNumOfLog2Lines;

	m_nPoolIDsubNodes = create_dataPool (&sPoolDesc);

	sPoolDesc.nTotalSize = sizeof (_t_sizetype) * this->get_node_max_data_size ();
	sPoolDesc.nEntrySize = sizeof (_t_sizetype);
	sPoolDesc.nCacheVectorSize = m_sCacheDescription.nSerVectorCacheNumOfLog2Lines;

	m_nPoolIDserialVector = this->create_dataPool (&sPoolDesc);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBaseIteratorWriteAccess (_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize)
	:	CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (rDataLayerProperties, psCacheDescription, nNodeSize)
{
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBaseIteratorWriteAccess (CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT)
	:	CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (rBT)
{
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeBaseIteratorWriteAccess ()
{
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename ::CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::begin ()
{
	typename ::CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator		sIter (this, 0);

	return (sIter);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename ::CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::end ()
{
	typename ::CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator		sIter (this, this->size ());

	return (sIter);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename ::CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rbegin ()
{
	typename ::CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator			sIter (this, this->size ());
	typename ::CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator	sRIter (sIter);

	return (sRIter);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename ::CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rend ()
{
	typename ::CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator			sIter (this, 0);
	typename ::CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator	sRIter (sIter);

	return (sRIter);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap
	(
		CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT
	)
{
	typedef CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>	CBTreeBase_t;

	CBTreeBase_t	&rThisBT = dynamic_cast <CBTreeBase_t &> (*this);
	CBTreeBase_t	&rSwapBT = dynamic_cast <CBTreeBase_t &> (rBT);

	rThisBT.swap (rSwapBT);
}

#endif // BTREE_CPP
