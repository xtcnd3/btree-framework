/************************************************************
**
** file:	btree.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree frameworks base class.
**
************************************************************/

#ifndef	BTREE_H
#define	BTREE_H

#include <iostream>
#include <fstream>
#include <set>

#include <string.h>

#include "btreeaux.h"
#include "btreeiter.h"

#if defined(__GNUC__) || defined(__GNUG__)
/*

g++ requires the definition of CBTreeIOpropertiesRAM and CBTreeRAMIO, since they are used as default
template parameters, although not being instantiated here.

*/

#include "btreeioram.h"

#endif

#define	OFFSETOF(s,m)								(uint32_t)(reinterpret_cast <uint64_t> (&(((s *) NULL) -> m)))
#define	SIZEOF(s,m)									(uint32_t)(sizeof (((s *) NULL) -> m))


typedef struct
{
	uint32_t	nMinNumberOfBytesPerSuperBlock;
//	uint32_t	nNodeDescriptorNumOfLog2Lines;
//	uint32_t	nDataCacheNumOfLog2Lines;
//	uint32_t	nSubNodeCacheNumOfLog2Lines;
//	uint32_t	nSerVectorCacheNumOfLog2Lines;
} bayerTreeCacheDescription_t;

template <class _ti_pos, class _t_data, class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter> >
class CBTreeBase
	:	public CBTreeSuper
	,	public virtual CBTreeIf<_t_sizetype>
{
#define	BAYERTREE_NODE_MAINTAINANCE_ALLOCATION			0x00000001ULL

protected:

	// typedefs
#pragma pack (1)

	typedef struct node_s
	{
		_t_nodeiter									nParent;		// index of parent node
		_t_sizetype									nMaxIdx;		// maximum linear index for this node
		_t_subnodeiter								nNumData;		// data sets in this node - if negative or zero then leaf node
	} node_t;
	
	typedef struct node_maintainence_s
	{
		uint32_t									uVector;
	} node_maintainence_t;

#pragma pack ()

public:

	typedef typename ::CBTreeBaseConstIterator<CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> >	const_iterator;
	typedef typename ::CBTreeBaseConstReverseIterator<const_iterator>																							const_reverse_iterator;

	typedef _ti_pos							position_t;
	typedef _t_data							data_t;
	typedef _t_sizetype						sizetype_t;
	typedef _t_nodeiter						nodeiter_t;
	typedef _t_subnodeiter					subnodeiter_t;
	typedef _t_datalayerproperties			datalayerproperties_t;
	typedef _t_datalayer					datalayer_t;

	// construction
						CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);
						CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT);

	// destruction
	virtual				~CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> ();

	// monitoring
	void				get_performance_counters	(uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR]);

	// attributes
	bool				empty						();
	_t_sizetype			size						();

	// removal
	void				clear						();

	// iterators
	const_iterator		cbegin						();
	const_iterator		cend						();
	const_reverse_iterator	crbegin					();
	const_reverse_iterator	crend					();

	void				swap						(CBTreeBase &rBT);

	// storage managment
	void				unload						();

	// debug
	bool				test_integrity				();

	void				show_integrity				(const char *pFilename);

