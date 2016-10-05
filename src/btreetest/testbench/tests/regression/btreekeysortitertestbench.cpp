/************************************************************
**
** file:	btreekeysortitertestbench.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for the key sort iterator data class.
**
************************************************************/

#include "btreekeysortitertestbench.h"

template<class _t_container, class _t_reference>
void TransferKeySortIterRefToTestContainer (_t_container *pContainer, _t_reference *pReference)
{
	typedef typename _t_container::value_type			data_t;
	typedef typename _t_reference::const_iterator		citer_t;

	citer_t		sIterRefBegin;
	citer_t		sIterRefEnd;
	data_t		sData;

	get_begin (pReference, sIterRefBegin);
	get_end (pReference, sIterRefEnd);

	for (; sIterRefBegin != sIterRefEnd; ::std::advance (sIterRefBegin, 1))
	{
		entry_conversion (sData, *sIterRefBegin);

		pContainer->insert (sData);
	}
}

template<class _t_container, class _t_reference, class _t_iterator, class _t_ref_iterator, class _t_sizetype>
void TestBTreeKeySortConstIterBasic (_t_container *pContainer, _t_reference *pReference, _t_iterator sIter, _t_ref_iterator sIterRef, _t_sizetype nIter, _t_sizetype nRIter, _t_sizetype nPos, uint32_t nTurn)
{
	typedef typename _t_container::value_type					data_t;
	
	_t_sizetype		nTestDiff;
	_t_iterator		sIterBegin;
	bool			bMismatch;

	data_t			sEntryViaIter;
	data_t			sEntryViaRef;
	
	get_begin (pContainer, sIterBegin);

	nTestDiff = ::std::distance (sIterBegin, sIter);

	bMismatch = nTestDiff != nIter;

	if (bMismatch)
	{
		::std::cerr << ::std::endl;
		::std::cerr << "unexpected iterator distance!" << ::std::endl;
		::std::cerr << "turn around: " << nTurn << ::std::endl;
		::std::cerr << "position: " << nPos << ::std::endl;
		::std::cerr << "iterator distance: " << nTestDiff;
		::std::cerr << "expected iterator distance: " << nIter;

		exit (-1);
	}

	if (is_reverse_iterator (pContainer, sIter))
	{
		::std::advance (sIterRef, nRIter);
	}
	else
	{
		::std::advance (sIterRef, nIter);
	}

	entry_conversion (sEntryViaRef, *sIterRef);

	sEntryViaIter = *sIter;

	if (sEntryViaRef != sEntryViaIter)
	{
		::std::cerr << ::std::endl;

		if (is_const_iterator (pContainer, sIter))
		{
			::std::cerr << "const ";
		}

		if (is_reverse_iterator (pContainer, sIter))
		{
			::std::cerr << "reverse ";
		}

		::std::cerr << "iterator mismatch!" << ::std::endl;
		::std::cerr << "turn around: " << nTurn << ::std::endl;
		::std::cerr << "position: " << nPos << ::std::endl;
		::std::cerr << "data via reference" << ::std::endl;
		::std::cerr << "nData: " << sEntryViaRef.nData << ::std::endl;
		::std::cerr << "nDebug: " << sEntryViaRef.nDebug << ::std::endl;
		::std::cerr << "data via iterator" << ::std::endl;
		::std::cerr << "nData: " << sEntryViaIter.nData << ::std::endl;
		::std::cerr << "nDebug: " << sEntryViaIter.nDebug << ::std::endl;

		exit (-1);
	}
}

template<class _t_container, class _t_reference>
void TestBTreeKeySortConstIterBasic (_t_container *pContainer, _t_reference *pReference, bool bDescend, typename _t_container::size_type nNumEntries, typename _t_container::size_type nStepSize, uint32_t nTurnArounds, bool bFillArray = true)
{
	typedef typename _t_container::iterator						iter_t;
	typedef typename _t_container::const_iterator				citer_t;
	typedef typename _t_container::reverse_iterator				riter_t;
	typedef typename _t_container::const_reverse_iterator		criter_t;

	typedef typename _t_reference::const_iterator				citer_ref_t;

	typedef typename _t_container::size_type					size_type;
	typedef typename _t_container::value_type					data_t;

	typedef typename _t_reference::value_type					data_ref_t;
	typedef typename _t_reference::size_type					size_type_ref;

	uint32_t				nSeed = 0;
	iter_t					sIter;
	citer_t					sCIter;
	riter_t					sRIter;
	criter_t				sCRIter;
	citer_ref_t				sCIterRef;
	data_ref_t				sEntryRef;
	size_type				i;
	size_type				nIter;
	size_type				nRIter;
	uint32_t				nTurn;
	size_type				nThisStepSize;
	size_type_ref			nNumEntriesRef = (size_type_ref) nNumEntries;

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
		associative_container_add_primitive (pReference, nNumEntriesRef, nSeed, BTREETEST_KEY_GENERATION_RANDOM);

		TransferKeySortIterRefToTestContainer (pContainer, pReference);
	}

	for (nTurn = 0; nTurn <= nTurnArounds; nTurn++)
	{
		::std::cout << "turn: " << nTurn << ::std::endl;

		bool	bForward = (((nTurn & 0x1) == 0x0) != bDescend);

		i = bForward ? 0ULL : pContainer->size ();

		sIter = bForward ? pContainer->begin () : pContainer->end ();
		sCIter = bForward ? pContainer->cbegin () : pContainer->cend ();
		sRIter = bForward ? pContainer->rbegin () : pContainer->rend ();
		sCRIter = bForward ? pContainer->crbegin () : pContainer->crend ();

		nIter = bForward ? 0 : (size_type) pReference->size ();
		nRIter = bForward ? (size_type) pReference->size () : 0;
		nRIter--;

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
						sCIter--;
						sRIter--;
						sCRIter--;
					}
					else
					{
						--sIter;
						--sCIter;
						--sRIter;
						--sCRIter;
					}

					nIter--;
					nRIter++;
				}
				else
				{
					sIter -= (size_type) nThisStepSize;
					sCIter -= (size_type) nThisStepSize;
					sRIter -= (size_type) nThisStepSize;
					sCRIter -= (size_type) nThisStepSize;

					nIter -= (size_type) nThisStepSize;
					nRIter += (size_type) nThisStepSize;
				}
			}

			::std::cout << i << " " << "\r" << ::std::flush;

			sCIterRef = pReference->cbegin ();

			TestBTreeKeySortConstIterBasic (pContainer, pReference, sIter, sCIterRef, nIter, nRIter, i, nTurn);
			TestBTreeKeySortConstIterBasic (pContainer, pReference, sCIter, sCIterRef, nIter, nRIter, i, nTurn);
			TestBTreeKeySortConstIterBasic (pContainer, pReference, sRIter, sCIterRef, nIter, nRIter, i, nTurn);
			TestBTreeKeySortConstIterBasic (pContainer, pReference, sCRIter, sCIterRef, nIter, nRIter, i, nTurn);

			if (bForward)
			{
				i += nStepSize;

				if (nStepSize == 1)
				{
					if ((i & 0x1) == 0x0)
					{
						sIter++;
						sCIter++;
						sRIter++;
						sCRIter++;
					}
					else
					{
						++sIter;
						++sCIter;
						++sRIter;
						++sCRIter;
					}

					nIter++;
					nRIter--;
				}
				else
				{
					sIter += (size_type) nStepSize;
					sCIter += (size_type) nStepSize;
					sRIter += (size_type) nStepSize;
					sCRIter += (size_type) nStepSize;

					nIter += (size_type) nStepSize;
					nRIter -= (size_type) nStepSize;
				}
			}
		}

		::std::cout << ::std::endl;
	}
}

