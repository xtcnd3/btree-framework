/************************************************************
**
** file:	btreemultiset.h
** author:	Andreas Steffens
** license:	LGPL v3
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

#include "abstract_data_classes/btreeassociativebase.h"

template<class _t_keytype, class _t_datalayerproperties = CBTreeIOpropertiesRAM <> >
class CBTreeMultiSet
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

	typedef CBTreeMultiSet												CBTreeMultiSet_t;

	typedef CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties>
																		CBTreeAssociativeBase_t;

	typedef typename CBTreeAssociativeBase_t::CBTreeAssociative_t		CBTreeAssociative_t;

	typedef CBTreeAssociativeIf<_t_keytype, _t_keytype, size_type>		CBTreeAssociativeIf_t;

	typedef typename CBTreeAssociative_t::CBTreeBaseDefaults_t			CBTreeBaseDefaults_t;

	typedef typename CBTreeBaseDefaults_t::CBTreeBaseIf_t				CBTreeBaseIf_t;

	typedef typename CBTreeBaseIf_t::CBTreeDefaults_t					CBTreeDefaults_t;

	typedef typename CBTreeDefaults_t::CBTreeIf_t						CBTreeIf_t;

	typedef typename CBTreeAssociativeBase_t::iterator					iterator;
	typedef typename CBTreeAssociativeBase_t::const_iterator			const_iterator;
	typedef typename CBTreeAssociativeBase_t::reverse_iterator			reverse_iterator;
	typedef typename CBTreeAssociativeBase_t::const_reverse_iterator	const_reverse_iterator;

	typedef	typename CBTreeAssociativeIf_t::key_compare					key_compare;
	typedef typename CBTreeAssociativeIf_t::value_compare				value_compare;

	// construction
							CBTreeMultiSet<_t_keytype, _t_datalayerproperties>
													(const _t_datalayerproperties &rDataLayerProperties, const sub_node_iter_type nNodeSize);

							CBTreeMultiSet<_t_keytype, _t_datalayerproperties>
													(const CBTreeMultiSet_t &rContainer, const bool bAssign = true);

							CBTreeMultiSet<_t_keytype, _t_datalayerproperties>
													(CBTreeMultiSet_t &&rRhsContainer);

	// destruction
	virtual					~CBTreeMultiSet<_t_keytype, _t_datalayerproperties>
													();

	void					swap					(CBTreeAssociativeIf_t &rContainerIf);
	void					swap					(CBTreeMultiSet_t &rContainer);
	
	CBTreeMultiSet &		operator=				(const CBTreeMultiSet &rContainer);
	CBTreeMultiSet &		operator=				(CBTreeMultiSet &&rRhsContainer);

protected:

	void					_swap					(CBTreeMultiSet_t &rContainer);
	
public:

	friend class CBTreeIterator<CBTreeIf_t>;
	friend class CBTreeConstIterator<CBTreeIf_t>;
	friend class CBTreeReverseIterator<iterator>;
	friend class CBTreeConstReverseIterator<const_iterator>;
};

#endif // BTREEMULTISET_H

#include "btreemultiset.cpp"
