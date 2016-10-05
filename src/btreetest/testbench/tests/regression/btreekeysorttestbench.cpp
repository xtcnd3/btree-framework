/************************************************************
**
** file:	btreekeysorttestbench.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for the key sort data class.
**
************************************************************/

#include "btreekeysorttestbench.h"

template<class _t_container>
void TestBTreeKeySortBasic (_t_container *pContainer, typename _t_container::size_type nMaxEntries, typename _t_container::size_type nVariation, uint32_t nRepetitions, btreetest_key_generation_e eGeneratorAdd, btreetest_key_generation_e eGeneratorRemove)
{
	uint32_t								ui32;
	typename _t_container::key_type			nLastKey;
	typename _t_container::key_type			nLastKeyInit;

	::std::cout << "basic test adds data at ";

	switch (eGeneratorAdd)
	{
	case BTREETEST_KEY_GENERATION_DESCEND	:
		{
			::std::cout << "the beginning ";

			nLastKeyInit = ~0x0;

			break;
		}

	case BTREETEST_KEY_GENERATION_ASCEND		:
		{
			::std::cout << "the end ";

			nLastKeyInit = 0x0;

			break;
		}

	case BTREETEST_KEY_GENERATION_RANDOM		:
		{
			::std::cout << "a random position ";

			nLastKeyInit = 0x0;

			break;
		}

	case BTREETEST_KEY_GENERATION_CONST		:
		{
			::std::cout << "the same position ";

			nLastKeyInit = 0x0;

			break;
		}

	default									:
		{
			::std::cout << "? ";

			break;
		}
	}

	::std::cout << "and removes it again at ";

	switch (eGeneratorRemove)
	{
	case BTREETEST_KEY_GENERATION_DESCEND	:
		{
			::std::cout << "the beginning";

			break;
		}

	case BTREETEST_KEY_GENERATION_ASCEND		:
		{
			::std::cout << "the end";

			break;
		}

	case BTREETEST_KEY_GENERATION_RANDOM		:
		{
			::std::cout << "a random position";

			break;
		}

	case BTREETEST_KEY_GENERATION_CONST		:
		{
			::std::cout << "the same position";

			break;
		}

	default									:
		{
			::std::cout << "?";

			break;
		}
	}

	::std::cout << ::std::endl;

	nLastKey = nLastKeyInit;

	associative_container_add_primitive (pContainer, nMaxEntries, nLastKey, eGeneratorAdd);

	for (ui32 = 0; ui32 < nRepetitions; ui32++)
	{
		::std::cout << "cycle: " << ui32 + 1 << ::std::endl;

		nLastKey = nLastKeyInit;

		associative_container_remove_primitive (pContainer, nVariation, ~0x0, nLastKey, eGeneratorRemove);

		nLastKey = nLastKeyInit;

		associative_container_add_primitive (pContainer, nVariation, nLastKey, eGeneratorAdd);
	}

	nLastKey = nLastKeyInit;

	associative_container_remove_primitive (pContainer, nMaxEntries, ~0x0, nLastKey, eGeneratorRemove);

	::std::cout << "finished..." << ::std::endl;
}

template<class _t_container>
void TestBTreeKeySortRemoveAll (_t_container *pContainer, typename _t_container::size_type nEntries)
{
	typename _t_container::key_type			nLastKey = 0;

	::std::cout << "test inserts data and empties the data container by calling clear" << ::std::endl;

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	pContainer->clear ();
}

template<class _t_container>
void TestBTreeKeySortMultiRemove (_t_container *pContainer, typename _t_container::size_type nEntries)
{
	typename _t_container::key_type			nLastKey = 0;

	::std::cout << "test removes key sets" << ::std::endl;

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	associative_container_remove_primitive (pContainer, nEntries, ~0x0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);
}

template<class _t_container>
void TestBTreeKeySortUnload (_t_container *pContainer, typename _t_container::size_type nInitEntries, typename _t_container::size_type nAddEntries, typename _t_container::size_type nRemoveEntries, typename _t_container::size_type nExitEntries)
{
	typename _t_container::key_type			nLastKey = 0;

	::std::cout << "test uses unload after every operation" << ::std::endl;

	::std::cout << "unloading..." << ::std::endl;

	pContainer->unload ();

	associative_container_add_primitive (pContainer, nInitEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	::std::cout << "unloading..." << ::std::endl;

	pContainer->unload ();

	nLastKey++;

	associative_container_add_primitive (pContainer, nAddEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	::std::cout << "unloading..." << ::std::endl;

	pContainer->unload ();

	associative_container_remove_primitive (pContainer, nRemoveEntries, ~0x0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	::std::cout << "unloading..." << ::std::endl;

	pContainer->unload ();

	nLastKey--;

	associative_container_remove_primitive (pContainer, nExitEntries, ~0x0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	::std::cout << "unloading..." << ::std::endl;

	pContainer->unload ();

	associative_container_add_primitive (pContainer, nInitEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	::std::cout << "unloading..." << ::std::endl;

	pContainer->unload ();

	::std::cout << "unloading..." << ::std::endl;

	pContainer->unload ();

	::std::cout << "remove all" << ::std::endl;

	pContainer->clear ();

	::std::cout << "unloading..." << ::std::endl;

	pContainer->unload ();
}

template<class _t_container>
void TestBTreeKeySortRemoveInstance (_t_container *pContainer, typename _t_container::size_type nEntries, uint32_t nInstances)
{
	uint32_t								ui32;
	typename _t_container::key_type			nLastKey = 0;

	::std::cout << "test removes key instances" << ::std::endl;

	for (ui32 = 0; ui32 < nInstances; ui32++)
	{
		associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);
	}

	associative_container_remove_primitive (pContainer, nEntries, 2, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	associative_container_remove_primitive (pContainer, nEntries, 0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	associative_container_remove_primitive (pContainer, nEntries, ~0x0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);
}

template<class _t_container>
void TestBTreeKeySortCCdeterminePosition (_t_container *pContainer, uint32_t nNodeSize)
{
	typename _t_container::key_type				nKey = 0xABCDEF98;
	typename _t_container::size_type			nEntries;
	uint32_t									nInst;

	::std::cout << "test exercises code coverage regarding determinePosition" << ::std::endl;

	nEntries = nNodeSize * 2 + (nNodeSize * 2) - 1;

	associative_container_add_primitive (pContainer, nEntries, nKey, BTREETEST_KEY_GENERATION_CONST);

	nInst = nNodeSize * 2 - 1;

	associative_container_remove_primitive (pContainer, 1, nInst, nKey, BTREETEST_KEY_GENERATION_CONST);
}

template<class _t_container>
void TestBTreeKeySortCCfindFirstKey (_t_container *pContainer, uint32_t nNodeSize)
{
	typename _t_container::key_type							nKey = 0xABCDEF98;
	typename _t_container::size_type						nEntries;
	uint32_t												nInst;
	uint32_t												ui32;
	typename _t_container::iterator							sIter;

	::std::cout << "test exercises code coverage regarding findFirstKey" << ::std::endl;

	for (ui32 = 0, nEntries = 1; ui32 < 6; ui32++)
	{
		nEntries *= nNodeSize;
	}

	associative_container_add_primitive (pContainer, nEntries, nKey, BTREETEST_KEY_GENERATION_CONST);

	while (pContainer->size () > 0)
	{
		sIter = pContainer->lower_bound (nKey);

		typename _t_container::value_type		sValue (*sIter);

		nInst = generate_rand32 () % pContainer->size ();
		nInst /= 2;

		nKey = sValue.first;

		associative_container_remove_primitive (pContainer, 1, nInst, nKey, BTREETEST_KEY_GENERATION_CONST);
	}
}

template<class _t_container>
void TestBTreeKeySortHTMLoutput (_t_container *pContainer, typename _t_container::size_type nEntries)
{
	typename _t_container::key_type			nLastKey = 0;

	::std::cout << "tests if instance is left in undefined state after HTML output" << ::std::endl;

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	nLastKey++;

	pContainer->show_integrity ("test_keysort_output0_");

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	nLastKey++;

	pContainer->show_integrity ("test_keysort_output1_");

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);
}

template<class _t_container>
void TestBTreeKeySortCopyConstructorTest (_t_container *pClRef, _t_container sClCopy)
{
	if (sClCopy != *pClRef)
	{
		::std::cerr << ::std::endl << "ERROR: copied instance mismatches reference!" << ::std::endl;
		::std::cerr << "outputting reference to cc_reference.html" << ::std::endl;

		pClRef->show_integrity ("cc_reference.html");

		::std::cerr << "outputting copied instance to cc_copy.html" << ::std::endl;

		sClCopy.show_integrity ("cc_copy.html");

		exit (-1);
	}
}

template<class _t_container>
void TestBTreeKeySortCopyConstructor (_t_container *pContainer, typename _t_container::size_type nEntries)
{
	typename _t_container::key_type			nLastKey = 0;

	::std::cout << "exercises keysort type copy constructor" << ::std::endl;

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);

	nLastKey++;

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);

	nLastKey++;

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);

	pContainer->unload ();

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);

	nLastKey = 0;

	associative_container_remove_primitive (pContainer, nEntries, ~0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);

	nLastKey++;

	associative_container_remove_primitive (pContainer, nEntries, ~0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);

	pContainer->clear ();

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);
}

template<class _t_container>
void TestBTreeKeySortOperatorOverloadAssign (_t_container *pContainer, typename _t_container::size_type nEntries)
{
	typename _t_container::key_type			nLastKey = 0;
	_t_container							*pTarget;

	::std::cout << "exercises keysort type operator=" << ::std::endl;

	pTarget = new _t_container (*pContainer);

	if (pTarget == NULL)
	{
		::std::cerr << "ERROR: Insufficient memory!" << ::std::endl;

		exit (-1);
	}

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);

	nLastKey++;

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);

	nLastKey++;

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);

	pContainer->unload ();

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);

	nLastKey = 0;

	associative_container_remove_primitive (pContainer, nEntries, ~0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);

	nLastKey += 2;

	associative_container_remove_primitive (pContainer, nEntries, ~0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);

	pContainer->clear ();

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);

	delete pTarget;
}