template<class _t_container, class _t_reference>
void TestBTreeKeySortConstIterNodeSizeVsStepSize (_t_container *pContainer, _t_reference *pReference, bool bDescend, typename _t_container::size_type nNumEntries, uint32_t nFromNodeSize, uint32_t nToNodeSize, uint32_t nFromStepSize, uint32_t nToStepSize)
{
	typedef typename _t_container::size_type		size_type;
	typedef typename _t_reference::size_type		size_type_ref;

	size_type			nStepSize;
	size_type_ref		nNumEntriesRef = (size_type_ref) nNumEntries;
	uint32_t			nSeed;
	
	nSeed = 0;

	associative_container_add_primitive (pReference, nNumEntriesRef, nSeed, BTREETEST_KEY_GENERATION_RANDOM);
		
	TransferKeySortIterRefToTestContainer (pContainer, pReference);
	
	for (nStepSize = nFromStepSize; nStepSize < nToStepSize; nStepSize <<= 1)
	{
		TestBTreeKeySortConstIterBasic (pContainer, pReference, bDescend, nNumEntries, nStepSize, 0, false);
	}

	pContainer->clear ();
}

template<class _t_subscripttype, class _t_container, class _t_iterator>
void TestBTreeKeySortIterSubScriptor (_t_container *pContainer, _t_container *pContainerRef, _t_iterator &rIter, _t_subscripttype &rI, bool bForward)
{
	typedef typename _t_container::value_type						data_t;

	data_t			sData;

	if (bForward)
	{
		::std::cout << "progrssing ";
	}
	else
	{
		::std::cout << "reversing ";
	}

	if (is_const_iterator (pContainer, rIter))
	{
		::std::cout << "const ";
	}

	if (is_reverse_iterator (pContainer, rIter))
	{
		::std::cout << "reverse ";
	}

	::std::cout << "iterator ";
	
	if (bForward)
	{
		get_begin (pContainer, rIter);

		for (rI = 0; (typename ::std::make_unsigned<_t_subscripttype>::type) rI < pContainer->size (); rI++)
		{
			sData = rIter[rI];

			pContainerRef->insert (sData);
		}
	}
	else
	{
		get_end (pContainer, rIter);

		for (rI = (_t_subscripttype) pContainer->size (); rI > (_t_subscripttype) 0; )
		{
			rI--;

			rI -= (_t_subscripttype) pContainer->size ();
			{
				sData = rIter[rI];

				pContainerRef->insert (sData);
			}
			rI += (_t_subscripttype) pContainer->size ();
		}
	}

	if (*pContainer != *pContainerRef)
	{
		::std::cout << "failed" << ::std::endl;
		
		exit (-1);
	}

	::std::cout << "passed" << ::std::endl;

	pContainerRef->clear ();
}

template<class _t_subscripttype, class _t_container>
void TestBTreeKeySortIterSubScriptor (_t_container *pContainer, _t_container *pCntContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::value_type						data_t;

	typedef typename _t_container::iterator							iter_t;
	typedef typename _t_container::const_iterator					citer_t;
	typedef typename _t_container::reverse_iterator					riter_t;
	typedef typename _t_container::const_reverse_iterator			criter_t;

	uint32_t				nSeed = 0;
	iter_t					sIter;
	citer_t					sCIter;
	riter_t					sRIter;
	criter_t				sCRIter;
	_t_subscripttype		i;
	::std::string			sTypeStr;
	
	::std::cout << "basic keysort iterator sub-scription test" << ::std::endl;
	
	get_typename (i, sTypeStr);

	::std::cout << "_t_subscripttype is set to: " << sTypeStr << ::std::endl;
	
	associative_container_add_primitive (pContainer, nNumEntries, nSeed, BTREETEST_KEY_GENERATION_RANDOM);

	TestBTreeKeySortIterSubScriptor (pContainer, pCntContainer, sIter, i, true);
	TestBTreeKeySortIterSubScriptor (pContainer, pCntContainer, sCIter, i, true);
	TestBTreeKeySortIterSubScriptor (pContainer, pCntContainer, sRIter, i, true);
	TestBTreeKeySortIterSubScriptor (pContainer, pCntContainer, sCRIter, i, true);

	TestBTreeKeySortIterSubScriptor (pContainer, pCntContainer, sIter, i, false);
	TestBTreeKeySortIterSubScriptor (pContainer, pCntContainer, sCIter, i, false);
	TestBTreeKeySortIterSubScriptor (pContainer, pCntContainer, sRIter, i, false);
	TestBTreeKeySortIterSubScriptor (pContainer, pCntContainer, sCRIter, i, false);
}

