/************************************************************
**
** file:	btreemultiset.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** multi set data class, which has an interface interchangeable
** with ::std::multiset.
**
************************************************************/

#ifndef BTREEMULTISET_H
#define	BTREEMULTISET_H

#include "./associative/base/btreeassociativebase.h"

template <class _t_keytype, class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter> >
class CBTreeMultiSet
	:	public CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

	typedef CBTreeMultiSet												CBTreeMultiSet_t;

	typedef CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
																		CBTreeAssociativeBase_t;

	typedef typename CBTreeAssociativeBase_t::CBTreeAssociative_t		CBTreeAssociative_t;

	typedef typename CBTreeAssociative_t::CBTreeBase_t					CBTreeBase_t;

	typedef CBTreeAssociativeIf<_t_keytype, _t_keytype, _t_sizetype>	CBTreeAssociativeIf_t;

	typedef typename CBTreeBase_t::CBTreeBaseIf_t						CBTreeBaseIf_t;

	typedef typename CBTreeBaseIf_t::CBTreeDefaults_t					CBTreeDefaults_t;

	typedef typename CBTreeAssociativeBase_t::iterator					iterator;
	typedef typename CBTreeAssociativeBase_t::const_iterator			const_iterator;
	typedef typename CBTreeAssociativeBase_t::reverse_iterator			reverse_iterator;
	typedef typename CBTreeAssociativeBase_t::const_reverse_iterator	const_reverse_iterator;

//	typedef typename CBTreeAssociativeBase_t::position_t				position_t;
	typedef const _t_keytype											data_t;
	typedef _t_sizetype													size_type;
	typedef _t_nodeiter													nodeiter_t;
	typedef _t_subnodeiter												subnodeiter_t;
	typedef _t_datalayerproperties										datalayerproperties_t;
	typedef _t_datalayer												datalayer_t;

	typedef _t_keytype													value_t;

	typedef	typename CBTreeAssociativeIf_t::key_compare					key_compare;
	typedef typename CBTreeAssociativeIf_t::value_compare				value_compare;

	// construction
							CBTreeMultiSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

							CBTreeMultiSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(const CBTreeMultiSet_t &rBT, bool bAssign = true);

	// destruction
	virtual					~CBTreeMultiSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													();

	void					swap					(CBTreeAssociativeIf_t &rContainerIf);
	void					swap					(CBTreeMultiSet_t &rContainer);
	
	key_compare				key_comp				() const;
	value_compare			value_comp				() const;

protected:

	void					_swap					(CBTreeMultiSet_t &rContainer);
	
public:

	friend class CBTreeIterator<CBTreeBase_t>;
	friend class CBTreeConstIterator<CBTreeBase_t>;
	friend class CBTreeReverseIterator<iterator>;
	friend class CBTreeConstReverseIterator<const_iterator>;
};

#endif // BTREEMULTISET_H

#include "btreemultiset.cpp"
