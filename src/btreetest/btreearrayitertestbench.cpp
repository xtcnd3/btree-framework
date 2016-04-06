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

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype>
void TestBTreeArrayConstIterBasic (_t_obj *pClArray, bool bDescend, uint32_t nNumEntries, uint32_t nStepSize, uint32_t nTurnArounds, bool bFillArray = true)
{
	typedef typename _t_obj::const_iterator				citer_t;
	typedef typename _t_obj::const_reverse_iterator		criter_t;

	_t_objprim				*pClArrayPrim;
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

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	if (bFillArray)
	{
		arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	}

	for (nTurn = 0; nTurn <= nTurnArounds; nTurn++)
	{
		cout << "turn: " << nTurn << endl;

		bool	bForward = (((nTurn & 0x1) == 0x0) != bDescend);

		ui64 = bForward ? 0ULL : pClArray->size ();

		sCIter = bForward ? pClArray->cbegin () : pClArray->cend ();
		sCRIter = bForward ? pClArray->crbegin () : pClArray->crend ();

		while (bForward ? ui64 < pClArray->size () : ui64 > 0)
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

			sEntryViaIf = pClArray->at (ui64);

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

			sEntryViaIf = pClArray->at (pClArray->size () - 1ULL - ui64);

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
void TestBTreeArrayConstIterNodeSizeVsStepSize (bool bDescend, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t &sCacheDesc, uint32_t nNumEntries, _t_subnodeiter nFromNodeSize, _t_subnodeiter nToNodeSize, uint32_t nFromStepSize, uint32_t nToStepSize)
{
	typedef ::std::vector<uint32_t>::iterator			vec_iter_t;
	typedef ::std::vector<uint32_t>						vector_t;

	typedef ::std::vector<arrayEntry_t>					vector_test_t;
	typedef ::std::vector<arrayEntry_t>::iterator		vector_test_iter_t;

	_t_subnodeiter		nNodeSize;
	uint32_t			nStepSize;
	_t_obj				*pClArray;
	vector_t			sPrimeNodeSizeVector;
	vec_iter_t			sVecIter;
	vector_test_t		sTestVector;
	uint32_t			i;
	arrayEntry_t		sEntry;
	uint32_t			nPrime;

	for (nNodeSize = nFromNodeSize; nNodeSize < nToNodeSize; nNodeSize++)
	{
		sPrimeNodeSizeVector.push_back (nNodeSize);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sEntry.nDebug = g_nDebug;
		sEntry.nData = generate_rand32 ();

		sTestVector.push_back (sEntry);

		g_nDebug++;
	}

	while (sPrimeNodeSizeVector.size () > 0)
	{
		nNodeSize = sPrimeNodeSizeVector.at (0);

		pClArray = new _t_obj (rDataLayerProperties, &sCacheDesc, nNodeSize);

		if (pClArray == NULL)
		{
			cerr << "TestBTreeArrayConstIterNodeSizeVsStepSize: ERROR: insufficient memory!" << endl;

			exit (-1);
		}

		pClArray->template assign<vector_test_iter_t> (sTestVector.begin (), sTestVector.end ());

		for (nStepSize = nFromStepSize; nStepSize < nToStepSize; nStepSize <<= 1)
		{
			TestBTreeArrayConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, bDescend, nNumEntries, nStepSize, 0, false);
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

		delete pClArray;
	}
}

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype>
void TestBTreeArrayIterBasic (_t_obj *pClArray, bool bDescend, uint32_t nNumEntries, uint32_t nStepSize, uint32_t nTurnArounds, bool bFillArray = true)
{
	typedef typename _t_obj::iterator					iter_t;
	typedef typename _t_obj::reverse_iterator			riter_t;

	_t_objprim				*pClArrayPrim;
	iter_t					sIter;
	riter_t					sRIter;
	_t_data					sEntryViaIf;
	_t_data					sEntryViaIter;
	uint64_t				ui64;
	uint32_t				nTurn;
	uint32_t				nThisStepSize;
	
	cout << "basic read-write iterator test ";

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

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	if (bFillArray)
	{
		arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	}

	for (nTurn = 0; nTurn <= nTurnArounds; nTurn++)
	{
		cout << "turn: " << nTurn << endl;

		bool	bForward = (((nTurn & 0x1) == 0x0) != bDescend);

		ui64 = bForward ? 0ULL : pClArray->size ();

		sIter = bForward ? pClArray->begin () : pClArray->end ();
		sRIter = bForward ? pClArray->rbegin () : pClArray->rend ();

		while (bForward ? ui64 < pClArray->size () : ui64 > 0)
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
						sIter--;
						sRIter--;
					}
					else
					{
						--sIter;
						--sRIter;
					}
				}
				else
				{
					sIter -= (_t_sizetype) nThisStepSize;
					sRIter -= (_t_sizetype) nThisStepSize;
				}
			}

			cout << ui64 << " " << "\r" << flush;

			sEntryViaIf = pClArray->at (ui64);

			sEntryViaIter = *sIter;

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

			sEntryViaIf = pClArray->at (pClArray->size () - 1ULL - ui64);

			sEntryViaIter = *sRIter;

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
						sIter++;
						sRIter++;
					}
					else
					{
						++sIter;
						++sRIter;
					}
				}
				else
				{
					sIter += (_t_sizetype) nStepSize;
					sRIter += (_t_sizetype) nStepSize;
				}
			}
		}

		cout << endl;
	}
}

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype, class _t_subnodeiter, class _t_datalayerproperties>
void TestBTreeArrayIterNodeSizeVsStepSize (bool bDescend, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t &sCacheDesc, uint32_t nNumEntries, _t_subnodeiter nFromNodeSize, _t_subnodeiter nToNodeSize, uint32_t nFromStepSize, uint32_t nToStepSize)
{
	typedef ::std::vector<uint32_t>::iterator			vec_iter_t;
	typedef ::std::vector<uint32_t>						vector_t;

	typedef ::std::vector<arrayEntry_t>					vector_test_t;
	typedef ::std::vector<arrayEntry_t>::iterator		vector_test_iter_t;

	_t_subnodeiter		nNodeSize;
	uint32_t			nStepSize;
	_t_obj				*pClArray;
	vector_t			sPrimeNodeSizeVector;
	vec_iter_t			sVecIter;
	vector_test_t		sTestVector;
	uint32_t			i;
	arrayEntry_t		sEntry;
	uint32_t			nPrime;

	for (nNodeSize = nFromNodeSize; nNodeSize < nToNodeSize; nNodeSize++)
	{
		sPrimeNodeSizeVector.push_back (nNodeSize);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sEntry.nDebug = g_nDebug;
		sEntry.nData = generate_rand32 ();

		sTestVector.push_back (sEntry);

		g_nDebug++;
	}

	while (sPrimeNodeSizeVector.size () > 0)
	{
		nNodeSize = sPrimeNodeSizeVector.at (0);

		pClArray = new _t_obj (rDataLayerProperties, &sCacheDesc, nNodeSize);

		if (pClArray == NULL)
		{
			cerr << "TestBTreeArrayConstIterNodeSizeVsStepSize: ERROR: insufficient memory!" << endl;

			exit (-1);
		}

		pClArray->template assign<vector_test_iter_t> (sTestVector.begin (), sTestVector.end ());

		for (nStepSize = nFromStepSize; nStepSize < nToStepSize; nStepSize <<= 1)
		{
			TestBTreeArrayIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, bDescend, nNumEntries, nStepSize, 0, false);
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

		delete pClArray;
	}
}

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayIterBiDirectionalDereference (_t_obj *pClArray, uint32_t nNumEntries)
{
	typedef typename _t_obj::iterator					iter_t;
	typedef typename _t_obj::reverse_iterator			riter_t;

	_t_objprim				*pClArrayPrim;
	iter_t					sWriteIter;
	riter_t					sWriteRIter;
	_t_data					sEntryViaIter;
	uint64_t				ui64;
	_t_obj					sClRefArray (*pClArray);

	cout << "basic array iterator bi-directional dereference test" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	
	sWriteIter = pClArray->begin ();
	sWriteRIter = pClArray->rbegin ();

	sClRefArray = *pClArray;

	for (ui64 = 0; ui64 < pClArray->size (); ui64++)
	{
		sEntryViaIter = *sWriteIter;
		sEntryViaIter.nData = ~sEntryViaIter.nData;
		*sWriteIter = sEntryViaIter;

		sEntryViaIter = *sWriteRIter;
		sEntryViaIter.nData = ~sEntryViaIter.nData;
		*sWriteRIter = sEntryViaIter;

		sWriteIter++;
		sWriteRIter++;
	}

	if (sClRefArray != *pClArray)
	{
		cerr << endl;
		cerr << "array mismatch!" << endl;
		
		exit (-1);
	}
}

