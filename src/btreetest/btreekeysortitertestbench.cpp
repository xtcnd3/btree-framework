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

template <class _t_container, class _t_reference>
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

template <class _t_container, class _t_reference, class _t_iterator, class _t_ref_iterator, class _t_sizetype>
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

template <class _t_container, class _t_reference>
void TestBTreeKeySortConstIterBasic (_t_container *pContainer, _t_reference *pReference, bool bDescend, uint32_t nNumEntries, uint32_t nStepSize, uint32_t nTurnArounds, bool bFillArray = true)
{
	typedef typename _t_container::iterator						iter_t;
	typedef typename _t_container::const_iterator				citer_t;
	typedef typename _t_container::reverse_iterator				riter_t;
	typedef typename _t_container::const_reverse_iterator		criter_t;

	typedef typename _t_reference::const_iterator				citer_ref_t;

	typedef typename _t_container::size_type					size_type;

	typedef typename _t_container::value_type					data_t;
	typedef typename _t_reference::value_type					data_ref_t;

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
	uint32_t				nThisStepSize;

	cout << "basic read-only iterator test ";

	if (bDescend == false)
	{
		cout << "ascends ";
	}
	else
	{
		cout << "descends ";
	}
	
	cout << nNumEntries << " entries ";
	cout << "with step size " << nStepSize;
	cout << " and turns around " << nTurnArounds << " times." << endl;

	if (bFillArray)
	{
		associative_container_add_primitive (pReference, nNumEntries, nSeed, BTREETEST_KEY_GENERATION_RANDOM);

		TransferKeySortIterRefToTestContainer (pContainer, pReference);
	}

	for (nTurn = 0; nTurn <= nTurnArounds; nTurn++)
	{
		cout << "turn: " << nTurn << endl;

		bool	bForward = (((nTurn & 0x1) == 0x0) != bDescend);

		i = bForward ? 0ULL : pContainer->size ();

		sIter = bForward ? pContainer->begin () : pContainer->end ();
		sCIter = bForward ? pContainer->cbegin () : pContainer->cend ();
		sRIter = bForward ? pContainer->rbegin () : pContainer->rend ();
		sCRIter = bForward ? pContainer->crbegin () : pContainer->crend ();

		nIter = bForward ? 0 : pReference->size ();
		nRIter = bForward ? pReference->size () : 0;
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

			cout << i << " " << "\r" << flush;

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

		cout << endl;
	}
}

template <class _t_container, class _t_reference>
void TestBTreeKeySortConstIterNodeSizeVsStepSize (_t_container *pContainer, _t_reference *pReference, bool bDescend, typename _t_container::datalayerproperties_t &rDataLayerProperties, bayerTreeCacheDescription_t &sCacheDesc, uint32_t nNumEntries, typename _t_container::subnodeiter_t nFromNodeSize, typename _t_container::subnodeiter_t nToNodeSize, uint32_t nFromStepSize, uint32_t nToStepSize)
{
	typedef typename _t_container::value_type			data_t;
	typedef typename _t_container::subnodeiter_t		subnodeiter_t;

	typedef typename _t_reference::const_iterator		citer_ref_t;

	typedef ::std::vector<uint32_t>::iterator			vec_iter_t;
	typedef ::std::vector<uint32_t>						vector_t;

	subnodeiter_t		nNodeSize;
	uint32_t			nStepSize;
	_t_container		*pTestContainer;
	uint32_t			nSeed;
	vector_t			sPrimeNodeSizeVector;
	vec_iter_t			sVecIter;
	uint32_t			nPrime;
	citer_ref_t			sCIterRefBegin;
	citer_ref_t			sCIterRefEnd;

	nSeed = 0;

	associative_container_add_primitive (pReference, nNumEntries, nSeed, BTREETEST_KEY_GENERATION_RANDOM);
		
	for (nNodeSize = nFromNodeSize; nNodeSize < nToNodeSize; nNodeSize++)
	{
		sPrimeNodeSizeVector.push_back (nNodeSize);
	}

	while (sPrimeNodeSizeVector.size () > 0)
	{
		nNodeSize = sPrimeNodeSizeVector.at (0);

		pTestContainer = new _t_container (rDataLayerProperties, &sCacheDesc, nNodeSize);

		if (pTestContainer == NULL)
		{
			::std::cerr << "ERROR: insufficient memory!" << ::std::endl;

			exit (-1);
		}

		TransferKeySortIterRefToTestContainer (pTestContainer, pReference);
		
		for (nStepSize = nFromStepSize; nStepSize < nToStepSize; nStepSize <<= 1)
		{
			TestBTreeKeySortConstIterBasic (pTestContainer, pReference, bDescend, nNumEntries, nStepSize, 0, false);
		}

		nPrime = sPrimeNodeSizeVector.at (0);

		for (sVecIter = sPrimeNodeSizeVector.end (); sVecIter != sPrimeNodeSizeVector.begin (); )
		{
			sVecIter--;

			if (((*sVecIter) % nPrime) == 0)
			{
				sPrimeNodeSizeVector.erase (sVecIter);

				sVecIter = sPrimeNodeSizeVector.end ();
			}
		}
		
		delete pTestContainer;
	}
}

