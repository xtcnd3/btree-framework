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

	_t_map									sClMM (*pClM, false);
	uint32_t									nLastKey = 0;
	citer_t										sIter;
	citer_t										sIterRslt;
	
	cout << "exercises assignment operator compatible to STL interface CBTreeMap<>:: operator= (CBTreeMap<> &)" << endl;

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

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

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastSeed, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
}

template <class _t_map>
void TestBTreeMapSTLifInsertViaIterator (_t_map *pClM, uint32_t nNumEntries, typename _t_map::datalayerproperties_t &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nNodeSize)
{
	typedef typename _t_map::value_t												value_t;

	typedef typename _t_map::iterator												iter_t;
	typedef typename _t_map::const_iterator										citer_t;
	typedef typename _t_map::reverse_iterator										riter_t;
	typedef typename _t_map::const_reverse_iterator								criter_t;

	typedef typename ::std::list<value_t>::iterator										itlist_t;
	typedef typename ::std::list<value_t>::const_iterator								citlist_t;
	typedef typename ::std::list<value_t>::reverse_iterator								ritlist_t;
	typedef typename ::std::list<value_t>::const_reverse_iterator						critlist_t;

	typedef typename ::std::vector<value_t>::iterator									itvec_t;
	typedef typename ::std::vector<value_t>::const_iterator								citvec_t;
	typedef typename ::std::vector<value_t>::reverse_iterator							ritvec_t;
	typedef typename ::std::vector<value_t>::const_reverse_iterator						critvec_t;

	typedef CBTreeMap<uint32_t, mapMap_t, size_t>								CBTreeMap_VectorFeed_t;

	typedef CBTreeTestMap<typename _t_map::sizetype_t, typename _t_map::nodeiter_t, typename _t_map::subnodeiter_t, typename _t_map::datalayerproperties_t, typename _t_map::datalayer_t>
																						CBTreeMapTest_Pair_t;

	typedef typename CBTreeMapTest_Pair_t::iterator								iter_pair_t;
	typedef typename CBTreeMapTest_Pair_t::const_iterator							citer_pair_t;
	typedef typename CBTreeMapTest_Pair_t::reverse_iterator						riter_pair_t;
	typedef typename CBTreeMapTest_Pair_t::const_reverse_iterator					criter_pair_t;

	typedef typename ::std::map<uint32_t, mapMap_t>::iterator						itmap_t;
	typedef typename ::std::map<uint32_t, mapMap_t>::const_iterator				citmap_t;
	typedef typename ::std::map<uint32_t, mapMap_t>::reverse_iterator				ritmap_t;
	typedef typename ::std::map<uint32_t, mapMap_t>::const_reverse_iterator		critmap_t;

	typedef typename ::std::multimap<uint32_t, mapMap_t>::iterator					itmmap_t;
	typedef typename ::std::multimap<uint32_t, mapMap_t>::const_iterator			citmmap_t;
	typedef typename ::std::multimap<uint32_t, mapMap_t>::reverse_iterator			ritmmap_t;
	typedef typename ::std::multimap<uint32_t, mapMap_t>::const_reverse_iterator	critmmap_t;
	
	_t_map									sClMMTarget (*pClM, false);
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
	CBTreeMap_VectorFeed_t					sMapVectorFeed (sRAMproperties, psCacheDescription, nNodeSize);
	CBTreeMapTest_Pair_t					sMapPair (rDataLayerProperties, psCacheDescription, nNodeSize);
	CBTreeMapTest_Pair_t					sMapTargetPair (sMapPair);
	value_t										sMapPairData;
	citer_pair_t								sCIterPairA, sCIterPairB;
	criter_pair_t								sCRIterPairA, sCRIterPairB;
	::std::map<uint32_t, mapMap_t>			sMap;
	itmap_t										sItMapA, sItMapB;
	citmap_t									sCItMapA, sCItMapB;
	ritmap_t									sRItMapA, sRItMapB;
	critmap_t									sCRItMapA, sCRItMapB;
	::std::multimap<uint32_t, mapMap_t>	sMMap;
	itmmap_t									sItMMapA, sItMMapB;
	citmmap_t									sCItMMapA, sCItMMapB;
	ritmmap_t									sRItMMapA, sRItMMapB;
	critmmap_t									sCRItMMapA, sCRItMMapB;
	
	cout << "exercises method compatible to STL interface CBTreeMap<>:: insert<_t_iterator> (_t_iterator, _t_iterator)" << endl;

#if 0 // for this code to compile, the b-tree framework under test must support const methods
// this is to be implemented

	cout << "target::insert<_t_obj::iter_t> (iter_t, iter_t)" << endl;

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMMTarget.template insert <iter_t> (pClM->begin (), pClM->end ());

	pClM->clear ();
	sClMMTarget.clear ();

#endif

	cout << "target::insert<_t_obj::citer_t> (citer_t, citer_t)" << endl;

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMMTarget.template insert <citer_t> (pClM->cbegin (), pClM->cend ());

	pClM->clear ();
	sClMMTarget.clear ();

#if 0 // for this code to compile, the b-tree framework under test must support const methods
// this is to be implemented

	cout << "target::insert<_t_obj::riter_t> (riter_t, riter_t)" << endl;

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMMTarget.template insert <riter_t> (pClM->rbegin (), pClM->rend ());

	pClM->clear ();
	sClMMTarget.clear ();

#endif

	cout << "target::insert<_t_obj::criter_t> (criter_t, criter_t)" << endl;

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMMTarget.template insert <criter_t> (pClM->crbegin (), pClM->crend ());

	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<_t_obj::citer_t> (citer_t == citer_t)" << endl;

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sCIterA = pClM->cbegin ();
	sCIterB = pClM->cbegin ();

	while (sCIterA != pClM->cend ())
	{
		sClMMTarget.template insert <citer_t> (sCIterA, sCIterB);

		sCIterA++;
		sCIterB++;
	}

	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<_t_obj::criter_t> (criter_t == criter_t)" << endl;

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sCRIterA = pClM->crbegin ();
	sCRIterB = pClM->crbegin ();

	while (sCRIterA != pClM->crend ())
	{
		sClMMTarget.template insert <criter_t> (sCRIterA, sCRIterB);

		sCRIterA++;
		sCRIterB++;
	}

	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<_t_obj::citer_t> (>citer_t, citer_t<)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sCIterA = pClM->cbegin ();
	sCIterB = pClM->cend ();

	sCIterA += (int) (nNumEntries / 4);
	sCIterB -= (int) (nNumEntries / 4);

	sClMMTarget.template insert <citer_t> (sCIterA, sCIterB);

	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<_t_obj::criter_t> (>criter_t, criter_t<)" << endl;

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sCRIterA = pClM->crbegin ();
	sCRIterB = pClM->crend ();

	sCRIterA += (int) (nNumEntries / 4);
	sCRIterB -= (int) (nNumEntries / 4);

	sClMMTarget.template insert <criter_t> (sCRIterA, sCRIterB);

	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<_t_obj::citer_t> (target::citer_t, target::citer_t)" << endl;

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	pClM->template insert <citer_t> (pClM->cbegin (), pClM->cend ());

	pClM->clear ();

	cout << "target::insert<_t_obj::criter_t> (target::criter_t, target::criter_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	pClM->template insert <criter_t> (pClM->crbegin (), pClM->crend ());

	pClM->clear ();

	cout << "target::insert<_t_obj::citer_t> (target::citer_t == target::citer_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sCIterA = pClM->cbegin ();
	sCIterB = pClM->cbegin ();

	while (sCIterA != pClM->cend ())
	{
		pClM->template insert <citer_t> (sCIterA, sCIterB);

		sCIterA++;
		sCIterB++;
	}

	pClM->clear ();

	cout << "target::insert<_t_obj::criter_t> (target::criter_t == target::criter_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sCRIterA = pClM->crbegin ();
	sCRIterB = pClM->crbegin ();

	while (sCRIterA != pClM->crend ())
	{
		pClM->template insert <criter_t> (sCRIterA, sCRIterB);

		sCRIterA++;
		sCRIterB++;
	}

	pClM->clear ();

	cout << "target::insert<_t_obj::citer_t> (>target::citer_t, target::citer_t<)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	pClM->template insert <citer_t> (pClM->cbegin (), pClM->cend ());
	pClM->template insert <citer_t> (pClM->cbegin (), pClM->cend ());
	pClM->template insert <citer_t> (pClM->cbegin (), pClM->cend ());
	pClM->template insert <citer_t> (pClM->cbegin (), pClM->cend ());

	sCIterA = pClM->cbegin ();
	sCIterB = pClM->cend ();

	sCIterA += (int) (nNumEntries / 4);
	sCIterB -= (int) (nNumEntries / 4);

	pClM->template insert <citer_t> (sCIterA, sCIterB);

	pClM->clear ();

	cout << "target::insert<_t_obj::criter_t> (>target::criter_t, target::criter_t<)" << endl;

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	pClM->template insert <criter_t> (pClM->crbegin (), pClM->crend ());
	pClM->template insert <criter_t> (pClM->crbegin (), pClM->crend ());
	pClM->template insert <criter_t> (pClM->crbegin (), pClM->crend ());
	pClM->template insert <criter_t> (pClM->crbegin (), pClM->crend ());

	sCRIterA = pClM->crbegin ();
	sCRIterB = pClM->crend ();

	sCRIterA += (int) (nNumEntries / 4);
	sCRIterB -= (int) (nNumEntries / 4);

	pClM->template insert <criter_t> (sCRIterA, sCRIterB);

	pClM->clear ();

	cout << "target::insert<list::iter_t> (iter_t, iter_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClM->cbegin (), pClM->cend ());

	sClMMTarget.template insert <itlist_t> (sList.begin (), sList.end ());

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::citer_t> (citer_t, citer_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClM->cbegin (), pClM->cend ());

	sClMMTarget.template insert <citlist_t> (sList.cbegin (), sList.cend ());

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::riter_t> (riter_t, riter_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClM->cbegin (), pClM->cend ());

	sClMMTarget.template insert <ritlist_t> (sList.rbegin (), sList.rend ());

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::criter_t> (criter_t, criter_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClM->cbegin (), pClM->cend ());

	sClMMTarget.template insert <critlist_t> (sList.crbegin (), sList.crend ());

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::iter_t> (iter_t == iter_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClM->cbegin (), pClM->cend ());

	sItListA = sList.begin ();
	sItListB = sList.begin ();

	while (sItListA != sList.end ())
	{
		sClMMTarget.template insert <itlist_t> (sItListA, sItListB);

		sItListA++;
		sItListB++;
	}

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::citer_t> (citer_t == citer_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClM->cbegin (), pClM->cend ());

	sCItListA = sList.cbegin ();
	sCItListB = sList.cbegin ();

	while (sCItListA != sList.cend ())
	{
		sClMMTarget.template insert <citlist_t> (sCItListA, sCItListB);

		sCItListA++;
		sCItListB++;
	}

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::riter_t> (riter_t == riter_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClM->cbegin (), pClM->cend ());

	sRItListA = sList.rbegin ();
	sRItListB = sList.rbegin ();

	while (sRItListA != sList.rend ())
	{
		sClMMTarget.template insert <ritlist_t> (sRItListA, sRItListB);

		sRItListA++;
		sRItListB++;
	}

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::criter_t> (criter_t == criter_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClM->cbegin (), pClM->cend ());

	sCRItListA = sList.crbegin ();
	sCRItListB = sList.crbegin ();

	while (sCRItListA != sList.crend ())
	{
		sClMMTarget.template insert <critlist_t> (sCRItListA, sCRItListB);

		sCRItListA++;
		sCRItListB++;
	}

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::iter_t> (>iter_t, iter_t<)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClM->cbegin (), pClM->cend ());

	sItListA = sList.begin ();
	sItListB = sList.end ();

	::std::advance<itlist_t, int> (sItListA, nNumEntries / 4);
	::std::advance<itlist_t, int> (sItListB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <itlist_t> (sItListA, sItListB);

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::citer_t> (>citer_t, citer_t<)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClM->cbegin (), pClM->cend ());

	sCItListA = sList.cbegin ();
	sCItListB = sList.cend ();

	::std::advance<citlist_t, int> (sCItListA, nNumEntries / 4);
	::std::advance<citlist_t, int> (sCItListB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <citlist_t> (sCItListA, sCItListB);

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::riter_t> (>riter_t, riter_t<)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClM->cbegin (), pClM->cend ());

	sRItListA = sList.rbegin ();
	sRItListB = sList.rend ();

	::std::advance<ritlist_t, int> (sRItListA, nNumEntries / 4);
	::std::advance<ritlist_t, int> (sRItListB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <ritlist_t> (sRItListA, sRItListB);

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<list::criter_t> (>criter_t, criter_t<)" << endl;

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClM->cbegin (), pClM->cend ());

	sCRItListA = sList.crbegin ();
	sCRItListB = sList.crend ();

	::std::advance<critlist_t, int> (sCRItListA, nNumEntries / 4);
	::std::advance<critlist_t, int> (sCRItListB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <critlist_t> (sCRItListA, sCRItListB);

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::iter_t> (iter_t, iter_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMapVectorFeed.template insert <citer_t> (pClM->cbegin (), pClM->cend ());

	sVector.template insert <typename CBTreeMap_VectorFeed_t::const_iterator> (sVector.begin (), sMapVectorFeed.cbegin (), sMapVectorFeed.cend ());

	sClMMTarget.template insert <itvec_t> (sVector.begin (), sVector.end ());

	sVector.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::citer_t> (citer_t, citer_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMapVectorFeed.template insert <citer_t> (pClM->cbegin (), pClM->cend ());

	sVector.template insert <typename CBTreeMap_VectorFeed_t::const_iterator> (sVector.begin (), sMapVectorFeed.cbegin (), sMapVectorFeed.cend ());

	sClMMTarget.template insert <citvec_t> (sVector.cbegin (), sVector.cend ());

	sVector.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::riter_t> (riter_t, riter_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMapVectorFeed.template insert <citer_t> (pClM->cbegin (), pClM->cend ());

	sVector.template insert <typename CBTreeMap_VectorFeed_t::const_iterator> (sVector.begin (), sMapVectorFeed.cbegin (), sMapVectorFeed.cend ());

	sClMMTarget.template insert <ritvec_t> (sVector.rbegin (), sVector.rend ());

	sVector.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::criter_t> (criter_t, criter_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMapVectorFeed.template insert <citer_t> (pClM->cbegin (), pClM->cend ());

	sVector.template insert <typename CBTreeMap_VectorFeed_t::const_iterator> (sVector.begin (), sMapVectorFeed.cbegin (), sMapVectorFeed.cend ());

	sClMMTarget.template insert <critvec_t> (sVector.crbegin (), sVector.crend ());

	sVector.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::iter_t> (iter_t == iter_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMapVectorFeed.template insert <citer_t> (pClM->cbegin (), pClM->cend ());

	sVector.template insert <typename CBTreeMap_VectorFeed_t::const_iterator> (sVector.begin (), sMapVectorFeed.cbegin (), sMapVectorFeed.cend ());

	sItVecA = sVector.begin ();
	sItVecB = sVector.begin ();

	while (sItVecA != sVector.end ())
	{
		sClMMTarget.template insert <itvec_t> (sItVecA, sItVecB);

		sItVecA++;
		sItVecB++;
	}

	sVector.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::citer_t> (citer_t == citer_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMapVectorFeed.template insert <citer_t> (pClM->cbegin (), pClM->cend ());

	sVector.template insert <typename CBTreeMap_VectorFeed_t::const_iterator> (sVector.begin (), sMapVectorFeed.cbegin (), sMapVectorFeed.cend ());

	sCItVecA = sVector.cbegin ();
	sCItVecB = sVector.cbegin ();

	while (sCItVecA != sVector.cend ())
	{
		sClMMTarget.template insert <citvec_t> (sCItVecA, sCItVecB);

		sCItVecA++;
		sCItVecB++;
	}

	sVector.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::riter_t> (riter_t == riter_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMapVectorFeed.template insert <citer_t> (pClM->cbegin (), pClM->cend ());

	sVector.template insert <typename CBTreeMap_VectorFeed_t::const_iterator> (sVector.begin (), sMapVectorFeed.cbegin (), sMapVectorFeed.cend ());

	sRItVecA = sVector.rbegin ();
	sRItVecB = sVector.rbegin ();

	while (sRItVecA != sVector.rend ())
	{
		sClMMTarget.template insert <ritvec_t> (sRItVecA, sRItVecB);

		sRItVecA++;
		sRItVecB++;
	}

	sVector.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::criter_t> (criter_t == criter_t)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMapVectorFeed.template insert <citer_t> (pClM->cbegin (), pClM->cend ());

	sVector.template insert <typename CBTreeMap_VectorFeed_t::const_iterator> (sVector.begin (), sMapVectorFeed.cbegin (), sMapVectorFeed.cend ());

	sCRItVecA = sVector.crbegin ();
	sCRItVecB = sVector.crbegin ();

	while (sCRItVecA != sVector.crend ())
	{
		sClMMTarget.template insert <critvec_t> (sCRItVecA, sCRItVecB);

		sCRItVecA++;
		sCRItVecB++;
	}

	sVector.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::iter_t> (>iter_t, iter_t<)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMapVectorFeed.template insert <citer_t> (pClM->cbegin (), pClM->cend ());

	sVector.template insert <typename CBTreeMap_VectorFeed_t::const_iterator> (sVector.begin (), sMapVectorFeed.cbegin (), sMapVectorFeed.cend ());

	sItVecA = sVector.begin ();
	sItVecB = sVector.end ();

	::std::advance<itvec_t, int> (sItVecA, nNumEntries / 4);
	::std::advance<itvec_t, int> (sItVecB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <itvec_t> (sItVecA, sItVecB);

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::citer_t> (>citer_t, citer_t<)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMapVectorFeed.template insert <citer_t> (pClM->cbegin (), pClM->cend ());

	sVector.template insert <typename CBTreeMap_VectorFeed_t::const_iterator> (sVector.begin (), sMapVectorFeed.cbegin (), sMapVectorFeed.cend ());

	sCItVecA = sVector.cbegin ();
	sCItVecB = sVector.cend ();

	::std::advance<citvec_t, int> (sCItVecA, nNumEntries / 4);
	::std::advance<citvec_t, int> (sCItVecB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <citvec_t> (sCItVecA, sCItVecB);

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::riter_t> (>riter_t, riter_t<)" << endl;
	
	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMapVectorFeed.template insert <citer_t> (pClM->cbegin (), pClM->cend ());

	sVector.template insert <typename CBTreeMap_VectorFeed_t::const_iterator> (sVector.begin (), sMapVectorFeed.cbegin (), sMapVectorFeed.cend ());

	sRItVecA = sVector.rbegin ();
	sRItVecB = sVector.rend ();

	::std::advance<ritvec_t, int> (sRItVecA, nNumEntries / 4);
	::std::advance<ritvec_t, int> (sRItVecB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <ritvec_t> (sRItVecA, sRItVecB);

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<vector::criter_t> (>criter_t, criter_t<)" << endl;

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMapVectorFeed.template insert <citer_t> (pClM->cbegin (), pClM->cend ());

	sVector.template insert <typename CBTreeMap_VectorFeed_t::const_iterator> (sVector.begin (), sMapVectorFeed.cbegin (), sMapVectorFeed.cend ());

	sCRItVecA = sVector.crbegin ();
	sCRItVecB = sVector.crend ();

	::std::advance<critvec_t, int> (sCRItVecA, nNumEntries / 4);
	::std::advance<critvec_t, int> (sCRItVecB, 0 - (nNumEntries / 4));

	sClMMTarget.template insert <critvec_t> (sCRItVecA, sCRItVecB);

	sList.clear ();
	pClM->clear ();
	sClMMTarget.clear ();

	cout << "target::insert<map::iter_t> (iter_t, iter_t)" << endl;

	multiMapPrim_add<::std::map<uint32_t, mapMap_t>, ::std::map<uint32_t, mapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sMapPair.template insert<itmap_t> (sMap.begin (), sMap.end ());

	sMapPair.clear ();

	cout << "target::insert<map::citer_t> (citer_t, citer_t)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, mapMap_t>, ::std::map<uint32_t, mapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sMapPair.template insert<citmap_t> (sMap.cbegin (), sMap.cend ());

	sMapPair.clear ();

	cout << "target::insert<map::riter_t> (riter_t, riter_t)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, mapMap_t>, ::std::map<uint32_t, mapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sMapPair.template insert<ritmap_t> (sMap.rbegin (), sMap.rend ());

	sMapPair.clear ();

	cout << "target::insert<map::criter_t> (criter_t, criter_t)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, mapMap_t>, ::std::map<uint32_t, mapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sMapPair.template insert<critmap_t> (sMap.crbegin (), sMap.crend ());

	sMapPair.clear ();

	cout << "target::insert<map::iter_t> (iter_t == iter_t)" << endl;

	multiMapPrim_add<::std::map<uint32_t, mapMap_t>, ::std::map<uint32_t, mapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sItMapA = sMap.begin ();
	sItMapB = sMap.begin ();

	while (sItMapA != sMap.end ())
	{
		sMapPair.template insert <itmap_t> (sItMapA, sItMapB);

		sItMapA++;
		sItMapB++;
	}

	sMapPair.clear ();
	
	cout << "target::insert<map::citer_t> (citer_t == citer_t)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, mapMap_t>, ::std::map<uint32_t, mapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCItMapA = sMap.cbegin ();
	sCItMapB = sMap.cbegin ();

	while (sCItMapA != sMap.cend ())
	{
		sMapPair.template insert <citmap_t> (sCItMapA, sCItMapB);

		sCItMapA++;
		sCItMapB++;
	}

	sMapPair.clear ();
	
	cout << "target::insert<map::riter_t> (riter_t == riter_t)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, mapMap_t>, ::std::map<uint32_t, mapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sRItMapA = sMap.rbegin ();
	sRItMapB = sMap.rbegin ();

	while (sRItMapA != sMap.rend ())
	{
		sMapPair.template insert <ritmap_t> (sRItMapA, sRItMapB);

		sRItMapA++;
		sRItMapB++;
	}

	sMapPair.clear ();
	
	cout << "target::insert<map::criter_t> (criter_t == criter_t)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, mapMap_t>, ::std::map<uint32_t, mapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCRItMapA = sMap.crbegin ();
	sCRItMapB = sMap.crbegin ();

	while (sCRItMapA != sMap.crend ())
	{
		sMapPair.template insert <critmap_t> (sCRItMapA, sCRItMapB);

		sCRItMapA++;
		sCRItMapB++;
	}

	sMapPair.clear ();
	
	cout << "target::insert<map::iter_t> (>iter_t, iter_t<)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, mapMap_t>, ::std::map<uint32_t, mapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sItMapA = sMap.begin ();
	sItMapB = sMap.end ();

	::std::advance<itmap_t, int> (sItMapA, (int) (nNumEntries / 4));
	::std::advance<itmap_t, int> (sItMapB, (int) (0 - (nNumEntries / 4)));

	sMapPair.template insert <itmap_t> (sItMapA, sItMapB);

	sMapPair.clear ();

	cout << "target::insert<map::citer_t> (>citer_t, citer_t<)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, mapMap_t>, ::std::map<uint32_t, mapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCItMapA = sMap.cbegin ();
	sCItMapB = sMap.cend ();

	::std::advance<citmap_t, int> (sCItMapA, (int) (nNumEntries / 4));
	::std::advance<citmap_t, int> (sCItMapB, (int) (0 - (nNumEntries / 4)));

	sMapPair.template insert <citmap_t> (sCItMapA, sCItMapB);

	sMapPair.clear ();

	cout << "target::insert<map::riter_t> (>riter_t, riter_t<)" << endl;
	
	multiMapPrim_add<::std::map<uint32_t, mapMap_t>, ::std::map<uint32_t, mapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sRItMapA = sMap.rbegin ();
	sRItMapB = sMap.rend ();

	::std::advance<ritmap_t, int> (sRItMapA, (int) (nNumEntries / 4));
	::std::advance<ritmap_t, int> (sRItMapB, (int) (0 - (nNumEntries / 4)));

	sMapPair.template insert <ritmap_t> (sRItMapA, sRItMapB);

	sMapPair.clear ();

	cout << "target::insert<map::criter_t> (>criter_t, criter_t<)" << endl;

	multiMapPrim_add<::std::map<uint32_t, mapMap_t>, ::std::map<uint32_t, mapMap_t>::value_type> (&sMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCRItMapA = sMap.crbegin ();
	sCRItMapB = sMap.crend ();

	::std::advance<critmap_t, int> (sCRItMapA, (int) (nNumEntries / 4));
	::std::advance<critmap_t, int> (sCRItMapB, (int) (0 - (nNumEntries / 4)));

	sMapPair.template insert <critmap_t> (sCRItMapA, sCRItMapB);

	sMapPair.clear ();

	cout << "target::insert<multimap::iter_t> (iter_t, iter_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, mapMap_t>, ::std::multimap<uint32_t, mapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sMapPair.template insert<itmmap_t> (sMMap.begin (), sMMap.end ());

	sMapPair.clear ();

	cout << "target::insert<multimap::citer_t> (citer_t, citer_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, mapMap_t>, ::std::multimap<uint32_t, mapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sMapPair.template insert<citmmap_t> (sMMap.cbegin (), sMMap.cend ());

	sMapPair.clear ();

	cout << "target::insert<multimap::riter_t> (riter_t, riter_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, mapMap_t>, ::std::multimap<uint32_t, mapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sMapPair.template insert<ritmmap_t> (sMMap.rbegin (), sMMap.rend ());

	sMapPair.clear ();

	cout << "target::insert<multimap::criter_t> (criter_t, criter_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, mapMap_t>, ::std::multimap<uint32_t, mapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sMapPair.template insert<critmmap_t> (sMMap.crbegin (), sMMap.crend ());

	sMapPair.clear ();

	cout << "target::insert<multimap::iter_t> (iter_t == iter_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, mapMap_t>, ::std::multimap<uint32_t, mapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sItMMapA = sMMap.begin ();
	sItMMapB = sMMap.begin ();

	while (sItMMapA != sMMap.end ())
	{
		sMapPair.template insert <itmmap_t> (sItMMapA, sItMMapB);

		sItMMapA++;
		sItMMapB++;
	}

	sMapPair.clear ();
	
	cout << "target::insert<multimap::citer_t> (citer_t == citer_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, mapMap_t>, ::std::multimap<uint32_t, mapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCItMMapA = sMMap.cbegin ();
	sCItMMapB = sMMap.cbegin ();

	while (sCItMMapA != sMMap.cend ())
	{
		sMapPair.template insert <citmmap_t> (sCItMMapA, sCItMMapB);

		sCItMMapA++;
		sCItMMapB++;
	}

	sMapPair.clear ();
	
	cout << "target::insert<multimap::riter_t> (riter_t == riter_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, mapMap_t>, ::std::multimap<uint32_t, mapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sRItMMapA = sMMap.rbegin ();
	sRItMMapB = sMMap.rbegin ();

	while (sRItMMapA != sMMap.rend ())
	{
		sMapPair.template insert <ritmmap_t> (sRItMMapA, sRItMMapB);

		sRItMMapA++;
		sRItMMapB++;
	}

	sMapPair.clear ();
	
	cout << "target::insert<multimap::criter_t> (criter_t == criter_t)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, mapMap_t>, ::std::multimap<uint32_t, mapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCRItMMapA = sMMap.crbegin ();
	sCRItMMapB = sMMap.crbegin ();

	while (sCRItMMapA != sMMap.crend ())
	{
		sMapPair.template insert <critmmap_t> (sCRItMMapA, sCRItMMapB);

		sCRItMMapA++;
		sCRItMMapB++;
	}

	sMapPair.clear ();
	
	cout << "target::insert<multimap::iter_t> (>iter_t, iter_t<)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, mapMap_t>, ::std::multimap<uint32_t, mapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sItMMapA = sMMap.begin ();
	sItMMapB = sMMap.end ();

	::std::advance<itmmap_t, int> (sItMMapA, (int) (nNumEntries / 4));
	::std::advance<itmmap_t, int> (sItMMapB, (int) (0 - (nNumEntries / 4)));

	sMapPair.template insert <itmmap_t> (sItMMapA, sItMMapB);

	sMapPair.clear ();

	cout << "target::insert<multimap::citer_t> (>citer_t, citer_t<)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, mapMap_t>, ::std::multimap<uint32_t, mapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCItMMapA = sMMap.cbegin ();
	sCItMMapB = sMMap.cend ();

	::std::advance<citmmap_t, int> (sCItMMapA, (int) (nNumEntries / 4));
	::std::advance<citmmap_t, int> (sCItMMapB, (int) (0 - (nNumEntries / 4)));

	sMapPair.template insert <citmmap_t> (sCItMMapA, sCItMMapB);

	sMapPair.clear ();

	cout << "target::insert<multimap::riter_t> (>riter_t, riter_t<)" << endl;
	
	multiMapPrim_add<::std::multimap<uint32_t, mapMap_t>, ::std::multimap<uint32_t, mapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sRItMMapA = sMMap.rbegin ();
	sRItMMapB = sMMap.rend ();

	::std::advance<ritmmap_t, int> (sRItMMapA, (int) (nNumEntries / 4));
	::std::advance<ritmmap_t, int> (sRItMMapB, (int) (0 - (nNumEntries / 4)));

	sMapPair.template insert <ritmmap_t> (sRItMMapA, sRItMMapB);

	sMapPair.clear ();

	cout << "target::insert<multimap::criter_t> (>criter_t, criter_t<)" << endl;

	multiMapPrim_add<::std::multimap<uint32_t, mapMap_t>, ::std::multimap<uint32_t, mapMap_t>::value_type> (&sMMap, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	
	sCRItMMapA = sMMap.crbegin ();
	sCRItMMapB = sMMap.crend ();

	::std::advance<critmmap_t, int> (sCRItMMapA, (int) (nNumEntries / 4));
	::std::advance<critmmap_t, int> (sCRItMMapB, (int) (0 - (nNumEntries / 4)));

	sMapPair.template insert <critmmap_t> (sCRItMMapA, sCRItMMapB);

	sMapPair.clear ();
}

template <class _t_map>
void TestBTreeMapSTLifEraseViaIterator (_t_map *pClM, uint32_t nNumEntries)
{
	typedef typename _t_map::const_iterator						citer_t;

	uint32_t									nLastKey = 0;
	citer_t										sIter;
	citer_t										sIterRslt;
	
	cout << "exercises method compatible to STL interface CBTreeMap<>:: erase (const_iterator)" << endl;

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	while (pClM->size () > 0)
	{
		sIter = pClM->cbegin ();

		sIter += (int) (generate_rand32 () % pClM->size ());

		sIterRslt = pClM->erase (sIter);

		if (sIter != sIterRslt)
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
	typedef typename _t_map::const_iterator										citer_t;
	
	uint32_t									nLastKey;
	citer_t										sIter;
	uint32_t									i;
	typename _t_map::sizetype_t			nRslt;
	
	cout << "exercises method compatible to STL interface CBTreeMap<>:: erase (const keytype &)" << endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	}

	while (pClM->size () > 0)
	{
		sIter = pClM->cbegin ();

		sIter += (int) (generate_rand32 () % pClM->size ());

		nRslt = pClM->erase ((*sIter).first);

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
	typedef typename _t_map::const_iterator										citer_t;

	uint32_t									nLastKey;
	citer_t										sIterBegin;
	citer_t										sIterEnd;
	citer_t										sIterRslt;
	uint32_t									i;
	
	cout << "exercises method compatible to STL interface CBTreeMap<>:: erase (const_iterator, const_iterator)" << endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
	}

	while (pClM->size () > 0)
	{
		sIterBegin = pClM->cbegin ();
		sIterEnd = pClM->cend ();

		if (pClM->size () > 1)
		{
			sIterBegin += (int) (generate_rand32 () % (pClM->size () / 2));
			sIterEnd -= (int) (generate_rand32 () % (pClM->size () / 2));
		}

		sIterRslt = pClM->erase (sIterBegin, sIterEnd);

		if (sIterRslt != sIterBegin)
		{
			cerr << endl;
			cerr << "TestBTreeMapSTLifEraseViaIteratorMultiple: returned iterator mismatches!" << endl;

			exit (-1);
		}
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

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClM->begin ();
	sIterEnd = pClM->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = ((typename _t_map::value_t)(*sIter)).first;

		sIterUpper = pClM->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			nNextKey = ((typename _t_map::value_t)(*sIterUpper)).first;

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

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sIterBegin = pClM->begin ();
	sIterEnd = pClM->end ();

	for (sIter = sIterBegin; sIter != sIterEnd; sIter++)
	{
		nKey = ((typename _t_map::value_t)(*sIter)).first;

		sIterUpper = pClM->upper_bound (nKey);

		if (sIterUpper != sIterEnd)
		{
			if (!sMComp (((typename _t_map::value_t)(*sIter)), ((typename _t_map::value_t)(*sIterUpper))))
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

	uint32_t						nLastKey = 0;
	_t_map						sClMapTarget (*pClM, false);
	_t_map						sClMapRef (*pClM, false);
	_t_map						sClMapTargetRef (*pClM, false);

	cout << "exercises method compatible to STL interface CBTreeMap<>:: swap (CBTreeMap &)" << endl;

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMapRef.template insert<citer_t> (pClM->cbegin (), pClM->cend ());

	if ((*pClM) != sClMapRef)
	{
		cerr << "ERROR: Unexpected multimap mismatch!" << endl;

		exit (-1);
	}

	multiMapPrim_add<_t_map, typename _t_map::value_t> (&sClMapTarget, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);

	sClMapTargetRef.template insert<citer_t> (sClMapTarget.cbegin (), sClMapTarget.cend ());

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

	multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_ASCEND_KEY);

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

		multiMapPrim_add<_t_map, typename _t_map::value_t> (pClM, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_ASCEND_KEY);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sCIterLower = pClM->lower_bound (i + 1);
		sCIterUpper = pClM->upper_bound (i + 1);

		sCIterLower += (int) 1;

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

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeSTLmap (uint32_t nTestNum, _t_subnodeiter nNodeSize, _t_datalayerproperties sDataProperties, bayerTreeCacheDescription_t &sCacheDesc, int argc, char **argv)
{
	typedef CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		CBTreeTestMap_t;

	CBTreeTestMap_t		*pClM = new CBTreeTestMap_t (sDataProperties, &sCacheDesc, nNodeSize);

	if (pClM == NULL)
	{
		cerr << "TestBTreeSTLmap: ERROR: insufficient memory!" << endl;

		exit (-1);
	}

	cout << "b-tree multi-map test bench selected" << endl;

	switch (nTestNum)
	{
	case BTREETEST_MAP_ASSIGNMENT_OPERATOR	:
		{
			TestBTreeMapSTLifAssignmentOperator<CBTreeTestMap_t> (pClM, 64);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_INSERT	:
		{
			TestBTreeMapSTLifInsert<CBTreeTestMap_t> (pClM, 64);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_INSERT_VIA_ITERATOR	:
		{
			TestBTreeMapSTLifInsertViaIterator<CBTreeTestMap_t> (pClM, 64, sDataProperties, &sCacheDesc, nNodeSize);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_ERASE_VIA_ITERATOR	:
		{
			TestBTreeMapSTLifEraseViaIterator<CBTreeTestMap_t> (pClM, 64);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_ERASE_VIA_KEY	:
		{
			TestBTreeMapSTLifEraseViaKey<CBTreeTestMap_t> (pClM, 64, 5);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE	:
		{
			TestBTreeMapSTLifEraseViaIteratorMultiple<CBTreeTestMap_t> (pClM, 64, 5);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_KEY_COMP	:
		{
			TestBTreeMapSTLifKeyComp<CBTreeTestMap_t> (pClM, 64);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_VALUE_COMP	:
		{
			TestBTreeMapSTLifValueComp<CBTreeTestMap_t> (pClM, 64);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_SWAP	:
		{
			TestBTreeMapSTLifSwap<CBTreeTestMap_t> (pClM, 128);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_FIND	:
		{
			TestBTreeMapSTLifFind<CBTreeTestMap_t> (pClM, 64);

			break;
		}
	
	case BTREETEST_MAP_STL_IF_LOWER_BOUND_UPPER_BOUND	:
		{
			TestBTreeMapSTLifLowerBoundUpperBound<CBTreeTestMap_t> (pClM, 64, 5);

			break;
		}
	
	default:
		{
			cerr << "ERROR: TestBTreeSTLmap: Unknown test or test not specified!" << endl;

			break;
		}
	}

	delete pClM;
}
