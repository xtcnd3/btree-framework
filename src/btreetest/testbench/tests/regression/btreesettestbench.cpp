/************************************************************
**
** file:	btreesettestbench.cpp
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

#include "btreesettestbench.h"

template<class _t_set>
void TestBTreeSetSTLifAssignmentOperator (_t_set *pClSet, typename _t_set::size_type nNumEntries)
{
	typedef typename _t_set::const_iterator		citer_t;
	typedef typename _t_set::key_type			key_type;

	_t_set										sClSet (*pClSet);
	key_type									nLastKey = 0;
	citer_t										sIter;
	citer_t										sIterRslt;
	
	::std::cout << "exercises assignment operator compatible to STL interface CBTreeSet<>:: operator= (CBTreeSet<> &)" << ::std::endl;

	multiSetPrim_add (pClSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sClSet = *pClSet;

	if (sClSet != (*pClSet))
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeSetSTLifAssignmentOperator<>: ERROR: mismatch after assignment operation" << ::std::endl;

		exit (-1);
	}
}

template<class _t_set>
void TestBTreeSetSTLifInsert (_t_set *pClSet, typename _t_set::size_type nNumEntries)
{
	typename _t_set::key_type	nLastSeed = 0;

	::std::cout << "CBTreeSet::insert (const valuetype &) is exercised" << ::std::endl;

	multiSetPrim_add (pClSet, nNumEntries, nLastSeed, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
}

template<class _t_set>
void TestBTreeSetSTLifEraseViaIterator (_t_set *pClSet, typename _t_set::size_type nNumEntries)
{
	typedef typename _t_set::const_iterator		citer_t;
	typedef typename _t_set::size_type			size_type;
	typedef typename _t_set::key_type			key_type;

	key_type									nLastKey = 0;
	citer_t										sIter;
	citer_t										sIterRslt;
	uint32_t									nDebug = 0;
	size_type									nOffset;
	size_type									nRslt;
	
	::std::cout << "exercises method compatible to STL interface CBTreeSet<>:: erase (const_iterator)" << ::std::endl;

	multiSetPrim_add (pClSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	while (pClSet->size () > 0)
	{
		nOffset = generate_rand32 () % pClSet->size ();
		
		sIter = pClSet->cbegin ();

		::std::advance (sIter, nOffset);

		sIterRslt = pClSet->erase (sIter);

		nRslt = ::std::distance (pClSet->cbegin (), sIterRslt);

		if (nOffset != nRslt)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeSetSTLifEraseViaIterator: returned iterator mismatches!" << ::std::endl;

			exit (-1);
		}

		nDebug++;
	}
}

template<class _t_set>
void TestBTreeSetSTLifEraseViaKey (_t_set *pClSet, typename _t_set::size_type nNumEntries, uint32_t nInstances)
{
	typedef typename _t_set::const_iterator		citer_t;
	typedef typename _t_set::size_type			size_type;
	typedef typename _t_set::key_type			key_type;

	key_type						nLastKey;
	key_type						nKey;
	citer_t							sIter;
	uint32_t						i;
	size_type						nRslt;
	size_type						nOffset;
	
	::std::cout << "exercises method compatible to STL interface CBTreeSet<>:: erase (const keytype &)" << ::std::endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		multiSetPrim_add (pClSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	}

	while (pClSet->size () > 0)
	{
		nOffset = generate_rand32 () % pClSet->size ();
		
		sIter = pClSet->cbegin ();

		::std::advance (sIter, nOffset);

		nKey = (*sIter);

		nRslt = pClSet->erase (nKey);

		if (nRslt != 1)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeSetSTLifEraseViaKey: unexpected number of items deleted!" << ::std::endl;

			exit (-1);
		}
	}
}

template<class _t_set>
void TestBTreeSetSTLifEraseViaIteratorMultiple (_t_set *pClSet, typename _t_set::size_type nNumEntries, uint32_t nInstances)
{
	typedef typename _t_set::const_iterator		citer_t;
	typedef typename _t_set::size_type			size_type;
	typedef typename _t_set::key_type			key_type;

	key_type									nLastKey;
	citer_t										sIterBegin;
	citer_t										sIterEnd;
	citer_t										sIterRslt;
	uint32_t									i;
	size_type									nBegin;
	size_type									nRslt;
	
	::std::cout << "exercises method compatible to STL interface CBTreeSet<>:: erase (const_iterator, const_iterator)" << ::std::endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		multiSetPrim_add (pClSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	}

	while (pClSet->size () > 0)
	{
		sIterBegin = pClSet->cbegin ();
		sIterEnd = pClSet->cend ();

		nBegin = 0;

		if (pClSet->size () > 1)
		{
			nBegin = generate_rand32 () % (pClSet->size () / 2);

			::std::advance (sIterBegin, nBegin);
			::std::advance (sIterEnd, (int) (0 - (generate_rand32 () % (pClSet->size () / 2))));
		}

		sIterRslt = pClSet->erase (sIterBegin, sIterEnd);

		nRslt = ::std::distance (pClSet->cbegin (), sIterRslt);

		if (nRslt != nBegin)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeSetSTLifEraseViaIteratorMultiple: returned iterator mismatches!" << ::std::endl;

			exit (-1);
		}
	}
}

template<class _t_set>
void TestBTreeSetSTLifKeyComp (_t_set *pClSet, typename _t_set::size_type nNumEntries)
{
	typedef typename _t_set::iterator			iter_t;
	typedef typename _t_set::key_type			key_type;

	key_type									nLastKey = 0;
	iter_t										sIterBegin;
	iter_t										sIterEnd;
	iter_t										sIter;
	iter_t										sIterUpper;
	key_type									nKey;
	key_type									nNextKey;
	typename _t_set::key_compare				sMMComp = pClSet->key_comp ();
	
	::std::cout << "exercises method compatible to STL interface CBTreeSet<>:: key_comp ()" << ::std::endl;

	multiSetPrim_add (pClSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClSet->begin ();
	sIterEnd = pClSet->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = ((key_type)(*sIter));

		sIterUpper = pClSet->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			nNextKey = ((key_type)(*sIterUpper));

			if (!sMMComp (nKey, nNextKey))
			{
				::std::cerr << ::std::endl;
				::std::cerr << "TestBTreeSetSTLifKeyComp: ERROR: CBTreeSet::key_compare (const keytype &, const keytype &) delivered unexpected result!" << ::std::endl;

				exit (-1);
			}
		}
	}
}

template<class _t_set>
void TestBTreeSetSTLifValueComp (_t_set *pClSet, typename _t_set::size_type nNumEntries)
{
	typedef typename _t_set::iterator		iter_t;
	typedef typename _t_set::key_type		key_type;

	key_type									nLastKey = 0;
	iter_t										sIterBegin;
	iter_t										sIterEnd;
	iter_t										sIter;
	iter_t										sIterUpper;
	key_type									nKey;
	typename _t_set::value_compare				sMMComp = pClSet->value_comp ();
	
	::std::cout << "exercises method compatible to STL interface CBTreeSet<>:: value_comp ()" << ::std::endl;

	multiSetPrim_add (pClSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClSet->begin ();
	sIterEnd = pClSet->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = ((key_type)(*sIter));

		sIterUpper = pClSet->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			if (!sMMComp (((key_type)(*sIter)), ((key_type)(*sIterUpper))))
			{
				::std::cerr << ::std::endl;
				::std::cerr << "TestBTreeSetSTLifValueComp: ERROR: CBTreeSet::value_compare (const valuetype &, const valuetype &) delivered unexpected result!" << ::std::endl;

				exit (-1);
			}
		}
	}
}

template<class _t_set>
void TestBTreeSetSTLifSwap (_t_set *pClSet, typename _t_set::size_type nNumEntries)
{
	typedef typename _t_set::const_iterator			citer_t;
	typedef typename _t_set::key_type				key_type;

	key_type					nLastKey = 0;
	_t_set						sClSetTarget (*pClSet);
	_t_set						sClSetRef (*pClSet);
	_t_set						sClSetTargetRef (*pClSet);

	::std::cout << "exercises method compatible to STL interface CBTreeSet<>:: swap (CBTreeSet &)" << ::std::endl;

	multiSetPrim_add (pClSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sClSetRef.insert (pClSet->cbegin (), pClSet->cend ());

	if ((*pClSet) != sClSetRef)
	{
		::std::cerr << "ERROR: Unexpected multimap mismatch!" << ::std::endl;

		exit (-1);
	}

	multiSetPrim_add (&sClSetTarget, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sClSetTargetRef.insert (sClSetTarget.cbegin (), sClSetTarget.cend ());

	if (sClSetTarget != sClSetTargetRef)
	{
		::std::cerr << "ERROR: Unexpected target mismatch!" << ::std::endl;

		exit (-1);
	}

	pClSet->swap (sClSetTarget);

	if ((*pClSet) != sClSetTargetRef)
	{
		::std::cerr << "ERROR: swapped multimap mismatches target reference!" << ::std::endl;

		exit (-1);
	}

	if (sClSetTarget != sClSetRef)
	{
		::std::cerr << "ERROR: target mismatches multimap reference!" << ::std::endl;

		exit (-1);
	}

	sClSetTarget.swap (*pClSet);

	if ((*pClSet) != sClSetRef)
	{
		::std::cerr << "ERROR: multimap mismatches multimap reference!" << ::std::endl;

		exit (-1);
	}

	if (sClSetTarget != sClSetTargetRef)
	{
		::std::cerr << "ERROR: target mismatches target reference!" << ::std::endl;

		exit (-1);
	}
}

template<class _t_set>
void TestBTreeSetSTLifFind (_t_set *pClSet, typename _t_set::size_type nNumEntries)
{
	typedef typename _t_set::const_iterator			citer_t;
	typedef typename _t_set::key_type				key_type;

	key_type						nLastKey = 1;
	key_type						nKey;
	citer_t							sCIter;
	citer_t							sCIterRslt;
	
	::std::cout << "exercises method compatible to STL interface CBTreeSet<>:: find (const _t_keytype &)" << ::std::endl;

	multiSetPrim_add (pClSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_ASCEND_KEY);

	for (sCIter = pClSet->cbegin (); sCIter != pClSet->cend (); sCIter++)
	{
		nKey = *sCIter;

		sCIterRslt = pClSet->find (nKey);

		if (sCIter != sCIterRslt)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeSetSTLifFind: ERROR: iterator mismatch!" << ::std::endl;

			exit (-1);
		}
	}

	nKey = 0;

	sCIterRslt = pClSet->find (nKey);

	if (sCIterRslt != pClSet->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeSetSTLifFind: ERROR: find () didn't return cend () on lower bound check!" << ::std::endl;

		exit (-1);
	}

	nKey = (key_type) (nNumEntries + 1);

	sCIterRslt = pClSet->find (nKey);

	if (sCIterRslt != pClSet->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeSetSTLifFind: ERROR: find () didn't return cend () on upper bound check!" << ::std::endl;

		exit (-1);
	}
}

template<class _t_set>
void TestBTreeSetSTLifLowerBoundUpperBound (_t_set *pClSet, typename _t_set::size_type nNumEntries, uint32_t nInstance)
{
	typedef typename _t_set::const_iterator			citer_t;
	typedef typename _t_set::key_type				key_type;

	key_type						nLastKey;
	uint32_t						i;
	key_type						nKey;
	citer_t							sCIterLower;
	citer_t							sCIterUpper;
	
	::std::cout << "exercises method compatible to STL interface CBTreeSet<>:: lower_bound () upper_bound () count ()" << ::std::endl;
	
	for (i = 0; i < nInstance; i++)
	{
		nLastKey = 1;

		multiSetPrim_add (pClSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_ASCEND_KEY);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		nKey = i + 1;

		sCIterLower = pClSet->lower_bound (nKey);
		sCIterUpper = pClSet->upper_bound (nKey);

		::std::advance (sCIterLower, (int) 1);

		if (sCIterLower != sCIterUpper)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeSetSTLifLowerBoundUpperBound: ERROR: boundary mismatch!" << ::std::endl;

			exit (-1);
		}

		if (pClSet->count (nKey) != 1)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeSetSTLifLowerBoundUpperBound: ERROR: count () returned with unexpected value!" << ::std::endl;

			exit (-1);
		}
	}

	nKey = 0;

	sCIterLower = pClSet->lower_bound (nKey);

	if (sCIterLower != pClSet->cbegin ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeSetSTLifLowerBoundUpperBound: ERROR: lower_bound () didn't return cbegin () when testing for lowest key!" << ::std::endl;

		exit (-1);
	}

	nKey = (key_type) (nNumEntries + 1);

	sCIterUpper = pClSet->upper_bound (nKey);

	if (sCIterUpper != pClSet->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeSetSTLifLowerBoundUpperBound: ERROR: upper_bound () didn't return cend () when testing for lowest key!" << ::std::endl;

		exit (-1);
	}
}

template<class _t_container>
void TestBTreeSTLset (uint32_t nTestNum, uint32_t nNodeSize, uint32_t nPageSize, _t_container *pContainerpSetWrapper)
{
	typename _t_container::size_test_type		sTypeSelect;
	::std::string								sTypeStr;

	get_typename (sTypeSelect, sTypeStr);

	::std::cout << "b-tree set test bench selected using type " << sTypeStr << ::std::endl;

	pContainerpSetWrapper = new _t_container (nNodeSize, nPageSize);

	if (pContainerpSetWrapper == NULL)
	{
		::std::cerr << "insufficient memory!" << ::std::endl;

		exit (-1);
	}

	switch (nTestNum)
	{
	case BTREETEST_SET_ASSIGNMENT_OPERATOR	:
		{
			TestBTreeSetSTLifAssignmentOperator (pContainerpSetWrapper, 64);

			break;
		}
	
	case BTREETEST_SET_STL_IF_INSERT	:
		{
			TestBTreeSetSTLifInsert (pContainerpSetWrapper, 64);

			break;
		}
	
	case BTREETEST_SET_STL_IF_INSERT_VIA_ITERATOR	:
		{
			TestBTreeSetSTLifInsertViaIterator (pContainerpSetWrapper, 64);

			break;
		}
	
	case BTREETEST_SET_STL_IF_ERASE_VIA_ITERATOR	:
		{
			TestBTreeSetSTLifEraseViaIterator (pContainerpSetWrapper, 64);

			break;
		}
	
	case BTREETEST_SET_STL_IF_ERASE_VIA_KEY	:
		{
			TestBTreeSetSTLifEraseViaKey (pContainerpSetWrapper, 64, 2);

			break;
		}
	
	case BTREETEST_SET_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE	:
		{
			TestBTreeSetSTLifEraseViaIteratorMultiple (pContainerpSetWrapper, 64, 2);

			break;
		}
	
	case BTREETEST_SET_STL_IF_KEY_COMP	:
		{
			TestBTreeSetSTLifKeyComp (pContainerpSetWrapper, 64);

			break;
		}
	
	case BTREETEST_SET_STL_IF_VALUE_COMP	:
		{
			TestBTreeSetSTLifValueComp (pContainerpSetWrapper, 64);

			break;
		}
	
	case BTREETEST_SET_STL_IF_SWAP	:
		{
			TestBTreeSetSTLifSwap (pContainerpSetWrapper, 128);

			break;
		}
	
	case BTREETEST_SET_STL_IF_FIND	:
		{
			TestBTreeSetSTLifFind (pContainerpSetWrapper, 64);

			break;
		}
	
	case BTREETEST_SET_STL_IF_LOWER_BOUND_UPPER_BOUND	:
		{
			TestBTreeSetSTLifLowerBoundUpperBound (pContainerpSetWrapper, 64, 5);

			break;
		}
	
	default:
		{
			::std::cerr << "ERROR: TestBTreeSTLset: Unknown test or test not specified!" << ::std::endl;

			exit (-1);

			break;
		}
	}

	delete pContainerpSetWrapper;
}
