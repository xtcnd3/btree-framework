/************************************************************
**
** file:	btreearrayitertestbench.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for the array iterator data class.
**
************************************************************/

#include "btreearrayitertestbench.h"

template<class _t_iterator, class _t_container, class _t_reference>
void TestBTreeArrayIterBasic (_t_container *pContainer, _t_reference *pReference, bool bDescend, typename _t_container::size_type nNumEntries, typename _t_container::size_type nStepSize, uint32_t nTurnArounds, bool bFillArray = true)
{
	typedef typename _t_container::value_type		data_t;
	typedef typename _t_container::size_type		size_type;
	typedef typename _t_reference::const_iterator	citer_ref_t;

	_t_iterator				sIter;
	_t_iterator				sIterBegin;
	_t_iterator				sIterEnd;
	citer_ref_t				sCIterRef;
	data_t					sEntryViaRef;
	data_t					sEntryViaIter;
	size_type				i;
	uint32_t				nTurn;
	size_type				nThisStepSize;

	::std::cout << "basic read-only iterator test ";

	if (bDescend == false)
	{
		::std::cout << "ascends ";
	}
	else
	{
		::std::cout << "descends ";
	}

	::std::cout << nNumEntries << " entries ";
	::std::cout << "with step size " << nStepSize;
	::std::cout << " and turns around " << nTurnArounds << " times." << ::std::endl;

	if (bFillArray)
	{
		arrayPrim_add (pContainer, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	}

	get_begin (pContainer, sIterBegin);
	get_end (pContainer, sIterEnd);

	pReference->assign (sIterBegin, sIterEnd);

	for (nTurn = 0; nTurn <= nTurnArounds; nTurn++)
	{
		::std::cout << "turn: " << nTurn << ::std::endl;

		bool	bForward = (((nTurn & 0x1) == 0x0) != bDescend);

		i = bForward ? 0ULL : pContainer->size ();

		if (bForward)
		{
			get_begin (pContainer, sIter);
			get_begin (pReference, sCIterRef);
		}
		else
		{
			get_end (pContainer, sIter);
			get_end (pReference, sCIterRef);
		}

		while (bForward ? i < pContainer->size () : i > 0)
		{
			if (!bForward)
			{
				if (i >= nStepSize)
				{
					nThisStepSize = nStepSize;
				}
				else
				{
					nThisStepSize = (uint32_t) i;
				}

				i -= nThisStepSize;

				if (nStepSize == 1)
				{
					if ((i & 0x1) == 0x0)
					{
						sIter--;
						sCIterRef--;
					}
					else
					{
						--sIter;
						--sCIterRef;
					}
				}
				else
				{
					sIter -= (size_type) nThisStepSize;
					::std::advance (sCIterRef, (typename ::std::make_signed<size_type>::type) (0 - nThisStepSize));
				}
			}

			::std::cout << i << " " << "\r" << ::std::flush;

			sEntryViaRef = *sCIterRef;

			sEntryViaIter = *sIter;

			if (sEntryViaRef != sEntryViaIter)
			{
				::std::cerr << ::std::endl;
				::std::cerr << "iterator mismatch!" << ::std::endl;
				::std::cerr << "turn around: " << nTurn << ::std::endl;
				::std::cerr << "position: " << i << ::std::endl;
				::std::cerr << "data via interface" << ::std::endl;
				::std::cerr << "nData: " << sEntryViaRef.nData << ::std::endl;
				::std::cerr << "nDebug: " << sEntryViaRef.nDebug << ::std::endl;
				::std::cerr << "data via iterator" << ::std::endl;
				::std::cerr << "nData: " << sEntryViaIter.nData << ::std::endl;
				::std::cerr << "nDebug: " << sEntryViaIter.nDebug << ::std::endl;

				exit (-1);
			}

			if (bForward)
			{
				if ((i + nStepSize) >= pContainer->size ())
				{
					nThisStepSize = pContainer->size () - i;
				}
				else
				{
					nThisStepSize = nStepSize;
				}

				i += nThisStepSize;

				if (nThisStepSize == 1)
				{
					if ((i & 0x1) == 0x0)
					{
						sIter++;
						sCIterRef++;
					}
					else
					{
						++sIter;
						++sCIterRef;
					}
				}
				else
				{
					sIter += (size_type) nThisStepSize;
					::std::advance (sCIterRef, nThisStepSize);
				}
			}
		}

		::std::cout << ::std::endl;
	}
}

template<class _t_container, class _t_reference>
void TestBTreeArrayIterNodeSizeVsStepSize (_t_container *pContainer, _t_reference *pReference, bool bDescend, typename _t_container::size_type nNumEntries, typename _t_container::size_type nFromNodeSize, typename _t_container::size_type nToNodeSize, typename _t_container::size_type nFromStepSize, typename _t_container::size_type nToStepSize)
{
	typename _t_container::size_type		nStepSize;
	uint32_t								i;
	arrayEntry_t							sEntry;

	for (i = 0; i < nNumEntries; i++)
	{
		sEntry.nDebug = g_nDebug;
		sEntry.nData = generate_rand32 ();

		pContainer->push_back (sEntry);

		g_nDebug++;
	}

	for (nStepSize = nFromStepSize; nStepSize < nToStepSize; nStepSize <<= 1)
	{
		TestBTreeArrayIterBasic<typename _t_container::iterator> (pContainer, pReference, bDescend, nNumEntries, nStepSize, 0, false);
		TestBTreeArrayIterBasic<typename _t_container::const_iterator> (pContainer, pReference, bDescend, nNumEntries, nStepSize, 0, false);
		TestBTreeArrayIterBasic<typename _t_container::reverse_iterator> (pContainer, pReference, bDescend, nNumEntries, nStepSize, 0, false);
		TestBTreeArrayIterBasic<typename _t_container::const_reverse_iterator> (pContainer, pReference, bDescend, nNumEntries, nStepSize, 0, false);
	}

	pContainer->clear ();
}

template<class _t_iterator, class _t_container, class _t_reference>
void TestBTreeArrayIterBiDirectionalDereference (_t_container *pContainer, _t_reference *pReference, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::value_type				value_type;
	typedef typename _t_container::size_type				size_type;
	typedef typename _t_reference::iterator					iter_ref_t;

	_t_iterator				sIter;
	_t_iterator				sIterBegin;
	_t_iterator				sIterEnd;
	value_type				sEntry;
	size_type				i;
	iter_ref_t				sIterRef;

	::std::cout << "basic array iterator bi-directional dereference test" << ::std::endl;

	arrayPrim_add (pContainer, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	get_begin (pContainer, sIter);

	get_begin (pContainer, sIterBegin);
	get_end (pContainer, sIterEnd);

	pReference->assign (sIterBegin, sIterEnd);

	get_begin (pReference, sIterRef);

	for (i = 0; i < pContainer->size (); i++)
	{
		sEntry = *sIter;
		sEntry.nData = ~sEntry.nData;
		*sIter = sEntry;

		sEntry = *sIterRef;
		sEntry.nData = ~sEntry.nData;
		*sIterRef = sEntry;

		sIter++;
		sIterRef++;
	}

	get_begin (pContainer, sIter);

	get_begin (pReference, sIterRef);

	for (i = 0; i < pContainer->size (); i++)
	{
		if (((value_type) *sIter) != ((value_type) *sIterRef))
		{
			::std::cerr << ::std::endl;
			::std::cerr << "array mismatch!" << ::std::endl;

			exit (-1);
		}

		sIter++;
		sIterRef++;
	}

	pContainer->clear ();
	pReference->clear ();
}

template<class _t_subscripttype, class _t_iterator, class _t_container, class _t_reference>
void TestBTreeArrayIterBiDirectionalSubScriptor (_t_container *pContainer, _t_reference *pReference, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::value_type				value_type;
	typedef typename _t_reference::iterator					iter_ref_t;

	_t_iterator				sIter;
	_t_iterator				sIterBegin;
	_t_iterator				sIterEnd;
	value_type				sEntry;
	_t_subscripttype		i;
	iter_ref_t				sIterRef;
	::std::string			sTypeStr;

	::std::cout << "basic array iterator bi-directional sub-scription test" << ::std::endl;

	get_typename (i, sTypeStr);

	::std::cout << "_t_subscripttype is set to: " << sTypeStr << ::std::endl;

	arrayPrim_add (pContainer, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	get_begin (pContainer, sIter);

	get_begin (pContainer, sIterBegin);
	get_end (pContainer, sIterEnd);

	pReference->assign (sIterBegin, sIterEnd);

	get_begin (pReference, sIterRef);

	for (i = 0; i < (_t_subscripttype) pContainer->size (); i++)
	{
		sEntry = sIter[i];
		sEntry.nData = ~sEntry.nData;
		sIter[i] = sEntry;

		sEntry = *sIterRef;
		sEntry.nData = ~sEntry.nData;
		*sIterRef = sEntry;

		sIterRef++;
	}

	get_begin (pContainer, sIter);

	get_begin (pReference, sIterRef);

	for (i = 0; i < (_t_subscripttype) pContainer->size (); i++)
	{
		if (((value_type) *sIter) != ((value_type) *sIterRef))
		{
			::std::cerr << ::std::endl;
			::std::cerr << "array mismatch!" << ::std::endl;

			exit (-1);
		}

		sIter++;
		sIterRef++;
	}

	pContainer->clear ();
	pReference->clear ();
}

template<class _t_offsettype, class _t_iterator, class _t_container, class _t_reference>
void TestBTreeArrayIterCompound (_t_container *pContainer, _t_reference *pReference, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::value_type				value_type;
	typedef typename _t_reference::iterator					iter_ref_t;

	_t_iterator				sIter;
	_t_iterator				sIterBegin;
	_t_iterator				sIterEnd;
	value_type				sEntry;
	_t_offsettype			i;
	iter_ref_t				sIterRef;
	::std::string			sTypeStr;

	::std::cout << "basic array iterator compound operator test" << ::std::endl;

	get_typename (i, sTypeStr);

	::std::cout << "_t_offsettype is set to: " << sTypeStr << ::std::endl;

	arrayPrim_add (pContainer, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	get_begin (pContainer, sIter);

	get_begin (pContainer, sIterBegin);
	get_end (pContainer, sIterEnd);

	pReference->assign (sIterBegin, sIterEnd);

	get_begin (pReference, sIterRef);

	for (i = 0; i < (_t_offsettype) pContainer->size (); i++)
	{
		sIter += i;
		{
			sEntry = *sIter;
			sEntry.nData = ~sEntry.nData;
			*sIter = sEntry;
		}
		sIter -= i;

		sEntry = *sIterRef;
		sEntry.nData = ~sEntry.nData;
		*sIterRef = sEntry;

		sIterRef++;
	}

	get_begin (pContainer, sIter);

	get_begin (pReference, sIterRef);

	for (i = 0; i < (_t_offsettype) pContainer->size (); i++)
	{
		if (((value_type) *sIter) != ((value_type) *sIterRef))
		{
			::std::cerr << ::std::endl;
			::std::cerr << "array mismatch!" << ::std::endl;

			exit (-1);
		}

		sIter++;
		sIterRef++;
	}

	for (i = 0; i < (_t_offsettype) pContainer->size (); i++)
	{
		sIter -= (i + (_t_offsettype) 1);
		{
			sEntry = *sIter;
			sEntry.nData = ~sEntry.nData;
			*sIter = sEntry;
		}
		sIter += (i + (_t_offsettype) 1);

		sIterRef--;

		sEntry = *sIterRef;
		sEntry.nData = ~sEntry.nData;
		*sIterRef = sEntry;
	}

	get_begin (pContainer, sIter);

	get_begin (pReference, sIterRef);

	for (i = 0; i < (_t_offsettype) pContainer->size (); i++)
	{
		if (((value_type) *sIter) != ((value_type) *sIterRef))
		{
			::std::cerr << ::std::endl;
			::std::cerr << "array mismatch!" << ::std::endl;

			exit (-1);
		}

		sIter++;
		sIterRef++;
	}

	pContainer->clear ();
	pReference->clear ();
}

template<class _t_iterator, class _t_container, class _t_reference>
void TestBTreeArrayIterCompoundIter (_t_container *pContainer, _t_reference *pReference, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::value_type				value_type;

	typedef typename _t_reference::iterator					iter_ref_t;

	_t_iterator				sIter;
	_t_iterator				sIterBegin;
	_t_iterator				sIterEnd;
	value_type				sEntry;
	_t_iterator				sI;
	iter_ref_t				sIterRef;

	::std::cout << "basic array iterator compound operator (iterator) test" << ::std::endl;

	arrayPrim_add (pContainer, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	get_begin (pContainer, sIter);

	get_begin (pContainer, sIterBegin);
	get_end (pContainer, sIterEnd);

	pReference->assign (sIterBegin, sIterEnd);

	get_begin (pReference, sIterRef);

	for (sI = sIterBegin; sI != sIterEnd; sI++)
	{
		sIter += sI;
		{
			sEntry = *sIter;
			sEntry.nData = ~sEntry.nData;
			*sIter = sEntry;
		}
		sIter -= sI;
	}

	get_end (pContainer, sIter);

	for (sI = sIterEnd; sI != sIterBegin; )
	{
		sI--;

		sI -= pContainer->size ();
		{
			sIter += sI;
			{
				sEntry = *sIter;
				sEntry.nData = ~sEntry.nData;
				*sIter = sEntry;
			}
			sIter -= sI;
		}
		sI += pContainer->size ();
	}

	get_begin (pContainer, sIter);
	get_begin (pReference, sIterRef);

	while (sIter != sIterEnd)
	{
		if (((value_type) *sIter) != ((value_type) *sIterRef))
		{
			::std::cerr << ::std::endl;
			::std::cerr << "array mismatch! (reverse)" << ::std::endl;

			exit (-1);
		}

		sIter++;
		sIterRef++;
	}

	pContainer->clear ();
	pReference->clear ();
}

template<class _t_sizetype, class _t_iterator, class _t_container, class _t_reference>
void TestBTreeArrayIterArithmeticOperators (_t_container *pContainer, _t_reference *pReference, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::value_type				value_type;

	typedef typename _t_reference::const_iterator			citer_ref_t;

	_t_iterator				sIter;
	_t_iterator				sIterBegin;
	_t_iterator				sIterEnd;
	value_type				sEntryViaIter;
	value_type				sEntryViaRef;
	_t_sizetype				nOffset = 0;
	citer_ref_t				sCIterRef;

	::std::cout << "basic array iterator arithmetic operators test" << ::std::endl;

	arrayPrim_add (pContainer, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	get_begin (pContainer, sIterBegin);
	get_end (pContainer, sIterEnd);

	pReference->assign (sIterBegin, sIterEnd);

	get_begin (pReference, sCIterRef);

	::std::cout << "_t_iterator = _t_iterator + _t_sizetype" << ::std::endl;

	nOffset++;

	sIter = sIterBegin + (_t_sizetype) nOffset;

	::std::advance (sCIterRef, nOffset);
	{
		sEntryViaRef = *sCIterRef;
	}
	get_begin (pReference, sCIterRef);

	sEntryViaIter = *sIter;

	if (sEntryViaRef != sEntryViaIter)
	{
		::std::cerr << "failed!" << ::std::endl;

		exit (-1);
	}

	sEntryViaRef.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	::std::cout << "_t_iterator = _t_iterator + int" << ::std::endl;

	nOffset++;

	sIter = sIterBegin + (int) nOffset;

	::std::advance (sCIterRef, nOffset);
	{
		sEntryViaRef = *sCIterRef;
	}
	get_begin (pReference, sCIterRef);

	sEntryViaIter = *sIter;

	if (sEntryViaRef != sEntryViaIter)
	{
		::std::cerr << "failed!" << ::std::endl;

		exit (-1);
	}

	sEntryViaRef.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	::std::cout << "_t_iterator = _t_sizetype + _t_iterator" << ::std::endl;

	nOffset++;

	sIter = ((_t_sizetype) nOffset) + sIterBegin;

	::std::advance (sCIterRef, nOffset);
	{
		sEntryViaRef = *sCIterRef;
	}
	get_begin (pReference, sCIterRef);

	sEntryViaIter = *sIter;

	if (sEntryViaRef != sEntryViaIter)
	{
		::std::cerr << "failed!" << ::std::endl;

		exit (-1);
	}

	sEntryViaRef.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	::std::cout << "_t_iterator = int + _t_iterator" << ::std::endl;

	nOffset++;

	sIter = ((int) nOffset) + sIterBegin;

	::std::advance (sCIterRef, nOffset);
	{
		sEntryViaRef = *sCIterRef;
	}
	get_begin (pReference, sCIterRef);

	sEntryViaIter = *sIter;

	if (sEntryViaRef != sEntryViaIter)
	{
		::std::cerr << "failed!" << ::std::endl;

		exit (-1);
	}

	sEntryViaRef.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	::std::cout << "_t_iterator = _t_iterator - _t_sizetype" << ::std::endl;

	nOffset++;

	sIter = sIterEnd - (_t_sizetype) nOffset;

	::std::advance (sCIterRef, pReference->size () - nOffset);
	{
		sEntryViaRef = *sCIterRef;
	}
	get_begin (pReference, sCIterRef);

	sEntryViaIter = *sIter;

	if (sEntryViaRef != sEntryViaIter)
	{
		::std::cerr << "failed!" << ::std::endl;

		exit (-1);
	}

	sEntryViaRef.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	::std::cout << "const_iterator = const_iterator - int" << ::std::endl;

	nOffset++;

	sIter = sIterEnd - (int) nOffset;

	::std::advance (sCIterRef, pReference->size () - nOffset);
	{
		sEntryViaRef = *sCIterRef;
	}
	get_begin (pReference, sCIterRef);

	sEntryViaIter = *sIter;

	if (sEntryViaRef != sEntryViaIter)
	{
		::std::cerr << "failed!" << ::std::endl;

		exit (-1);
	}

	sEntryViaRef.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	::std::cout << "_t_sizetype = _t_iterator - _t_iterator" << ::std::endl;

	_t_sizetype		nSize = (_t_sizetype) (sIterEnd - sIterBegin);

	if (nSize != pContainer->size ())
	{
		::std::cout << "failed!" << ::std::endl;
	}

	pContainer->clear ();
	pReference->clear ();
}

template<class _t_container, class _t_lhiter, class _t_rhiter>
void TestBTreeArrayIterCompareOperators (_t_container *pContainer, typename _t_container::size_type nNumEntries, _t_lhiter sLHIterBegin, _t_lhiter sLHIterEnd, _t_rhiter sRHIterBegin, _t_rhiter sRHIterEnd)
{
	_t_lhiter		sLhs;
	_t_rhiter		sRhs;

	{
		::std::cout << " <";

		sLhs = sLHIterBegin;
		sRhs = sRHIterBegin;

		if (sLhs < sRhs)
		{
			exit (-1);
		}

		sRhs = sRHIterEnd;

		if (!(sLhs < sRhs))
		{
			exit (-1);
		}

		sLhs = sLHIterEnd;

		if (sLhs < sRhs)
		{
			exit (-1);
		}

		sRhs = sRHIterBegin;

		if (sLhs < sRhs)
		{
			exit (-1);
		}
	}

	{
		::std::cout << " <=";

		sLhs = sLHIterBegin;
		sRhs = sRHIterBegin;

		if (!(sLhs <= sRhs))
		{
			exit (-1);
		}

		sRhs = sRHIterEnd;

		if (!(sLhs <= sRhs))
		{
			exit (-1);
		}

		sLhs = sLHIterEnd;

		if (!(sLhs <= sRhs))
		{
			exit (-1);
		}

		sRhs = sRHIterBegin;

		if (sLhs <= sRhs)
		{
			exit (-1);
		}
	}

	{
		::std::cout << " >";

		sLhs = sLHIterBegin;
		sRhs = sRHIterBegin;

		if (sLhs > sRhs)
		{
			exit (-1);
		}

		sRhs = sRHIterEnd;

		if (sLhs > sRhs)
		{
			exit (-1);
		}

		sLhs = sLHIterEnd;

		if (sLhs > sRhs)
		{
			exit (-1);
		}

		sRhs = sRHIterBegin;

		if (!(sLhs > sRhs))
		{
			exit (-1);
		}
	}

	{
		::std::cout << " >=";

		sLhs = sLHIterBegin;
		sRhs = sRHIterBegin;

		if (!(sLhs >= sRhs))
		{
			exit (-1);
		}

		sRhs = sRHIterEnd;

		if (sLhs >= sRhs)
		{
			exit (-1);
		}

		sLhs = sLHIterEnd;

		if (!(sLhs >= sRhs))
		{
			exit (-1);
		}

		sRhs = sRHIterBegin;

		if (!(sLhs >= sRhs))
		{
			exit (-1);
		}
	}

	{
		::std::cout << " ==";

		sLhs = sLHIterBegin;
		sRhs = sRHIterBegin;

		if (!(sLhs == sRhs))
		{
			exit (-1);
		}

		sRhs = sRHIterEnd;

		if (sLhs == sRhs)
		{
			exit (-1);
		}

		sLhs = sLHIterEnd;

		if (!(sLhs == sRhs))
		{
			exit (-1);
		}

		sRhs = sRHIterBegin;

		if (sLhs == sRhs)
		{
			exit (-1);
		}
	}

	{
		::std::cout << " !=";

		sLhs = sLHIterBegin;
		sRhs = sRHIterBegin;

		if (sLhs != sRhs)
		{
			exit (-1);
		}

		sRhs = sRHIterEnd;

		if (!(sLhs != sRhs))
		{
			exit (-1);
		}

		sLhs = sLHIterEnd;

		if (sLhs != sRhs)
		{
			exit (-1);
		}

		sRhs = sRHIterBegin;

		if (!(sLhs != sRhs))
		{
			exit (-1);
		}
	}

	::std::cout << ::std::endl;
}

template<class _t_container>
void TestBTreeArrayIterCompareOperators (_t_container *pContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::iterator					iter_t;
	typedef typename _t_container::reverse_iterator			riter_t;
	typedef typename _t_container::const_iterator			citer_t;
	typedef typename _t_container::const_reverse_iterator	criter_t;

	::std::cout << "basic array iterator compare operators test" << ::std::endl;

	arrayPrim_add (pContainer, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	::std::cout << "iterator versus iterator";

	TestBTreeArrayIterCompareOperators<_t_container, iter_t, iter_t> (pContainer, nNumEntries, pContainer->begin (), pContainer->end (), pContainer->begin (), pContainer->end ());

	::std::cout << "const_iterator versus const_iterator";

	TestBTreeArrayIterCompareOperators<_t_container, citer_t, citer_t> (pContainer, nNumEntries, pContainer->cbegin (), pContainer->cend (), pContainer->cbegin (), pContainer->cend ());

	::std::cout << "const_iterator versus iterator";

	TestBTreeArrayIterCompareOperators<_t_container, citer_t, iter_t> (pContainer, nNumEntries, pContainer->cbegin (), pContainer->cend (), pContainer->begin (), pContainer->end ());

	::std::cout << "iterator versus const_iterator";

	TestBTreeArrayIterCompareOperators<_t_container, iter_t, citer_t> (pContainer, nNumEntries, pContainer->begin (), pContainer->end (), pContainer->cbegin (), pContainer->cend ());

	::std::cout << "reverse_iterator versus reverse_iterator";

	TestBTreeArrayIterCompareOperators<_t_container, riter_t, riter_t> (pContainer, nNumEntries, pContainer->rbegin (), pContainer->rend (), pContainer->rbegin (), pContainer->rend ());

	::std::cout << "const_reverse_iterator versus const_reverse_iterator";

	TestBTreeArrayIterCompareOperators<_t_container, criter_t, criter_t> (pContainer, nNumEntries, pContainer->crbegin (), pContainer->crend (), pContainer->crbegin (), pContainer->crend ());

	::std::cout << "const_reverse_iterator versus reverse_iterator";

//	TestBTreeArrayIterCompareOperators<_t_container, criter_t, riter_t> (pContainer, nNumEntries, pContainer->crbegin (), pContainer->crend (), pContainer->rbegin (), pContainer->rend ());
	::std::cout << ::std::endl;

	::std::cout << "reverse_iterator versus const_reverse_iterator";

//	TestBTreeArrayIterCompareOperators<_t_container, riter_t, criter_t> (pContainer, nNumEntries, pContainer->rbegin (), pContainer->rend (), pContainer->crbegin (), pContainer->crend ());
	::std::cout << ::std::endl;
}

template<class _t_iterator, class _t_container>
void TestBTreeArrayIterSwap (_t_container *pContainer, _t_container *pCntContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::value_type				value_type;

	_t_iterator				sIter;
	_t_iterator				sIter2;
	value_type				sEntry;
	uint64_t				ui64;

	::std::cout << "basic array iterator swap test" << ::std::endl;

	arrayPrim_add (pContainer, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	get_begin (pContainer, sIter);
	get_begin (pContainer, sIter2);

	*pCntContainer = *pContainer;

	for (ui64 = 0; ui64 < pContainer->size (); ui64++)
	{
		sEntry = *sIter;
		sEntry.nData = ~sEntry.nData;
		*sIter = sEntry;

		sIter++;
	}

	sIter.swap (sIter2);

	for (ui64 = 0; ui64 < pContainer->size (); ui64++)
	{
		sIter2--;

		sEntry = *sIter2;
		sEntry.nData = ~sEntry.nData;
		*sIter2 = sEntry;
	}

	if (*pCntContainer != *pContainer)
	{
		::std::cerr << ::std::endl;
		::std::cerr << "array mismatch!" << ::std::endl;

		exit (-1);
	}

	pContainer->clear ();
	pCntContainer->clear ();
}

template<class _t_iterator, class _t_container>
void TestBTreeArrayIterConstSwap (_t_container *pContainer, typename _t_container::size_type nNumEntries)
{
	_t_iterator				sIter;
	_t_iterator				sIter2;

	::std::cout << "basic array const iterator swap test" << ::std::endl;

	arrayPrim_add (pContainer, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	get_begin (pContainer, sIter);
	get_end (pContainer, sIter2);

	if (sIter >= sIter2)
	{
		exit (-1);
	}

	::std::cout << "swap ";

	sIter.swap (sIter2);

	if (sIter <= sIter2)
	{
		exit (-1);
	}

	::std::cout << "swap ";

	sIter2.swap (sIter);

	if (sIter >= sIter2)
	{
		exit (-1);
	}

	::std::cout << ::std::endl;

	pContainer->clear ();
}

template<class _t_iterator, class _t_container>
void TestBTreeArrayIterCCsameInstanceCompare (_t_container *pContainer)
{
	_t_iterator		sIter;
	_t_iterator		sIterDummy;
	_t_iterator		*(asIter0[]) = {&sIter, &sIterDummy};
	_t_iterator		*(asIter1[]) = {&sIterDummy, &sIter, &sIter};

	if (*(asIter0[0]) != *(asIter1[2]))
	{
		::std::cerr << "ERROR: uninitialised self instance compare failed!" << ::std::endl;

		exit (-1);
	}

	if ( ! (*(asIter0[0]) == *(asIter1[2])))
	{
		::std::cerr << "ERROR: uninitialised self instance compare failed! (reversed)" << ::std::endl;

		exit (-1);
	}

	get_end (pContainer, sIter);

	if (*(asIter0[0]) != *(asIter1[2]))
	{
		::std::cerr << "ERROR: empty / invalid self instance compare failed!" << ::std::endl;

		exit (-1);
	}

	if ( ! (*(asIter0[0]) == *(asIter1[2])))
	{
		::std::cerr << "ERROR: empty / invalid self instance compare failed! (reversed)" << ::std::endl;

		exit (-1);
	}

	arrayPrim_add (pContainer, 1, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	get_begin (pContainer, sIter);

	if (*(asIter0[0]) != *(asIter1[2]))
	{
		::std::cerr << "ERROR: valid self instance compare failed!" << ::std::endl;

		exit (-1);
	}

	if ( ! (*(asIter0[0]) == *(asIter1[2])))
	{
		::std::cerr << "ERROR: valid self instance compare failed! (reverse)" << ::std::endl;

		exit (-1);
	}

	pContainer->clear ();
}

template<class _t_iterator, class _t_container>
void TestBTreeArrayIterCCisTimeStampUpToDateAfterReAssignment (_t_container *pContainer, _t_container *pCntContainer)
{
	typedef typename _t_container::value_type		value_type;

	_t_iterator		sIter;
	_t_iterator		sCntIter;
	value_type		sData;
	value_type		sCntData;

	arrayPrim_add (pContainer, 1, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	*pCntContainer = *pContainer;

	get_begin (pContainer, sIter);

	sData = *sIter;

	get_begin (pCntContainer, sCntIter);

	sCntData = *sCntIter;

	sIter.swap (sCntIter);

	sIter = sCntIter;

	if (sData != *sIter)
	{
		::std::cerr << "ERROR: TestBTreeArrayIterCCisTimeStampUpToDateAfterReAssignment: Unexpected result!" << ::std::endl;

		exit (-1);
	}

	pContainer->clear ();
	pCntContainer->clear ();
}

template<class _t_iterator, class _t_container>
void TestBTreeArrayIterCCupdateTimeStampReRegister (_t_container *pContainer, _t_container *pCntContainer)
{
	typedef typename _t_container::value_type		value_type;

	_t_iterator		sIter;
	value_type		sData0;
	value_type		sData1;
	value_type		sData2;

	arrayPrim_add (pContainer, 2, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	*pCntContainer = *pContainer;

	get_begin (pContainer, sIter);

	sData0 = *sIter;

	get_begin (pCntContainer, sIter);

	sData1 = *sIter;

	::std::advance (sIter, 1);

	sData2 = *sIter;

	if ((sData0 != sData1) || (sData0 == sData2))
	{
		::std::cerr << "ERROR: TestBTreeArrayIterCCupdateTimeStampReRegister: Unexpected result!" << ::std::endl;

		exit (-1);
	}

	pContainer->clear ();
	pCntContainer->clear ();
}

template<class _t_iterator, class _t_container>
void TestBTreeArrayIterCCassignUninitializedInstance (_t_container *pContainer)
{
	typedef typename _t_container::value_type		value_type;

	_t_iterator		sIter;
	_t_iterator		sCntIter;

	sIter = sCntIter;

	sCntIter = sIter;

	arrayPrim_add (pContainer, 1, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	get_begin (pContainer, sIter);

	sIter = sCntIter;

	sCntIter = sIter;

	sIter.swap (sCntIter);

	get_begin (pContainer, sIter);

	sIter.swap (sCntIter);
}

template<class _t_container>
void TestBTreeArrayIterCCallBeginEndMethods (_t_container *pContainer)
{
	typedef typename _t_container::iterator					iterator;
	typedef typename _t_container::const_iterator			const_iterator;
	typedef typename _t_container::reverse_iterator			reverse_iterator;
	typedef typename _t_container::const_reverse_iterator	const_reverse_iterator;

	typedef typename _t_container::value_type				value_type;

	iterator						sIterBegin;
	const_iterator					sCIterBegin;
	const_iterator					sCIterBeginAlt;
	reverse_iterator				sRIterBegin;
	const_reverse_iterator			sCRIterBegin;
	const_reverse_iterator			sCRIterBeginAlt;

	iterator						sIterEnd;
	const_iterator					sCIterEnd;
	const_iterator					sCIterEndAlt;
	reverse_iterator				sRIterEnd;
	const_reverse_iterator			sCRIterEnd;
	const_reverse_iterator			sCRIterEndAlt;

	arrayPrim_add (pContainer, 2, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	sIterBegin = pContainer->begin ();
	sCIterBegin = pContainer->cbegin ();
	sCIterBeginAlt = ((const _t_container *) pContainer)->begin ();
	sRIterBegin = pContainer->rbegin ();
	sCRIterBegin = pContainer->crbegin ();
	sCRIterBeginAlt = ((const _t_container *) pContainer)->rbegin ();
	
	sIterEnd = pContainer->end ();
	sCIterEnd = pContainer->cend ();
	sCIterEndAlt = ((const _t_container *) pContainer)->end ();
	sRIterEnd = pContainer->rend ();
	sCRIterEnd = pContainer->crend ();
	sCRIterEndAlt = ((const _t_container *) pContainer)->rend ();

	if (sCIterBegin != sCIterBeginAlt)
	{
		::std::cerr << "TestBTreeArrayIterCCallBeginEndMethods: ERROR: sCIterBegin and sCIterBeginAlt mismatch!" << ::std::endl;

		exit (-1);
	}

	if (sCRIterBegin != sCRIterBeginAlt)
	{
		::std::cerr << "TestBTreeArrayIterCCallBeginEndMethods: ERROR: sCRIterBegin and sCRIterBeginAlt mismatch!" << ::std::endl;

		exit (-1);
	}

	if (sCIterEnd != sCIterEndAlt)
	{
		::std::cerr << "TestBTreeArrayIterCCallBeginEndMethods: ERROR: sCIterEnd and sCIterEndAlt mismatch!" << ::std::endl;

		exit (-1);
	}

	if (sCRIterEnd != sCRIterEndAlt)
	{
		::std::cerr << "TestBTreeArrayIterCCallBeginEndMethods: ERROR: sCRIterEnd and sCRIterEndAlt mismatch!" << ::std::endl;

		exit (-1);
	}

	sIterEnd--;
	sCIterEnd--;
	sCIterEndAlt--;
	sRIterEnd--;
	sCRIterEnd--;
	sCRIterEndAlt--;

	if (sCIterEnd != sCIterEndAlt)
	{
		::std::cerr << "TestBTreeArrayIterCCallBeginEndMethods: ERROR: sCIterEnd and sCIterEndAlt mismatch after decrementation!" << ::std::endl;

		exit (-1);
	}

	if (sCRIterEnd != sCRIterEndAlt)
	{
		::std::cerr << "TestBTreeArrayIterCCallBeginEndMethods: ERROR: sCRIterEnd and sCRIterEndAlt mismatch after decrementation!" << ::std::endl;

		exit (-1);
	}

	if (((value_type) *sIterBegin) != *sCRIterEndAlt)
	{
		::std::cerr << "TestBTreeArrayIterCCallBeginEndMethods: ERROR: sIterBegin and sCRIterEndAlt refer to different content!" << ::std::endl;

		exit (-1);
	}

	if (*sCIterBegin != *sCRIterEnd)
	{
		::std::cerr << "TestBTreeArrayIterCCallBeginEndMethods: ERROR: sCIterBegin and sCRIterEnd refer to different content!" << ::std::endl;

		exit (-1);
	}

	if (*sCIterBeginAlt != *sRIterEnd)
	{
		::std::cerr << "TestBTreeArrayIterCCallBeginEndMethods: ERROR: sCIterBeginAlt and sRIterEnd refer to different content!" << ::std::endl;

		exit (-1);
	}

	if (((value_type) *sRIterBegin) != *sCIterEndAlt)
	{
		::std::cerr << "TestBTreeArrayIterCCallBeginEndMethods: ERROR: sRIterBegin and sCIterEndAlt refer to different content!" << ::std::endl;

		exit (-1);
	}

	if (*sCRIterBegin != *sCIterEnd)
	{
		::std::cerr << "TestBTreeArrayIterCCallBeginEndMethods: ERROR: sCRIterBegin and sCIterEnd refer to different content!" << ::std::endl;

		exit (-1);
	}

	if (*sCRIterBeginAlt != *sIterEnd)
	{
		::std::cerr << "TestBTreeArrayIterCCallBeginEndMethods: ERROR: sCRIterBeginAlt and sIterEnd refer to different content!" << ::std::endl;

		exit (-1);
	}
}

template<class _t_sizetype>
void TestBTreeArrayIter (uint32_t nTest, uint32_t nNodeSize, uint32_t nPageSize)
{
	typedef	arrayEntry_t													data_t;

	typedef typename ::std::list<data_t>									reference_t;
	typedef CBTreeArrayIf<data_t, _t_sizetype>								container_t;
	typedef typename ::std::vector<container_t *>							test_vector_t;

	test_vector_t																	asContainers;
	reference_t																		sRefList;
	CBTreeIOpropertiesRAM<_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t>		sRAMprop6565;
	CBTreeIOpropertiesRAM<_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t>		sRAMprop6555;
	CBTreeIOpropertiesRAM<_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t>		sRAMprop5555;
	CBTreeIOpropertiesRAM<_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t>		sRAMprop5554;
	CBTreeIOpropertiesRAM<_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t>		sRAMprop5454;
	CBTreeIOpropertiesRAM<_t_sizetype, uint32_t, uint16_t, uint16_t, uint16_t>		sRAMprop5444;
	CBTreeIOpropertiesRAM<_t_sizetype, uint16_t, uint16_t, uint16_t, uint16_t>		sRAMprop4444;
	CBTreeIOpropertiesFile<_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t>		sFilePropertiesMin6565 ("./", 1);
	CBTreeIOpropertiesFile<_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t>		sFilePropertiesMin6555 ("./", 1);
	CBTreeIOpropertiesFile<_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t>		sFilePropertiesMin5555 ("./", 1);
	CBTreeIOpropertiesFile<_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t>		sFilePropertiesMin5554 ("./", 1);
	CBTreeIOpropertiesFile<_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t>		sFilePropertiesMin5454 ("./", 1);
	CBTreeIOpropertiesFile<_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t>		sFilePropertiesDefault6565 ("./");
	CBTreeIOpropertiesFile<_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t>		sFilePropertiesDefault6555 ("./");
	CBTreeIOpropertiesFile<_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t>		sFilePropertiesDefault5555 ("./");
	CBTreeIOpropertiesFile<_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t>		sFilePropertiesDefault5554 ("./");
	CBTreeIOpropertiesFile<_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t>		sFilePropertiesDefault5454 ("./");
	CBTreeIOpropertiesFile<_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t>		sFilePropertiesLarge6565 ("./", 16777216);
	CBTreeIOpropertiesFile<_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t>		sFilePropertiesLarge6555 ("./", 16777216);
	bayerTreeCacheDescription_t														sCacheDescPageSize = {nPageSize};
	bayerTreeCacheDescription_t														sCacheDescMin = {1};
	bayerTreeCacheDescription_t														sCacheDescNearestBigger = {nPageSize * 2 / 3};
	bayerTreeCacheDescription_t														sCacheDescLarge = {nPageSize * 16};
	uint32_t																		i = 0;
	container_t																		*pContainer;

	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerRAM6565_n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM6555_n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM5555_n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerRAM5554_n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerRAM5454_n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM5444_n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint16_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM4444_n;

	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerRAM6565_2n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM6555_2n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM5555_2n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerRAM5554_2n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerRAM5454_2n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM5444_2n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint16_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM4444_2n;

	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerRAM6565_4n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM6555_4n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM5555_4n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerRAM5554_4n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerRAM5454_4n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM5444_4n;
	CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint16_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM4444_4n;

	CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerFile6565min;
	CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile6555min;
	CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile5555min;
	CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerFile5554min;
	CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerFile5454min;

	CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerFile6565default;
	CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile6555default;
	CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile5555default;
	CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerFile5554default;
	CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerFile5454default;

	CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerFile6565large;
	CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile6555large;
	CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile5555large;
	CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerFile5554large;
	CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerFile5454large;

	::std::cout << "b-tree array iterator test bench selected" << ::std::endl;

	m_pContainerRAM6565_n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> > (sRAMprop6565, &sCacheDescPageSize, nNodeSize);
	m_pContainerRAM6555_n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> > (sRAMprop6555, &sCacheDescPageSize, nNodeSize);
	m_pContainerRAM5555_n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> > (sRAMprop5555, &sCacheDescPageSize, nNodeSize);
	m_pContainerRAM5554_n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> > (sRAMprop5554, &sCacheDescPageSize, nNodeSize);
	m_pContainerRAM5454_n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> > (sRAMprop5454, &sCacheDescPageSize, nNodeSize);
	m_pContainerRAM5444_n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint16_t, uint16_t> > (sRAMprop5444, &sCacheDescPageSize, nNodeSize);
	m_pContainerRAM4444_n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint16_t, uint16_t, uint16_t, uint16_t> > (sRAMprop4444, &sCacheDescPageSize, nNodeSize);
	m_pContainerRAM6565_2n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> > (sRAMprop6565, &sCacheDescPageSize, nNodeSize * 2);
	m_pContainerRAM6555_2n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> > (sRAMprop6555, &sCacheDescPageSize, nNodeSize * 2);
	m_pContainerRAM5555_2n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> > (sRAMprop5555, &sCacheDescPageSize, nNodeSize * 2);
	m_pContainerRAM5554_2n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> > (sRAMprop5554, &sCacheDescPageSize, nNodeSize * 2);
	m_pContainerRAM5454_2n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> > (sRAMprop5454, &sCacheDescPageSize, nNodeSize * 2);
	m_pContainerRAM5444_2n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint16_t, uint16_t> > (sRAMprop5444, &sCacheDescPageSize, nNodeSize * 2);
	m_pContainerRAM4444_2n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint16_t, uint16_t, uint16_t, uint16_t> > (sRAMprop4444, &sCacheDescPageSize, nNodeSize * 2);
	m_pContainerRAM6565_4n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> > (sRAMprop6565, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerRAM6555_4n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> > (sRAMprop6555, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerRAM5555_4n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> > (sRAMprop5555, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerRAM5554_4n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> > (sRAMprop5554, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerRAM5454_4n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> > (sRAMprop5454, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerRAM5444_4n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint16_t, uint16_t> > (sRAMprop5444, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerRAM4444_4n = new CBTreeArray<data_t, CBTreeIOpropertiesRAM <_t_sizetype, uint16_t, uint16_t, uint16_t, uint16_t> > (sRAMprop4444, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerFile6565min = new CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> > (sFilePropertiesMin6565, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerFile6555min = new CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesMin6555, &sCacheDescPageSize, nNodeSize * 8);
	m_pContainerFile5555min = new CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesMin5555, &sCacheDescMin, nNodeSize * 7);
	m_pContainerFile5554min = new CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> > (sFilePropertiesMin5554, &sCacheDescNearestBigger, nNodeSize * 6);
	m_pContainerFile5454min = new CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> > (sFilePropertiesMin5454, &sCacheDescLarge, nNodeSize * 5);

	m_pContainerFile6565default = new CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> > (sFilePropertiesDefault6565, &sCacheDescNearestBigger, nNodeSize * 3);
	m_pContainerFile6555default = new CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesDefault6555, &sCacheDescNearestBigger, nNodeSize * 2);
	m_pContainerFile5555default = new CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesDefault5555, &sCacheDescLarge, nNodeSize);
	m_pContainerFile5554default = new CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> > (sFilePropertiesDefault5554, &sCacheDescPageSize, nNodeSize * 8);
	m_pContainerFile5454default = new CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> > (sFilePropertiesDefault5454, &sCacheDescMin, nNodeSize * 7);

	m_pContainerFile6565large = new CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> > (sFilePropertiesLarge6565, &sCacheDescPageSize, nNodeSize * 5);
	m_pContainerFile6555large = new CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesLarge6555, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerFile5555large = new CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesDefault5555, &sCacheDescMin, nNodeSize * 3);
	m_pContainerFile5554large = new CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> > (sFilePropertiesDefault5554, &sCacheDescNearestBigger, nNodeSize * 2);
	m_pContainerFile5454large = new CBTreeArray<data_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> > (sFilePropertiesDefault5454, &sCacheDescLarge, nNodeSize);

	if (NULL == m_pContainerRAM6565_n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM6565_n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM6555_n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM6555_n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5555_n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5555_n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5554_n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5554_n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5454_n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5454_n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5444_n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5444_n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM4444_n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM4444_n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM6565_2n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM6565_2n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM6555_2n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM6555_2n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5555_2n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5555_2n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5554_2n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5554_2n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5454_2n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5454_2n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5444_2n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5444_2n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM4444_2n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM4444_2n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM6565_4n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM6565_4n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM6555_4n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM6555_4n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5555_4n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5555_4n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5554_4n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5554_4n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5454_4n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5454_4n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5444_4n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5444_4n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM4444_4n) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM4444_4n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile6565min) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile6565min)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile6555min) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile6555min)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5555min) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5555min)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5554min) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5554min)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5454min) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5454min)" << ::std::endl; exit (-1);}

	if (NULL == m_pContainerFile6565default) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile6565default)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile6555default) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile6555default)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5555default) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5555default)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5554default) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5554default)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5454default) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5454default)" << ::std::endl; exit (-1);}

	if (NULL == m_pContainerFile6565large) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile6565large)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile6555large) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile6555large)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5555large) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5555large)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5554large) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5554large)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5454large) {::std::cerr << "TestBTreeArrayIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5454large)" << ::std::endl; exit (-1);}

	pContainer = dynamic_cast<container_t *> (m_pContainerRAM6565_n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM6555_n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM5555_n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM5554_n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM5454_n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM5444_n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM4444_n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM6565_2n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM6555_2n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM5555_2n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM5554_2n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM5454_2n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM5444_2n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM4444_2n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM6565_4n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM6555_4n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM5555_4n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM5554_4n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM5454_4n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM5444_4n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerRAM4444_4n); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerFile6565min); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerFile6555min); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerFile5555min); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerFile5554min); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerFile5454min); asContainers.push_back (pContainer);

	pContainer = dynamic_cast<container_t *> (m_pContainerFile6565default); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerFile6555default); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerFile5555default); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerFile5554default); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerFile5454default); asContainers.push_back (pContainer);

	pContainer = dynamic_cast<container_t *> (m_pContainerFile6565large); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerFile6555large); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerFile5555large); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerFile5554large); asContainers.push_back (pContainer);
	pContainer = dynamic_cast<container_t *> (m_pContainerFile5454large); asContainers.push_back (pContainer);

	for (i = 0; i < asContainers.size (); i++)
	{
		pContainer = asContainers[i];

		switch (nTest)
		{
		case BTREETEST_ARRAY_ITER_ASCEND	:
			{
				TestBTreeArrayIterBasic<typename container_t::iterator> (pContainer, &sRefList, false, 128, 1, 0);
				TestBTreeArrayIterBasic<typename container_t::const_iterator> (pContainer, &sRefList, false, 128, 1, 0);
				TestBTreeArrayIterBasic<typename container_t::reverse_iterator> (pContainer, &sRefList, false, 128, 1, 0);
				TestBTreeArrayIterBasic<typename container_t::const_reverse_iterator> (pContainer, &sRefList, false, 128, 1, 0);

				break;
			}

		case BTREETEST_ARRAY_ITER_ASCEND_SMALL	:
			{
				TestBTreeArrayIterBasic<typename container_t::iterator> (pContainer, &sRefList, false, 16, 1, 0);
				TestBTreeArrayIterBasic<typename container_t::const_iterator> (pContainer, &sRefList, false, 16, 1, 0);
				TestBTreeArrayIterBasic<typename container_t::reverse_iterator> (pContainer, &sRefList, false, 16, 1, 0);
				TestBTreeArrayIterBasic<typename container_t::const_reverse_iterator> (pContainer, &sRefList, false, 16, 1, 0);

				break;
			}

		case BTREETEST_ARRAY_ITER_DESCEND	:
			{
				TestBTreeArrayIterBasic<typename container_t::iterator> (pContainer, &sRefList, true, 128, 1, 0);
				TestBTreeArrayIterBasic<typename container_t::const_iterator> (pContainer, &sRefList, true, 128, 1, 0);
				TestBTreeArrayIterBasic<typename container_t::reverse_iterator> (pContainer, &sRefList, true, 128, 1, 0);
				TestBTreeArrayIterBasic<typename container_t::const_reverse_iterator> (pContainer, &sRefList, true, 128, 1, 0);

				break;
			}

		case BTREETEST_ARRAY_ITER_DESCEND_SMALL	:
			{
				TestBTreeArrayIterBasic<typename container_t::iterator> (pContainer, &sRefList, true, 16, 1, 0);
				TestBTreeArrayIterBasic<typename container_t::const_iterator> (pContainer, &sRefList, true, 16, 1, 0);
				TestBTreeArrayIterBasic<typename container_t::reverse_iterator> (pContainer, &sRefList, true, 16, 1, 0);
				TestBTreeArrayIterBasic<typename container_t::const_reverse_iterator> (pContainer, &sRefList, true, 16, 1, 0);

				break;
			}

		case BTREETEST_ARRAY_ITER_CMPLX_ASCEND	:
			{
				TestBTreeArrayIterBasic<typename container_t::iterator> (pContainer, &sRefList, false, 128, 5, 3);
				TestBTreeArrayIterBasic<typename container_t::const_iterator> (pContainer, &sRefList, false, 128, 5, 3);
				TestBTreeArrayIterBasic<typename container_t::reverse_iterator> (pContainer, &sRefList, false, 128, 5, 3);
				TestBTreeArrayIterBasic<typename container_t::const_reverse_iterator> (pContainer, &sRefList, false, 128, 5, 3);

				break;
			}

		case BTREETEST_ARRAY_ITER_CMPLX_ASCEND_SMALL	:
			{
				TestBTreeArrayIterBasic<typename container_t::iterator> (pContainer, &sRefList, false, 16, 2, 3);
				TestBTreeArrayIterBasic<typename container_t::const_iterator> (pContainer, &sRefList, false, 16, 2, 3);
				TestBTreeArrayIterBasic<typename container_t::reverse_iterator> (pContainer, &sRefList, false, 16, 2, 3);
				TestBTreeArrayIterBasic<typename container_t::const_reverse_iterator> (pContainer, &sRefList, false, 16, 2, 3);

				break;
			}

		case BTREETEST_ARRAY_ITER_CMPLX_DESCEND	:
			{
				TestBTreeArrayIterBasic<typename container_t::iterator> (pContainer, &sRefList, true, 128, 4, 3);
				TestBTreeArrayIterBasic<typename container_t::const_iterator> (pContainer, &sRefList, true, 128, 4, 3);
				TestBTreeArrayIterBasic<typename container_t::reverse_iterator> (pContainer, &sRefList, true, 128, 4, 3);
				TestBTreeArrayIterBasic<typename container_t::const_reverse_iterator> (pContainer, &sRefList, true, 128, 4, 3);

				break;
			}

		case BTREETEST_ARRAY_ITER_CMPLX_DESCEND_SMALL	:
			{
				TestBTreeArrayIterBasic<typename container_t::iterator> (pContainer, &sRefList, true, 16, 2, 3);
				TestBTreeArrayIterBasic<typename container_t::const_iterator> (pContainer, &sRefList, true, 16, 2, 3);
				TestBTreeArrayIterBasic<typename container_t::reverse_iterator> (pContainer, &sRefList, true, 16, 2, 3);
				TestBTreeArrayIterBasic<typename container_t::const_reverse_iterator> (pContainer, &sRefList, true, 16, 2, 3);

				break;
			}

		case BTREETEST_ARRAY_ITER_NODESIZE_VS_STEPSIZE_ASCEND		:
			{
				TestBTreeArrayIterNodeSizeVsStepSize (pContainer, &sRefList, false, 512, 3, 257, 1, 513);

				break;
			}

		case BTREETEST_ARRAY_ITER_NODESIZE_VS_STEPSIZE_DESCEND	:
			{
				TestBTreeArrayIterNodeSizeVsStepSize (pContainer, &sRefList, true, 512, 3, 257, 1, 513);

				break;
			}

		case BTREETEST_ARRAY_ITER_BI_DIRECTIONAL_DEREFERENCE	:
			{
				TestBTreeArrayIterBiDirectionalDereference<typename container_t::iterator> (pContainer, &sRefList, 128);
				TestBTreeArrayIterBiDirectionalDereference<typename container_t::reverse_iterator> (pContainer, &sRefList, 128);

				break;
			}

		case BTREETEST_ARRAY_ITER_BI_DIRECTIONAL_SUBSCRIPTOR	:
			{
				TestBTreeArrayIterBiDirectionalSubScriptor<_t_sizetype, typename container_t::iterator> (pContainer, &sRefList, 128);
				TestBTreeArrayIterBiDirectionalSubScriptor<_t_sizetype, typename container_t::reverse_iterator> (pContainer, &sRefList, 128);

				break;
			}

		case BTREETEST_ARRAY_ITER_BI_DIRECTIONAL_SUBSCRIPTOR_INT	:
			{
				TestBTreeArrayIterBiDirectionalSubScriptor<int, typename container_t::iterator> (pContainer, &sRefList, 128);
				TestBTreeArrayIterBiDirectionalSubScriptor<int, typename container_t::reverse_iterator> (pContainer, &sRefList, 128);

				break;
			}

		case BTREETEST_ARRAY_ITER_COMPOUND	:
			{
				TestBTreeArrayIterCompound<_t_sizetype, typename container_t::iterator> (pContainer, &sRefList, 128);
				TestBTreeArrayIterCompound<_t_sizetype, typename container_t::reverse_iterator> (pContainer, &sRefList, 128);

				break;
			}

		case BTREETEST_ARRAY_ITER_COMPOUND_INT	:
			{
				TestBTreeArrayIterCompound<int, typename container_t::iterator> (pContainer, &sRefList, 128);
				TestBTreeArrayIterCompound<int, typename container_t::reverse_iterator> (pContainer, &sRefList, 128);

				break;
			}

		case BTREETEST_ARRAY_ITER_COMPOUND_ITER	:
			{
				TestBTreeArrayIterCompoundIter<typename container_t::iterator> (pContainer, &sRefList, 128);

				break;
			}

		case BTREETEST_ARRAY_ITER_ARITHMETIC_OPERATORS	:
			{
				TestBTreeArrayIterArithmeticOperators<_t_sizetype, typename container_t::iterator> (pContainer, &sRefList, 40);
				TestBTreeArrayIterArithmeticOperators<_t_sizetype, typename container_t::const_iterator> (pContainer, &sRefList, 40);
				TestBTreeArrayIterArithmeticOperators<_t_sizetype, typename container_t::reverse_iterator> (pContainer, &sRefList, 40);
				TestBTreeArrayIterArithmeticOperators<_t_sizetype, typename container_t::const_reverse_iterator> (pContainer, &sRefList, 40);

				TestBTreeArrayIterArithmeticOperators<int, typename container_t::iterator> (pContainer, &sRefList, 40);
				TestBTreeArrayIterArithmeticOperators<int, typename container_t::const_iterator> (pContainer, &sRefList, 40);
				TestBTreeArrayIterArithmeticOperators<int, typename container_t::reverse_iterator> (pContainer, &sRefList, 40);
				TestBTreeArrayIterArithmeticOperators<int, typename container_t::const_reverse_iterator> (pContainer, &sRefList, 40);

				break;
			}

		case BTREETEST_ARRAY_ITER_COMPARE_OPERATORS	:
			{
				TestBTreeArrayIterCompareOperators (pContainer, 16);

				break;
			}

		case BTREETEST_ARRAY_ITER_SWAP	:
			{
				container_t		*pCntContainer = asContainers[(i + 1) % asContainers.size ()];

				TestBTreeArrayIterSwap<typename container_t::iterator> (pContainer, pCntContainer, 128);
				TestBTreeArrayIterSwap<typename container_t::reverse_iterator> (pContainer, pCntContainer, 128);

				break;
			}

		case BTREETEST_ARRAY_ITER_CONST_SWAP	:
			{
				TestBTreeArrayIterConstSwap<typename container_t::iterator> (pContainer, 16);
				TestBTreeArrayIterConstSwap<typename container_t::const_iterator> (pContainer, 16);
				TestBTreeArrayIterConstSwap<typename container_t::reverse_iterator> (pContainer, 16);
				TestBTreeArrayIterConstSwap<typename container_t::const_reverse_iterator> (pContainer, 16);

				break;
			}

		case BTREETEST_ARRAY_ITER_CC_SAME_INSTANCE_COMPARE	:
			{
				TestBTreeArrayIterCCsameInstanceCompare<typename container_t::iterator> (pContainer);
				TestBTreeArrayIterCCsameInstanceCompare<typename container_t::const_iterator> (pContainer);
				TestBTreeArrayIterCCsameInstanceCompare<typename container_t::reverse_iterator> (pContainer);
				TestBTreeArrayIterCCsameInstanceCompare<typename container_t::const_reverse_iterator> (pContainer);

				break;
			}

		case BTREETEST_ARRAY_ITER_CC_IS_TIME_STAMP_UP_TO_DATE_AFTER_RE_ASSIGNMENT:
			{
				container_t		*pCntContainer = asContainers[(i + 1) % asContainers.size ()];

				TestBTreeArrayIterCCisTimeStampUpToDateAfterReAssignment<typename container_t::iterator> (pContainer, pCntContainer);
				TestBTreeArrayIterCCisTimeStampUpToDateAfterReAssignment<typename container_t::const_iterator> (pContainer, pCntContainer);
				TestBTreeArrayIterCCisTimeStampUpToDateAfterReAssignment<typename container_t::reverse_iterator> (pContainer, pCntContainer);
				TestBTreeArrayIterCCisTimeStampUpToDateAfterReAssignment<typename container_t::const_reverse_iterator> (pContainer, pCntContainer);

				break;
			}

		case BTREETEST_ARRAY_ITER_CC_UPDATE_TIME_STAMP_RE_REGISTER:
			{
				container_t		*pCntContainer = asContainers[(i + 1) % asContainers.size ()];

				TestBTreeArrayIterCCupdateTimeStampReRegister<typename container_t::iterator> (pContainer, pCntContainer);
				TestBTreeArrayIterCCupdateTimeStampReRegister<typename container_t::const_iterator> (pContainer, pCntContainer);
				TestBTreeArrayIterCCupdateTimeStampReRegister<typename container_t::reverse_iterator> (pContainer, pCntContainer);
				TestBTreeArrayIterCCupdateTimeStampReRegister<typename container_t::const_reverse_iterator> (pContainer, pCntContainer);

				break;
			}

		case BTREETEST_ARRAY_ITER_CC_ASSIGN_UNINITIALIZED_INSTANCE:
			{
				TestBTreeArrayIterCCassignUninitializedInstance<typename container_t::iterator> (pContainer);
				TestBTreeArrayIterCCassignUninitializedInstance<typename container_t::const_iterator> (pContainer);
				TestBTreeArrayIterCCassignUninitializedInstance<typename container_t::reverse_iterator> (pContainer);
				TestBTreeArrayIterCCassignUninitializedInstance<typename container_t::const_reverse_iterator> (pContainer);

				break;
			}

		case BTREETEST_ARRAY_ITER_CC_ALL_BEGIN_END_METHODS:
			{
				TestBTreeArrayIterCCallBeginEndMethods (pContainer);
				
				break;
			}

		default									:
			{
				::std::cerr << "ERROR: TestBTreeArrayIter: Unknown test or test not specified!" << ::std::endl;

				exit (-1);

				break;
			}
		}
	}

	for (i = 0; i < asContainers.size (); i++)
	{
		pContainer = asContainers[i];

		delete pContainer;
	}
}
