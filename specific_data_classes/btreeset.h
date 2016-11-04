/************************************************************
**
** file:	btreeset.h
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** set data class, which has an interface interchangeable
** with ::std::set.
**
************************************************************/

#ifndef BTREESET_H
#define	BTREESET_H

#include "abstract_data_classes/btreeassociativebase.h"

template<class _t_keytype, class _t_datalayerproperties = CBTreeIOpropertiesRAM <> >
class CBTreeSet
	:	public CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties>
{
public:

	typedef _t_keytype													value_type;
	typedef _t_keytype													key_type;
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

	typedef CBTreeSet													CBTreeSet_t;

	typedef CBTreeAssociativeBase<key_type, key_type, _t_datalayerproperties>
																		CBTreeAssociativeBase_t;

	typedef typename CBTreeAssociativeBase_t::CBTreeAssociative_t		CBTreeAssociative_t;

	typedef CBTreeAssociativeIf<key_type, key_type, size_type>			CBTreeAssociativeIf_t;

	typedef typename CBTreeAssociative_t::CBTreeBaseDefaults_t			CBTreeBaseDefaults_t;

	typedef typename CBTreeBaseDefaults_t::CBTreeBaseIf_t				CBTreeBaseIf_t;

	typedef typename CBTreeBaseIf_t::CBTreeDefaults_t					CBTreeDefaults_t;

	typedef typename CBTreeDefaults_t::CBTreeIf_t						CBTreeIf_t;

	typedef typename CBTreeAssociativeBase_t::iterator					iterator;
	typedef typename CBTreeAssociativeBase_t::const_iterator			const_iterator;
	typedef typename CBTreeAssociativeBase_t::reverse_iterator			reverse_iterator;
	typedef typename CBTreeAssociativeBase_t::const_reverse_iterator	const_reverse_iterator;

	typedef	typename CBTreeAssociativeBase_t::iterator_state_t			iterator_state_t;
	typedef	typename CBTreeAssociativeBase_t::position_t				position_t;

	typedef	typename CBTreeAssociativeIf_t::key_compare					key_compare;
	typedef typename CBTreeAssociativeIf_t::value_compare				value_compare;

	// construction
							CBTreeSet<_t_keytype, _t_datalayerproperties>
													(_t_datalayerproperties &rDataLayerProperties, typename _t_datalayerproperties::sub_node_iter_type nNodeSize);

							CBTreeSet<_t_keytype, _t_datalayerproperties>
													(const CBTreeSet_t &rBT, bool bAssign = true);

	// destruction
	virtual					~CBTreeSet<_t_keytype, _t_datalayerproperties>
													();

	template<class _t_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);
	iterator				insert					(const value_type &rData);

	iterator				insert					(const_iterator sCIterHint, const value_type &rData);

	void					swap					(CBTreeAssociativeIf_t &rContainerIf);
	void					swap					(CBTreeSet_t &rContainer);

//	key_compare				key_comp				() const;
//	value_compare			value_comp				() const;
	
protected:

	void					_swap					(CBTreeSet_t &rContainer);

	template<class _t_iterator>
	bool					test_self_reference_of_iterator_to_this (_t_iterator &sItFirst, _t_iterator &sItLast, bool &bSelfReverse, const_iterator **ppsItFirst, const_iterator **ppsItLast)
	{
		return (btree_associative_container_iterator_self_reference_arbiter<_t_keytype, _t_keytype, _t_datalayerproperties, _t_iterator>::test_self_reference_of_iterator_to_this (this, sItFirst, sItLast, bSelfReverse, ppsItFirst, ppsItLast));
	}

public:

	friend class CBTreeIterator<CBTreeIf_t>;
	friend class CBTreeConstIterator<CBTreeIf_t>;
	friend class CBTreeReverseIterator<iterator>;
	friend class CBTreeConstReverseIterator<const_iterator>;
};

#endif // BTREESET_H

#include "btreeset.cpp"
