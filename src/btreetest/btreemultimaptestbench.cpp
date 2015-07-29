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

	_t_multimap									sClMM (*pClMM, false);
	uint32_t									nLastKey = 0;
	citer_t										sIter;
	citer_t										sIterRslt;
	
	cout << "exercises assignment operator compatible to STL interface CBTreeMultiMap<>:: operator= (CBTreeMultiMap<> &)" << endl;

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

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

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastSeed, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
}

template <class _t_multimap>
void TestBTreeMultiMapSTLifInsertViaIterator (_t_multimap *pClMM, uint32_t nNumEntries, typename _t_multimap::datalayerproperties_t &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nNodeSize)
{
	typedef typename _t_multimap::value_t												value_t;

	typedef typename _t_multimap::iterator												iter_t;
	typedef typename _t_multimap::const_iterator										citer_t;
	typedef typename _t_multimap::reverse_iterator										riter_t;
	typedef typename _t_multimap::const_reverse_iterator								criter_t;

	typedef typename ::std::list<value_t>::iterator										itlist_t;
	typedef typename ::std::list<value_t>::const_iterator								citlist_t;
	typedef typename ::std::list<value_t>::reverse_iterator								ritlist_t;
	typedef typename ::std::list<value_t>::const_reverse_iterator						critlist_t;

	typedef typename ::std::vector<value_t>::iterator									itvec_t;
	typedef typename ::std::vector<value_t>::const_iterator								citvec_t;
	typedef typename ::std::vector<value_t>::reverse_iterator							ritvec_t;
	typedef typename ::std::vector<value_t>::const_reverse_iterator						critvec_t;

	typedef CBTreeMultiMap<uint32_t, multiMapMap_t, size_t>								CBTreeMultiMap_VectorFeed_t;

	typedef CBTreeTestMultiMap<typename _t_multimap::sizetype_t, typename _t_multimap::nodeiter_t, typename _t_multimap::subnodeiter_t, typename _t_multimap::datalayerproperties_t, typename _t_multimap::datalayer_t>
																						CBTreeMultiMapTest_Pair_t;

	typedef typename CBTreeMultiMapTest_Pair_t::iterator								iter_pair_t;
	typedef typename CBTreeMultiMapTest_Pair_t::const_iterator							citer_pair_t;
	typedef typename CBTreeMultiMapTest_Pair_t::reverse_iterator						riter_pair_t;
	typedef typename CBTreeMultiMapTest_Pair_t::const_reverse_iterator					criter_pair_t;

	typedef typename ::std::map<uint32_t, multiMapMap_t>::iterator						itmap_t;
	typedef typename ::std::map<uint32_t, multiMapMap_t>::const_iterator				citmap_t;
	typedef typename ::std::map<uint32_t, multiMapMap_t>::reverse_iterator				ritmap_t;
	typedef typename ::std::map<uint32_t, multiMapMap_t>::const_reverse_iterator		critmap_t;

	typedef typename ::std::multimap<uint32_t, multiMapMap_t>::iterator					itmmap_t;
	typedef typename ::std::multimap<uint32_t, multiMapMap_t>::const_iterator			citmmap_t;
	typedef typename ::std::multimap<uint32_t, multiMapMap_t>::reverse_iterator			ritmmap_t;
	typedef typename ::std::multimap<uint32_t, multiMapMap_t>::const_reverse_iterator	critmmap_t;
	
	_t_multimap									sClMMTarget (*pClMM, false);
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
	CBTreeMultiMap_VectorFeed_t					sMultiMapVectorFeed (sRAMproperties, psCacheDescription, nNodeSize);
	CBTreeMultiMapTest_Pair_t					sMultiMapPair (rDataLayerProperties, psCacheDescription, nNodeSize);
	CBTreeMultiMapTest_Pair_t					sMultiMapTargetPair (sMultiMapPair);
	value_t										sMultiMapPairData;
	citer_pair_t								sCIterPairA, sCIterPairB;
	criter_pair_t								sCRIterPairA, sCRIterPairB;
	::std::map<uint32_t, multiMapMap_t>			sMap;
	itmap_t										sItMapA, sItMapB;
	citmap_t									sCItMapA, sCItMapB;
	ritmap_t									sRItMapA, sRItMapB;
	critmap_t									sCRItMapA, sCRItMapB;
	::std::multimap<uint32_t, multiMapMap_t>	sMMap;
	itmmap_t									sItMMapA, sItMMapB;
	citmmap_t									sCItMMapA, sCItMMapB;
	ritmmap_t									sRItMMapA, sRItMMapB;
	critmmap_t									sCRItMMapA, sCRItMMapB;
	
	cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: insert<_t_iterator> (_t_iterator, _t_iterator)" << endl;

#if 0 // for this code to compile, the b-tree framework under test must support const methods
// this is to be implemented

	cout << "target::insert<_t_obj::iter_t> (iter_t, iter_t)" << endl;

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMMTarget.template insert <iter_t> (pClMM->begin (), pClMM->end ());

	pClMM->clear ();
	sClMMTarget.clear ();

#endif

	cout << "target::insert<_t_obj::citer_t> (citer_t, citer_t)" << endl;

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMMTarget.template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());

	pClMM->clear ();
	sClMMTarget.clear ();

