/************************************************************
**
** file:	btreemap.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** map data class, which has an interface interchangeable with
** ::std::map.
**
************************************************************/

#ifndef BTREEMAP_H
#define	BTREEMAP_H

#include "./associative/base/btreeassociativebase.h"

template <class _t_keytype, class _t_maptype, class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter> >
class CBTreeMap
	:	public CBTreeAssociativeBase<::std::pair<_t_keytype, _t_maptype>, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

	typedef CBTreeMap													CBTreeMap_t;

	typedef CBTreeAssociativeBase< ::std::pair<_t_keytype, _t_maptype>, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
																		CBTreeAssociativeBase_t;

	typedef typename CBTreeAssociativeBase_t::CBTreeAssociative_t		CBTreeAssociative_t;

	typedef CBTreeAssociativeIf< ::std::pair<_t_keytype, _t_maptype>, _t_keytype, _t_sizetype>	CBTreeAssociativeIf_t;

	typedef typename CBTreeAssociative_t::CBTreeBase_t					CBTreeBase_t;

	typedef typename CBTreeBase_t::CBTreeBaseIf_t						CBTreeBaseIf_t;

	typedef typename CBTreeBaseIf_t::CBTreeDefaults_t					CBTreeDefaults_t;

	typedef typename CBTreeAssociativeBase_t::iterator					iterator;
	typedef typename CBTreeAssociativeBase_t::const_iterator			const_iterator;
	typedef typename CBTreeAssociativeBase_t::reverse_iterator			reverse_iterator;
	typedef typename CBTreeAssociativeBase_t::const_reverse_iterator	const_reverse_iterator;

	typedef typename ::std::pair<const _t_keytype, _t_maptype>			data_t;
	typedef _t_sizetype													size_type;
	typedef _t_nodeiter													nodeiter_t;
	typedef _t_subnodeiter												subnodeiter_t;
	typedef _t_datalayerproperties										datalayerproperties_t;
	typedef _t_datalayer												datalayer_t;

	typedef typename ::std::pair<_t_keytype, _t_maptype>				value_t;

	typedef	typename CBTreeAssociativeIf_t::key_compare					key_compare;
	typedef typename CBTreeAssociativeIf_t::value_compare				value_compare;

	// construction
							CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

							CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(const CBTreeMap_t &rBT, bool bAssign = true);

	// destruction
	virtual					~CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													();

	template <class _t_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);
	iterator				insert					(const value_t &rData);

	void					swap					(CBTreeAssociativeIf_t &rContainerIf);
	void					swap					(CBTreeMap_t &rContainer);

	key_compare				key_comp				() const;
	value_compare			value_comp				() const;
	
protected:

	_t_keytype				*extract_key			(_t_keytype *pKey, const value_t &rData) const;

	void					_swap					(CBTreeMap_t &rContainer);

	template<class _t_iterator>
	bool					test_self_reference_of_iterator_to_this (_t_iterator &sItFirst, _t_iterator &sItLast, bool &bSelfReverse, const_iterator **ppsItFirst, const_iterator **ppsItLast)
	{
		return (btree_keysort_self_reference_of_iterator_to_this_arbiter<value_t, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, _t_iterator>::test_self_reference_of_iterator_to_this (this, sItFirst, sItLast, bSelfReverse, ppsItFirst, ppsItLast));
	}

public:

	friend class CBTreeIterator<CBTreeBase_t>;
	friend class CBTreeConstIterator<CBTreeBase_t>;
	friend class CBTreeReverseIterator<iterator>;
	friend class CBTreeConstReverseIterator<const_iterator>;

};

#endif // BTREEMAP_H

#include "btreemap.cpp"

