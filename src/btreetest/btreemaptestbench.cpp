/************************************************************
**
** file:	btreemaptestbench.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for the map application class.
**
************************************************************/

#include "btreemaptestbench.h"

template <class _t_map>
void TestBTreeMapSTLifAssignmentOperator (_t_map *pClM, uint32_t nNumEntries)
{
	typedef typename _t_map::const_iterator						citer_t;

	_t_map										sClMM (*pClM);
	uint32_t									nLastKey = 0;
	citer_t										sIter;
	citer_t										sIterRslt;
	
	cout << "exercises assignment operator compatible to STL interface CBTreeMap<>:: operator= (CBTreeMap<> &)" << endl;

	multiMapPrim_add (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMM = *pClM;

	if (sClMM != (*pClM))
	{
		cerr << endl;
		cerr << "TestBTreeMapSTLifAssignmentOperator<>: ERROR: mismatch after assignment operation" << endl;

		exit (-1);
	}
}

template <class _t_map>
void TestBTreeMapSTLifInsert (_t_map *pClM, uint32_t nNumEntries)
{
	uint32_t	nLastSeed = 0;

	cout << "CBTreeMap::insert (const valuetype &) is exercised" << endl;

	multiMapPrim_add (pClM, nNumEntries, nLastSeed, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
}

template <class _t_map>
void TestBTreeMapSTLifEraseViaIterator (_t_map *pClM, uint32_t nNumEntries)
{
	typedef typename _t_map::const_iterator		citer_t;
	typedef typename _t_map::size_type			size_type;

	uint32_t									nLastKey = 0;
	citer_t										sIter;
	citer_t										sIterRslt;
	size_type									nOffset;
	size_type									nRslt;
	
	cout << "exercises method compatible to STL interface CBTreeMap<>:: erase (const_iterator)" << endl;

	multiMapPrim_add (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	while (pClM->size () > 0)
	{
		nOffset = generate_rand32 () % pClM->size ();

		sIter = pClM->cbegin ();

		::std::advance (sIter, nOffset);

		sIterRslt = pClM->erase (sIter);

		nRslt = ::std::distance (pClM->cbegin (), sIterRslt);

		if (nOffset != nRslt)
		{
			cerr << endl;
			cerr << "TestBTreeMapSTLifEraseViaIterator: returned iterator mismatches!" << endl;

			exit (-1);
		}
	}
}

template <class _t_map>
void TestBTreeMapSTLifEraseViaKey (_t_map *pClM, uint32_t nNumEntries, uint32_t nInstances)
{
	typedef typename _t_map::const_iterator		citer_t;
	
	uint32_t									nLastKey;
	uint32_t									nKey;
	citer_t										sIter;
	uint32_t									i;
	typename _t_map::size_type					nRslt;
	typename _t_map::size_type					nOffset;
	
	cout << "exercises method compatible to STL interface CBTreeMap<>:: erase (const keytype &)" << endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		multiMapPrim_add (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	}

	while (pClM->size () > 0)
	{
		sIter = pClM->cbegin ();

		nOffset = generate_rand32 () % pClM->size ();

		::std::advance (sIter, nOffset);

		nKey = (*sIter).first;

		nRslt = pClM->erase (nKey);

		if (nRslt != 1)
		{
			cerr << endl;
			cerr << "TestBTreeMapSTLifEraseViaKey: unexpected number of items deleted!" << endl;

			exit (-1);
		}
	}
}

template <class _t_map>
void TestBTreeMapSTLifEraseViaIteratorMultiple (_t_map *pClM, uint32_t nNumEntries, uint32_t nInstances)
{
	typedef typename _t_map::const_iterator		citer_t;
	typedef typename _t_map::size_type			size_type;

	uint32_t									nLastKey;
	citer_t										sIterBegin;
	citer_t										sIterEnd;
	citer_t										sIterRslt;
	uint32_t									i;
	size_type									nBegin;
	size_type									nRslt;

#if defined (_DEBUG)

	uint32_t									nDebug = 0;

#endif
	
	cout << "exercises method compatible to STL interface CBTreeMap<>:: erase (const_iterator, const_iterator)" << endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		multiMapPrim_add (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	}

	while (pClM->size () > 0)
	{
		sIterBegin = pClM->cbegin ();
		sIterEnd = pClM->cend ();

		nBegin = 0;

		if (pClM->size () > 1)
		{
			nBegin = generate_rand32 () % (pClM->size () / 2);

			::std::advance (sIterBegin, nBegin);
			::std::advance (sIterEnd, (int) (0 - (generate_rand32 () % (pClM->size () / 2))));
		}

		sIterRslt = pClM->erase (sIterBegin, sIterEnd);

		nRslt = ::std::distance (pClM->cbegin (), sIterRslt);

		if (nRslt != nBegin)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeMapSTLifEraseViaIteratorMultiple: returned iterator mismatches!" << ::std::endl;
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

template<class _t_map>
void TestBTreeMapSTLifKeyComp (_t_map *pClM, uint32_t nNumEntries)
{
	typedef typename _t_map::iterator		iter_t;

	uint32_t									nLastKey = 0;
	iter_t										sIterBegin;
	iter_t										sIterEnd;
	iter_t										sIter;
	iter_t										sIterUpper;
	uint32_t									nKey;
	uint32_t									nNextKey;
	typename _t_map::key_compare			sMComp = pClM->key_comp ();
	
	cout << "exercises method compatible to STL interface CBTreeMap<>:: key_comp ()" << endl;

	multiMapPrim_add (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClM->begin ();
	sIterEnd = pClM->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = get_entry_key (*sIter);

		sIterUpper = pClM->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			nNextKey = get_entry_key (*sIterUpper);

			if (!sMComp (nKey, nNextKey))
			{
				cerr << endl;
				cerr << "TestBTreeMapSTLifKeyComp: ERROR: CBTreeMap::key_compare (const keytype &, const keytype &) delivered unexpected result!" << endl;

				exit (-1);
			}
		}
	}
}

template<class _t_map>
void TestBTreeMapSTLifValueComp (_t_map *pClM, uint32_t nNumEntries)
{
	typedef typename _t_map::iterator			iter_t;

	uint32_t									nLastKey = 0;
	iter_t										sIterBegin;
	iter_t										sIterEnd;
	iter_t										sIter;
	iter_t										sIterUpper;
	uint32_t									nKey;
	typename _t_map::value_compare				sMComp = pClM->value_comp ();
	
	cout << "exercises method compatible to STL interface CBTreeMap<>:: value_comp ()" << endl;

	multiMapPrim_add (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClM->begin ();
	sIterEnd = pClM->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = get_entry_key (*sIter);

		sIterUpper = pClM->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			if (!sMComp (((typename _t_map::value_type)(*sIter)), ((typename _t_map::value_type)(*sIterUpper))))
			{
				cerr << endl;
				cerr << "TestBTreeMapSTLifValueComp: ERROR: CBTreeMap::value_compare (const valuetype &, const valuetype &) delivered unexpected result!" << endl;

				exit (-1);
			}
		}
	}
}

template <class _t_map>
void TestBTreeMapSTLifSwap (_t_map *pClM, uint32_t nNumEntries)
{
	typedef typename _t_map::const_iterator			citer_t;

	uint32_t					nLastKey = 0;
	_t_map						sClMapTarget (*pClM);
	_t_map						sClMapRef (*pClM);
	_t_map						sClMapTargetRef (*pClM);
	citer_t						sCIterBegin;
	citer_t						sCIterEnd;

	cout << "exercises method compatible to STL interface CBTreeMap<>:: swap (CBTreeMap &)" << endl;

	multiMapPrim_add (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMapRef.insert (pClM->cbegin (), pClM->cend ());

	if ((*pClM) != sClMapRef)
	{
		cerr << "ERROR: Unexpected multimap mismatch!" << endl;

		exit (-1);
	}

	multiMapPrim_add (&sClMapTarget, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sCIterBegin = sClMapTarget.cbegin ();
	sCIterEnd = sClMapTarget.cend ();

	sClMapTargetRef.insert (sCIterBegin, sCIterEnd);

	if (sClMapTarget != sClMapTargetRef)
	{
		cerr << "ERROR: Unexpected target mismatch!" << endl;

		exit (-1);
	}

	pClM->swap (sClMapTarget);

	if ((*pClM) != sClMapTargetRef)
	{
		cerr << "ERROR: swapped multimap mismatches target reference!" << endl;

		exit (-1);
	}

	if (sClMapTarget != sClMapRef)
	{
		cerr << "ERROR: target mismatches multimap reference!" << endl;

		exit (-1);
	}

	sClMapTarget.swap (*pClM);

	if ((*pClM) != sClMapRef)
	{
		cerr << "ERROR: multimap mismatches multimap reference!" << endl;

		exit (-1);
	}

	if (sClMapTarget != sClMapTargetRef)
	{
		cerr << "ERROR: target mismatches target reference!" << endl;

		exit (-1);
	}
}

template <class _t_map>
void TestBTreeMapSTLifFind (_t_map *pClM, uint32_t nNumEntries)
{
	typedef typename _t_map::const_iterator			citer_t;

	uint32_t						nLastKey = 1;
	citer_t							sCIter;
	citer_t							sCIterRslt;
	
	cout << "exercises method compatible to STL interface CBTreeMap<>:: find (const _t_keytype &)" << endl;

	multiMapPrim_add (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_ASCEND_KEY);

	for (sCIter = pClM->cbegin (); sCIter != pClM->cend (); sCIter++)
	{
		sCIterRslt = pClM->find ((*sCIter).first);

		if (sCIter != sCIterRslt)
		{
			cerr << endl;
			cerr << "TestBTreeMapSTLifFind: ERROR: iterator mismatch!" << endl;

			exit (-1);
		}
	}

	sCIterRslt = pClM->find (0);

	if (sCIterRslt != pClM->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeMapSTLifFind: ERROR: find () didn't return cend () on lower bound check!" << endl;

		exit (-1);
	}

	sCIterRslt = pClM->find (nNumEntries + 1);

	if (sCIterRslt != pClM->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeMapSTLifFind: ERROR: find () didn't return cend () on upper bound check!" << endl;

		exit (-1);
	}
}

template <class _t_map>
void TestBTreeMapSTLifLowerBoundUpperBound (_t_map *pClM, uint32_t nNumEntries, uint32_t nInstance)
{
	typedef typename _t_map::const_iterator			citer_t;

	uint32_t						nLastKey;
	uint32_t						i;
	citer_t							sCIterLower;
	citer_t							sCIterUpper;
	
	cout << "exercises method compatible to STL interface CBTreeMap<>:: lower_bound () upper_bound () count ()" << endl;
	
	for (i = 0; i < nInstance; i++)
	{
		nLastKey = 1;

		multiMapPrim_add (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_ASCEND_KEY);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sCIterLower = pClM->lower_bound (i + 1);
		sCIterUpper = pClM->upper_bound (i + 1);

		::std::advance (sCIterLower, (int) 1);

		if (sCIterLower != sCIterUpper)
		{
			cerr << endl;
			cerr << "TestBTreeMapSTLifLowerBoundUpperBound: ERROR: boundary mismatch!" << endl;

			exit (-1);
		}

		if (pClM->count (i + 1) != 1)
		{
			cerr << endl;
			cerr << "TestBTreeMapSTLifLowerBoundUpperBound: ERROR: count () returned with unexpected value!" << endl;

			exit (-1);
		}
	}

	sCIterLower = pClM->lower_bound (0);

	if (sCIterLower != pClM->cbegin ())
	{
		cerr << endl;
		cerr << "TestBTreeMapSTLifLowerBoundUpperBound: ERROR: lower_bound () didn't return cbegin () when testing for lowest key!" << endl;

		exit (-1);
	}

	sCIterUpper = pClM->upper_bound (nNumEntries + 1);

	if (sCIterUpper != pClM->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeMapSTLifLowerBoundUpperBound: ERROR: upper_bound () didn't return cend () when testing for lowest key!" << endl;

		exit (-1);
	}
}

template <class _t_container>
void TestBTreeSTLmap (uint32_t nTestNum, _t_container *pMapWrapper)
{
	cout << "b-tree multi-map test bench selected" << endl;

	switch (nTestNum)
	{
	case BTREETEST_MAP_ASSIGNMENT_OPERATOR	:
		{
			TestBTreeMapSTLifAssignmentOperator (pMapWrapper, 64);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_INSERT	:
		{
			TestBTreeMapSTLifInsert (pMapWrapper, 64);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_INSERT_VIA_ITERATOR	:
		{
			TestBTreeMapSTLifInsertViaIterator (pMapWrapper, 64);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_ERASE_VIA_ITERATOR	:
		{
			TestBTreeMapSTLifEraseViaIterator (pMapWrapper, 64);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_ERASE_VIA_KEY	:
		{
			TestBTreeMapSTLifEraseViaKey (pMapWrapper, 64, 2);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE	:
		{
			TestBTreeMapSTLifEraseViaIteratorMultiple (pMapWrapper, 64, 2);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_KEY_COMP	:
		{
			TestBTreeMapSTLifKeyComp (pMapWrapper, 64);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_VALUE_COMP	:
		{
			TestBTreeMapSTLifValueComp (pMapWrapper, 64);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_SWAP	:
		{
			TestBTreeMapSTLifSwap (pMapWrapper, 128);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_FIND	:
		{
			TestBTreeMapSTLifFind (pMapWrapper, 64);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_LOWER_BOUND_UPPER_BOUND	:
		{
			TestBTreeMapSTLifLowerBoundUpperBound (pMapWrapper, 64, 5);

			break;
		}
	
	default:
		{
			cerr << "ERROR: TestBTreeSTLmap: Unknown test or test not specified!" << endl;

			break;
		}
	}
}