template<class _t_offsettype, class _t_container, class _t_iterator>
void TestBTreeKeySortIterCompound (_t_container *pContainer, _t_container *pContainerRef, _t_iterator &rIter, _t_offsettype &rI, bool bForward)
{
	typedef typename _t_container::value_type						data_t;

	data_t			sData;

	if (bForward)
	{
		::std::cout << "progrssing ";
	}
	else
	{
		::std::cout << "reversing ";
	}

	if (is_const_iterator (pContainer, rIter))
	{
		::std::cout << "const ";
	}

	if (is_reverse_iterator (pContainer, rIter))
	{
		::std::cout << "reverse ";
	}

	::std::cout << "iterator ";
	
	if (bForward)
	{
		get_begin (pContainer, rIter);

		for (rI = 0; rI < (_t_offsettype) pContainer->size (); rI++)
		{
			rIter += rI;
			{
				sData = *rIter;

				pContainerRef->insert (sData);
			}
			rIter -= rI;
		}
	}
	else
	{
		get_end (pContainer, rIter);

		for (rI = (_t_offsettype) pContainer->size (); rI > (_t_offsettype) 0; )
		{
			rI--;

			rI -= (_t_offsettype) pContainer->size ();
			{
				rIter += rI;
				{
					sData = *rIter;

					pContainerRef->insert (sData);
				}
				rIter -= rI;
			}
			rI += (_t_offsettype) pContainer->size ();
		}
	}

	if (*pContainer != *pContainerRef)
	{
		::std::cout << "failed" << ::std::endl;
		
		exit (-1);
	}

	::std::cout << "passed" << ::std::endl;

	pContainerRef->clear ();
}

template<class _t_offsettype, class _t_container>
void TestBTreeKeySortIterCompound (_t_container *pContainer, _t_container *pCntContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::iterator							iter_t;
	typedef typename _t_container::const_iterator					citer_t;
	typedef typename _t_container::reverse_iterator					riter_t;
	typedef typename _t_container::const_reverse_iterator			criter_t;

	uint32_t				nSeed = 0;
	citer_t					sIter;
	citer_t					sCIter;
	criter_t				sRIter;
	criter_t				sCRIter;
	_t_offsettype			i;
	::std::string			strType;
	
	::std::cout << "basic keysort iterator compound operator test" << ::std::endl;

	get_typename (i, strType);
	
	::std::cout << "_t_sizetype is set to: " << strType << ::std::endl;
	
	associative_container_add_primitive (pContainer, nNumEntries, nSeed, BTREETEST_KEY_GENERATION_RANDOM);

	TestBTreeKeySortIterCompound (pContainer, pCntContainer, sIter, i, true);
	TestBTreeKeySortIterCompound (pContainer, pCntContainer, sCIter, i, true);
	TestBTreeKeySortIterCompound (pContainer, pCntContainer, sRIter, i, true);
	TestBTreeKeySortIterCompound (pContainer, pCntContainer, sCRIter, i, true);

	TestBTreeKeySortIterCompound (pContainer, pCntContainer, sIter, i, false);
	TestBTreeKeySortIterCompound (pContainer, pCntContainer, sCIter, i, false);
	TestBTreeKeySortIterCompound (pContainer, pCntContainer, sRIter, i, false);
	TestBTreeKeySortIterCompound (pContainer, pCntContainer, sCRIter, i, false);
}

template<class _t_container>
void TestBTreeKeySortIterCompoundIter (_t_container *pContainer, _t_container *pCntContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::value_type				data_t;

	typedef typename _t_container::const_iterator			citer_t;
	
	uint32_t				nSeed = 0;
	citer_t					sCIter;
	data_t					sData;
	citer_t					sCI;
	
	::std::cout << "basic keysort iterator compound operator (iterator) test" << ::std::endl;
	
	associative_container_add_primitive (pContainer, nNumEntries, nSeed, BTREETEST_KEY_GENERATION_RANDOM);
	
	sCIter = pContainer->cbegin ();
	
	::std::cout << "forward iterator ";

	for (sCI = pContainer->cbegin (); sCI != pContainer->cend (); sCI++)
	{
		sCIter += sCI;
		{
			sData = *sCIter;
			
			pCntContainer->insert (sData);
		}
		sCIter -= sCI;
	}

	::std::cout << "testing ";

	if (*pCntContainer != *pContainer)
	{
		::std::cerr << ::std::endl;
		::std::cerr << "keysort mismatch (forward iterator compound operation)!" << ::std::endl;
		
		exit (-1);
	}

	::std::cout << "passed" << ::std::endl;

	pCntContainer->clear ();

	sCIter = pContainer->cend ();
	
	::std::cout << "reverse iterator ";

	for (sCI = pContainer->cend (); sCI != pContainer->cbegin (); )
	{
		sCI--;

		sCI -= pContainer->size ();
		{
			sCIter += sCI;
			{
				sData = *sCIter;
				
				pCntContainer->insert (sData);
			}
			sCIter -= sCI;
		}
		sCI += pContainer->size ();
	}

	::std::cout << "testing ";

	if (*pCntContainer != *pContainer)
	{
		::std::cerr << ::std::endl;
		::std::cerr << "keysort mismatch (reverse iterator compound operation)!" << ::std::endl;
		
		exit (-1);
	}

	::std::cout << "passed" << ::std::endl;

	pCntContainer->clear ();
}