template <class _t_obj, class _t_objprim, class _t_subscripttype, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayIterBiDirectionalSubScriptor (_t_obj *pClArray, uint32_t nNumEntries)
{
	typedef typename _t_obj::iterator					iter_t;
	typedef typename _t_obj::reverse_iterator			riter_t;

	_t_objprim				*pClArrayPrim;
	iter_t					sWriteIter;
	riter_t					sWriteRIter;
	_t_data					sEntryViaIter;
	_t_subscripttype		i;
	_t_obj					sClRefArray (*pClArray);
	
	cout << "basic array iterator bi-directional sub-scription test" << endl;
	
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

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	
	sWriteIter = pClArray->begin ();
	sWriteRIter = pClArray->rbegin ();

	sClRefArray = *pClArray;

	for (i = 0; i < (_t_subscripttype) pClArray->size (); i++)
	{
		sEntryViaIter = sWriteIter[i];
		sEntryViaIter.nData = ~sEntryViaIter.nData;
		sWriteIter[i] = sEntryViaIter;

		sEntryViaIter = sWriteRIter[i];
		sEntryViaIter.nData = ~sEntryViaIter.nData;
		sWriteRIter[i] = sEntryViaIter;
	}

	if (sClRefArray != *pClArray)
	{
		cerr << endl;
		cerr << "array mismatch (positiv sub-scription)!" << endl;
		
		exit (-1);
	}

	sWriteIter = pClArray->end ();
	sWriteRIter = pClArray->rend ();

	for (i = (_t_subscripttype) pClArray->size (); i > 0; )
	{
		i--;

		i -= (_t_subscripttype) pClArray->size ();
		{
			sEntryViaIter = sWriteIter[i];
			sEntryViaIter.nData = ~sEntryViaIter.nData;
			sWriteIter[i] = sEntryViaIter;

			sEntryViaIter = sWriteRIter[i];
			sEntryViaIter.nData = ~sEntryViaIter.nData;
			sWriteRIter[i] = sEntryViaIter;
		}
		i += (_t_subscripttype) pClArray->size ();
	}

	if (sClRefArray != *pClArray)
	{
		cerr << endl;
		cerr << "array mismatch (negativ sub-scription)!" << endl;
		
		exit (-1);
	}
}