protected:
 
	// setup
	void				create_root					();

	// complex primitives
	_t_nodeiter			create_node					(_t_nodeiter nParent);
	_t_sizetype			add_to_node					(_ti_pos sPos, const _t_data &rData, _t_nodeiter nNode, uint32_t nDepth, _t_sizetype *pnPos = NULL);
	_t_nodeiter			split_node					(_t_nodeiter nNode);
	virtual _ti_pos		determine_position			(_ti_pos sPos, _t_nodeiter nNode, _t_subnodeiter &nSubPos, _t_subnodeiter &nSubData, bool &bFound) = 0;
	_t_sizetype			remove_from_node			(_ti_pos sPos, _t_nodeiter nNode, uint32_t nDepth);
	_t_nodeiter			merge_node					(_t_nodeiter nNode, _t_subnodeiter nSub);
	void				rotate						(_t_nodeiter nNode, _t_subnodeiter nSub, _t_subnodeiter &newSub, bool bLeftToRight);
	void				replace_with_adjacent_entry	(_t_nodeiter nNode, _t_subnodeiter nSub, bool bFromLeft);

	_t_sizetype			serialize					(const _ti_pos nFrom, const _t_sizetype nLen, _t_data *pData, bool bReadOpr = true);

	// primitives
	_t_subnodeiter		insert_data_into_leaf		(_t_nodeiter nNode, _t_subnodeiter nSubPos, const _t_data &rData);
	_t_subnodeiter		insert_data_into_node		(_t_nodeiter nNode, _t_subnodeiter nSubPos, const _t_data &rData, _t_nodeiter nSubNode, int triMod);

	void				remove_data_from_leaf		(_t_nodeiter nNode, _t_subnodeiter nSub);
	void				remove_data_from_node		(_t_nodeiter nNode, _t_subnodeiter nSub, _t_subnodeiter nSubNode);

	void				replace_data				(_t_nodeiter nNode, _t_subnodeiter nSub, const _t_data &rData);

	virtual void		rebuild_node				(const _t_nodeiter nNode, const int32_t triMod = 0, _t_subnodeiter nSubStart = 0) = 0;

	// manuvering
	_t_subnodeiter		find_sub_node_offset		(_t_nodeiter nNode, _t_nodeiter nSubNode);
	
	void				move_prev					(_t_nodeiter nNode, _t_subnodeiter nSub, _t_nodeiter &nPrevNode, _t_subnodeiter &nPrevSubPos, bool &bBounce);
	void				move_next					(_t_nodeiter nNode, _t_subnodeiter nSub, _t_nodeiter &nNextNode, _t_subnodeiter &nNextSubPos, bool &bBounce);

	virtual _ti_pos		generate_prev_position		(const _t_nodeiter nNode, const _t_subnodeiter nSub, _ti_pos sPos) = 0;
	virtual _ti_pos		generate_next_position		(const _t_nodeiter nNode, const _t_subnodeiter nSub, _ti_pos sPos) = 0;

	virtual _t_subnodeiter	find_next_sub_pos		(const _t_nodeiter nNode, _ti_pos &sPos) = 0;

	virtual void		convert_pos_to_container_pos	(_t_nodeiter nNode, _t_sizetype nPos, _t_nodeiter &rRsltNode, _t_subnodeiter &rRsltSubPos);

	_t_subnodeiter		get_data_count				(const _t_nodeiter nNode);
	_t_subnodeiter		get_data_count				(const node_t &sNodeDesc);
	_t_subnodeiter		get_sub_node_count			(const _t_nodeiter nNode);
	_t_subnodeiter		get_sub_node_count			(const node_t &sNodeDesc);
	_t_sizetype			get_max_index				(const _t_nodeiter nNode);
	_t_sizetype			get_max_index				(const node_t &sNodeDesc);
	_t_nodeiter			get_parent					(const _t_nodeiter nNode);
	_t_nodeiter			get_parent					(const node_t &sNodeDesc);

	bool				is_root						(const _t_nodeiter nNode);

	bool				is_leaf						(const _t_nodeiter nNode);
	bool				is_leaf						(const node_t &nodeDesc);

	node_t *			get_node					(const _t_nodeiter nNode);

	_t_nodeiter *		get_sub_node				(_t_nodeiter nNode, _t_subnodeiter nEntry);

	_t_nodeiter *		get_sub_node_buffer			(_t_nodeiter nNode);

	_t_data *			get_data					(_t_nodeiter nNode, _t_subnodeiter nEntry);
	void				set_data					(_t_nodeiter nNode, _t_subnodeiter nEntry, _t_data sData);

	_t_data *			get_data_buffer				(const _t_nodeiter nNode);

	_t_sizetype *		get_serVector				(_t_nodeiter nNode);

	// maintanence (allocation)
	void				set_reservation				(_t_nodeiter nNode);
	bool				get_reservation				(_t_nodeiter nNode);
	void				unset_reservation			(_t_nodeiter nNode);
	void				reset_reservation			(_t_nodeiter nNode);

	// run time quantification
	_t_subnodeiter		get_node_max_sub_nodes		();
	_t_subnodeiter		get_node_max_data_size		();

	void				register_iterator			(const_iterator *pIter);
	void				unregister_iterator			(const_iterator *pIter);

	// rebuild FIFO
	void				rebuild_FIFO_put			(_t_nodeiter nNode);
	_t_nodeiter			rebuild_FIFO_peek			();
	_t_nodeiter			rebuild_FIFO_get			();
	uint32_t			rebuild_FIFO_fillstate		();

	// debug
	void				attache_traced_node			(_t_nodeiter nNode);
	void				detache_traced_node			();

	void				auto_shrink_inc				();
	void				auto_shrink_dec				();
	void				auto_shrink					();

	float				get_average_access_depth	();

	virtual bool		show_data					(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) = 0;
	virtual bool		show_node					(std::ofstream &ofs, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) = 0;
	bool				show_tree					(std::ofstream &ofs, _t_nodeiter nNode, _t_nodeiter nParent, char *pAlloc);
	void				shownodelist				(std::ofstream &ofs, char *pAlloc);

	// properties
	_t_nodeiter										m_nRootNode;
	
	_t_nodeiter										m_nTreeSize;			// number of tree nodes incl. all reservation leaks - is s
	_t_datalayer									*m_pData;
	bayerTreeCacheDescription_t							m_sCacheDescription;
	_t_datalayerproperties							*m_pClDataLayerProperties;

	_t_subnodeiter									m_nNodeSize;			// is t

	uint32_t										m_nGrowBy;

	// data pool IDs
	uint32_t										m_nPoolIDnodeDesc;
	uint32_t										m_nPoolIDnodeMaintenance;
	uint32_t										m_nPoolIDdata;
	uint32_t										m_nPoolIDsubNodes;
	uint32_t										m_nPoolIDserialVector;

	// rebuild FIFO
	_t_nodeiter										*m_pRebuildFIFO;
	uint32_t										m_nRebuildFIFOSize;
	uint32_t										m_nRebuildFIFOreadPos;
	uint32_t										m_nRebuildFIFOwritePos;

	// auto shrink
	uint32_t										m_nAutoShrinkThreshold;
	uint32_t										m_nAutoShrinkCounter;

