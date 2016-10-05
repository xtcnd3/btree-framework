/************************************************************
**
** file:	btreemultisettestbench.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for the multi set application class.
**
************************************************************/

#ifndef BTREEMULTISETTESTBENCH_H
#define	BTREEMULTISETTESTBENCH_H

#include <stdint.h>

#include <set>
#include <list>
#include <vector>

template<class _t_key>
class CBTreeTestBenchMultiSet;

#include "testbench/tests/regression/btreesettestbench.h"

#include "testbench/application_classes/regression/btreetestmultiset.h"

#include "testbench/primitives/btreemultisetprimitives.h"

#include "testbench/common/btreetestcommon.h"
#include "specific_data_classes/btreemultiset.h"

template<class _t_key>
class CBTreeTestBenchMultiSet	:	public ::std::multiset<_t_key>
{
public:

				CBTreeTestBenchMultiSet<_t_key> ()	
					:	::std::multiset<_t_key> () 
				{};

				CBTreeTestBenchMultiSet<_t_key> (const CBTreeTestBenchMultiSet &rSet)	
					:	::std::multiset<_t_key> (rSet)
				{};

	explicit	CBTreeTestBenchMultiSet<_t_key> (const ::std::set<_t_key> &rSet)
					:	::std::multiset<_t_key> (rSet)
				{};

				~CBTreeTestBenchMultiSet<_t_key> ()	
				{};
/*
	template<class _t_iterator>
	void insert (const typename ::std::multiset<_t_key>::iterator &rDummyIter, _t_iterator &rIterFirst, _t_iterator &rIterLast)
	{
		rDummyIter;

		::std::multiset<_t_key>::insert (rIterFirst, rIterLast);
	};
*/
	template<class _t_iterator>
	void insert (const typename ::std::multiset<_t_key>::const_iterator &rDummyIter, _t_iterator &rIterFirst, _t_iterator &rIterLast)
	{
		rDummyIter;

		::std::multiset<_t_key>::insert (rIterFirst, rIterLast);
	};

	template<class _t_iterator>
	void insert (_t_iterator &rIterFirst, _t_iterator &rIterLast)
	{
		::std::multiset<_t_key>::insert (rIterFirst, rIterLast);
	};

	typename ::std::multiset<_t_key>::iterator insert (const typename ::std::multiset<_t_key>::value_type &rVal)
	{
		return (::std::multiset<_t_key>::insert (rVal));
	};
};

typedef CBTreeTestBenchMultiSet<uint32_t>			multiset_reference_t;

typedef enum
{
	BTREETEST_MULTISET_ASSIGNMENT_OPERATOR, 
	BTREETEST_MULTISET_STL_IF_INSERT = 1000, 
	BTREETEST_MULTISET_STL_IF_INSERT_VIA_ITERATOR, 
	BTREETEST_MULTISET_STL_IF_ERASE_VIA_ITERATOR, 
	BTREETEST_MULTISET_STL_IF_ERASE_VIA_KEY, 
	BTREETEST_MULTISET_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE, 
	BTREETEST_MULTISET_STL_IF_KEY_COMP, 
	BTREETEST_MULTISET_STL_IF_VALUE_COMP, 
	BTREETEST_MULTISET_STL_IF_SWAP, 
	BTREETEST_MULTISET_STL_IF_FIND, 
	BTREETEST_MULTISET_STL_IF_LOWER_BOUND_UPPER_BOUND
} btreetest_multiset_t;

template<class _t_container>
void TestBTreeSTLmultiSet (uint32_t nTestNum, uint32_t nNodeSize, uint32_t nPageSize, _t_container *pContainerpMSetWrapper);

#endif // !BTREEMULTISETTESTBENCH_H