template <class _t_obj, class _t_objprim, class _t_offsettype, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayIterCompound (_t_obj *pClArray, uint32_t nNumEntries)
{
	typedef typename _t_obj::iterator					iter_t;
	typedef typename _t_obj::reverse_iterator			riter_t;

	_t_objprim				*pClArrayPrim;
	iter_t					sWriteIter;
	riter_t					sWriteRIter;
	_t_data					sEntryViaIter;
	_t_offsettype			i;
	_t_obj					sClRefArray (*pClArray);
	
	cout << "basic array iterator compound operator test" << endl;
	
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

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	
	sWriteIter = pClArray->begin ();
	sWriteRIter = pClArray->rbegin ();

	sClRefArray = *pClArray;

	for (i = 0; i < (_t_offsettype) pClArray->size (); i++)
	{
		sWriteIter += i;
		{
			sEntryViaIter = *sWriteIter;
			sEntryViaIter.nData = ~sEntryViaIter.nData;
			*sWriteIter = sEntryViaIter;
		}
		sWriteIter -= i;

		sWriteRIter += i;
		{
			sEntryViaIter = *sWriteRIter;
			sEntryViaIter.nData = ~sEntryViaIter.nData;
			*sWriteRIter = sEntryViaIter;
		}
		sWriteRIter -= i;
	}

	if (sClRefArray != *pClArray)
	{
		cerr << endl;
		cerr << "array mismatch (positiv iteration)!" << endl;
		
		exit (-1);
	}

	sWriteIter = pClArray->end ();
	sWriteRIter = pClArray->rend ();

	for (i = (_t_offsettype) pClArray->size (); i > 0; )
	{
		i--;

		i -= (_t_offsettype) pClArray->size ();
		{
			sWriteIter += i;
			{
				sEntryViaIter = *sWriteIter;
				sEntryViaIter.nData = ~sEntryViaIter.nData;
				*sWriteIter = sEntryViaIter;
			}
			sWriteIter -= i;

			sWriteRIter += i;
			{
				sEntryViaIter = *sWriteRIter;
				sEntryViaIter.nData = ~sEntryViaIter.nData;
				*sWriteRIter = sEntryViaIter;
			}
			sWriteRIter -= i;
		}
		i += (_t_offsettype) pClArray->size ();
	}

	if (sClRefArray != *pClArray)
	{
		cerr << endl;
		cerr << "array mismatch (negativ iteration)!" << endl;
		
		exit (-1);
	}
}

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayIterCompoundIter (_t_obj *pClArray, uint32_t nNumEntries)
{
	typedef typename _t_obj::iterator					iter_t;
	typedef typename _t_obj::reverse_iterator			riter_t;

	_t_objprim				*pClArrayPrim;
	iter_t					sWriteIter;
	_t_data					sEntryViaIter;
	iter_t					sI;
	_t_obj					sClRefArray (*pClArray);
	
	cout << "basic array iterator compound operator (iterator) test" << endl;
	
	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	
	sWriteIter = pClArray->begin ();
	
	sClRefArray = *pClArray;

	for (sI = pClArray->begin (); sI != pClArray->end (); sI++)
	{
		sWriteIter += sI;
		{
			sEntryViaIter = *sWriteIter;
			sEntryViaIter.nData = ~sEntryViaIter.nData;
			*sWriteIter = sEntryViaIter;
		}
		sWriteIter -= sI;
	}

	sWriteIter = pClArray->end ();
	
	for (sI = pClArray->end (); sI != pClArray->begin (); )
	{
		sI--;

		sI -= pClArray->size ();
		{
			sWriteIter += sI;
			{
				sEntryViaIter = *sWriteIter;
				sEntryViaIter.nData = ~sEntryViaIter.nData;
				*sWriteIter = sEntryViaIter;
			}
			sWriteIter -= sI;
		}
		sI += pClArray->size ();
	}

	if (sClRefArray != *pClArray)
	{
		cerr << endl;
		cerr << "array mismatch (negativ sub-scription)!" << endl;
		
		exit (-1);
	}
}

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayIterArithmeticOperators (_t_obj *pClArray, uint32_t nNumEntries)
{
	typedef typename _t_obj::iterator					iter_t;
	typedef typename _t_obj::reverse_iterator			riter_t;
	typedef typename _t_obj::const_iterator				citer_t;
	typedef typename _t_obj::const_reverse_iterator		criter_t;

	_t_objprim				*pClArrayPrim;
	iter_t					sIter;
	riter_t					sRIter;
	citer_t					sCIter;
	criter_t				sCRIter;
	_t_data					sEntryViaIter;
	_t_data					sEntryViaIf;
	_t_sizetype				nOffset = 0;
	
	cout << "basic array iterator arithmetic operators test" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	
	sIter = pClArray->begin ();
	sRIter = pClArray->rbegin ();
	sCIter = pClArray->cbegin ();
	sCRIter = pClArray->crbegin ();

	cout << "const_iterator = const_iterator + _t_sizetype" << endl;

	nOffset++;

	sCIter = pClArray->cbegin () + (_t_sizetype) nOffset;

	sEntryViaIf = pClArray->at (nOffset);

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

	sCIter = pClArray->cbegin () + (int) nOffset;

	sEntryViaIf = pClArray->at (nOffset);

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

	sCIter = ((_t_sizetype) nOffset) + pClArray->cbegin ();

	sEntryViaIf = pClArray->at (nOffset);

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

	sCIter = ((int) nOffset) + pClArray->cbegin ();

	sEntryViaIf = pClArray->at (nOffset);

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

	sCIter = pClArray->cend () - (_t_sizetype) nOffset;

	sEntryViaIf = pClArray->at (pClArray->size () - nOffset);

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

	sCIter = pClArray->cend () - (int) nOffset;

	sEntryViaIf = pClArray->at (pClArray->size () - nOffset);

	sEntryViaIter = *sCIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;
	
	cout << "_t_sizetype = const_iterator - const_iterator" << endl;

	_t_sizetype		nSize = pClArray->cend () - pClArray->cbegin ();

	if (nSize != pClArray->size ())
	{
		cout << "failed!" << endl;
	}

	cout << "iterator = iterator + _t_sizetype" << endl;
	
	nOffset++;

	sIter = pClArray->begin () + (_t_sizetype) nOffset;

	sEntryViaIf = pClArray->at (nOffset);

	sEntryViaIter = *sIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	cout << "iterator = iterator + int" << endl;
	
	nOffset++;

	sIter = pClArray->begin () + (int) nOffset;

	sEntryViaIf = pClArray->at (nOffset);

	sEntryViaIter = *sIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	cout << "iterator = _t_sizetype + iterator" << endl;
	
	nOffset++;

	sIter = (_t_sizetype) nOffset + pClArray->begin ();

	sEntryViaIf = pClArray->at (nOffset);

	sEntryViaIter = *sIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	cout << "iterator = int + iterator" << endl;
	
	nOffset++;

	sIter = (int) nOffset + pClArray->begin ();

	sEntryViaIf = pClArray->at (nOffset);

	sEntryViaIter = *sIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	cout << "iterator = iterator - _t_sizetype" << endl;
	
	nOffset++;

	sIter = pClArray->end () - (_t_sizetype) nOffset;

	sEntryViaIf = pClArray->at (pClArray->size () - nOffset);

	sEntryViaIter = *sIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;
	
	cout << "iterator = iterator - int" << endl;
	
	nOffset++;

	sIter = pClArray->end () - (int) nOffset;

	sEntryViaIf = pClArray->at (pClArray->size () - nOffset);

	sEntryViaIter = *sIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;
	
	cout << "_t_sizetype = iterator - iterator" << endl;

	nSize = pClArray->end () - pClArray->begin ();

	if (nSize != pClArray->size ())
	{
		cout << "failed!" << endl;
	}

	cout << "const_reverse_iterator = const_reverse_iterator + _t_sizetype" << endl;
	
	nOffset++;

	sCRIter = pClArray->crbegin () + (_t_sizetype) nOffset;

	sEntryViaIf = pClArray->at (pClArray->size () - (nOffset + 1));

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

	sCRIter = pClArray->crbegin () + (int) nOffset;

	sEntryViaIf = pClArray->at (pClArray->size () - (nOffset + 1));

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

	sCRIter = pClArray->crend () - (_t_sizetype) nOffset;

	sEntryViaIf = pClArray->at (nOffset - 1);

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

	sCRIter = pClArray->crend () - (int) nOffset;

	sEntryViaIf = pClArray->at (nOffset - 1);

	sEntryViaIter = *sCRIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	cout << "reverse_iterator = reverse_iterator + _t_sizetype" << endl;
	
	nOffset++;

	sRIter = pClArray->rbegin () + (_t_sizetype) nOffset;

	sEntryViaIf = pClArray->at (pClArray->size () - (nOffset + 1));

	sEntryViaIter = *sRIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	cout << "reverse_iterator = reverse_iterator + int" << endl;
	
	nOffset++;

	sRIter = pClArray->rbegin () + (int) nOffset;

	sEntryViaIf = pClArray->at (pClArray->size () - (nOffset + 1));

	sEntryViaIter = *sRIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;

	cout << "reverse_iterator = reverse_iterator - _t_sizetype" << endl;
	
	nOffset++;

	sRIter = pClArray->rend () - (_t_sizetype) nOffset;

	sEntryViaIf = pClArray->at (nOffset - 1);

	sEntryViaIter = *sRIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}

	sEntryViaIf.nData = 0xCCCCCCCC;
	sEntryViaIter.nData = 0xDDDDDDDD;
	
	cout << "reverse_iterator = reverse_iterator - int" << endl;

	nOffset++;

	sRIter = pClArray->rend () - (int) nOffset;

	sEntryViaIf = pClArray->at (nOffset - 1);

	sEntryViaIter = *sRIter;

	if (sEntryViaIf != sEntryViaIter)
	{
		cerr << "failed!" << endl;

		exit (-1);
	}
}

