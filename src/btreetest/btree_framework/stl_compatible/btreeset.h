/************************************************************
**
** file:	btreeset.h
** author:	Andreas Steffens
** license:	GPL v2
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

#include <utility>

#include "btreekeysort.h"
#include "btreeiter.h"

template <class _t_keytype, class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter> >
class CBTreeSet
	:	public CBTreeKeySort<_t_keytype, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

	template<class _ti_keytype>
	struct key_compare_s
	{
		bool operator()(const _ti_keytype& rKey0, const _ti_keytype& rKey1) const
		{	
			typename ::std::is_arithmetic<_ti_keytype>::type		sCompareSelect;

			return (CBTreeSet::comp (rKey0, rKey1, sCompareSelect) < 0);
		}
	};

	template<class _t_valuetype>
	struct value_compare_s
	{
		bool operator()(const _t_valuetype& rVal0, const _t_valuetype& rVal1) const
		{	
			typename ::std::is_arithmetic<_t_keytype>::type		sCompareSelect;

			//return (CBTreeSet::comp (rVal0.first, rVal1.first, sCompareSelect) < 0);
			return (CBTreeSet::comp (rVal0, rVal1, sCompareSelect) < 0);
		}
	};

	typedef CBTreeKeySort<_t_keytype, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
																		CBTreeKeySort_t;

	typedef typename CBTreeKeySort_t::CBTreeBase_t						CBTreeBase_t;

	typedef CBTreeSet												CBTreeSet_t;

	typedef typename ::CBTreeBaseIterator<CBTreeBase_t>					iterator;
	typedef typename ::CBTreeBaseReverseIterator<iterator>				reverse_iterator;

#if defined(__GNUC__) || defined(__GNUG__)

	typedef	typename CBTreeBase_t::node_t								node_t;

	typedef typename ::CBTreeBaseConstIterator<CBTreeBase_t>			const_iterator;
	typedef typename ::CBTreeBaseConstReverseIterator<const_iterator>	const_reverse_iterator;

#endif

	typedef typename CBTreeKeySort_t::position_t						position_t;
	typedef const _t_keytype											data_t;
	typedef _t_sizetype													sizetype_t;
	typedef _t_nodeiter													nodeiter_t;
	typedef _t_subnodeiter												subnodeiter_t;
	typedef _t_datalayerproperties										datalayerproperties_t;
	typedef _t_datalayer												datalayer_t;

	typedef _t_keytype													value_t;

	typedef typename ::std::pair<iterator, bool>						insert_rslt_t;

	typedef	struct key_compare_s<_t_keytype>							key_compare;
	typedef struct value_compare_s<value_t>								value_compare;

	// construction
							CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

							CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(CBTreeSet_t &rBT, bool bAssign = true);

	// destruction
	virtual					~CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													();

	CBTreeSet_t &			operator=				(CBTreeSet_t &rBT);

	iterator				begin					();
	iterator				end						();
	reverse_iterator		rbegin					();
	reverse_iterator		rend					();
//	const_iterator			cbegin					();
//	const_iterator			cend					();
//	const_reverse_iterator	crbegin					();
//	const_reverse_iterator	crend					();

//	bool					empty					();
//	_t_sizetype				size					();
	_t_sizetype				max_size				();

	template <class _t_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);
	insert_rslt_t			insert					(const value_t &rData);

	iterator				erase					(const_iterator sCIterPos);
	_t_sizetype				erase					(const _t_keytype &rKey);
	iterator				erase					(const_iterator sCIterFirst, const_iterator sCIterLast);

//	void					swap					(CBTreeSet &rKeySort);

//	void					clear					();

	key_compare				key_comp				() const;
	value_compare			value_comp				() const;
	
	iterator				find					(const _t_keytype &rKey);
//	_t_sizetype				count					(const _t_keytype &rKey);
	iterator				lower_bound				(const _t_keytype &rKey);
	iterator				upper_bound				(const _t_keytype &rKey);

protected:

	int						comp					(const _t_keytype &rKey0, const _t_keytype &rKey1);

	static int				comp					(const _t_keytype &rKey0, const _t_keytype &rKey1, ::std::true_type);
	static int				comp					(const _t_keytype &rKey0, const _t_keytype &rKey1, ::std::false_type);

	template<class _t_iterator>
	bool					test_self_reference_of_iterator_to_this (_t_iterator &sItFirst, _t_iterator &sItLast, bool &bSelfReverse, const_iterator **ppsItFirst, const_iterator **ppsItLast)
	{
		return (btree_keysort_self_reference_of_iterator_to_this_arbiter<_t_keytype, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, _t_iterator>::test_self_reference_of_iterator_to_this (this, sItFirst, sItLast, bSelfReverse, ppsItFirst, ppsItLast));
	}

public:

	friend class CBTreeBaseIterator<CBTreeBase_t>;
	friend class CBTreeBaseConstIterator<CBTreeBase_t>;
	friend class CBTreeBaseReverseIterator<iterator>;
	friend class CBTreeBaseConstReverseIterator<const_iterator>;

};

#endif // BTREESET_H

#include "btreeset.cpp"