template<class _t_container>
void TestBTreeKeySortOperatorOverloadAssignMultiInstance (_t_container *pContainer, typename _t_container::size_type nOuterEntries, typename _t_container::size_type nInnerEntries, uint32_t nMulti)
{
	typename _t_container::key_type				nLastKey = 0;
	typename _t_container::size_type			ui32;
	typename _t_container::size_type			nEntries;
	_t_container								*pTarget;

	::std::cout << "exercises keysort type operator= while more than one instance per key is in use" << ::std::endl;

	pTarget = new _t_container (*pContainer);

	if (pTarget == NULL)
	{
		::std::cerr << "ERROR: Insufficient memory!" << ::std::endl;

		exit (-1);
	}

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);

	for (ui32 = 0; ui32 < nMulti; ui32++)
	{
		if ((ui32 == 0) || (ui32 == (nMulti - 1)))
		{
			nEntries = nOuterEntries;
		}
		else
		{
			nEntries = nInnerEntries;
		}

		associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_CONST);

		nLastKey++;

		*pTarget = *pContainer;

		associative_container_compare_primitive (pContainer, pTarget);
	}

	pContainer->unload ();

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);

	nLastKey = 0;

	associative_container_remove_primitive (pContainer, nOuterEntries, 0, nLastKey, BTREETEST_KEY_GENERATION_CONST);

	nLastKey++;

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);

	associative_container_remove_primitive (pContainer, nInnerEntries, 0, nLastKey, BTREETEST_KEY_GENERATION_CONST);

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);

	pContainer->clear ();

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);

	delete pTarget;
}

template<class _t_container>
void TestBTreeKeySortLowerBoundOnLeafNode (_t_container *pContainer, uint32_t nNodeSize)
{
	typename _t_container::const_iterator	sCIter;
	typename _t_container::key_type			nLastKey = 0;
	typename _t_container::size_type		nRslt;

	::std::cout << "exercises lower_bound on leaf node with node size " << nNodeSize << ::std::endl;

	associative_container_add_primitive (pContainer, nNodeSize * 2 - 1, nLastKey, BTREETEST_KEY_GENERATION_CONST);

	sCIter = pContainer->lower_bound (nLastKey);

	nRslt = ::std::distance (pContainer->cbegin (), sCIter);

	if (nRslt != 0)
	{
		::std::cerr << "ERROR: TestBTreeKeySortLowerBoundOnLeafNode: pContainer->lower_bound did not return with iterator equivalent to begin ()" << ::std::endl;

		pContainer->show_integrity ("lower_bound_0_");

		exit (-1);
	}

	pContainer->clear ();

	associative_container_add_primitive (pContainer, nNodeSize - 1, nLastKey, BTREETEST_KEY_GENERATION_CONST);

	nLastKey += 2;

	associative_container_add_primitive (pContainer, nNodeSize - 1, nLastKey, BTREETEST_KEY_GENERATION_CONST);

	sCIter = pContainer->lower_bound (nLastKey);

	nRslt = ::std::distance (pContainer->cbegin (), sCIter);

	if (nRslt != (nNodeSize - 1))
	{
		::std::cerr << "ERROR: TestBTreeKeySortLowerBoundOnLeafNode: pContainer->lower_bound did not return with iterator equivalent to value t - 1" << ::std::endl;

		pContainer->show_integrity ("lower_bound_1_");

		exit (-1);
	}

	nLastKey -= 2;

	sCIter = pContainer->lower_bound (nLastKey);

	nRslt = ::std::distance (pContainer->cbegin (), sCIter);

	if (nRslt != 0)
	{
		::std::cerr << "ERROR: TestBTreeKeySortLowerBoundOnLeafNode: pContainer->lower_bound did not return with iterator equivalent to begin ()" << ::std::endl;

		pContainer->show_integrity ("lower_bound_2_");

		exit (-1);
	}

	nLastKey++;

	associative_container_add_primitive (pContainer, 2, nLastKey, BTREETEST_KEY_GENERATION_CONST);

	sCIter = pContainer->lower_bound (nLastKey);

	nRslt = ::std::distance (pContainer->cbegin (), sCIter);

	if (nRslt != (nNodeSize - 1))
	{
		::std::cerr << "ERROR: TestBTreeKeySortLowerBoundOnLeafNode: pContainer->lower_bound did not return with iterator equivalent to value t - 1" << ::std::endl;

		pContainer->show_integrity ("lower_bound_3_");

		exit (-1);
	}

	nLastKey--;

	associative_container_add_primitive (pContainer, nNodeSize * 2 - 1, nLastKey, BTREETEST_KEY_GENERATION_CONST);

	sCIter = pContainer->lower_bound (nLastKey);

	nRslt = ::std::distance (pContainer->cbegin (), sCIter);

	if (nRslt != 0)
	{
		::std::cerr << "ERROR: TestBTreeKeySortLowerBoundOnLeafNode: pContainer->lower_bound did not return with iterator equivalent to begin ()" << ::std::endl;

		pContainer->show_integrity ("lower_bound_4_");

		exit (-1);
	}

	nLastKey += 3;

	sCIter = pContainer->lower_bound (nLastKey);

	nRslt = ::std::distance (pContainer->cbegin (), sCIter);

	if (nRslt != pContainer->size ())
	{
		::std::cerr << "ERROR: TestBTreeKeySortLowerBoundOnLeafNode: pContainer->lower_bound did not return with iterator equivalent to end ()" << ::std::endl;

		pContainer->show_integrity ("lower_bound_5_");

		exit (-1);
	}
}

