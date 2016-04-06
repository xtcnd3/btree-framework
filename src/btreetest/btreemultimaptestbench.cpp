/************************************************************
**
** file:	btreemultimaptestbench.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for the multi map application class.
**
************************************************************/

#include "btreemultimaptestbench.h"

template <class _t_multimap>
void TestBTreeMultiMapSTLifAssignmentOperator (_t_multimap *pClMM, uint32_t nNumEntries)
{
	typedef typename _t_multimap::const_iterator						citer_t;

	_t_multimap									sClMM (*pClMM);
	uint32_t									nLastKey = 0;
	citer_t										sIter;
	citer_t										sIterRslt;
	
	cout << "exercises assignment operator compatible to STL interface CBTreeMultiMap<>:: operator= (CBTreeMultiMap<> &)" << endl;

	multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMM = *pClMM;

	if (sClMM != (*pClMM))
	{
		cerr << endl;
		cerr << "TestBTreeMultiMapSTLifAssignmentOperator<>: ERROR: mismatch after assignment operation" << endl;

		exit (-1);
	}
}

template <class _t_multimap>
void TestBTreeMultiMapSTLifInsert (_t_multimap *pClMM, uint32_t nNumEntries)
{
	uint32_t	nLastSeed = 0;

	cout << "CBTreeMultiMap::insert (const valuetype &) is exercised" << endl;

	multiMapPrim_add (pClMM, nNumEntries, nLastSeed, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
}

template <class _t_multimap>
void TestBTreeMultiMapSTLifEraseViaIterator (_t_multimap *pClMM, uint32_t nNumEntries)
{
	typedef typename _t_multimap::const_iterator	citer_t;
	typedef typename _t_multimap::size_type			size_type;

	uint32_t									nLastKey = 0;
	citer_t										sIter;
	citer_t										sIterRslt;
	size_type									nOffset;
	size_type									nRslt;
	
	cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: erase (const_iterator)" << endl;

	multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	while (pClMM->size () > 0)
	{
		nOffset = generate_rand32 () % pClMM->size ();

		sIter = pClMM->cbegin ();

		::std::advance (sIter, nOffset);

		sIterRslt = pClMM->erase (sIter);

		nRslt = ::std::distance (pClMM->cbegin (), sIterRslt);

		if (nOffset != nRslt)
		{
			cerr << endl;
			cerr << "TestBTreeMultiMapSTLifEraseViaIterator: returned iterator mismatches!" << endl;

			exit (-1);
		}
	}
}

template <class _t_multimap>
void TestBTreeMultiMapSTLifEraseViaKey (_t_multimap *pClMM, uint32_t nNumEntries, uint32_t nInstances)
{
	typedef typename _t_multimap::const_iterator										citer_t;
	
	uint32_t									nLastKey;
	uint32_t									nKey;
	citer_t										sIter;
	uint32_t									i;
	typename _t_multimap::size_type				nRslt;
	typename _t_multimap::size_type				nOffset;
	
	cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: erase (const keytype &)" << endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	}

	while (pClMM->size () > 0)
	{
		sIter = pClMM->cbegin ();

		nOffset = generate_rand32 () % pClMM->size ();

		::std::advance (sIter, nOffset);

		nKey = (*sIter).first;

		nRslt = pClMM->erase (nKey);

		if (nRslt != nInstances)
		{
			cerr << endl;
			cerr << "TestBTreeMultiMapSTLifEraseViaKey: unexpected number of items deleted!" << endl;

			exit (-1);
		}
	}
}

template <class _t_multimap>
void TestBTreeMultiMapSTLifEraseViaIteratorMultiple (_t_multimap *pClMM, uint32_t nNumEntries, uint32_t nInstances)
{
	typedef typename _t_multimap::const_iterator	citer_t;
	typedef typename _t_multimap::size_type			size_type;

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
	
	cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: erase (const_iterator, const_iterator)" << endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	}

	while (pClMM->size () > 0)
	{
		sIterBegin = pClMM->cbegin ();
		sIterEnd = pClMM->cend ();

		nBegin = 0;

		if (pClMM->size () > 1)
		{
			nBegin = generate_rand32 () % (pClMM->size () / 2);

			::std::advance (sIterBegin, nBegin);
			::std::advance (sIterEnd, (int) (0 - (generate_rand32 () % (pClMM->size () / 2))));
		}

		sIterRslt = pClMM->erase (sIterBegin, sIterEnd);

		nRslt = ::std::distance (pClMM->cbegin (), sIterRslt);

		if (nRslt != nBegin)
		{
			cerr << endl;
			cerr << "TestBTreeMultiMapSTLifEraseViaIteratorMultiple: returned iterator mismatches!" << endl;
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

template<class _t_multimap>
void TestBTreeMultiMapSTLifKeyComp (_t_multimap *pClMM, uint32_t nNumEntries)
{
	typedef typename _t_multimap::iterator		iter_t;

	uint32_t									nLastKey = 0;
	iter_t										sIterBegin;
	iter_t										sIterEnd;
	iter_t										sIter;
	iter_t										sIterUpper;
	uint32_t									nKey;
	uint32_t									nNextKey;
	typename _t_multimap::key_compare			sMMComp = pClMM->key_comp ();
	
	cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: key_comp ()" << endl;

	multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClMM->begin ();
	sIterEnd = pClMM->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = ((typename _t_multimap::value_type)(*sIter)).first;

		sIterUpper = pClMM->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			nNextKey = ((typename _t_multimap::value_type)(*sIterUpper)).first;

			if (!sMMComp (nKey, nNextKey))
			{
				cerr << endl;
				cerr << "TestBTreeMultiMapSTLifKeyComp: ERROR: CBTreeMultiMap::key_compare (const keytype &, const keytype &) delivered unexpected result!" << endl;

				exit (-1);
			}
		}
	}
}

template<class _t_multimap>
void TestBTreeMultiMapSTLifValueComp (_t_multimap *pClMM, uint32_t nNumEntries)
{
	typedef typename _t_multimap::iterator		iter_t;

	uint32_t									nLastKey = 0;
	iter_t										sIterBegin;
	iter_t										sIterEnd;
	iter_t										sIter;
	iter_t										sIterUpper;
	uint32_t									nKey;
	typename _t_multimap::value_compare			sMMComp = pClMM->value_comp ();
	
	cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: value_comp ()" << endl;

	multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClMM->begin ();
	sIterEnd = pClMM->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = ((typename _t_multimap::value_type)(*sIter)).first;

		sIterUpper = pClMM->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			if (!sMMComp (((typename _t_multimap::value_type)(*sIter)), ((typename _t_multimap::value_type)(*sIterUpper))))
			{
				cerr << endl;
				cerr << "TestBTreeMultiMapSTLifValueComp: ERROR: CBTreeMultiMap::value_compare (const valuetype &, const valuetype &) delivered unexpected result!" << endl;

				exit (-1);
			}
		}
	}
}

