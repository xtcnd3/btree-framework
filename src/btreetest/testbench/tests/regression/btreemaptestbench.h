/************************************************************
**
** file:	btreemaptestbench.h
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

#ifndef BTREEMAPTESTBENCH_H
#define	BTREEMAPTESTBENCH_H

#include <stdint.h>

#include <map>
#include <list>
#include <vector>

#include <type_traits>

#include "testbench/application_classes/regression/btreetestmap.h"

#include "testbench/primitives/btreemultimapprimitives.h"

#include "testbench/common/btreetestcommon.h"
#include "specific_data_classes/btreemap.h"

template<class _t_key, class _t_map>
class CBTreeTestBenchMultiMap;

typedef enum
{
	BTREETEST_MAP_ASSIGNMENT_OPERATOR, 
	BTREETEST_MAP_STL_IF_INSERT = 1000, 
	BTREETEST_MAP_STL_IF_INSERT_VIA_ITERATOR, 
	BTREETEST_MAP_STL_IF_ERASE_VIA_ITERATOR, 
	BTREETEST_MAP_STL_IF_ERASE_VIA_KEY, 
	BTREETEST_MAP_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE, 
	BTREETEST_MAP_STL_IF_KEY_COMP, 
	BTREETEST_MAP_STL_IF_VALUE_COMP, 
	BTREETEST_MAP_STL_IF_SWAP, 
	BTREETEST_MAP_STL_IF_FIND, 
	BTREETEST_MAP_STL_IF_LOWER_BOUND_UPPER_BOUND
} btreetest_map_t;

typedef enum
{
	BTREETEST_MAP_STL_IF_INSERT_VIA_ITERATOR_PART, 
	BTREETEST_MAP_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, 
	BTREETEST_MAP_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, 
	BTREETEST_MAP_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, 
	BTREETEST_MAP_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, 
} btreetest_map_stl_if_insert_via_iterator_e;

template<class _t_key, class _t_map>
class CBTreeTestBenchMap	:	public ::std::map<_t_key, _t_map>
{
public:

				CBTreeTestBenchMap<_t_key, _t_map> ()	
					:	::std::map<_t_key, _t_map> () 
				{};

				CBTreeTestBenchMap<_t_key, _t_map> (const CBTreeTestBenchMap &rMap)	
					:	::std::map<_t_key, _t_map> (rMap) 
				{};

	explicit	CBTreeTestBenchMap<_t_key, _t_map> (const ::std::map<_t_key, _t_map> &rMap)	
					:	::std::map<_t_key, _t_map> (rMap) 
				{};

				~CBTreeTestBenchMap<_t_key, _t_map> ()	
				{};

	template<class _t_iterator>
	void insert (const typename ::std::map<_t_key, _t_map>::iterator &rDummyIter, _t_iterator &rIterFirst, _t_iterator &rIterLast)
	{
		rDummyIter;

		::std::map<_t_key, _t_map>::insert (rIterFirst, rIterLast);
	};

	template<class _t_iterator>
	void insert (const typename ::std::map<_t_key, _t_map>::const_iterator &rDummyIter, _t_iterator &rIterFirst, _t_iterator &rIterLast)
	{
		rDummyIter;

		::std::map<_t_key, _t_map>::insert (rIterFirst, rIterLast);
	};

	template<class _t_iterator>
	void insert (_t_iterator &rIterFirst, _t_iterator &rIterLast)
	{
		::std::map<_t_key, _t_map>::insert (rIterFirst, rIterLast);
	};

	// this version of insert only returns an iterator, instead of pair<iterator, bool>, although
	// this class is derived from ::std::map
	// - the masking of the bool component of this method has been done to make it interface compatible
	//   with any other associative container class, when set as a template parameter
	typename ::std::map<_t_key, _t_map>::iterator insert (const typename ::std::map<_t_key, _t_map>::value_type &rVal)
	{
		::std::pair<typename ::std::map<_t_key, _t_map>::iterator, bool>	sRetval;

		if (::std::map<_t_key, _t_map>::count (rVal.first) == 0)
		{
			sRetval = ::std::map<_t_key, _t_map>::insert (rVal);

			return (sRetval.first);
		}
		else
		{
			return (::std::map<_t_key, _t_map>::end ());
		}
	};
};

typedef CBTreeTestBenchMap<uint32_t, mapMap_t>				map_reference_t;

template<class _t_container, class _t_iterator>
void TestBTreeMapSTLifInsertViaIteratorPart (const char *pszTitle, int nArg, uint32_t nNumInstances, uint32_t nNumEntries, _t_container *pContainer, _t_iterator &rIter)
{
	uint32_t					nLastKey = 0;
	uint32_t					i;
	_t_iterator					sIterBegin;
	_t_iterator					sIterEnd;
	
	::std::cout << pszTitle << ::std::endl << ::std::flush;

	for (i = 0; i < nNumInstances; i++)
	{
		multiMapPrim_add (pContainer, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
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
void TestBTreeMapSTLifInsertViaIteratorPart (const char *pszTitle, int nArg, uint32_t nNumInstances, uint32_t nNumEntries, _t_dest_container *pDestContainer, _t_src_container *pSrcContainer, _t_dest_iterator &rDestIter, _t_src_iterator &rSrcIter)
{
	uint32_t					nLastKey = 0;
	uint32_t					i;
	_t_src_iterator				sSrcIterBegin;
	_t_src_iterator				sSrcIterEnd;
	
	::std::cout << pszTitle << ::std::endl << ::std::flush;

	for (i = 0; i < nNumInstances; i++)
	{
		multiMapPrim_add (pSrcContainer, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
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
void TestBTreeMapSTLifInsertViaIteratorSame (const char *pszTitle, int nArg, uint32_t nNumInstances, uint32_t nNumEntries, _t_dest_container *pDestContainer, _t_src_container *pSrcContainer, _t_dest_iterator &rDestIter, _t_src_iterator &rSrcIter)
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
		multiMapPrim_add (pSrcContainer, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
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
			::std::cerr << "TestBTreeMapSTLifInsertViaIteratorSame: destination container ought to be empty!" << ::std::endl;

			exit (-1);
		}
	}
	else
	{
		if (pDestContainer->size () != pSrcContainer->size ())
		{
			::std::cerr << "TestBTreeMapSTLifInsertViaIteratorSame: destination container and source container mismatch in size!" << ::std::endl;

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
			::std::cerr << "TestBTreeMapSTLifInsertViaIteratorSame: destination container ought to be empty! (2)" << ::std::endl;

			exit (-1);
		}
	}
	else
	{
		if (pDestContainer->size () != pSrcContainer->size ())
		{
			::std::cerr << "TestBTreeMapSTLifInsertViaIteratorSame: destination container and source container mismatch in size! (2)" << ::std::endl;

			exit (-1);
		}
	}

	pDestContainer->clear ();
	pSrcContainer->clear ();
}

template<class _t_dest_container, class _t_src_container, class _t_ext_container, class _t_dest_iterator, class _t_src_iterator, class _t_ext_iterator>
void TestBTreeMapSTLifInsertViaIteratorPartExtern (const char *pszTitle, int nArg, uint32_t nNumInstances, uint32_t nNumEntries, _t_dest_container *pDestContainer, _t_src_container *pSrcContainer, _t_ext_container *pExtContainer, _t_dest_iterator &rDestIter, _t_src_iterator &rSrcIter, _t_ext_iterator &rExtIter)
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
		multiMapPrim_add (pSrcContainer, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
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

template<class _t_container, class _t_iterator, class _t_ext_iterator>
void generic_insert (_t_container *pContainer, const _t_iterator &rIterPos, _t_ext_iterator &rExtFirst, _t_ext_iterator &rExtLast, ::std::true_type)
{
	rIterPos;

	pContainer->insert (rExtFirst, rExtLast);
}

template<class _t_container, class _t_iterator, class _t_ext_iterator>
void generic_insert (_t_container *pContainer, const _t_iterator &rIterPos, _t_ext_iterator &rExtFirst, _t_ext_iterator &rExtLast, ::std::false_type)
{
//	pContainer->insert (rIterPos, rExtFirst, rExtLast);
	container_insert (pContainer, rIterPos, rExtFirst, rExtLast);
}

template<class _t_dest_container, class _t_src_container, class _t_ext_container, class _t_dest_iterator, class _t_src_iterator, class _t_ext_iterator>
void TestBTreeMapSTLifInsertViaIteratorSameExtern (const char *pszTitle, int nArg, uint32_t nNumInstances, uint32_t nNumEntries, _t_dest_container *pDestContainer, _t_src_container *pSrcContainer, _t_ext_container *pExtContainer, _t_dest_iterator &rDestIter, _t_src_iterator &rSrcIter, _t_ext_iterator &rExtIter)
{
	typedef typename ::std::map<typename _t_ext_iterator::value_type::first_type, typename _t_ext_iterator::value_type::second_type>		map_t;
	typedef typename ::std::multimap<typename _t_ext_iterator::value_type::first_type, typename _t_ext_iterator::value_type::second_type>	mmap_t;

	typename ::std::conditional< ::std::is_same<_t_ext_container, map_t> ::value || ::std::is_same<_t_ext_container, mmap_t> ::value, ::std::true_type, ::std::false_type> ::type
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
		multiMapPrim_add (pSrcContainer, nNumEntries, nLastKey, BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY);
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
			::std::cerr << "TestBTreeMapSTLifInsertViaIteratorSameExtern: destination container ought to be empty!" << ::std::endl;

			exit (-1);
		}
	}
	else
	{
		if (pDestContainer->size () != pSrcContainer->size ())
		{
			::std::cerr << "TestBTreeMapSTLifInsertViaIteratorSameExtern: destination container and source container mismatch in size!" << ::std::endl;

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
			::std::cerr << "TestBTreeMapSTLifInsertViaIteratorSameExtern: destination container ought to be empty! (2)" << ::std::endl;

			exit (-1);
		}
	}
	else
	{
		if (pDestContainer->size () != pSrcContainer->size ())
		{
			::std::cerr << "TestBTreeMapSTLifInsertViaIteratorSameExtern: destination container and source container mismatch in size! (2)" << ::std::endl;

			exit (-1);
		}
	}

	pDestContainer->clear ();
	pSrcContainer->clear ();
	
}

template<class _t_map>
void TestBTreeMapSTLifInsertViaIterator (_t_map *pClM, uint32_t nNumEntries)
{
	typedef typename _t_map::value_type													value_t;
	typedef typename value_t::second_type												second_type;

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

	typedef typename ::std::map<uint32_t, second_type>::iterator						itmap_t;
	typedef typename ::std::map<uint32_t, second_type>::const_iterator					citmap_t;
	typedef typename ::std::map<uint32_t, second_type>::reverse_iterator				ritmap_t;
	typedef typename ::std::map<uint32_t, second_type>::const_reverse_iterator			critmap_t;

	typedef typename ::std::multimap<uint32_t, second_type>::iterator					itmmap_t;
	typedef typename ::std::multimap<uint32_t, second_type>::const_iterator				citmmap_t;
	typedef typename ::std::multimap<uint32_t, second_type>::reverse_iterator			ritmmap_t;
	typedef typename ::std::multimap<uint32_t, second_type>::const_reverse_iterator		critmmap_t;
	
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
	value_t										sMapPairData;
	CBTreeTestBenchMap<uint32_t, second_type>	sMap;
	itmap_t										sItMap;
	citmap_t									sCItMap;
	ritmap_t									sRItMap;
	critmap_t									sCRItMap;
	CBTreeTestBenchMultiMap<uint32_t, second_type>	sMMap;
	itmmap_t									sItMMap;
	citmmap_t									sCItMMap;
	ritmmap_t									sRItMMap;
	critmmap_t									sCRItMMap;
	
	::std::cout << "exercises method compatible to STL interface CBTreeMap[Multi]<>:: insert<_t_iterator> (_t_iterator, _t_iterator)" << ::std::endl;

	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::iter_t> (iter_t, iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sIterA, sIterB);
	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::citer_t> (citer_t, citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sCIterA, sCIterB);
	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::riter_t> (riter_t, riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sRIterA, sRIterB);
	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::criter_t> (criter_t, criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sCRIterA, sCRIterB);

	TestBTreeMapSTLifInsertViaIteratorSame ("target::insert<_t_obj::iter_t> (iter_t == iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sIterA, sIterB);
	TestBTreeMapSTLifInsertViaIteratorSame ("target::insert<_t_obj::citer_t> (citer_t == citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sCIterA, sCIterB);
	TestBTreeMapSTLifInsertViaIteratorSame ("target::insert<_t_obj::riter_t> (riter_t == riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sRIterA, sRIterB);
	TestBTreeMapSTLifInsertViaIteratorSame ("target::insert<_t_obj::criter_t> (criter_t == criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, sCRIterA, sCRIterB);

	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::iter_t> (>iter_t, iter_t<)", (int) (nNumEntries / 4), 1, nNumEntries, &sClMMTarget, pClM, sIterA, sIterB);
	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::citer_t> (>citer_t, citer_t<)", (int) (nNumEntries / 4), 1, nNumEntries, &sClMMTarget, pClM, sCIterA, sCIterB);
	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::riter_t> (>riter_t, riter_t<)", (int) (nNumEntries / 4), 1, nNumEntries, &sClMMTarget, pClM, sRIterA, sRIterB);
	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::criter_t> (>criter_t, criter_t<)", (int) (nNumEntries / 4), 1, nNumEntries, &sClMMTarget, pClM, sCRIterA, sCRIterB);

	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::iter_t> (target::iter_t, target::iter_t)", 0, 1, nNumEntries, pClM, sIterA);
	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::citer_t> (target::citer_t, target::citer_t)", 0, 1, nNumEntries, pClM, sCIterA);
	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::riter_t> (target::riter_t, target::riter_t)", 0, 1, nNumEntries, pClM, sRIterA);
	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::criter_t> (target::criter_t, target::criter_t)", 0, 1, nNumEntries, pClM, sCRIterA);

	TestBTreeMapSTLifInsertViaIteratorSame ("target::insert<_t_obj::iter_t> (target::iter_t == target::iter_t)", 0, 1, nNumEntries, pClM, pClM, sIterA, sIterB);
	TestBTreeMapSTLifInsertViaIteratorSame ("target::insert<_t_obj::citer_t> (target::citer_t == target::citer_t)", 0, 1, nNumEntries, pClM, pClM, sCIterA, sCIterB);
	TestBTreeMapSTLifInsertViaIteratorSame ("target::insert<_t_obj::riter_t> (target::riter_t == target::riter_t)", 0, 1, nNumEntries, pClM, pClM, sRIterA, sRIterB);
	TestBTreeMapSTLifInsertViaIteratorSame ("target::insert<_t_obj::criter_t> (target::criter_t == target::criter_t)", 0, 1, nNumEntries, pClM, pClM, sCRIterA, sCRIterB);

	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::iter_t> (>iter_t, iter_t<) x2", (int) (nNumEntries / 4), 2, nNumEntries, pClM, sIterA);
	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::citer_t> (>citer_t, citer_t<) x2", (int) (nNumEntries / 4), 2, nNumEntries, pClM, sCIterA);
	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::riter_t> (>riter_t, riter_t<) x2", (int) (nNumEntries / 4), 2, nNumEntries, pClM, sRIterA);
	TestBTreeMapSTLifInsertViaIteratorPart ("target::insert<_t_obj::criter_t> (>criter_t, criter_t<) x2", (int) (nNumEntries / 4), 2, nNumEntries, pClM, sCRIterA);

	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<list::iter_t> (iter_t, iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sIterA, sCIterB, sItList);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<list::citer_t> (citer_t, citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sCIterA, sCIterB, sCItList);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<list::riter_t> (riter_t, riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sRIterA, sCIterB, sItList);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<list::criter_t> (criter_t, criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sCRIterA, sCIterB, sCItList);

	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<list::iter_t> (iter_t == iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sIterA, sIterB, sItList);
	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<list::citer_t> (citer_t == citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sCIterA, sCIterB, sCItList);
	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<list::riter_t> (riter_t == riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sRIterA, sRIterB, sItList);
	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<list::criter_t> (criter_t == criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sList, sCRIterA, sCRIterB, sCItList);

	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<list::iter_t> (>iter_t, iter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sList, sIterA, sCIterB, sItList);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<list::citer_t> (>citer_t, citer_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sList, sCIterA, sCIterB, sCItList);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<list::riter_t> (>riter_t, riter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sList, sRIterA, sCIterB, sItList);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<list::criter_t> (>criter_t, criter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sList, sCRIterA, sCIterB, sCItList);

	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<vector::iter_t> (iter_t, iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sIterA, sCIterB, sItVec);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<vector::citer_t> (citer_t, citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sCIterA, sCIterB, sCItVec);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<vector::riter_t> (riter_t, riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sRIterA, sCIterB, sItVec);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<vector::criter_t> (criter_t, criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sCRIterA, sCIterB, sCItVec);

	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<vector::iter_t> (iter_t == iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sIterA, sIterB, sItVec);
	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<vector::citer_t> (citer_t == citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sCIterA, sCIterB, sCItVec);
	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<vector::riter_t> (riter_t == riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sRIterA, sRIterB, sItVec);
	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<vector::criter_t> (criter_t == criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sVector, sCRIterA, sCRIterB, sCItVec);

	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<vector::iter_t> (>iter_t, iter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sVector, sIterA, sCIterB, sItVec);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<vector::citer_t> (>citer_t, citer_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sVector, sCIterA, sCIterB, sCItVec);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<vector::riter_t> (>riter_t, riter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sVector, sRIterA, sCIterB, sItVec);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<vector::criter_t> (>criter_t, criter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sVector, sCRIterA, sCIterB, sCItVec);

	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<map::iter_t> (iter_t, iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMap, sIterA, sIterB, sItMap);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<map::citer_t> (citer_t, citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMap, sCIterA, sCIterB, sCItMap);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<map::riter_t> (riter_t, riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMap, sRIterA, sRIterB, sItMap);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<map::criter_t> (criter_t, criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMap, sCRIterA, sCRIterB, sCItMap);

	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<map::iter_t> (iter_t == iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMap, sIterA, sIterB, sItMap);
	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<map::citer_t> (citer_t == citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMap, sCIterA, sCIterB, sCItMap);
	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<map::riter_t> (riter_t == riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMap, sRIterA, sRIterB, sItMap);
	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<map::criter_t> (criter_t == criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMap, sCRIterA, sCRIterB, sCItMap);

	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<map::iter_t> (>iter_t, iter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sMap, sIterA, sIterB, sItMap);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<map::citer_t> (>citer_t, citer_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sMap, sCIterA, sCIterB, sCItMap);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<map::riter_t> (>riter_t, riter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sMap, sRIterA, sRIterB, sItMap);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<map::criter_t> (>criter_t, criter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sMap, sCRIterA, sCRIterB, sCItMap);

	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<mmap::iter_t> (iter_t, iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMMap, sIterA, sIterB, sItMMap);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<mmap::citer_t> (citer_t, citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMMap, sCIterA, sCIterB, sCItMMap);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<mmap::riter_t> (riter_t, riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMMap, sRIterA, sRIterB, sItMMap);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<mmap::criter_t> (criter_t, criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMMap, sCRIterA, sCRIterB, sCItMMap);

	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<mmap::iter_t> (iter_t == iter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMMap, sIterA, sIterB, sItMMap);
	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<mmap::citer_t> (citer_t == citer_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMMap, sCIterA, sCIterB, sCItMMap);
	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<mmap::riter_t> (riter_t == riter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMMap, sRIterA, sRIterB, sItMMap);
	TestBTreeMapSTLifInsertViaIteratorSameExtern ("target::insert<mmap::criter_t> (criter_t == criter_t)", 0, 1, nNumEntries, &sClMMTarget, pClM, &sMMap, sCRIterA, sCRIterB, sCItMMap);

	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<mmap::iter_t> (>iter_t, iter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sMMap, sIterA, sIterB, sItMMap);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<mmap::citer_t> (>citer_t, citer_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sMMap, sCIterA, sCIterB, sCItMMap);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<mmap::riter_t> (>riter_t, riter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sMMap, sRIterA, sRIterB, sItMMap);
	TestBTreeMapSTLifInsertViaIteratorPartExtern ("target::insert<mmap::criter_t> (>criter_t, criter_t<)", 0, 1, (int) (nNumEntries / 4), &sClMMTarget, pClM, &sMMap, sCRIterA, sCRIterB, sCItMMap);
}

template<class _t_container>
void TestBTreeSTLmap (uint32_t nTestNum, uint32_t nNodeSize, uint32_t nPageSize, _t_container *pMapWrapper);

#endif // !BTREEMAPTESTBENCH_H
