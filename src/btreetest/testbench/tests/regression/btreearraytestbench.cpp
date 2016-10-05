/************************************************************
**
** file:	btreearraytestbench.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for the array data class.
**
************************************************************/

#include "btreearraytestbench.h"

template<class _t_container>
void TestBTreeArrayBasic (_t_container *pContainer, typename _t_container::size_type nMaxEntries, typename _t_container::size_type nVariation, uint32_t nRepetitions, btreetest_array_primitive_seek_e eGeneratorAdd, btreetest_array_primitive_seek_e eGeneratorRemove)
{
	uint32_t		ui32;

	::std::cout << "basic test adds data at ";

	switch (eGeneratorAdd)
	{
	case BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN		:
		{
			::std::cout << "the beginning ";

			break;
		}

	case BTREETEST_ARRAY_PRIMITIVE_SEEK_END		:
		{
			::std::cout << "the end ";

			break;
		}

	case BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM	:
		{
			::std::cout << "a random position ";

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
	case BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN		:
		{
			::std::cout << "the beginning" << ::std::endl;

			break;
		}

	case BTREETEST_ARRAY_PRIMITIVE_SEEK_END		:
		{
			::std::cout << "the end" << ::std::endl;

			break;
		}

	case BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM	:
		{
			::std::cout << "a random position" << ::std::endl;

			break;
		}

	default									:
		{
			::std::cout << "?" << ::std::endl;

			break;
		}
	}

	arrayPrim_add (pContainer, nMaxEntries, eGeneratorAdd);

	for (ui32 = 0; ui32 < nRepetitions; ui32++)
	{
		::std::cout << "cycle: " << ui32 + 1 << ::std::endl;

		arrayPrim_remove (pContainer, nVariation, eGeneratorRemove);

		arrayPrim_add (pContainer, nVariation, eGeneratorAdd);
	}

	arrayPrim_remove (pContainer, nMaxEntries, eGeneratorRemove);

	::std::cout << "finished..." << ::std::endl;
}

template<class _t_container>
void TestBTreeArrayRemoveAll (_t_container *pContainer, typename _t_container::size_type nEntries)
{
	::std::cout << "test inserts data and empties the data container by calling clear" << ::std::endl;

	arrayPrim_add (pContainer, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	pContainer->clear ();
}

template<class _t_container>
void TestBTreeArrayReplace (_t_container *pContainer, uint32_t nInitEntries, uint32_t nInitReplace, uint32_t nAddEntries, uint32_t nAddReplace)
{
	::std::cout << "test replaces data" << ::std::endl;

	arrayPrim_add (pContainer, nInitEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	arrayPrim_replace (pContainer, nAddEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	arrayPrim_add (pContainer, nAddEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	arrayPrim_replace (pContainer, nAddReplace, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
}

template<class _t_container>
void TestBTreeArrayUnload (_t_container *pContainer, uint32_t nInitEntries, uint32_t nAddEntries, uint32_t nRemoveEntries, uint32_t nExitEntries)
{
	::std::cout << "test uses unload after every operation" << ::std::endl;

	::std::cout << "unloading..." << ::std::endl;

	pContainer->unload ();

	arrayPrim_add (pContainer, nInitEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	::std::cout << "unloading..." << ::std::endl;

	pContainer->unload ();

	arrayPrim_add (pContainer, nAddEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	::std::cout << "unloading..." << ::std::endl;

	pContainer->unload ();

	arrayPrim_remove (pContainer, nRemoveEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	::std::cout << "unloading..." << ::std::endl;

	pContainer->unload ();

	arrayPrim_remove (pContainer, nExitEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	::std::cout << "unloading..." << ::std::endl;

	pContainer->unload ();

	arrayPrim_add (pContainer, nInitEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	::std::cout << "unloading..." << ::std::endl;

	pContainer->unload ();

	::std::cout << "remove all" << ::std::endl;

	pContainer->clear ();

	::std::cout << "unloading..." << ::std::endl;

	pContainer->unload ();
}

template<class _t_container>
void TestBTreeArrayHTMLoutput (_t_container *pContainer, typename _t_container::size_type nEntries)
{
	::std::cout << "tests if instance is left in undefined state after HTML output" << ::std::endl;

	arrayPrim_add (pContainer, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	pContainer->show_integrity ("test_array_output_0_");

	arrayPrim_add (pContainer, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	pContainer->show_integrity ("test_array_output_1_");

	arrayPrim_add (pContainer, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
}

template<class _t_container>
void TestBTreeArrayCopyConstructorTest (_t_container *pClRef, _t_container sClCopy)
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
void TestBTreeArrayCopyConstructor (_t_container *pContainer, typename _t_container::size_type nEntries)
{
	::std::cout << "exercises array type copy constructor" << ::std::endl;

	TestBTreeArrayCopyConstructorTest (pContainer, *pContainer);

	arrayPrim_add (pContainer, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	TestBTreeArrayCopyConstructorTest (pContainer, *pContainer);

	arrayPrim_add (pContainer, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	TestBTreeArrayCopyConstructorTest (pContainer, *pContainer);

	arrayPrim_add (pContainer, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	TestBTreeArrayCopyConstructorTest (pContainer, *pContainer);

	pContainer->unload ();

	TestBTreeArrayCopyConstructorTest (pContainer, *pContainer);

	arrayPrim_remove (pContainer, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	TestBTreeArrayCopyConstructorTest (pContainer, *pContainer);

	arrayPrim_remove (pContainer, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	TestBTreeArrayCopyConstructorTest (pContainer, *pContainer);

	pContainer->clear ();

	TestBTreeArrayCopyConstructorTest (pContainer, *pContainer);
}

template<class _t_container>
void TestBTreeArrayOperatorOverloadAssign (_t_container *pContainer, typename _t_container::size_type nEntries, const uint32_t nAddCycles, const uint32_t nRemoveCycles)
{
	uint32_t			ui32;
	_t_container		*pClTarget;

	::std::cout << "exercises array type operator=" << ::std::endl;

	pClTarget = new _t_container (*pContainer);

	if (pClTarget == NULL)
	{
		::std::cerr << "ERROR: Insufficient memory!" << ::std::endl;

		exit (-1);
	}

	for (ui32 = 0; ui32 < nAddCycles; ui32++)
	{
		*pClTarget = *pContainer;

		if (*pClTarget != *pContainer)
		{
			::std::cerr << "TestBTreeArrayOperatorOverloadAssign (): ERROR: assign operator failed! (add)(" << ui32 << ")" << ::std::endl;

			exit (-1);
		}

		arrayPrim_add (pContainer, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	}

	*pClTarget = *pContainer;

	if (*pClTarget != *pContainer)
	{
		::std::cerr << "TestBTreeArrayOperatorOverloadAssign (): ERROR: assign operator failed! (add)(" << ui32 << ")" << ::std::endl;

		exit (-1);
	}

	pContainer->unload ();

	for (ui32 = 0; ui32 < nRemoveCycles; ui32++)
	{
		*pClTarget = *pContainer;

		if (*pClTarget != *pContainer)
		{
			::std::cerr << "TestBTreeArrayOperatorOverloadAssign (): ERROR: assign operator failed! (remove)(" << ui32 << ")" << ::std::endl;

			exit (-1);
		}

		arrayPrim_remove (pContainer, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	}

	*pClTarget = *pContainer;

	if (*pClTarget != *pContainer)
	{
		::std::cerr << "TestBTreeArrayOperatorOverloadAssign (): ERROR: assign operator failed! (remove)(" << ui32 << ")" << ::std::endl;

		exit (-1);
	}

	pContainer->clear ();

	*pClTarget = *pContainer;

	if (*pClTarget != *pContainer)
	{
		::std::cerr << "TestBTreeArrayOperatorOverloadAssign (): ERROR: assign operator failed! (clear)" << ::std::endl;

		exit (-1);
	}

	delete pClTarget;
}

template <class _t_container>
void TestBTreeArrayOperatorOverloadSubScript (_t_container *pContainer, typename _t_container::size_type nEntries)
{
	typedef typename _t_container::size_type		size_type;
	typedef typename _t_container::value_type		value_type;

	size_type			ui32;
	_t_container		*pClTarget;
	value_type			sData;

	::std::cout << "exercises array type operator[]" << ::std::endl;

	pClTarget = new _t_container (*pContainer);

	if (pClTarget == NULL)
	{
		::std::cerr << "ERROR: Insufficient memory!" << ::std::endl;

		exit (-1);
	}

	arrayPrim_add (pContainer, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	arrayPrim_add (pClTarget, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN);

	for (ui32 = 0; ui32 < nEntries; ui32++)
	{
		sData = (*pContainer)[ui32];

		(*pClTarget)[ui32] = sData;
	}

	if ((*pClTarget) != (*pContainer))
	{
		::std::cerr << "TestBTreeArrayOperatorOverloadSubScript: ERROR: mismatch unexpected!" << ::std::endl;

		exit (-1);
	}

	pClTarget->clear ();

	arrayPrim_add (pClTarget, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN);

	for (ui32 = 0; ui32 < nEntries; ui32++)
	{
		(*pClTarget)[ui32] = (*pContainer)[ui32];
	}

	if ((*pClTarget) != (*pContainer))
	{
		::std::cerr << "TestBTreeArrayOperatorOverloadSubScript: ERROR: mismatch unexpected! (2)" << ::std::endl;

		exit (-1);
	}

	delete pClTarget;
}

template<class _t_container>
void TestBTreeArraySerialize (_t_container *pContainer, typename _t_container::size_type nSize, typename _t_container::size_type nWindowSize)
{
	typedef typename _t_container::value_type		value_type;
	typedef typename _t_container::size_type		size_type;

	uint32_t			ui32;
	size_type			nRslt;
	value_type			*pnData = NULL;

	::std::cout << "exercises serialize method" << ::std::endl;

	pnData = new value_type [nWindowSize];

	if (pnData == NULL)
	{
		::std::cerr << "ERROR: TestBTreeArraySerialize: insufficient memory!" << ::std::endl;

		exit (-1);
	}

	arrayPrim_add (pContainer, nSize, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	for (ui32 = 0; ui32 < (nSize - nWindowSize + 1); ui32++)
	{
		nRslt = pContainer->serialize (ui32, nWindowSize, pnData);

		::std::cout << "testing from " << ui32 << " to " << ui32 + nWindowSize << ::std::endl;

		if (nRslt != nWindowSize)
		{
			::std::cerr << "ERROR: TestBTreeArraySerialize: size mismatch!" << ::std::endl;
			::std::cerr << "iteration: " << ui32 << ::std::endl;
			::std::cerr << "window size: " << nWindowSize << ::std::endl;
			::std::cerr << "returned size: " << nRslt << ::std::endl;

			exit (-1);
		}
	}

	delete [] pnData;
}

template<class _t_container>
void TestBTreeArraySerializeIncomplete (_t_container *pContainer, typename _t_container::size_type nSize, typename _t_container::size_type nWindowSize, uint32_t nOffset)
{
	typedef typename _t_container::value_type		value_type;
	typedef typename _t_container::size_type		size_type;

	size_type			nRslt;
	value_type			*pnData;

	::std::cout << "exercises serialize method with an incomplete transfer" << ::std::endl;

	pnData = new value_type [nWindowSize];

	if (pnData == NULL)
	{
		::std::cerr << "ERROR: TestBTreeArraySerializeIncomplete: insufficient memory!" << ::std::endl;

		exit (-1);
	}

	if (nSize > 0)
	{
		arrayPrim_add (pContainer, nSize, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	}

	::std::cout << "testing from " << nOffset << " to " << nWindowSize + nOffset << ::std::endl;

	nRslt = pContainer->serialize (nOffset, nWindowSize, pnData);

	if (nRslt != (nSize - nOffset))
	{
		::std::cerr << "ERROR: TestBTreeArraySerializeIncomplete: size mismatch!" << ::std::endl;
		::std::cerr << "expected size: " << nSize - nOffset << ::std::endl;
		::std::cerr << "returned size: " << nRslt << ::std::endl;

		exit (-1);
	}

	if (nSize == 0)
	{
		arrayPrim_add (pContainer, 1, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

		arrayPrim_remove (pContainer, 1, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

		nRslt = pContainer->serialize (nOffset, nWindowSize, pnData);

		if (nRslt != 0)
		{
			::std::cerr << "ERROR: TestBTreeArraySerializeIncomplete: size mismatch! (data layer initialized)" << ::std::endl;
			::std::cerr << "expected size: " << 0 << ::std::endl;
			::std::cerr << "returned size: " << nRslt << ::std::endl;

			exit (-1);
		}
	}

	delete [] pnData;
}

template<class _t_container>
void TestBTreeArrayCodeCoverageRootMergeWithLowerRight (_t_container *pContainer, uint32_t nNodeSize)
{
	typedef typename _t_container::size_type		size_type;

	size_type			nSize = nNodeSize * 2 - 1;

	::std::cout << "exercises CBTreeBaseDefaults::merge_node with root node having only one item left, while the right has the lower ID" << ::std::endl;

	nSize *= nNodeSize * 2;
	nSize *= nNodeSize * 2;

	if ((65 * (nNodeSize * 2 - 1)) > nSize)
	{
		nSize = 65 * (nNodeSize * 2 - 1);
	}

	arrayPrim_add (pContainer, nSize, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	arrayPrim_remove (pContainer, nSize - 1, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	arrayPrim_add (pContainer, nNodeSize * 2, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	arrayPrim_remove (pContainer, nNodeSize * 2, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
}

template <class _t_container>
void TestBTreeArraySelfReferenceViaSubScriptWrapper (_t_container *pContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::size_type					size_type;
	typedef typename _t_container::value_type					value_type;
	typedef typename _t_container::const_iterator				const_iterator;
	typedef typename _t_container::const_reverse_iterator		const_reverse_iterator;

	size_type				i;
	size_type				ri;
	size_type				nSize;
	value_type				sEntry;
	const_iterator			sCIter;
	
	::std::cout << "exercises CBTreeBaseDefaults<>::CBTreeArray<>::CBTreeArrayAccessWrapper<>::operator= (const CBTreeArrayAccessWrapper<> &) using its own input as reference" << ::std::endl;

	arrayPrim_add (pContainer, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

	nSize = pContainer->size ();

	get_begin (pContainer, sCIter);

	for (i = 0; i < nSize; i++, sCIter++)
	{
		sEntry = *sCIter;

		if (sEntry.nDebug != i)
		{
			::std::cerr << "ERROR: initialisation created unexpected debug sequence!" << ::std::endl;

			exit (-1);
		}
	}

	for (i = 0, ri = nSize; i < (nSize / 2); i++)
	{
		ri--;

		sEntry = (*pContainer)[i];
		(*pContainer)[i] = (*pContainer)[ri];
		(*pContainer)[ri] = sEntry;
	}

	get_begin (pContainer, sCIter);

	for (i = 0, ri = nSize; i < nSize; i++, sCIter++)
	{
		ri--;

		sEntry = *sCIter;

		if (sEntry.nDebug != ri)
		{
			::std::cerr << "ERROR: debug sequence mismatch!" << ::std::endl;

			exit (-1);
		}
	}
}

template<class _t_container>
void TestBTreeArraySTLifAssignFill (_t_container *pContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::value_type		value_type;

	value_type		sEntry;

	::std::cout << "exercises methods compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: assign(_t_sizetype, const _t_data &)" << ::std::endl;

	sEntry.nDebug = 0;
	sEntry.nData = ~0x0U;

	pContainer->assign (nNumEntries, sEntry);
}

template<class _t_container, class _t_ext_container, class _t_iterator, class _t_ext_iterator>
void TestBTreeArraySTLifAssignViaIteratorEx (btreetest_array_stl_if_assign_via_iterator_e eTest, const char *pszTestTitle, _t_container *pClTarget, _t_container *pClArray, _t_ext_container *pContainer, _t_iterator &rIterA, _t_iterator &rIterB, _t_ext_iterator &rExtIterA, _t_ext_iterator &rExtIterB, typename _t_container::size_type nNumEntries)
{
	_t_iterator			sIterBegin;
	_t_iterator			sIterEnd;
	_t_ext_iterator		sExtIterBegin;
	_t_ext_iterator		sExtIterEnd;

	::std::cout << pszTestTitle << ::std::endl;

	arrayPrim_add (pClArray, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

	get_begin (pClArray, sIterBegin);
	get_end (pClArray, sIterEnd);

	switch (eTest)
	{
		case BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL	:
		{
			pClTarget->assign (sIterBegin, sIterEnd);

			break;
		}

		case BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN:
		{
			pContainer->assign (sIterBegin, sIterEnd);

			get_begin (pContainer, sExtIterBegin);
			get_end (pContainer, sExtIterEnd);

			pClTarget->assign (sExtIterBegin, sExtIterEnd);

			break;
		}

		case BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_SELF_REFERENCE:
		{
			pClTarget->assign (sIterBegin, sIterEnd);

			get_begin (pClTarget, rIterA);
			get_end (pClTarget, rIterB);

			pClTarget->assign_via_self_reference (rIterA, rIterB);

			break;
		}

		case BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_EQUIVALENT_OR_SAME:
		{
			get_end (pClArray, rIterA);
			get_end (pClArray, rIterB);

			pClTarget->assign (rIterA, rIterB);

			if (!pClTarget->empty ())
			{
				::std::cerr << "TestBTreeArraySTLifAssignViaIteratorEx: equivalent or same test: test 1 - container was not empty!" << ::std::endl;

				exit (-1);
			}

			pClTarget->assign (rIterB, rIterA);

			if (!pClTarget->empty ())
			{
				::std::cerr << "TestBTreeArraySTLifAssignViaIteratorEx: equivalent or same test: test 2 - container was not empty!" << ::std::endl;

				exit (-1);
			}

			get_begin (pClArray, rIterA);
			get_begin (pClArray, rIterB);

			pClTarget->assign (rIterA, rIterB);

			if (!pClTarget->empty ())
			{
				::std::cerr << "TestBTreeArraySTLifAssignViaIteratorEx: equivalent or same test: test 3 - container was not empty!" << ::std::endl;

				exit (-1);
			}

			pClTarget->assign (rIterB, rIterA);

			if (!pClTarget->empty ())
			{
				::std::cerr << "TestBTreeArraySTLifAssignViaIteratorEx: equivalent or same test: test 4 - container was not empty!" << ::std::endl;

				exit (-1);
			}

			while (rIterA != sIterEnd)
			{
				pClTarget->assign (rIterA, rIterB);

				rIterA++;
				rIterB++;

				if (!pClTarget->empty ())
				{
					::std::cerr << "TestBTreeArraySTLifAssignViaIteratorEx: equivalent or same test: test 5 - container was not empty!" << ::std::endl;

					exit (-1);
				}
			}

			get_begin (pClTarget, rIterA);
			get_end (pClTarget, rIterB);

			pClTarget->assign_via_self_reference (rIterA, rIterB);

			if (!pClTarget->empty ())
			{
				::std::cerr << "TestBTreeArraySTLifAssignViaIteratorEx: equivalent or same test: test 6 - container was not empty!" << ::std::endl;

				exit (-1);
			}

			break;
		}

		case BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_PART_SELF_REFERENCE:
		{
			pClTarget->assign (sIterBegin, sIterEnd);

			get_begin (pClTarget, rIterA);
			get_end (pClTarget, rIterB);

			int		nSkew = generate_rand32 ();

			nSkew = nSkew % ((int) ((nNumEntries / 4) - 2));
			nSkew++;

			::std::advance (rIterA, (int) ((nNumEntries / 4) + nSkew));
			::std::advance (rIterB, (int) ((0 - (nNumEntries / 4)) + nSkew));

			pClTarget->assign_via_self_reference (rIterA, rIterB);

			break;
		}

		default:
		{
			::std::cerr << "TestBTreeArraySTLifAssignViaIteratorEx: unknown test selected!" << ::std::endl;

			exit (-1);

			break;
		}
	}

	pClTarget->clear ();
	pClArray->clear ();
}

template<class _t_container>
void TestBTreeArraySTLifAssignViaIterator (_t_container *pContainer, typename _t_container::size_type nNumEntries)
{
	typedef ::std::list<arrayEntry_t>						list_t;
	typedef ::std::vector<arrayEntry_t>						vector_t;

	typedef typename list_t::iterator						itlist_t;
	typedef typename list_t::const_iterator					citlist_t;
	typedef typename list_t::reverse_iterator				ritlist_t;
	typedef typename list_t::const_reverse_iterator			critlist_t;

	typedef typename vector_t::iterator						itvector_t;
	typedef typename vector_t::const_iterator				citvector_t;
	typedef typename vector_t::reverse_iterator				ritvector_t;
	typedef typename vector_t::const_reverse_iterator		critvector_t;

	typedef typename _t_container::iterator					iter_t;
	typedef typename _t_container::const_iterator			citer_t;
	typedef typename _t_container::reverse_iterator			riter_t;
	typedef typename _t_container::const_reverse_iterator	criter_t;

	_t_container					*pClTarget;
	list_t							sList;
	vector_t						sVector;
	iter_t							sIterA, sIterB;
	citer_t							sCIterA, sCIterB;
	riter_t							sRIterA, sRIterB;
	criter_t						sCRIterA, sCRIterB;
	itlist_t						sItListBegin, sItListEnd;
	citlist_t						sCItListBegin, sCItListEnd;
	ritlist_t						sRItListBegin, sRItListEnd;
	critlist_t						sCRItListBegin, sCRItListEnd;
	itvector_t						sItVecBegin, sItVecEnd;
	citvector_t						sCItVecBegin, sCItVecEnd;
	ritvector_t						sRItVecBegin, sRItVecEnd;
	critvector_t					sCRItVecBegin, sCRItVecEnd;

	::std::cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: assign<_t_iterator>(_t_iterator, _t_iterator)" << ::std::endl;

	pClTarget = new _t_container (*pContainer);

	if (pClTarget == NULL)
	{
		::std::cerr << "TestBTreeArraySTLifAssignViaIterator: ERROR: insufficient mempry!" << ::std::endl;

		exit (-1);
	}

	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL, "target::assign<_t_obj::iter_t> (iter_t, iter_t)", pClTarget, pContainer, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL, "target::assign<_t_obj::citer_t> (citer_t, citer_t)", pClTarget, pContainer, &sList, sCIterA, sCIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL, "target::assign<_t_obj::riter_t> (riter_t, riter_t)", pClTarget, pContainer, &sList, sRIterA, sRIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL, "target::assign<_t_obj::criter_t> (criter_t, criter_t)", pClTarget, pContainer, &sList, sCRIterA, sCRIterB, sItListBegin, sItListEnd, nNumEntries);

	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<list::iter> (list::iter, list::iter)", pClTarget, pContainer, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<list::citer> (list::citer, list::citer)", pClTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<list::riter> (list::riter, list::riter)", pClTarget, pContainer, &sList, sRIterA, sRIterB, sRItListBegin, sRItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<list::criter> (list::criter, list::criter)", pClTarget, pContainer, &sList, sCRIterA, sCRIterB, sCRItListBegin, sCRItListEnd, nNumEntries);

	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<vector::iter> (vector::iter, vector::iter)", pClTarget, pContainer, &sVector, sIterA, sIterB, sItVecBegin, sItVecEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<vector::citer> (vector::citer, vector::citer)", pClTarget, pContainer, &sVector, sCIterA, sCIterB, sCItVecBegin, sCItVecEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<vector::riter> (vector::riter, vector::riter)", pClTarget, pContainer, &sVector, sRIterA, sRIterB, sRItVecBegin, sRItVecEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<vector::criter> (vector::criter, vector::criter)", pClTarget, pContainer, &sVector, sCRIterA, sCRIterB, sCRItVecBegin, sCRItVecEnd, nNumEntries);

	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_SELF_REFERENCE, "target::assign<_t_obj::iter_t> (target::iter_t, target::iter_t)", pClTarget, pContainer, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_SELF_REFERENCE, "target::assign<_t_obj::citer_t> (target::citer_t, target::citer_t)", pClTarget, pContainer, &sList, sCIterA, sCIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_SELF_REFERENCE, "target::assign<_t_obj::riter_t> (target::riter_t, target::riter_t)", pClTarget, pContainer, &sList, sRIterA, sRIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_SELF_REFERENCE, "target::assign<_t_obj::criter_t> (target::criter_t, target::criter_t)", pClTarget, pContainer, &sList, sCRIterA, sCRIterB, sItListBegin, sItListEnd, nNumEntries);

	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_EQUIVALENT_OR_SAME, "target::assign<_t_obj::iter_t> (iter_t == iter_t)", pClTarget, pContainer, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_EQUIVALENT_OR_SAME, "target::assign<_t_obj::citer_t> (citer_t == citer_t)", pClTarget, pContainer, &sList, sCIterA, sCIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_EQUIVALENT_OR_SAME, "target::assign<_t_obj::riter_t> (riter_t == riter_t)", pClTarget, pContainer, &sList, sRIterA, sRIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_EQUIVALENT_OR_SAME, "target::assign<_t_obj::criter_t> (criter_t == criter_t)", pClTarget, pContainer, &sList, sCRIterA, sCRIterB, sItListBegin, sItListEnd, nNumEntries);

	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_PART_SELF_REFERENCE, "target::assign<_t_obj::iter_t> (>target::iter_t, target::iter_t<)", pClTarget, pContainer, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_PART_SELF_REFERENCE, "target::assign<_t_obj::citer_t> (>target::citer_t, target::citer_t<)", pClTarget, pContainer, &sList, sCIterA, sCIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_PART_SELF_REFERENCE, "target::assign<_t_obj::riter_t> (>target::riter_t, target::riter_t<)", pClTarget, pContainer, &sList, sRIterA, sRIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_PART_SELF_REFERENCE, "target::assign<_t_obj::criter_t> (>target::criter_t, target::criter_t<)", pClTarget, pContainer, &sList, sCRIterA, sCRIterB, sItListBegin, sItListEnd, nNumEntries);

	delete pClTarget;
}

template<class _t_container>
void TestBTreeArraySTLifPushBackPopBack (_t_container *pContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::value_type		value_type;

	uint32_t		i;
	value_type		sEntry;

	::std::cout << "exercises methods compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: push_back (const _t_data &) pop_back ()" << ::std::endl;

	::std::cout << "adding via push_back" << ::std::endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sEntry.nDebug = i;
		sEntry.nData = generate_rand32 ();

		pContainer->push_back (sEntry);
	}

	::std::cout << "removing via pop_back" << ::std::endl;

	for (i = 0; i < nNumEntries; i++)
	{
		pContainer->pop_back ();
	}
}

template<class _t_container>
void TestBTreeArraySTLifInsertMultiple (_t_container *pContainer, typename _t_container::size_type nNumEntries, typename _t_container::size_type nLen)
{
	typedef typename _t_container::iterator				iter_t;
	typedef typename _t_container::const_iterator		citer_t;
	typedef typename _t_container::value_type			value_type;

	uint32_t			i;
	value_type			sEntry;
	citer_t				sCIter;

	::std::cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: insert (const_iterator, const _t_sizetype, const _t_data &)" << ::std::endl;

	arrayPrim_add (pContainer, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	for (i = 0; i < nNumEntries; i++)
	{
		sCIter = pContainer->cbegin ();

		::std::advance (sCIter, (int) (generate_rand32 () % (uint32_t) pContainer->size ()));

		sEntry.nDebug = (uint32_t) pContainer->size ();
		sEntry.nData = generate_rand32 ();

		pContainer->insert (sCIter, nLen, sEntry);
	}
}

template<class _t_container, class _t_ext_container, class _t_iterator, class _t_ext_iterator>
void TestBTreeArraySTLifInsertViaIteratorEx (btreetest_array_stl_if_insert_via_iterator_e eTest, const char *pszTestTitel, _t_container *pClTarget, _t_container *pClArray, _t_ext_container *pContainer, _t_iterator &rIterA, _t_iterator &rIterB, _t_ext_iterator &rExtIterA, _t_ext_iterator &rExtIterB, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::const_iterator			citer_t;
	typedef typename _t_container::size_type				size_type;

	citer_t			sCIterHalf;
	_t_iterator		sIterBegin;
	_t_iterator		sIterEnd;
	_t_ext_iterator	sExtIterBegin;
	_t_ext_iterator	sExtIterEnd;
	size_type		nSize;

	::std::cout << pszTestTitel << ::std::endl;

	arrayPrim_add (pClArray, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

	get_begin (pClArray, sIterBegin);
	get_end (pClArray, sIterEnd);

	switch (eTest)
	{
		case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY	:
		{
			pClTarget->assign (sIterBegin, sIterEnd);

			pClArray->clear ();

			arrayPrim_add (pClArray, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

			sCIterHalf = pClTarget->cbegin ();

			::std::advance (sCIterHalf, pClTarget->size () / 2);

			get_begin (pClArray, sIterBegin);
			get_end (pClArray, sIterEnd);

			pClTarget->insert (sCIterHalf, sIterBegin, sIterEnd);

			break;
		}

		case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN:
		{
			pClTarget->assign (sIterBegin, sIterEnd);

			pClArray->clear ();

			arrayPrim_add (pClArray, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

			get_begin (pClArray, sIterBegin);
			get_end (pClArray, sIterEnd);

			pContainer->assign (sIterBegin, sIterEnd);

			sCIterHalf = pClTarget->cbegin ();

			::std::advance (sCIterHalf, pClTarget->size () / 2);

			get_begin (pContainer, sExtIterBegin);
			get_end (pContainer, sExtIterEnd);

			pClTarget->insert (sCIterHalf, sExtIterBegin, sExtIterEnd);

			break;
		}

		case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE:
		{
			pClTarget->assign (sIterBegin, sIterEnd);

			sCIterHalf = pClTarget->cbegin ();

			::std::advance (sCIterHalf, pClTarget->size () / 2);

			get_begin (pClTarget, sIterBegin);
			get_end (pClTarget, sIterEnd);

			pClTarget->insert_via_self_reference (sCIterHalf, sIterBegin, sIterEnd);

			break;
		}

		case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_INEFFECTIVE:
		{
			pClTarget->assign (sIterBegin, sIterEnd);

			pClArray->clear ();

			arrayPrim_add (pClArray, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

			get_end (pClArray, rIterA);
			get_end (pClArray, rIterB);

			get_begin (pClArray, sIterBegin);
			get_end (pClArray, sIterEnd);

			citer_t			sCIter;

			nSize = pClTarget->size ();

			for (sCIter = pClTarget->cbegin (); sCIter != pClTarget->cend (); sCIter++)
			{
				pClTarget->insert (sCIter, rIterA, rIterB);
			}

			if (pClTarget->size () != nSize)
			{
				::std::cerr << "TestBTreeArraySTLifInsertViaIterator: ineffective test: test 1 - container size changed!" << ::std::endl;

				exit (-1);
			}

			for (sCIter = pClTarget->cbegin (); sCIter != pClTarget->cend (); sCIter++)
			{
				pClTarget->insert (sCIter, rIterB, rIterA);
			}

			if (pClTarget->size () != nSize)
			{
				::std::cerr << "TestBTreeArraySTLifInsertViaIterator: ineffective test: test 2 - container size changed!" << ::std::endl;

				exit (-1);
			}

			get_begin (pClArray, rIterA);
			get_begin (pClArray, rIterB);

			for (sCIter = pClTarget->cbegin (); sCIter != pClTarget->cend (); sCIter++)
			{
				pClTarget->insert (sCIter, rIterA, rIterB);
			}

			if (pClTarget->size () != nSize)
			{
				::std::cerr << "TestBTreeArraySTLifInsertViaIterator: ineffective test: test 3 - container size changed!" << ::std::endl;

				exit (-1);
			}

			for (sCIter = pClTarget->cbegin (); sCIter != pClTarget->cend (); sCIter++)
			{
				pClTarget->insert (sCIter, rIterB, rIterA);
			}

			if (pClTarget->size () != nSize)
			{
				::std::cerr << "TestBTreeArraySTLifInsertViaIterator: ineffective test: test 4 - container size changed!" << ::std::endl;

				exit (-1);
			}

			while (rIterA != sIterEnd)
			{
				for (sCIter = pClTarget->cbegin (); sCIter != pClTarget->cend (); sCIter++)
				{
					pClTarget->insert (sCIter, rIterA, rIterB);
				}

				rIterA++;
				rIterB++;

				if (pClTarget->size () != nSize)
				{
					::std::cerr << "TestBTreeArraySTLifInsertViaIterator: ineffective test: test 5 - container size changed!" << ::std::endl;

					exit (-1);
				}
			}

			break;
		}

		case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_PART_SELF_REFERENCE:
		{
			pClTarget->assign (sIterBegin, sIterEnd);

			sCIterHalf = pClTarget->cbegin ();

			::std::advance (sCIterHalf, pClTarget->size () / 2);

			get_begin (pClTarget, sIterBegin);
			get_end (pClTarget, sIterEnd);

			int		nSkew = generate_rand32 ();

			nSkew = nSkew % ((int) ((nNumEntries / 4) - 2));
			nSkew++;

			::std::advance (sIterBegin, (int) ((nNumEntries / 4) + nSkew));
			::std::advance (sIterEnd, (int) ((0 - (nNumEntries / 4)) + nSkew));

			pClTarget->insert_via_self_reference (sCIterHalf, sIterBegin, sIterEnd);

			break;
		}

		default:
		{
			::std::cerr << "TestBTreeArraySTLifInsertViaIterator: unknown test!" << ::std::endl;

			exit (-1);

			break;
		}
	}

	pClTarget->clear ();
	pClArray->clear ();
}

template<class _t_container>
void TestBTreeArraySTLifInsertViaIterator (_t_container *pContainer, typename _t_container::size_type nNumEntries)
{
	typedef ::std::list<arrayEntry_t>						list_t;
	typedef ::std::vector<arrayEntry_t>						vector_t;

	typedef typename list_t::iterator						itlist_t;
	typedef typename list_t::const_iterator					citlist_t;
	typedef typename list_t::reverse_iterator				ritlist_t;
	typedef typename list_t::const_reverse_iterator			critlist_t;

	typedef typename vector_t::iterator						itvector_t;
	typedef typename vector_t::const_iterator				citvector_t;
	typedef typename vector_t::reverse_iterator				ritvector_t;
	typedef typename vector_t::const_reverse_iterator		critvector_t;

	typedef typename _t_container::iterator					iter_t;
	typedef typename _t_container::const_iterator			citer_t;
	typedef typename _t_container::reverse_iterator			riter_t;
	typedef typename _t_container::const_reverse_iterator	criter_t;

	_t_container					*pClTarget;
	::std::list<arrayEntry_t>		sList;
	::std::vector<arrayEntry_t>		sVector;
	iter_t							sIterA, sIterB;
	citer_t							sCIterA, sCIterB;
	riter_t							sRIterA, sRIterB;
	criter_t						sCRIterA, sCRIterB;
	citer_t							sCIterHalf;
	itlist_t						sItListBegin, sItListEnd;
	citlist_t						sCItListBegin, sCItListEnd;
	ritlist_t						sRItListBegin, sRItListEnd;
	critlist_t						sCRItListBegin, sCRItListEnd;
	itvector_t						sItVecBegin, sItVecEnd;
	citvector_t						sCItVecBegin, sCItVecEnd;
	ritvector_t						sRItVecBegin, sRItVecEnd;
	critvector_t					sCRItVecBegin, sCRItVecEnd;

	::std::cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: insert<_t_iterator>(const_iterator, _t_iterator, _t_iterator)" << ::std::endl;

	pClTarget = new _t_container (*pContainer);

	if (pClTarget == NULL)
	{
		::std::cerr << "TestBTreeArraySTLifAssignViaIterator: ERROR: insufficient memory!" << ::std::endl;

		exit (-1);
	}

	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY, "target::insert<_t_obj::iter_t> (iter_t, iter_t)", pClTarget, pContainer, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY, "target::insert<_t_obj::citer_t> (citer_t, citer_t)", pClTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY, "target::insert<_t_obj::riter_t> (riter_t, riter_t)", pClTarget, pContainer, &sList, sRIterA, sRIterB, sRItListBegin, sRItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY, "target::insert<_t_obj::criter_t> (criter_t, criter_t)", pClTarget, pContainer, &sList, sCRIterA, sCRIterB, sCRItListBegin, sCRItListEnd, nNumEntries);

	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<list::iter_t> (list::iter_t, list::iter_t)", pClTarget, pContainer, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<list::citer_t> (list::citer_t, list::citer_t)", pClTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<list::riter_t> (list::riter_t, list::riter_t)", pClTarget, pContainer, &sList, sRIterA, sRIterB, sRItListBegin, sRItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<list::criter_t> (list::criter_t, list::criter_t)", pClTarget, pContainer, &sList, sCRIterA, sCRIterB, sCRItListBegin, sCRItListEnd, nNumEntries);

	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<vector::iter_t> (vector::iter_t, vector::iter_t)", pClTarget, pContainer, &sVector, sIterA, sIterB, sItVecBegin, sItVecEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<vector::citer_t> (vector::citer_t, vector::citer_t)", pClTarget, pContainer, &sVector, sCIterA, sCIterB, sCItVecBegin, sCItVecEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<vector::riter_t> (vector::riter_t, vector::riter_t)", pClTarget, pContainer, &sVector, sRIterA, sRIterB, sRItVecBegin, sRItVecEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<vector::criter_t> (vector::criter_t, vector::criter_t)", pClTarget, pContainer, &sVector, sCRIterA, sCRIterB, sCRItVecBegin, sCRItVecEnd, nNumEntries);

	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::iter_t> (target::iter_t, target::iter_t)", pClTarget, pContainer, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::citer_t> (target::citer_t, target::citer_t)", pClTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::riter_t> (target::riter_t, target::riter_t)", pClTarget, pContainer, &sList, sRIterA, sRIterB, sRItListBegin, sRItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::criter_t> (target::criter_t, target::criter_t)", pClTarget, pContainer, &sList, sCRIterA, sCRIterB, sCRItListBegin, sCRItListEnd, nNumEntries);

	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_INEFFECTIVE, "target::insert<_t_obj::iter_t> (iter_t == iter_t)", pClTarget, pContainer, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_INEFFECTIVE, "target::insert<_t_obj::citer_t> (citer_t == citer_t)", pClTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_INEFFECTIVE, "target::insert<_t_obj::riter_t> (riter_t == riter_t)", pClTarget, pContainer, &sList, sRIterA, sRIterB, sRItListBegin, sRItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_INEFFECTIVE, "target::insert<_t_obj::criter_t> (criter_t == criter_t)", pClTarget, pContainer, &sList, sCRIterA, sCRIterB, sCRItListBegin, sCRItListEnd, nNumEntries);

	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_PART_SELF_REFERENCE, "target::insert<_t_obj::iter_t> (>target::iter_t, target::iter_t<)", pClTarget, pContainer, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_PART_SELF_REFERENCE, "target::insert<_t_obj::citer_t> (>target::citer_t, target::citer_t<)", pClTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_PART_SELF_REFERENCE, "target::insert<_t_obj::riter_t> (>target::riter_t, target::riter_t<)", pClTarget, pContainer, &sList, sRIterA, sRIterB, sRItListBegin, sRItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_PART_SELF_REFERENCE, "target::insert<_t_obj::criter_t> (>target::criter_t, target::criter_t<)", pClTarget, pContainer, &sList, sCRIterA, sCRIterB, sCRItListBegin, sCRItListEnd, nNumEntries);

	delete pClTarget;
}

template<class _t_container, class _t_iterator>
void TestBTreeArraySTLifInsertViaIteratorLarge (_t_container *pContainer, _t_iterator &rIterType, typename _t_container::size_type nNumEntries, uint32_t nInsertSize)
{
	typedef typename _t_container::const_iterator					citer_t;
	typedef typename _t_container::value_type						value_type;
	typedef typename ::std::vector<value_type>::const_iterator		citer_vec_t;

	uint32_t						i;
	uint32_t						j;
	::std::vector<value_type>		sVector;
	_t_iterator						sIterA, sIterB;
	citer_t							sCIter;
	value_type						sEntry;
	citer_vec_t						sCIterVecBegin;
	citer_vec_t						sCIterVecEnd;

	::std::cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: insert<_t_iterator>(const_iterator, _t_iterator, _t_iterator)" << ::std::endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sEntry.nDebug = g_nDebug;
		sEntry.nData = generate_rand32 ();

		g_nDebug++;

		sVector.push_back (sEntry);
	}

	if (is_const_iterator (pContainer, rIterType))
	{
		if (is_reverse_iterator (pContainer, rIterType))
		{
			::std::cout << "target::assign<_t_obj::criter_t> (>target::criter_t, target::criter_t<)" << ::std::endl;
		}
		else
		{
			::std::cout << "target::assign<_t_obj::citer_t> (>target::citer_t, target::citer_t<)" << ::std::endl;
		}
	}
	else
	{
		if (is_reverse_iterator (pContainer, rIterType))
		{
			::std::cout << "target::assign<_t_obj::riter_t> (>target::riter_t, target::riter_t<)" << ::std::endl;
		}
		else
		{
			::std::cout << "target::assign<_t_obj::iter_t> (>target::iter_t, target::iter_t<)" << ::std::endl;
		}
	}

	get_begin (&sVector, sCIterVecBegin);
	get_end (&sVector, sCIterVecEnd);

	pContainer->assign (sCIterVecBegin, sCIterVecEnd);

	sIterB = sIterA = pContainer->begin ();
	::std::advance (sIterB, (int) nInsertSize);

	for (i = 0; i < 3; i++)
	{
		sCIter = pContainer->cbegin ();

		for (j = 0; j < 4; j++)
		{
			::std::advance (sCIter, pContainer->size () / 5);

			pContainer->insert_via_self_reference (sCIter, sIterA, sIterB);
		}

		::std::advance (sIterA, (pContainer->size () - nInsertSize) / 4);
		::std::advance (sIterB, (pContainer->size () - nInsertSize) / 4);
	}
}

template<class _t_container>
void TestBTreeArraySTLifEraseSingle (_t_container *pContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::iterator				iter_t;
	typedef typename _t_container::const_iterator		citer_t;

	uint32_t			i;
	citer_t				sCIter;

	::std::cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: erase (const_iterator)" << ::std::endl;

	arrayPrim_add (pContainer, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	for (i = 0; i < nNumEntries; i++)
	{
		::std::cout << pContainer->size () << " <-- ";

		sCIter = pContainer->cbegin ();

		::std::advance (sCIter, (int) (generate_rand32 () % (nNumEntries - i)));

		pContainer->erase (sCIter);

		::std::cout << pContainer->size () << ::std::endl;
	}
}

template<class _t_container>
void TestBTreeArraySTLifEraseMultiple (_t_container *pContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::iterator							iter_t;
	typedef typename _t_container::const_iterator					citer_t;
	typedef typename _t_container::value_type						value_type;
	typedef typename _t_container::size_type						size_type;
	typedef typename ::std::list<value_type>::const_iterator		citer_list_t;

	::std::list<value_type>			sList;
	size_type						i;
	size_type						j;
	size_type						k;
	citer_t							sCIterA;
	citer_t							sCIterB;
	value_type						sEntry;
	citer_list_t					sCIterListBegin;
	citer_list_t					sCIterListEnd;

	::std::cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: erase (const_iterator, const_iterator)" << ::std::endl;

	for (i = 0; i < nNumEntries; i += 5)
	{
		for (j = i; j < nNumEntries; j += (nNumEntries / 7))
		{
			::std::cout << i << " / " << nNumEntries << "  " << (j - i) << " / " << (nNumEntries - i) << "  \r";

			sList.clear ();

			for (k = 0; k < nNumEntries; k++)
			{
				sEntry.nData = generate_rand32 ();
				sEntry.nDebug = g_nDebug;

				sList.push_back (sEntry);

				g_nDebug++;
			}

			get_begin (&sList, sCIterListBegin);
			get_end (&sList, sCIterListEnd);

			pContainer->assign (sCIterListBegin, sCIterListEnd);

			sCIterB = sCIterA = pContainer->cbegin ();
			::std::advance (sCIterB, (int) i);

			::std::advance (sCIterA, (int) (j - i));
			::std::advance (sCIterB, (int) (j - i));

			pContainer->erase (sCIterA, sCIterB);
		}
	}
}

template<class _t_container>
void TestBTreeArraySTLifSwap (_t_container *pContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::iterator							iter_t;
	typedef typename _t_container::const_iterator					citer_t;
	typedef typename _t_container::value_type						value_type;
	typedef typename ::std::list<value_type>::const_iterator		citer_list_t;

	::std::list<value_type>			sList;
	uint32_t						i;
	value_type						sEntry;
	_t_container					sClArrayTarget (*pContainer);
	_t_container					sClArrayRef (*pContainer);
	_t_container					sClArrayTargetRef (*pContainer);
	citer_list_t					sCIterListBegin;
	citer_list_t					sCIterListEnd;

	::std::cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: swap ()" << ::std::endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sEntry.nData = generate_rand32 ();
		sEntry.nDebug = g_nDebug;

		sList.push_back (sEntry);

		g_nDebug++;
	}

	get_begin (&sList, sCIterListBegin);
	get_end (&sList, sCIterListEnd);

	pContainer->assign (sCIterListBegin, sCIterListEnd);
	sClArrayRef.assign (sCIterListBegin, sCIterListEnd);

	if ((*pContainer) != sClArrayRef)
	{
		::std::cerr << "ERROR: Unexpected array mismatch!" << ::std::endl;

		exit (-1);
	}

	sList.clear ();

	for (i = 0; i < (nNumEntries / 2); i++)
	{
		sEntry.nData = generate_rand32 ();
		sEntry.nDebug = g_nDebug;

		sList.push_back (sEntry);

		g_nDebug++;
	}

	get_begin (&sList, sCIterListBegin);
	get_end (&sList, sCIterListEnd);

	sClArrayTarget.assign (sCIterListBegin, sCIterListEnd);
	sClArrayTargetRef.assign (sCIterListBegin, sCIterListEnd);

	if (sClArrayTarget != sClArrayTargetRef)
	{
		::std::cerr << "ERROR: Unexpected target mismatch!" << ::std::endl;

		exit (-1);
	}

	pContainer->swap (sClArrayTarget);

	if ((*pContainer) != sClArrayTargetRef)
	{
		::std::cerr << "ERROR: array mismatches target reference!" << ::std::endl;

		exit (-1);
	}

	if (sClArrayTarget != sClArrayRef)
	{
		::std::cerr << "ERROR: target mismatches array reference!" << ::std::endl;

		exit (-1);
	}

	sClArrayTarget.swap (*pContainer);

	if ((*pContainer) != sClArrayRef)
	{
		::std::cerr << "ERROR: array mismatches array reference!" << ::std::endl;

		exit (-1);
	}

	if (sClArrayTarget != sClArrayTargetRef)
	{
		::std::cerr << "ERROR: target mismatches target reference!" << ::std::endl;

		exit (-1);
	}
}

template<class _t_container>
void TestBTreeArray (uint32_t nTest, uint32_t nNodeSize, uint32_t nPageSize, _t_container *pArrayWrapper)
{
	typename _t_container::size_test_type		sTypeSelect;
	::std::string								sTypeStr;

	get_typename (sTypeSelect, sTypeStr);

	::std::cout << "b-tree array test bench selected using type " << sTypeStr << ::std::endl;

	pArrayWrapper = new _t_container (nNodeSize, nPageSize);

	if (pArrayWrapper == NULL)
	{
		::std::cerr << "insufficient memory!" << ::std::endl;

		exit (-1);
	}

	switch (nTest)
	{
	case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_ITER		:
	case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_CITER		:
	case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_RITER		:
	case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_CRITER	:
		{
			pArrayWrapper->skip_containers_with_limited_address_space (true);

			break;
		}
	}
											
	switch (nTest)
	{
	case BTREETEST_ARRAY_BASIC_FIFO			:
		{
			TestBTreeArrayBasic (pArrayWrapper, 512, 256, 3, BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

			break;
		}

	case BTREETEST_ARRAY_BASIC_FIFO_SMALL	:
		{
			TestBTreeArrayBasic (pArrayWrapper, 16, 0, 0, BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

			break;
		}

	case BTREETEST_ARRAY_BASIC_LILO			:
		{
			TestBTreeArrayBasic (pArrayWrapper, 512, 256, 3, BTREETEST_ARRAY_PRIMITIVE_SEEK_END, BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN);

			break;
		}

	case BTREETEST_ARRAY_BASIC_LILO_SMALL	:
		{
			TestBTreeArrayBasic (pArrayWrapper, 16, 0, 0, BTREETEST_ARRAY_PRIMITIVE_SEEK_END, BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN);

			break;
		}

	case BTREETEST_ARRAY_BASIC_RANDOM		:
		{
			TestBTreeArrayBasic (pArrayWrapper, 512, 256, 3, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

			break;
		}

	case BTREETEST_ARRAY_BASIC_RANDOM_SMALL	:
		{
			TestBTreeArrayBasic (pArrayWrapper, 16, 0, 0, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

			break;
		}

	case BTREETEST_ARRAY_CLEAR			:
		{
			TestBTreeArrayRemoveAll (pArrayWrapper, 256);

			break;
		}

	case BTREETEST_ARRAY_REPLACE			:
		{
			TestBTreeArrayReplace (pArrayWrapper, 16, 8192, 512, 16);

			break;
		}

	case BTREETEST_ARRAY_UNLOAD				:
		{
			TestBTreeArrayUnload (pArrayWrapper, 128, 128, 128, 128);

			break;
		}

	case BTREETEST_ARRAY_HTML_OUTPUT				:
		{
			TestBTreeArrayHTMLoutput (pArrayWrapper, 64);

			break;
		}

	case BTREETEST_ARRAY_COPY_CONSTRUCTOR			:
		{
			TestBTreeArrayCopyConstructor (pArrayWrapper, 64);

			break;
		}

	case BTREETEST_ARRAY_OPERATOR_OVERLOAD_ASSIGN	:
		{
			TestBTreeArrayOperatorOverloadAssign (pArrayWrapper, 64, 3, 3);

			break;
		}

	case BTREETEST_ARRAY_OPERATOR_OVERLOAD_SUBSCRIPT	:
		{
			TestBTreeArrayOperatorOverloadSubScript (pArrayWrapper, 64);

			break;
		}

	case BTREETEST_ARRAY_SERLIALIZE_ALL					:
		{
			TestBTreeArraySerialize (pArrayWrapper, 128, 128);

			break;
		}

	case BTREETEST_ARRAY_SERLIALIZE_HALF_NODE			:
		{
			TestBTreeArraySerialize (pArrayWrapper, 128, 64);

			break;
		}

	case BTREETEST_ARRAY_SERLIALIZE_PART_NODE			:
		{
			TestBTreeArraySerialize (pArrayWrapper, 128, nNodeSize - 1);

			break;
		}

	case BTREETEST_ARRAY_SERLIALIZE_INCOMPLETE			:
		{
			TestBTreeArraySerializeIncomplete (pArrayWrapper, 128, 128, 64);

			break;
		}

	case BTREETEST_ARRAY_CODE_COVERAGE_ROOT_MERGE_WITH_LOWER_RIGHT	:
		{
			TestBTreeArrayCodeCoverageRootMergeWithLowerRight (pArrayWrapper, nNodeSize);

			break;
		}

	case BTREETEST_ARRAY_SELF_REFERENCE_VIA_SUBSCRIPT_WRAPPER	:
		{
			TestBTreeArraySelfReferenceViaSubScriptWrapper (pArrayWrapper, 10);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_ASSIGN_FILL:
		{
			TestBTreeArraySTLifAssignFill (pArrayWrapper, 512);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR	:
		{
			TestBTreeArraySTLifAssignViaIterator (pArrayWrapper, 64);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_PUSH_BACK_POP_BACK	:
		{
			TestBTreeArraySTLifPushBackPopBack (pArrayWrapper, 128);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_INSERT_MULTIPLE:
		{
			TestBTreeArraySTLifInsertMultiple (pArrayWrapper, 64, 4);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR:
		{
			TestBTreeArraySTLifInsertViaIterator (pArrayWrapper, 64);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_ITER	:
		{
			typename _t_container::iterator		sIter;

			TestBTreeArraySTLifInsertViaIteratorLarge (pArrayWrapper, sIter, 131072, 16384);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_CITER	:
		{
			typename _t_container::iterator		sCIter;

			TestBTreeArraySTLifInsertViaIteratorLarge (pArrayWrapper, sCIter, 131072, 16384);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_RITER	:
		{
			typename _t_container::iterator		sRIter;

			TestBTreeArraySTLifInsertViaIteratorLarge (pArrayWrapper, sRIter, 131072, 16384);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_CRITER	:
		{
			typename _t_container::iterator		sCRIter;

			TestBTreeArraySTLifInsertViaIteratorLarge (pArrayWrapper, sCRIter, 131072, 16384);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_ERASE_SINGLE	:
		{
			TestBTreeArraySTLifEraseSingle (pArrayWrapper, 128);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_ERASE_MULTIPLE	:
		{
			TestBTreeArraySTLifEraseMultiple (pArrayWrapper, 128);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_SWAP	:
		{
			TestBTreeArraySTLifSwap (pArrayWrapper, 128);

			break;
		}

	case BTREETEST_ARRAY_SERLIALIZE_ZERO_LENGTH			:
		{
			TestBTreeArraySerializeIncomplete (pArrayWrapper, 0, 1, 0);

			break;
		}

	default									:
		{
			::std::cerr << "ERROR: TestBTreeArray: Unknown test or test not specified!" << ::std::endl;

			exit (-1);

			break;
		}
	}

	delete pArrayWrapper;
}
