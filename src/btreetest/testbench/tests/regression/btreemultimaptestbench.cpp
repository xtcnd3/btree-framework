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

template<class _t_multimap>
void TestBTreeMultiMapSTLifAssignmentOperator (_t_multimap *pClMM, typename _t_multimap::size_type nNumEntries)
{
	typedef typename _t_multimap::const_iterator						citer_t;
	typedef typename _t_multimap::key_type								key_type;

	_t_multimap					sClMM (*pClMM);
	key_type					nLastKey = 0;
	citer_t						sIter;
	citer_t						sIterRslt;
	
	::std::cout << "exercises assignment operator compatible to STL interface CBTreeMultiMap<>:: operator= (CBTreeMultiMap<> &)" << ::std::endl;

	multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMM = *pClMM;

	if (sClMM != (*pClMM))
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeMultiMapSTLifAssignmentOperator<>: ERROR: mismatch after assignment operation" << ::std::endl;

		exit (-1);
	}
}

template<class _t_multimap>
void TestBTreeMultiMapSTLifInsert (_t_multimap *pClMM, typename _t_multimap::size_type nNumEntries)
{
	typename _t_multimap::key_type	nLastSeed = 0;

	::std::cout << "CBTreeMultiMap::insert (const valuetype &) is exercised" << ::std::endl;

	multiMapPrim_add (pClMM, nNumEntries, nLastSeed, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
}

template<class _t_multimap>
void TestBTreeMultiMapSTLifEraseViaIterator (_t_multimap *pClMM, typename _t_multimap::size_type nNumEntries)
{
	typedef typename _t_multimap::const_iterator	citer_t;
	typedef typename _t_multimap::size_type			size_type;
	typedef typename _t_multimap::key_type			key_type;

	key_type									nLastKey = 0;
	citer_t										sIter;
	citer_t										sIterRslt;
	size_type									nOffset;
	size_type									nRslt;
	
	::std::cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: erase (const_iterator)" << ::std::endl;

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
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeMultiMapSTLifEraseViaIterator: returned iterator mismatches!" << ::std::endl;

			exit (-1);
		}
	}
}

template<class _t_multimap>
void TestBTreeMultiMapSTLifEraseViaKey (_t_multimap *pClMM, typename _t_multimap::size_type nNumEntries, uint32_t nInstances)
{
	typedef typename _t_multimap::const_iterator	citer_t;
	typedef typename _t_multimap::size_type			size_type;
	typedef typename _t_multimap::key_type			key_type;

	key_type						nLastKey;
	key_type						nKey;
	citer_t							sIter;
	uint32_t						i;
	size_type						nRslt;
	size_type						nOffset;
	
	::std::cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: erase (const keytype &)" << ::std::endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	}

	while (pClMM->size () > 0)
	{
		nOffset = generate_rand32 () % pClMM->size ();

		sIter = pClMM->cbegin ();

		::std::advance (sIter, nOffset);

		nKey = (*sIter).first;

		nRslt = pClMM->erase (nKey);

		if (nRslt != nInstances)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeMultiMapSTLifEraseViaKey: unexpected number of items deleted!" << ::std::endl;

			exit (-1);
		}
	}
}

template<class _t_multimap>
void TestBTreeMultiMapSTLifEraseViaIteratorMultiple (_t_multimap *pClMM, typename _t_multimap::size_type nNumEntries, uint32_t nInstances)
{
	typedef typename _t_multimap::const_iterator	citer_t;
	typedef typename _t_multimap::size_type			size_type;
	typedef typename _t_multimap::key_type			key_type;

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
	
	::std::cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: erase (const_iterator, const_iterator)" << ::std::endl;

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
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeMultiMapSTLifEraseViaIteratorMultiple: returned iterator mismatches!" << ::std::endl;
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
void TestBTreeMultiMapSTLifKeyComp (_t_multimap *pClMM, typename _t_multimap::size_type nNumEntries)
{
	typedef typename _t_multimap::iterator		iter_t;
	typedef typename _t_multimap::key_type		key_type;
	typedef typename _t_multimap::value_type	value_type;

	key_type									nLastKey = 0;
	iter_t										sIterBegin;
	iter_t										sIterEnd;
	iter_t										sIter;
	iter_t										sIterUpper;
	key_type									nKey;
	uint32_t									nNextKey;
	typename _t_multimap::key_compare			sMMComp = pClMM->key_comp ();
	
	::std::cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: key_comp ()" << ::std::endl;

	multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClMM->begin ();
	sIterEnd = pClMM->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = ((value_type)(*sIter)).first;

		sIterUpper = pClMM->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			nNextKey = ((value_type)(*sIterUpper)).first;

			if (!sMMComp (nKey, nNextKey))
			{
				::std::cerr << ::std::endl;
				::std::cerr << "TestBTreeMultiMapSTLifKeyComp: ERROR: CBTreeMultiMap::key_compare (const keytype &, const keytype &) delivered unexpected result!" << ::std::endl;

				exit (-1);
			}
		}
	}
}