template <class _t_subscripttype, class _t_container, class _t_iterator>
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

		for (rI = 0; rI < pContainer->size (); rI++)
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

template <class _t_subscripttype, class _t_container>
void TestBTreeKeySortIterSubScriptor (_t_container *pContainer, uint32_t nNumEntries)
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
	_t_container			sContainerRef (*pContainer);
	
	cout << "basic keysort iterator sub-scription test" << endl;
	
#if defined(__GNUC__) || defined(__GNUG__)

	int		nStatus;
	char	*pszTypeid = abi::__cxa_demangle (typeid (_t_subscripttype).name (), 0, 0, &nStatus);

	if (pszTypeid != NULL)
	{
		cout << "_t_sizetype is set to: " << pszTypeid << endl;
		
		free ((void *) pszTypeid);
	}
	else
	{
		cout << "_t_sizetype is set to: " << typeid (_t_subscripttype).name () << endl;
	}
	
#else

	cout << "_t_sizetype is set to: " << typeid (_t_subscripttype).name () << endl;
	
#endif

	associative_container_add_primitive (pContainer, nNumEntries, nSeed, BTREETEST_KEY_GENERATION_RANDOM);

	TestBTreeKeySortIterSubScriptor (pContainer, &sContainerRef, sIter, i, true);
	TestBTreeKeySortIterSubScriptor (pContainer, &sContainerRef, sCIter, i, true);
	TestBTreeKeySortIterSubScriptor (pContainer, &sContainerRef, sRIter, i, true);
	TestBTreeKeySortIterSubScriptor (pContainer, &sContainerRef, sCRIter, i, true);

	TestBTreeKeySortIterSubScriptor (pContainer, &sContainerRef, sIter, i, false);
	TestBTreeKeySortIterSubScriptor (pContainer, &sContainerRef, sCIter, i, false);
	TestBTreeKeySortIterSubScriptor (pContainer, &sContainerRef, sRIter, i, false);
	TestBTreeKeySortIterSubScriptor (pContainer, &sContainerRef, sCRIter, i, false);
}

template <class _t_offsettype, class _t_container, class _t_iterator>
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

		for (rI = 0; rI < pContainer->size (); rI++)
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

template <class _t_offsettype, class _t_container>
void TestBTreeKeySortIterCompound (_t_container *pContainer, uint32_t nNumEntries)
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
	_t_container			sContainerRef (*pContainer);
	::std::string			strType;
	
	cout << "basic keysort iterator compound operator test" << endl;

	get_typename (i, strType);
	
	cout << "_t_sizetype is set to: " << strType << endl;
	
	associative_container_add_primitive (pContainer, nNumEntries, nSeed, BTREETEST_KEY_GENERATION_RANDOM);

	TestBTreeKeySortIterCompound (pContainer, &sContainerRef, sIter, i, true);
	TestBTreeKeySortIterCompound (pContainer, &sContainerRef, sCIter, i, true);
	TestBTreeKeySortIterCompound (pContainer, &sContainerRef, sRIter, i, true);
	TestBTreeKeySortIterCompound (pContainer, &sContainerRef, sCRIter, i, true);

	TestBTreeKeySortIterCompound (pContainer, &sContainerRef, sIter, i, false);
	TestBTreeKeySortIterCompound (pContainer, &sContainerRef, sCIter, i, false);
	TestBTreeKeySortIterCompound (pContainer, &sContainerRef, sRIter, i, false);
	TestBTreeKeySortIterCompound (pContainer, &sContainerRef, sCRIter, i, false);
}