template <class _t_multimap>
void TestBTreeMultiMapSTLifSwap (_t_multimap *pClMM, uint32_t nNumEntries)
{
	typedef typename _t_multimap::const_iterator			citer_t;

	uint32_t						nLastKey = 0;
	_t_multimap						sClMultiMapTarget ((const _t_multimap &) *pClMM);
	_t_multimap						sClMultiMapRef ((const _t_multimap &) *pClMM);
	_t_multimap						sClMultiMapTargetRef ((const _t_multimap &) *pClMM);

	cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: swap (CBTreeMultiMap &)" << endl;

	multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMultiMapRef.template insert<citer_t> (pClMM->cbegin (), pClMM->cend ());

	if ((*pClMM) != sClMultiMapRef)
	{
		cerr << "ERROR: Unexpected multimap mismatch!" << endl;

		exit (-1);
	}

	multiMapPrim_add (&sClMultiMapTarget, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMultiMapTargetRef.template insert<citer_t> (sClMultiMapTarget.cbegin (), sClMultiMapTarget.cend ());

	if (sClMultiMapTarget != sClMultiMapTargetRef)
	{
		cerr << "ERROR: Unexpected target mismatch!" << endl;

		exit (-1);
	}

	pClMM->swap (sClMultiMapTarget);

	if ((*pClMM) != sClMultiMapTargetRef)
	{
		cerr << "ERROR: swapped multimap mismatches target reference!" << endl;

		exit (-1);
	}

	if (sClMultiMapTarget != sClMultiMapRef)
	{
		cerr << "ERROR: target mismatches multimap reference!" << endl;

		exit (-1);
	}

	sClMultiMapTarget.swap (*pClMM);

	if ((*pClMM) != sClMultiMapRef)
	{
		cerr << "ERROR: multimap mismatches multimap reference!" << endl;

		exit (-1);
	}

	if (sClMultiMapTarget != sClMultiMapTargetRef)
	{
		cerr << "ERROR: target mismatches target reference!" << endl;

		exit (-1);
	}
}

template <class _t_multimap>
void TestBTreeMultiMapSTLifFind (_t_multimap *pClMM, uint32_t nNumEntries)
{
	typedef typename _t_multimap::const_iterator			citer_t;

	uint32_t						nLastKey = 1;
	citer_t							sCIter;
	citer_t							sCIterRslt;
	
	cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: find (const _t_keytype &)" << endl;

	multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_ASCEND_KEY);

	for (sCIter = pClMM->cbegin (); sCIter != pClMM->cend (); sCIter++)
	{
		sCIterRslt = pClMM->find ((*sCIter).first);

		if (sCIter != sCIterRslt)
		{
			cerr << endl;
			cerr << "TestBTreeMultiMapSTLifFind: ERROR: iterator mismatch!" << endl;

			exit (-1);
		}
	}

	sCIterRslt = pClMM->find (0);

	if (sCIterRslt != pClMM->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeMultiMapSTLifFind: ERROR: find () didn't return cend () on lower bound check!" << endl;

		exit (-1);
	}

	sCIterRslt = pClMM->find (nNumEntries + 1);

	if (sCIterRslt != pClMM->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeMultiMapSTLifFind: ERROR: find () didn't return cend () on upper bound check!" << endl;

		exit (-1);
	}
}