template<class _t_container>
void TestBTreeKeySortSerialize (_t_container *pContainer, uint32_t nInstance, typename _t_container::size_type nSize, typename _t_container::size_type nWindowSize)
{
	typedef typename _t_container::value_type				data_t;
	typedef typename _t_container::const_iterator			citer_t;

	typename _t_container::key_type			nLastKey = 0;
	uint32_t								ui32;
	typename _t_container::size_type		nRslt;
	data_t									*pnData;

	::std::cout << "exercises serialize method" << ::std::endl;

	pnData = new data_t [nWindowSize];

	if (pnData == NULL)
	{
		::std::cerr << "ERROR: TestBTreeKeySortSerialize: insufficient memory!" << ::std::endl;

		exit (-1);
	}

	for (ui32 = 0; ui32 < nInstance; ui32++)
	{
		associative_container_add_primitive (pContainer, nSize, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

		nLastKey = 0;
	}

	for (ui32 = 0; ui32 < ((nSize * nInstance) - nWindowSize + 1); ui32++)
	{
		nRslt = pContainer->serialize (ui32, nWindowSize, pnData);

		::std::cout << "testing from " << ui32 << " to " << ui32 + nWindowSize << ::std::endl;

		if (nRslt != nWindowSize)
		{
			::std::cerr << "ERROR: TestBTreeKeySortSerialize: size mismatch!" << ::std::endl;
			::std::cerr << "iteration: " << ui32 << ::std::endl;
			::std::cerr << "window size: " << nWindowSize << ::std::endl;
			::std::cerr << "returned size: " << nRslt << ::std::endl;
			::std::cerr << "outputting serialize_????.html" << ::std::endl << ::std::flush;

			pContainer->show_integrity ("serialize_");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}
	}

	delete [] pnData;
}

template<class _t_container>
void TestBTreeKeySortSerializeIncomplete (_t_container *pContainer, uint32_t nNodeSize, typename _t_container::size_type nSize, typename _t_container::size_type nWindowSize, typename _t_container::size_type nOffset)
{
	typedef typename _t_container::value_type			data_t;

	typename _t_container::key_type		nLastKey = 0;
	typename _t_container::size_type	nRslt;
	data_t								*pnData;

	::std::cout << "exercises serialize method with an incomplete transfer" << ::std::endl;

	pnData = new data_t [nWindowSize];

	if (pnData == NULL)
	{
		::std::cerr << "ERROR: TestBTreeKeySortSerialize: insufficient memory!" << ::std::endl;

		exit (-1);
	}

	associative_container_add_primitive (pContainer, nSize, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	::std::cout << "testing from " << nOffset << " to " << nWindowSize + nOffset << ::std::endl;

	nRslt = pContainer->serialize (nOffset, nWindowSize, pnData);

	if (nRslt != (nSize - nOffset))
	{
		::std::cerr << "ERROR: TestBTreeKeySortSerialize: size mismatch!" << ::std::endl;
		::std::cerr << "expected size: " << nSize - nOffset << ::std::endl;
		::std::cerr << "returned size: " << nRslt << ::std::endl;

		exit (-1);
	}

	delete [] pnData;
}

template<class _t_container>
void TestBTreeKeySortSTLifInsert (_t_container *pContainer, uint32_t nNumEntries)
{
	typedef typename _t_container::value_type			data_t;
	typedef typename _t_container::const_iterator		citer_t;

	citer_t									sCIter;
	citer_t									sCIterRd;
	uint32_t								i;
	data_t									sEntry;
	uint32_t								nCnt;
	typename _t_container::key_type			nKey;

	::std::cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeySort<>:: insert (const _t_data &)" << ::std::endl;

	for (i = 0; i < nNumEntries; i++)
	{
		::std::cout << i << " / " << nNumEntries << "\r";

		do
		{
			nKey = generate_rand32 ();
		} while (pContainer->count (nKey) != 0);

		set_entry_key (sEntry, nKey);
		set_entry_data (sEntry, generate_rand32 ());
		set_entry_debug (sEntry, i);

		sCIter = pContainer->insert (sEntry);

		sCIterRd = sCIter;

		for (nCnt = 0; sCIterRd != pContainer->cbegin (); nCnt++)
		{
			if (get_entry_key (*sCIterRd) > get_entry_key (sEntry))
			{
				break;
			}

			sCIterRd--;
		}

		if (sCIterRd != pContainer->cbegin ())
		{
			::std::cerr << ::std::endl;
			::std::cerr << "ERROR: TestBTreeKeySortSTLinsert: returned key doesn't fulfill required sort condition! (<=)" << ::std::endl;
			::std::cerr << "new entry key: " << get_entry_key (sEntry) << ::std::endl;
			::std::cerr << " returned key: " << (*sCIterRd).first << ::std::endl;

			exit (-1);
		}

		sCIterRd = sCIter;

		for (nCnt = 0; sCIterRd != pContainer->cend (); nCnt++)
		{
			if (get_entry_key (*sCIterRd) < get_entry_key (sEntry))
			{
				break;
			}

			sCIterRd++;
		}

		if (sCIterRd != pContainer->cend ())
		{
			::std::cerr << ::std::endl;
			::std::cerr << "ERROR: TestBTreeKeySortSTLinsert: returned key doesn't fulfill required sort condition! (>=)" << ::std::endl;
			::std::cerr << "new entry key: " << get_entry_key (sEntry) << ::std::endl;
			::std::cerr << " returned key: " << (*sCIterRd).first << ::std::endl;

			exit (-1);
		}
	}

	::std::cout << i << " / " << nNumEntries << "\n";
}

template<class _t_dest_container, class _t_src_container, class _t_container, class _t_iterator, class _t_ext_iterator>
void TestBTreeKeySortSTLifInsertViaIteratorEx (btreetest_keysort_stl_if_insert_via_iterator_e eTest, const char *pszTestTitle, int nArg, uint32_t nNumInstances, _t_dest_container *pDestContainer, _t_src_container *pSrcContainer, _t_container *pContainer, _t_iterator &rIterA, _t_iterator &rIterB, _t_ext_iterator &rExtIterA, _t_ext_iterator &rExtIterB, typename _t_src_container::size_type nNumEntries, typename _t_src_container::key_type &rLastKey)
{
	typedef typename _t_src_container::size_type				size_type;

	_t_iterator		sIterBegin;
	_t_iterator		sIterEnd;
	_t_ext_iterator	sExtIterBegin;
	_t_ext_iterator	sExtIterEnd;
	size_type		nSize;
	uint32_t		ui32;

	::std::cout << pszTestTitle << ::std::endl;

	associative_container_add_primitive (pSrcContainer, nNumEntries, rLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	get_begin (pSrcContainer, sIterBegin);
	get_end (pSrcContainer, sIterEnd);

	nSize = pSrcContainer->size ();

	switch (eTest)
	{
		case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART:
		{
			::std::advance (sIterBegin, nArg);
			::std::advance (sIterEnd, 0 - nArg);

			pDestContainer->insert (sIterBegin, sIterEnd);

			break;
		}

		case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT:
		{
			pDestContainer->insert (sIterBegin, sIterEnd);

			get_begin (pSrcContainer, rIterA);
			get_begin (pSrcContainer, rIterB);

			while (rIterA != sIterEnd)
			{
				pDestContainer->insert (rIterA, rIterB);

				rIterA++;
				rIterB++;

				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: equivalent or same test 1: container size changed!" << ::std::endl;

					exit (-1);
				}
			}

			get_begin (pDestContainer, rIterA);
			get_begin (pDestContainer, rIterB);

			get_end (pDestContainer, sIterEnd);

			while (rIterA != sIterEnd)
			{
				pDestContainer->insert (rIterA, rIterB);

				rIterA++;
				rIterB++;

				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: equivalent or same test 2: container size changed!" << ::std::endl;

					exit (-1);
				}
			}

			break;
		}

		case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE	:
		{
			for (ui32 = 0; ui32 < nNumInstances; ui32++)
			{
				pDestContainer->insert (sIterBegin, sIterEnd);
			}

			get_begin (pDestContainer, sIterBegin);
			get_end (pDestContainer, sIterEnd);

			::std::advance (sIterBegin, nArg);
			::std::advance (sIterEnd, 0 - nArg);

			if (!is_const_iterator (pDestContainer, sIterBegin))
			{
				if (!is_reverse_iterator (pDestContainer, sIterBegin))
				{
					typename _t_dest_container::test_iterator				sIter;

					pDestContainer->insert_self_reference (sIterBegin, sIterEnd, sIter);
				}
				else
				{
					typename _t_dest_container::test_reverse_iterator		sRIter;

					pDestContainer->insert_self_reference (sIterBegin, sIterEnd, sRIter);
				}
			}
			else
			{
				if (!is_reverse_iterator (pDestContainer, sIterBegin))
				{
					typename _t_dest_container::test_const_iterator			sCIter;

					pDestContainer->insert_self_reference (sIterBegin, sIterEnd, sCIter);
				}
				else
				{
					typename _t_dest_container::test_const_reverse_iterator	sCRIter;

					pDestContainer->insert_self_reference (sIterBegin, sIterEnd, sCRIter);
				}
			}

			break;
		}

		case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN:
		{
			container_assign (pContainer, sIterBegin, sIterEnd);

			get_begin (pContainer, sExtIterBegin);
			get_end (pContainer, sExtIterEnd);

			::std::advance (sExtIterBegin, nArg);
			::std::advance (sExtIterEnd, 0 - nArg);

			pDestContainer->insert (sExtIterBegin, sExtIterEnd);

			break;
		}

		case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN:
		{
			container_assign (pContainer, sIterBegin, sIterEnd);

			get_begin (pContainer, rExtIterA);
			get_begin (pContainer, rExtIterB);

			get_end (pContainer, sExtIterEnd);

			nSize = pDestContainer->size ();

			while (rExtIterA != sExtIterEnd)
			{
				pDestContainer->insert (rExtIterA, rExtIterB);

				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: empty equivalent or same extern test (A == B): container size changed!" << ::std::endl;

					exit (-1);
				}

				pDestContainer->insert (rExtIterB, rExtIterA);

				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: empty equivalent or same extern test (B == A): container size changed!" << ::std::endl;

					exit (-1);
				}

				pDestContainer->insert (rExtIterA, rExtIterA);

				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: empty equivalent or same extern test (A == A): container size changed!" << ::std::endl;

					exit (-1);
				}

				pDestContainer->insert (rExtIterB, rExtIterB);

				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: empty equivalent or same extern test (B == B): container size changed!" << ::std::endl;

					exit (-1);
				}

				rExtIterA++;
				rExtIterB++;
			}

			pDestContainer->insert (sIterBegin, sIterEnd);

			nSize = pDestContainer->size ();

			get_begin (pContainer, rExtIterA);
			get_begin (pContainer, rExtIterB);

			while (rExtIterA != sExtIterEnd)
			{
				pDestContainer->insert (rExtIterA, rExtIterB);

				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: equivalent or same extern test (A == B): container size changed!" << ::std::endl;

					exit (-1);
				}

				pDestContainer->insert (rExtIterB, rExtIterA);

				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: equivalent or same extern test (B == A): container size changed!" << ::std::endl;

					exit (-1);
				}

				pDestContainer->insert (rExtIterA, rExtIterA);

				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: equivalent or same extern test (A == A): container size changed!" << ::std::endl;

					exit (-1);
				}

				pDestContainer->insert (rExtIterB, rExtIterB);

				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: equivalent or same extern test (B == B): container size changed!" << ::std::endl;

					exit (-1);
				}

				rExtIterA++;
				rExtIterB++;
			}

			break;
		}

		default:
		{
			::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: unknown test!" << ::std::endl;

			exit (-1);

			break;
		}
	}

	pContainer->clear ();
	pDestContainer->clear ();
	pSrcContainer->clear ();
}

template<class _t_dest_container, class _t_src_container, class _t_data, class _t_dest_iterator, class _t_src_iterator>
void TestBTreeKeySortSTLifInsertViaIteratorPairEx (btreetest_keysort_stl_if_insert_via_iterator_e eTest, const char *pszTestTitle, int nArg, uint32_t nNumInstances, _t_dest_container *pDestContainer, _t_src_container *pSrcContainer, _t_data &rKeySortPairData, _t_dest_iterator &rDestIter, _t_src_iterator &rSrcIter, typename _t_src_container::size_type nNumEntries)
{
	_t_src_iterator									sSrcIterBegin;
	_t_src_iterator									sSrcIterBeginB;
	_t_src_iterator									sSrcIterEnd;
	_t_dest_iterator								sDestIterBegin;
	_t_dest_iterator								sDestIterBeginB;
	_t_dest_iterator								sDestIterEnd;
	typename _t_src_container::size_type			nSize;
	typename _t_src_container::size_type			ui32;

	::std::cout << pszTestTitle << ::std::endl;

	for (ui32 = 0; ui32 < nNumEntries; ui32++)
	{
		_t_data 	sKeySortPairData (generate_rand32 (), {generate_rand32 (), g_nDebug});

		g_nDebug++;

		pSrcContainer->insert (sKeySortPairData);

		::std::cout << ui32 << " / " << nNumEntries << "\r";
	}

	::std::cout << ui32 << " / " << nNumEntries << ::std::endl << ::std::flush;

	get_begin (pSrcContainer, sSrcIterBegin);
	get_end (pSrcContainer, sSrcIterEnd);

	nSize = pSrcContainer->size ();

	switch (eTest)
	{
		case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART:
		{
			::std::advance (sSrcIterBegin, nArg);
			::std::advance (sSrcIterEnd, 0 - nArg);

			pDestContainer->insert (sSrcIterBegin, sSrcIterEnd);

			break;
		}

		case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT:
		{
			pDestContainer->insert (sSrcIterBegin, sSrcIterEnd);

			while (sSrcIterBegin != sSrcIterEnd)
			{
				pDestContainer->insert (sSrcIterBegin, sSrcIterBegin);

				::std::advance (sSrcIterBegin, 1);

				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorPairEx: equivalent or same test 1: container size changed!" << ::std::endl;

					exit (-1);
				}
			}

			get_begin (pSrcContainer, sSrcIterBegin);
			get_begin (pSrcContainer, sSrcIterBeginB);

			while (sSrcIterBegin != sSrcIterEnd)
			{
				pDestContainer->insert (sSrcIterBegin, sSrcIterBeginB);

				::std::advance (sSrcIterBegin, 1);
				::std::advance (sSrcIterBeginB, 1);

				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorPairEx: equivalent or same test 2: container size changed!" << ::std::endl;

					exit (-1);
				}
			}

			get_begin (pSrcContainer, sSrcIterBegin);
			get_begin (pSrcContainer, sSrcIterBeginB);

			while (sSrcIterBegin != sSrcIterEnd)
			{
				pDestContainer->insert (sSrcIterBeginB, sSrcIterBegin);

				::std::advance (sSrcIterBegin, 1);
				::std::advance (sSrcIterBeginB, 1);

				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorPairEx: equivalent or same test 3: container size changed!" << ::std::endl;

					exit (-1);
				}
			}

			break;
		}

		default:
		{
			::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorPairEx: unknown test!" << ::std::endl;

			exit (-1);

			break;
		}
	}

	pSrcContainer->clear ();
	pDestContainer->clear ();
}

