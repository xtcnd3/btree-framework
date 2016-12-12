/************************************************************
**
** file:	btreebaseif.h
** author:	Andreas Steffens
** license:	LGPL v3
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

template<class _ti_pos, class _t_data, class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t>
class CBTreeBaseIf
	:	public CBTreeDefaults<_t_data, _t_sizetype>
{
public:

	typedef CBTreeDefaults<_t_data, _t_sizetype>						CBTreeDefaults_t;

	typedef _t_data														value_type;
	typedef _t_sizetype													size_type;
	typedef _t_nodeiter													nodeiter_t;
	typedef _t_subnodeiter												subnodeiter_t;

	typedef value_type&													reference;
	typedef const value_type&											const_reference;
	typedef value_type*													pointer;
	typedef const value_type*											const_pointer;
	typedef	typename ::std::make_signed<size_type>::type				difference_type;

	typedef typename CBTreeDefaults_t::iterator							iterator;
	typedef typename CBTreeDefaults_t::const_iterator					const_iterator;
	typedef typename CBTreeDefaults_t::reverse_iterator					reverse_iterator;
	typedef typename CBTreeDefaults_t::const_reverse_iterator			const_reverse_iterator;

	// construction
							CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>
													(const _t_subnodeiter nNodeSize);

	explicit				CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>
													(const CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> &rContainer);

							CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>
													(CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> &&rRhsContainer);

	// destruction
	virtual					~CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> ();

protected:
 
	virtual _ti_pos			determine_position		(_ti_pos sPos, _t_nodeiter nNode, _t_subnodeiter &nSubPos, _t_subnodeiter &nSubData, bool &bFound) const = 0;

	virtual void			rebuild_node			(const _t_nodeiter nNode, const int32_t triMod = 0, _t_subnodeiter nSubStart = 0) = 0;

	virtual _ti_pos			generate_prev_position	(const _t_nodeiter nNode, const _t_subnodeiter nSub, _ti_pos sPos) const = 0;
	virtual _ti_pos			generate_next_position	(const _t_nodeiter nNode, const _t_subnodeiter nSub, _ti_pos sPos) const = 0;

	virtual _t_subnodeiter	find_next_sub_pos		(const _t_nodeiter nNode, _ti_pos &sPos) const = 0;

	virtual void			convert_pos_to_container_pos	(const _t_nodeiter nNode, const _t_sizetype nPos, _t_nodeiter &rRsltNode, _t_subnodeiter &rRsltSubPos) const = 0;

	// manuvering
	virtual _t_subnodeiter	find_sub_node_offset	(const _t_nodeiter nNode, const _t_nodeiter nSubNode) const = 0;

	virtual void			move_prev				(const _t_nodeiter nNode, const _t_subnodeiter nSub, _t_nodeiter &nPrevNode, _t_subnodeiter &nPrevSubPos, bool &bBounce) const = 0;
	virtual void			move_next				(const _t_nodeiter nNode, const _t_subnodeiter nSub, _t_nodeiter &nNextNode, _t_subnodeiter &nNextSubPos, bool &bBounce) const = 0;

	// node property access methods
	virtual _t_subnodeiter	get_data_count			(const _t_nodeiter nNode) const = 0;
	virtual _t_sizetype		get_max_index			(const _t_nodeiter nNode) const = 0;
	virtual _t_nodeiter		get_parent				(const _t_nodeiter nNode) const = 0;

	virtual bool			is_leaf					(const _t_nodeiter nNode) const = 0;

	virtual _t_nodeiter *	get_sub_node			(const _t_nodeiter nNode, const _t_subnodeiter nEntry) const = 0;

	virtual _t_data *		get_data				(const _t_nodeiter nNode, const _t_subnodeiter nEntry) const = 0;

	virtual _t_sizetype *	get_serVector			(const _t_nodeiter nNode) const = 0;

	virtual bool			show_data				(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const = 0;
	virtual bool			show_node				(std::ofstream &ofs, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const = 0;
};

#endif // BTREEBASEIF_H

#include "btreebaseif.cpp"