template <class _t_multimap>
void TestBTreeKeySortSTLifLowerBoundUpperBound (_t_multimap *pClMM, uint32_t nNumEntries, uint32_t nInstance)
{
	typedef typename _t_multimap::const_iterator			citer_t;

	uint32_t						nLastKey;
	uint32_t						i;
	citer_t							sCIterLower;
	citer_t							sCIterUpper;
	
	cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: lower_bound () upper_bound () count ()" << endl;
	
	for (i = 0; i < nInstance; i++)
	{
		nLastKey = 1;

		multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_ASCEND_KEY);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sCIterLower = pClMM->lower_bound (i + 1);
		sCIterUpper = pClMM->upper_bound (i + 1);

		::std::advance (sCIterLower, (int) nInstance);

		if (sCIterLower != sCIterUpper)
		{
			cerr << endl;
			cerr << "TestBTreeMultiMapSTLifLowerBoundUpperBound: ERROR: boundary mismatch!" << endl;

			exit (-1);
		}

		if (pClMM->count (i + 1) != nInstance)
		{
			cerr << endl;
			cerr << "TestBTreeMultiMapSTLifLowerBoundUpperBound: ERROR: count () returned with unexpected value!" << endl;

			exit (-1);
		}
	}

	sCIterLower = pClMM->lower_bound (0);

	if (sCIterLower != pClMM->cbegin ())
	{
		cerr << endl;
		cerr << "TestBTreeMultiMapSTLifLowerBoundUpperBound: ERROR: lower_bound () didn't return cbegin () when testing for lowest key!" << endl;

		exit (-1);
	}

	sCIterUpper = pClMM->upper_bound (nNumEntries + 1);

	if (sCIterUpper != pClMM->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeMultiMapSTLifLowerBoundUpperBound: ERROR: upper_bound () didn't return cend () when testing for lowest key!" << endl;

		exit (-1);
	}
}

template <class _t_container>
void TestBTreeSTLmultiMap (uint32_t nTestNum, _t_container *pMMapWrapper)
{
	cout << "b-tree multi-map test bench selected" << endl;

	switch (nTestNum)
	{
	case BTREETEST_MULTIMAP_ASSIGNMENT_OPERATOR	:
		{
			TestBTreeMultiMapSTLifAssignmentOperator (pMMapWrapper, 64);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_INSERT	:
		{
			TestBTreeMultiMapSTLifInsert (pMMapWrapper, 64);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_INSERT_VIA_ITERATOR	:
		{
			TestBTreeMapSTLifInsertViaIterator (pMMapWrapper, 64);
			
			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_ERASE_VIA_ITERATOR	:
		{
			TestBTreeMultiMapSTLifEraseViaIterator (pMMapWrapper, 64);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_ERASE_VIA_KEY	:
		{
			TestBTreeMultiMapSTLifEraseViaKey (pMMapWrapper, 64, 5);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE	:
		{
			TestBTreeMultiMapSTLifEraseViaIteratorMultiple (pMMapWrapper, 64, 5);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_KEY_COMP	:
		{
			TestBTreeMultiMapSTLifKeyComp (pMMapWrapper, 64);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_VALUE_COMP	:
		{
			TestBTreeMultiMapSTLifValueComp (pMMapWrapper, 64);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_SWAP	:
		{
			TestBTreeMultiMapSTLifSwap (pMMapWrapper, 128);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_FIND	:
		{
			TestBTreeMultiMapSTLifFind (pMMapWrapper, 64);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_LOWER_BOUND_UPPER_BOUND	:
		{
			TestBTreeKeySortSTLifLowerBoundUpperBound (pMMapWrapper, 64, 5);

			break;
		}
	
	default:
		{
			cerr << "ERROR: TestBTreeSTLmultiMap: Unknown test or test not specified!" << endl;

			break;
		}
	}
}
