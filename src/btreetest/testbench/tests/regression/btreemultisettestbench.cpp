/************************************************************
**
** file:	btreemultisettestbench.cpp
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

#include "btreemultisettestbench.h"

template<class _t_multiset>
void TestBTreeMultiSetSTLifAssignmentOperator (_t_multiset *pClMSet, typename _t_multiset::size_type nNumEntries)
{
	typedef typename _t_multiset::const_iterator						citer_t;
	typedef typename _t_multiset::key_type								key_type;

	_t_multiset									sClMSet (*pClMSet);
	key_type									nLastKey = 0;
	citer_t										sIter;
	citer_t										sIterRslt;
	
	::std::cout << "exercises assignment operator compatible to STL interface CBTreeMultiSet<>:: operator= (CBTreeMultiSet<> &)" << ::std::endl;

	multiSetPrim_add (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sClMSet = *pClMSet;

	if (sClMSet != (*pClMSet))
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeMultiSetSTLifAssignmentOperator<>: ERROR: mismatch after assignment operation" << ::std::endl;

		exit (-1);
	}
}

template<class _t_multiset>
void TestBTreeMultiSetSTLifInsert (_t_multiset *pClMSet, typename _t_multiset::size_type nNumEntries)
{
	typename _t_multiset::key_type	nLastSeed = 0;

	::std::cout << "CBTreeMultiSet::insert (const valuetype &) is exercised" << ::std::endl;

	multiSetPrim_add (pClMSet, nNumEntries, nLastSeed, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
}

template<class _t_multiset>
void TestBTreeMultiSetSTLifEraseViaIterator (_t_multiset *pClMSet, typename _t_multiset::size_type nNumEntries)
{
	typedef typename _t_multiset::const_iterator	citer_t;
	typedef typename _t_multiset::size_type			size_type;
	typedef typename _t_multiset::key_type			key_type;

	key_type									nLastKey = 0;
	citer_t										sIter;
	citer_t										sIterRslt;
	size_type									nOffset;
	size_type									nRslt;
	
	::std::cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: erase (const_iterator)" << ::std::endl;

	multiSetPrim_add (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	while (pClMSet->size () > 0)
	{
		nOffset = generate_rand32 () % pClMSet->size ();

		sIter = pClMSet->cbegin ();

		::std::advance (sIter, nOffset);

		sIterRslt = pClMSet->erase (sIter);

		nRslt = ::std::distance (pClMSet->cbegin (), sIterRslt);

		if (nOffset != nRslt)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeMultiSetSTLifEraseViaIterator: returned iterator mismatches!" << ::std::endl;

			exit (-1);
		}
	}
}

template<class _t_multiset>
void TestBTreeMultiSetSTLifEraseViaKey (_t_multiset *pClMSet, typename _t_multiset::size_type nNumEntries, uint32_t nInstances)
{
	typedef typename _t_multiset::const_iterator				citer_t;
	typedef typename _t_multiset::size_type						size_type;
	typedef typename _t_multiset::key_type						key_type;

	key_type						nLastKey;
	citer_t							sIter;
	uint32_t						i;
	key_type						nKey;
	size_type						nRslt;
	size_type						nOffset;
	
	::std::cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: erase (const keytype &)" << ::std::endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		multiSetPrim_add (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	}

	while (pClMSet->size () > 0)
	{
		nOffset = generate_rand32 () % pClMSet->size ();

		sIter = pClMSet->cbegin ();

		::std::advance (sIter, nOffset);

		nKey = (*sIter);

		nRslt = pClMSet->erase (nKey);

		if (nRslt != nInstances)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeMultiSetSTLifEraseViaKey: unexpected number of items deleted!" << ::std::endl;

			exit (-1);
		}
	}
}

template<class _t_multiset>
void TestBTreeMultiSetSTLifEraseViaIteratorMultiple (_t_multiset *pClMSet, typename _t_multiset::size_type nNumEntries, uint32_t nInstances)
{
	typedef typename _t_multiset::const_iterator	citer_t;
	typedef typename _t_multiset::size_type			size_type;
	typedef typename _t_multiset::key_type			key_type;

	key_type									nLastKey;
	citer_t										sIterBegin;
	citer_t										sIterEnd;
	citer_t										sIterRslt;
	uint32_t									i;
	size_type									nBegin;
	size_type									nRslt;
	
#if defined (_DEBUG)

	uint32_t									nDebug = 0;

#endif

	::std::cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: erase (const_iterator, const_iterator)" << ::std::endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		multiSetPrim_add (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	}

	while (pClMSet->size () > 0)
	{
		sIterBegin = pClMSet->cbegin ();
		sIterEnd = pClMSet->cend ();

		nBegin = 0;

		if (pClMSet->size () > 1)
		{
			nBegin = generate_rand32 () % (pClMSet->size () / 2);

			::std::advance (sIterBegin, nBegin);
			::std::advance (sIterEnd, (int) (0 - (generate_rand32 () % (pClMSet->size () / 2))));
		}

		sIterRslt = pClMSet->erase (sIterBegin, sIterEnd);

		nRslt = ::std::distance (pClMSet->cbegin (), sIterRslt);

		if (nRslt != nBegin)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeMultiSetSTLifEraseViaIteratorMultiple: returned iterator mismatches!" << ::std::endl;
			::std::cerr << "expected value: " << nBegin << ::std::endl;
			::std::cerr << "returned value: " << nRslt << ::std::endl;

#if defined (_DEBUG)

			::std::cerr << "iteration: " << nDebug << ::std::endl;

#endif

			exit (-1);
		}

#if defined (_DEBUG)

		nDebug++;

#endif
	}
}

template<class _t_multiset>
void TestBTreeMultiSetSTLifKeyComp (_t_multiset *pClMSet, typename _t_multiset::size_type nNumEntries)
{
	typedef typename _t_multiset::iterator		iter_t;
	typedef typename _t_multiset::key_type		key_type;

	key_type									nLastKey = 0;
	iter_t										sIterBegin;
	iter_t										sIterEnd;
	iter_t										sIter;
	iter_t										sIterUpper;
	key_type									nKey;
	key_type									nNextKey;
	typename _t_multiset::key_compare			sMMComp = pClMSet->key_comp ();
	
	::std::cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: key_comp ()" << ::std::endl;

	multiSetPrim_add (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClMSet->begin ();
	sIterEnd = pClMSet->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = ((key_type)(*sIter));

		sIterUpper = pClMSet->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			nNextKey = ((key_type)(*sIterUpper));

			if (!sMMComp (nKey, nNextKey))
			{
				::std::cerr << ::std::endl;
				::std::cerr << "TestBTreeMultiSetSTLifKeyComp: ERROR: CBTreeMultiSet::key_compare (const keytype &, const keytype &) delivered unexpected result!" << ::std::endl;

				exit (-1);
			}
		}
	}
}

template<class _t_multiset>
void TestBTreeMultiSetSTLifValueComp (_t_multiset *pClMSet, typename _t_multiset::size_type nNumEntries)
{
	typedef typename _t_multiset::iterator		iter_t;
	typedef typename _t_multiset::key_type		key_type;

	key_type									nLastKey = 0;
	iter_t										sIterBegin;
	iter_t										sIterEnd;
	iter_t										sIter;
	iter_t										sIterUpper;
	key_type									nKey;
	typename _t_multiset::value_compare			sMMComp = pClMSet->value_comp ();
	
	::std::cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: value_comp ()" << ::std::endl;

	multiSetPrim_add (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClMSet->begin ();
	sIterEnd = pClMSet->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = ((key_type)(*sIter));

		sIterUpper = pClMSet->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			if (!sMMComp (((key_type)(*sIter)), ((key_type)(*sIterUpper))))
			{
				::std::cerr << ::std::endl;
				::std::cerr << "TestBTreeMultiSetSTLifValueComp: ERROR: CBTreeMultiSet::value_compare (const valuetype &, const valuetype &) delivered unexpected result!" << ::std::endl;

				exit (-1);
			}
		}
	}
}

template<class _t_multiset>
void TestBTreeMultiSetSTLifSwap (_t_multiset *pClMSet, typename _t_multiset::size_type nNumEntries)
{
	typedef typename _t_multiset::const_iterator			citer_t;
	typedef typename _t_multiset::key_type					key_type;

	key_type						nLastKey = 0;
	_t_multiset						sClMultiSetTarget (*pClMSet);
	_t_multiset						sClMultiSetRef (*pClMSet);
	_t_multiset						sClMultiSetTargetRef (*pClMSet);

	::std::cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: swap (CBTreeMultiSet &)" << ::std::endl;

	multiSetPrim_add (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sClMultiSetRef.insert (pClMSet->cbegin (), pClMSet->cend ());

	if ((*pClMSet) != sClMultiSetRef)
	{
		::std::cerr << "ERROR: Unexpected multiset mismatch!" << ::std::endl;

		exit (-1);
	}

	multiSetPrim_add (&sClMultiSetTarget, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sClMultiSetTargetRef.insert (sClMultiSetTarget.cbegin (), sClMultiSetTarget.cend ());

	if (sClMultiSetTarget != sClMultiSetTargetRef)
	{
		::std::cerr << "ERROR: Unexpected target mismatch!" << ::std::endl;

		exit (-1);
	}

	pClMSet->swap (sClMultiSetTarget);

	if ((*pClMSet) != sClMultiSetTargetRef)
	{
		::std::cerr << "ERROR: swapped multiset mismatches target reference!" << ::std::endl;

		exit (-1);
	}

	if (sClMultiSetTarget != sClMultiSetRef)
	{
		::std::cerr << "ERROR: target mismatches multiset reference!" << ::std::endl;

		exit (-1);
	}

	sClMultiSetTarget.swap (*pClMSet);

	if ((*pClMSet) != sClMultiSetRef)
	{
		::std::cerr << "ERROR: multiset mismatches multiset reference!" << ::std::endl;

		exit (-1);
	}

	if (sClMultiSetTarget != sClMultiSetTargetRef)
	{
		::std::cerr << "ERROR: target mismatches target reference!" << ::std::endl;

		exit (-1);
	}
}

template<class _t_multiset>
void TestBTreeMultiSetSTLifFind (_t_multiset *pClMSet, typename _t_multiset::size_type nNumEntries)
{
	typedef typename _t_multiset::const_iterator			citer_t;
	typedef typename _t_multiset::key_type					key_type;

	key_type						nLastKey = 1;
	key_type						nKey;
	citer_t							sCIter;
	citer_t							sCIterRslt;
	
	::std::cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: find (const _t_keytype &)" << ::std::endl;

	multiSetPrim_add (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_ASCEND_KEY);

	for (sCIter = pClMSet->cbegin (); sCIter != pClMSet->cend (); sCIter++)
	{
		nKey = *sCIter;

		sCIterRslt = pClMSet->find (nKey);

		if (sCIter != sCIterRslt)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeMultiSetSTLifFind: ERROR: iterator mismatch!" << ::std::endl;

			exit (-1);
		}
	}

	nKey = 0;

	sCIterRslt = pClMSet->find (nKey);

	if (sCIterRslt != pClMSet->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeMultiSetSTLifFind: ERROR: find () didn't return cend () on lower bound check!" << ::std::endl;

		exit (-1);
	}

	nKey = (key_type) (nNumEntries + 1);

	sCIterRslt = pClMSet->find (nKey);

	if (sCIterRslt != pClMSet->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeMultiSetSTLifFind: ERROR: find () didn't return cend () on upper bound check!" << ::std::endl;

		exit (-1);
	}
}

template<class _t_multiset>
void TestBTreeMultiSetSTLifLowerBoundUpperBound (_t_multiset *pClMSet, typename _t_multiset::size_type nNumEntries, uint32_t nInstance)
{
	typedef typename _t_multiset::const_iterator			citer_t;
	typedef typename _t_multiset::key_type					key_type;

	key_type						nLastKey;
	uint32_t						i;
	key_type						nKey;
	citer_t							sCIterLower;
	citer_t							sCIterUpper;
	
	::std::cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: lower_bound () upper_bound () count ()" << ::std::endl;
	
	for (i = 0; i < nInstance; i++)
	{
		nLastKey = 1;

		multiSetPrim_add (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_ASCEND_KEY);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		nKey = i + 1;

		sCIterLower = pClMSet->lower_bound (nKey);
		sCIterUpper = pClMSet->upper_bound (nKey);

		::std::advance (sCIterLower, (int) nInstance);

		if (sCIterLower != sCIterUpper)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeMultiSetSTLifLowerBoundUpperBound: ERROR: boundary mismatch!" << ::std::endl;

			exit (-1);
		}

		if (pClMSet->count (nKey) != nInstance)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeMultiSetSTLifLowerBoundUpperBound: ERROR: count () returned with unexpected value!" << ::std::endl;

			exit (-1);
		}
	}

	nKey = 0;

	sCIterLower = pClMSet->lower_bound (nKey);

	if (sCIterLower != pClMSet->cbegin ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeMultiSetSTLifLowerBoundUpperBound: ERROR: lower_bound () didn't return cbegin () when testing for lowest key!" << ::std::endl;

		exit (-1);
	}

	nKey = (key_type) (nNumEntries + 1);

	sCIterUpper = pClMSet->upper_bound (nKey);

	if (sCIterUpper != pClMSet->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeMultiSetSTLifLowerBoundUpperBound: ERROR: upper_bound () didn't return cend () when testing for lowest key!" << ::std::endl;

		exit (-1);
	}
}

template<class _t_container>
void TestBTreeSTLmultiSet (uint32_t nTestNum, uint32_t nNodeSize, uint32_t nPageSize, _t_container *pContainerpMSetWrapper)
{
	typename _t_container::size_test_type		sTypeSelect;
	::std::string								sTypeStr;

	get_typename (sTypeSelect, sTypeStr);

	::std::cout << "b-tree multi-set test bench selected using type " << sTypeStr << ::std::endl;

	pContainerpMSetWrapper = new _t_container (nNodeSize, nPageSize);

	if (pContainerpMSetWrapper == NULL)
	{
		::std::cerr << "insufficient memory!" << ::std::endl;

		exit (-1);
	}

	switch (nTestNum)
	{
	case BTREETEST_MULTISET_ASSIGNMENT_OPERATOR	:
		{
			TestBTreeMultiSetSTLifAssignmentOperator (pContainerpMSetWrapper, 64);

			break;
		}
		
	case BTREETEST_MULTISET_STL_IF_INSERT	:
		{
			TestBTreeMultiSetSTLifInsert (pContainerpMSetWrapper, 64);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_INSERT_VIA_ITERATOR	:
		{
			TestBTreeSetSTLifInsertViaIterator (pContainerpMSetWrapper, 64);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_ERASE_VIA_ITERATOR	:
		{
			TestBTreeMultiSetSTLifEraseViaIterator (pContainerpMSetWrapper, 64);

			break;
		}
		
	case BTREETEST_MULTISET_STL_IF_ERASE_VIA_KEY	:
		{
			TestBTreeMultiSetSTLifEraseViaKey (pContainerpMSetWrapper, 64, 5);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE	:
		{
			TestBTreeMultiSetSTLifEraseViaIteratorMultiple (pContainerpMSetWrapper, 64, 5);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_KEY_COMP	:
		{
			TestBTreeMultiSetSTLifKeyComp (pContainerpMSetWrapper, 64);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_VALUE_COMP	:
		{
			TestBTreeMultiSetSTLifValueComp (pContainerpMSetWrapper, 64);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_SWAP	:
		{
			TestBTreeMultiSetSTLifSwap (pContainerpMSetWrapper, 128);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_FIND	:
		{
			TestBTreeMultiSetSTLifFind (pContainerpMSetWrapper, 64);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_LOWER_BOUND_UPPER_BOUND	:
		{
			TestBTreeMultiSetSTLifLowerBoundUpperBound (pContainerpMSetWrapper, 64, 5);

			break;
		}
	
	default:
		{
			::std::cerr << "ERROR: TestBTreeSTLmultiSet: Unknown test or test not specified!" << ::std::endl;

			exit (-1);

			break;
		}
	}

	delete pContainerpMSetWrapper;
}
