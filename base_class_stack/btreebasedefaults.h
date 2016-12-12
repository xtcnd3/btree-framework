/************************************************************
**
** file:	btreebasedefaults.h
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree frameworks base class.
**
************************************************************/

#ifndef	BTREEBASEDEFAULTS_H
#define	BTREEBASEDEFAULTS_H

#include "btreebaseif.h"

#if defined(__GNUC__) || defined(__GNUG__)
/*

g++ requires the definition of CBTreeIOpropertiesRAM and CBTreeRAMIO, since they are used as default
templateparameters, although not being instantiated here.

*/

#include "btreeioram.h"

#endif

template<class _ti_pos, class _t_data, class _t_datalayerproperties = CBTreeIOpropertiesRAM <> >
class CBTreeBaseDefaults
	:	public CBTreeBaseIf	<
								_ti_pos, 
								_t_data, 
								typename _t_datalayerproperties::size_type, 
								typename _t_datalayerproperties::node_iter_type, 
								typename _t_datalayerproperties::sub_node_iter_type
							>
{
public:

	typedef CBTreeBaseDefaults																	CBTreeBaseDefaults_t;

	typedef _t_data																				value_type;
	typedef typename _t_datalayerproperties::size_type											size_type;
	typedef typename _t_datalayerproperties::node_iter_type										node_iter_type;
	typedef typename _t_datalayerproperties::sub_node_iter_type									sub_node_iter_type;
	typedef _t_datalayerproperties																data_layer_properties_type;
	typedef typename _t_datalayerproperties::data_layer_type									data_layer_type;

	typedef value_type&																			reference;
	typedef const value_type&																	const_reference;
	typedef value_type*																			pointer;
	typedef const value_type*																	const_pointer;
	typedef	typename ::std::make_signed<size_type>::type										difference_type;

	typedef CBTreeBaseIf<_ti_pos, value_type, size_type, node_iter_type, sub_node_iter_type>	CBTreeBaseIf_t;

	typedef	typename CBTreeBaseIf_t::CBTreeDefaults_t											CBTreeDefaults_t;

	typedef typename CBTreeBaseIf_t::iterator													iterator;
	typedef typename CBTreeBaseIf_t::const_iterator												const_iterator;
	typedef typename CBTreeBaseIf_t::reverse_iterator											reverse_iterator;
	typedef typename CBTreeBaseIf_t::const_reverse_iterator										const_reverse_iterator;
	
#define	BAYERTREE_NODE_MAINTAINANCE_ALLOCATION			0x00000001UL

protected:

	// typedefs
#pragma pack (1)

	typedef struct node_s
	{
		node_iter_type								nParent;		// index of parent node
		size_type									nMaxIdx;		// maximum linear index for this node
		sub_node_iter_type							nNumData;		// data sets in this node - if negative or zero then leaf node
	} node_t;
	
	typedef struct node_maintainence_s
	{
		uint32_t									uVector;
	} node_maintainence_t;

#pragma pack ()

	typedef struct iterator_state_s
	{
		size_type									nAssociatedPos;
		node_iter_type								nNode;
		sub_node_iter_type							nSubPos;
	} iterator_state_t;

public:

	// construction
						CBTreeBaseDefaults<_ti_pos, _t_data, _t_datalayerproperties>
													(const _t_datalayerproperties &rDataLayerProperties, const sub_node_iter_type nNodeSize);

	explicit			CBTreeBaseDefaults<_ti_pos, _t_data, _t_datalayerproperties>
													(const CBTreeBaseDefaults<_ti_pos, _t_data, _t_datalayerproperties> &rContainer);

						CBTreeBaseDefaults<_ti_pos, _t_data, _t_datalayerproperties>
													(CBTreeBaseDefaults<_ti_pos, _t_data, _t_datalayerproperties> &&rRhsContainer);

						// destruction
	virtual				~CBTreeBaseDefaults<_ti_pos, _t_data, _t_datalayerproperties>
													();

	// attributes
	bool				empty						() const;
	size_type			size						() const;

	// removal
	void				clear						();

	// storage managment
	virtual void		unload						();

	// debug
	bool				test_integrity				() const;

	void				show_integrity				(const char *pFilename) const;

#if defined (BTREEFRAMEWORK_TEST_BENCH_ENABLED)

	void				test						() const;

	virtual void		set_atomic_testing			(const bool bEnable);

#endif

	CBTreeBaseDefaults &	operator=				(CBTreeBaseDefaults &&rRhsContainer);

protected:
 
	// setup
	void				create_root					();

	// complex primitives
	node_iter_type		create_node					(const node_iter_type nNextParent);
	size_type			add_to_node					(_ti_pos sPos, const node_iter_type nNode, const uint32_t nDepth, node_iter_type &rnRsltNode, sub_node_iter_type &rnRsltSub, size_type *pnPos = NULL);
	node_iter_type		split_node					(const node_iter_type nNode);
	size_type			remove_from_node			(_ti_pos sPos, const node_iter_type nNode, const uint32_t nDepth);
	node_iter_type		merge_node					(const node_iter_type nNode, const sub_node_iter_type nSub);
	void				rotate						(const node_iter_type nNode, const sub_node_iter_type nSub, sub_node_iter_type &newSub, const bool bLeftToRight);
	void				replace_with_adjacent_entry	(const node_iter_type nNode, const sub_node_iter_type nSub, const bool bFromLeft);

	size_type			serialize					(const _ti_pos nFrom, const size_type nLen, value_type *pData, const bool bReadOpr = true) const;

	bool				insert_via_iterator			(const_iterator &rCIterPos, node_iter_type &rNode, sub_node_iter_type &rSubPos);

	// primitives
	void				insert_data_into_node		(node_iter_type &rnNode, sub_node_iter_type &rnSubPos, node_iter_type nSubNode = (node_iter_type) ~0x0, int triMod = -1);

	void				remove_data_from_leaf		(const node_iter_type nNode, const sub_node_iter_type nSub);
	void				remove_data_from_node		(const node_iter_type nNode, const sub_node_iter_type nSub, const sub_node_iter_type nSubNode);

	void				replace_data				(const node_iter_type nNode, const sub_node_iter_type nSub, const value_type &rData);

	// manuvering
	sub_node_iter_type	find_sub_node_offset		(const node_iter_type nNode, const node_iter_type nSubNode) const;
	
	void				move_prev					(const node_iter_type nNode, const sub_node_iter_type nSub, node_iter_type &nPrevNode, sub_node_iter_type &nPrevSubPos, bool &bBounce) const;
	void				move_next					(const node_iter_type nNode, const sub_node_iter_type nSub, node_iter_type &nNextNode, sub_node_iter_type &nNextSubPos, bool &bBounce) const;

	// position coversion
	void				convert_pos_to_container_pos	(const node_iter_type nNode, const size_type nPos, node_iter_type &rRsltNode, sub_node_iter_type &rRsltSubPos) const;

	// maintanence (allocation)
	void				set_reservation				(const node_iter_type nNode);
	bool				get_reservation				(const node_iter_type nNode) const;
	void				unset_reservation			(const node_iter_type nNode);
	void				reset_reservation			(const node_iter_type nNode);

	// node property access methods
	sub_node_iter_type	get_data_count				(const node_iter_type nNode) const;
	sub_node_iter_type	get_data_count				(const node_t &sNodeDesc) const;
	sub_node_iter_type	get_sub_node_count			(const node_iter_type nNode) const;
	sub_node_iter_type	get_sub_node_count			(const node_t &sNodeDesc) const;
	size_type			get_max_index				(const node_iter_type nNode) const;
	size_type			get_max_index				(const node_t &sNodeDesc) const;
	node_iter_type		get_parent					(const node_iter_type nNode) const;
	node_iter_type		get_parent					(const node_t &sNodeDesc) const;

	bool				is_root						(const node_iter_type nNode) const;

	inline bool			is_leaf						(const node_iter_type nNode) const;
	inline bool			is_leaf						(const node_t &nodeDesc) const;

	node_t *			get_node					(const node_iter_type nNode) const;

	node_iter_type *	get_sub_node				(const node_iter_type nNode, const sub_node_iter_type nEntry) const;

	node_iter_type *	get_sub_node_buffer			(const node_iter_type nNode) const;

	value_type *		get_data					(const node_iter_type nNode, const sub_node_iter_type nEntry) const;
	void				set_data					(const node_iter_type nNode, const sub_node_iter_type nEntry, value_type sData);

	value_type *		get_data_buffer				(const node_iter_type nNode) const;

	size_type *			get_serVector				(const node_iter_type nNode) const;

	void				auto_shrink_inc				();
	void				auto_shrink_dec				();
	void				auto_shrink					();

	// run time quantification
	sub_node_iter_type	get_node_max_sub_nodes		() const;
	sub_node_iter_type	get_node_min_data_size		() const;
	sub_node_iter_type	get_node_max_data_size		() const;

	// rebuild FIFO
	void				rebuild_FIFO_put			(node_iter_type nNode);
	node_iter_type		rebuild_FIFO_peek			() const;
	node_iter_type		rebuild_FIFO_get			();
	uint32_t			rebuild_FIFO_fillstate		();

	// debug
	void				attache_traced_node			(node_iter_type nNode);
	void				detache_traced_node			();

	// heuristic
	float				get_average_access_depth	() const;

	// iterator interface
	uint32_t			get_iter_state_size			() const;
	void				reset_iter_state			(void *pState) const;
	void				evaluate_iter				(void *pState, const size_type nOffsetPos) const;
	void				evaluate_iter_by_seek		(void *pState, const size_type nNewPos) const;
	bool				is_iter_pos_evaluated		(void *pState, const size_type nPos) const;

	value_type			*get_iter_data				(void *pState) const;
	void				set_iter_data				(void *pState, const value_type &rData);

	bool				is_simple_evaluation_possible (void *pState, size_type nPos) const;

	// debug aid
	bool				show_tree					(std::ofstream &ofs, const node_iter_type nNode, const node_iter_type nParent, char *pAlloc) const;
	void				shownodelist				(std::ofstream &ofs, char *pAlloc) const;

	void				_swap						(CBTreeBaseDefaults &rContainer);

	void				_local_swap					(CBTreeBaseDefaults &rContainer);

	// properties
	node_iter_type									m_nRootNode;
	
	node_iter_type									m_nTreeSize;			// number of tree nodes incl. all reservation leaks - is s

	sub_node_iter_type								m_nNodeSize;			// is t

	size_type										m_nGrowBy;

	// data pool IDs
	uint32_t										m_nPoolIDnodeDesc;
	uint32_t										m_nPoolIDnodeMaintenance;
	uint32_t										m_nPoolIDdata;
	uint32_t										m_nPoolIDsubNodes;
	uint32_t										m_nPoolIDserialVector;

	// rebuild FIFO
	node_iter_type									*m_pRebuildFIFO;
	uint32_t										m_nRebuildFIFOSize;
	uint32_t										m_nRebuildFIFOreadPos;
	uint32_t										m_nRebuildFIFOwritePos;

	// auto shrink
	uint32_t										m_nAutoShrinkThreshold;
	uint32_t										m_nAutoShrinkCounter;

#if defined (_DEBUG)

	// path trace
	node_iter_type									*m_pNodeTrace;
	uint32_t										m_nNodeTraceDepth;
	uint32_t										m_nNodeTraceDepthMax;

#endif

	// reservation
	node_iter_type									m_nNextAlloc;

	// data layer
	data_layer_type									*m_pData;
	data_layer_properties_type						*m_pClDataLayerProperties;

private:

	void				setup_pools					();

};

#endif // BTREEBASEDEFAULTS_H

#include "btreebasedefaults.cpp"