template<class _t_container, class _t_pair_container>
void TestBTreeKeySortSTLifInsertViaIterator (_t_container *pContainer, _t_pair_container *pPairContainer, typename _t_container::size_type nNumEntries, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nNodeSize)
{
	typedef ::std::list<typename _t_container::value_type>								list_t;
	typedef ::std::vector<typename _t_container::value_type>							vector_t;
	typedef	::std::map<uint32_t, keySortMap_t>											map_t;
	typedef ::std::multimap<uint32_t, keySortMap_t>										mmap_t;

	typedef typename _t_container::const_iterator										citer_t;
	typedef typename _t_container::const_reverse_iterator								criter_t;

	typedef typename list_t::iterator													itlist_t;
	typedef typename list_t::const_iterator												citlist_t;
	typedef typename list_t::reverse_iterator											ritlist_t;
	typedef typename list_t::const_reverse_iterator										critlist_t;

	typedef typename vector_t::iterator													itvec_t;
	typedef typename vector_t::const_iterator											citvec_t;
	typedef typename vector_t::reverse_iterator											ritvec_t;
	typedef typename vector_t::const_reverse_iterator									critvec_t;

	typedef CBTreeKeySort<keySortEntry_t, uint32_t, size_t>								CBTreeKeySort_VectorFeed_t;

	typedef typename _t_pair_container::const_iterator									citer_pair_t;
	typedef typename _t_pair_container::const_reverse_iterator							criter_pair_t;

	typedef typename map_t::iterator													itmap_t;
	typedef typename map_t::const_iterator												citmap_t;
	typedef typename map_t::reverse_iterator											ritmap_t;
	typedef typename map_t::const_reverse_iterator										critmap_t;

	typedef typename mmap_t::iterator													itmmap_t;
	typedef typename mmap_t::const_iterator												citmmap_t;
	typedef typename mmap_t::reverse_iterator											ritmmap_t;
	typedef typename mmap_t::const_reverse_iterator										critmmap_t;

	typename _t_pair_container::size_type		nNumEntriesPair = (typename _t_pair_container::size_type) nNumEntries;
	_t_container								sContainerTarget (*pContainer);
	typename _t_container::key_type				nLastKey = 0;
	citer_t										sCIterA, sCIterB;
	criter_t									sCRIterA, sCRIterB;

	list_t										sList;
	vector_t									sVector;

	itlist_t									sItListBegin, sItListEnd;
	citlist_t									sCItListBegin, sCItListEnd;
	ritlist_t									sRItListBegin, sRItListEnd;
	critlist_t									sCRItListBegin, sCRItListEnd;
	itvec_t										sItVecBegin, sItVecEnd;
	citvec_t									sCItVecBegin, sCItVecEnd;
	ritvec_t									sRItVecBegin, sRItVecEnd;
	critvec_t									sCRItVecBegin, sCRItVecEnd;
	_t_pair_container							sKeySortTargetPair (*pPairContainer);
	keySortPair_t								sKeySortPairData;
	citer_pair_t								sCIterTargetPair, sCIterPair;
	criter_pair_t								sCRIterTargetPair, sCRIterPair;
	map_t										sMap;
	mmap_t										sMMap;
	itmap_t										sItMap;
	citmap_t									sCItMap;
	ritmap_t									sRItMap;
	critmap_t									sCRItMap;
	itmmap_t									sItMMap;
	citmmap_t									sCItMMap;
	ritmmap_t									sRItMMap;
	critmmap_t									sCRItMMap;

	::std::cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeySort<>:: insert<_t_iterator> (_t_iterator, _t_iterator)" << ::std::endl;

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_obj::citer_t> (citer_t, citer_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_obj::criter_t> (criter_t, criter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCRIterA, sCRIterB, sCRItListBegin, sCRItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<_t_obj::citer_t> (citer_t == citer_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<_t_obj::criter_t> (criter_t == criter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCRIterA, sCRIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_obj::citer_t> (>citer_t, citer_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_obj::criter_t> (>criter_t, criter_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sList, sCRIterA, sCRIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::citer_t> (target::citer_t, target::citer_t)", 0, 1, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::criter_t> (target::criter_t, target::criter_t)", 0, 1, &sContainerTarget, pContainer, &sList, sCRIterA, sCRIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::citer_t> x 4 (>target::citer_t, target::citer_t<)", (int) (nNumEntries / 4), 4, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::criter_t> x 4 (>target::criter_t, target::criter_t<)", (int) (nNumEntries / 4), 4, &sContainerTarget, pContainer, &sList, sCRIterA, sCRIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::iter_t> (iter_t, iter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sItListBegin, sItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::citer_t> (citer_t, citer_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::riter_t> (riter_t, riter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sRItListBegin, sRItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::criter_t> (criter_t, criter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCRItListBegin, sCRItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::iter_t> (>iter_t, iter_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sItListBegin, sItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::citer_t> (>citer_t, citer_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::riter_t> (>riter_t, riter_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sRItListBegin, sRItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::criter_t> (>criter_t, criter_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCRItListBegin, sCRItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<list::iter_t> (iter_t == iter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sItListBegin, sItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<list::citer_t> (citer_t == citer_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<list::riter_t> (riter_t == riter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sRItListBegin, sRItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<list::criter_t> (criter_t == criter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCRItListBegin, sCRItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::iter_t> (iter_t, iter_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sItVecBegin, sItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::citer_t> (citer_t, citer_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sCItVecBegin, sCItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::riter_t> (riter_t, riter_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sRItVecBegin, sRItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::criter_t> (criter_t, criter_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sCRItVecBegin, sCRItVecEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::iter_t> (>iter_t, iter_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sItVecBegin, sItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::citer_t> (>citer_t, citer_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sCItVecBegin, sCItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::riter_t> (>riter_t, riter_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sRItVecBegin, sRItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::criter_t> (>criter_t, criter_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sCRItVecBegin, sCRItVecEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<vector::iter_t> (iter_t == iter_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sItVecBegin, sItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<vector::citer_t> (citer_t == citer_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sCItVecBegin, sCItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<vector::riter_t> (riter_t == riter_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sRItVecBegin, sRItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<vector::criter_t> (criter_t == criter_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sCRItVecBegin, sCRItVecEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::citer_t> (citer_t, citer_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCIterTargetPair, sCIterPair, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::criter_t> (criter_t, criter_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCRIterTargetPair, sCRIterPair, nNumEntriesPair);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<_t_container<pair>::citer_t> (citer_t == citer_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCIterTargetPair, sCIterPair, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<_t_container<pair>::criter_t> (criter_t == criter_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCRIterTargetPair, sCRIterPair, nNumEntriesPair);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::citer_t> (>citer_t, citer_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCIterTargetPair, sCIterPair, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::criter_t> (>criter_t, criter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCRIterTargetPair, sCRIterPair, nNumEntriesPair);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::citer_t> (target::citer_t, target::citer_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCIterTargetPair, sCIterPair, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::criter_t> (target::criter_t, target::criter_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCRIterTargetPair, sCRIterPair, nNumEntriesPair);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<_t_container<pair>::citer_t> (target::citer_t == target::citer_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCIterTargetPair, sCIterPair, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<_t_container<pair>::criter_t> (target::criter_t == target::criter_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCRIterTargetPair, sCRIterPair, nNumEntriesPair);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::citer_t> (>target::citer_t, target::citer_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCIterTargetPair, sCIterPair, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::criter_t> (>target::criter_t, target::criter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCRIterTargetPair, sCRIterPair, nNumEntriesPair);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::iter_t> (iter_t, iter_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sItMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::citer_t> (citer_t, citer_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sCItMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::riter_t> (riter_t, riter_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sRItMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::criter_t> (criter_t, criter_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sCRItMap, nNumEntriesPair);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::map::iter_t> (iter_t == iter_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sItMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::map::citer_t> (citer_t == citer_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sCItMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::map::riter_t> (riter_t == riter_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sRItMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::map::criter_t> (criter_t == criter_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sCRItMap, nNumEntriesPair);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::iter_t> (>iter_t, iter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sItMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::citer_t> (>citer_t, citer_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sCItMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::riter_t> (>riter_t, riter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sRItMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::criter_t> (>criter_t, criter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sCRItMap, nNumEntriesPair);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::iter_t> (iter_t, iter_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sItMMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::citer_t> (citer_t, citer_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sCItMMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::riter_t> (riter_t, riter_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sRItMMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::criter_t> (criter_t, criter_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sCRItMMap, nNumEntriesPair);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::multimap::iter_t> (iter_t == iter_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sItMMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::multimap::citer_t> (citer_t == citer_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sCItMMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::multimap::riter_t> (riter_t == riter_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sRItMMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::multimap::criter_t> (criter_t == criter_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sCRItMMap, nNumEntriesPair);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::iter_t> (>iter_t, iter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sItMMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::citer_t> (>citer_t, citer_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sCItMMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::riter_t> (>riter_t, riter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sRItMMap, nNumEntriesPair);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::criter_t> (>criter_t, criter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sCRItMMap, nNumEntriesPair);
}

template<class _t_container, class _t_pair_container>
void TestBTreeKeySortSTLifSwap (_t_container *pContainer, _t_pair_container *pPairContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::const_iterator						citer_t;

	typedef typename _t_pair_container::const_iterator					citer_pair_t;

	typename _t_container::key_type			nLastKey = 0;
	typename _t_pair_container::size_type	nNumEntriesPair = (typename _t_pair_container::size_type) nNumEntries;
	_t_container							sContainerTarget (*pContainer);
	_t_container							sContainerRef (*pContainer);
	_t_container							sContainerTargetRef (*pContainer);
	_t_pair_container						sContainerPairTarget (*pPairContainer);
	_t_pair_container						sContainerPairRef (*pPairContainer);
	_t_pair_container						sContainerPairTargetRef (*pPairContainer);
	typename _t_pair_container::size_type	i;

	::std::cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeysort<>:: swap ()" << ::std::endl;

	associative_container_add_primitive (pContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	sContainerRef.insert (pContainer->cbegin (), pContainer->cend ());

	if ((*pContainer) != sContainerRef)
	{
		::std::cerr << "ERROR: Unexpected keysort mismatch!" << ::std::endl;

		exit (-1);
	}

	associative_container_add_primitive (&sContainerTarget, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	sContainerTargetRef.insert (sContainerTarget.cbegin (), sContainerTarget.cend ());

	if (sContainerTarget != sContainerTargetRef)
	{
		::std::cerr << "ERROR: Unexpected target mismatch!" << ::std::endl;

		exit (-1);
	}

	pContainer->swap (sContainerTarget);

	if ((*pContainer) != sContainerTargetRef)
	{
		::std::cerr << "ERROR: array mismatches target reference!" << ::std::endl;

		exit (-1);
	}

	if (sContainerTarget != sContainerRef)
	{
		::std::cerr << "ERROR: target mismatches array reference!" << ::std::endl;

		exit (-1);
	}

	sContainerTarget.swap (*pContainer);

	if ((*pContainer) != sContainerRef)
	{
		::std::cerr << "ERROR: array mismatches array reference!" << ::std::endl;

		exit (-1);
	}

	if (sContainerTarget != sContainerTargetRef)
	{
		::std::cerr << "ERROR: target mismatches target reference!" << ::std::endl;

		exit (-1);
	}

	for (i = 0; i < nNumEntriesPair; i++)
	{
		keySortPair_t			sEntryPair (generate_rand32 (), {generate_rand32 (), g_nDebug});

		g_nDebug++;

		pPairContainer->insert (sEntryPair);

		::std::cout << i << " / " << nNumEntriesPair << "\r";
	}

	::std::cout << i << " / " << nNumEntriesPair << ::std::endl;

	sContainerPairRef.insert (pPairContainer->cbegin (), pPairContainer->cend ());

	if (((_t_pair_container &) *pPairContainer) != sContainerPairRef)
	{
		::std::cerr << "ERROR: Unexpected keysort mismatch! (pair)" << ::std::endl;

		exit (-1);
	}

	for (i = 0; i < nNumEntriesPair; i++)
	{
		keySortPair_t			sEntryPair (generate_rand32 (), {generate_rand32 (), g_nDebug});

		g_nDebug++;

		sContainerPairTarget.insert (sEntryPair);

		::std::cout << i << " / " << nNumEntriesPair << "\r";
	}

	::std::cout << i << " / " << nNumEntriesPair << ::std::endl;

	sContainerPairTargetRef.insert (sContainerPairTarget.cbegin (), sContainerPairTarget.cend ());

	if (sContainerPairTarget != sContainerPairTargetRef)
	{
		::std::cerr << "ERROR: Unexpected target mismatch! (pair)" << ::std::endl;

		exit (-1);
	}

	pPairContainer->swap (sContainerPairTarget);

	if (((_t_pair_container &) *pPairContainer) != sContainerPairTargetRef)
	{
		::std::cerr << "ERROR: array mismatches target reference! (pair)" << ::std::endl;

		exit (-1);
	}

	if (sContainerPairTarget != sContainerPairRef)
	{
		::std::cerr << "ERROR: target mismatches array reference! (pair)" << ::std::endl;

		exit (-1);
	}

	sContainerPairTarget.swap (((_t_pair_container &) *pPairContainer));

	if (((_t_pair_container &) *pPairContainer) != sContainerPairRef)
	{
		::std::cerr << "ERROR: array mismatches array reference! (pair)" << ::std::endl;

		exit (-1);
	}

	if (sContainerPairTarget != sContainerPairTargetRef)
	{
		::std::cerr << "ERROR: target mismatches target reference! (pair)" << ::std::endl;

		exit (-1);
	}
}

template<class _t_container, class _t_pair_container>
void TestBTreeKeySortSTLifFind (_t_container *pContainer, _t_pair_container *pPairContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::const_iterator				citer_t;

	typedef typename _t_pair_container::const_iterator			citer_pair_t;

	typedef typename _t_container::key_type						key_type;

	key_type								nLastKey = 1;
	key_type								nKey;
	typename _t_pair_container::key_type	nPairKey;
	typename _t_pair_container::size_type	nNumEntriesPair = (typename _t_pair_container::size_type) nNumEntries;
	typename _t_pair_container::size_type	i;
	citer_t									sCIter;
	citer_t									sCIterRslt;
	citer_pair_t							sCItPair;
	citer_pair_t							sCItPairRslt;

	::std::cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeysort<>:: find ()" << ::std::endl;

	associative_container_add_primitive (pContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_ASCEND);

	for (sCIter = pContainer->cbegin (); sCIter != pContainer->cend (); sCIter++)
	{
		nKey = get_entry_key (*sCIter);

		if ((nKey & 0x1) == 0x0)
		{
			sCIterRslt = pContainer->find (nKey);
		}
		else
		{
			sCIterRslt = ((const _t_container *) pContainer)->find (nKey);
		}

		if (sCIter != sCIterRslt)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeKeySortSTLifFind: ERROR: iterator mismatch!" << ::std::endl;

			exit (-1);
		}
	}

	nKey = 0;

	sCIterRslt = pContainer->find (nKey);

	if (sCIterRslt != pContainer->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on lower bound check!" << ::std::endl;

		exit (-1);
	}

	sCIterRslt = ((const _t_container *) pContainer)->find (nKey);

	if (sCIterRslt != pContainer->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on lower bound check! (const)" << ::std::endl;

		exit (-1);
	}

	nKey = (key_type) (nNumEntries + 1);

	sCIterRslt = pContainer->find (nKey);

	if (sCIterRslt != pContainer->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on upper bound check!" << ::std::endl;

		exit (-1);
	}

	sCIterRslt = ((const _t_container *) pContainer)->find (nKey);

	if (sCIterRslt != pContainer->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on upper bound check! (const)" << ::std::endl;

		exit (-1);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		keySortPair_t		sEntryPair ((typename _t_pair_container::key_type) (i + 1), {generate_rand32 (), g_nDebug});

		g_nDebug++;

		pPairContainer->insert (sEntryPair);

		::std::cout << i << " / " << nNumEntries << "\r";
	}

	::std::cout << i << " / " << nNumEntries << ::std::endl;

	for (sCItPair = pPairContainer->cbegin (); sCItPair != pPairContainer->cend (); sCItPair++)
	{
		nPairKey = get_entry_key (*sCItPair);

		if ((nPairKey & 0x1) == 0x0)
		{
			sCItPairRslt = pPairContainer->find (nPairKey);
		}
		else
		{
			sCItPairRslt = ((const _t_pair_container *) pPairContainer)->find (nPairKey);
		}

		if (sCItPair != sCItPairRslt)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeKeySortSTLifFind: ERROR: iterator mismatch! (pair)" << ::std::endl;

			exit (-1);
		}
	}

	nPairKey = 0;

	sCItPairRslt = pPairContainer->find (nPairKey);

	if (sCItPairRslt != pPairContainer->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on lower bound check! (pair)" << ::std::endl;

		exit (-1);
	}

	sCItPairRslt = ((const _t_pair_container *) pPairContainer)->find (nPairKey);

	if (sCItPairRslt != pPairContainer->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on lower bound check! (const pair)" << ::std::endl;

		exit (-1);
	}

	nPairKey = (typename _t_pair_container::key_type) (nNumEntries + 1);

	sCItPairRslt = pPairContainer->find (nPairKey);

	if (sCItPairRslt != pPairContainer->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on upper bound check! (pair)" << ::std::endl;

		exit (-1);
	}

	sCItPairRslt = ((const _t_pair_container *) pPairContainer)->find (nPairKey);

	if (sCItPairRslt != pPairContainer->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on upper bound check! (const pair)" << ::std::endl;

		exit (-1);
	}
}

template<class _t_container, class _t_pair_container>
void TestBTreeKeySortSTLifLowerBoundUpperBound (_t_container *pContainer, _t_pair_container *pPairContainer, typename _t_container::size_type nNumEntries, uint32_t nInstance)
{
	typedef typename _t_container::const_iterator			citer_t;

	typedef typename _t_pair_container::const_iterator		citer_pair_t;

	typedef typename _t_container::key_type					key_type;

	key_type								nLastKey;
	key_type								nKey;
	typename _t_pair_container::key_type	nPairKey;
	typename _t_pair_container::size_type	nNumEntriesPair = (typename _t_pair_container::size_type) nNumEntries;
	uint32_t								i;
	uint32_t								j;
	citer_t									sCIterLower;
	citer_t									sCIterUpper;
	citer_pair_t							sCItPairLower;
	citer_pair_t							sCItPairUpper;

	::std::cout << "exercises methods compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeysort<>:: lower_bound () upper_bound () count ()" << ::std::endl;

	for (i = 0; i < nInstance; i++)
	{
		nLastKey = 1;

		associative_container_add_primitive (pContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_ASCEND);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		nKey = i + 1;

		sCIterLower = pContainer->lower_bound (nKey);
		sCIterUpper = pContainer->upper_bound (nKey);

		::std::advance (sCIterLower, (int) nInstance);

		if (sCIterLower != sCIterUpper)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: boundary mismatch!" << ::std::endl;

			exit (-1);
		}

		if (pContainer->count (nKey) != nInstance)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: count () returned with unexpected value!" << ::std::endl;

			exit (-1);
		}
	}

	nKey = 0;

	sCIterLower = pContainer->lower_bound (nKey);

	if (sCIterLower != pContainer->cbegin ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: lower_bound () didn't return cbegin () when testing for lowest key!" << ::std::endl;

		exit (-1);
	}

	nKey = (key_type) (nNumEntries + 1);

	sCIterUpper = pContainer->upper_bound (nKey);

	if (sCIterUpper != pContainer->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: upper_bound () didn't return cend () when testing for lowest key!" << ::std::endl;

		exit (-1);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		for (j = 0; j < nInstance; j++)
		{
			keySortPair_t			sEntryPair (i + 1, {generate_rand32 (), g_nDebug});

			g_nDebug++;

			pPairContainer->insert (sEntryPair);
		}

		::std::cout << i << " / " << nNumEntries << "\r";
	}

	::std::cout << i << " / " << nNumEntries << ::std::endl;

	for (i = 0; i < nNumEntries; i++)
	{
		nPairKey = i + 1;

		sCItPairLower = pPairContainer->lower_bound (nPairKey);
		sCItPairUpper = pPairContainer->upper_bound (nPairKey);

		::std::advance (sCItPairLower, (int) nInstance);

		if (sCItPairLower != sCItPairUpper)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: boundary mismatch! (pair)" << ::std::endl;

			exit (-1);
		}

		if (pPairContainer->count (nPairKey) != nInstance)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: count () returned with unexpected value! (pair)" << ::std::endl;

			exit (-1);
		}
	}

	nPairKey = 0;

	sCItPairLower = pPairContainer->lower_bound (nPairKey);

	if (sCItPairLower != pPairContainer->cbegin ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: lower_bound () didn't return cbegin () when testing for lowest key! (pair)" << ::std::endl;

		exit (-1);
	}

	nPairKey = (typename _t_pair_container::key_type) nNumEntries + 1;

	sCItPairUpper = pPairContainer->upper_bound (nPairKey);

	if (sCItPairUpper != pPairContainer->cend ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: upper_bound () didn't return cend () when testing for lowest key! (pair)" << ::std::endl;

		exit (-1);
	}
}

template<class _t_container, class _t_pair_container>
void TestBTreeKeySortCCsetIterData (_t_container *pContainer, _t_pair_container *pPairContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::const_iterator		citer_t;
	typedef typename _t_container::value_type			value_type;
	typedef typename _t_container::key_type				key_type;
	typedef typename _t_container::size_type			size_type;

	typedef typename _t_pair_container::const_iterator	citer_pair_t;
	typedef typename _t_pair_container::value_type		value_pair_type;
	typedef typename _t_pair_container::key_type		key_pair_type;
	typedef typename _t_pair_container::size_type		size_pair_type;

	key_type								nLastKey = 0;
	key_pair_type							nLastPairKey = 0;
	citer_t									sCIter;
	citer_pair_t							sCItPair;
	size_type								i;
	size_pair_type							ip;

	::std::cout << "this test exercises method CBTreeKeysort<>::set_iter_data ()" << ::std::endl;

	associative_container_add_primitive (pContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	for (i = 0; i < pContainer->size (); i++)
	{
		get_begin (pContainer, sCIter);

		::std::advance (sCIter, i);

		value_type			sValue (*sCIter);

		set_entry_data (sValue, ~get_entry_data (sValue));

		if ((i & 0x1) != 0x0)
		{
			set_entry_key (sValue, ~get_entry_key (sValue));
		}

		pContainer->set_iter_data (sCIter, sValue);
	}

	associative_container_add_primitive (pPairContainer, nNumEntries, nLastPairKey, BTREETEST_KEY_GENERATION_RANDOM);

	for (ip = 0; ip < pPairContainer->size (); ip++)
	{
		get_begin (pPairContainer, sCItPair);

		::std::advance (sCItPair, ip);

		value_pair_type			sPairValue (*sCItPair);

		set_entry_data (sPairValue, ~get_entry_data (sPairValue));

		if ((ip & 0x1) != 0x0)
		{
			set_entry_key (sPairValue, ~get_entry_key (sPairValue));
		}

		pPairContainer->set_iter_data (sCItPair, sPairValue);
	}
}

template<class _t_container, class _t_pair_container>
void TestBTreeKeySortSTLifEraseViaIterator (_t_container *pContainer, _t_pair_container *pPairContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::const_iterator					citer_t;

	typedef typename _t_container::size_type						size_type;
	typedef typename _t_container::key_type							key_type;

	typedef typename _t_pair_container::const_iterator				citer_pair_t;

	typedef typename _t_pair_container::size_type					size_type_pair;

	key_type									nLastKey = 0;
	citer_t										sCIter;
	citer_t										sCIterRslt;
	size_type									nIter;
	size_type									nIterRslt;
	uint32_t									i;
	citer_pair_t								sCIterPair;
	citer_pair_t								sCIterPairRslt;
	size_type_pair								nIterPair;
	size_type_pair								nIterPairRslt;
	size_type_pair								nNumEntriesPair = (size_type_pair) nNumEntries;

	::std::cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeySort<>:: erase (const_iterator)" << ::std::endl;

	associative_container_add_primitive (pContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	while (pContainer->size () > 0)
	{
		sCIter = pContainer->cbegin ();

		::std::advance (sCIter, (int) (generate_rand32 () % pContainer->size ()));

		nIter = ::std::distance (pContainer->cbegin (), sCIter);

		sCIterRslt = pContainer->erase (sCIter);

		nIterRslt = std::distance (pContainer->cbegin (), sCIterRslt);

		if (nIter != nIterRslt)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeKeySortSTLifEraseViaIterator: returned iterator mismatches!" << ::std::endl;

			exit (-1);
		}
	}

	for (i = 0; i < nNumEntries; i++)
	{
		keySortMap_t 		sKeySortMap = {generate_rand32 (), g_nDebug};
		keySortPair_t		sKeySortPairData (generate_rand32 (), sKeySortMap);

		g_nDebug++;

		pPairContainer->insert (sKeySortPairData);

		::std::cout << i << " / " << nNumEntries << "\r";
	}

	::std::cout << i << " / " << nNumEntries << ::std::endl;

	while (pPairContainer->size () > 0)
	{
		sCIterPair = pPairContainer->cbegin ();

		::std::advance (sCIterPair, (int) (generate_rand32 () % pPairContainer->size ()));

		nIterPair = ::std::distance (pPairContainer->cbegin (), sCIterPair);

		sCIterPairRslt = pPairContainer->erase (sCIterPair);

		nIterPairRslt = ::std::distance (pPairContainer->cbegin (), sCIterPairRslt);

		if (nIterPair != nIterPairRslt)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeKeySortSTLifEraseViaIterator: returned iterator (pair type) mismatches!" << ::std::endl;

			exit (-1);
		}
	}
}

template<class _t_container, class _t_pair_container>
void TestBTreeKeySortSTLifEraseViaKey (_t_container *pContainer, _t_pair_container *pPairContainer, typename _t_container::size_type nNumEntries, uint32_t nInstances)
{
	typedef typename _t_container::const_iterator			citer_t;

	typedef typename _t_container::size_type				size_type;
	typedef typename _t_container::key_type					key_type;

	typedef typename _t_pair_container::const_iterator		citer_pair_t;

	typedef typename _t_pair_container::key_type			key_type_pair;

	key_type									nLastKey;
	citer_t										sCIter;
	uint32_t									i;
	size_type									nRslt;
	uint32_t									j;
	citer_pair_t								sCIterPair;
	key_type									nKey;
	key_type_pair								nKeyPair;

	::std::cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeySort<>:: erase (const _t_key)" << ::std::endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		associative_container_add_primitive (pContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);
	}

	while (pContainer->size () > 0)
	{
		sCIter = pContainer->cbegin ();

		::std::advance (sCIter, (int) (generate_rand32 () % pContainer->size ()));

		nKey = (*sCIter).first;

		nRslt = pContainer->erase (nKey);

		if (nRslt != nInstances)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeKeySortSTLifEraseViaKey: unexpected number of items deleted!" << ::std::endl;

			exit (-1);
		}
	}

	for (i = 0; i < nNumEntries; i++)
	{
		nKeyPair = generate_rand32 ();

		for (j = 0; j < nInstances; j++)
		{
			keySortMap_t					sKeySortMap = {generate_rand32 (), g_nDebug};
			keySortPair_t					sKeySortPairData (nKeyPair, sKeySortMap);

			g_nDebug++;

			pPairContainer->insert (sKeySortPairData);
		}

		::std::cout << i << " / " << nNumEntries << "\r";
	}

	::std::cout << i << " / " << nNumEntries << ::std::endl;

	while (pPairContainer->size () > 0)
	{
		sCIterPair = pPairContainer->cbegin ();

		::std::advance (sCIterPair, (int) (generate_rand32 () % pPairContainer->size ()));

		nKeyPair = (*sCIterPair).first;

		nRslt = pPairContainer->erase (nKeyPair);

		if (nRslt != nInstances)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeKeySortSTLifEraseViaKey: unexpected number of items deleted! (pair)" << ::std::endl;

			exit (-1);
		}
	}
}

template<class _t_container, class _t_pair_container>
void TestBTreeKeySortSTLifEraseViaIteratorMultiple (_t_container *pContainer, _t_pair_container *pPairContainer, typename _t_container::size_type nNumEntries, uint32_t nInstances)
{
	typedef typename _t_container::const_iterator				citer_t;

	typedef typename _t_container::size_type					size_type;
	typedef typename _t_container::key_type						key_type;

	typedef typename _t_pair_container::const_iterator			citer_pair_t;

	typedef typename _t_pair_container::size_type				size_type_pair;
	typedef typename _t_pair_container::key_type				key_type_pair;

	uint32_t									nDebug = 0;
	key_type									nLastKey;
	citer_t										sCIterBegin;
	citer_t										sCIterEnd;
	citer_t										sCIterRslt;
	size_type									nIterBegin;
	size_type									nIterRslt;
	uint32_t									i;
	uint32_t									j;
	citer_pair_t								sCIterPairBegin;
	citer_pair_t								sCIterPairEnd;
	citer_pair_t								sCIterPairRslt;
	size_type_pair								nIterPairBegin;
	size_type_pair								nIterPairRslt;
	key_type_pair								nKeyPair;
	size_type_pair								nNumEntriesPair = (size_type_pair) nNumEntries;

	::std::cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeySort<>:: erase (const_iterator, const_iterator)" << ::std::endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		associative_container_add_primitive (pContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);
	}

	while (pContainer->size () > 0)
	{
		sCIterBegin = pContainer->cbegin ();
		sCIterEnd = pContainer->cend ();

		if (pContainer->size () > 1)
		{
			::std::advance (sCIterBegin, (int) (generate_rand32 () % (pContainer->size () / 2)));
			::std::advance (sCIterEnd, 0 - ((int) (generate_rand32 () % (pContainer->size () / 2))));
		}

		nIterBegin = ::std::distance (pContainer->cbegin (), sCIterBegin);

		sCIterRslt = pContainer->erase (sCIterBegin, sCIterEnd);

		nIterRslt = ::std::distance (pContainer->cbegin (), sCIterRslt);

		if (nIterRslt != nIterBegin)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeKeySortSTLifEraseViaIteratorMultiple: returned iterator mismatches!" << ::std::endl;

			exit (-1);
		}
	}

	for (i = 0; i < nNumEntries; i++)
	{
		nKeyPair = generate_rand32 ();

		for (j = 0; j < nInstances; j++)
		{
			keySortMap_t				sKeySortMap = {generate_rand32 (), g_nDebug};
			keySortPair_t				sKeySortPairData (nKeyPair, sKeySortMap);

			g_nDebug++;

			pPairContainer->insert (sKeySortPairData);
		}

		::std::cout << i << " / " << nNumEntries << "\r";
	}

	::std::cout << i << " / " << nNumEntries << ::std::endl;

	while (pPairContainer->size () > 0)
	{
		sCIterPairBegin = pPairContainer->cbegin ();
		sCIterPairEnd = pPairContainer->cend ();

		::std::advance (sCIterPairBegin, (int) (generate_rand32 () % (pPairContainer->size () / 2)));
		::std::advance (sCIterPairEnd, 0 - (int) (generate_rand32 () % (pPairContainer->size () / 2)));

		nIterPairBegin = ::std::distance (pPairContainer->cbegin (), sCIterPairBegin);

		sCIterPairRslt = pPairContainer->erase (sCIterPairBegin, sCIterPairEnd);

		nIterPairRslt = ::std::distance (pPairContainer->cbegin (), sCIterPairRslt);

		if (nIterPairRslt != nIterPairBegin)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "TestBTreeKeySortSTLifEraseViaKey: returned iterator (pair type) mismatches!" << ::std::endl;

			exit (-1);
		}
	}
}

template<class _t_container>
void TestBTreeKeySortCCoperatorOverloadCompare (_t_container *pContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::iterator						iter_t;
	typedef typename _t_container::const_iterator				citer_t;

	typedef typename _t_container::size_type					size_type;
	typedef typename _t_container::key_type						key_type;
	typedef typename _t_container::value_type					value_type;

	_t_container		sCntContainer (*pContainer);
	key_type			nLastKey = 0;
	citer_t				sCIterHalf;
	citer_t				sCIterEnd;

	associative_container_add_primitive (pContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	if (!pContainer->compare_individual_containers (*pContainer))
	{
		::std::cerr << "TestBTreeKeySortCCoperatorOverloadCompare: ERROR: Unexpected result! container A not equal to container A" << ::std::endl;

		exit (-1);
	}

	if (!sCntContainer.compare_individual_containers (sCntContainer))
	{
		::std::cerr << "TestBTreeKeySortCCoperatorOverloadCompare: ERROR: Unexpected result! empty container B not equal to empty container B" << ::std::endl;

		exit (-1);
	}

	if (pContainer->compare_individual_containers (sCntContainer))
	{
		::std::cerr << "TestBTreeKeySortCCoperatorOverloadCompare: ERROR: Unexpected result! container A equal to empty container B" << ::std::endl;

		exit (-1);
	}

	if (sCntContainer.compare_individual_containers (*pContainer))
	{
		::std::cerr << "TestBTreeKeySortCCoperatorOverloadCompare: ERROR: Unexpected result! empty container B equal to container A" << ::std::endl;

		exit (-1);
	}

	nLastKey = 2;

	associative_container_add_primitive (&sCntContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	if (pContainer->compare_individual_containers (sCntContainer))
	{
		::std::cerr << "TestBTreeKeySortCCoperatorOverloadCompare: ERROR: Unexpected result! container A equal to container B" << ::std::endl;

		exit (-1);
	}

	if (sCntContainer.compare_individual_containers (*pContainer))
	{
		::std::cerr << "TestBTreeKeySortCCoperatorOverloadCompare: ERROR: Unexpected result! container B equal to container A" << ::std::endl;

		exit (-1);
	}

	sCntContainer.clear ();

	nLastKey = 0;

	associative_container_add_primitive (&sCntContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	if (!pContainer->compare_individual_containers (sCntContainer))
	{
		::std::cerr << "TestBTreeKeySortCCoperatorOverloadCompare: ERROR: Unexpected result! container A containing the same data as container B not equal to container B containing the same data as container A" << ::std::endl;

		exit (-1);
	}

	if (!sCntContainer.compare_individual_containers (*pContainer))
	{
		::std::cerr << "TestBTreeKeySortCCoperatorOverloadCompare: ERROR: Unexpected result! container B containing the same data as container A not equal to container A containing the same data as container B" << ::std::endl;

		exit (-1);
	}

	get_begin (&sCntContainer, sCIterHalf);
	get_end (&sCntContainer, sCIterEnd);

	::std::advance (sCIterHalf, nNumEntries / 2);

	sCntContainer.erase (sCIterHalf, sCIterEnd);

	if (pContainer->compare_individual_containers (sCntContainer))
	{
		::std::cerr << "TestBTreeKeySortCCoperatorOverloadCompare: ERROR: Unexpected result! container A containing the same data as container B and more not equal to container B containing the first half data of container A" << ::std::endl;

		exit (-1);
	}

	if (sCntContainer.compare_individual_containers (*pContainer))
	{
		::std::cerr << "TestBTreeKeySortCCoperatorOverloadCompare: ERROR: Unexpected result! container B containing the first half data of container A not equal to container A containing the same data as container B and more" << ::std::endl;

		exit (-1);
	}

	associative_container_add_primitive (pContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	{
		sCntContainer = *pContainer;

		iter_t			sIter;

		get_begin (&sCntContainer, sIter);

		size_type		nSteps = generate_rand32 () % (nNumEntries * 2);

		::std::advance (sIter, nSteps);

		value_type		sValue = *sIter;
		key_type		sKey = get_entry_key (sValue);

		sKey = ~sKey;

		set_entry_key (sValue, sKey);

		sCntContainer.erase (sIter);

		sCntContainer.insert (sValue);

		if (pContainer->compare_individual_containers (sCntContainer))
		{
			::std::cerr << "TestBTreeKeySortCCoperatorOverloadCompare: ERROR: Unexpected result! container A with one or two different key sets different from container B equal to container B" << ::std::endl;

			exit (-1);
		}
	}

	{
		sCntContainer = *pContainer;

		iter_t			sIter;

		get_begin (&sCntContainer, sIter);

		::std::advance (sIter, generate_rand32 () % (nNumEntries * 2));

		value_type		sValue = *sIter;

		set_entry_data (sValue, ~get_entry_data (sValue));

		sCntContainer.erase (sIter);

		sCntContainer.insert (sValue);

		if (pContainer->compare_individual_containers (sCntContainer))
		{
			::std::cerr << "TestBTreeKeySortCCoperatorOverloadCompare: ERROR: Unexpected result! container A with one different data entry different from container B equal to container B" << ::std::endl;

			exit (-1);
		}
	}

	pContainer->clear ();
	sCntContainer.clear ();

	nLastKey = 0;

	associative_container_add_primitive (pContainer, 64, nLastKey, BTREETEST_KEY_GENERATION_CONST);

	sCntContainer = *pContainer;

	if (!pContainer->compare_individual_containers (sCntContainer))
	{
		::std::cerr << "TestBTreeKeySortCCoperatorOverloadCompare: ERROR: Unexpected result! container A not equal to container B - with container A and B each containing a large key set" << ::std::endl;

		exit (-1);
	}

	{
		iter_t			sIter;

		get_begin (&sCntContainer, sIter);

		::std::advance (sIter, generate_rand32 () % sCntContainer.size ());

		value_type		sValue = *sIter;

		set_entry_data (sValue, ~get_entry_data (sValue));

		sCntContainer.erase (sIter);

		sCntContainer.insert (sValue);

		if (pContainer->compare_individual_containers (sCntContainer))
		{
			::std::cerr << "TestBTreeKeySortCCoperatorOverloadCompare: ERROR: Unexpected result! container A equal to container B - with container A and B each containing a large key set and one data entry different" << ::std::endl;

			exit (-1);
		}
	}

	pContainer->clear ();
	sCntContainer.clear ();
}

template<class _t_container, class _t_pair_container>
void TestBTreeKeySort (uint32_t nTest, uint32_t nNodeSize, uint32_t nPageSize, _t_container *pKeySortTestWrapper, _t_pair_container *pKeySortPairTestWrapper)
{
	typename _t_container::size_test_type		sTypeSelect;
	::std::string								sTypeStr;

	get_typename (sTypeSelect, sTypeStr);

	::std::cout << "b-tree keysort test bench selected using type " << sTypeStr << ::std::endl;

	pKeySortTestWrapper = new _t_container (nNodeSize, nPageSize);

	if (pKeySortTestWrapper == NULL)
	{
		::std::cerr << "insufficient memory!" << ::std::endl;

		exit (-1);
	}

	pKeySortPairTestWrapper = new _t_pair_container (nNodeSize, nPageSize);

	if (pKeySortPairTestWrapper == NULL)
	{
		::std::cerr << "insufficient memory!" << ::std::endl;

		exit (-1);
	}

	switch (nTest)
	{
	case BTREETEST_KEYSORT_BASIC_ASCEND			:
		{
			TestBTreeKeySortBasic (pKeySortTestWrapper, 512, 256, 3, BTREETEST_KEY_GENERATION_ASCEND, BTREETEST_KEY_GENERATION_ASCEND);
			TestBTreeKeySortBasic (pKeySortPairTestWrapper, 512, 256, 3, BTREETEST_KEY_GENERATION_ASCEND, BTREETEST_KEY_GENERATION_ASCEND);

			break;
		}

	case BTREETEST_KEYSORT_BASIC_ASCEND_SMALL	:
		{
			TestBTreeKeySortBasic (pKeySortTestWrapper, 16, 0, 0, BTREETEST_KEY_GENERATION_ASCEND, BTREETEST_KEY_GENERATION_ASCEND);
			TestBTreeKeySortBasic (pKeySortPairTestWrapper, 16, 0, 0, BTREETEST_KEY_GENERATION_ASCEND, BTREETEST_KEY_GENERATION_ASCEND);

			break;
		}

	case BTREETEST_KEYSORT_BASIC_DESCEND			:
		{
			TestBTreeKeySortBasic (pKeySortTestWrapper, 512, 256, 3, BTREETEST_KEY_GENERATION_DESCEND, BTREETEST_KEY_GENERATION_DESCEND);
			TestBTreeKeySortBasic (pKeySortPairTestWrapper, 512, 256, 3, BTREETEST_KEY_GENERATION_DESCEND, BTREETEST_KEY_GENERATION_DESCEND);

			break;
		}

	case BTREETEST_KEYSORT_BASIC_DESCEND_SMALL	:
		{
			TestBTreeKeySortBasic (pKeySortTestWrapper, 16, 0, 0, BTREETEST_KEY_GENERATION_DESCEND, BTREETEST_KEY_GENERATION_DESCEND);
			TestBTreeKeySortBasic (pKeySortPairTestWrapper, 16, 0, 0, BTREETEST_KEY_GENERATION_DESCEND, BTREETEST_KEY_GENERATION_DESCEND);

			break;
		}

	case BTREETEST_KEYSORT_BASIC_RANDOM		:
		{
			TestBTreeKeySortBasic (pKeySortTestWrapper, 512, 256, 3, BTREETEST_KEY_GENERATION_RANDOM, BTREETEST_KEY_GENERATION_RANDOM);
			TestBTreeKeySortBasic (pKeySortPairTestWrapper, 512, 256, 3, BTREETEST_KEY_GENERATION_RANDOM, BTREETEST_KEY_GENERATION_RANDOM);

			break;
		}

	case BTREETEST_KEYSORT_BASIC_RANDOM_SMALL	:
		{
			TestBTreeKeySortBasic (pKeySortTestWrapper, 16, 0, 0, BTREETEST_KEY_GENERATION_RANDOM, BTREETEST_KEY_GENERATION_RANDOM);
			TestBTreeKeySortBasic (pKeySortPairTestWrapper, 16, 0, 0, BTREETEST_KEY_GENERATION_RANDOM, BTREETEST_KEY_GENERATION_RANDOM);

			break;
		}

	case BTREETEST_KEYSORT_CLEAR			:
		{
			TestBTreeKeySortRemoveAll (pKeySortTestWrapper, 256);
			TestBTreeKeySortRemoveAll (pKeySortPairTestWrapper, 256);

			break;
		}

	case BTREETEST_KEYSORT_REPLACE			:
		{
			TestBTreeKeySortMultiRemove (pKeySortTestWrapper, 64);
			TestBTreeKeySortMultiRemove (pKeySortPairTestWrapper, 64);

			break;
		}

	case BTREETEST_KEYSORT_UNLOAD				:
		{
			TestBTreeKeySortUnload (pKeySortTestWrapper, 128, 128, 128, 128);
			TestBTreeKeySortUnload (pKeySortPairTestWrapper, 128, 128, 128, 128);

			break;
		}

	case BTREETEST_KEYSORT_REMOVE_INSTANCES		:
		{
			TestBTreeKeySortRemoveInstance (pKeySortTestWrapper, 64, 3);
			TestBTreeKeySortRemoveInstance (pKeySortPairTestWrapper, 64, 3);

			break;
		}

	case BTREETEST_KEYSORT_HTML_OUTPUT						:
		{
			TestBTreeKeySortHTMLoutput (pKeySortTestWrapper, 64);
			TestBTreeKeySortHTMLoutput (pKeySortPairTestWrapper, 64);

			break;
		}

	case BTREETEST_KEYSORT_COPY_CONSTRUCTOR					:
		{
			TestBTreeKeySortCopyConstructor (pKeySortTestWrapper, 64);
			TestBTreeKeySortCopyConstructor (pKeySortPairTestWrapper, 64);

			break;
		}

	case BTREETEST_KEYSORT_OPERATOR_OVERLOAD_ASSIGN			:
		{
			TestBTreeKeySortOperatorOverloadAssign (pKeySortTestWrapper, 64);
			TestBTreeKeySortOperatorOverloadAssign (pKeySortPairTestWrapper, 64);

			break;
		}

	case BTREETEST_KEYSORT_OPERATOR_OVERLOAD_ASSIGN_MULTI_INSTANCE	:
		{
			TestBTreeKeySortOperatorOverloadAssignMultiInstance (pKeySortTestWrapper, 64, 32, 5);
			TestBTreeKeySortOperatorOverloadAssignMultiInstance (pKeySortPairTestWrapper, 64, 32, 5);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_ALL					:
		{
			TestBTreeKeySortSerialize (pKeySortTestWrapper, 1, 128, 128);
			TestBTreeKeySortSerialize (pKeySortPairTestWrapper, 1, 128, 128);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_HALF_NODE				:
		{
			TestBTreeKeySortSerialize (pKeySortTestWrapper, 1, 128, 64);
			TestBTreeKeySortSerialize (pKeySortPairTestWrapper, 1, 128, 64);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_PART_NODE				:
		{
			TestBTreeKeySortSerialize (pKeySortTestWrapper, 1, 128, nNodeSize - 1);
			TestBTreeKeySortSerialize (pKeySortPairTestWrapper, 1, 128, nNodeSize - 1);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_INCOMPLETE				:
		{
			TestBTreeKeySortSerializeIncomplete (pKeySortTestWrapper, nNodeSize, 128, 128, 64);
			TestBTreeKeySortSerializeIncomplete (pKeySortPairTestWrapper, nNodeSize, 128, 128, 64);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_MULTI_INSTANCE_ALL		:
		{
			TestBTreeKeySortSerialize (pKeySortTestWrapper, 2, 64, 128);
			TestBTreeKeySortSerialize (pKeySortPairTestWrapper, 2, 64, 128);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_MULTI_INSTANCE_HALF_NODE	:
		{
			TestBTreeKeySortSerialize (pKeySortTestWrapper, 2, 64, 64);
			TestBTreeKeySortSerialize (pKeySortPairTestWrapper, 2, 64, 64);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_MULTI_INSTANCE_PART_NODE	:
		{
			TestBTreeKeySortSerialize (pKeySortTestWrapper, 2, 64, nNodeSize - 1);
			TestBTreeKeySortSerialize (pKeySortPairTestWrapper, 2, 64, nNodeSize - 1);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_INSERT:
		{
			TestBTreeKeySortSTLifInsert (pKeySortTestWrapper, 64);
			TestBTreeKeySortSTLifInsert (pKeySortPairTestWrapper, 64);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR	:
		{
			bayerTreeCacheDescription_t	sCacheDesc = {nPageSize};

			TestBTreeKeySortSTLifInsertViaIterator (pKeySortTestWrapper, pKeySortPairTestWrapper, 64, &sCacheDesc, nNodeSize);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_ERASE_VIA_ITERATOR	:
		{
			TestBTreeKeySortSTLifEraseViaIterator (pKeySortTestWrapper, pKeySortPairTestWrapper, 64);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_ERASE_VIA_KEY:
		{
			TestBTreeKeySortSTLifEraseViaKey (pKeySortTestWrapper, pKeySortPairTestWrapper, 64, 5);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE	:
		{
			TestBTreeKeySortSTLifEraseViaIteratorMultiple (pKeySortTestWrapper, pKeySortPairTestWrapper, 64, 5);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_SWAP	:
		{
			TestBTreeKeySortSTLifSwap (pKeySortTestWrapper, pKeySortPairTestWrapper, 128);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_FIND	:
		{
			TestBTreeKeySortSTLifFind (pKeySortTestWrapper, pKeySortPairTestWrapper, 128);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_LOWER_BOUND_UPPER_BOUND	:
		{
			TestBTreeKeySortSTLifLowerBoundUpperBound (pKeySortTestWrapper, pKeySortPairTestWrapper, 64, 5);

			break;
		}

	case BTREETEST_KEYSORT_CODE_COVERAGE_DETERMINE_POSITION	:
		{
			TestBTreeKeySortCCdeterminePosition (pKeySortTestWrapper, nNodeSize);
			TestBTreeKeySortCCdeterminePosition (pKeySortPairTestWrapper, nNodeSize);

			break;
		}

	case BTREETEST_KEYSORT_CODE_COVERAGE_FIND_FIRST_KEY		:
		{
			TestBTreeKeySortCCfindFirstKey (pKeySortTestWrapper, nNodeSize);
			TestBTreeKeySortCCfindFirstKey (pKeySortPairTestWrapper, nNodeSize);

			break;
		}

	case BTREETEST_KEYSORT_CODE_COVERAGE_GET_INIT_POS_OF_KEY_ON_LEAF_NODE	:
		{
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize * 2);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize * 3);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize * 4);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize * 5);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize * 6);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize * 7);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize * 8);

			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize * 2);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize * 3);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize * 4);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize * 5);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize * 6);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize * 7);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize * 8);

			break;
		}

	case BTREETEST_KEYSORT_CODE_COVERAGE_SET_ITER_DATA	:
		{
			TestBTreeKeySortCCsetIterData (pKeySortTestWrapper, pKeySortPairTestWrapper, 16);

			break;
		}

	case BTREETEST_KEYSORT_CODE_COVERAGE_OPERATOR_OVERLOAD_COMPARE:
		{
			TestBTreeKeySortCCoperatorOverloadCompare (pKeySortTestWrapper, 16);
			TestBTreeKeySortCCoperatorOverloadCompare (pKeySortPairTestWrapper, 16);

			break;
		}

	default									:
		{
			::std::cerr << "ERROR: TestBTreeKeySort: Unknown test or test not specified!" << ::std::endl;

			exit (-1);

			break;
		}
	}

	delete pKeySortTestWrapper;
	delete pKeySortPairTestWrapper;
}
