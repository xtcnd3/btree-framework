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

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype>
void TestBTreeKeySortConstIterBasic (_t_obj *pClKeySort, bool bDescend, uint32_t nNumEntries, uint32_t nStepSize, uint32_t nTurnArounds, bool bFillArray = true)
{
	typedef typename _t_obj::const_iterator				citer_t;
	typedef typename _t_obj::const_reverse_iterator		criter_t;

	_t_objprim				*pClKeySortPrim;
	uint32_t				nDebug = 0;
	uint32_t				nSeed = 0;
	citer_t					sCIter;
	criter_t				sCRIter;
	_t_data					sEntryViaIf;
	_t_data					sEntryViaIter;
	uint64_t				ui64;
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

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	if (bFillArray)
	{
		keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nSeed, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
	}

	for (nTurn = 0; nTurn <= nTurnArounds; nTurn++)
	{
		cout << "turn: " << nTurn << endl;

		bool	bForward = (((nTurn & 0x1) == 0x0) != bDescend);

		ui64 = bForward ? 0ULL : pClKeySort->size ();

		sCIter = bForward ? pClKeySort->cbegin () : pClKeySort->cend ();
		sCRIter = bForward ? pClKeySort->crbegin () : pClKeySort->crend ();

		while (bForward ? ui64 < pClKeySort->size () : ui64 > 0)
		{
			if (!bForward)
			{
				if (ui64 >= nStepSize)
				{
					nThisStepSize = nStepSize;
				}
				else
				{
					nThisStepSize = (uint32_t) ui64;
				}

				ui64 -= nThisStepSize;

				if (nStepSize == 1)
				{
					if ((ui64 & 0x1) == 0x0)
					{
						sCIter--;
						sCRIter--;
					}
					else
					{
						--sCIter;
						--sCRIter;
					}
				}
				else
				{
					sCIter -= (_t_sizetype) nThisStepSize;
					sCRIter -= (_t_sizetype) nThisStepSize;
				}
			}

			cout << ui64 << " " << "\r" << flush;

			pClKeySort->get_at (ui64, sEntryViaIf);

			sEntryViaIter = *sCIter;

			if (sEntryViaIf != sEntryViaIter)
			{
				cerr << endl;
				cerr << "iterator mismatch!" << endl;
				cerr << "turn around: " << nTurn << endl;
				cerr << "position: " << ui64 << endl;
				cerr << "data via interface" << endl;
				cerr << "nData: " << sEntryViaIf.nData << endl;
				cerr << "nDebug: " << sEntryViaIf.nDebug << endl;
				cerr << "data via iterator" << endl;
				cerr << "nData: " << sEntryViaIter.nData << endl;
				cerr << "nDebug: " << sEntryViaIter.nDebug << endl;

				exit (-1);
			}

			pClKeySort->get_at (pClKeySort->size () - 1ULL - ui64, sEntryViaIf);

			sEntryViaIter = *sCRIter;

			if (sEntryViaIf != sEntryViaIter)
			{
				cerr << endl;
				cerr << "reverse iterator mismatch!" << endl;
				cerr << "turn around: " << nTurn << endl;
				cerr << "position: " << ui64 << endl;
				cerr << "data via interface" << endl;
				cerr << "nData: " << sEntryViaIf.nData << endl;
				cerr << "nDebug: " << sEntryViaIf.nDebug << endl;
				cerr << "data via iterator" << endl;
				cerr << "nData: " << sEntryViaIter.nData << endl;
				cerr << "nDebug: " << sEntryViaIter.nDebug << endl;

				exit (-1);
			}

			if (bForward)
			{
				ui64 += nStepSize;

				if (nStepSize == 1)
				{
					if ((ui64 & 0x1) == 0x0)
					{
						sCIter++;
						sCRIter++;
					}
					else
					{
						++sCIter;
						++sCRIter;
					}
				}
				else
				{
					sCIter += (_t_sizetype) nStepSize;
					sCRIter += (_t_sizetype) nStepSize;
				}
			}
		}

		cout << endl;
	}
}

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype, class _t_subnodeiter, class _t_datalayerproperties>
void TestBTreeKeySortConstIterNodeSizeVsStepSize (bool bDescend, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t &sCacheDesc, uint32_t nNumEntries, _t_subnodeiter nFromNodeSize, _t_subnodeiter nToNodeSize, uint32_t nFromStepSize, uint32_t nToStepSize)
{
	typedef ::std::vector<uint32_t>::iterator			vec_iter_t;
	typedef ::std::vector<uint32_t>						vector_t;

	_t_subnodeiter		nNodeSize;
	uint32_t			nStepSize;
	_t_obj				*pClKeySort;
	_t_objprim			*pClKeySortPrim;
	uint32_t			nDebug;
	uint32_t			nSeed;
	vector_t			sPrimeNodeSizeVector;
	vec_iter_t			sVecIter;
	uint32_t			nPrime;

	for (nNodeSize = nFromNodeSize; nNodeSize < nToNodeSize; nNodeSize++)
	{
		sPrimeNodeSizeVector.push_back (nNodeSize);
	}

	while (sPrimeNodeSizeVector.size () > 0)
	{
		nNodeSize = sPrimeNodeSizeVector.at (0);

		pClKeySort = new _t_obj (rDataLayerProperties, &sCacheDesc, nNodeSize);

		pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

		nDebug = 0;
		nSeed = 0;

		keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nSeed, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

		for (nStepSize = nFromStepSize; nStepSize < nToStepSize; nStepSize <<= 1)
		{
			TestBTreeKeySortConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClKeySort, bDescend, nNumEntries, nStepSize, 0, false);
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
		
		delete pClKeySort;
	}
}