template <class _t_container>
void TestBTreeKeySortIterCompoundIter (_t_container *pContainer, uint32_t nNumEntries)
{
	typedef typename _t_container::value_type				data_t;

	typedef typename _t_container::const_iterator			citer_t;
	
	uint32_t				nSeed = 0;
	citer_t					sCIter;
	data_t					sData;
	citer_t					sCI;
	_t_container			sContainerRef (*pContainer);
	
	cout << "basic keysort iterator compound operator (iterator) test" << endl;
	
	associative_container_add_primitive (pContainer, nNumEntries, nSeed, BTREETEST_KEY_GENERATION_RANDOM);
	
	sCIter = pContainer->cbegin ();
	
	cout << "forward iterator ";

	for (sCI = pContainer->cbegin (); sCI != pContainer->cend (); sCI++)
	{
		sCIter += sCI;
		{
			sData = *sCIter;
			
			sContainerRef.insert (sData);
		}
		sCIter -= sCI;
	}

	cout << "testing ";

	if (sContainerRef != *pContainer)
	{
		cerr << endl;
		cerr << "keysort mismatch (forward iterator compound operation)!" << endl;
		
		exit (-1);
	}

	cout << "passed" << endl;

	sContainerRef.clear ();

	sCIter = pContainer->cend ();
	
	cout << "reverse iterator ";

	for (sCI = pContainer->cend (); sCI != pContainer->cbegin (); )
	{
		sCI--;

		sCI -= pContainer->size ();
		{
			sCIter += sCI;
			{
				sData = *sCIter;
				
				sContainerRef.insert (sData);
			}
			sCIter -= sCI;
		}
		sCI += pContainer->size ();
	}

	cout << "testing ";

	if (sContainerRef != *pContainer)
	{
		cerr << endl;
		cerr << "keysort mismatch (reverse iterator compound operation)!" << endl;
		
		exit (-1);
	}

	cout << "passed" << endl;
}

template <class _t_container, class _t_reference, class _t_dest_iterator, class _t_operand_a, class _t_operand_b>
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

template <class _t_container, class _t_reference, class _t_dest_iterator, class _t_operand_a, class _t_operand_b>
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

template <class _t_container, class _t_reference, class _t_dest_iterator, class _t_operand_a, class _t_operand_b>
void TestBTreeKeySortIterArithmeticOperatorAdd (_t_container *pContainer, _t_reference *pReference, const char *pszDest, const char *pszOperandA, const char *pszOperandB, _t_dest_iterator &rIterDest, _t_operand_a &rOperandA, _t_operand_b &rOperandB, const typename _t_container::size_type nExpectedOffset)
{
	typename ::std::is_integral<_t_dest_iterator>::type		sDestSelect;

	::std::cout << pszDest << " = " << pszOperandA << " + " << pszOperandB << " ";

	rIterDest = rOperandA + rOperandB;

	TestBTreeKeySortIterArithmeticOperators (pContainer, pReference, rIterDest, rOperandA, rOperandB, nExpectedOffset, sDestSelect);

	::std::cout << ::std::endl;
}

template <class _t_container, class _t_reference, class _t_dest_iterator, class _t_operand_a, class _t_operand_b>
void TestBTreeKeySortIterArithmeticOperatorSub (_t_container *pContainer, _t_reference *pReference, const char *pszDest, const char *pszOperandA, const char *pszOperandB, _t_dest_iterator &rIterDest, _t_operand_a &rOperandA, _t_operand_b &rOperandB, const typename _t_container::size_type nExpectedOffset)
{
	typename ::std::is_integral<_t_dest_iterator>::type		sDestSelect;

	::std::cout << pszDest << " = " << pszOperandA << " - " << pszOperandB << " ";

	rIterDest = rOperandA - rOperandB;

	TestBTreeKeySortIterArithmeticOperators (pContainer, pReference, rIterDest, rOperandA, rOperandB, nExpectedOffset, sDestSelect);

	::std::cout << ::std::endl;
}