template<class _t_container, class _t_reference, class _t_dest_iterator, class _t_operand_a, class _t_operand_b>
void TestBTreeKeySortIterArithmeticOperators (_t_container *pContainer, _t_reference *pReference, _t_dest_iterator &rIterDest, _t_operand_a &rOperandA, _t_operand_b &rOperandB, const typename _t_container::size_type nExpectedOffset, ::std::false_type)
{
	typedef typename _t_container::size_type		sizetype;
	typedef typename _t_container::value_type		data_t;

	typedef typename _t_container::const_iterator	citer_t;

	typedef typename _t_reference::const_iterator	citer_ref_t;

	_t_dest_iterator	sIterBegin;
	sizetype			nDiff;
	citer_ref_t			sCIterRef;
	data_t				sData;
	data_t				sDataRef;

	get_begin (pContainer, sIterBegin);

	nDiff = ::std::distance (sIterBegin, rIterDest);

	if (is_reverse_iterator (pContainer, rIterDest))
	{
		nDiff = pContainer->size () - nDiff - 1;
	}
	
	if (nDiff != nExpectedOffset)
	{
		::std::cout << "failed" << ::std::endl;
		::std::cout << "resulting distance doesn't match expected result!" << ::std::endl;

		exit (-1);
	}

	get_begin (pReference, sCIterRef);

	::std::advance (sCIterRef, nExpectedOffset);

	citer_ref_t			sCIterRefEnd;
	_t_dest_iterator	sIterEnd;

	get_end (pReference, sCIterRefEnd);
	get_end (pContainer, sIterEnd);

	if ((sCIterRef == sCIterRefEnd) != (rIterDest == sIterEnd))
	{
		::std::cout << "failed" << ::std::endl;
		::std::cout << "data set position and reference position are not matching on cend ()!" << ::std::endl;

		exit (-1);
	}

	if ((sCIterRef != sCIterRefEnd) || (rIterDest != sIterEnd))
	{
		entry_conversion (sDataRef, *sCIterRef);

		sData = *rIterDest;

		if (sData != sDataRef)
		{
			::std::cout << "failed" << ::std::endl;
			::std::cout << "returned data set doesn't match reference data set!" << ::std::endl;

			exit (-1);
		}
	}
	
	::std::cout << "passed";
}

template<class _t_container, class _t_reference, class _t_dest_iterator, class _t_operand_a, class _t_operand_b>
void TestBTreeKeySortIterArithmeticOperators (_t_container *pContainer, _t_reference *pReference, _t_dest_iterator &rDiff, _t_operand_a &rOperandA, _t_operand_b &rOperandB, const typename _t_container::size_type nExpectedOffset, ::std::true_type)
{
	typedef typename _t_container::size_type		sizetype;
	
	if (rDiff != nExpectedOffset)
	{
		::std::cout << "failed" << ::std::endl;
		::std::cout << "resulting distance doesn't match expected result!" << ::std::endl;

		exit (-1);
	}

	sizetype			nNewExpectedOffset;
	_t_operand_a		sIterA;
	::std::false_type	sFalse;

	get_begin (pContainer, sIterA);

	nNewExpectedOffset = ::std::distance (sIterA, rOperandB);

	TestBTreeKeySortIterArithmeticOperators (pContainer, pReference, rOperandB, rOperandA, rDiff, nNewExpectedOffset, sFalse);
}

template<class _t_container, class _t_reference, class _t_dest_iterator, class _t_operand_a, class _t_operand_b>
void TestBTreeKeySortIterArithmeticOperatorAdd (_t_container *pContainer, _t_reference *pReference, const char *pszDest, const char *pszOperandA, const char *pszOperandB, _t_dest_iterator &rIterDest, _t_operand_a &rOperandA, _t_operand_b &rOperandB, const typename _t_container::size_type nExpectedOffset)
{
	typename ::std::is_integral<_t_dest_iterator>::type		sDestSelect;

	::std::cout << pszDest << " = " << pszOperandA << " + " << pszOperandB << " ";

	rIterDest = rOperandA + rOperandB;

	TestBTreeKeySortIterArithmeticOperators (pContainer, pReference, rIterDest, rOperandA, rOperandB, nExpectedOffset, sDestSelect);

	::std::cout << ::std::endl;
}

template<class _t_container, class _t_reference, class _t_dest_iterator, class _t_operand_a, class _t_operand_b>
void TestBTreeKeySortIterArithmeticOperatorSub (_t_container *pContainer, _t_reference *pReference, const char *pszDest, const char *pszOperandA, const char *pszOperandB, _t_dest_iterator &rIterDest, _t_operand_a &rOperandA, _t_operand_b &rOperandB, const typename _t_container::size_type nExpectedOffset)
{
	typename ::std::is_integral<_t_dest_iterator>::type		sDestSelect;

	::std::cout << pszDest << " = " << pszOperandA << " - " << pszOperandB << " ";

	rIterDest = rOperandA - rOperandB;

	TestBTreeKeySortIterArithmeticOperators (pContainer, pReference, rIterDest, rOperandA, rOperandB, nExpectedOffset, sDestSelect);

	::std::cout << ::std::endl;
}