template <class _t_obj, class _t_objprim, class _t_subscripttype, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortIterSubScriptor (_t_obj *pClKeySort, uint32_t nNumEntries)
{
	typedef typename _t_obj::const_iterator					citer_t;
	typedef typename _t_obj::const_reverse_iterator			criter_t;

	_t_objprim				*pClKeySortPrim;
	uint32_t				nDebug = 0;
	uint32_t				nSeed = 0;
	citer_t					sCIter;
	criter_t				sCRIter;
	_t_data					sEntryViaIter;
	_t_subscripttype		i;
	_t_obj					sClRefKeySort (*pClKeySort);
	
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

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nSeed, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
	
	sCIter = pClKeySort->cbegin ();
	sCRIter = pClKeySort->crbegin ();

	cout << "positiv forward iterator ";

	for (i = 0; i < (_t_subscripttype) pClKeySort->size (); i++)
	{
		sEntryViaIter = sCIter[i];

		sClRefKeySort.insert (sEntryViaIter);
	}

	cout << "testing ";

	if (sClRefKeySort != *pClKeySort)
	{
		cerr << endl;
		cerr << "keysort mismatch (positiv forward iterator sub-scription)!" << endl;
		
		exit (-1);
	}

	cout << "passed" << endl;

	sClRefKeySort.clear ();

	cout << "positiv reverse iterator ";

	for (i = 0; i < (_t_subscripttype) pClKeySort->size (); i++)
	{
		sEntryViaIter = sCRIter[i];
		
		sClRefKeySort.insert (sEntryViaIter);
	}

	cout << "testing ";

	if (sClRefKeySort != *pClKeySort)
	{
		cerr << endl;
		cerr << "keysort mismatch (positiv reverse iterator sub-scription)!" << endl;
		
		exit (-1);
	}

	cout << "passed" << endl;

	sCIter = pClKeySort->cend ();
	sCRIter = pClKeySort->crend ();

	sClRefKeySort.clear ();

	cout << "negativ forward iterator ";

	for (i = (_t_subscripttype) pClKeySort->size (); i > 0; )
	{
		i--;

		i -= (_t_subscripttype) pClKeySort->size ();
		{
			sEntryViaIter = sCIter[i];
			
			sClRefKeySort.insert (sEntryViaIter);
		}
		i += (_t_subscripttype) pClKeySort->size ();
	}

	cout << "testing ";

	if (sClRefKeySort != *pClKeySort)
	{
		cerr << endl;
		cerr << "keysort mismatch (negativ forward iterator sub-scription)!" << endl;
		
		exit (-1);
	}

	cout << "passed" << endl;

	sClRefKeySort.clear ();

	cout << "negativ reverse iterator ";

	for (i = (_t_subscripttype) pClKeySort->size (); i > 0; )
	{
		i--;

		i -= (_t_subscripttype) pClKeySort->size ();
		{
			sEntryViaIter = sCRIter[i];

			sClRefKeySort.insert (sEntryViaIter);
		}
		i += (_t_subscripttype) pClKeySort->size ();
	}

	cout << "testing ";
	
	if (sClRefKeySort != *pClKeySort)
	{
		cerr << endl;
		cerr << "keysort mismatch (negativ reverse iterator sub-scription)!" << endl;
		
		exit (-1);
	}

	cout << "passed" << endl;
}

