/************************************************************
**
** file:	btreebaseif.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree frameworks base class
** on the low level interface level.
**
************************************************************/

#ifndef	BTREEBASEIF_H
#define	BTREEBASEIF_H

#include "btreedefaults.h"

template <class _ti_pos, class _t_data, class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t>
class CBTreeBaseIf
	:	public CBTreeDefaults<_t_data, _t_sizetype>
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

	typedef struct iterator_state_s
	{
		_t_sizetype		nAssociatedPos;
		_t_nodeiter		nNode;
		_t_subnodeiter	nSubPos;
	} iterator_state_t;

public:

	typedef CBTreeDefaults<_t_data, _t_sizetype>						CBTreeDefaults_t;

//	typedef _ti_pos														position_t;
	typedef _t_data														data_t;
	typedef _t_sizetype													size_type;
	typedef _t_nodeiter													nodeiter_t;
	typedef _t_subnodeiter												subnodeiter_t;

	// construction
						CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>
													(const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

						CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>
													(const CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> &rBT);

	// destruction
	virtual				~CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> ();

	CBTreeBaseIf &	operator=					(const CBTreeBaseIf &rBT);

protected:
 
	virtual _ti_pos		determine_position			(_ti_pos sPos, _t_nodeiter nNode, _t_subnodeiter &nSubPos, _t_subnodeiter &nSubData, bool &bFound) const = 0;

	virtual void		rebuild_node				(const _t_nodeiter nNode, const int32_t triMod = 0, _t_subnodeiter nSubStart = 0) = 0;

	virtual _ti_pos		generate_prev_position		(const _t_nodeiter nNode, const _t_subnodeiter nSub, _ti_pos sPos) = 0;
	virtual _ti_pos		generate_next_position		(const _t_nodeiter nNode, const _t_subnodeiter nSub, _ti_pos sPos) = 0;

	virtual _t_subnodeiter	find_next_sub_pos		(const _t_nodeiter nNode, _ti_pos &sPos) const = 0;

	virtual void		convert_pos_to_container_pos	(_t_nodeiter nNode, _t_sizetype nPos, _t_nodeiter &rRsltNode, _t_subnodeiter &rRsltSubPos) const;

	// manuvering
	virtual _t_subnodeiter	find_sub_node_offset	(_t_nodeiter nNode, _t_nodeiter nSubNode) const = 0;

	virtual void			move_prev				(_t_nodeiter nNode, _t_subnodeiter nSub, _t_nodeiter &nPrevNode, _t_subnodeiter &nPrevSubPos, bool &bBounce) const = 0;
	virtual void			move_next				(_t_nodeiter nNode, _t_subnodeiter nSub, _t_nodeiter &nNextNode, _t_subnodeiter &nNextSubPos, bool &bBounce) const = 0;

	// node property access methods
	virtual _t_subnodeiter	get_data_count			(const _t_nodeiter nNode) const = 0;
	virtual _t_sizetype		get_max_index			(const _t_nodeiter nNode) const = 0;
	virtual _t_nodeiter		get_parent				(const _t_nodeiter nNode) const = 0;

	virtual bool			is_leaf					(const _t_nodeiter nNode) const = 0;

	virtual _t_nodeiter *	get_sub_node			(const _t_nodeiter nNode, const _t_subnodeiter nEntry) const = 0;

	virtual _t_data *		get_data				(const _t_nodeiter nNode, const _t_subnodeiter nEntry) const = 0;

	virtual _t_sizetype *	get_serVector			(const _t_nodeiter nNode) const = 0;

	// run time quantification
	_t_subnodeiter		get_node_max_sub_nodes		() const;
	_t_subnodeiter		get_node_max_data_size		() const;

	// rebuild FIFO
	void				rebuild_FIFO_put			(_t_nodeiter nNode);
	_t_nodeiter			rebuild_FIFO_peek			();
	_t_nodeiter			rebuild_FIFO_get			();
	uint32_t			rebuild_FIFO_fillstate		();

	// debug
	void				attache_traced_node			(_t_nodeiter nNode);
	void				detache_traced_node			();

	float				get_average_access_depth	();

	// iterator interface
	uint32_t			get_iter_state_size			() const;
	void				reset_iter_state			(void *pState) const;
	void				evaluate_iter				(void *pState, _t_sizetype nOffsetPos) const;
	void				evaluate_iter_by_seek		(void *pState, _t_sizetype nNewPos) const;
	bool				is_iter_pos_evaluated		(void *pState, _t_sizetype nPos) const;

	_t_data				*get_iter_data				(void *pState) const;
	void				set_iter_data				(void *pState, const _t_data &rData);

	void				_swap						(CBTreeBaseIf &rBT);

	// properties
	_t_nodeiter										m_nRootNode;
	
	_t_nodeiter										m_nTreeSize;			// number of tree nodes incl. all reservation leaks - is s

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

	// reservation
	_t_nodeiter										m_nNextAlloc;

private:

	void				setup_pools					();

};

#endif // BTREEBASEIF_H

#include "btreebaseif.cpp"