template<class _t_container, class _t_reference>
void TestBTreeKeySortIterArithmeticOperators (_t_container *pContainer, _t_reference *pReference, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::value_type					data_t;
	typedef typename _t_container::size_type					size_type;

	typedef typename _t_reference::size_type					size_type_ref;

	typedef typename _t_container::iterator						iter_t;
	typedef typename _t_container::const_iterator				citer_t;
	typedef typename _t_container::reverse_iterator				riter_t;
	typedef typename _t_container::const_reverse_iterator		criter_t;

	const uint32_t			nRepetitions = 1;
	uint32_t				i;
	uint32_t				nSeed = 0;
	iter_t					sIter;
	citer_t					sCIter;
	riter_t					sRIter;
	criter_t				sCRIter;
	iter_t					sIterBegin;
	citer_t					sCIterBegin;
	riter_t					sRIterBegin;
	criter_t				sCRIterBegin;
	iter_t					sIterEnd;
	citer_t					sCIterEnd;
	riter_t					sRIterEnd;
	criter_t				sCRIterEnd;
	size_type				nOffset = 0;
	int						nOffsetInt;
	size_type				nResult;
	size_type_ref			nNumEntriesRef = (size_type_ref) nNumEntries;
	
	::std::cout << "basic keysort iterator arithmetic operators test" << ::std::endl;

	associative_container_add_primitive (pReference, nNumEntriesRef, nSeed, BTREETEST_KEY_GENERATION_RANDOM);

	TransferKeySortIterRefToTestContainer (pContainer, pReference);

	get_begin (pContainer, sIterBegin);
	get_begin (pContainer, sCIterBegin);
	get_begin (pContainer, sRIterBegin);
	get_begin (pContainer, sCRIterBegin);

	get_end (pContainer, sIterEnd);
	get_end (pContainer, sCIterEnd);
	get_end (pContainer, sRIterEnd);
	get_end (pContainer, sCRIterEnd);

	for (i = 0; i < nRepetitions; i++)
	{
		nOffset = generate_rand32 ();
		nOffset = nOffset % pContainer->size ();

		nOffsetInt = (int) nOffset;

		TestBTreeKeySortIterArithmeticOperatorAdd (pContainer, pReference, "iterator", "iterator", "sizetype", sIter, sIterBegin, nOffset, nOffset);
		TestBTreeKeySortIterArithmeticOperatorAdd (pContainer, pReference, "const_iterator", "const_iterator", "sizetype", sCIter, sCIterBegin, nOffset, nOffset);

		TestBTreeKeySortIterArithmeticOperatorAdd (pContainer, pReference, "iterator", "sizetype", "iterator", sIter, nOffset, sIterBegin, nOffset);
		TestBTreeKeySortIterArithmeticOperatorAdd (pContainer, pReference, "const_iterator", "sizetype", "const_iterator", sCIter, nOffset, sCIterBegin, nOffset);

		TestBTreeKeySortIterArithmeticOperatorAdd (pContainer, pReference, "iterator", "iterator", "int", sIter, sIterBegin, nOffsetInt, nOffset);
		TestBTreeKeySortIterArithmeticOperatorAdd (pContainer, pReference, "const_iterator", "const_iterator", "int", sCIter, sCIterBegin, nOffsetInt, nOffset);

		TestBTreeKeySortIterArithmeticOperatorAdd (pContainer, pReference, "iterator", "int", "iterator", sIter, nOffsetInt, sIterBegin, nOffset);
		TestBTreeKeySortIterArithmeticOperatorAdd (pContainer, pReference, "const_iterator", "int", "const_iterator", sCIter, nOffsetInt, sCIterBegin, nOffset);

		TestBTreeKeySortIterArithmeticOperatorSub (pContainer, pReference, "iterator", "iterator", "sizetype", sIter, sIterEnd, nOffset, pContainer->size () - nOffset);
		TestBTreeKeySortIterArithmeticOperatorSub (pContainer, pReference, "const_iterator", "const_iterator", "sizetype", sCIter, sCIterEnd, nOffset, pContainer->size () - nOffset);

		TestBTreeKeySortIterArithmeticOperatorSub (pContainer, pReference, "iterator", "iterator", "int", sIter, sIterEnd, nOffsetInt, pContainer->size () - nOffset);
		TestBTreeKeySortIterArithmeticOperatorSub (pContainer, pReference, "const_iterator", "const_iterator", "int", sCIter, sCIterEnd, nOffsetInt, pContainer->size () - nOffset);

		TestBTreeKeySortIterArithmeticOperatorSub (pContainer, pReference, "sizetype", "iterator", "iterator", nResult, sIterBegin, sIterEnd, 0 - pContainer->size ());
		TestBTreeKeySortIterArithmeticOperatorSub (pContainer, pReference, "sizetype", "const_iterator", "const_iterator", nResult, sCIterBegin, sCIterEnd, 0 - pContainer->size ());

		TestBTreeKeySortIterArithmeticOperatorAdd (pContainer, pReference, "reverse_iterator", "reverse_iterator", "sizetype", sRIter, sRIterBegin, nOffset, pContainer->size () - nOffset - 1);
		TestBTreeKeySortIterArithmeticOperatorAdd (pContainer, pReference, "reverse_const_iterator", "reverse_const_iterator", "sizetype", sCRIter, sCRIterBegin, nOffset, pContainer->size () - nOffset - 1);

		TestBTreeKeySortIterArithmeticOperatorAdd (pContainer, pReference, "reverse_iterator", "reverse_iterator", "int", sRIter, sRIterBegin, nOffsetInt, pContainer->size () - nOffset - 1);
		TestBTreeKeySortIterArithmeticOperatorAdd (pContainer, pReference, "reverse_const_iterator", "reverse_const_iterator", "int", sCRIter, sCRIterBegin, nOffsetInt, pContainer->size () - nOffset - 1);

		TestBTreeKeySortIterArithmeticOperatorSub (pContainer, pReference, "reverse_iterator", "reverse_iterator", "sizetype", sRIter, sRIterEnd, nOffset, nOffset - 1);
		TestBTreeKeySortIterArithmeticOperatorSub (pContainer, pReference, "reverse_const_iterator", "reverse_const_iterator", "sizetype", sCRIter, sCRIterEnd, nOffset, nOffset - 1);

		TestBTreeKeySortIterArithmeticOperatorSub (pContainer, pReference, "reverse_iterator", "reverse_iterator", "int", sRIter, sRIterEnd, nOffsetInt, nOffset - 1);
		TestBTreeKeySortIterArithmeticOperatorSub (pContainer, pReference, "reverse_const_iterator", "reverse_const_iterator", "int", sCRIter, sCRIterEnd, nOffsetInt, nOffset - 1);
	}
}

