/************************************************************
**
** file:	btreekeysort.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** key sort (ordered associative) data class.
**
************************************************************/

#ifndef	BTREEKEYSORT_H
#define	BTREEKEYSORT_H

#include <iterator>
#include <list>

#include "btreeassociative.h"

template <class _t_data, class _t_key = _t_data, class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter> >
class CBTreeKeySort
	:	virtual public CBTreeAssociativeIf<_t_data, _t_key, _t_sizetype>
	,	public CBTreeAssociative <_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

	typedef CBTreeKeySort											CBTreeKeySort_t;

	typedef CBTreeAssociative <_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
																	CBTreeAssociative_t;

	typedef CBTreeAssociativeIf<_t_data, _t_key, _t_sizetype>		CBTreeAssociativeIf_t;

	typedef typename CBTreeAssociative_t::CBTreeBase_t				CBTreeBase_t;

	typedef typename CBTreeBase_t::CBTreeBaseIf_t					CBTreeBaseIf_t;

	typedef typename CBTreeBaseIf_t::CBTreeDefaults_t				CBTreeDefaults_t;

	typedef	typename CBTreeBase_t::node_t							node_t;

	typedef	typename CBTreeAssociative_t::iterator					iterator;
	typedef	typename CBTreeAssociative_t::const_iterator			const_iterator;
	typedef	typename CBTreeAssociative_t::reverse_iterator			reverse_iterator;
	typedef	typename CBTreeAssociative_t::const_reverse_iterator	const_reverse_iterator;

	typedef _t_data													data_t;
	typedef _t_sizetype												size_type;
	typedef _t_nodeiter												nodeiter_t;
	typedef _t_subnodeiter											subnodeiter_t;
	typedef _t_datalayerproperties									datalayerproperties_t;
	typedef _t_datalayer											datalayer_t;

	typedef	typename CBTreeAssociative <_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator_state_t		iterator_state_t;

	// construction
						CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

						CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign = true);

	// destruction
	virtual				~CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													();
	
	void				swap						(CBTreeAssociativeIf_t &rContainerIf);
	void				swap						(CBTreeKeySort_t &rContainer);
	
	void				clear						();

	CBTreeKeySort_t &	operator=					(const CBTreeKeySort_t &rBT);

protected:

	void				set_iter_data				(void *pState, const _t_data &rData);

	void				_swap						(CBTreeKeySort_t &rContainer);
};

#endif // BTREEKEYSORT_H

#include "btreekeysort.cpp"