template <class _t_container, class _t_reference>
void TestBTreeKeySortIterArithmeticOperators (_t_container *pContainer, _t_reference *pReference, uint32_t nNumEntries)
{
	typedef typename _t_container::value_type					data_t;
	typedef typename _t_container::size_type					sizetype;

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
	sizetype				nOffset = 0;
	int						nOffsetInt;
	sizetype				nResult;
	
	cout << "basic keysort iterator arithmetic operators test" << endl;

	associative_container_add_primitive (pReference, nNumEntries, nSeed, BTREETEST_KEY_GENERATION_RANDOM);

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

template <class _t_lhiter, class _t_rhiter>
void TestBTreeKeySortIterCompareOperators (_t_lhiter sLHIterBegin, _t_lhiter sLHIterEnd, _t_rhiter sRHIterBegin, _t_rhiter sRHIterEnd)
{
	_t_lhiter		sLhs;
	_t_rhiter		sRhs;

	{
		cout << " <";

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
		cout << " <=";

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
		cout << " >";

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
		cout << " >=";

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
		cout << " ==";

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
		cout << " !=";

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

	cout << endl;
}

template <class _t_container>
void TestBTreeKeySortIterCompareOperators (_t_container *pContainer, uint32_t nNumEntries)
{
	typedef typename _t_container::iterator						iter_t;
	typedef typename _t_container::const_iterator				citer_t;
	typedef typename _t_container::reverse_iterator				riter_t;
	typedef typename _t_container::const_reverse_iterator		criter_t;

	uint32_t				nSeed = 0;
	
	cout << "basic array iterator compare operators test" << endl;

	associative_container_add_primitive (pContainer, nNumEntries, nSeed, BTREETEST_KEY_GENERATION_RANDOM);
	
	cout << "iterator versus iterator";
	
	TestBTreeKeySortIterCompareOperators (pContainer->begin (), pContainer->end (), pContainer->begin (), pContainer->end ());

	cout << "const_iterator versus const_iterator";
	
	TestBTreeKeySortIterCompareOperators (pContainer->cbegin (), pContainer->cend (), pContainer->cbegin (), pContainer->cend ());

	cout << "reverse_iterator versus reverse_iterator";

	TestBTreeKeySortIterCompareOperators (pContainer->rbegin (), pContainer->rend (), pContainer->rbegin (), pContainer->rend ());

	cout << "const_reverse_iterator versus const_reverse_iterator";

	TestBTreeKeySortIterCompareOperators (pContainer->crbegin (), pContainer->crend (), pContainer->crbegin (), pContainer->crend ());
}

template <class _t_container>
void TestBTreeKeySortIterConstSwap (_t_container *pContainer, uint32_t nNumEntries)
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
	
	cout << "basic keysort const iterator swap test" << endl;

	associative_container_add_primitive (pContainer, nNumEntries, nSeed, BTREETEST_KEY_GENERATION_RANDOM);

	get_begin (pContainer, sIter);
	get_begin (pContainer, sCIter);
	get_begin (pContainer, sRIter);
	get_begin (pContainer, sCRIter);
	
	get_end (pContainer, sIter2);
	get_end (pContainer, sCIter2);
	get_end (pContainer, sRIter2);
	get_end (pContainer, sCRIter2);
	
	cout << "iterator ";

	if (sIter >= sIter2)
	{
		exit (-1);
	}

	cout << "swap ";

	sIter.swap (sIter2);

	if (sIter <= sIter2)
	{
		exit (-1);
	}

	cout << "swap ";

	sIter2.swap (sIter);
	
	if (sIter >= sIter2)
	{
		exit (-1);
	}

	cout << endl;

	cout << "const_iterator ";

	if (sCIter >= sCIter2)
	{
		exit (-1);
	}

	cout << "swap ";

	sCIter.swap (sCIter2);

	if (sCIter <= sCIter2)
	{
		exit (-1);
	}

	cout << "swap ";

	sCIter2.swap (sCIter);
	
	if (sCIter >= sCIter2)
	{
		exit (-1);
	}

	cout << endl;

	cout << "reverse_iterator ";

	if (sRIter >= sRIter2)
	{
		exit (-1);
	}

	cout << "swap ";

	sRIter.swap (sRIter2);

	if (sRIter <= sRIter2)
	{
		exit (-1);
	}

	cout << "swap ";

	sRIter2.swap (sRIter);
	
	if (sRIter >= sRIter2)
	{
		exit (-1);
	}

	cout << endl;
	
	cout << "const_reverse_iterator ";

	if (sCRIter >= sCRIter2)
	{
		exit (-1);
	}

	cout << "swap ";

	sCRIter.swap (sCRIter2);

	if (sCRIter <= sCRIter2)
	{
		exit (-1);
	}

	cout << "swap ";

	sCRIter2.swap (sCRIter);
	
	if (sCRIter >= sCRIter2)
	{
		exit (-1);
	}

	cout << endl;
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortIter (uint32_t nTest, _t_subnodeiter nNodeSize, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t &sCacheDesc, int argc, char **argv)
{
	typedef CBTreeKeySort <keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
																								container_t;

	typedef typename ::std::multimap<const uint32_t, keySortMap_t, multiMapMemCmp<uint32_t> >	reference_memcmp_t;

	container_t											*pClKeySort;
	reference_memcmp_t									*pClRefMemCmpKeySort;

	cout << "b-tree keysort iterator test bench selected" << endl;

	pClRefMemCmpKeySort = new reference_memcmp_t ();

	if (pClRefMemCmpKeySort == NULL)
	{
		cerr << "ERROR: Insufficient memory!" << endl;

		exit (-1);
	}

	pClKeySort = new container_t (rDataLayerProperties, &sCacheDesc, nNodeSize);

	if (pClKeySort == NULL)
	{
		cerr << "ERROR: Insufficient memory!" << endl;

		exit (-1);
	}

	switch (nTest)
	{
	case BTREETEST_KEY_SORT_ITER_CONST_ASCEND	:
		{
			TestBTreeKeySortConstIterBasic (pClKeySort, pClRefMemCmpKeySort, false, 128, 1, 0);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_ASCEND_SMALL	:
		{
			TestBTreeKeySortConstIterBasic (pClKeySort, pClRefMemCmpKeySort, false, 16, 1, 0);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_DESCEND	:
		{
			TestBTreeKeySortConstIterBasic (pClKeySort, pClRefMemCmpKeySort, true, 128, 1, 0);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_DESCEND_SMALL	:
		{
			TestBTreeKeySortConstIterBasic (pClKeySort, pClRefMemCmpKeySort, true, 16, 1, 0);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_CMPLX_ASCEND	:
		{
			TestBTreeKeySortConstIterBasic (pClKeySort, pClRefMemCmpKeySort, false, 128, 5, 3);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_CMPLX_ASCEND_SMALL	:
		{
			TestBTreeKeySortConstIterBasic (pClKeySort, pClRefMemCmpKeySort, false, 16, 2, 3);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_CMPLX_DESCEND	:
		{
			TestBTreeKeySortConstIterBasic (pClKeySort, pClRefMemCmpKeySort, true, 128, 4, 3);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_CMPLX_DESCEND_SMALL	:
		{
			TestBTreeKeySortConstIterBasic (pClKeySort, pClRefMemCmpKeySort, true, 16, 2, 3);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_NODESIZE_VS_STEPSIZE_ASCEND		:
		{
			TestBTreeKeySortConstIterNodeSizeVsStepSize (pClKeySort, pClRefMemCmpKeySort, false, rDataLayerProperties, sCacheDesc, 512, 3, 257, 1, 513);
			
			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_NODESIZE_VS_STEPSIZE_DESCEND	:
		{
			TestBTreeKeySortConstIterNodeSizeVsStepSize (pClKeySort, pClRefMemCmpKeySort, true, rDataLayerProperties, sCacheDesc, 512, 3, 257, 1, 513);

			break;
		}


	case BTREETEST_KEY_SORT_ITER_SUBSCRIPTOR	:
		{
			TestBTreeKeySortIterSubScriptor<_t_sizetype> (pClKeySort, 128);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_SUBSCRIPTOR_INT	:
		{
			TestBTreeKeySortIterSubScriptor<int> (pClKeySort, 128);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_COMPOUND	:
		{
			TestBTreeKeySortIterCompound<_t_sizetype> (pClKeySort, 128);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_COMPOUND_INT	:
		{
			TestBTreeKeySortIterCompound<int> (pClKeySort, 128);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_COMPOUND_ITER	:
		{
			TestBTreeKeySortIterCompoundIter (pClKeySort, 128);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_ARITHMETIC_OPERATORS	:
		{
			TestBTreeKeySortIterArithmeticOperators (pClKeySort, pClRefMemCmpKeySort, 40);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_COMPARE_OPERATORS	:
		{
			TestBTreeKeySortIterCompareOperators (pClKeySort, 16);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_SWAP	:
		{
			TestBTreeKeySortIterConstSwap (pClKeySort, 16);

			break;
		}

	default									:
		{
			cerr << "ERROR: TestBTreeKeySortIter: Unknown test or test not specified!" << endl;

			break;
		}
	}

	delete pClRefMemCmpKeySort;

	delete pClKeySort;
}