template<class _t_lhiter, class _t_rhiter>
void TestBTreeKeySortIterCompareOperators (_t_lhiter sLHIterBegin, _t_lhiter sLHIterEnd, _t_rhiter sRHIterBegin, _t_rhiter sRHIterEnd)
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
void TestBTreeKeySortIterCompareOperators (_t_container *pContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::iterator						iter_t;
	typedef typename _t_container::const_iterator				citer_t;
	typedef typename _t_container::reverse_iterator				riter_t;
	typedef typename _t_container::const_reverse_iterator		criter_t;

	uint32_t				nSeed = 0;
	
	::std::cout << "basic array iterator compare operators test" << ::std::endl;

	associative_container_add_primitive (pContainer, nNumEntries, nSeed, BTREETEST_KEY_GENERATION_RANDOM);
	
	::std::cout << "iterator versus iterator";
	
	TestBTreeKeySortIterCompareOperators (pContainer->begin (), pContainer->end (), pContainer->begin (), pContainer->end ());

	::std::cout << "const_iterator versus const_iterator";
	
	TestBTreeKeySortIterCompareOperators (pContainer->cbegin (), pContainer->cend (), pContainer->cbegin (), pContainer->cend ());

	::std::cout << "reverse_iterator versus reverse_iterator";

	TestBTreeKeySortIterCompareOperators (pContainer->rbegin (), pContainer->rend (), pContainer->rbegin (), pContainer->rend ());

	::std::cout << "const_reverse_iterator versus const_reverse_iterator";

	TestBTreeKeySortIterCompareOperators (pContainer->crbegin (), pContainer->crend (), pContainer->crbegin (), pContainer->crend ());
}

template<class _t_container>
void TestBTreeKeySortIterConstSwap (_t_container *pContainer, typename _t_container::size_type nNumEntries)
{
	typedef typename _t_container::iterator							iter_t;
	typedef typename _t_container::const_iterator					citer_t;
	typedef typename _t_container::reverse_iterator					riter_t;
	typedef typename _t_container::const_reverse_iterator			criter_t;

	uint32_t				nSeed = 0;
	iter_t					sIter;
	iter_t					sIter2;
	citer_t					sCIter;
	citer_t					sCIter2;
	riter_t					sRIter;
	riter_t					sRIter2;
	criter_t				sCRIter;
	criter_t				sCRIter2;
	
	::std::cout << "basic keysort const iterator swap test" << ::std::endl;

	associative_container_add_primitive (pContainer, nNumEntries, nSeed, BTREETEST_KEY_GENERATION_RANDOM);

	get_begin (pContainer, sIter);
	get_begin (pContainer, sCIter);
	get_begin (pContainer, sRIter);
	get_begin (pContainer, sCRIter);
	
	get_end (pContainer, sIter2);
	get_end (pContainer, sCIter2);
	get_end (pContainer, sRIter2);
	get_end (pContainer, sCRIter2);
	
	::std::cout << "iterator ";

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

	::std::cout << "const_iterator ";

	if (sCIter >= sCIter2)
	{
		exit (-1);
	}

	::std::cout << "swap ";

	sCIter.swap (sCIter2);

	if (sCIter <= sCIter2)
	{
		exit (-1);
	}

	::std::cout << "swap ";

	sCIter2.swap (sCIter);
	
	if (sCIter >= sCIter2)
	{
		exit (-1);
	}

	::std::cout << ::std::endl;

	::std::cout << "reverse_iterator ";

	if (sRIter >= sRIter2)
	{
		exit (-1);
	}

	::std::cout << "swap ";

	sRIter.swap (sRIter2);

	if (sRIter <= sRIter2)
	{
		exit (-1);
	}

	::std::cout << "swap ";

	sRIter2.swap (sRIter);
	
	if (sRIter >= sRIter2)
	{
		exit (-1);
	}

	::std::cout << ::std::endl;
	
	::std::cout << "const_reverse_iterator ";

	if (sCRIter >= sCRIter2)
	{
		exit (-1);
	}

	::std::cout << "swap ";

	sCRIter.swap (sCRIter2);

	if (sCRIter <= sCRIter2)
	{
		exit (-1);
	}

	::std::cout << "swap ";

	sCRIter2.swap (sCRIter);
	
	if (sCRIter >= sCRIter2)
	{
		exit (-1);
	}

	::std::cout << ::std::endl;
}