template <class _t_obj, class _t_lhiter, class _t_rhiter>
void TestBTreeArrayIterCompareOperators (_t_obj *pClArray, uint32_t nNumEntries, _t_lhiter sLHIterBegin, _t_lhiter sLHIterEnd, _t_rhiter sRHIterBegin, _t_rhiter sRHIterEnd)
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
void TestBTreeArrayIterCompareOperators (_t_obj *pClArray, uint32_t nNumEntries)
{
	typedef typename _t_obj::iterator					iter_t;
	typedef typename _t_obj::reverse_iterator			riter_t;
	typedef typename _t_obj::const_iterator				citer_t;
	typedef typename _t_obj::const_reverse_iterator		criter_t;

	_t_objprim				*pClArrayPrim;
	cout << "basic array iterator compare operators test" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	
	cout << "iterator versus iterator";

	TestBTreeArrayIterCompareOperators<_t_obj, iter_t, iter_t> (pClArray, nNumEntries, pClArray->begin (), pClArray->end (), pClArray->begin (), pClArray->end ());

	cout << "const_iterator versus const_iterator";
	
	TestBTreeArrayIterCompareOperators<_t_obj, citer_t, citer_t> (pClArray, nNumEntries, pClArray->cbegin (), pClArray->cend (), pClArray->cbegin (), pClArray->cend ());

	cout << "const_iterator versus iterator";
	
	//TestBTreeArrayIterCompareOperators<_t_obj, citer_t, iter_t> (pClArray, nNumEntries, pClArray->cbegin (), pClArray->cend (), pClArray->begin (), pClArray->end ());
	cout << endl;

	cout << "iterator versus const_iterator";

	cout << endl;

	cout << "reverse_iterator versus reverse_iterator";
	
	TestBTreeArrayIterCompareOperators<_t_obj, riter_t, riter_t> (pClArray, nNumEntries, pClArray->rbegin (), pClArray->rend (), pClArray->rbegin (), pClArray->rend ());

	cout << "const_reverse_iterator versus const_reverse_iterator";

	TestBTreeArrayIterCompareOperators<_t_obj, criter_t, criter_t> (pClArray, nNumEntries, pClArray->crbegin (), pClArray->crend (), pClArray->crbegin (), pClArray->crend ());

	cout << "const_reverse_iterator versus reverse_iterator";

	cout << endl;

	cout << "reverse_iterator versus const_reverse_iterator";

	cout << endl;
}

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayIterSwap (_t_obj *pClArray, uint32_t nNumEntries)
{
	typedef typename _t_obj::iterator					iter_t;
	typedef typename _t_obj::reverse_iterator			riter_t;

	_t_objprim				*pClArrayPrim;
	iter_t					sWriteIter;
	iter_t					sWriteIter2;
	riter_t					sRWriteIter;
	riter_t					sRWriteIter2;
	_t_data					sEntryViaIter;
	uint64_t				ui64;
	_t_obj					sClRefArray (*pClArray);
	
	cout << "basic array iterator swap test" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	
	sWriteIter = pClArray->begin ();
	sWriteIter2 = pClArray->begin ();

	sRWriteIter = pClArray->rbegin ();
	sRWriteIter2 = pClArray->rbegin ();

	sClRefArray = *pClArray;

	for (ui64 = 0; ui64 < pClArray->size (); ui64++)
	{
		sEntryViaIter = *sWriteIter;
		sEntryViaIter.nData = ~sEntryViaIter.nData;
		*sWriteIter = sEntryViaIter;

		sWriteIter++;

		sEntryViaIter = *sRWriteIter;
		sEntryViaIter.nData = ~sEntryViaIter.nData;
		*sRWriteIter = sEntryViaIter;

		sRWriteIter++;
	}

	sWriteIter.swap (sWriteIter2);
	sRWriteIter.swap (sRWriteIter2);

	for (ui64 = 0; ui64 < pClArray->size (); ui64++)
	{
		sWriteIter2--;

		sEntryViaIter = *sWriteIter2;
		sEntryViaIter.nData = ~sEntryViaIter.nData;
		*sWriteIter2 = sEntryViaIter;

		sRWriteIter2--;

		sEntryViaIter = *sRWriteIter2;
		sEntryViaIter.nData = ~sEntryViaIter.nData;
		*sRWriteIter2 = sEntryViaIter;
	}

	if (sClRefArray != *pClArray)
	{
		cerr << endl;
		cerr << "array mismatch!" << endl;
		
		exit (-1);
	}
}

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayIterConstSwap (_t_obj *pClArray, uint32_t nNumEntries)
{
	typedef typename _t_obj::const_iterator					citer_t;
	typedef typename _t_obj::const_reverse_iterator			criter_t;

	_t_objprim				*pClArrayPrim;
	citer_t					sCIter;
	citer_t					sCIter2;
	criter_t				sCRIter;
	criter_t				sCRIter2;
	
	cout << "basic array const iterator swap test" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	
	sCIter = pClArray->cbegin ();
	sCIter2 = pClArray->cend ();

	sCRIter = pClArray->crbegin ();
	sCRIter2 = pClArray->crend ();

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
void TestBTreeArrayIter (uint32_t nTest, _t_subnodeiter nNodeSize, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t &sCacheDesc, int argc, char **argv)
{
	_t_obj												*pClArray;

	cout << "b-tree array iterator test bench selected" << endl;

	pClArray = new _t_obj (rDataLayerProperties, &sCacheDesc, nNodeSize);

	if (pClArray == NULL)
	{
		cerr << "ERROR: Insufficient memory!" << endl;

		exit (-1);
	}

	switch (nTest)
	{
	case BTREETEST_ARRAY_ITER_CONST_ASCEND	:
		{
			TestBTreeArrayConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, false, 128, 1, 0);

			break;
		}

	case BTREETEST_ARRAY_ITER_CONST_ASCEND_SMALL	:
		{
			TestBTreeArrayConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, false, 16, 1, 0);

			break;
		}

	case BTREETEST_ARRAY_ITER_CONST_DESCEND	:
		{
			TestBTreeArrayConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, true, 128, 1, 0);

			break;
		}

	case BTREETEST_ARRAY_ITER_CONST_DESCEND_SMALL	:
		{
			TestBTreeArrayConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, true, 16, 1, 0);

			break;
		}

	case BTREETEST_ARRAY_ITER_CONST_CMPLX_ASCEND	:
		{
			TestBTreeArrayConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, false, 128, 5, 3);

			break;
		}

	case BTREETEST_ARRAY_ITER_CONST_CMPLX_ASCEND_SMALL	:
		{
			TestBTreeArrayConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, false, 16, 2, 3);

			break;
		}

	case BTREETEST_ARRAY_ITER_CONST_CMPLX_DESCEND	:
		{
			TestBTreeArrayConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, true, 128, 4, 3);

			break;
		}

	case BTREETEST_ARRAY_ITER_CONST_CMPLX_DESCEND_SMALL	:
		{
			TestBTreeArrayConstIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, true, 16, 2, 3);

			break;
		}

	case BTREETEST_ARRAY_ITER_CONST_NODESIZE_VS_STEPSIZE_ASCEND		:
		{
			TestBTreeArrayConstIterNodeSizeVsStepSize<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_subnodeiter, _t_datalayerproperties> (false, rDataLayerProperties, sCacheDesc, 512, 3, 257, 1, 513);
			
			break;
		}

	case BTREETEST_ARRAY_ITER_CONST_NODESIZE_VS_STEPSIZE_DESCEND	:
		{
			TestBTreeArrayConstIterNodeSizeVsStepSize<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_subnodeiter, _t_datalayerproperties> (true, rDataLayerProperties, sCacheDesc, 512, 3, 257, 1, 513);

			break;
		}

	case BTREETEST_ARRAY_ITER_ASCEND	:
		{
			TestBTreeArrayIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, false, 128, 1, 0);

			break;
		}

	case BTREETEST_ARRAY_ITER_ASCEND_SMALL	:
		{
			TestBTreeArrayIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, false, 16, 1, 0);

			break;
		}

	case BTREETEST_ARRAY_ITER_DESCEND	:
		{
			TestBTreeArrayIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, true, 128, 1, 0);

			break;
		}

	case BTREETEST_ARRAY_ITER_DESCEND_SMALL	:
		{
			TestBTreeArrayIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, true, 16, 1, 0);

			break;
		}

	case BTREETEST_ARRAY_ITER_CMPLX_ASCEND	:
		{
			TestBTreeArrayIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, false, 128, 5, 3);

			break;
		}

	case BTREETEST_ARRAY_ITER_CMPLX_ASCEND_SMALL	:
		{
			TestBTreeArrayIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, false, 16, 2, 3);

			break;
		}

	case BTREETEST_ARRAY_ITER_CMPLX_DESCEND	:
		{
			TestBTreeArrayIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, true, 128, 4, 3);

			break;
		}

	case BTREETEST_ARRAY_ITER_CMPLX_DESCEND_SMALL	:
		{
			TestBTreeArrayIterBasic<_t_obj, _t_objprim, _t_data, _t_sizetype> (pClArray, true, 16, 2, 3);

			break;
		}

	case BTREETEST_ARRAY_ITER_NODESIZE_VS_STEPSIZE_ASCEND		:
		{
			TestBTreeArrayIterNodeSizeVsStepSize<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_subnodeiter, _t_datalayerproperties> (false, rDataLayerProperties, sCacheDesc, 512, 3, 257, 1, 513);
			
			break;
		}

	case BTREETEST_ARRAY_ITER_NODESIZE_VS_STEPSIZE_DESCEND	:
		{
			TestBTreeArrayIterNodeSizeVsStepSize<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_subnodeiter, _t_datalayerproperties> (true, rDataLayerProperties, sCacheDesc, 512, 3, 257, 1, 513);

			break;
		}

	case BTREETEST_ARRAY_ITER_BI_DIRECTIONAL_DEREFERENCE	:
		{
			TestBTreeArrayIterBiDirectionalDereference<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClArray, 128);

			break;
		}

	case BTREETEST_ARRAY_ITER_BI_DIRECTIONAL_SUBSCRIPTOR	:
		{
			TestBTreeArrayIterBiDirectionalSubScriptor<_t_obj, _t_objprim, _t_sizetype, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClArray, 128);

			break;
		}

	case BTREETEST_ARRAY_ITER_BI_DIRECTIONAL_SUBSCRIPTOR_INT	:
		{
			TestBTreeArrayIterBiDirectionalSubScriptor<_t_obj, _t_objprim, int, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClArray, 128);

			break;
		}

	case BTREETEST_ARRAY_ITER_COMPOUND	:
		{
			TestBTreeArrayIterCompound<_t_obj, _t_objprim, _t_sizetype, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClArray, 128);

			break;
		}

	case BTREETEST_ARRAY_ITER_COMPOUND_INT	:
		{
			TestBTreeArrayIterCompound<_t_obj, _t_objprim, int, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClArray, 128);

			break;
		}

	case BTREETEST_ARRAY_ITER_COMPOUND_ITER	:
		{
			TestBTreeArrayIterCompoundIter<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClArray, 128);

			break;
		}

	case BTREETEST_ARRAY_ITER_ARITHMETIC_OPERATORS	:
		{
			TestBTreeArrayIterArithmeticOperators<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClArray, 40);

			break;
		}

	case BTREETEST_ARRAY_ITER_COMPARE_OPERATORS	:
		{
			TestBTreeArrayIterCompareOperators<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClArray, 16);

			break;
		}

	case BTREETEST_ARRAY_ITER_SWAP	:
		{
			TestBTreeArrayIterSwap<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClArray, 128);

			break;
		}

	case BTREETEST_ARRAY_ITER_CONST_SWAP	:
		{
			TestBTreeArrayIterConstSwap<_t_obj, _t_objprim, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (pClArray, 16);

			break;
		}

	default									:
		{
			cerr << "ERROR: TestBTreeArrayIter: Unknown test or test not specified!" << endl;

			break;
		}
	}

	delete pClArray;
}
