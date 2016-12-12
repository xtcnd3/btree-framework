/************************************************************
**
** file:	btreemap.h
** author:	Andreas Steffens
** license:	LGPL v3
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

#include "abstract_data_classes/btreeassociativebase.h"

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties = CBTreeIOpropertiesRAM <> >
class CBTreeMap
	:	public CBTreeAssociativeBase<::std::pair<_t_keytype, _t_maptype>, _t_keytype, _t_datalayerproperties>
{
public:

	typedef typename ::std::pair<_t_keytype, _t_maptype>				value_type;
	typedef _t_keytype													key_type;
	typedef _t_maptype													map_type;
	typedef typename _t_datalayerproperties::size_type					size_type;
	typedef typename _t_datalayerproperties::node_iter_type				node_iter_type;
	typedef typename _t_datalayerproperties::sub_node_iter_type			sub_node_iter_type;
	typedef _t_datalayerproperties										data_layer_properties_type;
	typedef typename _t_datalayerproperties::data_layer_type			data_layer_type;

	typedef value_type&													reference;
	typedef const value_type&											const_reference;
	typedef value_type*													pointer;
	typedef const value_type*											const_pointer;
	typedef	typename ::std::make_signed<size_type>::type				difference_type;

	typedef CBTreeMap													CBTreeMap_t;

	typedef CBTreeAssociativeBase<value_type, _t_keytype, _t_datalayerproperties>
																		CBTreeAssociativeBase_t;

	typedef typename CBTreeAssociativeBase_t::CBTreeAssociative_t		CBTreeAssociative_t;

	typedef CBTreeAssociativeIf<value_type, _t_keytype, size_type>		CBTreeAssociativeIf_t;

	typedef typename CBTreeAssociative_t::CBTreeBaseDefaults_t			CBTreeBaseDefaults_t;

	typedef typename CBTreeBaseDefaults_t::CBTreeBaseIf_t				CBTreeBaseIf_t;

	typedef typename CBTreeBaseIf_t::CBTreeDefaults_t					CBTreeDefaults_t;

	typedef typename CBTreeDefaults_t::CBTreeIf_t						CBTreeIf_t;

	typedef typename CBTreeAssociativeBase_t::iterator					iterator;
	typedef typename CBTreeAssociativeBase_t::const_iterator			const_iterator;
	typedef typename CBTreeAssociativeBase_t::reverse_iterator			reverse_iterator;
	typedef typename CBTreeAssociativeBase_t::const_reverse_iterator	const_reverse_iterator;

	typedef typename ::std::pair<iterator, iterator>					equal_range_type;
	typedef typename ::std::pair<const_iterator, const_iterator>		equal_range_const_type;
	
	typedef	typename CBTreeAssociativeBase_t::iterator_state_t			iterator_state_t;
	typedef	typename CBTreeAssociativeBase_t::position_t				position_t;

	typedef	typename CBTreeAssociativeIf_t::key_compare					key_compare;
	typedef typename CBTreeAssociativeIf_t::value_compare				value_compare;

	// construction
							CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>
													(const _t_datalayerproperties &rDataLayerProperties, const sub_node_iter_type nNodeSize);

							CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>
													(const CBTreeMap_t &rContainer, const bool bAssign = true);

							CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>
													(CBTreeMap_t &&rRhsContainer);

	// destruction
	virtual					~CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>
													();

	template<class _t_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);
	iterator				insert					(const value_type &rData);

	iterator				insert					(const_iterator sCIterHint, const value_type &rData);

	iterator				emplace					(value_type && rrData);
	iterator				emplace_hint			(const_iterator sCIterHint, value_type && rrData);

	template<class ..._t_va_args>
	iterator				emplace					(_t_va_args && ... rrArgs);
	
	template<class ..._t_va_args>
	iterator				emplace_hint			(const_iterator sCIterHint, _t_va_args && ... rrArgs);

	equal_range_type		equal_range				(const key_type &rKey);
	equal_range_const_type	equal_range				(const key_type &rKey) const;

	void					swap					(CBTreeAssociativeIf_t &rContainerIf);
	void					swap					(CBTreeMap_t &rContainer);
	
	CBTreeMap &				operator=				(const CBTreeMap &rContainer);
	CBTreeMap &				operator=				(CBTreeMap &&rRhsContainer);

protected:

	key_type				*extract_key			(key_type *pKey, const value_type &rData) const;

	void					_swap					(CBTreeMap_t &rContainer);

	template<class _t_iterator>
	bool					test_self_reference_of_iterator_to_this (_t_iterator &sItFirst, _t_iterator &sItLast, bool &bSelfReverse, const_iterator **ppsItFirst, const_iterator **ppsItLast)
	{
		return (btree_associative_container_iterator_self_reference_arbiter<value_type, data_layer_properties_type, _t_iterator>::test_self_reference_of_iterator_to_this (this, sItFirst, sItLast, bSelfReverse, ppsItFirst, ppsItLast));
	}

public:

	friend class CBTreeIterator<CBTreeIf_t>;
	friend class CBTreeConstIterator<CBTreeIf_t>;
	friend class CBTreeReverseIterator<iterator>;
	friend class CBTreeConstReverseIterator<const_iterator>;

};

#endif // BTREEMAP_H

#include "btreemap.cpp"