template<class _t_sizetype>
void TestBTreeKeySortIter (uint32_t nTest, uint32_t nNodeSize, uint32_t nPageSize)
{
	typedef	keySortEntry_t																		data_t;

	typedef typename ::std::multimap<const uint32_t, keySortMap_t, multiMapMemCmp<uint32_t> >	reference_memcmp_t;
	typedef CBTreeAssociativeIf<data_t, uint32_t, _t_sizetype>									container_t;
	typedef typename ::std::vector<container_t *>												test_vector_t;

	test_vector_t																	asContainers;
	reference_memcmp_t																*pClRefMemCmpKeySort;
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

	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerRAM6565_n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM6555_n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM5555_n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerRAM5554_n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerRAM5454_n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM5444_n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint16_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM4444_n;

	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerRAM6565_2n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM6555_2n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM5555_2n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerRAM5554_2n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerRAM5454_2n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM5444_2n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint16_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM4444_2n;

	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerRAM6565_4n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM6555_4n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM5555_4n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerRAM5554_4n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerRAM5454_4n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM5444_4n;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint16_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM4444_4n;

	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerFile6565min;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile6555min;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile5555min;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerFile5554min;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerFile5454min;

	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerFile6565default;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile6555default;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile5555default;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerFile5554default;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerFile5454default;

	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerFile6565large;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile6555large;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile5555large;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerFile5554large;
	CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerFile5454large;

	::std::cout << "b-tree keysort iterator test bench selected" << ::std::endl;

	m_pContainerRAM6565_n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> > (sRAMprop6565, &sCacheDescPageSize, nNodeSize);
	m_pContainerRAM6555_n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> > (sRAMprop6555, &sCacheDescPageSize, nNodeSize);
	m_pContainerRAM5555_n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> > (sRAMprop5555, &sCacheDescPageSize, nNodeSize);
	m_pContainerRAM5554_n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> > (sRAMprop5554, &sCacheDescPageSize, nNodeSize);
	m_pContainerRAM5454_n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> > (sRAMprop5454, &sCacheDescPageSize, nNodeSize);
	m_pContainerRAM5444_n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint16_t, uint16_t> > (sRAMprop5444, &sCacheDescPageSize, nNodeSize);
	m_pContainerRAM4444_n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint16_t, uint16_t, uint16_t, uint16_t> > (sRAMprop4444, &sCacheDescPageSize, nNodeSize);
	m_pContainerRAM6565_2n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> > (sRAMprop6565, &sCacheDescPageSize, nNodeSize * 2);
	m_pContainerRAM6555_2n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> > (sRAMprop6555, &sCacheDescPageSize, nNodeSize * 2);
	m_pContainerRAM5555_2n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> > (sRAMprop5555, &sCacheDescPageSize, nNodeSize * 2);
	m_pContainerRAM5554_2n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> > (sRAMprop5554, &sCacheDescPageSize, nNodeSize * 2);
	m_pContainerRAM5454_2n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> > (sRAMprop5454, &sCacheDescPageSize, nNodeSize * 2);
	m_pContainerRAM5444_2n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint16_t, uint16_t> > (sRAMprop5444, &sCacheDescPageSize, nNodeSize * 2);
	m_pContainerRAM4444_2n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint16_t, uint16_t, uint16_t, uint16_t> > (sRAMprop4444, &sCacheDescPageSize, nNodeSize * 2);
	m_pContainerRAM6565_4n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> > (sRAMprop6565, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerRAM6555_4n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> > (sRAMprop6555, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerRAM5555_4n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> > (sRAMprop5555, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerRAM5554_4n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> > (sRAMprop5554, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerRAM5454_4n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> > (sRAMprop5454, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerRAM5444_4n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint32_t, uint16_t, uint16_t, uint16_t> > (sRAMprop5444, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerRAM4444_4n = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesRAM <_t_sizetype, uint16_t, uint16_t, uint16_t, uint16_t> > (sRAMprop4444, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerFile6565min = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> > (sFilePropertiesMin6565, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerFile6555min = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesMin6555, &sCacheDescPageSize, nNodeSize * 8);
	m_pContainerFile5555min = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesMin5555, &sCacheDescMin, nNodeSize * 7);
	m_pContainerFile5554min = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> > (sFilePropertiesMin5554, &sCacheDescNearestBigger, nNodeSize * 6);
	m_pContainerFile5454min = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> > (sFilePropertiesMin5454, &sCacheDescLarge, nNodeSize * 5);

	m_pContainerFile6565default = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> > (sFilePropertiesDefault6565, &sCacheDescNearestBigger, nNodeSize * 3);
	m_pContainerFile6555default = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesDefault6555, &sCacheDescNearestBigger, nNodeSize * 2);
	m_pContainerFile5555default = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesDefault5555, &sCacheDescLarge, nNodeSize);
	m_pContainerFile5554default = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> > (sFilePropertiesDefault5554, &sCacheDescPageSize, nNodeSize * 8);
	m_pContainerFile5454default = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> > (sFilePropertiesDefault5454, &sCacheDescMin, nNodeSize * 7);

	m_pContainerFile6565large = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint64_t, uint32_t> > (sFilePropertiesLarge6565, &sCacheDescPageSize, nNodeSize * 5);
	m_pContainerFile6555large = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint64_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesLarge6555, &sCacheDescPageSize, nNodeSize * 4);
	m_pContainerFile5555large = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesDefault5555, &sCacheDescMin, nNodeSize * 3);
	m_pContainerFile5554large = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint32_t, uint32_t, uint16_t> > (sFilePropertiesDefault5554, &sCacheDescNearestBigger, nNodeSize * 2);
	m_pContainerFile5454large = new CBTreeKeySort<data_t, uint32_t, CBTreeIOpropertiesFile <_t_sizetype, uint32_t, uint16_t, uint32_t, uint16_t> > (sFilePropertiesDefault5454, &sCacheDescLarge, nNodeSize);

	if (NULL == m_pContainerRAM6565_n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM6565_n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM6555_n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM6555_n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5555_n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5555_n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5554_n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5554_n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5454_n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5454_n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5444_n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5444_n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM4444_n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM4444_n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM6565_2n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM6565_2n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM6555_2n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM6555_2n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5555_2n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5555_2n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5554_2n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5554_2n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5454_2n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5454_2n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5444_2n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5444_2n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM4444_2n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM4444_2n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM6565_4n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM6565_4n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM6555_4n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM6555_4n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5555_4n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5555_4n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5554_4n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5554_4n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5454_4n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5454_4n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM5444_4n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM5444_4n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerRAM4444_4n) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerRAM4444_4n)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile6565min) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile6565min)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile6555min) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile6555min)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5555min) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5555min)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5554min) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5554min)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5454min) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5454min)" << ::std::endl; exit (-1);}

	if (NULL == m_pContainerFile6565default) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile6565default)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile6555default) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile6555default)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5555default) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5555default)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5554default) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5554default)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5454default) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5454default)" << ::std::endl; exit (-1);}

	if (NULL == m_pContainerFile6565large) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile6565large)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile6555large) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile6555large)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5555large) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5555large)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5554large) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5554large)" << ::std::endl; exit (-1);}
	if (NULL == m_pContainerFile5454large) {::std::cerr << "TestBTreeKeySortIter<_t_sizetype> (uint32_t, uint32_t, uint32_t): ERROR: insufficient memory! (m_pContainerFile5454large)" << ::std::endl; exit (-1);}

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

	pClRefMemCmpKeySort = new reference_memcmp_t ();

	if (pClRefMemCmpKeySort == NULL)
	{
		::std::cerr << "ERROR: Insufficient memory!" << ::std::endl;

		exit (-1);
	}

	for (i = 0; i < asContainers.size (); i++)
	{
		pContainer = asContainers[i];

		switch (nTest)
		{
		case BTREETEST_KEY_SORT_ITER_CONST_ASCEND	:
			{
				TestBTreeKeySortConstIterBasic (pContainer, pClRefMemCmpKeySort, false, 128, 1, 0);

				break;
			}

		case BTREETEST_KEY_SORT_ITER_CONST_ASCEND_SMALL	:
			{
				TestBTreeKeySortConstIterBasic (pContainer, pClRefMemCmpKeySort, false, 16, 1, 0);

				break;
			}

		case BTREETEST_KEY_SORT_ITER_CONST_DESCEND	:
			{
				TestBTreeKeySortConstIterBasic (pContainer, pClRefMemCmpKeySort, true, 128, 1, 0);

				break;
			}

		case BTREETEST_KEY_SORT_ITER_CONST_DESCEND_SMALL	:
			{
				TestBTreeKeySortConstIterBasic (pContainer, pClRefMemCmpKeySort, true, 16, 1, 0);

				break;
			}

		case BTREETEST_KEY_SORT_ITER_CONST_CMPLX_ASCEND	:
			{
				TestBTreeKeySortConstIterBasic (pContainer, pClRefMemCmpKeySort, false, 128, 5, 3);

				break;
			}

		case BTREETEST_KEY_SORT_ITER_CONST_CMPLX_ASCEND_SMALL	:
			{
				TestBTreeKeySortConstIterBasic (pContainer, pClRefMemCmpKeySort, false, 16, 2, 3);

				break;
			}

		case BTREETEST_KEY_SORT_ITER_CONST_CMPLX_DESCEND	:
			{
				TestBTreeKeySortConstIterBasic (pContainer, pClRefMemCmpKeySort, true, 128, 4, 3);

				break;
			}

		case BTREETEST_KEY_SORT_ITER_CONST_CMPLX_DESCEND_SMALL	:
			{
				TestBTreeKeySortConstIterBasic (pContainer, pClRefMemCmpKeySort, true, 16, 2, 3);

				break;
			}

		case BTREETEST_KEY_SORT_ITER_CONST_NODESIZE_VS_STEPSIZE_ASCEND		:
			{
				TestBTreeKeySortConstIterNodeSizeVsStepSize (pContainer, pClRefMemCmpKeySort, false, 512, 3, 257, 1, 513);
				
				break;
			}

		case BTREETEST_KEY_SORT_ITER_CONST_NODESIZE_VS_STEPSIZE_DESCEND	:
			{
				TestBTreeKeySortConstIterNodeSizeVsStepSize (pContainer, pClRefMemCmpKeySort, true, 512, 3, 257, 1, 513);

				break;
			}


		case BTREETEST_KEY_SORT_ITER_SUBSCRIPTOR	:
			{
				container_t		*pCntContainer = asContainers[(i + 1) % asContainers.size ()];

				TestBTreeKeySortIterSubScriptor<_t_sizetype> (pContainer, pCntContainer, 128);

				break;
			}

		case BTREETEST_KEY_SORT_ITER_SUBSCRIPTOR_INT	:
			{
				container_t		*pCntContainer = asContainers[(i + 1) % asContainers.size ()];

				TestBTreeKeySortIterSubScriptor<int> (pContainer, pCntContainer, 128);

				break;
			}

		case BTREETEST_KEY_SORT_ITER_COMPOUND	:
			{
				container_t		*pCntContainer = asContainers[(i + 1) % asContainers.size ()];

				TestBTreeKeySortIterCompound<_t_sizetype> (pContainer, pCntContainer, 128);

				break;
			}

		case BTREETEST_KEY_SORT_ITER_COMPOUND_INT	:
			{
				container_t		*pCntContainer = asContainers[(i + 1) % asContainers.size ()];

				TestBTreeKeySortIterCompound<int> (pContainer, pCntContainer, 128);

				break;
			}

		case BTREETEST_KEY_SORT_ITER_COMPOUND_ITER	:
			{
				container_t		*pCntContainer = asContainers[(i + 1) % asContainers.size ()];

				TestBTreeKeySortIterCompoundIter (pContainer, pCntContainer, 128);

				break;
			}

		case BTREETEST_KEY_SORT_ITER_ARITHMETIC_OPERATORS	:
			{
				TestBTreeKeySortIterArithmeticOperators (pContainer, pClRefMemCmpKeySort, 64);

				break;
			}

		case BTREETEST_KEY_SORT_ITER_COMPARE_OPERATORS	:
			{
				TestBTreeKeySortIterCompareOperators (pContainer, 16);

				break;
			}

		case BTREETEST_KEY_SORT_ITER_CONST_SWAP	:
			{
				TestBTreeKeySortIterConstSwap (pContainer, 16);

				break;
			}

		default									:
			{
				::std::cerr << "ERROR: TestBTreeKeySortIter: Unknown test or test not specified!" << ::std::endl;

				exit (-1);

				break;
			}
		}

		pContainer->clear ();

		pClRefMemCmpKeySort->clear ();
	}

	for (i = 0; i < asContainers.size (); i++)
	{
		pContainer = asContainers[i];

		delete pContainer;
	}

	delete pClRefMemCmpKeySort;
}