template<class _t_multimap>
void TestBTreeMultiMapSTLifValueComp (_t_multimap *pClMM, typename _t_multimap::size_type nNumEntries)
{
	typedef typename _t_multimap::iterator		iter_t;
	typedef typename _t_multimap::key_type		key_type;
	typedef typename _t_multimap::value_type	value_type;

	key_type									nLastKey = 0;
	iter_t										sIterBegin;
	iter_t										sIterEnd;
	iter_t										sIter;
	iter_t										sIterUpper;
	key_type									nKey;
	typename _t_multimap::value_compare			sMMComp = pClMM->value_comp ();
	
	::std::cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: value_comp ()" << ::std::endl;

	multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClMM->begin ();
	sIterEnd = pClMM->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = ((value_type)(*sIter)).first;

		sIterUpper = pClMM->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			if (!sMMComp (((value_type)(*sIter)), ((value_type)(*sIterUpper))))
			{
				::std::cerr << ::std::endl;
				::std::cerr << "TestBTreeMultiMapSTLifValueComp: ERROR: CBTreeMultiMap::value_compare (const valuetype &, const valuetype &) delivered unexpected result!" << ::std::endl;

				exit (-1);
			}
		}
	}
}

template<class _t_multimap>
void TestBTreeMultiMapSTLifSwap (_t_multimap *pClMM, typename _t_multimap::size_type nNumEntries)
{
	typedef typename _t_multimap::const_iterator			citer_t;
	typedef typename _t_multimap::key_type					key_type;
	
	key_type						nLastKey = 0;
	_t_multimap						sClMultiMapTarget ((const _t_multimap &) *pClMM);
	_t_multimap						sClMultiMapRef ((const _t_multimap &) *pClMM);
	_t_multimap						sClMultiMapTargetRef ((const _t_multimap &) *pClMM);

	::std::cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: swap (CBTreeMultiMap &)" << ::std::endl;

	multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMultiMapRef.insert (pClMM->cbegin (), pClMM->cend ());

	if ((*pClMM) != sClMultiMapRef)
	{
		::std::cerr << "ERROR: Unexpected multimap mismatch!" << ::std::endl;

		exit (-1);
	}

	multiMapPrim_add (&sClMultiMapTarget, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMultiMapTargetRef.insert (sClMultiMapTarget.cbegin (), sClMultiMapTarget.cend ());

	if (sClMultiMapTarget != sClMultiMapTargetRef)
	{
		::std::cerr << "ERROR: Unexpected target mismatch!" << ::std::endl;

		exit (-1);
	}

	pClMM->swap (sClMultiMapTarget);

	if ((*pClMM) != sClMultiMapTargetRef)
	{
		::std::cerr << "ERROR: swapped multimap mismatches target reference!" << ::std::endl;

		exit (-1);
	}

	if (sClMultiMapTarget != sClMultiMapRef)
	{
		::std::cerr << "ERROR: target mismatches multimap reference!" << ::std::endl;

		exit (-1);
	}

	sClMultiMapTarget.swap (*pClMM);

	if ((*pClMM) != sClMultiMapRef)
	{
		::std::cerr << "ERROR: multimap mismatches multimap reference!" << ::std::endl;

		exit (-1);
	}

	if (sClMultiMapTarget != sClMultiMapTargetRef)
	{
		::std::cerr << "ERROR: target mismatches target reference!" << ::std::endl;

		exit (-1);
	}
}

template<class _t_multimap>
void TestBTreeMultiMapSTLifFind (_t_multimap *pClMM, typename _t_multimap::size_type nNumEntries)
{
	typedef typename _t_multimap::const_iterator			citer_t;
	typedef typename _t_multimap::key_type					key_type;

	key_type			nLastKey = 1;
	key_type			nKey;
	citer_t				sCIter;
	citer_t				sCIterRslt;
	
	::std::cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: find (const _t_keytype &)" << ::std::endl;

	multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_ASCEND_KEY);

	for (sCIter = pClMM->cbegin (); sCIter != pClMM->cend (); sCIter++)
	{
		nKey = (*sCIter).first;

		sCIterRslt = pClMM->find (nKey);

		if (sCIter != sCIterRslt)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeMultiMapSTLifFind: ERROR: iterator mismatch!" << ::std::endl;

			exit (-1);
		}
	}

	nKey = 0;

	sCIterRslt = pClMM->find (nKey);

	if (sCIterRslt != pClMM->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeMultiMapSTLifFind: ERROR: find () didn't return cend () on lower bound check!" << ::std::endl;

		exit (-1);
	}

	nKey = (key_type) (nNumEntries + 1);

	sCIterRslt = pClMM->find (nKey);

	if (sCIterRslt != pClMM->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeMultiMapSTLifFind: ERROR: find () didn't return cend () on upper bound check!" << ::std::endl;

		exit (-1);
	}
}