template <class _t_obj, class _t_objprim, class _t_offsettype, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortIterCompound (_t_obj *pClKeySort, uint32_t nNumEntries)
{
	typedef typename _t_obj::const_iterator					citer_t;
	typedef typename _t_obj::const_reverse_iterator			criter_t;

	_t_objprim				*pClKeySortPrim;
	uint32_t				nDebug = 0;
	uint32_t				nSeed = 0;
	citer_t					sCIter;
	criter_t				sCRIter;
	_t_data					sEntryViaIter;
	_t_offsettype			i;
	_t_obj					sClRefKeySort (*pClKeySort);
	
	cout << "basic keysort iterator compound operator test" << endl;
	
#if defined(__GNUC__) || defined(__GNUG__)

	int		nStatus;
	char	*pszTypeid = abi::__cxa_demangle (typeid (_t_offsettype).name (), 0, 0, &nStatus);

	if (pszTypeid != NULL)
	{
		cout << "_t_sizetype is set to: " << pszTypeid << endl;
		
		free ((void *) pszTypeid);
	}
	else
	{
		cout << "_t_sizetype is set to: " << typeid (_t_offsettype).name () << endl;
	}
	
#else

	cout << "_t_sizetype is set to: " << typeid (_t_offsettype).name () << endl;
	
#endif

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nSeed, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
	
	sCIter = pClKeySort->cbegin ();
	sCRIter = pClKeySort->crbegin ();

	cout << "positiv forward iterator ";

	for (i = 0; i < (_t_offsettype) pClKeySort->size (); i++)
	{
		sCIter += i;
		{
			sEntryViaIter = *sCIter;
			
			sClRefKeySort.insert (sEntryViaIter);
		}
		sCIter -= i;
	}

	cout << "testing ";

	if (sClRefKeySort != *pClKeySort)
	{
		cerr << endl;
		cerr << "keysort mismatch (positiv forward iterator)!" << endl;
		
		exit (-1);
	}

	cout << "passed" << endl;

	sClRefKeySort.clear ();

	cout << "positiv reverse iterator ";

	for (i = 0; i < (_t_offsettype) pClKeySort->size (); i++)
	{
		sCRIter += i;
		{
			sEntryViaIter = *sCRIter;
			
			sClRefKeySort.insert (sEntryViaIter);
		}
		sCRIter -= i;
	}

	cout << "testing ";

	if (sClRefKeySort != *pClKeySort)
	{
		cerr << endl;
		cerr << "keysort mismatch (positiv reverse iterator)!" << endl;
		
		exit (-1);
	}

	cout << "passed" << endl;

	sClRefKeySort.clear ();

	sCIter = pClKeySort->cend ();
	sCRIter = pClKeySort->crend ();

	cout << "negativ forward iterator ";

	for (i = (_t_offsettype) pClKeySort->size (); i > 0; )
	{
		i--;

		i -= (_t_offsettype) pClKeySort->size ();
		{
			sCIter += i;
			{
				sEntryViaIter = *sCIter;
				
				sClRefKeySort.insert (sEntryViaIter);
			}
			sCIter -= i;
		}
		i += (_t_offsettype) pClKeySort->size ();
	}

	cout << "testing ";

	if (sClRefKeySort != *pClKeySort)
	{
		cerr << endl;
		cerr << "keysort mismatch (negativ forward iterator)!" << endl;
		
		exit (-1);
	}

	cout << "passed" << endl;

	sClRefKeySort.clear ();

	cout << "negativ reverse iterator ";

	for (i = (_t_offsettype) pClKeySort->size (); i > 0; )
	{
		i--;

		i -= (_t_offsettype) pClKeySort->size ();
		{
			sCRIter += i;
			{
				sEntryViaIter = *sCRIter;
				
				sClRefKeySort.insert (sEntryViaIter);
			}
			sCRIter -= i;
		}
		i += (_t_offsettype) pClKeySort->size ();
	}

	cout << "testing ";

	if (sClRefKeySort != *pClKeySort)
	{
		cerr << endl;
		cerr << "keysort mismatch (negativ reverse iterator)!" << endl;
		
		exit (-1);
	}

	cout << "passed" << endl;
}

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortIterCompoundIter (_t_obj *pClKeySort, uint32_t nNumEntries)
{
	typedef typename _t_obj::const_iterator					citer_t;
	typedef typename _t_obj::const_reverse_iterator			criter_t;

	_t_objprim				*pClKeySortPrim;
	uint32_t				nDebug = 0;
	uint32_t				nSeed = 0;
	citer_t					sCIter;
	_t_data					sEntryViaIter;
	citer_t					sCI;
	_t_obj					sClRefKeySort (*pClKeySort);
	
	cout << "basic keysort iterator compound operator (itreator) test" << endl;
	
	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nSeed, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
	
	sCIter = pClKeySort->cbegin ();
	
	cout << "forward iterator ";

	for (sCI = pClKeySort->cbegin (); sCI != pClKeySort->cend (); sCI++)
	{
		sCIter += sCI;
		{
			sEntryViaIter = *sCIter;
			
			sClRefKeySort.insert (sEntryViaIter);
		}
		sCIter -= sCI;
	}

	cout << "testing ";

	if (sClRefKeySort != *pClKeySort)
	{
		cerr << endl;
		cerr << "keysort mismatch (forward iterator compound operation)!" << endl;
		
		exit (-1);
	}

	cout << "passed" << endl;

	sClRefKeySort.clear ();

	sCIter = pClKeySort->cend ();
	
	cout << "reverse iterator ";

	for (sCI = pClKeySort->cend (); sCI != pClKeySort->cbegin (); )
	{
		sCI--;

		sCI -= pClKeySort->size ();
		{
			sCIter += sCI;
			{
				sEntryViaIter = *sCIter;
				
				sClRefKeySort.insert (sEntryViaIter);
			}
			sCIter -= sCI;
		}
		sCI += pClKeySort->size ();
	}

	cout << "testing ";

	if (sClRefKeySort != *pClKeySort)
	{
		cerr << endl;
		cerr << "keysort mismatch (reverse iterator compound operation)!" << endl;
		
		exit (-1);
	}

	cout << "passed" << endl;
}

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortIterArithmeticOperators (_t_obj *pClKeySort, uint32_t nNumEntries)
{
	typedef typename _t_obj::const_iterator				citer_t;
	typedef typename _t_obj::const_reverse_iterator		criter_t;

	_t_objprim				*pClKeySortPrim;
	uint32_t				nDebug = 0;
	uint32_t				nSeed = 0;
	citer_t					sCIter;
	criter_t				sCRIter;
	_t_data					sEntryViaIter;
	_t_data					sEntryViaIf;
	_t_sizetype				nOffset = 0;
	
	cout << "basic keysort iterator arithmetic operators test" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nSeed, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
	
	sCIter = pClKeySort->cbegin ();
	sCRIter = pClKeySort->crbegin ();

	cout << "const_iterator = const_iterator + _t_sizetype" << endl;

	nOffset++;

	sCIter = pClKeySort->cbegin () + (_t_sizetype) nOffset;

	pClKeySort->get_at (nOffset, sEntryViaIf);

	sEntryViaIter = *sCIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	cout << "const_iterator = const_iterator + int" << endl;
	
	nOffset++;

	sCIter = pClKeySort->cbegin () + (int) nOffset;

	pClKeySort->get_at (nOffset, sEntryViaIf);

	sEntryViaIter = *sCIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	cout << "const_iterator = _t_sizetype + const_iterator" << endl;
	
	nOffset++;

	sCIter = ((_t_sizetype) nOffset) + pClKeySort->cbegin ();

	pClKeySort->get_at (nOffset, sEntryViaIf);

	sEntryViaIter = *sCIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	cout << "const_iterator = int + const_iterator" << endl;
	
	nOffset++;

	sCIter = ((int) nOffset) + pClKeySort->cbegin ();

	pClKeySort->get_at (nOffset, sEntryViaIf);

	sEntryViaIter = *sCIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	cout << "const_iterator = const_iterator - _t_sizetype" << endl;
	
	nOffset++;

	sCIter = pClKeySort->cend () - (_t_sizetype) nOffset;

	pClKeySort->get_at (pClKeySort->size () - nOffset, sEntryViaIf);

	sEntryViaIter = *sCIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;
	
	cout << "const_iterator = const_iterator - int" << endl;
	
	nOffset++;

	sCIter = pClKeySort->cend () - (int) nOffset;

	pClKeySort->get_at (pClKeySort->size () - nOffset, sEntryViaIf);

	sEntryViaIter = *sCIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;
	
	cout << "_t_sizetype = const_iterator - const_iterator" << endl;

	_t_sizetype		nSize = pClKeySort->cend () - pClKeySort->cbegin ();

	if (nSize != pClKeySort->size ())
	{
		cout << "failed!" << endl;
	}

	cout << "const_reverse_iterator = const_reverse_iterator + _t_sizetype" << endl;
	
	nOffset++;

	sCRIter = pClKeySort->crbegin () + (_t_sizetype) nOffset;

	pClKeySort->get_at (pClKeySort->size () - (nOffset + 1), sEntryViaIf);

	sEntryViaIter = *sCRIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	cout << "const_reverse_iterator = const_reverse_iterator + int" << endl;
	
	nOffset++;

	sCRIter = pClKeySort->crbegin () + (int) nOffset;

	pClKeySort->get_at (pClKeySort->size () - (nOffset + 1), sEntryViaIf);

	sEntryViaIter = *sCRIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	cout << "const_reverse_iterator = const_reverse_iterator - _t_sizetype" << endl;
	
	nOffset++;

	sCRIter = pClKeySort->crend () - (_t_sizetype) nOffset;

	pClKeySort->get_at (nOffset - 1, sEntryViaIf);

	sEntryViaIter = *sCRIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	cout << "const_reverse_iterator = const_reverse_iterator - int" << endl;
	
	nOffset++;

	sCRIter = pClKeySort->crend () - (int) nOffset;

	pClKeySort->get_at (nOffset - 1, sEntryViaIf);

	sEntryViaIter = *sCRIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;
}