#if defined (_DEBUG)
	// path trace
	_t_nodeiter										*m_pNodeTrace;
	uint32_t										m_nNodeTraceDepth;
	uint32_t										m_nNodeTraceDepthMax;
#endif

	// key buffer
	uint8_t											*m_pKeyBuffer;
	uint32_t										m_nKeyBufferSize;
	uint32_t										m_nKeyBufferFillState;
	bool											m_bKeyBufferWasTooSmall;

	// reservation
	_t_nodeiter										m_nNextAlloc;

#if defined (USE_PERFORMANCE_COUNTERS)
	uint64_t										m_nHitCtr;
	uint64_t										m_nMissCtr;
#endif

#if defined (BTREE_ITERATOR_REGISTRATION)
	typename ::std::set<const_iterator *>			*m_psIterRegister;
#endif

private:

	void				setup_pools					();

public:

	friend class CBTreeBaseIterator<CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> >;
	friend class CBTreeBaseConstIterator<CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> >;
};

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
class CBTreeBaseIteratorWriteAccess
	:	public CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

	typedef typename ::CBTreeBaseIterator<CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> >		iterator;
	typedef typename ::CBTreeBaseReverseIterator<iterator>																														reverse_iterator;

						CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

						CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT);


						~CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
							();

	// iterators
	iterator			begin						();
	iterator			end							();
	reverse_iterator	rbegin						();
	reverse_iterator	rend						();

	void				swap						(CBTreeBaseIteratorWriteAccess &rBT);

public:

	friend class CBTreeBaseIterator<CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> >;
	friend class CBTreeBaseConstIterator<CBTreeBaseIteratorWriteAccess<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> >;
};

#endif // BTREE_H

#include "btree.cpp"
