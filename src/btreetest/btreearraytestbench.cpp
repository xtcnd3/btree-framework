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

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayBasic (_t_obj *pClArray, uint32_t nMaxEntries, uint32_t nVariation, uint32_t nRepetitions, btreetest_array_primitive_seek_e eGeneratorAdd, btreetest_array_primitive_seek_e eGeneratorRemove)
{
	uint32_t		ui32;
	_t_objprim		*pClArrayPrim;

	cout << "basic test adds data at ";

	switch (eGeneratorAdd)
	{
	case BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN		:
		{
			cout << "the beginning ";

			break;
		}

	case BTREETEST_ARRAY_PRIMITIVE_SEEK_END		:
		{
			cout << "the end ";

			break;
		}

	case BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM	:
		{
			cout << "a random position ";

			break;
		}

	default									:
		{
			cout << "? ";

			break;
		}
	}

	cout << "and removes it again at ";

	switch (eGeneratorRemove)
	{
	case BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN		:
		{
			cout << "the beginning" << endl;

			break;
		}

	case BTREETEST_ARRAY_PRIMITIVE_SEEK_END		:
		{
			cout << "the end" << endl;

			break;
		}

	case BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM	:
		{
			cout << "a random position" << endl;

			break;
		}

	default									:
		{
			cout << "?" << endl;

			break;
		}
	}

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nMaxEntries, eGeneratorAdd);

	for (ui32 = 0; ui32 < nRepetitions; ui32++)
	{
		cout << "cycle: " << ui32 + 1 << endl;

		arrayPrim_remove (pClArrayPrim, nVariation, eGeneratorRemove);

		arrayPrim_add (pClArrayPrim, nVariation, eGeneratorAdd);
	}

	arrayPrim_remove (pClArrayPrim, nMaxEntries, eGeneratorRemove);

	cout << "finished..." << endl;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayRemoveAll (_t_obj *pClArray, uint32_t nEntries)
{
	_t_objprim	*pClArrayPrim;

	cout << "test inserts data and empties the data container by calling clear" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	pClArray->clear ();
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayReplace (_t_obj *pClArray, uint32_t nInitEntries, uint32_t nInitReplace, uint32_t nAddEntries, uint32_t nAddReplace)
{
	_t_objprim	*pClArrayPrim;

	cout << "test replaces data" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nInitEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	arrayPrim_replace (pClArrayPrim, nAddEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	arrayPrim_add (pClArrayPrim, nAddEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	arrayPrim_replace (pClArrayPrim, nAddReplace, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayUnload (_t_obj *pClArray, uint32_t nInitEntries, uint32_t nAddEntries, uint32_t nRemoveEntries, uint32_t nExitEntries)
{
	_t_objprim	*pClArrayPrim;

	cout << "test uses unload after every operation" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	cout << "unloading..." << endl;

	pClArray->unload ();

	arrayPrim_add (pClArrayPrim, nInitEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	cout << "unloading..." << endl;
	
	pClArray->unload ();

	arrayPrim_add (pClArrayPrim, nAddEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	cout << "unloading..." << endl;
	
	pClArray->unload ();

	arrayPrim_remove (pClArrayPrim, nRemoveEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	cout << "unloading..." << endl;

	pClArray->unload ();

	arrayPrim_remove (pClArrayPrim, nExitEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	
	cout << "unloading..." << endl;
	
	pClArray->unload ();

	arrayPrim_add (pClArrayPrim, nInitEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	cout << "unloading..." << endl;
	
	pClArray->unload ();

	cout << "remove all" << endl;

	pClArray->clear ();

	cout << "unloading..." << endl;
	
	pClArray->unload ();
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void BTreeArrayMulti (_t_obj **ppClArrays, uint32_t nNumArray, uint32_t nAddEntries, uint32_t nAddMultiple, uint32_t nRemoveEntries, uint32_t nRemoveMultiple)
{
	uint32_t			ui32, uj32;
	bool				bError = false;
	_t_objprim			**pClArraysPrim;
	std::stringstream	aszArrayName;

	pClArraysPrim = new _t_objprim * [nNumArray];

	if (pClArraysPrim == NULL)
	{
		cerr << "ERROR: insufficient memory!" << endl;

		exit (-1);
	}

	for (ui32 = 0; ui32 < nNumArray; ui32++)
	{
		pClArraysPrim[ui32] = dynamic_cast <_t_objprim *> (ppClArrays[ui32]);
	}

	for (ui32 = 0; ui32 < nAddMultiple; ui32++)
	{
		for (uj32 = 0; uj32 < nNumArray; uj32++)
		{
			srand (ui32);

			arrayPrim_add (pClArraysPrim[uj32], nAddEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
		}

		for (uj32 = 1; uj32 < nNumArray; uj32++)
		{
			if ((* (ppClArrays[0])) != (* (ppClArrays[uj32])))
			{
				cerr << "ERROR: array %u mismatches master array" << endl;

				bError = true;
			}
		}

		if (bError)
		{
			for (uj32 = 0; uj32 < nNumArray; uj32++)
			{
				aszArrayName.clear ();
				aszArrayName << "array" << aszArrayName.width (2) << aszArrayName.fill ('0') << uj32 << ".html" << aszArrayName.width (0) << aszArrayName.fill (' ');

				cerr << "outputting " << aszArrayName.str().c_str() << endl;

				ppClArrays[ui32]->show_integrity (aszArrayName.str ().c_str ());
			}

			exit (-1);
		}
	}

	for (ui32 = 0; ui32 < nRemoveMultiple; ui32++)
	{
		for (uj32 = 0; uj32 < nNumArray; uj32++)
		{
			srand (ui32);

			arrayPrim_remove (pClArraysPrim[uj32], nRemoveEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
		}

		for (uj32 = 1; uj32 < nNumArray; uj32++)
		{
			if ((* (ppClArrays[0])) != (* (ppClArrays[uj32])))
			{
				cerr << "ERROR: array %u mismatches master array" << endl;

				bError = true;
			}
		}

		if (bError)
		{
			for (uj32 = 0; uj32 < nNumArray; uj32++)
			{
				aszArrayName.clear ();
				aszArrayName << "array" << aszArrayName.width (2) << aszArrayName.fill ('0') << uj32 << ".html" << aszArrayName.width (0) << aszArrayName.fill (' ');

				cerr << "outputting " << aszArrayName.str().c_str() << endl;

				ppClArrays[ui32]->show_integrity (aszArrayName.str ().c_str ());
			}

			exit (-1);
		}
	}

	delete [] pClArraysPrim;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayMultiNodeSizes (_t_obj *pClMaster, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nFromNodeSize, uint32_t nToNodeSize, uint32_t nAddEntries, uint32_t nAddMultiple, uint32_t nRemoveEntries, uint32_t nRemoveMultiple)
{
	uint32_t								ui32;
	uint32_t								nNumArray;
	_t_obj									**pClArrays;

	cout << "test exercises several instances using different node size settings" << endl;

	nNumArray = nToNodeSize - nFromNodeSize + 2;

	pClArrays = new _t_obj * [nNumArray];

	if (pClArrays == NULL)
	{
		cerr << "ERROR: insufficient memory!" << endl;

		exit (-1);
	}

	pClArrays[0] = pClMaster;
	
	for (ui32 = nFromNodeSize; ui32 <= nToNodeSize; ui32++)
	{
		pClArrays[ui32 - nFromNodeSize + 1] = new _t_obj (rDataLayerProperties, psCacheDescription, ui32);

		if (pClArrays[ui32 - nFromNodeSize + 1] == NULL)
		{
			cerr << "ERROR: insufficient memory!" << endl;

			exit (-1);
		}
	}

	BTreeArrayMulti<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArrays, nNumArray, nAddEntries, nAddMultiple, nRemoveEntries, nRemoveMultiple);

	for (ui32 = nFromNodeSize; ui32 <= nToNodeSize; ui32++)
	{
		delete pClArrays[ui32 - nFromNodeSize + 1];
	}
	delete [] pClArrays;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayMultiCacheSize (_t_obj *pClMaster, uint32_t nNumArray, uint32_t nNumMultiCacheSizes, bayerTreeCacheDescription_t *psMultiCacheDesc, _t_datalayerproperties **ppMultiCacheSizeDataLayerProperties, uint32_t nNodeSize, uint32_t nAddEntries, uint32_t nAddMultiple, uint32_t nRemoveEntries, uint32_t nRemoveMultiple)
{
	uint32_t						ui32;
	_t_obj							**pClArrays;

	cout << "test exercises several instances using different cache settings" << endl;

	pClArrays = new _t_obj * [nNumMultiCacheSizes + 1];

	if (pClArrays == NULL)
	{
		cerr << "ERROR: insufficient memory!" << endl;

		exit (-1);
	}

	pClArrays[0] = pClMaster;
	
	for (ui32 = 0; ui32 < nNumMultiCacheSizes; ui32++)
	{
		pClArrays[ui32 + 1] = new _t_obj (*(ppMultiCacheSizeDataLayerProperties[ui32]), &(psMultiCacheDesc[ui32]), nNodeSize);

		if (pClArrays[ui32 + 1] == NULL)
		{
			cerr << "ERROR: insufficient memory!" << endl;

			exit (-1);
		}
	}

	BTreeArrayMulti<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArrays, nNumMultiCacheSizes + 1, nAddEntries, nAddMultiple, nRemoveEntries, nRemoveMultiple);

	for (ui32 = 0; ui32 < nNumMultiCacheSizes; ui32++)
	{
		delete pClArrays[ui32 + 1];
	}

	delete [] pClArrays;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayMultiTemplateParams (_t_obj *pClMaster, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nNumMultiTemplateParams, _t_objprim **ppClArraysPrim, uint32_t nNodeSize, uint32_t nAddEntries, uint32_t nAddMultiple, uint32_t nRemoveEntries, uint32_t nRemoveMultiple)
{
	const uint32_t	nNumArray = nNumMultiTemplateParams + 1;
	uint32_t		ui32;
	uint32_t		uj32;
	bool			bError = false;
	_t_objprim		**ppClArraysPrimAll;

	cout << "test exercises several instances using different template parameter settings" << endl;

	ppClArraysPrimAll = new _t_objprim * [nNumArray];

	if (ppClArraysPrimAll == NULL)
	{
		cerr << "ERROR: insufficient memory!";

		exit (-1);
	}

	ppClArraysPrimAll[0] = dynamic_cast <_t_objprim *> (pClMaster);

	for (ui32 = 0; ui32 < nNumMultiTemplateParams; ui32++)
	{
		ppClArraysPrimAll[ui32 + 1] = ppClArraysPrim[ui32];
	}

	for (ui32 = 0; ui32 < nAddMultiple; ui32++)
	{
		for (uj32 = 0; uj32 < nNumArray; uj32++)
		{
			srand (ui32);

			arrayPrim_add (ppClArraysPrimAll[uj32], nAddEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
		}

		for (uj32 = 1; uj32 < nNumArray; uj32++)
		{
			if (!arrayPrim_compare (ppClArraysPrimAll[0], ppClArraysPrimAll[uj32]))
			{
				cerr << "ERROR: array " << uj32 << " mismatches master array!" << endl;

				bError = true;
			}
		}

		if (bError)
		{
			for (ui32 = 0; ui32 < nNumArray; ui32++)
			{
				std::stringstream	strFileName;
				_t_obj			*pClArray;

				pClArray = dynamic_cast <_t_obj *> (ppClArraysPrimAll[ui32]);

				strFileName << "array" << ui32 << ".html";

				pClArray->show_integrity (strFileName.str ().c_str ());
			}

			break;
		}
	}

	if (!bError)
	{
		for (ui32 = 0; ui32 < nRemoveMultiple; ui32++)
		{
			for (uj32 = 0; uj32 < nNumArray; uj32++)
			{
				srand (ui32);

				arrayPrim_remove (ppClArraysPrimAll[uj32], nRemoveEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
			}

			for (uj32 = 1; uj32 < nNumArray; uj32++)
			{
				if (!arrayPrim_compare (ppClArraysPrimAll[0], ppClArraysPrimAll[uj32]))
				{
					cerr << "ERROR: array " << uj32 << " mismatches master array!" << endl;

					bError = true;
				}
			}

			if (bError)
			{
				for (ui32 = 0; ui32 < nNumArray; ui32++)
				{
					std::stringstream	strFileName;
					_t_obj			*pClArray;

					pClArray = dynamic_cast <_t_obj *> (ppClArraysPrimAll[ui32]);

					strFileName << "array" << ui32 << ".html";

					pClArray->show_integrity (strFileName.str ().c_str ());
				}

				break;
			}
		}
	}

	delete [] ppClArraysPrimAll;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayHTMLoutput (_t_obj *pClArray, uint32_t nEntries)
{
	_t_objprim	*pClArrayPrim;

	cout << "tests if instance is left in undefined state after HTML output" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	pClArray->show_integrity ("test_array_output_0000.html");

	arrayPrim_add (pClArrayPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	pClArray->show_integrity ("test_array_output_0001.html");

	arrayPrim_add (pClArrayPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayCopyConstructorTest (_t_obj *pClRef, _t_obj sClCopy)
{
	if (sClCopy != *pClRef)
	{
		cerr << endl << "ERROR: copied instance mismatches reference!" << endl;
		cerr << "outputting reference to cc_reference.html" << endl;

		pClRef->show_integrity ("cc_reference.html");

		cerr << "outputting copied instance to cc_copy.html" << endl;

		sClCopy.show_integrity ("cc_copy.html");

		exit (-1);
	}
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayCopyConstructor (_t_obj *pClArray, uint32_t nEntries)
{
	_t_objprim	*pClArrayPrim;

	cout << "exercises array type copy constructor" << endl;

	TestBTreeArrayCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, *pClArray);

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	TestBTreeArrayCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, *pClArray);
	
	arrayPrim_add (pClArrayPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	TestBTreeArrayCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, *pClArray);
	
	arrayPrim_add (pClArrayPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	TestBTreeArrayCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, *pClArray);

	pClArray->unload ();

	TestBTreeArrayCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, *pClArray);

	arrayPrim_remove (pClArrayPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	TestBTreeArrayCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, *pClArray);

	arrayPrim_remove (pClArrayPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	TestBTreeArrayCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, *pClArray);

	pClArray->clear ();

	TestBTreeArrayCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, *pClArray);
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayOperatorOverloadAssign (_t_obj *pClArray, uint32_t nEntries)
{
	_t_objprim			*pClArrayPrim;
	_t_obj				*pClTarget;
	_t_objprim			*pClTargetPrim;

	cout << "exercises array type operator=" << endl;

	pClTarget = new _t_obj (*pClArray);

	if (pClTarget == NULL)
	{
		cerr << "ERROR: Insufficient memory!" << endl;

		exit (-1);
	}

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);
	pClTargetPrim = dynamic_cast <_t_objprim *> (pClTarget);

	*pClTarget = *pClArray;

	arrayPrim_compare (pClTargetPrim, pClArrayPrim);
	
	arrayPrim_add (pClArrayPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	*pClTarget = *pClArray;
	
	arrayPrim_add (pClArrayPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	*pClTarget = *pClArray;
	
	arrayPrim_add (pClArrayPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	*pClTarget = *pClArray;
	
	pClArray->unload ();

	*pClTarget = *pClArray;
	
	arrayPrim_remove (pClArrayPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	*pClTarget = *pClArray;
	
	arrayPrim_remove (pClArrayPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	*pClTarget = *pClArray;
	
	pClArray->clear ();

	*pClTarget = *pClArray;
	
	delete pClTarget;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayOperatorOverloadSubScript (_t_obj *pClArray, uint32_t nEntries)
{
	uint32_t			ui32;
	_t_objprim			*pClArrayPrim;
	_t_obj				*pClTarget;
	_t_objprim			*pClTargetPrim;
	arrayEntry_t		sData;

	cout << "exercises array type operator[]" << endl;

	pClTarget = new _t_obj (*pClArray);

	if (pClTarget == NULL)
	{
		cerr << "ERROR: Insufficient memory!" << endl;

		exit (-1);
	}

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);
	pClTargetPrim = dynamic_cast <_t_objprim *> (pClTarget);
	
	arrayPrim_add (pClArrayPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	arrayPrim_add (pClTargetPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN);

	for (ui32 = 0; ui32 < nEntries; ui32++)
	{
		sData = (*pClArray)[ui32];

		(*pClTarget)[ui32] = sData;
	}

	arrayPrim_compare (pClTargetPrim, pClArrayPrim);

	pClTarget->clear ();

	arrayPrim_add (pClTargetPrim, nEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN);

	for (ui32 = 0; ui32 < nEntries; ui32++)
	{
		(*pClTarget)[ui32] = (*pClArray)[ui32];
	}

	arrayPrim_compare (pClTargetPrim, pClArrayPrim);

	delete pClTarget;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArraySerialize (_t_obj *pClArray, uint32_t nSize, uint32_t nWindowSize)
{
	uint32_t			nLastKey = 0;
	uint32_t			ui32, uj32;
	uint64_t			nRslt;
	arrayEntry_t		*pnData;
	_t_objprim			*pClArrayPrim;
	arrayEntry_t		sData;
	
	cout << "exercises serialize method" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	pnData = new arrayEntry_t [nWindowSize];

	if (pnData == NULL)
	{
		cerr << "ERROR: TestBTreeArraySerialize: insufficient memory!" << endl;

		exit (-1);
	}
	
	arrayPrim_add (pClArrayPrim, nSize, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
	
	for (ui32 = 0; ui32 < (nSize - nWindowSize + 1); ui32++)
	{
		nRslt = pClArray->serialize (ui32, nWindowSize, pnData);

		cout << "testing from " << ui32 << " to " << ui32 + nWindowSize << endl;

		if ((uint32_t) nRslt != nWindowSize)
		{
			cerr << "ERROR: TestBTreeArraySerialize: size mismatch!" << endl;
			cerr << "iteration: " << ui32 << endl;
			cerr << "window size: " << nWindowSize << endl;
			cerr << "returned size: " << nRslt << endl;

			exit (-1);
		}

		for (uj32 = 0; uj32 < nWindowSize; uj32++)
		{
			sData = pClArray->at (ui32 + uj32);

			if (memcmp ((void *) &(pnData[uj32]), (void *) &sData, sizeof (sData)) != 0)
			{
				cerr << "ERROR: TestBTreeArraySerialize: data mismatch!" << endl;
				cerr << "iteration: " << ui32 << endl;
				cerr << "position: " << uj32 << endl;
				cerr << "data: " << pnData[uj32].nData << " " << sData.nData << endl;
				cerr << "debug: " << pnData[uj32].nDebug << " " << sData.nDebug << endl;

				exit (-1);
			}
		}
	}

	delete [] pnData;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArraySerializeIncomplete (_t_obj *pClArray, uint32_t nSize, uint32_t nWindowSize, uint32_t nOffset)
{
	uint64_t			nRslt;
	arrayEntry_t		*pnData;
	_t_objprim			*pClArrayPrim;
	
	cout << "exercises serialize method with an incomplete transfer" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	pnData = new arrayEntry_t [nWindowSize];

	if (pnData == NULL)
	{
		cerr << "ERROR: TestBTreeArraySerializeIncomplete: insufficient memory!" << endl;

		exit (-1);
	}
	
	arrayPrim_add (pClArrayPrim, nSize, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	cout << "testing from " << nOffset << " to " << nWindowSize + nOffset << endl;

	nRslt = pClArray->serialize (nOffset, nWindowSize, pnData);

	if ((uint32_t) nRslt != (nSize - nOffset))
	{
		cerr << "ERROR: TestBTreeArraySerializeIncomplete: size mismatch!" << endl;
		cerr << "expected size: " << nSize - nOffset << endl;
		cerr << "returned size: " << nRslt << endl;

		exit (-1);
	}

	delete [] pnData;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayCodeCoverageRootMergeWithLowerRight (_t_obj *pClArray, uint32_t nNodeSize)
{
	uint32_t			nSize = nNodeSize * 2 - 1;
	_t_objprim			*pClArrayPrim;
	
	cout << "exercises CBTreeBaseDefaults::merge_node with root node having only one item left, while the right has the lower ID" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	nSize *= nNodeSize * 2;
	nSize *= nNodeSize * 2;

	if ((65 * (nNodeSize * 2 - 1)) > nSize)
	{
		nSize = 65 * (nNodeSize * 2 - 1);
	}

	arrayPrim_add (pClArrayPrim, nSize, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	arrayPrim_remove (pClArrayPrim, nSize - 1, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	arrayPrim_add (pClArrayPrim, nNodeSize * 2, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	arrayPrim_remove (pClArrayPrim, nNodeSize * 2, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArraySelfReferenceViaSubScriptWrapper (_t_obj *pClArray, uint32_t nNumEntries)
{
	uint32_t			i;
	uint32_t			ri;
	uint32_t			nSize;
	_t_objprim			*pClArrayPrim;
	arrayEntry_t		sEntry;
	
	cout << "exercises CBTreeBaseDefaults<>::CBTreeArray<>::CBTreeArrayAccessWrapper<>::operator= (const CBTreeArrayAccessWrapper<> &) using its own input as reference" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

	nSize = (uint32_t) pClArray->size ();

	for (i = 0; i < nSize; i++)
	{
		sEntry = pClArray->at (i);

		if (sEntry.nDebug != i)
		{
			cerr << "ERROR: initialisation created unexpected debug sequence!" << endl;

			exit (-1);
		}
	}

	for (i = 0, ri = nSize; i < (nSize / 2); i++)
	{
		ri--;

		sEntry = (*pClArray)[i];
		(*pClArray)[i] = (*pClArray)[ri];
		(*pClArray)[ri] = sEntry;
	}

	for (i = 0, ri = nSize; i < nSize; i++)
	{
		ri--;

		sEntry = pClArray->at (i);

		if (sEntry.nDebug != ri)
		{
			cerr << "ERROR: debug sequence mismatch!" << endl;

			exit (-1);
		}
	}
}

template <class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayInTrinsicDataType (uint32_t nNodeSize, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t &sCacheDesc)
{
	typedef typename CBTreeArray<int, uint64_t, uint64_t, uint32_t, _t_datalayerproperties, _t_datalayer>::const_iterator		citer_t;

	CBTreeArray<int, uint64_t, uint64_t, uint32_t, _t_datalayerproperties, _t_datalayer>	sClArrayInt (rDataLayerProperties, &sCacheDesc, nNodeSize);
	citer_t																					sCIter;

	sCIter = sClArrayInt.cbegin ();

	sClArrayInt.insert (sCIter, 0);

	sCIter++;

	sClArrayInt.insert (sCIter, 1);

	cout << "test if indirect read access with intrinsic types is possible!" << endl;

	if (sClArrayInt.at (0) != sClArrayInt.at (1))
	{
		cout << "success!" << endl;
	}
	else
	{
		cerr << "failure!" << endl;
	}

	int		nVal = 1;

	cout << "test if indirect write access with intrinsic types is possible!" << endl;

	sClArrayInt.at (0) = nVal;
	sClArrayInt.at (1) = nVal;

	if (sClArrayInt.at (0) == sClArrayInt.at (1))
	{
		cout << "success!" << endl;
	}
	else
	{
		cerr << "failure!" << endl;
	}
}

template <class _t_obj, class _t_objprim>
void TestBTreeArraySTLifAtFrontBack (_t_obj *pClArray, uint32_t nNumEntries)
{
	uint32_t			i;
	uint32_t			nSize;
	_t_objprim			*pClArrayPrim;
	arrayEntry_t		sEntry;
	uint32_t			nRef;
	
	cout << "exercises methods compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: at() front() back()" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

	nSize = (uint32_t) pClArray->size ();

	for (i = 0; i < nSize; i++)
	{
		sEntry = pClArray->at (i);

		if (i != sEntry.nDebug)
		{
			cerr << "TestBTreeArraySTLifAtFrontBack: ERROR: unexpected debug sequence!" << i << " " << sEntry.nDebug << endl;

			exit (-1);
		}
	}

	for (i = 0; i < nSize; i++)
	{
		sEntry = pClArray->at (i);

		sEntry.nDebug = nSize - 1 - i;

		pClArray->at (i) = sEntry;
	}

	if (((arrayEntry_t) pClArray->front ()).nDebug != (nSize - 1))
	{
		cerr << "TestBTreeArraySTLifAtFrontBack: ERROR: front() read error!" << endl;

		exit (-1);
	}

	if (((arrayEntry_t) pClArray->back ()).nDebug != 0)
	{
		cerr << "TestBTreeArraySTLifAtFrontBack: ERROR: back() read error!" << endl;

		exit (-1);
	}

	sEntry = pClArray->front ();

	sEntry.nDebug = ~sEntry.nDebug;

	pClArray->front () = sEntry;

	sEntry = pClArray->back ();

	sEntry.nDebug = ~sEntry.nDebug;

	pClArray->back () = sEntry;

	for (i = 0; i < nSize; i++)
	{
		if (i == 0)
		{
			nRef = ~(nSize - 1);
		}
		else if (i == (nSize - 1))
		{
			nRef = ~0;
		}
		else
		{
			nRef = nSize - 1 - i;
		}

		sEntry = pClArray->at (i);

		if (nRef != sEntry.nDebug)
		{
			cerr << "TestBTreeArraySTLifAtFrontBack: ERROR: unexpected reference!" << endl;

			exit (-1);
		}
	}
}

template <class _t_obj>
void TestBTreeArraySTLifAssignFill (_t_obj *pClArray, uint32_t nNumEntries)
{
	arrayEntry_t		sEntry;
	
	cout << "exercises methods compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: assign(_t_sizetype, const _t_data &)" << endl;

	sEntry.nDebug = 0;
	sEntry.nData = ~0x0U;

	pClArray->assign (nNumEntries, sEntry);
}

template <class _t_obj, class _t_objprim, class _t_container, class _t_iterator, class _t_ext_iterator>
void TestBTreeArraySTLifAssignViaIteratorEx (btreetest_array_stl_if_assign_via_iterator_e eTest, const char *pszTestTitle, _t_obj *pClTargetArray, _t_objprim *pClArrayPrim, _t_container *pContainer, _t_iterator &rIterA, _t_iterator &rIterB, _t_ext_iterator &rExtIterA, _t_ext_iterator &rExtIterB, uint32_t nNumEntries)
{
	_t_iterator			sIterBegin;
	_t_iterator			sIterEnd;
	_t_ext_iterator		sExtIterBegin;
	_t_ext_iterator		sExtIterEnd;

	cout << pszTestTitle << endl;

	arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

	get_begin (pClArrayPrim, sIterBegin);
	get_end (pClArrayPrim, sIterEnd);

	switch (eTest)
	{
		case BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL	:
		{
			pClTargetArray->assign (sIterBegin, sIterEnd);

			break;
		}

		case BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN:
		{
			pContainer->assign (sIterBegin, sIterEnd);

			get_begin (pContainer, sExtIterBegin);
			get_end (pContainer, sExtIterEnd);

			pClTargetArray->assign (sExtIterBegin, sExtIterEnd);

			break;
		}

		case BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_SELF_REFERENCE:
		{
			pClTargetArray->assign (sIterBegin, sIterEnd);

			get_begin (pClTargetArray, rIterA);
			get_end (pClTargetArray, rIterB);

			pClTargetArray->assign (rIterA, rIterB);

			break;
		}

		case BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_EQUIVALENT_OR_SAME:
		{
			get_end (pClArrayPrim, rIterA);
			get_end (pClArrayPrim, rIterB);

			pClTargetArray->assign (rIterA, rIterB);

			if (!pClTargetArray->empty ())
			{
				::std::cerr << "TestBTreeArraySTLifAssignViaIteratorEx: equivalent or same test: test 1 - container was not empty!" << ::std::endl;

				exit (-1);
			}

			pClTargetArray->assign (rIterB, rIterA);
		
			if (!pClTargetArray->empty ())
			{
				::std::cerr << "TestBTreeArraySTLifAssignViaIteratorEx: equivalent or same test: test 2 - container was not empty!" << ::std::endl;

				exit (-1);
			}

			get_begin (pClArrayPrim, rIterA);
			get_begin (pClArrayPrim, rIterB);

			pClTargetArray->assign (rIterA, rIterB);

			if (!pClTargetArray->empty ())
			{
				::std::cerr << "TestBTreeArraySTLifAssignViaIteratorEx: equivalent or same test: test 3 - container was not empty!" << ::std::endl;

				exit (-1);
			}

			pClTargetArray->assign (rIterB, rIterA);
		
			if (!pClTargetArray->empty ())
			{
				::std::cerr << "TestBTreeArraySTLifAssignViaIteratorEx: equivalent or same test: test 4 - container was not empty!" << ::std::endl;

				exit (-1);
			}

			while (rIterA != sIterEnd)
			{
				pClTargetArray->assign (rIterA, rIterB);
		
				rIterA++;
				rIterB++;

				if (!pClTargetArray->empty ())
				{
					::std::cerr << "TestBTreeArraySTLifAssignViaIteratorEx: equivalent or same test: test 5 - container was not empty!" << ::std::endl;

					exit (-1);
				}
			}

			get_begin (pClTargetArray, rIterA);
			get_end (pClTargetArray, rIterB);
		
			pClTargetArray->assign (rIterA, rIterB);

			if (!pClTargetArray->empty ())
			{
				::std::cerr << "TestBTreeArraySTLifAssignViaIteratorEx: equivalent or same test: test 6 - container was not empty!" << ::std::endl;

				exit (-1);
			}

			break;
		}

		case BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_PART_SELF_REFERENCE:
		{
			pClTargetArray->assign (sIterBegin, sIterEnd);

			get_begin (pClTargetArray, rIterA);
			get_end (pClTargetArray, rIterB);

			rIterA += (int) (nNumEntries / 4);
			rIterB -= (int) (nNumEntries / 4);

			pClTargetArray->assign (rIterA, rIterB);

			break;
		}

		default:
		{
			::std::cerr << "TestBTreeArraySTLifAssignViaIteratorEx: unknown test selected!" << ::std::endl;

			exit (-1);

			break;
		}
	}

	pClArrayPrim->clear ();
}

template <class _t_obj, class _t_objprim>
void TestBTreeArraySTLifAssignViaIterator (_t_obj *pClArray, uint32_t nNumEntries)
{
	typedef ::std::list<arrayEntry_t>					list_t;
	typedef ::std::vector<arrayEntry_t>					vector_t;

	typedef typename list_t::iterator					itlist_t;
	typedef typename list_t::const_iterator				citlist_t;
	typedef typename list_t::reverse_iterator			ritlist_t;
	typedef typename list_t::const_reverse_iterator		critlist_t;

	typedef typename vector_t::iterator					itvector_t;
	typedef typename vector_t::const_iterator			citvector_t;
	typedef typename vector_t::reverse_iterator			ritvector_t;
	typedef typename vector_t::const_reverse_iterator	critvector_t;

	typedef typename _t_objprim::iterator				iter_t;
	typedef typename _t_objprim::const_iterator			citer_t;
	typedef typename _t_objprim::reverse_iterator		riter_t;
	typedef typename _t_objprim::const_reverse_iterator	criter_t;

	_t_obj							*pClTargetArray;
	_t_objprim						*pClArrayPrim;
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
	
	cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: assign<_t_iterator>(_t_iterator, _t_iterator)" << endl;

	pClTargetArray = new _t_obj (*pClArray);

	if (pClTargetArray == NULL)
	{
		cerr << "TestBTreeArraySTLifAssignViaIterator: ERROR: insufficient mempry!" << endl;

		exit (-1);
	}

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL, "target::assign<_t_obj::iter_t> (iter_t, iter_t)", pClTargetArray, pClArrayPrim, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL, "target::assign<_t_obj::citer_t> (citer_t, citer_t)", pClTargetArray, pClArrayPrim, &sList, sCIterA, sCIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL, "target::assign<_t_obj::riter_t> (riter_t, riter_t)", pClTargetArray, pClArrayPrim, &sList, sRIterA, sRIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL, "target::assign<_t_obj::criter_t> (criter_t, criter_t)", pClTargetArray, pClArrayPrim, &sList, sCRIterA, sCRIterB, sItListBegin, sItListEnd, nNumEntries);

	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<list::iter> (list::iter, list::iter)", pClTargetArray, pClArrayPrim, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<list::citer> (list::citer, list::citer)", pClTargetArray, pClArrayPrim, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<list::riter> (list::riter, list::riter)", pClTargetArray, pClArrayPrim, &sList, sRIterA, sRIterB, sRItListBegin, sRItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<list::criter> (list::criter, list::criter)", pClTargetArray, pClArrayPrim, &sList, sCRIterA, sCRIterB, sCRItListBegin, sCRItListEnd, nNumEntries);

	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<vector::iter> (vector::iter, vector::iter)", pClTargetArray, pClArrayPrim, &sVector, sIterA, sIterB, sItVecBegin, sItVecEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<vector::citer> (vector::citer, vector::citer)", pClTargetArray, pClArrayPrim, &sVector, sCIterA, sCIterB, sCItVecBegin, sCItVecEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<vector::riter> (vector::riter, vector::riter)", pClTargetArray, pClArrayPrim, &sVector, sRIterA, sRIterB, sRItVecBegin, sRItVecEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_FULL_EXTERN, "target::assign<vector::criter> (vector::criter, vector::criter)", pClTargetArray, pClArrayPrim, &sVector, sCRIterA, sCRIterB, sCRItVecBegin, sCRItVecEnd, nNumEntries);

	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_SELF_REFERENCE, "target::assign<_t_obj::iter_t> (target::iter_t, target::iter_t)", pClTargetArray, pClArrayPrim, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_SELF_REFERENCE, "target::assign<_t_obj::citer_t> (target::citer_t, target::citer_t)", pClTargetArray, pClArrayPrim, &sList, sCIterA, sCIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_SELF_REFERENCE, "target::assign<_t_obj::riter_t> (target::riter_t, target::riter_t)", pClTargetArray, pClArrayPrim, &sList, sRIterA, sRIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_SELF_REFERENCE, "target::assign<_t_obj::criter_t> (target::criter_t, target::criter_t)", pClTargetArray, pClArrayPrim, &sList, sCRIterA, sCRIterB, sItListBegin, sItListEnd, nNumEntries);

	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_EQUIVALENT_OR_SAME, "target::assign<_t_obj::iter_t> (iter_t == iter_t)", pClTargetArray, pClArrayPrim, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_EQUIVALENT_OR_SAME, "target::assign<_t_obj::citer_t> (citer_t == citer_t)", pClTargetArray, pClArrayPrim, &sList, sCIterA, sCIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_EQUIVALENT_OR_SAME, "target::assign<_t_obj::riter_t> (riter_t == riter_t)", pClTargetArray, pClArrayPrim, &sList, sRIterA, sRIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_EQUIVALENT_OR_SAME, "target::assign<_t_obj::criter_t> (criter_t == criter_t)", pClTargetArray, pClArrayPrim, &sList, sCRIterA, sCRIterB, sItListBegin, sItListEnd, nNumEntries);

	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_PART_SELF_REFERENCE, "target::assign<_t_obj::iter_t> (>target::iter_t, target::iter_t<)", pClTargetArray, pClArrayPrim, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_PART_SELF_REFERENCE, "target::assign<_t_obj::citer_t> (>target::citer_t, target::citer_t<)", pClTargetArray, pClArrayPrim, &sList, sCIterA, sCIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_PART_SELF_REFERENCE, "target::assign<_t_obj::riter_t> (>target::riter_t, target::riter_t<)", pClTargetArray, pClArrayPrim, &sList, sRIterA, sRIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifAssignViaIteratorEx (BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR_PART_SELF_REFERENCE, "target::assign<_t_obj::criter_t> (>target::criter_t, target::criter_t<)", pClTargetArray, pClArrayPrim, &sList, sCRIterA, sCRIterB, sItListBegin, sItListEnd, nNumEntries);

	delete pClTargetArray;
}

template <class _t_obj>
void TestBTreeArraySTLifPushBackPopBack (_t_obj *pClArray, uint32_t nNumEntries)
{
	uint32_t		i;
	arrayEntry_t	sEntry;

	cout << "exercises methods compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: push_back (const _t_data &) pop_back ()" << endl;

	cout << "adding via push_back" << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sEntry.nDebug = i;
		sEntry.nData = generate_rand32 ();

		pClArray->push_back (sEntry);
	}

	cout << "removing via pop_back" << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		pClArray->pop_back ();
	}
}

template <class _t_obj, class _t_objprim>
void TestBTreeArraySTLifInsertSingle (_t_obj *pClArray, uint32_t nNumEntries)
{
	typedef typename _t_obj::iterator				iter_t;
	typedef typename _t_obj::const_iterator			citer_t;
	
	uint32_t			i;
	_t_objprim			*pClArrayPrim;
	arrayEntry_t		sEntry;
	citer_t				sCIter;
	
	cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: insert (const_iterator, const _t_data &)" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	for (i = 0; i < nNumEntries; i++)
	{
		sCIter = pClArray->cbegin ();

		sCIter += (int) (generate_rand32 () % (uint32_t) pClArray->size ());

		sEntry.nDebug = (uint32_t) pClArray->size ();
		sEntry.nData = generate_rand32 ();

		pClArray->insert (sCIter, sEntry);
	}
}

template <class _t_obj, class _t_objprim>
void TestBTreeArraySTLifInsertMultiple (_t_obj *pClArray, uint32_t nNumEntries, uint32_t nLen)
{
	typedef typename _t_obj::iterator				iter_t;
	typedef typename _t_obj::const_iterator			citer_t;
	
	uint32_t			i;
	_t_objprim			*pClArrayPrim;
	arrayEntry_t		sEntry;
	citer_t				sCIter;
	
	cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: insert (const_iterator, const _t_sizetype, const _t_data &)" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	for (i = 0; i < nNumEntries; i++)
	{
		sCIter = pClArray->cbegin ();

		sCIter += (int) (generate_rand32 () % (uint32_t) pClArray->size ());

		sEntry.nDebug = (uint32_t) pClArray->size ();
		sEntry.nData = generate_rand32 ();

		pClArray->insert (sCIter, nLen, sEntry);
	}
}

template <class _t_obj, class _t_objprim, class _t_container, class _t_iterator, class _t_ext_iterator>
void TestBTreeArraySTLifInsertViaIteratorEx (btreetest_array_stl_if_insert_via_iterator_e eTest, const char *pszTestTitel, _t_objprim *pClArrayPrim, _t_obj *pClTargetArray, _t_container *pContainer, _t_iterator &rIterA, _t_iterator &rIterB, _t_ext_iterator &rExtIterA, _t_ext_iterator &rExtIterB, uint32_t nNumEntries)
{
	typedef typename _t_obj::const_iterator			citer_t;
	typedef typename _t_objprim::size_type			size_type;

	citer_t			sCIterHalf;
	_t_iterator		sIterBegin;
	_t_iterator		sIterEnd;
	_t_ext_iterator	sExtIterBegin;
	_t_ext_iterator	sExtIterEnd;
	size_type		nSize;

	cout << pszTestTitel << endl;

	arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

	get_begin (pClArrayPrim, sIterBegin);
	get_end (pClArrayPrim, sIterEnd);

	switch (eTest)
	{
		case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY	:
		{
			pClTargetArray->template assign<_t_iterator> (sIterBegin, sIterEnd);

			pClArrayPrim->clear ();
		
			arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);
		
			sCIterHalf = pClTargetArray->cbegin ();
		
			sCIterHalf += pClTargetArray->size () / 2;

			get_begin (pClArrayPrim, sIterBegin);
			get_end (pClArrayPrim, sIterEnd);
		
			pClTargetArray->template insert<_t_iterator> (sCIterHalf, sIterBegin, sIterEnd);

			break;
		}

		case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN:
		{
			pClTargetArray->template assign<_t_iterator> (sIterBegin, sIterEnd);

			pClArrayPrim->clear ();
		
			arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

			get_begin (pClArrayPrim, sIterBegin);
			get_end (pClArrayPrim, sIterEnd);
		
			pContainer->template assign<_t_iterator> (sIterBegin, sIterEnd);
		
			sCIterHalf = pClTargetArray->cbegin ();
		
			sCIterHalf += pClTargetArray->size () / 2;

			get_begin (pContainer, sExtIterBegin);
			get_end (pContainer, sExtIterEnd);

			pClTargetArray->template insert<_t_ext_iterator> (sCIterHalf, sExtIterBegin, sExtIterEnd);

			break;
		}

		case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE:
		{
			pClTargetArray->template assign<_t_iterator> (sIterBegin, sIterEnd);
		
			sCIterHalf = pClTargetArray->cbegin ();
		
			sCIterHalf += pClTargetArray->size () / 2;
		
			get_begin (pClTargetArray, sIterBegin);
			get_end (pClTargetArray, sIterEnd);
		
			pClTargetArray->template insert<_t_iterator> (sCIterHalf, sIterBegin, sIterEnd);

			break;
		}

		case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_INEFFECTIVE:
		{
			pClTargetArray->template assign<_t_iterator> (sIterBegin, sIterEnd);

			pClArrayPrim->clear ();

			arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

			get_end (pClArrayPrim, rIterA);
			get_end (pClArrayPrim, rIterB);

			get_begin (pClArrayPrim, sIterBegin);
			get_end (pClArrayPrim, sIterEnd);

			citer_t			sCIter;

			nSize = pClTargetArray->size ();

			for (sCIter = pClArrayPrim->cbegin (); sCIter < pClArrayPrim->cend (); sCIter++)
			{
				pClTargetArray->template insert<_t_iterator> (sCIter, rIterA, rIterB);
			}

			if (pClTargetArray->size () != nSize)
			{
				::std::cerr << "TestBTreeArraySTLifInsertViaIterator: ineffective test: test 1 - container size changed!" << ::std::endl;

				exit (-1);
			}

			for (sCIter = pClArrayPrim->cbegin (); sCIter < pClArrayPrim->cend (); sCIter++)
			{
				pClTargetArray->template insert<_t_iterator> (sCIter, rIterB, rIterA);
			}
		
			if (pClTargetArray->size () != nSize)
			{
				::std::cerr << "TestBTreeArraySTLifInsertViaIterator: ineffective test: test 2 - container size changed!" << ::std::endl;

				exit (-1);
			}

			get_begin (pClArrayPrim, rIterA);
			get_begin (pClArrayPrim, rIterB);

			for (sCIter = pClArrayPrim->cbegin (); sCIter < pClArrayPrim->cend (); sCIter++)
			{
				pClTargetArray->template insert<_t_iterator> (sCIter, rIterA, rIterB);
			}

			if (pClTargetArray->size () != nSize)
			{
				::std::cerr << "TestBTreeArraySTLifInsertViaIterator: ineffective test: test 3 - container size changed!" << ::std::endl;

				exit (-1);
			}

			for (sCIter = pClArrayPrim->cbegin (); sCIter < pClArrayPrim->cend (); sCIter++)
			{
				pClTargetArray->template insert<_t_iterator> (sCIter, rIterB, rIterA);
			}
		
			if (pClTargetArray->size () != nSize)
			{
				::std::cerr << "TestBTreeArraySTLifInsertViaIterator: ineffective test: test 4 - container size changed!" << ::std::endl;

				exit (-1);
			}

			while (rIterA != sIterEnd)
			{
				for (sCIter = pClArrayPrim->cbegin (); sCIter < pClArrayPrim->cend (); sCIter++)
				{
					pClTargetArray->template insert<_t_iterator> (sCIter, rIterA, rIterB);
				}
		
				rIterA++;
				rIterB++;

				if (pClTargetArray->size () != nSize)
				{
					::std::cerr << "TestBTreeArraySTLifInsertViaIterator: ineffective test: test 5 - container size changed!" << ::std::endl;

					exit (-1);
				}
			}

			break;
		}

		case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_PART_SELF_REFERENCE:
		{
			pClTargetArray->template assign<_t_iterator> (sIterBegin, sIterEnd);
		
			sCIterHalf = pClTargetArray->cbegin ();
		
			sCIterHalf += pClTargetArray->size () / 2;
		
			get_begin (pClTargetArray, sIterBegin);
			get_end (pClTargetArray, sIterEnd);

			sIterBegin += (int) (nNumEntries / 4);
			sIterEnd -= (int) (nNumEntries / 4);
		
			pClTargetArray->template insert<_t_iterator> (sCIterHalf, sIterBegin, sIterEnd);

			break;
		}

		default:
		{
			cerr << "TestBTreeArraySTLifInsertViaIterator: unknown test!" << endl;

			exit (-1);

			break;
		}
	}

	pClArrayPrim->clear ();
}

template <class _t_obj, class _t_objprim>
void TestBTreeArraySTLifInsertViaIterator (_t_obj *pClArray, uint32_t nNumEntries)
{
	typedef ::std::list<arrayEntry_t>					list_t;
	typedef ::std::vector<arrayEntry_t>					vector_t;

	typedef typename list_t::iterator					itlist_t;
	typedef typename list_t::const_iterator				citlist_t;
	typedef typename list_t::reverse_iterator			ritlist_t;
	typedef typename list_t::const_reverse_iterator		critlist_t;

	typedef typename vector_t::iterator					itvector_t;
	typedef typename vector_t::const_iterator			citvector_t;
	typedef typename vector_t::reverse_iterator			ritvector_t;
	typedef typename vector_t::const_reverse_iterator	critvector_t;

	typedef typename _t_obj::iterator					iter_t;
	typedef typename _t_obj::const_iterator				citer_t;
	typedef typename _t_obj::reverse_iterator			riter_t;
	typedef typename _t_obj::const_reverse_iterator		criter_t;

	_t_obj							*pClTargetArray;
	_t_objprim						*pClArrayPrim;
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
	
	cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: insert<_t_iterator>(const_iterator, _t_iterator, _t_iterator)" << endl;

	pClTargetArray = new _t_obj (*pClArray);

	if (pClTargetArray == NULL)
	{
		cerr << "TestBTreeArraySTLifAssignViaIterator: ERROR: insufficient mempry!" << endl;

		exit (-1);
	}

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY, "target::insert<_t_obj::iter_t> (iter_t, iter_t)", pClArrayPrim, pClTargetArray, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY, "target::insert<_t_obj::citer_t> (citer_t, citer_t)", pClArrayPrim, pClTargetArray, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY, "target::insert<_t_obj::riter_t> (riter_t, riter_t)", pClArrayPrim, pClTargetArray, &sList, sRIterA, sRIterB, sRItListBegin, sRItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY, "target::insert<_t_obj::criter_t> (criter_t, criter_t)", pClArrayPrim, pClTargetArray, &sList, sCRIterA, sCRIterB, sCRItListBegin, sCRItListEnd, nNumEntries);

	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<list::iter_t> (list::iter_t, list::iter_t)", pClArrayPrim, pClTargetArray, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<list::citer_t> (list::citer_t, list::citer_t)", pClArrayPrim, pClTargetArray, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<list::riter_t> (list::riter_t, list::riter_t)", pClArrayPrim, pClTargetArray, &sList, sRIterA, sRIterB, sRItListBegin, sRItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<list::criter_t> (list::criter_t, list::criter_t)", pClArrayPrim, pClTargetArray, &sList, sCRIterA, sCRIterB, sCRItListBegin, sCRItListEnd, nNumEntries);

	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<vector::iter_t> (vector::iter_t, vector::iter_t)", pClArrayPrim, pClTargetArray, &sVector, sIterA, sIterB, sItVecBegin, sItVecEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<vector::citer_t> (vector::citer_t, vector::citer_t)", pClArrayPrim, pClTargetArray, &sVector, sCIterA, sCIterB, sCItVecBegin, sCItVecEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<vector::riter_t> (vector::riter_t, vector::riter_t)", pClArrayPrim, pClTargetArray, &sVector, sRIterA, sRIterB, sRItVecBegin, sRItVecEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_HALF_WAY_EXTERN, "target::insert<vector::criter_t> (vector::criter_t, vector::criter_t)", pClArrayPrim, pClTargetArray, &sVector, sCRIterA, sCRIterB, sCRItVecBegin, sCRItVecEnd, nNumEntries);

	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::iter_t> (target::iter_t, target::iter_t)", pClArrayPrim, pClTargetArray, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::citer_t> (target::citer_t, target::citer_t)", pClArrayPrim, pClTargetArray, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::riter_t> (target::riter_t, target::riter_t)", pClArrayPrim, pClTargetArray, &sList, sRIterA, sRIterB, sRItListBegin, sRItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::criter_t> (target::criter_t, target::criter_t)", pClArrayPrim, pClTargetArray, &sList, sCRIterA, sCRIterB, sCRItListBegin, sCRItListEnd, nNumEntries);

	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_INEFFECTIVE, "target::insert<_t_obj::iter_t> (iter_t == iter_t)", pClArrayPrim, pClTargetArray, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_INEFFECTIVE, "target::insert<_t_obj::citer_t> (citer_t == citer_t)", pClArrayPrim, pClTargetArray, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_INEFFECTIVE, "target::insert<_t_obj::riter_t> (riter_t == riter_t)", pClArrayPrim, pClTargetArray, &sList, sRIterA, sRIterB, sRItListBegin, sRItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_INEFFECTIVE, "target::insert<_t_obj::criter_t> (criter_t == criter_t)", pClArrayPrim, pClTargetArray, &sList, sCRIterA, sCRIterB, sCRItListBegin, sCRItListEnd, nNumEntries);

	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_PART_SELF_REFERENCE, "target::insert<_t_obj::iter_t> (>target::iter_t, target::iter_t<)", pClArrayPrim, pClTargetArray, &sList, sIterA, sIterB, sItListBegin, sItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_PART_SELF_REFERENCE, "target::insert<_t_obj::citer_t> (>target::citer_t, target::citer_t<)", pClArrayPrim, pClTargetArray, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_PART_SELF_REFERENCE, "target::insert<_t_obj::riter_t> (>target::riter_t, target::riter_t<)", pClArrayPrim, pClTargetArray, &sList, sRIterA, sRIterB, sRItListBegin, sRItListEnd, nNumEntries);
	TestBTreeArraySTLifInsertViaIteratorEx (BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_PART_SELF_REFERENCE, "target::insert<_t_obj::criter_t> (>target::criter_t, target::criter_t<)", pClArrayPrim, pClTargetArray, &sList, sCRIterA, sCRIterB, sCRItListBegin, sCRItListEnd, nNumEntries);

	delete pClTargetArray;
}

template <class _t_obj, class _t_objprim>
void TestBTreeArraySTLifInsertViaIteratorLarge (_t_obj *pClArray, uint32_t nWhich, uint32_t nNumEntries, uint32_t nInsertSize)
{
	typedef typename _t_obj::iterator				iter_t;
	typedef typename _t_obj::const_iterator			citer_t;
	typedef typename _t_obj::reverse_iterator		riter_t;
	typedef typename _t_obj::const_reverse_iterator	criter_t;

	uint32_t						i;
	uint32_t						j;
	::std::vector<arrayEntry_t>		sVector;
	iter_t							sIterA, sIterB;
	citer_t							sCIterA, sCIterB;
	riter_t							sRIterA, sRIterB;
	criter_t						sCRIterA, sCRIterB;
	citer_t							sCIter;
	arrayEntry_t					sEntry;
	
	cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: insert<_t_iterator>(const_iterator, _t_iterator, _t_iterator)" << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sEntry.nDebug = g_nDebug;
		sEntry.nData = generate_rand32 ();

		g_nDebug++;

		sVector.push_back (sEntry);
	}

	if (nWhich == 0)
	{
		cout << "target::assign<_t_obj::iter_t> (>target::iter_t, target::iter_t<)" << endl;

		pClArray->template assign< ::std::vector<arrayEntry_t>::iterator> (sVector.begin (), sVector.end ());

		sIterA = pClArray->begin ();
		sIterB = sIterA + (int) nInsertSize;

		for (i = 0; i < 3; i++)
		{
			sCIter = pClArray->cbegin ();

			for (j = 0; j < 4; j++)
			{
				sCIter += pClArray->size () / 5;

				pClArray->template insert <iter_t> (sCIter, sIterA, sIterB);
			}

			sIterA += (pClArray->size () - nInsertSize) / 4;
			sIterB += (pClArray->size () - nInsertSize) / 4;
		}
	}
	else if (nWhich == 1)
	{
		cout << "target::assign<_t_obj::citer_t> (>target::citer_t, target::citer_t<)" << endl;

		pClArray->template assign< ::std::vector<arrayEntry_t>::iterator> (sVector.begin (), sVector.end ());

		sCIterA = pClArray->cbegin ();
		sCIterB = sCIterA + (int) nInsertSize;

		for (i = 0; i < 3; i++)
		{
			sCIter = pClArray->cbegin ();

			for (j = 0; j < 4; j++)
			{
				sCIter += pClArray->size () / 5;

				pClArray->template insert <citer_t> (sCIter, sCIterA, sCIterB);
			}

			sCIterA += (pClArray->size () - nInsertSize) / 4;
			sCIterB += (pClArray->size () - nInsertSize) / 4;
		}

	}
	else if (nWhich == 2)
	{
		cout << "target::assign<_t_obj::riter_t> (>target::riter_t, target::riter_t<)" << endl;

		pClArray->template assign< ::std::vector<arrayEntry_t>::iterator> (sVector.begin (), sVector.end ());

		sRIterA = pClArray->rbegin ();
		sRIterB = sRIterA + (int) nInsertSize;

		for (i = 0; i < 3; i++)
		{
			sCIter = pClArray->cbegin ();

			for (j = 0; j < 4; j++)
			{
				sCIter += pClArray->size () / 5;

				pClArray->template insert <riter_t> (sCIter, sRIterA, sRIterB);
			}

			sRIterA += (pClArray->size () - nInsertSize) / 5;
			sRIterB += (pClArray->size () - nInsertSize) / 5;
		}

	}
	else if (nWhich == 3)
	{
		cout << "target::assign<_t_obj::criter_t> (>target::criter_t, target::criter_t<)" << endl;

		pClArray->template assign< ::std::vector<arrayEntry_t>::iterator> (sVector.begin (), sVector.end ());

		sCRIterA = pClArray->crbegin ();
		sCRIterB = sCRIterA + (int) nInsertSize;

		for (i = 0; i < 3; i++)
		{
			sCIter = pClArray->cbegin ();

			for (j = 0; j < 4; j++)
			{
				sCIter += pClArray->size () / 5;

				pClArray->template insert <criter_t> (sCIter, sCRIterA, sCRIterB);
			}

			sCRIterA += (pClArray->size () - nInsertSize) / 5;
			sCRIterB += (pClArray->size () - nInsertSize) / 5;
		}
	}
	else
	{
		cerr << "TestBTreeArraySTLifInsertViaIteratorLarge: ERROR: unknown sub-test not specified!" << endl;

		exit (-1);
	}
}

template <class _t_obj, class _t_objprim>
void TestBTreeArraySTLifEraseSingle (_t_obj *pClArray, uint32_t nNumEntries)
{
	typedef typename _t_obj::iterator				iter_t;
	typedef typename _t_obj::const_iterator			citer_t;
	
	uint32_t			i;
	_t_objprim			*pClArrayPrim;
	citer_t				sCIter;
	
	cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: erase (const_iterator)" << endl;

	pClArrayPrim = dynamic_cast <_t_objprim *> (pClArray);

	arrayPrim_add (pClArrayPrim, nNumEntries, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

	for (i = 0; i < nNumEntries; i++)
	{
		cout << pClArray->size () << " <-- ";

		sCIter = pClArray->cbegin ();

		sCIter += (int) (generate_rand32 () % (nNumEntries - i));

		pClArray->erase (sCIter);

		cout << pClArray->size () << endl;
	}
}

template <class _t_obj>
void TestBTreeArraySTLifEraseMultiple (_t_obj *pClArray, uint32_t nNumEntries)
{
	typedef typename _t_obj::iterator				iter_t;
	typedef typename _t_obj::const_iterator			citer_t;

	::std::list<arrayEntry_t>		sList;
	uint32_t						i;
	uint32_t						j;
	uint32_t						k;
	citer_t							sCIterA;
	citer_t							sCIterB;
	arrayEntry_t					sEntry;

	cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: erase (const_iterator, const_iterator)" << endl;

	for (i = 0; i < nNumEntries; i += 5)
	{
		for (j = i; j < nNumEntries; j += (nNumEntries / 7))
		{
			cout << i << " / " << nNumEntries << "  " << (j - i) << " / " << (nNumEntries - i) << "  \r";

			sList.clear ();

			for (k = 0; k < nNumEntries; k++)
			{
				sEntry.nData = generate_rand32 ();
				sEntry.nDebug = g_nDebug;

				sList.push_back (sEntry);

				g_nDebug++;
			}

			pClArray->assign (sList.begin (), sList.end ());

			sCIterA = pClArray->cbegin ();
			sCIterB = sCIterA + (int) i;

			sCIterA += (int) (j - i);
			sCIterB += (int) (j - i);

			pClArray->erase (sCIterA, sCIterB);
		}
	}
}

template <class _t_obj>
void TestBTreeArraySTLifSwap (_t_obj *pClArray, uint32_t nNumEntries)
{
	typedef typename _t_obj::iterator				iter_t;
	typedef typename _t_obj::const_iterator			citer_t;

	::std::list<arrayEntry_t>		sList;
	uint32_t						i;
	arrayEntry_t					sEntry;
	_t_obj							sClArrayTarget (*pClArray);
	_t_obj							sClArrayRef (*pClArray);
	_t_obj							sClArrayTargetRef (*pClArray);

	cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeArray<>:: swap ()" << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sEntry.nData = generate_rand32 ();
		sEntry.nDebug = g_nDebug;

		sList.push_back (sEntry);

		g_nDebug++;
	}

	pClArray->template assign< ::std::list<arrayEntry_t>::iterator> (sList.begin (), sList.end ());
	sClArrayRef.template assign< ::std::list<arrayEntry_t>::iterator> (sList.begin (), sList.end ());

	if ((*pClArray) != sClArrayRef)
	{
		cerr << "ERROR: Unexpected array mismatch!" << endl;

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

	sClArrayTarget.template assign< ::std::list<arrayEntry_t>::iterator> (sList.begin (), sList.end ());
	sClArrayTargetRef.template assign< ::std::list<arrayEntry_t>::iterator> (sList.begin (), sList.end ());

	if (sClArrayTarget != sClArrayTargetRef)
	{
		cerr << "ERROR: Unexpected target mismatch!" << endl;

		exit (-1);
	}

	pClArray->swap (sClArrayTarget);

	if ((*pClArray) != sClArrayTargetRef)
	{
		cerr << "ERROR: array mismatches target reference!" << endl;

		exit (-1);
	}

	if (sClArrayTarget != sClArrayRef)
	{
		cerr << "ERROR: target mismatches array reference!" << endl;

		exit (-1);
	}

	sClArrayTarget.swap (*pClArray);

	if ((*pClArray) != sClArrayRef)
	{
		cerr << "ERROR: array mismatches array reference!" << endl;

		exit (-1);
	}

	if (sClArrayTarget != sClArrayTargetRef)
	{
		cerr << "ERROR: target mismatches target reference!" << endl;

		exit (-1);
	}
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArray (uint32_t nTest, uint32_t nNodeSize, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t &sCacheDesc, uint32_t nNumMultiCacheSizes, bayerTreeCacheDescription_t *psMultiCacheDesc, uint32_t nNumCacheProperties, _t_datalayerproperties **ppMultiCacheSizeDataLayerProperties, uint32_t nNumMultiTemplateParams, _t_objprim **ppClArraysPrim, int argc, char **argv)
{
	_t_obj							*pClArray;

	cout << "b-tree array test bench selected" << endl;

	pClArray = new _t_obj (rDataLayerProperties, &sCacheDesc, nNodeSize);

	if (pClArray == NULL)
	{
		cerr << "ERROR: Insufficient memory!" << endl;

		exit (-1);
	}

	switch (nTest)
	{
	case BTREETEST_ARRAY_BASIC_FIFO			:
		{
			TestBTreeArrayBasic<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 512, 256, 3, BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

			break;
		}

	case BTREETEST_ARRAY_BASIC_FIFO_SMALL	:
		{
			TestBTreeArrayBasic<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 16, 0, 0, BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN, BTREETEST_ARRAY_PRIMITIVE_SEEK_END);

			break;
		}

	case BTREETEST_ARRAY_BASIC_LILO			:
		{
			TestBTreeArrayBasic<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 512, 256, 3, BTREETEST_ARRAY_PRIMITIVE_SEEK_END, BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN);

			break;
		}

	case BTREETEST_ARRAY_BASIC_LILO_SMALL	:
		{
			TestBTreeArrayBasic<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 16, 0, 0, BTREETEST_ARRAY_PRIMITIVE_SEEK_END, BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN);

			break;
		}

	case BTREETEST_ARRAY_BASIC_RANDOM		:
		{
			TestBTreeArrayBasic<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 512, 256, 3, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

			break;
		}

	case BTREETEST_ARRAY_BASIC_RANDOM_SMALL	:
		{
			TestBTreeArrayBasic<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 16, 0, 0, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM, BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM);

			break;
		}

	case BTREETEST_ARRAY_clear			:
		{
			TestBTreeArrayRemoveAll<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 256);

			break;
		}

	case BTREETEST_ARRAY_REPLACE			:
		{
			TestBTreeArrayReplace<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 16, 8192, 512, 16);

			break;
		}

	case BTREETEST_ARRAY_UNLOAD				:
		{
			TestBTreeArrayUnload<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 128, 128, 128, 128);

			break;
		}

	case BTREETEST_ARRAY_MULTI_NODESIZES			:
		{
			TestBTreeArrayMultiNodeSizes<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, rDataLayerProperties, &sCacheDesc, 3, 16, 64, 4, 16, 16);

			break;
		}

	case BTREETEST_ARRAY_MULTI_CACHE_SETTINGS		:
		{
			TestBTreeArrayMultiCacheSize<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, nNumCacheProperties, nNumMultiCacheSizes, psMultiCacheDesc, ppMultiCacheSizeDataLayerProperties, nNodeSize, 64, 4, 16, 16);

			break;
		}

	case BTREETEST_ARRAY_MULTI_TEMPLATE_PARAMETERS	:
		{
			TestBTreeArrayMultiTemplateParams<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, &sCacheDesc, nNumMultiTemplateParams, ppClArraysPrim, nNodeSize, 64, 4, 16, 16);

			break;
		}

	case BTREETEST_ARRAY_HTML_OUTPUT				:
		{
			TestBTreeArrayHTMLoutput<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 64);

			break;
		}

	case BTREETEST_ARRAY_COPY_CONSTRUCTOR			:
		{
			TestBTreeArrayCopyConstructor<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 64);

			break;
		}

	case BTREETEST_ARRAY_OPERATOR_OVERLOAD_ASSIGN	:
		{
			TestBTreeArrayOperatorOverloadAssign<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 64);

			break;
		}

	case BTREETEST_ARRAY_OPERATOR_OVERLOAD_SUBSCRIPT	:
		{
			TestBTreeArrayOperatorOverloadSubScript<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 64);

			break;
		}

	case BTREETEST_ARRAY_SERLIALIZE_ALL					:
		{
			TestBTreeArraySerialize<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 128, 128);

			break;
		}

	case BTREETEST_ARRAY_SERLIALIZE_HALF_NODE			:
		{
			TestBTreeArraySerialize<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 128, 64);

			break;
		}

	case BTREETEST_ARRAY_SERLIALIZE_PART_NODE			:
		{
			TestBTreeArraySerialize<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 128, nNodeSize - 1);

			break;
		}

	case BTREETEST_ARRAY_SERLIALIZE_INCOMPLETE			:
		{
			TestBTreeArraySerializeIncomplete<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 128, 128, 64);

			break;
		}

	case BTREETEST_ARRAY_CODE_COVERAGE_ROOT_MERGE_WITH_LOWER_RIGHT	:
		{
			TestBTreeArrayCodeCoverageRootMergeWithLowerRight<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, nNodeSize);

			break;
		}

	case BTREETEST_ARRAY_SELF_REFERENCE_VIA_SUBSCRIPT_WRAPPER	:
		{
			TestBTreeArraySelfReferenceViaSubScriptWrapper<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClArray, 10);

			break;
		}

	case BTREETEST_ARRAY_INTRINSIC_DATA_TYPE	:
		{
			TestBTreeArrayInTrinsicDataType<_t_datalayerproperties, _t_datalayer> (nNodeSize, rDataLayerProperties, sCacheDesc);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_AT_FRONT_BACK	:
		{
			TestBTreeArraySTLifAtFrontBack<_t_obj, _t_objprim> (pClArray, 10);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_ASSIGN_FILL:
		{
			TestBTreeArraySTLifAssignFill<_t_obj> (pClArray, 512);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR	:
		{
			TestBTreeArraySTLifAssignViaIterator<_t_obj, _t_objprim> (pClArray, 64);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_PUSH_BACK_POP_BACK	:
		{
			TestBTreeArraySTLifPushBackPopBack<_t_obj> (pClArray, 128);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_INSERT_SINGLE	:
		{
			TestBTreeArraySTLifInsertSingle<_t_obj, _t_objprim> (pClArray, 64);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_INSERT_MULTIPLE:
		{
			TestBTreeArraySTLifInsertMultiple<_t_obj, _t_objprim> (pClArray, 64, 4);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR:
		{
			TestBTreeArraySTLifInsertViaIterator<_t_obj, _t_objprim> (pClArray, 64);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_ITER	:
		{
			TestBTreeArraySTLifInsertViaIteratorLarge<_t_obj, _t_objprim> (pClArray, 0, 131072, 16384);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_CITER	:
		{
			TestBTreeArraySTLifInsertViaIteratorLarge<_t_obj, _t_objprim> (pClArray, 1, 131072, 16384);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_RITER	:
		{
			TestBTreeArraySTLifInsertViaIteratorLarge<_t_obj, _t_objprim> (pClArray, 2, 131072, 16384);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_CRITER	:
		{
			TestBTreeArraySTLifInsertViaIteratorLarge<_t_obj, _t_objprim> (pClArray, 3, 131072, 16384);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_ERASE_SINGLE	:
		{
			TestBTreeArraySTLifEraseSingle<_t_obj, _t_objprim> (pClArray, 128);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_ERASE_MULTIPLE	:
		{
			TestBTreeArraySTLifEraseMultiple<_t_obj> (pClArray, 128);

			break;
		}

	case BTREETEST_ARRAY_STL_IF_SWAP	:
		{
			TestBTreeArraySTLifSwap<_t_obj> (pClArray, 128);

			break;
		}

	default									:
		{
			cerr << "ERROR: TestBTreeArray: Unknown test or test not specified!" << endl;

			break;
		}
	}

	delete pClArray;
}
