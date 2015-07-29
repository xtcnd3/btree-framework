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

template <class _t_multiset>
void TestBTreeMultiSetSTLifAssignmentOperator (_t_multiset *pClMSet, uint32_t nNumEntries)
{
	typedef typename _t_multiset::const_iterator						citer_t;

	_t_multiset									sClMSet (*pClMSet, false);
	uint32_t									nLastKey = 0;
	citer_t										sIter;
	citer_t										sIterRslt;
	
	cout << "exercises assignment operator compatible to STL interface CBTreeMultiSet<>:: operator= (CBTreeMultiSet<> &)" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sClMSet = *pClMSet;

	if (sClMSet != (*pClMSet))
	{
		cerr << endl;
		cerr << "TestBTreeMultiSetSTLifAssignmentOperator<>: ERROR: mismatch after assignment operation" << endl;

		exit (-1);
	}
}

template <class _t_multiset>
void TestBTreeMultiSetSTLifInsert (_t_multiset *pClMSet, uint32_t nNumEntries)
{
	uint32_t	nLastSeed = 0;

	cout << "CBTreeMultiSet::insert (const valuetype &) is exercised" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastSeed, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
}

template <class _t_multiset>
void TestBTreeMultiSetSTLifInsertViaIterator (_t_multiset *pClMSet, uint32_t nNumEntries, typename _t_multiset::datalayerproperties_t &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nNodeSize)
{
	typedef typename _t_multiset::value_t												value_t;

	typedef typename _t_multiset::iterator												iter_t;
	typedef typename _t_multiset::const_iterator										citer_t;
	typedef typename _t_multiset::reverse_iterator										riter_t;
	typedef typename _t_multiset::const_reverse_iterator								criter_t;

	typedef typename ::std::list<value_t>::iterator										itlist_t;
	typedef typename ::std::list<value_t>::const_iterator								citlist_t;
	typedef typename ::std::list<value_t>::reverse_iterator								ritlist_t;
	typedef typename ::std::list<value_t>::const_reverse_iterator						critlist_t;

	typedef typename ::std::vector<value_t>::iterator									itvec_t;
	typedef typename ::std::vector<value_t>::const_iterator								citvec_t;
	typedef typename ::std::vector<value_t>::reverse_iterator							ritvec_t;
	typedef typename ::std::vector<value_t>::const_reverse_iterator						critvec_t;

	typedef CBTreeMultiSet<uint32_t, size_t>											CBTreeMultiSet_VectorFeed_t;

	typedef CBTreeTestMultiSet<typename _t_multiset::sizetype_t, typename _t_multiset::nodeiter_t, typename _t_multiset::subnodeiter_t, typename _t_multiset::datalayerproperties_t, typename _t_multiset::datalayer_t>
																						CBTreeMultiSetTest_Pair_t;

	typedef typename CBTreeMultiSetTest_Pair_t::iterator								iter_pair_t;
	typedef typename CBTreeMultiSetTest_Pair_t::const_iterator							citer_pair_t;
	typedef typename CBTreeMultiSetTest_Pair_t::reverse_iterator						riter_pair_t;
	typedef typename CBTreeMultiSetTest_Pair_t::const_reverse_iterator					criter_pair_t;

	typedef typename ::std::set<uint32_t>::iterator										itset_t;
	typedef typename ::std::set<uint32_t>::const_iterator								citset_t;
	typedef typename ::std::set<uint32_t>::reverse_iterator								ritset_t;
	typedef typename ::std::set<uint32_t>::const_reverse_iterator						critset_t;

	typedef typename ::std::multiset<uint32_t>::iterator								itmset_t;
	typedef typename ::std::multiset<uint32_t>::const_iterator							citmset_t;
	typedef typename ::std::multiset<uint32_t>::reverse_iterator						ritmset_t;
	typedef typename ::std::multiset<uint32_t>::const_reverse_iterator					critmset_t;
	
	_t_multiset									sClMSetTarget (*pClMSet, false);
	uint32_t									nLastKey = 0;
	citer_t										sCIterA, sCIterB;
	criter_t									sCRIterA, sCRIterB;
	::std::list<value_t>						sList;
	::std::vector<value_t>						sVector;
	itlist_t									sItListA, sItListB;
	citlist_t									sCItListA, sCItListB;
	ritlist_t									sRItListA, sRItListB;
	critlist_t									sCRItListA, sCRItListB;
	itvec_t										sItVecA, sItVecB;
	citvec_t									sCItVecA, sCItVecB;
	ritvec_t									sRItVecA, sRItVecB;
	critvec_t									sCRItVecA, sCRItVecB;
	CBTreeIOpropertiesRAM						sRAMproperties;
	CBTreeMultiSet_VectorFeed_t					sMultiSetVectorFeed (sRAMproperties, psCacheDescription, nNodeSize);
	CBTreeMultiSetTest_Pair_t					sMultiSetPair (rDataLayerProperties, psCacheDescription, nNodeSize);
	CBTreeMultiSetTest_Pair_t					sMultiSetTargetPair (sMultiSetPair);
//	value_t										sMultiSetPairData;
	citer_pair_t								sCIterPairA, sCIterPairB;
	criter_pair_t								sCRIterPairA, sCRIterPairB;
	::std::set<uint32_t>						sSet;
	itset_t										sItSetA, sItSetB;
	citset_t									sCItSetA, sCItSetB;
	ritset_t									sRItSetA, sRItSetB;
	critset_t									sCRItSetA, sCRItSetB;
	::std::multiset<uint32_t>					sMSet;
	itmset_t									sItMSetA, sItMSetB;
	citmset_t									sCItMSetA, sCItMSetB;
	ritmset_t									sRItMSetA, sRItMSetB;
	critmset_t									sCRItMSetA, sCRItMSetB;
	
	cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: insert<_t_iterator> (_t_iterator, _t_iterator)" << endl;

#if 0 // for this code to compile, the b-tree framework under test must support const methods
// this is to be implemented

	cout << "target::insert<_t_obj::iter_t> (iter_t, iter_t)" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sClMSetTarget.template insert <iter_t> (pClMSet->begin (), pClMSet->end ());

	pClMSet->clear ();
	sClMSetTarget.clear ();

#endif

	cout << "target::insert<_t_obj::citer_t> (citer_t, citer_t)" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sClMSetTarget.template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	pClMSet->clear ();
	sClMSetTarget.clear ();

#if 0 // for this code to compile, the b-tree framework under test must support const methods
// this is to be implemented

	cout << "target::insert<_t_obj::riter_t> (riter_t, riter_t)" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sClMSetTarget.template insert <riter_t> (pClMSet->rbegin (), pClMSet->rend ());

	pClMSet->clear ();
	sClMSetTarget.clear ();

#endif

	cout << "target::insert<_t_obj::criter_t> (criter_t, criter_t)" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sClMSetTarget.template insert <criter_t> (pClMSet->crbegin (), pClMSet->crend ());

	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<_t_obj::citer_t> (citer_t == citer_t)" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sCIterA = pClMSet->cbegin ();
	sCIterB = pClMSet->cbegin ();

	while (sCIterA != pClMSet->cend ())
	{
		sClMSetTarget.template insert <citer_t> (sCIterA, sCIterB);

		sCIterA++;
		sCIterB++;
	}

	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<_t_obj::criter_t> (criter_t == criter_t)" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sCRIterA = pClMSet->crbegin ();
	sCRIterB = pClMSet->crbegin ();

	while (sCRIterA != pClMSet->crend ())
	{
		sClMSetTarget.template insert <criter_t> (sCRIterA, sCRIterB);

		sCRIterA++;
		sCRIterB++;
	}

	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<_t_obj::citer_t> (>citer_t, citer_t<)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sCIterA = pClMSet->cbegin ();
	sCIterB = pClMSet->cend ();

	sCIterA += (int) (nNumEntries / 4);
	sCIterB -= (int) (nNumEntries / 4);

	sClMSetTarget.template insert <citer_t> (sCIterA, sCIterB);

	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<_t_obj::criter_t> (>criter_t, criter_t<)" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sCRIterA = pClMSet->crbegin ();
	sCRIterB = pClMSet->crend ();

	sCRIterA += (int) (nNumEntries / 4);
	sCRIterB -= (int) (nNumEntries / 4);

	sClMSetTarget.template insert <criter_t> (sCRIterA, sCRIterB);

	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<_t_obj::citer_t> (target::citer_t, target::citer_t)" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	pClMSet->template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	pClMSet->clear ();

	cout << "target::insert<_t_obj::criter_t> (target::criter_t, target::criter_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	pClMSet->template insert <criter_t> (pClMSet->crbegin (), pClMSet->crend ());

	pClMSet->clear ();

	cout << "target::insert<_t_obj::citer_t> (target::citer_t == target::citer_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sCIterA = pClMSet->cbegin ();
	sCIterB = pClMSet->cbegin ();

	while (sCIterA != pClMSet->cend ())
	{
		pClMSet->template insert <citer_t> (sCIterA, sCIterB);

		sCIterA++;
		sCIterB++;
	}

	pClMSet->clear ();

	cout << "target::insert<_t_obj::criter_t> (target::criter_t == target::criter_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sCRIterA = pClMSet->crbegin ();
	sCRIterB = pClMSet->crbegin ();

	while (sCRIterA != pClMSet->crend ())
	{
		pClMSet->template insert <criter_t> (sCRIterA, sCRIterB);

		sCRIterA++;
		sCRIterB++;
	}

	pClMSet->clear ();

	cout << "target::insert<_t_obj::citer_t> (>target::citer_t, target::citer_t<)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	pClMSet->template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());
	pClMSet->template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());
	pClMSet->template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());
	pClMSet->template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	sCIterA = pClMSet->cbegin ();
	sCIterB = pClMSet->cend ();

	sCIterA += (int) (nNumEntries / 4);
	sCIterB -= (int) (nNumEntries / 4);

	pClMSet->template insert <citer_t> (sCIterA, sCIterB);

	pClMSet->clear ();

	cout << "target::insert<_t_obj::criter_t> (>target::criter_t, target::criter_t<)" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	pClMSet->template insert <criter_t> (pClMSet->crbegin (), pClMSet->crend ());
	pClMSet->template insert <criter_t> (pClMSet->crbegin (), pClMSet->crend ());
	pClMSet->template insert <criter_t> (pClMSet->crbegin (), pClMSet->crend ());
	pClMSet->template insert <criter_t> (pClMSet->crbegin (), pClMSet->crend ());

	sCRIterA = pClMSet->crbegin ();
	sCRIterB = pClMSet->crend ();

	sCRIterA += (int) (nNumEntries / 4);
	sCRIterB -= (int) (nNumEntries / 4);

	pClMSet->template insert <criter_t> (sCRIterA, sCRIterB);

	pClMSet->clear ();

	cout << "target::insert<list::iter_t> (iter_t, iter_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMSet->cbegin (), pClMSet->cend ());

	sClMSetTarget.template insert <itlist_t> (sList.begin (), sList.end ());

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<list::citer_t> (citer_t, citer_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMSet->cbegin (), pClMSet->cend ());

	sClMSetTarget.template insert <citlist_t> (sList.cbegin (), sList.cend ());

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<list::riter_t> (riter_t, riter_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMSet->cbegin (), pClMSet->cend ());

	sClMSetTarget.template insert <ritlist_t> (sList.rbegin (), sList.rend ());

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<list::criter_t> (criter_t, criter_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMSet->cbegin (), pClMSet->cend ());

	sClMSetTarget.template insert <critlist_t> (sList.crbegin (), sList.crend ());

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<list::iter_t> (iter_t == iter_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMSet->cbegin (), pClMSet->cend ());

	sItListA = sList.begin ();
	sItListB = sList.begin ();

	while (sItListA != sList.end ())
	{
		sClMSetTarget.template insert <itlist_t> (sItListA, sItListB);

		sItListA++;
		sItListB++;
	}

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<list::citer_t> (citer_t == citer_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMSet->cbegin (), pClMSet->cend ());

	sCItListA = sList.cbegin ();
	sCItListB = sList.cbegin ();

	while (sCItListA != sList.cend ())
	{
		sClMSetTarget.template insert <citlist_t> (sCItListA, sCItListB);

		sCItListA++;
		sCItListB++;
	}

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<list::riter_t> (riter_t == riter_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMSet->cbegin (), pClMSet->cend ());

	sRItListA = sList.rbegin ();
	sRItListB = sList.rbegin ();

	while (sRItListA != sList.rend ())
	{
		sClMSetTarget.template insert <ritlist_t> (sRItListA, sRItListB);

		sRItListA++;
		sRItListB++;
	}

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<list::criter_t> (criter_t == criter_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMSet->cbegin (), pClMSet->cend ());

	sCRItListA = sList.crbegin ();
	sCRItListB = sList.crbegin ();

	while (sCRItListA != sList.crend ())
	{
		sClMSetTarget.template insert <critlist_t> (sCRItListA, sCRItListB);

		sCRItListA++;
		sCRItListB++;
	}

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<list::iter_t> (>iter_t, iter_t<)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMSet->cbegin (), pClMSet->cend ());

	sItListA = sList.begin ();
	sItListB = sList.end ();

	::std::advance<itlist_t, int> (sItListA, nNumEntries / 4);
	::std::advance<itlist_t, int> (sItListB, 0 - (nNumEntries / 4));

	sClMSetTarget.template insert <itlist_t> (sItListA, sItListB);

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<list::citer_t> (>citer_t, citer_t<)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMSet->cbegin (), pClMSet->cend ());

	sCItListA = sList.cbegin ();
	sCItListB = sList.cend ();

	::std::advance<citlist_t, int> (sCItListA, nNumEntries / 4);
	::std::advance<citlist_t, int> (sCItListB, 0 - (nNumEntries / 4));

	sClMSetTarget.template insert <citlist_t> (sCItListA, sCItListB);

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<list::riter_t> (>riter_t, riter_t<)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMSet->cbegin (), pClMSet->cend ());

	sRItListA = sList.rbegin ();
	sRItListB = sList.rend ();

	::std::advance<ritlist_t, int> (sRItListA, nNumEntries / 4);
	::std::advance<ritlist_t, int> (sRItListB, 0 - (nNumEntries / 4));

	sClMSetTarget.template insert <ritlist_t> (sRItListA, sRItListB);

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<list::criter_t> (>criter_t, criter_t<)" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMSet->cbegin (), pClMSet->cend ());

	sCRItListA = sList.crbegin ();
	sCRItListB = sList.crend ();

	::std::advance<critlist_t, int> (sCRItListA, nNumEntries / 4);
	::std::advance<critlist_t, int> (sCRItListB, 0 - (nNumEntries / 4));

	sClMSetTarget.template insert <critlist_t> (sCRItListA, sCRItListB);

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<vector::iter_t> (iter_t, iter_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sMultiSetVectorFeed.template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	sVector.template insert <typename CBTreeMultiSet_VectorFeed_t::const_iterator> (sVector.begin (), sMultiSetVectorFeed.cbegin (), sMultiSetVectorFeed.cend ());

	sClMSetTarget.template insert <itvec_t> (sVector.begin (), sVector.end ());

	sVector.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<vector::citer_t> (citer_t, citer_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sMultiSetVectorFeed.template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	sVector.template insert <typename CBTreeMultiSet_VectorFeed_t::const_iterator> (sVector.begin (), sMultiSetVectorFeed.cbegin (), sMultiSetVectorFeed.cend ());

	sClMSetTarget.template insert <citvec_t> (sVector.cbegin (), sVector.cend ());

	sVector.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<vector::riter_t> (riter_t, riter_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sMultiSetVectorFeed.template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	sVector.template insert <typename CBTreeMultiSet_VectorFeed_t::const_iterator> (sVector.begin (), sMultiSetVectorFeed.cbegin (), sMultiSetVectorFeed.cend ());

	sClMSetTarget.template insert <ritvec_t> (sVector.rbegin (), sVector.rend ());

	sVector.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<vector::criter_t> (criter_t, criter_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sMultiSetVectorFeed.template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	sVector.template insert <typename CBTreeMultiSet_VectorFeed_t::const_iterator> (sVector.begin (), sMultiSetVectorFeed.cbegin (), sMultiSetVectorFeed.cend ());

	sClMSetTarget.template insert <critvec_t> (sVector.crbegin (), sVector.crend ());

	sVector.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<vector::iter_t> (iter_t == iter_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sMultiSetVectorFeed.template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	sVector.template insert <typename CBTreeMultiSet_VectorFeed_t::const_iterator> (sVector.begin (), sMultiSetVectorFeed.cbegin (), sMultiSetVectorFeed.cend ());

	sItVecA = sVector.begin ();
	sItVecB = sVector.begin ();

	while (sItVecA != sVector.end ())
	{
		sClMSetTarget.template insert <itvec_t> (sItVecA, sItVecB);

		sItVecA++;
		sItVecB++;
	}

	sVector.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<vector::citer_t> (citer_t == citer_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sMultiSetVectorFeed.template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	sVector.template insert <typename CBTreeMultiSet_VectorFeed_t::const_iterator> (sVector.begin (), sMultiSetVectorFeed.cbegin (), sMultiSetVectorFeed.cend ());

	sCItVecA = sVector.cbegin ();
	sCItVecB = sVector.cbegin ();

	while (sCItVecA != sVector.cend ())
	{
		sClMSetTarget.template insert <citvec_t> (sCItVecA, sCItVecB);

		sCItVecA++;
		sCItVecB++;
	}

	sVector.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<vector::riter_t> (riter_t == riter_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sMultiSetVectorFeed.template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	sVector.template insert <typename CBTreeMultiSet_VectorFeed_t::const_iterator> (sVector.begin (), sMultiSetVectorFeed.cbegin (), sMultiSetVectorFeed.cend ());

	sRItVecA = sVector.rbegin ();
	sRItVecB = sVector.rbegin ();

	while (sRItVecA != sVector.rend ())
	{
		sClMSetTarget.template insert <ritvec_t> (sRItVecA, sRItVecB);

		sRItVecA++;
		sRItVecB++;
	}

	sVector.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<vector::criter_t> (criter_t == criter_t)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sMultiSetVectorFeed.template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	sVector.template insert <typename CBTreeMultiSet_VectorFeed_t::const_iterator> (sVector.begin (), sMultiSetVectorFeed.cbegin (), sMultiSetVectorFeed.cend ());

	sCRItVecA = sVector.crbegin ();
	sCRItVecB = sVector.crbegin ();

	while (sCRItVecA != sVector.crend ())
	{
		sClMSetTarget.template insert <critvec_t> (sCRItVecA, sCRItVecB);

		sCRItVecA++;
		sCRItVecB++;
	}

	sVector.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<vector::iter_t> (>iter_t, iter_t<)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sMultiSetVectorFeed.template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	sVector.template insert <typename CBTreeMultiSet_VectorFeed_t::const_iterator> (sVector.begin (), sMultiSetVectorFeed.cbegin (), sMultiSetVectorFeed.cend ());

	sItVecA = sVector.begin ();
	sItVecB = sVector.end ();

	::std::advance<itvec_t, int> (sItVecA, nNumEntries / 4);
	::std::advance<itvec_t, int> (sItVecB, 0 - (nNumEntries / 4));

	sClMSetTarget.template insert <itvec_t> (sItVecA, sItVecB);

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<vector::citer_t> (>citer_t, citer_t<)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sMultiSetVectorFeed.template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	sVector.template insert <typename CBTreeMultiSet_VectorFeed_t::const_iterator> (sVector.begin (), sMultiSetVectorFeed.cbegin (), sMultiSetVectorFeed.cend ());

	sCItVecA = sVector.cbegin ();
	sCItVecB = sVector.cend ();

	::std::advance<citvec_t, int> (sCItVecA, nNumEntries / 4);
	::std::advance<citvec_t, int> (sCItVecB, 0 - (nNumEntries / 4));

	sClMSetTarget.template insert <citvec_t> (sCItVecA, sCItVecB);

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<vector::riter_t> (>riter_t, riter_t<)" << endl;
	
	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sMultiSetVectorFeed.template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	sVector.template insert <typename CBTreeMultiSet_VectorFeed_t::const_iterator> (sVector.begin (), sMultiSetVectorFeed.cbegin (), sMultiSetVectorFeed.cend ());

	sRItVecA = sVector.rbegin ();
	sRItVecB = sVector.rend ();

	::std::advance<ritvec_t, int> (sRItVecA, nNumEntries / 4);
	::std::advance<ritvec_t, int> (sRItVecB, 0 - (nNumEntries / 4));

	sClMSetTarget.template insert <ritvec_t> (sRItVecA, sRItVecB);

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<vector::criter_t> (>criter_t, criter_t<)" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sMultiSetVectorFeed.template insert <citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	sVector.template insert <typename CBTreeMultiSet_VectorFeed_t::const_iterator> (sVector.begin (), sMultiSetVectorFeed.cbegin (), sMultiSetVectorFeed.cend ());

	sCRItVecA = sVector.crbegin ();
	sCRItVecB = sVector.crend ();

	::std::advance<critvec_t, int> (sCRItVecA, nNumEntries / 4);
	::std::advance<critvec_t, int> (sCRItVecB, 0 - (nNumEntries / 4));

	sClMSetTarget.template insert <critvec_t> (sCRItVecA, sCRItVecB);

	sList.clear ();
	pClMSet->clear ();
	sClMSetTarget.clear ();

	cout << "target::insert<map::iter_t> (iter_t, iter_t)" << endl;

	multiSetPrim_add<::std::set<uint32_t>, ::std::set<uint32_t>::value_type> (&sSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sMultiSetPair.template insert<itset_t> (sSet.begin (), sSet.end ());

	sMultiSetPair.clear ();

	cout << "target::insert<map::citer_t> (citer_t, citer_t)" << endl;
	
	multiSetPrim_add<::std::set<uint32_t>, ::std::set<uint32_t>::value_type> (&sSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sMultiSetPair.template insert<citset_t> (sSet.cbegin (), sSet.cend ());

	sMultiSetPair.clear ();

	cout << "target::insert<map::riter_t> (riter_t, riter_t)" << endl;
	
	multiSetPrim_add<::std::set<uint32_t>, ::std::set<uint32_t>::value_type> (&sSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sMultiSetPair.template insert<ritset_t> (sSet.rbegin (), sSet.rend ());

	sMultiSetPair.clear ();

	cout << "target::insert<map::criter_t> (criter_t, criter_t)" << endl;
	
	multiSetPrim_add<::std::set<uint32_t>, ::std::set<uint32_t>::value_type> (&sSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sMultiSetPair.template insert<critset_t> (sSet.crbegin (), sSet.crend ());

	sMultiSetPair.clear ();

	cout << "target::insert<map::iter_t> (iter_t == iter_t)" << endl;

	multiSetPrim_add<::std::set<uint32_t>, ::std::set<uint32_t>::value_type> (&sSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sItSetA = sSet.begin ();
	sItSetB = sSet.begin ();

	while (sItSetA != sSet.end ())
	{
		sMultiSetPair.template insert <itset_t> (sItSetA, sItSetB);

		sItSetA++;
		sItSetB++;
	}

	sMultiSetPair.clear ();
	
	cout << "target::insert<map::citer_t> (citer_t == citer_t)" << endl;
	
	multiSetPrim_add<::std::set<uint32_t>, ::std::set<uint32_t>::value_type> (&sSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sCItSetA = sSet.cbegin ();
	sCItSetB = sSet.cbegin ();

	while (sCItSetA != sSet.cend ())
	{
		sMultiSetPair.template insert <citset_t> (sCItSetA, sCItSetB);

		sCItSetA++;
		sCItSetB++;
	}

	sMultiSetPair.clear ();
	
	cout << "target::insert<map::riter_t> (riter_t == riter_t)" << endl;
	
	multiSetPrim_add<::std::set<uint32_t>, ::std::set<uint32_t>::value_type> (&sSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sRItSetA = sSet.rbegin ();
	sRItSetB = sSet.rbegin ();

	while (sRItSetA != sSet.rend ())
	{
		sMultiSetPair.template insert <ritset_t> (sRItSetA, sRItSetB);

		sRItSetA++;
		sRItSetB++;
	}

	sMultiSetPair.clear ();
	
	cout << "target::insert<map::criter_t> (criter_t == criter_t)" << endl;
	
	multiSetPrim_add<::std::set<uint32_t>, ::std::set<uint32_t>::value_type> (&sSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sCRItSetA = sSet.crbegin ();
	sCRItSetB = sSet.crbegin ();

	while (sCRItSetA != sSet.crend ())
	{
		sMultiSetPair.template insert <critset_t> (sCRItSetA, sCRItSetB);

		sCRItSetA++;
		sCRItSetB++;
	}

	sMultiSetPair.clear ();
	
	cout << "target::insert<map::iter_t> (>iter_t, iter_t<)" << endl;
	
	multiSetPrim_add<::std::set<uint32_t>, ::std::set<uint32_t>::value_type> (&sSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sItSetA = sSet.begin ();
	sItSetB = sSet.end ();

	::std::advance<itset_t, int> (sItSetA, (int) (nNumEntries / 4));
	::std::advance<itset_t, int> (sItSetB, (int) (0 - (nNumEntries / 4)));

	sMultiSetPair.template insert <itset_t> (sItSetA, sItSetB);

	sMultiSetPair.clear ();

	cout << "target::insert<map::citer_t> (>citer_t, citer_t<)" << endl;
	
	multiSetPrim_add<::std::set<uint32_t>, ::std::set<uint32_t>::value_type> (&sSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sCItSetA = sSet.cbegin ();
	sCItSetB = sSet.cend ();

	::std::advance<citset_t, int> (sCItSetA, (int) (nNumEntries / 4));
	::std::advance<citset_t, int> (sCItSetB, (int) (0 - (nNumEntries / 4)));

	sMultiSetPair.template insert <citset_t> (sCItSetA, sCItSetB);

	sMultiSetPair.clear ();

	cout << "target::insert<map::riter_t> (>riter_t, riter_t<)" << endl;
	
	multiSetPrim_add<::std::set<uint32_t>, ::std::set<uint32_t>::value_type> (&sSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sRItSetA = sSet.rbegin ();
	sRItSetB = sSet.rend ();

	::std::advance<ritset_t, int> (sRItSetA, (int) (nNumEntries / 4));
	::std::advance<ritset_t, int> (sRItSetB, (int) (0 - (nNumEntries / 4)));

	sMultiSetPair.template insert <ritset_t> (sRItSetA, sRItSetB);

	sMultiSetPair.clear ();

	cout << "target::insert<map::criter_t> (>criter_t, criter_t<)" << endl;

	multiSetPrim_add<::std::set<uint32_t>, ::std::set<uint32_t>::value_type> (&sSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sCRItSetA = sSet.crbegin ();
	sCRItSetB = sSet.crend ();

	::std::advance<critset_t, int> (sCRItSetA, (int) (nNumEntries / 4));
	::std::advance<critset_t, int> (sCRItSetB, (int) (0 - (nNumEntries / 4)));

	sMultiSetPair.template insert <critset_t> (sCRItSetA, sCRItSetB);

	sMultiSetPair.clear ();

	cout << "target::insert<multiset::iter_t> (iter_t, iter_t)" << endl;
	
	multiSetPrim_add<::std::multiset<uint32_t>, ::std::multiset<uint32_t>::value_type> (&sMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sMultiSetPair.template insert<itmset_t> (sMSet.begin (), sMSet.end ());

	sMultiSetPair.clear ();

	cout << "target::insert<multiset::citer_t> (citer_t, citer_t)" << endl;
	
	multiSetPrim_add<::std::multiset<uint32_t>, ::std::multiset<uint32_t>::value_type> (&sMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sMultiSetPair.template insert<citmset_t> (sMSet.cbegin (), sMSet.cend ());

	sMultiSetPair.clear ();

	cout << "target::insert<multiset::riter_t> (riter_t, riter_t)" << endl;
	
	multiSetPrim_add<::std::multiset<uint32_t>, ::std::multiset<uint32_t>::value_type> (&sMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sMultiSetPair.template insert<ritmset_t> (sMSet.rbegin (), sMSet.rend ());

	sMultiSetPair.clear ();

	cout << "target::insert<multiset::criter_t> (criter_t, criter_t)" << endl;
	
	multiSetPrim_add<::std::multiset<uint32_t>, ::std::multiset<uint32_t>::value_type> (&sMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sMultiSetPair.template insert<critmset_t> (sMSet.crbegin (), sMSet.crend ());

	sMultiSetPair.clear ();

	cout << "target::insert<multiset::iter_t> (iter_t == iter_t)" << endl;
	
	multiSetPrim_add<::std::multiset<uint32_t>, ::std::multiset<uint32_t>::value_type> (&sMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sItMSetA = sMSet.begin ();
	sItMSetB = sMSet.begin ();

	while (sItMSetA != sMSet.end ())
	{
		sMultiSetPair.template insert <itmset_t> (sItMSetA, sItMSetB);

		sItMSetA++;
		sItMSetB++;
	}

	sMultiSetPair.clear ();
	
	cout << "target::insert<multiset::citer_t> (citer_t == citer_t)" << endl;
	
	multiSetPrim_add<::std::multiset<uint32_t>, ::std::multiset<uint32_t>::value_type> (&sMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sCItMSetA = sMSet.cbegin ();
	sCItMSetB = sMSet.cbegin ();

	while (sCItMSetA != sMSet.cend ())
	{
		sMultiSetPair.template insert <citmset_t> (sCItMSetA, sCItMSetB);

		sCItMSetA++;
		sCItMSetB++;
	}

	sMultiSetPair.clear ();
	
	cout << "target::insert<multiset::riter_t> (riter_t == riter_t)" << endl;
	
	multiSetPrim_add<::std::multiset<uint32_t>, ::std::multiset<uint32_t>::value_type> (&sMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sRItMSetA = sMSet.rbegin ();
	sRItMSetB = sMSet.rbegin ();

	while (sRItMSetA != sMSet.rend ())
	{
		sMultiSetPair.template insert <ritmset_t> (sRItMSetA, sRItMSetB);

		sRItMSetA++;
		sRItMSetB++;
	}

	sMultiSetPair.clear ();
	
	cout << "target::insert<multiset::criter_t> (criter_t == criter_t)" << endl;
	
	multiSetPrim_add<::std::multiset<uint32_t>, ::std::multiset<uint32_t>::value_type> (&sMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sCRItMSetA = sMSet.crbegin ();
	sCRItMSetB = sMSet.crbegin ();

	while (sCRItMSetA != sMSet.crend ())
	{
		sMultiSetPair.template insert <critmset_t> (sCRItMSetA, sCRItMSetB);

		sCRItMSetA++;
		sCRItMSetB++;
	}

	sMultiSetPair.clear ();
	
	cout << "target::insert<multiset::iter_t> (>iter_t, iter_t<)" << endl;
	
	multiSetPrim_add<::std::multiset<uint32_t>, ::std::multiset<uint32_t>::value_type> (&sMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sItMSetA = sMSet.begin ();
	sItMSetB = sMSet.end ();

	::std::advance<itmset_t, int> (sItMSetA, (int) (nNumEntries / 4));
	::std::advance<itmset_t, int> (sItMSetB, (int) (0 - (nNumEntries / 4)));

	sMultiSetPair.template insert <itmset_t> (sItMSetA, sItMSetB);

	sMultiSetPair.clear ();

	cout << "target::insert<multiset::citer_t> (>citer_t, citer_t<)" << endl;
	
	multiSetPrim_add<::std::multiset<uint32_t>, ::std::multiset<uint32_t>::value_type> (&sMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sCItMSetA = sMSet.cbegin ();
	sCItMSetB = sMSet.cend ();

	::std::advance<citmset_t, int> (sCItMSetA, (int) (nNumEntries / 4));
	::std::advance<citmset_t, int> (sCItMSetB, (int) (0 - (nNumEntries / 4)));

	sMultiSetPair.template insert <citmset_t> (sCItMSetA, sCItMSetB);

	sMultiSetPair.clear ();

	cout << "target::insert<multiset::riter_t> (>riter_t, riter_t<)" << endl;
	
	multiSetPrim_add<::std::multiset<uint32_t>, ::std::multiset<uint32_t>::value_type> (&sMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sRItMSetA = sMSet.rbegin ();
	sRItMSetB = sMSet.rend ();

	::std::advance<ritmset_t, int> (sRItMSetA, (int) (nNumEntries / 4));
	::std::advance<ritmset_t, int> (sRItMSetB, (int) (0 - (nNumEntries / 4)));

	sMultiSetPair.template insert <ritmset_t> (sRItMSetA, sRItMSetB);

	sMultiSetPair.clear ();

	cout << "target::insert<multiset::criter_t> (>criter_t, criter_t<)" << endl;

	multiSetPrim_add<::std::multiset<uint32_t>, ::std::multiset<uint32_t>::value_type> (&sMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	
	sCRItMSetA = sMSet.crbegin ();
	sCRItMSetB = sMSet.crend ();

	::std::advance<critmset_t, int> (sCRItMSetA, (int) (nNumEntries / 4));
	::std::advance<critmset_t, int> (sCRItMSetB, (int) (0 - (nNumEntries / 4)));

	sMultiSetPair.template insert <critmset_t> (sCRItMSetA, sCRItMSetB);

	sMultiSetPair.clear ();
}

template <class _t_multiset>
void TestBTreeMultiSetSTLifEraseViaIterator (_t_multiset *pClMSet, uint32_t nNumEntries)
{
	typedef typename _t_multiset::const_iterator						citer_t;

	uint32_t									nLastKey = 0;
	citer_t										sIter;
	citer_t										sIterRslt;
	
	cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: erase (const_iterator)" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	while (pClMSet->size () > 0)
	{
		sIter = pClMSet->cbegin ();

		sIter += (int) (generate_rand32 () % pClMSet->size ());

		sIterRslt = pClMSet->erase (sIter);

		if (sIter != sIterRslt)
		{
			cerr << endl;
			cerr << "TestBTreeMultiSetSTLifEraseViaIterator: returned iterator mismatches!" << endl;

			exit (-1);
		}
	}
}

template <class _t_multiset>
void TestBTreeMultiSetSTLifEraseViaKey (_t_multiset *pClMSet, uint32_t nNumEntries, uint32_t nInstances)
{
	typedef typename _t_multiset::const_iterator										citer_t;
	
	uint32_t									nLastKey;
	citer_t										sIter;
	uint32_t									i;
	typename _t_multiset::sizetype_t			nRslt;
	
	cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: erase (const keytype &)" << endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	}

	while (pClMSet->size () > 0)
	{
		sIter = pClMSet->cbegin ();

		sIter += (int) (generate_rand32 () % pClMSet->size ());

		nRslt = pClMSet->erase (((typename _t_multiset::value_t)(*sIter)));

		if (nRslt != nInstances)
		{
			cerr << endl;
			cerr << "TestBTreeMultiSetSTLifEraseViaKey: unexpected number of items deleted!" << endl;

			exit (-1);
		}
	}
}

template <class _t_multiset>
void TestBTreeMultiSetSTLifEraseViaIteratorMultiple (_t_multiset *pClMSet, uint32_t nNumEntries, uint32_t nInstances)
{
	typedef typename _t_multiset::const_iterator										citer_t;

	uint32_t									nLastKey;
	citer_t										sIterBegin;
	citer_t										sIterEnd;
	citer_t										sIterRslt;
	uint32_t									i;
	
	cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: erase (const_iterator, const_iterator)" << endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);
	}

	while (pClMSet->size () > 0)
	{
		sIterBegin = pClMSet->cbegin ();
		sIterEnd = pClMSet->cend ();

		if (pClMSet->size () > 1)
		{
			sIterBegin += (int) (generate_rand32 () % (pClMSet->size () / 2));
			sIterEnd -= (int) (generate_rand32 () % (pClMSet->size () / 2));
		}

		sIterRslt = pClMSet->erase (sIterBegin, sIterEnd);

		if (sIterRslt != sIterBegin)
		{
			cerr << endl;
			cerr << "TestBTreeMultiSetSTLifEraseViaIteratorMultiple: returned iterator mismatches!" << endl;

			exit (-1);
		}
	}
}

template<class _t_multiset>
void TestBTreeMultiSetSTLifKeyComp (_t_multiset *pClMSet, uint32_t nNumEntries)
{
	typedef typename _t_multiset::iterator		iter_t;

	typename _t_multiset::value_t				nLastKey = 0;
	iter_t										sIterBegin;
	iter_t										sIterEnd;
	iter_t										sIter;
	iter_t										sIterUpper;
	typename _t_multiset::value_t				nKey;
	typename _t_multiset::value_t				nNextKey;
	typename _t_multiset::key_compare			sMMComp = pClMSet->key_comp ();
	
	cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: key_comp ()" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClMSet->begin ();
	sIterEnd = pClMSet->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = ((typename _t_multiset::value_t)(*sIter));

		sIterUpper = pClMSet->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			nNextKey = ((typename _t_multiset::value_t)(*sIterUpper));

			if (!sMMComp (nKey, nNextKey))
			{
				cerr << endl;
				cerr << "TestBTreeMultiSetSTLifKeyComp: ERROR: CBTreeMultiSet::key_compare (const keytype &, const keytype &) delivered unexpected result!" << endl;

				exit (-1);
			}
		}
	}
}

template<class _t_multiset>
void TestBTreeMultiSetSTLifValueComp (_t_multiset *pClMSet, uint32_t nNumEntries)
{
	typedef typename _t_multiset::iterator		iter_t;

	uint32_t									nLastKey = 0;
	iter_t										sIterBegin;
	iter_t										sIterEnd;
	iter_t										sIter;
	iter_t										sIterUpper;
	uint32_t									nKey;
	typename _t_multiset::value_compare			sMMComp = pClMSet->value_comp ();
	
	cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: value_comp ()" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClMSet->begin ();
	sIterEnd = pClMSet->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = ((typename _t_multiset::value_t)(*sIter));

		sIterUpper = pClMSet->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			if (!sMMComp (((typename _t_multiset::value_t)(*sIter)), ((typename _t_multiset::value_t)(*sIterUpper))))
			{
				cerr << endl;
				cerr << "TestBTreeMultiSetSTLifValueComp: ERROR: CBTreeMultiSet::value_compare (const valuetype &, const valuetype &) delivered unexpected result!" << endl;

				exit (-1);
			}
		}
	}
}

template <class _t_multiset>
void TestBTreeMultiSetSTLifSwap (_t_multiset *pClMSet, uint32_t nNumEntries)
{
	typedef typename _t_multiset::const_iterator			citer_t;

	uint32_t						nLastKey = 0;
	_t_multiset						sClMultiSetTarget (*pClMSet, false);
	_t_multiset						sClMultiSetRef (*pClMSet, false);
	_t_multiset						sClMultiSetTargetRef (*pClMSet, false);

	cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: swap (CBTreeMultiSet &)" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sClMultiSetRef.template insert<citer_t> (pClMSet->cbegin (), pClMSet->cend ());

	if ((*pClMSet) != sClMultiSetRef)
	{
		cerr << "ERROR: Unexpected multiset mismatch!" << endl;

		exit (-1);
	}

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (&sClMultiSetTarget, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY);

	sClMultiSetTargetRef.template insert<citer_t> (sClMultiSetTarget.cbegin (), sClMultiSetTarget.cend ());

	if (sClMultiSetTarget != sClMultiSetTargetRef)
	{
		cerr << "ERROR: Unexpected target mismatch!" << endl;

		exit (-1);
	}

	pClMSet->swap (sClMultiSetTarget);

	if ((*pClMSet) != sClMultiSetTargetRef)
	{
		cerr << "ERROR: swapped multiset mismatches target reference!" << endl;

		exit (-1);
	}

	if (sClMultiSetTarget != sClMultiSetRef)
	{
		cerr << "ERROR: target mismatches multiset reference!" << endl;

		exit (-1);
	}

	sClMultiSetTarget.swap (*pClMSet);

	if ((*pClMSet) != sClMultiSetRef)
	{
		cerr << "ERROR: multiset mismatches multiset reference!" << endl;

		exit (-1);
	}

	if (sClMultiSetTarget != sClMultiSetTargetRef)
	{
		cerr << "ERROR: target mismatches target reference!" << endl;

		exit (-1);
	}
}

template <class _t_multiset>
void TestBTreeMultiSetSTLifFind (_t_multiset *pClMSet, uint32_t nNumEntries)
{
	typedef typename _t_multiset::const_iterator			citer_t;

	uint32_t						nLastKey = 1;
	citer_t							sCIter;
	citer_t							sCIterRslt;
	
	cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: find (const _t_keytype &)" << endl;

	multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_ASCEND_KEY);

	for (sCIter = pClMSet->cbegin (); sCIter != pClMSet->cend (); sCIter++)
	{
		sCIterRslt = pClMSet->find ((typename _t_multiset::value_t)(*sCIter));

		if (sCIter != sCIterRslt)
		{
			cerr << endl;
			cerr << "TestBTreeMultiSetSTLifFind: ERROR: iterator mismatch!" << endl;

			exit (-1);
		}
	}

	sCIterRslt = pClMSet->find (0);

	if (sCIterRslt != pClMSet->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeMultiSetSTLifFind: ERROR: find () didn't return cend () on lower bound check!" << endl;

		exit (-1);
	}

	sCIterRslt = pClMSet->find (nNumEntries + 1);

	if (sCIterRslt != pClMSet->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeMultiSetSTLifFind: ERROR: find () didn't return cend () on upper bound check!" << endl;

		exit (-1);
	}
}

template <class _t_multiset>
void TestBTreeMultiSetSTLifLowerBoundUpperBound (_t_multiset *pClMSet, uint32_t nNumEntries, uint32_t nInstance)
{
	typedef typename _t_multiset::const_iterator			citer_t;

	uint32_t						nLastKey;
	uint32_t						i;
	citer_t							sCIterLower;
	citer_t							sCIterUpper;
	
	cout << "exercises method compatible to STL interface CBTreeMultiSet<>:: lower_bound () upper_bound () count ()" << endl;
	
	for (i = 0; i < nInstance; i++)
	{
		nLastKey = 1;

		multiSetPrim_add<_t_multiset, typename _t_multiset::value_t> (pClMSet, nNumEntries, nLastKey, BTREETEST_MULTISET_PRIMITIVE_ASCEND_KEY);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sCIterLower = pClMSet->lower_bound (i + 1);
		sCIterUpper = pClMSet->upper_bound (i + 1);

		sCIterLower += (int) nInstance;

		if (sCIterLower != sCIterUpper)
		{
			cerr << endl;
			cerr << "TestBTreeMultiSetSTLifLowerBoundUpperBound: ERROR: boundary mismatch!" << endl;

			exit (-1);
		}

		if (pClMSet->count (i + 1) != nInstance)
		{
			cerr << endl;
			cerr << "TestBTreeMultiSetSTLifLowerBoundUpperBound: ERROR: count () returned with unexpected value!" << endl;

			exit (-1);
		}
	}

	sCIterLower = pClMSet->lower_bound (0);

	if (sCIterLower != pClMSet->cbegin ())
	{
		cerr << endl;
		cerr << "TestBTreeMultiSetSTLifLowerBoundUpperBound: ERROR: lower_bound () didn't return cbegin () when testing for lowest key!" << endl;

		exit (-1);
	}

	sCIterUpper = pClMSet->upper_bound (nNumEntries + 1);

	if (sCIterUpper != pClMSet->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeMultiSetSTLifLowerBoundUpperBound: ERROR: upper_bound () didn't return cend () when testing for lowest key!" << endl;

		exit (-1);
	}
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeSTLmultiSet (uint32_t nTestNum, _t_subnodeiter nNodeSize, _t_datalayerproperties sDataProperties, bayerTreeCacheDescription_t &sCacheDesc, int argc, char **argv)
{
	typedef CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		CBTreeTestMultiSet_t;

	CBTreeTestMultiSet_t		*pClMSet = new CBTreeTestMultiSet_t (sDataProperties, &sCacheDesc, nNodeSize);

	if (pClMSet == NULL)
	{
		cerr << "TestBTreeSTLmultiSet: ERROR: insufficient memory!" << endl;

		exit (-1);
	}

	cout << "b-tree multi-map test bench selected" << endl;

	switch (nTestNum)
	{
	case BTREETEST_MULTISET_ASSIGNMENT_OPERATOR	:
		{
			TestBTreeMultiSetSTLifAssignmentOperator<CBTreeTestMultiSet_t> (pClMSet, 64);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_INSERT	:
		{
			TestBTreeMultiSetSTLifInsert<CBTreeTestMultiSet_t> (pClMSet, 64);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_INSERT_VIA_ITERATOR	:
		{
			TestBTreeMultiSetSTLifInsertViaIterator<CBTreeTestMultiSet_t> (pClMSet, 64, sDataProperties, &sCacheDesc, nNodeSize);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_ERASE_VIA_ITERATOR	:
		{
			TestBTreeMultiSetSTLifEraseViaIterator<CBTreeTestMultiSet_t> (pClMSet, 64);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_ERASE_VIA_KEY	:
		{
			TestBTreeMultiSetSTLifEraseViaKey<CBTreeTestMultiSet_t> (pClMSet, 64, 5);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE	:
		{
			TestBTreeMultiSetSTLifEraseViaIteratorMultiple<CBTreeTestMultiSet_t> (pClMSet, 64, 5);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_KEY_COMP	:
		{
			TestBTreeMultiSetSTLifKeyComp<CBTreeTestMultiSet_t> (pClMSet, 64);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_VALUE_COMP	:
		{
			TestBTreeMultiSetSTLifValueComp<CBTreeTestMultiSet_t> (pClMSet, 64);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_SWAP	:
		{
			TestBTreeMultiSetSTLifSwap<CBTreeTestMultiSet_t> (pClMSet, 128);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_FIND	:
		{
			TestBTreeMultiSetSTLifFind<CBTreeTestMultiSet_t> (pClMSet, 64);

			break;
		}
	
	case BTREETEST_MULTISET_STL_IF_LOWER_BOUND_UPPER_BOUND	:
		{
			TestBTreeMultiSetSTLifLowerBoundUpperBound<CBTreeTestMultiSet_t> (pClMSet, 64, 5);

			break;
		}
	
	default:
		{
			cerr << "ERROR: TestBTreeSTLmultiSet: Unknown test or test not specified!" << endl;

			break;
		}
	}

	delete pClMSet;
}
