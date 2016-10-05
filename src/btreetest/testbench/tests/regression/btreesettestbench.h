/************************************************************
**
** file:	btreesettestbench.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for the set application class.
**
************************************************************/

#ifndef BTREESETTESTBENCH_H
#define	BTREESETTESTBENCH_H

#include <stdint.h>

#include <set>
#include <list>
#include <vector>

template<class _t_key>
class CBTreeTestBenchSet;

#include "testbench/tests/regression/btreemultisettestbench.h"

#include "testbench/application_classes/regression/btreetestset.h"

#include "testbench/primitives/btreemultisetprimitives.h"

#include "testbench/common/btreetestcommon.h"
#include "specific_data_classes/btreeset.h"

typedef enum
{
	BTREETEST_SET_ASSIGNMENT_OPERATOR, 
	BTREETEST_SET_STL_IF_INSERT = 1000, 
	BTREETEST_SET_STL_IF_INSERT_VIA_ITERATOR, 
	BTREETEST_SET_STL_IF_ERASE_VIA_ITERATOR, 
	BTREETEST_SET_STL_IF_ERASE_VIA_KEY, 
	BTREETEST_SET_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE, 
	BTREETEST_SET_STL_IF_KEY_COMP, 
	BTREETEST_SET_STL_IF_VALUE_COMP, 
	BTREETEST_SET_STL_IF_SWAP, 
	BTREETEST_SET_STL_IF_FIND, 
	BTREETEST_SET_STL_IF_LOWER_BOUND_UPPER_BOUND
} btreetest_set_t;

template<class _t_key>
class CBTreeTestBenchSet	:	public ::std::set<_t_key>
{
public:

				CBTreeTestBenchSet<_t_key> ()	
					:	::std::set<_t_key> () 
				{};

				CBTreeTestBenchSet<_t_key> (const CBTreeTestBenchSet &rMap)	
					:	::std::set<_t_key> (rMap) 
				{};

	explicit	CBTreeTestBenchSet<_t_key> (const ::std::set<_t_key> &rMap)	
					:	::std::set<_t_key> (rMap) 
				{};

				~CBTreeTestBenchSet<_t_key> ()	
				{};
/*
	template<class _t_iterator>
	void insert (const typename ::std::set<_t_key>::iterator &rDummyIter, _t_iterator &rIterFirst, _t_iterator &rIterLast)
	{
		rDummyIter;

		::std::set<_t_key>::insert (rIterFirst, rIterLast);
	};
*/
	template<class _t_iterator>
	void insert (const typename ::std::set<_t_key>::const_iterator &rDummyIter, _t_iterator &rIterFirst, _t_iterator &rIterLast)
	{
		rDummyIter;

		::std::set<_t_key>::insert (rIterFirst, rIterLast);
	};

	template<class _t_iterator>
	void insert (_t_iterator &rIterFirst, _t_iterator &rIterLast)
	{
		::std::set<_t_key>::insert (rIterFirst, rIterLast);
	};

	// this version of insert only returns an iterator, instead of pair<iterator, bool>, although
	// this class is derived from ::std::set
	// - the masking of the bool component of this method has been done to make it interface compatible
	//   with any other associative container class, when set as a template parameter
	typename ::std::set<_t_key>::iterator insert (const typename ::std::set<_t_key>::value_type &rVal)
	{
		::std::pair<typename ::std::set<_t_key>::iterator, bool>	sRetval;

		if (::std::set<_t_key>::count (rVal) == 0)
		{
			sRetval = ::std::set<_t_key>::insert (rVal);

			return (sRetval.first);
		}
		else
		{
			return (::std::set<_t_key>::end ());
		}
	};
};

typedef CBTreeTestBenchSet<uint32_t>				set_reference_t;