#if 0 // for this code to compile, the b-tree framework under test must support const methods
// this is to be implemented

	cout << "target::insert<_t_obj::riter_t> (riter_t, riter_t)" << endl;

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMMTarget.template insert <riter_t> (pClMM->rbegin (), pClMM->rend ());

	pClMM->clear ();
	sClMMTarget.clear ();

#endif

	cout << "target::insert<_t_obj::criter_t> (criter_t, criter_t)" << endl;

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMMTarget.template insert <criter_t> (pClMM->crbegin (), pClMM->crend ());

	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<_t_obj::citer_t> (citer_t == citer_t)" << endl;

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sCIterA = pClMM->cbegin ();
	sCIterB = pClMM->cbegin ();

	while (sCIterA != pClMM->cend ())
	{
		sClMMTarget.template insert <citer_t> (sCIterA, sCIterB);

		sCIterA++;
		sCIterB++;
	}

	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<_t_obj::criter_t> (criter_t == criter_t)" << endl;

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sCRIterA = pClMM->crbegin ();
	sCRIterB = pClMM->crbegin ();

	while (sCRIterA != pClMM->crend ())
	{
		sClMMTarget.template insert <criter_t> (sCRIterA, sCRIterB);

		sCRIterA++;
		sCRIterB++;
	}

	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<_t_obj::citer_t> (>citer_t, citer_t<)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sCIterA = pClMM->cbegin ();
	sCIterB = pClMM->cend ();

	sCIterA += (int) (nNumEntries / 4);
	sCIterB -= (int) (nNumEntries / 4);

	sClMMTarget.template insert <citer_t> (sCIterA, sCIterB);

	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<_t_obj::criter_t> (>criter_t, criter_t<)" << endl;

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sCRIterA = pClMM->crbegin ();
	sCRIterB = pClMM->crend ();

	sCRIterA += (int) (nNumEntries / 4);
	sCRIterB -= (int) (nNumEntries / 4);

	sClMMTarget.template insert <criter_t> (sCRIterA, sCRIterB);

	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<_t_obj::citer_t> (target::citer_t, target::citer_t)" << endl;

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	pClMM->template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());

	pClMM->clear ();

	cout << "target::insert<_t_obj::criter_t> (target::criter_t, target::criter_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	pClMM->template insert <criter_t> (pClMM->crbegin (), pClMM->crend ());

	pClMM->clear ();

	cout << "target::insert<_t_obj::citer_t> (target::citer_t == target::citer_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sCIterA = pClMM->cbegin ();
	sCIterB = pClMM->cbegin ();

	while (sCIterA != pClMM->cend ())
	{
		pClMM->template insert <citer_t> (sCIterA, sCIterB);

		sCIterA++;
		sCIterB++;
	}

	pClMM->clear ();

	cout << "target::insert<_t_obj::criter_t> (target::criter_t == target::criter_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sCRIterA = pClMM->crbegin ();
	sCRIterB = pClMM->crbegin ();

	while (sCRIterA != pClMM->crend ())
	{
		pClMM->template insert <criter_t> (sCRIterA, sCRIterB);

		sCRIterA++;
		sCRIterB++;
	}

	pClMM->clear ();

	cout << "target::insert<_t_obj::citer_t> (>target::citer_t, target::citer_t<)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	pClMM->template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());
	pClMM->template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());
	pClMM->template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());
	pClMM->template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());

	sCIterA = pClMM->cbegin ();
	sCIterB = pClMM->cend ();

	sCIterA += (int) (nNumEntries / 4);
	sCIterB -= (int) (nNumEntries / 4);

	pClMM->template insert <citer_t> (sCIterA, sCIterB);

	pClMM->clear ();

	cout << "target::insert<_t_obj::criter_t> (>target::criter_t, target::criter_t<)" << endl;

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	pClMM->template insert <criter_t> (pClMM->crbegin (), pClMM->crend ());
	pClMM->template insert <criter_t> (pClMM->crbegin (), pClMM->crend ());
	pClMM->template insert <criter_t> (pClMM->crbegin (), pClMM->crend ());
	pClMM->template insert <criter_t> (pClMM->crbegin (), pClMM->crend ());

	sCRIterA = pClMM->crbegin ();
	sCRIterB = pClMM->crend ();

	sCRIterA += (int) (nNumEntries / 4);
	sCRIterB -= (int) (nNumEntries / 4);

	pClMM->template insert <criter_t> (sCRIterA, sCRIterB);

	pClMM->clear ();

	cout << "target::insert<list::iter_t> (iter_t, iter_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMM->cbegin (), pClMM->cend ());

	sClMMTarget.template insert <itlist_t> (sList.begin (), sList.end ());

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::citer_t> (citer_t, citer_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMM->cbegin (), pClMM->cend ());

	sClMMTarget.template insert <citlist_t> (sList.cbegin (), sList.cend ());

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::riter_t> (riter_t, riter_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMM->cbegin (), pClMM->cend ());

	sClMMTarget.template insert <ritlist_t> (sList.rbegin (), sList.rend ());

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::criter_t> (criter_t, criter_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMM->cbegin (), pClMM->cend ());

	sClMMTarget.template insert <critlist_t> (sList.crbegin (), sList.crend ());

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::iter_t> (iter_t == iter_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMM->cbegin (), pClMM->cend ());

	sItListA = sList.begin ();
	sItListB = sList.begin ();

	while (sItListA != sList.end ())
	{
		sClMMTarget.template insert <itlist_t> (sItListA, sItListB);

		sItListA++;
		sItListB++;
	}

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::citer_t> (citer_t == citer_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMM->cbegin (), pClMM->cend ());

	sCItListA = sList.cbegin ();
	sCItListB = sList.cbegin ();

	while (sCItListA != sList.cend ())
	{
		sClMMTarget.template insert <citlist_t> (sCItListA, sCItListB);

		sCItListA++;
		sCItListB++;
	}

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::riter_t> (riter_t == riter_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMM->cbegin (), pClMM->cend ());

	sRItListA = sList.rbegin ();
	sRItListB = sList.rbegin ();

	while (sRItListA != sList.rend ())
	{
		sClMMTarget.template insert <ritlist_t> (sRItListA, sRItListB);

		sRItListA++;
		sRItListB++;
	}

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::criter_t> (criter_t == criter_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMM->cbegin (), pClMM->cend ());

	sCRItListA = sList.crbegin ();
	sCRItListB = sList.crbegin ();

	while (sCRItListA != sList.crend ())
	{
		sClMMTarget.template insert <critlist_t> (sCRItListA, sCRItListB);

		sCRItListA++;
		sCRItListB++;
	}

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::iter_t> (>iter_t, iter_t<)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMM->cbegin (), pClMM->cend ());

	sItListA = sList.begin ();
	sItListB = sList.end ();

	::std::advance<itlist_t, int> (sItListA, nNumEntries / 4);
	::std::advance<itlist_t, int> (sItListB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <itlist_t> (sItListA, sItListB);

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::citer_t> (>citer_t, citer_t<)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMM->cbegin (), pClMM->cend ());

	sCItListA = sList.cbegin ();
	sCItListB = sList.cend ();

	::std::advance<citlist_t, int> (sCItListA, nNumEntries / 4);
	::std::advance<citlist_t, int> (sCItListB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <citlist_t> (sCItListA, sCItListB);

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::riter_t> (>riter_t, riter_t<)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMM->cbegin (), pClMM->cend ());

	sRItListA = sList.rbegin ();
	sRItListB = sList.rend ();

	::std::advance<ritlist_t, int> (sRItListA, nNumEntries / 4);
	::std::advance<ritlist_t, int> (sRItListB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <ritlist_t> (sRItListA, sRItListB);

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::criter_t> (>criter_t, criter_t<)" << endl;

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClMM->cbegin (), pClMM->cend ());

	sCRItListA = sList.crbegin ();
	sCRItListB = sList.crend ();

	::std::advance<critlist_t, int> (sCRItListA, nNumEntries / 4);
	::std::advance<critlist_t, int> (sCRItListB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <critlist_t> (sCRItListA, sCRItListB);

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::iter_t> (iter_t, iter_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMultiMapVectorFeed.template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());

	sVector.template insert <typename CBTreeMultiMap_VectorFeed_t::const_iterator> (sVector.begin (), sMultiMapVectorFeed.cbegin (), sMultiMapVectorFeed.cend ());

	sClMMTarget.template insert <itvec_t> (sVector.begin (), sVector.end ());

	sVector.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::citer_t> (citer_t, citer_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMultiMapVectorFeed.template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());

	sVector.template insert <typename CBTreeMultiMap_VectorFeed_t::const_iterator> (sVector.begin (), sMultiMapVectorFeed.cbegin (), sMultiMapVectorFeed.cend ());

	sClMMTarget.template insert <citvec_t> (sVector.cbegin (), sVector.cend ());

	sVector.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::riter_t> (riter_t, riter_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMultiMapVectorFeed.template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());

	sVector.template insert <typename CBTreeMultiMap_VectorFeed_t::const_iterator> (sVector.begin (), sMultiMapVectorFeed.cbegin (), sMultiMapVectorFeed.cend ());

	sClMMTarget.template insert <ritvec_t> (sVector.rbegin (), sVector.rend ());

	sVector.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::criter_t> (criter_t, criter_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMultiMapVectorFeed.template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());

	sVector.template insert <typename CBTreeMultiMap_VectorFeed_t::const_iterator> (sVector.begin (), sMultiMapVectorFeed.cbegin (), sMultiMapVectorFeed.cend ());

	sClMMTarget.template insert <critvec_t> (sVector.crbegin (), sVector.crend ());

	sVector.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::iter_t> (iter_t == iter_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMultiMapVectorFeed.template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());

	sVector.template insert <typename CBTreeMultiMap_VectorFeed_t::const_iterator> (sVector.begin (), sMultiMapVectorFeed.cbegin (), sMultiMapVectorFeed.cend ());

	sItVecA = sVector.begin ();
	sItVecB = sVector.begin ();

	while (sItVecA != sVector.end ())
	{
		sClMMTarget.template insert <itvec_t> (sItVecA, sItVecB);

		sItVecA++;
		sItVecB++;
	}

	sVector.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::citer_t> (citer_t == citer_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMultiMapVectorFeed.template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());

	sVector.template insert <typename CBTreeMultiMap_VectorFeed_t::const_iterator> (sVector.begin (), sMultiMapVectorFeed.cbegin (), sMultiMapVectorFeed.cend ());

	sCItVecA = sVector.cbegin ();
	sCItVecB = sVector.cbegin ();

	while (sCItVecA != sVector.cend ())
	{
		sClMMTarget.template insert <citvec_t> (sCItVecA, sCItVecB);

		sCItVecA++;
		sCItVecB++;
	}

	sVector.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::riter_t> (riter_t == riter_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMultiMapVectorFeed.template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());

	sVector.template insert <typename CBTreeMultiMap_VectorFeed_t::const_iterator> (sVector.begin (), sMultiMapVectorFeed.cbegin (), sMultiMapVectorFeed.cend ());

	sRItVecA = sVector.rbegin ();
	sRItVecB = sVector.rbegin ();

	while (sRItVecA != sVector.rend ())
	{
		sClMMTarget.template insert <ritvec_t> (sRItVecA, sRItVecB);

		sRItVecA++;
		sRItVecB++;
	}

	sVector.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::criter_t> (criter_t == criter_t)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMultiMapVectorFeed.template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());

	sVector.template insert <typename CBTreeMultiMap_VectorFeed_t::const_iterator> (sVector.begin (), sMultiMapVectorFeed.cbegin (), sMultiMapVectorFeed.cend ());

	sCRItVecA = sVector.crbegin ();
	sCRItVecB = sVector.crbegin ();

	while (sCRItVecA != sVector.crend ())
	{
		sClMMTarget.template insert <critvec_t> (sCRItVecA, sCRItVecB);

		sCRItVecA++;
		sCRItVecB++;
	}

	sVector.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::iter_t> (>iter_t, iter_t<)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMultiMapVectorFeed.template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());

	sVector.template insert <typename CBTreeMultiMap_VectorFeed_t::const_iterator> (sVector.begin (), sMultiMapVectorFeed.cbegin (), sMultiMapVectorFeed.cend ());

	sItVecA = sVector.begin ();
	sItVecB = sVector.end ();

	::std::advance<itvec_t, int> (sItVecA, nNumEntries / 4);
	::std::advance<itvec_t, int> (sItVecB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <itvec_t> (sItVecA, sItVecB);

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::citer_t> (>citer_t, citer_t<)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMultiMapVectorFeed.template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());

	sVector.template insert <typename CBTreeMultiMap_VectorFeed_t::const_iterator> (sVector.begin (), sMultiMapVectorFeed.cbegin (), sMultiMapVectorFeed.cend ());

	sCItVecA = sVector.cbegin ();
	sCItVecB = sVector.cend ();

	::std::advance<citvec_t, int> (sCItVecA, nNumEntries / 4);
	::std::advance<citvec_t, int> (sCItVecB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <citvec_t> (sCItVecA, sCItVecB);

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::riter_t> (>riter_t, riter_t<)" << endl;
	
	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMultiMapVectorFeed.template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());

	sVector.template insert <typename CBTreeMultiMap_VectorFeed_t::const_iterator> (sVector.begin (), sMultiMapVectorFeed.cbegin (), sMultiMapVectorFeed.cend ());

	sRItVecA = sVector.rbegin ();
	sRItVecB = sVector.rend ();

	::std::advance<ritvec_t, int> (sRItVecA, nNumEntries / 4);
	::std::advance<ritvec_t, int> (sRItVecB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <ritvec_t> (sRItVecA, sRItVecB);

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::criter_t> (>criter_t, criter_t<)" << endl;

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMultiMapVectorFeed.template insert <citer_t> (pClMM->cbegin (), pClMM->cend ());

	sVector.template insert <typename CBTreeMultiMap_VectorFeed_t::const_iterator> (sVector.begin (), sMultiMapVectorFeed.cbegin (), sMultiMapVectorFeed.cend ());

	sCRItVecA = sVector.crbegin ();
	sCRItVecB = sVector.crend ();

	::std::advance<critvec_t, int> (sCRItVecA, nNumEntries / 4);
	::std::advance<critvec_t, int> (sCRItVecB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <critvec_t> (sCRItVecA, sCRItVecB);

	sList.clear ();
	pClMM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<map::iter_t> (iter_t, iter_t)" << endl;

	multiMapPrim_add<::std::map<uint32_t, multiMapMap_t>, ::std::map<uint32_t, multiMapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMultiMapPair.template insert<itmap_t> (sMap.begin (), sMap.end ());

	sMultiMapPair.clear ();

	cout << "target::insert<map::citer_t> (citer_t, citer_t)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, multiMapMap_t>, ::std::map<uint32_t, multiMapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sMultiMapPair.template insert<citmap_t> (sMap.cbegin (), sMap.cend ());

	sMultiMapPair.clear ();

	cout << "target::insert<map::riter_t> (riter_t, riter_t)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, multiMapMap_t>, ::std::map<uint32_t, multiMapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sMultiMapPair.template insert<ritmap_t> (sMap.rbegin (), sMap.rend ());

	sMultiMapPair.clear ();

	cout << "target::insert<map::criter_t> (criter_t, criter_t)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, multiMapMap_t>, ::std::map<uint32_t, multiMapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sMultiMapPair.template insert<critmap_t> (sMap.crbegin (), sMap.crend ());

	sMultiMapPair.clear ();

	cout << "target::insert<map::iter_t> (iter_t == iter_t)" << endl;

	multiMapPrim_add<::std::map<uint32_t, multiMapMap_t>, ::std::map<uint32_t, multiMapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sItMapA = sMap.begin ();
	sItMapB = sMap.begin ();

	while (sItMapA != sMap.end ())
	{
		sMultiMapPair.template insert <itmap_t> (sItMapA, sItMapB);

		sItMapA++;
		sItMapB++;
	}

	sMultiMapPair.clear ();
	
	cout << "target::insert<map::citer_t> (citer_t == citer_t)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, multiMapMap_t>, ::std::map<uint32_t, multiMapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCItMapA = sMap.cbegin ();
	sCItMapB = sMap.cbegin ();

	while (sCItMapA != sMap.cend ())
	{
		sMultiMapPair.template insert <citmap_t> (sCItMapA, sCItMapB);

		sCItMapA++;
		sCItMapB++;
	}

	sMultiMapPair.clear ();
	
	cout << "target::insert<map::riter_t> (riter_t == riter_t)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, multiMapMap_t>, ::std::map<uint32_t, multiMapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sRItMapA = sMap.rbegin ();
	sRItMapB = sMap.rbegin ();

	while (sRItMapA != sMap.rend ())
	{
		sMultiMapPair.template insert <ritmap_t> (sRItMapA, sRItMapB);

		sRItMapA++;
		sRItMapB++;
	}

	sMultiMapPair.clear ();
	
	cout << "target::insert<map::criter_t> (criter_t == criter_t)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, multiMapMap_t>, ::std::map<uint32_t, multiMapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCRItMapA = sMap.crbegin ();
	sCRItMapB = sMap.crbegin ();

	while (sCRItMapA != sMap.crend ())
	{
		sMultiMapPair.template insert <critmap_t> (sCRItMapA, sCRItMapB);

		sCRItMapA++;
		sCRItMapB++;
	}

	sMultiMapPair.clear ();
	
	cout << "target::insert<map::iter_t> (>iter_t, iter_t<)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, multiMapMap_t>, ::std::map<uint32_t, multiMapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sItMapA = sMap.begin ();
	sItMapB = sMap.end ();

	::std::advance<itmap_t, int> (sItMapA, (int) (nNumEntries / 4));
	::std::advance<itmap_t, int> (sItMapB, (int) (0 - (nNumEntries / 4)));

	sMultiMapPair.template insert <itmap_t> (sItMapA, sItMapB);

	sMultiMapPair.clear ();

	cout << "target::insert<map::citer_t> (>citer_t, citer_t<)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, multiMapMap_t>, ::std::map<uint32_t, multiMapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCItMapA = sMap.cbegin ();
	sCItMapB = sMap.cend ();

	::std::advance<citmap_t, int> (sCItMapA, (int) (nNumEntries / 4));
	::std::advance<citmap_t, int> (sCItMapB, (int) (0 - (nNumEntries / 4)));

	sMultiMapPair.template insert <citmap_t> (sCItMapA, sCItMapB);

	sMultiMapPair.clear ();

	cout << "target::insert<map::riter_t> (>riter_t, riter_t<)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, multiMapMap_t>, ::std::map<uint32_t, multiMapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sRItMapA = sMap.rbegin ();
	sRItMapB = sMap.rend ();

	::std::advance<ritmap_t, int> (sRItMapA, (int) (nNumEntries / 4));
	::std::advance<ritmap_t, int> (sRItMapB, (int) (0 - (nNumEntries / 4)));

	sMultiMapPair.template insert <ritmap_t> (sRItMapA, sRItMapB);

	sMultiMapPair.clear ();

	cout << "target::insert<map::criter_t> (>criter_t, criter_t<)" << endl;

	multiMapPrim_add<::std::map<uint32_t, multiMapMap_t>, ::std::map<uint32_t, multiMapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCRItMapA = sMap.crbegin ();
	sCRItMapB = sMap.crend ();

	::std::advance<critmap_t, int> (sCRItMapA, (int) (nNumEntries / 4));
	::std::advance<critmap_t, int> (sCRItMapB, (int) (0 - (nNumEntries / 4)));

	sMultiMapPair.template insert <critmap_t> (sCRItMapA, sCRItMapB);

	sMultiMapPair.clear ();

	cout << "target::insert<multimap::iter_t> (iter_t, iter_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, multiMapMap_t>, ::std::multimap<uint32_t, multiMapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sMultiMapPair.template insert<itmmap_t> (sMMap.begin (), sMMap.end ());

	sMultiMapPair.clear ();

	cout << "target::insert<multimap::citer_t> (citer_t, citer_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, multiMapMap_t>, ::std::multimap<uint32_t, multiMapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sMultiMapPair.template insert<citmmap_t> (sMMap.cbegin (), sMMap.cend ());

	sMultiMapPair.clear ();

	cout << "target::insert<multimap::riter_t> (riter_t, riter_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, multiMapMap_t>, ::std::multimap<uint32_t, multiMapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sMultiMapPair.template insert<ritmmap_t> (sMMap.rbegin (), sMMap.rend ());

	sMultiMapPair.clear ();

	cout << "target::insert<multimap::criter_t> (criter_t, criter_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, multiMapMap_t>, ::std::multimap<uint32_t, multiMapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sMultiMapPair.template insert<critmmap_t> (sMMap.crbegin (), sMMap.crend ());

	sMultiMapPair.clear ();

	cout << "target::insert<multimap::iter_t> (iter_t == iter_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, multiMapMap_t>, ::std::multimap<uint32_t, multiMapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sItMMapA = sMMap.begin ();
	sItMMapB = sMMap.begin ();

	while (sItMMapA != sMMap.end ())
	{
		sMultiMapPair.template insert <itmmap_t> (sItMMapA, sItMMapB);

		sItMMapA++;
		sItMMapB++;
	}

	sMultiMapPair.clear ();
	
	cout << "target::insert<multimap::citer_t> (citer_t == citer_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, multiMapMap_t>, ::std::multimap<uint32_t, multiMapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCItMMapA = sMMap.cbegin ();
	sCItMMapB = sMMap.cbegin ();

	while (sCItMMapA != sMMap.cend ())
	{
		sMultiMapPair.template insert <citmmap_t> (sCItMMapA, sCItMMapB);

		sCItMMapA++;
		sCItMMapB++;
	}

	sMultiMapPair.clear ();
	
	cout << "target::insert<multimap::riter_t> (riter_t == riter_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, multiMapMap_t>, ::std::multimap<uint32_t, multiMapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sRItMMapA = sMMap.rbegin ();
	sRItMMapB = sMMap.rbegin ();

	while (sRItMMapA != sMMap.rend ())
	{
		sMultiMapPair.template insert <ritmmap_t> (sRItMMapA, sRItMMapB);

		sRItMMapA++;
		sRItMMapB++;
	}

	sMultiMapPair.clear ();
	
	cout << "target::insert<multimap::criter_t> (criter_t == criter_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, multiMapMap_t>, ::std::multimap<uint32_t, multiMapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCRItMMapA = sMMap.crbegin ();
	sCRItMMapB = sMMap.crbegin ();

	while (sCRItMMapA != sMMap.crend ())
	{
		sMultiMapPair.template insert <critmmap_t> (sCRItMMapA, sCRItMMapB);

		sCRItMMapA++;
		sCRItMMapB++;
	}

	sMultiMapPair.clear ();
	
	cout << "target::insert<multimap::iter_t> (>iter_t, iter_t<)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, multiMapMap_t>, ::std::multimap<uint32_t, multiMapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sItMMapA = sMMap.begin ();
	sItMMapB = sMMap.end ();

	::std::advance<itmmap_t, int> (sItMMapA, (int) (nNumEntries / 4));
	::std::advance<itmmap_t, int> (sItMMapB, (int) (0 - (nNumEntries / 4)));

	sMultiMapPair.template insert <itmmap_t> (sItMMapA, sItMMapB);

	sMultiMapPair.clear ();

	cout << "target::insert<multimap::citer_t> (>citer_t, citer_t<)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, multiMapMap_t>, ::std::multimap<uint32_t, multiMapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCItMMapA = sMMap.cbegin ();
	sCItMMapB = sMMap.cend ();

	::std::advance<citmmap_t, int> (sCItMMapA, (int) (nNumEntries / 4));
	::std::advance<citmmap_t, int> (sCItMMapB, (int) (0 - (nNumEntries / 4)));

	sMultiMapPair.template insert <citmmap_t> (sCItMMapA, sCItMMapB);

	sMultiMapPair.clear ();

	cout << "target::insert<multimap::riter_t> (>riter_t, riter_t<)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, multiMapMap_t>, ::std::multimap<uint32_t, multiMapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sRItMMapA = sMMap.rbegin ();
	sRItMMapB = sMMap.rend ();

	::std::advance<ritmmap_t, int> (sRItMMapA, (int) (nNumEntries / 4));
	::std::advance<ritmmap_t, int> (sRItMMapB, (int) (0 - (nNumEntries / 4)));

	sMultiMapPair.template insert <ritmmap_t> (sRItMMapA, sRItMMapB);

	sMultiMapPair.clear ();

	cout << "target::insert<multimap::criter_t> (>criter_t, criter_t<)" << endl;

	multiMapPrim_add<::std::multimap<uint32_t, multiMapMap_t>, ::std::multimap<uint32_t, multiMapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCRItMMapA = sMMap.crbegin ();
	sCRItMMapB = sMMap.crend ();

	::std::advance<critmmap_t, int> (sCRItMMapA, (int) (nNumEntries / 4));
	::std::advance<critmmap_t, int> (sCRItMMapB, (int) (0 - (nNumEntries / 4)));

	sMultiMapPair.template insert <critmmap_t> (sCRItMMapA, sCRItMMapB);

	sMultiMapPair.clear ();
}

template <class _t_multimap>
void TestBTreeMultiMapSTLifEraseViaIterator (_t_multimap *pClMM, uint32_t nNumEntries)
{
	typedef typename _t_multimap::const_iterator						citer_t;

	uint32_t									nLastKey = 0;
	citer_t										sIter;
	citer_t										sIterRslt;
	
	cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: erase (const_iterator)" << endl;

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	while (pClMM->size () > 0)
	{
		sIter = pClMM->cbegin ();

		sIter += (int) (generate_rand32 () % pClMM->size ());

		sIterRslt = pClMM->erase (sIter);

		if (sIter != sIterRslt)
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
	citer_t										sIter;
	uint32_t									i;
	typename _t_multimap::sizetype_t			nRslt;
	
	cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: erase (const keytype &)" << endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	}

	while (pClMM->size () > 0)
	{
		sIter = pClMM->cbegin ();

		sIter += (int) (generate_rand32 () % pClMM->size ());

		nRslt = pClMM->erase ((*sIter).first);

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
	typedef typename _t_multimap::const_iterator										citer_t;

	uint32_t									nLastKey;
	citer_t										sIterBegin;
	citer_t										sIterEnd;
	citer_t										sIterRslt;
	uint32_t									i;
	
	cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: erase (const_iterator, const_iterator)" << endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	}

	while (pClMM->size () > 0)
	{
		sIterBegin = pClMM->cbegin ();
		sIterEnd = pClMM->cend ();

		if (pClMM->size () > 1)
		{
			sIterBegin += (int) (generate_rand32 () % (pClMM->size () / 2));
			sIterEnd -= (int) (generate_rand32 () % (pClMM->size () / 2));
		}

		sIterRslt = pClMM->erase (sIterBegin, sIterEnd);

		if (sIterRslt != sIterBegin)
		{
			cerr << endl;
			cerr << "TestBTreeMultiMapSTLifEraseViaIteratorMultiple: returned iterator mismatches!" << endl;

			exit (-1);
		}
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

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClMM->begin ();
	sIterEnd = pClMM->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = ((typename _t_multimap::value_t)(*sIter)).first;

		sIterUpper = pClMM->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			nNextKey = ((typename _t_multimap::value_t)(*sIterUpper)).first;

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

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClMM->begin ();
	sIterEnd = pClMM->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = ((typename _t_multimap::value_t)(*sIter)).first;

		sIterUpper = pClMM->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			if (!sMMComp (((typename _t_multimap::value_t)(*sIter)), ((typename _t_multimap::value_t)(*sIterUpper))))
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
	_t_multimap						sClMultiMapTarget (*pClMM, false);
	_t_multimap						sClMultiMapRef (*pClMM, false);
	_t_multimap						sClMultiMapTargetRef (*pClMM, false);

	cout << "exercises method compatible to STL interface CBTreeMultiMap<>:: swap (CBTreeMultiMap &)" << endl;

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMultiMapRef.template insert<citer_t> (pClMM->cbegin (), pClMM->cend ());

	if ((*pClMM) != sClMultiMapRef)
	{
		cerr << "ERROR: Unexpected multimap mismatch!" << endl;

		exit (-1);
	}

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (&sClMultiMapTarget, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

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

	multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_ASCEND_KEY);

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

		multiMapPrim_add<_t_multimap, typename _t_multimap::value_t> (pClMM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_ASCEND_KEY);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sCIterLower = pClMM->lower_bound (i + 1);
		sCIterUpper = pClMM->upper_bound (i + 1);

		sCIterLower += (int) nInstance;

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

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeSTLmultiMap (uint32_t nTestNum, _t_subnodeiter nNodeSize, _t_datalayerproperties sDataProperties, bayerTreeCacheDescription_t &sCacheDesc, int argc, char **argv)
{
	typedef CBTreeTestMultiMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		CBTreeTestMultiMap_t;

	CBTreeTestMultiMap_t		*pClMM = new CBTreeTestMultiMap_t (sDataProperties, &sCacheDesc, nNodeSize);

	if (pClMM == NULL)
	{
		cerr << "TestBTreeSTLmultiMap: ERROR: insufficient memory!" << endl;

		exit (-1);
	}

	cout << "b-tree multi-map test bench selected" << endl;

	switch (nTestNum)
	{
	case BTREETEST_MULTIMAP_ASSIGNMENT_OPERATOR	:
		{
			TestBTreeMultiMapSTLifAssignmentOperator<CBTreeTestMultiMap_t> (pClMM, 64);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_INSERT	:
		{
			TestBTreeMultiMapSTLifInsert<CBTreeTestMultiMap_t> (pClMM, 64);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_INSERT_VIA_ITERATOR	:
		{
			TestBTreeMultiMapSTLifInsertViaIterator<CBTreeTestMultiMap_t> (pClMM, 64, sDataProperties, &sCacheDesc, nNodeSize);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_ERASE_VIA_ITERATOR	:
		{
			TestBTreeMultiMapSTLifEraseViaIterator<CBTreeTestMultiMap_t> (pClMM, 64);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_ERASE_VIA_KEY	:
		{
			TestBTreeMultiMapSTLifEraseViaKey<CBTreeTestMultiMap_t> (pClMM, 64, 5);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE	:
		{
			TestBTreeMultiMapSTLifEraseViaIteratorMultiple<CBTreeTestMultiMap_t> (pClMM, 64, 5);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_KEY_COMP	:
		{
			TestBTreeMultiMapSTLifKeyComp<CBTreeTestMultiMap_t> (pClMM, 64);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_VALUE_COMP	:
		{
			TestBTreeMultiMapSTLifValueComp<CBTreeTestMultiMap_t> (pClMM, 64);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_SWAP	:
		{
			TestBTreeMultiMapSTLifSwap<CBTreeTestMultiMap_t> (pClMM, 128);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_FIND	:
		{
			TestBTreeMultiMapSTLifFind<CBTreeTestMultiMap_t> (pClMM, 64);

			break;
		}
	
	case BTREETEST_MULTIMAP_STL_IF_LOWER_BOUND_UPPER_BOUND	:
		{
			TestBTreeKeySortSTLifLowerBoundUpperBound<CBTreeTestMultiMap_t> (pClMM, 64, 5);

			break;
		}
	
	default:
		{
			cerr << "ERROR: TestBTreeSTLmultiMap: Unknown test or test not specified!" << endl;

			break;
		}
	}

	delete pClMM;
}
