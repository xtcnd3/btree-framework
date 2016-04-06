/************************************************************
**
** file:	btreebasedefaults.h
** author:	Andreas Steffens
** license:	GPL v2
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
template parameters, although not being instantiated here.

*/

#include "btreeioram.h"

#endif

template <class _ti_pos, class _t_data, class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter> >
class CBTreeBaseDefaults
	:	public CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>
{
public:

	typedef CBTreeBaseDefaults																	CBTreeBase_t;

	typedef CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>			CBTreeBaseIf_t;

//	typedef _ti_pos																				position_t;
	typedef _t_data																				data_t;
	typedef _t_sizetype																			size_type;
	typedef _t_nodeiter																			nodeiter_t;
	typedef _t_subnodeiter																		subnodeiter_t;
	typedef _t_datalayerproperties																datalayerproperties_t;
	typedef _t_datalayer																		datalayer_t;

	typedef	typename CBTreeBaseIf_t::node_t														node_t;
	typedef	typename CBTreeBaseIf_t::node_maintainence_t										node_maintainence_t;

	// construction
						CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

						CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(const CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT);

	// destruction
	virtual				~CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> ();

	// attributes
	bool				empty						() const;
	_t_sizetype			size						() const;

	// removal
	void				clear						();

	// storage managment
	virtual void		unload						();

	// debug
	bool				test_integrity				() const;

	void				show_integrity				(const char *pFilename) const;

#if defined (BTREEFRAMEWORK_TEST_BENCH_ENABLED)

	void				test						() const;

	void				set_atomic_testing			(bool bEnable);

#endif

	CBTreeBaseDefaults &		operator=					(const CBTreeBaseDefaults &rBT);

protected:
 
	// setup
	void				create_root					();

	// complex primitives
	_t_nodeiter			create_node					(_t_nodeiter nParent);
	_t_sizetype			add_to_node					(_ti_pos sPos, const _t_data &rData, _t_nodeiter nNode, uint32_t nDepth, _t_nodeiter &rnRsltNode, _t_subnodeiter &rnRsltSub, _t_sizetype *pnPos = NULL);
	_t_nodeiter			split_node					(_t_nodeiter nNode);
	_t_sizetype			remove_from_node			(_ti_pos sPos, _t_nodeiter nNode, uint32_t nDepth);
	_t_nodeiter			merge_node					(_t_nodeiter nNode, _t_subnodeiter nSub);
	void				rotate						(_t_nodeiter nNode, _t_subnodeiter nSub, _t_subnodeiter &newSub, bool bLeftToRight);
	void				replace_with_adjacent_entry	(_t_nodeiter nNode, _t_subnodeiter nSub, bool bFromLeft);

	_t_sizetype			serialize					(const _ti_pos nFrom, const _t_sizetype nLen, _t_data *pData, const bool bReadOpr = true) const;

	// primitives
	_t_subnodeiter		insert_data_into_node		(_t_nodeiter nNode, _t_subnodeiter nSubPos, const _t_data &rData, _t_nodeiter nSubNode = (_t_nodeiter) ~0x0, int triMod = -1);

	void				remove_data_from_leaf		(_t_nodeiter nNode, _t_subnodeiter nSub);
	void				remove_data_from_node		(_t_nodeiter nNode, _t_subnodeiter nSub, _t_subnodeiter nSubNode);

	void				replace_data				(_t_nodeiter nNode, _t_subnodeiter nSub, const _t_data &rData);

	// manuvering
	_t_subnodeiter		find_sub_node_offset		(_t_nodeiter nNode, _t_nodeiter nSubNode) const;
	
	void				move_prev					(_t_nodeiter nNode, _t_subnodeiter nSub, _t_nodeiter &nPrevNode, _t_subnodeiter &nPrevSubPos, bool &bBounce) const;
	void				move_next					(_t_nodeiter nNode, _t_subnodeiter nSub, _t_nodeiter &nNextNode, _t_subnodeiter &nNextSubPos, bool &bBounce) const;

	// maintanence (allocation)
	void				set_reservation				(_t_nodeiter nNode);
	bool				get_reservation				(_t_nodeiter nNode) const;
	void				unset_reservation			(_t_nodeiter nNode);
	void				reset_reservation			(_t_nodeiter nNode);

	// node property access methods
	_t_subnodeiter		get_data_count				(const _t_nodeiter nNode) const;
	_t_subnodeiter		get_data_count				(const node_t &sNodeDesc) const;
	_t_subnodeiter		get_sub_node_count			(const _t_nodeiter nNode) const;
	_t_subnodeiter		get_sub_node_count			(const node_t &sNodeDesc) const;
	_t_sizetype			get_max_index				(const _t_nodeiter nNode) const;
	_t_sizetype			get_max_index				(const node_t &sNodeDesc) const;
	_t_nodeiter			get_parent					(const _t_nodeiter nNode) const;
	_t_nodeiter			get_parent					(const node_t &sNodeDesc) const;

	bool				is_root						(const _t_nodeiter nNode) const;

	inline bool			is_leaf						(const _t_nodeiter nNode) const;
	inline bool			is_leaf						(const node_t &nodeDesc) const;

	node_t *			get_node					(const _t_nodeiter nNode) const;

	_t_nodeiter *		get_sub_node				(const _t_nodeiter nNode, const _t_subnodeiter nEntry) const;

	_t_nodeiter *		get_sub_node_buffer			(const _t_nodeiter nNode) const;

	_t_data *			get_data					(const _t_nodeiter nNode, const _t_subnodeiter nEntry) const;
	void				set_data					(const _t_nodeiter nNode, const _t_subnodeiter nEntry, _t_data sData);

	_t_data *			get_data_buffer				(const _t_nodeiter nNode) const;

	_t_sizetype *		get_serVector				(const _t_nodeiter nNode) const;

	void				auto_shrink_inc				();
	void				auto_shrink_dec				();
	void				auto_shrink					();

	virtual bool		show_data					(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const = 0;
	virtual bool		show_node					(std::ofstream &ofs, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const = 0;
	bool				show_tree					(std::ofstream &ofs, _t_nodeiter nNode, _t_nodeiter nParent, char *pAlloc) const;
	void				shownodelist				(std::ofstream &ofs, char *pAlloc) const;

	void				_swap						(CBTreeBaseDefaults &rBT);

	_t_datalayer									*m_pData;
	_t_datalayerproperties							*m_pClDataLayerProperties;
};

#endif // BTREEBASEDEFAULTS_H

#include "btreebasedefaults.cpp"