template<class _t_container, class _t_iterator>
void TestBTreeSetSTLifInsertViaIteratorPart (const char *pszTitle, int nArg, uint32_t nNumInstances, uint32_t nNumEntries, _t_container *pContainer, _t_iterator &rIter)
{
	uint32_t					nLastKey = 0;
	uint32_t					i;
	_t_iterator					sIterBegin;
	_t_iterator					sIterEnd;
	
	::std::cout << pszTitle << ::std::endl << ::std::flush;

	for (i = 0; i < nNumInstances; i++)
	{
		multiSetPrim_add (pContainer, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	}

	get_begin (pContainer, sIterBegin);
	get_end (pContainer, sIterEnd);

	::std::advance (sIterBegin, nArg);
	::std::advance (sIterEnd, 0 - nArg);

	if (!is_const_iterator (pContainer, sIterBegin))
	{
		if (!is_reverse_iterator (pContainer, sIterBegin))
		{
			typename _t_container::test_iterator				sIter;

			pContainer->insert_self_reference (sIterBegin, sIterEnd, sIter);
		}
		else
		{
			typename _t_container::test_reverse_iterator		sRIter;

			pContainer->insert_self_reference (sIterBegin, sIterEnd, sRIter);
		}
	}
	else
	{
		if (!is_reverse_iterator (pContainer, sIterBegin))
		{
			typename _t_container::test_const_iterator			sCIter;

			pContainer->insert_self_reference (sIterBegin, sIterEnd, sCIter);
		}
		else
		{
			typename _t_container::test_const_reverse_iterator	sCRIter;

			pContainer->insert_self_reference (sIterBegin, sIterEnd, sCRIter);
		}
	}

	pContainer->clear ();
}

template<class _t_dest_container, class _t_src_container, class _t_dest_iterator, class _t_src_iterator>
void TestBTreeSetSTLifInsertViaIteratorPart (const char *pszTitle, int nArg, uint32_t nNumInstances, uint32_t nNumEntries, _t_dest_container *pDestContainer, _t_src_container *pSrcContainer, _t_dest_iterator &rDestIter, _t_src_iterator &rSrcIter)
{
	uint32_t					nLastKey = 0;
	uint32_t					i;
	_t_src_iterator				sSrcIterBegin;
	_t_src_iterator				sSrcIterEnd;
	
	::std::cout << pszTitle << ::std::endl << ::std::flush;

	for (i = 0; i < nNumInstances; i++)
	{
		multiSetPrim_add (pSrcContainer, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	}

	get_begin (pSrcContainer, sSrcIterBegin);
	get_end (pSrcContainer, sSrcIterEnd);

	::std::advance (sSrcIterBegin, nArg);
	::std::advance (sSrcIterEnd, 0 - nArg);

	pDestContainer->insert (sSrcIterBegin, sSrcIterEnd);

	pDestContainer->clear ();
	pSrcContainer->clear ();
}

template<class _t_dest_container, class _t_src_container, class _t_dest_iterator, class _t_src_iterator>
void TestBTreeSetSTLifInsertViaIteratorSame (const char *pszTitle, int nArg, uint32_t nNumInstances, uint32_t nNumEntries, _t_dest_container *pDestContainer, _t_src_container *pSrcContainer, _t_dest_iterator &rDestIter, _t_src_iterator &rSrcIter)
{
	uint32_t					nLastKey = 0;
	uint32_t					i;
	_t_src_iterator				sSrcIterBegin;
	_t_src_iterator				sSrcIterEnd;
	_t_src_iterator				sSrcIterA;
	_t_src_iterator				sSrcIterB;
	_t_dest_iterator			sDestIterA;
	
	::std::cout << pszTitle << ::std::endl << ::std::flush;

	for (i = 0; i < nNumInstances; i++)
	{
		multiSetPrim_add (pSrcContainer, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	}

	get_end (pSrcContainer, sSrcIterEnd);

	get_begin (pSrcContainer, sSrcIterA);
	get_begin (pSrcContainer, sSrcIterB);

	while (sSrcIterA != sSrcIterEnd)
	{
		pDestContainer->insert (sSrcIterA, sSrcIterB);

		::std::advance (sSrcIterA, 1);
		::std::advance (sSrcIterB, 1);
	}

	if (pDestContainer != pSrcContainer)
	{
		if (pDestContainer->size () != 0)
		{
			::std::cerr << "TestBTreeSetSTLifInsertViaIteratorSame: destination container ought to be empty!" << ::std::endl;

			exit (-1);
		}
	}
	else
	{
		if (pDestContainer->size () != pSrcContainer->size ())
		{
			::std::cerr << "TestBTreeSetSTLifInsertViaIteratorSame: destination container and source container mismatch in size!" << ::std::endl;

			exit (-1);
		}
	}

	get_begin (pSrcContainer, sSrcIterA);
	
	while (sSrcIterA != sSrcIterEnd)
	{
		pDestContainer->insert (sSrcIterA, sSrcIterA);

		::std::advance (sSrcIterA, 1);
	}

	if (pDestContainer != pSrcContainer)
	{
		if (pDestContainer->size () != 0)
		{
			::std::cerr << "TestBTreeSetSTLifInsertViaIteratorSame: destination container ought to be empty! (2)" << ::std::endl;

			exit (-1);
		}
	}
	else
	{
		if (pDestContainer->size () != pSrcContainer->size ())
		{
			::std::cerr << "TestBTreeSetSTLifInsertViaIteratorSame: destination container and source container mismatch in size! (2)" << ::std::endl;

			exit (-1);
		}
	}

	pDestContainer->clear ();
	pSrcContainer->clear ();
}

template<class _t_dest_container, class _t_src_container, class _t_ext_container, class _t_dest_iterator, class _t_src_iterator, class _t_ext_iterator>
void TestBTreeSetSTLifInsertViaIteratorPartExtern (const char *pszTitle, int nArg, uint32_t nNumInstances, uint32_t nNumEntries, _t_dest_container *pDestContainer, _t_src_container *pSrcContainer, _t_ext_container *pExtContainer, _t_dest_iterator &rDestIter, _t_src_iterator &rSrcIter, _t_ext_iterator &rExtIter)
{
	uint32_t					nLastKey = 0;
	uint32_t					i;
	_t_src_iterator				sSrcIterBegin;
	_t_src_iterator				sSrcIterA;
	_t_src_iterator				sSrcIterB;
	_t_dest_iterator			sDestIterA;
	
	::std::cout << pszTitle << ::std::endl << ::std::flush;

	for (i = 0; i < nNumInstances; i++)
	{
		multiSetPrim_add (pSrcContainer, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	}

	_t_ext_iterator		sExtIterBegin;
	_t_ext_iterator		sExtIterEnd;

	get_begin (pExtContainer, sExtIterBegin);

	get_begin (pSrcContainer, sSrcIterA);
	sSrcIterBegin = sSrcIterA;
	get_end (pSrcContainer, sSrcIterB);

	::std::advance (sSrcIterA, nArg);
	::std::advance (sSrcIterB, 0 - nArg);

	pSrcContainer->test_all_containers_insert (pExtContainer, sExtIterBegin, sSrcIterBegin, sSrcIterA, sSrcIterB);

	get_begin (pExtContainer, sExtIterBegin);
	get_end (pExtContainer, sExtIterEnd);

	pDestContainer->insert (sExtIterBegin, sExtIterEnd);

	pDestContainer->clear ();
	pSrcContainer->clear ();
	pExtContainer->clear ();
}

template<class _t_dest_container, class _t_src_container, class _t_ext_container, class _t_dest_iterator, class _t_src_iterator, class _t_ext_iterator>
void TestBTreeSetSTLifInsertViaIteratorSameExtern (const char *pszTitle, int nArg, uint32_t nNumInstances, uint32_t nNumEntries, _t_dest_container *pDestContainer, _t_src_container *pSrcContainer, _t_ext_container *pExtContainer, _t_dest_iterator &rDestIter, _t_src_iterator &rSrcIter, _t_ext_iterator &rExtIter)
{
	typedef typename ::std::set<typename _t_ext_iterator::value_type>		set_t;
	typedef typename ::std::multiset<typename _t_ext_iterator::value_type>	mset_t;

	typename ::std::conditional< ::std::is_same<_t_ext_container, set_t> ::value || ::std::is_same<_t_ext_container, mset_t> ::value, ::std::true_type, ::std::false_type> ::type
								sSelectInsertMethod;

	uint32_t					nLastKey = 0;
	uint32_t					i;
	_t_src_iterator				sSrcIterBegin;
	_t_src_iterator				sSrcIterEnd;
	_t_src_iterator				sSrcIterA;
	_t_src_iterator				sSrcIterB;
	_t_dest_iterator			sDestIterA;
	
	::std::cout << pszTitle << ::std::endl << ::std::flush;

	for (i = 0; i < nNumInstances; i++)
	{
		multiSetPrim_add (pSrcContainer, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	}

	get_begin (pSrcContainer, sSrcIterBegin);
	get_end (pSrcContainer, sSrcIterEnd);

	generic_insert (pExtContainer, pExtContainer->cbegin (), sSrcIterBegin, sSrcIterEnd, sSelectInsertMethod);

	_t_ext_iterator		sExtIter;

	get_begin (pExtContainer, sExtIter);

	get_begin (pSrcContainer, sSrcIterA);
	get_begin (pSrcContainer, sSrcIterB);
	
	while (sSrcIterA != sSrcIterEnd)
	{
		pDestContainer->test_all_containers_insert (pExtContainer, sExtIter, sSrcIterBegin, sSrcIterA, sSrcIterB);

		::std::advance (sSrcIterA, 1);
		::std::advance (sSrcIterB, 1);

		::std::advance (sExtIter, 1);
	}

	if (pDestContainer != pSrcContainer)
	{
		if (pDestContainer->size () != 0)
		{
			::std::cerr << "TestBTreeSetSTLifInsertViaIteratorSameExtern: destination container ought to be empty!" << ::std::endl;

			exit (-1);
		}
	}
	else
	{
		if (pDestContainer->size () != pSrcContainer->size ())
		{
			::std::cerr << "TestBTreeSetSTLifInsertViaIteratorSameExtern: destination container and source container mismatch in size!" << ::std::endl;

			exit (-1);
		}
	}

	get_begin (pSrcContainer, sSrcIterA);
	sSrcIterBegin = sSrcIterA;
	
	while (sSrcIterA != sSrcIterEnd)
	{
		pDestContainer->test_all_containers_insert (pSrcContainer, sSrcIterA, sSrcIterBegin, sSrcIterA, sSrcIterA);

		::std::advance (sSrcIterA, 1);

		::std::cout << "*" << ::std::flush;
	}

	::std::cout << ::std::endl;

	if (pDestContainer != pSrcContainer)
	{
		if (pDestContainer->size () != 0)
		{
			::std::cerr << "TestBTreeSetSTLifInsertViaIteratorSameExtern: destination container ought to be empty! (2)" << ::std::endl;

			exit (-1);
		}
	}
	else
	{
		if (pDestContainer->size () != pSrcContainer->size ())
		{
			::std::cerr << "TestBTreeSetSTLifInsertViaIteratorSameExtern: destination container and source container mismatch in size! (2)" << ::std::endl;

			exit (-1);
		}
	}

	pDestContainer->clear ();
	pSrcContainer->clear ();
	
}

template<class _t_map>
void TestBTreeSetSTLifInsertViaIterator (_t_map *pClM, uint32_t nNumEntries)
{
	typedef typename _t_map::value_type													value_t;

	typedef typename _t_map::iterator													iter_t;
	typedef typename _t_map::const_iterator												citer_t;
	typedef typename _t_map::reverse_iterator											riter_t;
	typedef typename _t_map::const_reverse_iterator										criter_t;

	typedef typename ::std::list<value_t>::iterator										itlist_t;
	typedef typename ::std::list<value_t>::const_iterator								citlist_t;
	typedef typename ::std::list<value_t>::reverse_iterator								ritlist_t;
	typedef typename ::std::list<value_t>::const_reverse_iterator						critlist_t;

	typedef typename ::std::vector<value_t>::iterator									itvec_t;
	typedef typename ::std::vector<value_t>::const_iterator								citvec_t;
	typedef typename ::std::vector<value_t>::reverse_iterator							ritvec_t;
	typedef typename ::std::vector<value_t>::const_reverse_iterator						critvec_t;

	typedef typename ::std::set<uint32_t>::iterator										itset_t;
	typedef typename ::std::set<uint32_t>::const_iterator								citset_t;
	typedef typename ::std::set<uint32_t>::reverse_iterator								ritset_t;
	typedef typename ::std::set<uint32_t>::const_reverse_iterator						critset_t;

	typedef typename ::std::multiset<uint32_t>::iterator								itmset_t;
	typedef typename ::std::multiset<uint32_t>::const_iterator							citmset_t;
	typedef typename ::std::multiset<uint32_t>::reverse_iterator						ritmset_t;
	typedef typename ::std::multiset<uint32_t>::const_reverse_iterator					critmset_t;
	
	_t_map										sClMMTarget (*pClM);
	uint32_t									nLastKey = 0;
	iter_t										sIterA, sIterB;
	citer_t										sCIterA, sCIterB;
	riter_t										sRIterA, sRIterB;
	criter_t									sCRIterA, sCRIterB;
	::std::list<value_t>						sList;
	::std::vector<value_t>						sVector;
	itlist_t									sItList;
	citlist_t									sCItList;
	ritlist_t									sRItList;
	critlist_t									sCRItList;
	itvec_t										sItVec;
	citvec_t									sCItVec;
	ritvec_t									sRItVec;
	critvec_t									sCRItVec;
	CBTreeTestBenchSet<uint32_t>				sSet;
	itset_t										sItSet;
	citset_t									sCItSet;
	ritset_t									sRItSet;
	critset_t									sCRItSet;
	CBTreeTestBenchMultiSet<uint32_t>			sMSet;
	itmset_t									sItMSet;
	citmset_t									sCItMSet;
	ritmset_t									sRItMSet;
	critmset_t									sCRItMSet;
	
	::std::cout << "exercises method compatible to STL interface CBTreeSet[Multi]<>:: insert<_t_iterator> (_t_iterator, _t_iterator)" << ::std::endl;

	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::iter_t> (iter_t, iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sIterA, sIterB);
	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::citer_t> (citer_t, citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sCIterA, sCIterB);
	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::riter_t> (riter_t, riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sRIterA, sRIterB);
	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::criter_t> (criter_t, criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sCRIterA, sCRIterB);

	TestBTreeSetSTLifInsertViaIteratorSame ("target::insert<_t_obj::iter_t> (iter_t == iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sIterA, sIterB);
	TestBTreeSetSTLifInsertViaIteratorSame ("target::insert<_t_obj::citer_t> (citer_t == citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sCIterA, sCIterB);
	TestBTreeSetSTLifInsertViaIteratorSame ("target::insert<_t_obj::riter_t> (riter_t == riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sRIterA, sRIterB);
	TestBTreeSetSTLifInsertViaIteratorSame ("target::insert<_t_obj::criter_t> (criter_t == criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sCRIterA, sCRIterB);

	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::iter_t> (>iter_t, iter_t<)", (int) (nNumEntries / 4), 1, nNumEntries, &sClMMTarget, pClM, sIterA, sIterB);
	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::citer_t> (>citer_t, citer_t<)", (int) (nNumEntries / 4), 1, nNumEntries, &sClMMTarget, pClM, sCIterA, sCIterB);
	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::riter_t> (>riter_t, riter_t<)", (int) (nNumEntries / 4), 1, nNumEntries, &sClMMTarget, pClM, sRIterA, sRIterB);
	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::criter_t> (>criter_t, criter_t<)", (int) (nNumEntries / 4), 1, nNumEntries, &sClMMTarget, pClM, sCRIterA, sCRIterB);

	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::iter_t> (target::iter_t, target::iter_t)", 0, 1, nNumEntries, pClM, sIterA);
	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::citer_t> (target::citer_t, target::citer_t)", 0, 1, nNumEntries, pClM, sCIterA);
	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::riter_t> (target::riter_t, target::riter_t)", 0, 1, nNumEntries, pClM, sRIterA);
	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::criter_t> (target::criter_t, target::criter_t)", 0, 1, nNumEntries, pClM, sCRIterA);

	TestBTreeSetSTLifInsertViaIteratorSame ("target::insert<_t_obj::iter_t> (target::iter_t == target::iter_t)", 0, 1, nNumEntries, pClM, pClM, sIterA, sIterB);
	TestBTreeSetSTLifInsertViaIteratorSame ("target::insert<_t_obj::citer_t> (target::citer_t == target::citer_t)", 0, 1, nNumEntries, pClM, pClM, sCIterA, sCIterB);
	TestBTreeSetSTLifInsertViaIteratorSame ("target::insert<_t_obj::riter_t> (target::riter_t == target::riter_t)", 0, 1, nNumEntries, pClM, pClM, sRIterA, sRIterB);
	TestBTreeSetSTLifInsertViaIteratorSame ("target::insert<_t_obj::criter_t> (target::criter_t == target::criter_t)", 0, 1, nNumEntries, pClM, pClM, sCRIterA, sCRIterB);

	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::iter_t> (>iter_t, iter_t<) x2", (int) (nNumEntries / 4), 2, nNumEntries, pClM, sIterA);
	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::citer_t> (>citer_t, citer_t<) x2", (int) (nNumEntries / 4), 2, nNumEntries, pClM, sCIterA);
	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::riter_t> (>riter_t, riter_t<) x2", (int) (nNumEntries / 4), 2, nNumEntries, pClM, sRIterA);
	TestBTreeSetSTLifInsertViaIteratorPart ("target::insert<_t_obj::criter_t> (>criter_t, criter_t<) x2", (int) (nNumEntries / 4), 2, nNumEntries, pClM, sCRIterA);

	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<list::iter_t> (iter_t, iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sIterA, sCIterB, sItList);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<list::citer_t> (citer_t, citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sCIterA, sCIterB, sCItList);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<list::riter_t> (riter_t, riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sRIterA, sCIterB, sItList);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<list::criter_t> (criter_t, criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sCRIterA, sCIterB, sCItList);

	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<list::iter_t> (iter_t == iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sIterA, sIterB, sItList);
	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<list::citer_t> (citer_t == citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sCIterA, sCIterB, sCItList);
	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<list::riter_t> (riter_t == riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sRIterA, sRIterB, sItList);
	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<list::criter_t> (criter_t == criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sCRIterA, sCRIterB, sCItList);

	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<list::iter_t> (>iter_t, iter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sList, sIterA, sCIterB, sItList);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<list::citer_t> (>citer_t, citer_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sList, sCIterA, sCIterB, sCItList);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<list::riter_t> (>riter_t, riter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sList, sRIterA, sCIterB, sItList);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<list::criter_t> (>criter_t, criter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sList, sCRIterA, sCIterB, sCItList);

	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<vector::iter_t> (iter_t, iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sIterA, sCIterB, sItVec);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<vector::citer_t> (citer_t, citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sCIterA, sCIterB, sCItVec);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<vector::riter_t> (riter_t, riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sRIterA, sCIterB, sItVec);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<vector::criter_t> (criter_t, criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sCRIterA, sCIterB, sCItVec);

	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<vector::iter_t> (iter_t == iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sIterA, sIterB, sItVec);
	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<vector::citer_t> (citer_t == citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sCIterA, sCIterB, sCItVec);
	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<vector::riter_t> (riter_t == riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sRIterA, sRIterB, sItVec);
	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<vector::criter_t> (criter_t == criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sCRIterA, sCRIterB, sCItVec);

	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<vector::iter_t> (>iter_t, iter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sVector, sIterA, sCIterB, sItVec);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<vector::citer_t> (>citer_t, citer_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sVector, sCIterA, sCIterB, sCItVec);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<vector::riter_t> (>riter_t, riter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sVector, sRIterA, sCIterB, sItVec);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<vector::criter_t> (>criter_t, criter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sVector, sCRIterA, sCIterB, sCItVec);

	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<set::iter_t> (iter_t, iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sSet, sIterA, sIterB, sItSet);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<set::citer_t> (citer_t, citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sSet, sCIterA, sCIterB, sCItSet);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<set::riter_t> (riter_t, riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sSet, sRIterA, sRIterB, sItSet);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<set::criter_t> (criter_t, criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sSet, sCRIterA, sCRIterB, sCItSet);

	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<set::iter_t> (iter_t == iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sSet, sIterA, sIterB, sItSet);
	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<set::citer_t> (citer_t == citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sSet, sCIterA, sCIterB, sCItSet);
	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<set::riter_t> (riter_t == riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sSet, sRIterA, sRIterB, sItSet);
	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<set::criter_t> (criter_t == criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sSet, sCRIterA, sCRIterB, sCItSet);

	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<set::iter_t> (>iter_t, iter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sSet, sIterA, sIterB, sItSet);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<set::citer_t> (>citer_t, citer_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sSet, sCIterA, sCIterB, sCItSet);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<set::riter_t> (>riter_t, riter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sSet, sRIterA, sRIterB, sItSet);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<set::criter_t> (>criter_t, criter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sSet, sCRIterA, sCRIterB, sCItSet);

	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<mset::iter_t> (iter_t, iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMSet, sIterA, sIterB, sItMSet);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<mset::citer_t> (citer_t, citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMSet, sCIterA, sCIterB, sCItMSet);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<mset::riter_t> (riter_t, riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMSet, sRIterA, sRIterB, sItMSet);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<mset::criter_t> (criter_t, criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMSet, sCRIterA, sCRIterB, sCItMSet);

	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<mset::iter_t> (iter_t == iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMSet, sIterA, sIterB, sItMSet);
	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<mset::citer_t> (citer_t == citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMSet, sCIterA, sCIterB, sCItMSet);
	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<mset::riter_t> (riter_t == riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMSet, sRIterA, sRIterB, sItMSet);
	TestBTreeSetSTLifInsertViaIteratorSameExtern ("target::insert<mset::criter_t> (criter_t == criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMSet, sCRIterA, sCRIterB, sCItMSet);

	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<mset::iter_t> (>iter_t, iter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sMSet, sIterA, sIterB, sItMSet);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<mset::citer_t> (>citer_t, citer_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sMSet, sCIterA, sCIterB, sCItMSet);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<mset::riter_t> (>riter_t, riter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sMSet, sRIterA, sRIterB, sItMSet);
	TestBTreeSetSTLifInsertViaIteratorPartExtern ("target::insert<mset::criter_t> (>criter_t, criter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sMSet, sCRIterA, sCRIterB, sCItMSet);
}

template<class _t_container>
void TestBTreeSTLset (uint32_t nTestNum, uint32_t nNodeSize, uint32_t nPageSize, _t_container *pContainerpSetWrapper);

#endif // !BTREESETTESTBENCH_H