template<class _t_multimap>
void TestBTreeMultiMapSTLifLowerBoundUpperBound (_t_multimap *pClMM, typename _t_multimap::size_type nNumEntries, uint32_t nInstance)
{
	typedef typename _t_multimap::const_iterator			citer_t;
	typedef typename _t_multimap::key_type					key_type;

	key_type			nLastKey;
	key_type			nKey;
	uint32_t			i;
	citer_t				sCIterLower;
	citer_t				sCIterUpper;
	
	::std::cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: lower_bound () upper_bound () count ()" << ::std::endl;
	
	for (i = 0; i < nInstance; i++)
	{
		nLastKey = 1;

		multiMapPrim_add (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_ASCEND_KEY);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		nKey = i + 1;

		sCIterLower = pClMM->lower_bound (nKey);
		sCIterUpper = pClMM->upper_bound (nKey);

		::std::advance (sCIterLower, (int) nInstance);

		if (sCIterLower != sCIterUpper)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeMultiMapSTLifLowerBoundUpperBound: ERROR: boundary mismatch!" << ::std::endl;

			exit (-1);
		}

		if (pClMM->count (nKey) != nInstance)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeMultiMapSTLifLowerBoundUpperBound: ERROR: count () returned with unexpected value!" << ::std::endl;

			exit (-1);
		}
	}

	nKey = 0;

	sCIterLower = pClMM->lower_bound (nKey);

	if (sCIterLower != pClMM->cbegin ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeMultiMapSTLifLowerBoundUpperBound: ERROR: lower_bound () didn't return cbegin () when testing for lowest key!" << ::std::endl;

		exit (-1);
	}

	nKey = (key_type) (nNumEntries + 1);

	sCIterUpper = pClMM->upper_bound (nKey);

	if (sCIterUpper != pClMM->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeMultiMapSTLifLowerBoundUpperBound: ERROR: upper_bound () didn't return cend () when testing for lowest key!" << ::std::endl;

		exit (-1);
	}
}

template<class _t_container>
void TestBTreeSTLmultiMap (uint32_t nTestNum, uint32_t nNodeSize, uint32_t nPageSize, _t_container *pMMapWrapper)
{
	typename _t_container::size_test_type		sTypeSelect;
	::std::string								sTypeStr;

	get_typename (sTypeSelect, sTypeStr);

	::std::cout << "b-tree multi-map test bench selected using type " << sTypeStr << ::std::endl;

	pMMapWrapper = new _t_container (nNodeSize, nPageSize);

	if (pMMapWrapper == NULL)
	{
		::std::cerr << "insufficient memory!" << ::std::endl;

		exit (-1);
	}

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
			TestBTreeMultiMapSTLifLowerBoundUpperBound (pMMapWrapper, 64, 5);

			break;
		}
	
	default:
		{
			::std::cerr << "ERROR: TestBTreeSTLmultiMap: Unknown test or test not specified!" << ::std::endl;

			exit (-1);

			break;
		}
	}

	delete pMMapWrapper;
}