template <class _t_obj, class _t_lhiter, class _t_rhiter>
void TestBTreeKeySortIterCompareOperators (_t_obj *pClKeySort, uint32_t nNumEntries, _t_lhiter sLHIterBegin, _t_lhiter sLHIterEnd, _t_rhiter sRHIterBegin, _t_rhiter sRHIterEnd)
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

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortIterCompareOperators (_t_obj *pClKeySort, uint32_t nNumEntries)
{
	typedef typename _t_obj::const_iterator				citer_t;
	typedef typename _t_obj::const_reverse_iterator		criter_t;

	_t_objprim				*pClKeySortPrim;
	uint32_t				nDebug = 0;
	uint32_t				nSeed = 0;
	
	cout << "basic array iterator compare operators test" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nSeed, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
	
	cout << "const_iterator versus const_iterator";
	
	TestBTreeKeySortIterCompareOperators<_t_obj, citer_t, citer_t> (pClKeySort, nNumEntries, pClKeySort->cbegin (), pClKeySort->cend (), pClKeySort->cbegin (), pClKeySort->cend ());

	cout << "const_reverse_iterator versus const_reverse_iterator";

	TestBTreeKeySortIterCompareOperators<_t_obj, criter_t, criter_t> (pClKeySort, nNumEntries, pClKeySort->crbegin (), pClKeySort->crend (), pClKeySort->crbegin (), pClKeySort->crend ());
}

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortIterConstSwap (_t_obj *pClKeySort, uint32_t nNumEntries)
{
	typedef typename _t_obj::const_iterator					citer_t;
	typedef typename _t_obj::const_reverse_iterator			criter_t;

	_t_objprim				*pClKeySortPrim;
	uint32_t				nDebug = 0;
	uint32_t				nSeed = 0;
	citer_t					sCIter;
	citer_t					sCIter2;
	criter_t				sCRIter;
	criter_t				sCRIter2;
	
	cout << "basic keysort const iterator swap test" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nSeed, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
	
	sCIter = pClKeySort->cbegin ();
	sCIter2 = pClKeySort->cend ();

	sCRIter = pClKeySort->crbegin ();
	sCRIter2 = pClKeySort->crend ();

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

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortIter (uint32_t nTest, _t_subnodeiter nNodeSize, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t &sCacheDesc, int argc, char **argv)
{
	_t_obj												*pClKeySort;

	cout << "b-tree keysort iterator test bench selected" << endl;

	pClKeySort = new _t_obj (rDataLayerProperties, &sCacheDesc, nNodeSize, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));

	if (pClKeySort == NULL)
	{
		cerr << "ERROR: Insufficient memory!" << endl;

		exit (-1);
	}

	switch (nTest)
	{
	case BTREETEST_KEY_SORT_ITER_CONST_ASCEND	:
		{
			TestBTreeKeySortConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClKeySort, false, 128, 1, 0);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_ASCEND_SMALL	:
		{
			TestBTreeKeySortConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClKeySort, false, 16, 1, 0);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_DESCEND	:
		{
			TestBTreeKeySortConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClKeySort, true, 128, 1, 0);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_DESCEND_SMALL	:
		{
			TestBTreeKeySortConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClKeySort, true, 16, 1, 0);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_CMPLX_ASCEND	:
		{
			TestBTreeKeySortConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClKeySort, false, 128, 5, 3);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_CMPLX_ASCEND_SMALL	:
		{
			TestBTreeKeySortConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClKeySort, false, 16, 2, 3);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_CMPLX_DESCEND	:
		{
			TestBTreeKeySortConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClKeySort, true, 128, 4, 3);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_CMPLX_DESCEND_SMALL	:
		{
			TestBTreeKeySortConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClKeySort, true, 16, 2, 3);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_NODESIZE_VS_STEPSIZE_ASCEND		:
		{
			TestBTreeKeySortConstIterNodeSizeVsStepSize<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_subnodeiter, _t_datalayerproperties> (false, rDataLayerProperties, sCacheDesc, 512, 3, 257, 1, 513);
			
			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_NODESIZE_VS_STEPSIZE_DESCEND	:
		{
			TestBTreeKeySortConstIterNodeSizeVsStepSize<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_subnodeiter, _t_datalayerproperties> (true, rDataLayerProperties, sCacheDesc, 512, 3, 257, 1, 513);

			break;
		}


	case BTREETEST_KEY_SORT_ITER_SUBSCRIPTOR	:
		{
			TestBTreeKeySortIterSubScriptor<_t_obj, _t_objprim, _t_sizetype, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClKeySort, 128);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_SUBSCRIPTOR_INT	:
		{
			TestBTreeKeySortIterSubScriptor<_t_obj, _t_objprim, int, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClKeySort, 128);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_COMPOUND	:
		{
			TestBTreeKeySortIterCompound<_t_obj, _t_objprim, _t_sizetype, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClKeySort, 128);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_COMPOUND_INT	:
		{
			TestBTreeKeySortIterCompound<_t_obj, _t_objprim, int, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClKeySort, 128);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_COMPOUND_ITER	:
		{
			TestBTreeKeySortIterCompoundIter<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClKeySort, 128);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_ARITHMETIC_OPERATORS	:
		{
			TestBTreeKeySortIterArithmeticOperators<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClKeySort, 40);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_COMPARE_OPERATORS	:
		{
			TestBTreeKeySortIterCompareOperators<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClKeySort, 16);

			break;
		}

	case BTREETEST_KEY_SORT_ITER_CONST_SWAP	:
		{
			TestBTreeKeySortIterConstSwap<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClKeySort, 16);

			break;
		}

	default									:
		{
			cerr << "ERROR: TestBTreeKeySortIter: Unknown test or test not specified!" << endl;

			break;
		}
	}

	delete pClKeySort;
}
