/************************************************************
**
** file:	btreekeysorttestbench.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for the key sort data class.
**
************************************************************/

#include "btreekeysorttestbench.h"

template <class _t_container>
void TestBTreeKeySortBasic (_t_container *pContainer, uint32_t nMaxEntries, uint32_t nVariation, uint32_t nRepetitions, btreetest_key_generation_e eGeneratorAdd, btreetest_key_generation_e eGeneratorRemove)
{
	uint32_t			ui32;
	uint32_t			nLastKey;
	uint32_t			nLastKeyInit;

	cout << "basic test adds data at ";

	switch (eGeneratorAdd)
	{
	case BTREETEST_KEY_GENERATION_DESCEND	:
		{
			cout << "the beginning ";

			nLastKeyInit = ~0x0;

			break;
		}

	case BTREETEST_KEY_GENERATION_ASCEND		:
		{
			cout << "the end ";

			nLastKeyInit = 0x0;

			break;
		}

	case BTREETEST_KEY_GENERATION_RANDOM		:
		{
			cout << "a random position ";

			nLastKeyInit = 0x0;

			break;
		}

	case BTREETEST_KEY_GENERATION_CONST		:
		{
			cout << "the same position ";

			nLastKeyInit = 0x0;

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
	case BTREETEST_KEY_GENERATION_DESCEND	:
		{
			cout << "the beginning";

			break;
		}

	case BTREETEST_KEY_GENERATION_ASCEND		:
		{
			cout << "the end";

			break;
		}

	case BTREETEST_KEY_GENERATION_RANDOM		:
		{
			cout << "a random position";

			break;
		}

	case BTREETEST_KEY_GENERATION_CONST		:
		{
			cout << "the same position";

			break;
		}

	default									:
		{
			cout << "?";

			break;
		}
	}

	cout << endl;

	nLastKey = nLastKeyInit;

	associative_container_add_primitive (pContainer, nMaxEntries, nLastKey, eGeneratorAdd);

	for (ui32 = 0; ui32 < nRepetitions; ui32++)
	{
		cout << "cycle: " << ui32 + 1 << endl;

		nLastKey = nLastKeyInit;

		associative_container_remove_primitive (pContainer, nVariation, ~0x0, nLastKey, eGeneratorRemove);

		nLastKey = nLastKeyInit;

		associative_container_add_primitive (pContainer, nVariation, nLastKey, eGeneratorAdd);
	}

	nLastKey = nLastKeyInit;

	associative_container_remove_primitive (pContainer, nMaxEntries, ~0x0, nLastKey, eGeneratorRemove);

	cout << "finished..." << endl;
}

template <class _t_container>
void TestBTreeKeySortRemoveAll (_t_container *pContainer, uint32_t nEntries)
{
	uint32_t			nLastKey = 0;
	
	cout << "test inserts data and empties the data container by calling clear" << endl;

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	pContainer->clear ();
}

template <class _t_container>
void TestBTreeKeySortMultiRemove (_t_container *pContainer, uint32_t nEntries)
{
	uint32_t			nLastKey = 0;
	
	cout << "test removes key sets" << endl;

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	associative_container_remove_primitive (pContainer, nEntries, ~0x0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);
}

template <class _t_container>
void TestBTreeKeySortUnload (_t_container *pContainer, uint32_t nInitEntries, uint32_t nAddEntries, uint32_t nRemoveEntries, uint32_t nExitEntries)
{
	uint32_t			nLastKey = 0;
	
	cout << "test uses unload after every operation" << endl;

	cout << "unloading..." << endl;

	pContainer->unload ();

	associative_container_add_primitive (pContainer, nInitEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	cout << "unloading..." << endl;
	
	pContainer->unload ();

	nLastKey++;

	associative_container_add_primitive (pContainer, nAddEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	cout << "unloading..." << endl;
	
	pContainer->unload ();

	associative_container_remove_primitive (pContainer, nRemoveEntries, ~0x0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	cout << "unloading..." << endl;

	pContainer->unload ();

	nLastKey--;

	associative_container_remove_primitive (pContainer, nExitEntries, ~0x0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);
	
	cout << "unloading..." << endl;
	
	pContainer->unload ();

	associative_container_add_primitive (pContainer, nInitEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	cout << "unloading..." << endl;
	
	pContainer->unload ();

	cout << "unloading..." << endl;
	
	pContainer->unload ();

	cout << "remove all" << endl;

	pContainer->clear ();

	cout << "unloading..." << endl;
	
	pContainer->unload ();
}

template <class _t_container>
void TestBTreeKeySortRemoveInstance (_t_container *pContainer, uint32_t nEntries, uint32_t nInstances)
{
	uint32_t			ui32;
	uint32_t			nLastKey = 0;

	cout << "test removes key instances" << endl;

	for (ui32 = 0; ui32 < nInstances; ui32++)
	{
		associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);
	}

	associative_container_remove_primitive (pContainer, nEntries, 2, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	associative_container_remove_primitive (pContainer, nEntries, 0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	associative_container_remove_primitive (pContainer, nEntries, ~0x0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);
}

//template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
//void BTreeKeySortMulti (_t_obj **ppClKeySort, uint32_t nNumArray, uint32_t nAddEntries, uint32_t nAddMultiple, uint32_t nRemoveEntries, uint32_t nRemoveMultiple)
//{
//	uint32_t				ui32, uj32;
//	bool					bError = false;
//	_t_objprim				**pClKeySortPrim;
//	std::stringstream		aszArrayName;
//
//	pClKeySortPrim = new _t_objprim * [nNumArray];
//
//	if (pClKeySortPrim == NULL)
//	{
//		cerr << "ERROR: insufficient memory!" << endl;
//
//		exit (-1);
//	}
//
//	for (ui32 = 0; ui32 < nNumArray; ui32++)
//	{
//		pClKeySortPrim[ui32] = dynamic_cast <_t_objprim *> (ppClKeySort[ui32]);
//	}
//
//	for (ui32 = 0; ui32 < nAddMultiple; ui32++)
//	{
//		nDebug = 0;
//
//		for (uj32 = 0; uj32 < nNumArray; uj32++)
//		{
//			associative_container_add_primitive (pClKeySortPrim[uj32], nAddEntries, nDebug, ui32, BTREETEST_KEY_GENERATION_RANDOM);
//		}
//
//		for (uj32 = 1; uj32 < nNumArray; uj32++)
//		{
//			if ((* (ppClKeySort[0])) != (* (ppClKeySort[uj32])))
//			{
//				cerr << "ERROR: array " << uj32 << " mismatches master array" << endl;
//
//				bError = true;
//			}
//		}
//
//		if (bError)
//		{
//			for (uj32 = 0; uj32 < nNumArray; uj32++)
//			{
//				aszArrayName.clear ();
//				aszArrayName << "array" << aszArrayName.width (2) << aszArrayName.fill ('0') << uj32 << ".html" << aszArrayName.width (0) << aszArrayName.fill (' ');
//
//				cerr << "outputting " << aszArrayName.str ().c_str () << endl;
//
//				ppClKeySort[ui32]->show_integrity (aszArrayName.str ().c_str ());
//			}
//
//			exit (-1);
//		}
//	}
//
//	for (ui32 = 0; ui32 < nRemoveMultiple; ui32++)
//	{
//		for (uj32 = 0; uj32 < nNumArray; uj32++)
//		{
//			associative_container_remove_primitive (pClKeySortPrim[uj32], nRemoveEntries, 0, ui32, BTREETEST_KEY_GENERATION_RANDOM);
//		}
//
//		for (uj32 = 1; uj32 < nNumArray; uj32++)
//		{
//			if ((* (ppClKeySort[0])) != (* (ppClKeySort[uj32])))
//			{
//				cerr << "ERROR: array " << uj32 << " mismatches master array" << endl;
//
//				bError = true;
//			}
//		}
//
//		if (bError)
//		{
//			for (uj32 = 0; uj32 < nNumArray; uj32++)
//			{
//				aszArrayName.clear ();
//				aszArrayName << "array" << aszArrayName.width (2) << aszArrayName.fill ('0') << uj32 << ".html" << aszArrayName.width (0) << aszArrayName.fill (' ');
//
//				cerr << "outputting " << aszArrayName.str().c_str() << endl;
//
//				ppClKeySort[ui32]->show_integrity (aszArrayName.str ().c_str ());
//			}
//
//			exit (-1);
//		}
//	}
//
//	delete [] pClKeySortPrim;
//}
//
//template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
//void TestBTreeKeySortMultiNodeSizes (_t_obj *pClMaster, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nFromNodeSize, uint32_t nToNodeSize, uint32_t nAddEntries, uint32_t nAddMultiple, uint32_t nRemoveEntries, uint32_t nRemoveMultiple)
//{
//	uint32_t		nDebug = 0;
//	uint32_t		ui32;
//	uint32_t		nNumInst;
//	_t_obj			**ppClKeySort;
//
//	cout << "test exercises several instances using different node size settings" << endl;
//
//	nNumInst = nToNodeSize - nFromNodeSize + 2;
//
//	ppClKeySort = new _t_obj * [nNumInst];
//
//	if (ppClKeySort == NULL)
//	{
//		cerr << "ERROR: insufficient memory!" << endl;
//
//		exit (-1);
//	}
//
//	ppClKeySort[0] = pClMaster;
//	
//	for (ui32 = nFromNodeSize; ui32 <= nToNodeSize; ui32++)
//	{
//		ppClKeySort[ui32 - nFromNodeSize + 1] = new _t_obj (rDataLayerProperties, psCacheDescription, ui32, NULL);
//
//		if (ppClKeySort[ui32 - nFromNodeSize + 1] == NULL)
//		{
//			cerr << "ERROR: insufficient memory!" << endl;
//
//			exit (-1);
//		}
//	}
//
//	BTreeKeySortMulti<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (ppClKeySort, nNumInst, nAddEntries, nAddMultiple, nRemoveEntries, nRemoveMultiple);
//
//	for (ui32 = nFromNodeSize; ui32 <= nToNodeSize; ui32++)
//	{
//		delete ppClKeySort[ui32 - nFromNodeSize + 1];
//	}
//
//	delete [] ppClKeySort;
//}
//
//template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
//void TestBTreeKeySortMultiCacheSize (_t_obj *pClMaster, uint32_t nNumInst, uint32_t nNumMultiCacheSizes, bayerTreeCacheDescription_t *psMultiCacheDesc, _t_datalayerproperties **ppMultiCacheSizeDataLayerProperties, uint32_t nNodeSize, uint32_t nAddEntries, uint32_t nAddMultiple, uint32_t nRemoveEntries, uint32_t nRemoveMultiple)
//{
//	uint32_t						nDebug = 0;
//	uint32_t						ui32;
//	_t_obj							**ppClKeySort;
//	
//	cout << "test exercises several instances using different cache settings" << endl;
//
//	ppClKeySort = new _t_obj * [nNumInst + 1];
//
//	if (ppClKeySort == NULL)
//	{
//		cerr << "ERROR: insufficient memory!" << endl;
//
//		exit (-1);
//	}
//
//	ppClKeySort[0] = pClMaster;
//	
//	for (ui32 = 0; ui32 < nNumMultiCacheSizes; ui32++)
//	{
//		ppClKeySort[ui32 + 1] = new _t_obj (*(ppMultiCacheSizeDataLayerProperties[ui32]), &(psMultiCacheDesc[ui32]), nNodeSize, NULL);
//
//		if (ppClKeySort[ui32 + 1] == NULL)
//		{
//			cerr << "ERROR: insufficient memory!" << endl;
//
//			exit (-1);
//		}
//	}
//
//	BTreeKeySortMulti<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (ppClKeySort, nNumInst + 1, nAddEntries, nAddMultiple, nRemoveEntries, nRemoveMultiple);
//
//	for (ui32 = 0; ui32 < nNumMultiCacheSizes; ui32++)
//	{
//		delete ppClKeySort[ui32 + 1];
//	}
//
//	delete [] ppClKeySort;
//}
//
//template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
//void TestBTreeKeySortMultiTemplateParams (_t_obj *pClMaster, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nNumMultiTemplateParams, _t_objprim **ppClKeySortPrim, uint32_t nNodeSize, uint32_t nAddEntries, uint32_t nAddMultiple, uint32_t nRemoveEntries, uint32_t nRemoveMultiple)
//{
//	const uint32_t		nNumInst = nNumMultiTemplateParams + 1;
//	uint32_t			nDebug = 0;
//	uint32_t			ui32;
//	uint32_t			uj32;
//	bool				bError = false;
//	_t_objprim			**ppClKeySortPrimAll;
//
//	cout << "test exercises several instances using different template parameter settings" << endl;
//
//	ppClKeySortPrimAll = new _t_objprim * [nNumInst];
//
//	if (ppClKeySortPrimAll == NULL)
//	{
//		cerr << "ERROR: insufficient memory!";
//
//		exit (-1);
//	}
//
//	ppClKeySortPrimAll[0] = dynamic_cast <_t_objprim *> (pClMaster);
//
//	for (ui32 = 0; ui32 < nNumMultiTemplateParams; ui32++)
//	{
//		ppClKeySortPrimAll[ui32 + 1] = ppClKeySortPrim[ui32];
//	}
//
//	for (ui32 = 0; ui32 < nAddMultiple; ui32++)
//	{
//		nDebug = 0;
//
//		for (uj32 = 0; uj32 < nNumInst; uj32++)
//		{
//			associative_container_add_primitive (ppClKeySortPrimAll[uj32], nAddEntries, nDebug, ui32, BTREETEST_KEY_GENERATION_RANDOM);
//		}
//
//		for (uj32 = 1; uj32 < nNumInst; uj32++)
//		{
//			if (!associative_container_compare_primitive (ppClKeySortPrimAll[0], ppClKeySortPrimAll[uj32]))
//			{
//				cerr << "ERROR: array " << uj32 << " mismatches master array!" << endl;
//
//				bError = true;
//			}
//		}
//
//		if (bError)
//		{
//			for (ui32 = 0; ui32 < nNumInst; ui32++)
//			{
//				_t_obj			*pContainer;
//				std::stringstream	strFileName;
//
//				pContainer = dynamic_cast <_t_obj *> (ppClKeySortPrimAll[ui32]);
//
//				strFileName << "keysort" << ui32 << ".html";
//
//				pContainer->show_integrity (strFileName.str ().c_str ());
//			}
//
//
//			break;
//		}
//	}
//
//	if (!bError)
//	{
//		for (ui32 = 0; ui32 < nRemoveMultiple; ui32++)
//		{
//			for (uj32 = 0; uj32 < nNumInst; uj32++)
//			{
//				associative_container_remove_primitive (ppClKeySortPrimAll[uj32], nRemoveEntries, 0, ui32, BTREETEST_KEY_GENERATION_RANDOM);
//			}
//
//			for (uj32 = 1; uj32 < nNumInst; uj32++)
//			{
//				if (!associative_container_compare_primitive (ppClKeySortPrimAll[0], ppClKeySortPrimAll[uj32]))
//				{
//					cerr << "ERROR: array " << uj32 << " mismatches master array!" << endl;
//
//					bError = true;
//				}
//			}
//
//			if (bError)
//			{
//				for (ui32 = 0; ui32 < nNumInst; ui32++)
//				{
//					_t_obj			*pContainer;
//					std::stringstream	strFileName;
//
//					pContainer = dynamic_cast <_t_obj *> (ppClKeySortPrimAll[ui32]);
//
//					strFileName << "keysort" << ui32 << ".html";
//
//					pContainer->show_integrity (strFileName.str ().c_str ());
//				}
//
//
//				break;
//			}
//		}
//	}
//
//	delete [] ppClKeySortPrimAll;
//}
//
template <class _t_container>
void TestBTreeKeySortCCdeterminePosition (_t_container *pContainer, uint32_t nNodeSize)
{
	uint32_t			nKey = 0xABCDEF98;
	uint32_t			nEntries;
	uint32_t			nInst;
	
	cout << "test exercises code coverage regarding determinePosition" << endl;

	nEntries = nNodeSize * 2 + (nNodeSize * 2) - 1;

	associative_container_add_primitive (pContainer, nEntries, nKey, BTREETEST_KEY_GENERATION_CONST);

	nInst = nNodeSize * 2 - 1;

	associative_container_remove_primitive (pContainer, 1, nInst, nKey, BTREETEST_KEY_GENERATION_CONST);
}

template <class _t_container>
void TestBTreeKeySortCCfindFirstKey (_t_container *pContainer, uint32_t nNodeSize)
{
	uint32_t												nKey = 0xABCDEF98;
	uint32_t												nEntries;
	uint32_t												nInst;
	uint32_t												ui32;
	::std::multimap<uint32_t, keySortMap_t>::iterator		sIter;

	cout << "test exercises code coverage regarding findFirstKey" << endl;

	for (ui32 = 0, nEntries = 1; ui32 < 6; ui32++)
	{
		nEntries *= nNodeSize;
	}

	associative_container_add_primitive (pContainer, nEntries, nKey, BTREETEST_KEY_GENERATION_CONST);

	while (pContainer->size () > 0)
	{
		//pContainer->get (sKey, 0, &sEntry);
		sIter = pContainer->lower_bound (nKey);

		::std::multimap<uint32_t, keySortMap_t>::value_type		sValue (*sIter);

		nInst = generate_rand32 () % pContainer->size ();
		nInst /= 2;

		nKey = sValue.first;

		associative_container_remove_primitive (pContainer, 1, nInst, nKey, BTREETEST_KEY_GENERATION_CONST);
	}
}

template <class _t_container>
void TestBTreeKeySortHTMLoutput (_t_container *pContainer, uint32_t nEntries)
{
	uint32_t			nLastKey = 0;

	cout << "tests if instance is left in undefined state after HTML output" << endl;

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	nLastKey++;

	pContainer->show_integrity ("test_keysort_output0_");

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	nLastKey++;

	pContainer->show_integrity ("test_keysort_output1_");

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);
}

template <class _t_container>
void TestBTreeKeySortCopyConstructorTest (_t_container *pClRef, _t_container sClCopy)
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

template <class _t_container>
void TestBTreeKeySortCopyConstructor (_t_container *pContainer, uint32_t nEntries)
{
	uint32_t			nLastKey = 0;
	
	cout << "exercises keysort type copy constructor" << endl;

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);

	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);

	nLastKey++;
	
	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);
	
	nLastKey++;
	
	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);

	pContainer->unload ();

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);

	nLastKey = 0;

	associative_container_remove_primitive (pContainer, nEntries, ~0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);

	nLastKey++;

	associative_container_remove_primitive (pContainer, nEntries, ~0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);

	pContainer->clear ();

	TestBTreeKeySortCopyConstructorTest (pContainer, *pContainer);
}

//template <class _t_container>
//void TestBTreeKeySortCompare (_t_container *pClRef, _t_container *pClCopy)
//{
//	if (*pClCopy != *pClRef)
//	{
//		cerr << endl << "ERROR: copied instance mismatches reference!" << endl;
//		cerr << "outputting reference to comp_reference.html" << endl;
//
//		pClRef->show_integrity ("comp_reference.html");
//
//		cerr << "outputting copied instance to comp_copy.html" << endl;
//
//		pClCopy->show_integrity ("comp_copy.html");
//
//		exit (-1);
//	}
//}

template <class _t_container>
void TestBTreeKeySortOperatorOverloadAssign (_t_container *pContainer, uint32_t nEntries)
{
	uint32_t			nLastKey = 0;
	_t_container		*pTarget;
	
	cout << "exercises keysort type operator=" << endl;

	pTarget = new _t_container (*pContainer);

	if (pTarget == NULL)
	{
		cerr << "ERROR: Insufficient memory!" << endl;

		exit (-1);
	}

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);
	
	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);
	
	nLastKey++;
	
	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);
	
	nLastKey++;
	
	associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);
	
	pContainer->unload ();

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);
	
	nLastKey = 0;
	
	associative_container_remove_primitive (pContainer, nEntries, ~0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);
	
	nLastKey += 2;
	
	associative_container_remove_primitive (pContainer, nEntries, ~0, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);
	
	pContainer->clear ();

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);
	
	delete pTarget;
}

template <class _t_container>
void TestBTreeKeySortOperatorOverloadAssignMultiInstance (_t_container *pContainer, uint32_t nOuterEntries, uint32_t nInnerEntries, uint32_t nMulti)
{
	uint32_t			nLastKey = 0;
	uint32_t			ui32;
	uint32_t			nEntries;
	_t_container		*pTarget;

	cout << "exercises keysort type operator= while more than one instance per key is in use" << endl;

	pTarget = new _t_container (*pContainer);

	if (pTarget == NULL)
	{
		cerr << "ERROR: Insufficient memory!" << endl;

		exit (-1);
	}

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);
	
	for (ui32 = 0; ui32 < nMulti; ui32++)
	{
		if ((ui32 == 0) || (ui32 == (nMulti - 1)))
		{
			nEntries = nOuterEntries;
		}
		else
		{
			nEntries = nInnerEntries;
		}

		associative_container_add_primitive (pContainer, nEntries, nLastKey, BTREETEST_KEY_GENERATION_CONST);

		nLastKey++;

		*pTarget = *pContainer;

		associative_container_compare_primitive (pContainer, pTarget);
	}

	pContainer->unload ();

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);

	nLastKey = 0;
	
	associative_container_remove_primitive (pContainer, nOuterEntries, 0, nLastKey, BTREETEST_KEY_GENERATION_CONST);

	nLastKey++;

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);
	
	associative_container_remove_primitive (pContainer, nInnerEntries, 0, nLastKey, BTREETEST_KEY_GENERATION_CONST);

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);
	
	pContainer->clear ();

	*pTarget = *pContainer;

	associative_container_compare_primitive (pContainer, pTarget);
	
	delete pTarget;
}

template <class _t_container>
void TestBTreeKeySortLowerBoundOnLeafNode (_t_container *pContainer, uint32_t nNodeSize)
{
	typename _t_container::const_iterator	sCIter;
	uint32_t								nLastKey = 0;
	uint64_t								nRslt;
	
	::std::cout << "exercises lower_bound on leaf node with node size " << nNodeSize << ::std::endl;

	associative_container_add_primitive (pContainer, nNodeSize * 2 - 1, nLastKey, BTREETEST_KEY_GENERATION_CONST);

	sCIter = pContainer->lower_bound (nLastKey);

	nRslt = ::std::distance (pContainer->cbegin (), sCIter);

	if (nRslt != 0)
	{
		::std::cerr << "ERROR: TestBTreeKeySortLowerBoundOnLeafNode: pContainer->lower_bound did not return with iterator equivalent to begin ()" << ::std::endl;

		pContainer->show_integrity ("lower_bound_0_");

		exit (-1);
	}

	pContainer->clear ();

	associative_container_add_primitive (pContainer, nNodeSize - 1, nLastKey, BTREETEST_KEY_GENERATION_CONST);

	nLastKey += 2;

	associative_container_add_primitive (pContainer, nNodeSize - 1, nLastKey, BTREETEST_KEY_GENERATION_CONST);

	sCIter = pContainer->lower_bound (nLastKey);

	nRslt = ::std::distance (pContainer->cbegin (), sCIter);

	if (nRslt != (nNodeSize - 1))
	{
		::std::cerr << "ERROR: TestBTreeKeySortLowerBoundOnLeafNode: pContainer->lower_bound did not return with iterator equivalent to value t - 1" << ::std::endl;

		pContainer->show_integrity ("lower_bound_1_");

		exit (-1);
	}

	nLastKey -= 2;

	sCIter = pContainer->lower_bound (nLastKey);

	nRslt = ::std::distance (pContainer->cbegin (), sCIter);

	if (nRslt != 0)
	{
		::std::cerr << "ERROR: TestBTreeKeySortLowerBoundOnLeafNode: pContainer->lower_bound did not return with iterator equivalent to begin ()" << ::std::endl;

		pContainer->show_integrity ("lower_bound_2_");

		exit (-1);
	}

	nLastKey++;

	associative_container_add_primitive (pContainer, 2, nLastKey, BTREETEST_KEY_GENERATION_CONST);

	sCIter = pContainer->lower_bound (nLastKey);

	nRslt = ::std::distance (pContainer->cbegin (), sCIter);

	if (nRslt != (nNodeSize - 1))
	{
		::std::cerr << "ERROR: TestBTreeKeySortLowerBoundOnLeafNode: pContainer->lower_bound did not return with iterator equivalent to value t - 1" << ::std::endl;

		pContainer->show_integrity ("lower_bound_3_");

		exit (-1);
	}

	nLastKey--;

	associative_container_add_primitive (pContainer, nNodeSize * 2 - 1, nLastKey, BTREETEST_KEY_GENERATION_CONST);

	sCIter = pContainer->lower_bound (nLastKey);

	nRslt = ::std::distance (pContainer->cbegin (), sCIter);

	if (nRslt != 0)
	{
		::std::cerr << "ERROR: TestBTreeKeySortLowerBoundOnLeafNode: pContainer->lower_bound did not return with iterator equivalent to begin ()" << ::std::endl;

		pContainer->show_integrity ("lower_bound_4_");

		exit (-1);
	}

	nLastKey += 3;

	sCIter = pContainer->lower_bound (nLastKey);

	nRslt = ::std::distance (pContainer->cbegin (), sCIter);

	if (nRslt != pContainer->size ())
	{
		::std::cerr << "ERROR: TestBTreeKeySortLowerBoundOnLeafNode: pContainer->lower_bound did not return with iterator equivalent to end ()" << ::std::endl;

		pContainer->show_integrity ("lower_bound_5_");

		exit (-1);
	}
}

template <class _t_container>
void TestBTreeKeySortSerialize (_t_container *pContainer, uint32_t nInstance, uint32_t nSize, uint32_t nWindowSize)
{
	typedef typename _t_container::value_type				data_t;
	typedef typename _t_container::const_iterator			citer_t;

	uint32_t			nLastKey = 0;
	uint32_t			ui32;
	uint64_t			nRslt;
	data_t				*pnData;
	
	cout << "exercises serialize method" << endl;

	pnData = new data_t [nWindowSize];

	if (pnData == NULL)
	{
		cerr << "ERROR: TestBTreeKeySortSerialize: insufficient memory!" << endl;

		exit (-1);
	}
	
	for (ui32 = 0; ui32 < nInstance; ui32++)
	{
		associative_container_add_primitive (pContainer, nSize, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

		nLastKey = 0;
	}

	for (ui32 = 0; ui32 < ((nSize * nInstance) - nWindowSize + 1); ui32++)
	{
		nRslt = pContainer->serialize (ui32, nWindowSize, pnData);

		::std::cout << "testing from " << ui32 << " to " << ui32 + nWindowSize << ::std::endl;

		if ((uint32_t) nRslt != nWindowSize)
		{
			::std::cerr << "ERROR: TestBTreeKeySortSerialize: size mismatch!" << ::std::endl;
			::std::cerr << "iteration: " << ui32 << ::std::endl;
			::std::cerr << "window size: " << nWindowSize << ::std::endl;
			::std::cerr << "returned size: " << nRslt << ::std::endl;
			::std::cerr << "outputting serialize_????.html" << ::std::endl << ::std::flush;

			pContainer->show_integrity ("serialize_");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}
	}

	delete [] pnData;
}

template <class _t_container>
void TestBTreeKeySortSerializeIncomplete (_t_container *pContainer, uint32_t nNodeSize, uint32_t nSize, uint32_t nWindowSize, uint32_t nOffset)
{
	typedef typename _t_container::value_type			data_t;

	uint32_t			nLastKey = 0;
	uint64_t			nRslt;
	data_t				*pnData;
	
	cout << "exercises serialize method with an incomplete transfer" << endl;

	pnData = new data_t [nWindowSize];

	if (pnData == NULL)
	{
		cerr << "ERROR: TestBTreeKeySortSerialize: insufficient memory!" << endl;

		exit (-1);
	}
	
	associative_container_add_primitive (pContainer, nSize, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	cout << "testing from " << nOffset << " to " << nWindowSize + nOffset << endl;

	nRslt = pContainer->serialize (nOffset, nWindowSize, pnData);

	if ((uint32_t) nRslt != (nSize - nOffset))
	{
		cerr << "ERROR: TestBTreeKeySortSerialize: size mismatch!" << endl;
		cerr << "expected size: " << nSize - nOffset << endl;
		cerr << "returned size: " << nRslt << endl;

		exit (-1);
	}

	delete [] pnData;
}

template <class _t_container>
void TestBTreeKeySortSTLifInsert (_t_container *pContainer, uint32_t nNumEntries)
{
	typedef typename _t_container::value_type			data_t;
	typedef typename _t_container::const_iterator		citer_t;

	citer_t				sCIter;
	citer_t				sCIterRd;
	uint32_t			i;
	data_t				sEntry;
	uint32_t			nCnt;
	uint32_t			nKey;

	cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeySort<>:: insert (const _t_data &)" << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		cout << i << " / " << nNumEntries << "\r";

		do
		{
			nKey = generate_rand32 ();
		} while (pContainer->count (nKey) != 0);
		
		set_entry_key (sEntry, nKey);
		set_entry_data (sEntry, generate_rand32 ());
		set_entry_debug (sEntry, i);

		sCIter = pContainer->insert (sEntry);

		sCIterRd = sCIter;

		for (nCnt = 0; sCIterRd != pContainer->cbegin (); nCnt++)
		{
			if (get_entry_key (*sCIterRd) > get_entry_key (sEntry))
			{
				break;
			}

			sCIterRd--;
		}

		if (sCIterRd != pContainer->cbegin ())
		{
			cerr << endl;
			cerr << "ERROR: TestBTreeKeySortSTLinsert: returned key doesn't fulfill required sort condition! (<=)" << endl;
			cerr << "new entry key: " << get_entry_key (sEntry) << endl;
			cerr << " returned key: " << (*sCIterRd).first << endl;

			exit (-1);
		}

		sCIterRd = sCIter;

		for (nCnt = 0; sCIterRd != pContainer->cend (); nCnt++)
		{
			if (get_entry_key (*sCIterRd) < get_entry_key (sEntry))
			{
				break;
			}

			sCIterRd++;
		}

		if (sCIterRd != pContainer->cend ())
		{
			cerr << endl;
			cerr << "ERROR: TestBTreeKeySortSTLinsert: returned key doesn't fulfill required sort condition! (>=)" << endl;
			cerr << "new entry key: " << get_entry_key (sEntry) << endl;
			cerr << " returned key: " << (*sCIterRd).first << endl;

			exit (-1);
		}
	}

	cout << i << " / " << nNumEntries << "\n";
}

template <class _t_dest_container, class _t_src_container, class _t_container, class _t_iterator, class _t_ext_iterator>
void TestBTreeKeySortSTLifInsertViaIteratorEx (btreetest_keysort_stl_if_insert_via_iterator_e eTest, const char *pszTestTitle, int nArg, uint32_t nNumInstances, _t_dest_container *pDestContainer, _t_src_container *pSrcContainer, _t_container *pContainer, _t_iterator &rIterA, _t_iterator &rIterB, _t_ext_iterator &rExtIterA, _t_ext_iterator &rExtIterB, uint32_t nNumEntries, uint32_t &rLastKey)
{
	typedef typename _t_src_container::size_type				size_type;

	_t_iterator		sIterBegin;
	_t_iterator		sIterEnd;
	_t_ext_iterator	sExtIterBegin;
	_t_ext_iterator	sExtIterEnd;
	size_type		nSize;
	uint32_t		ui32;

	::std::cout << pszTestTitle << ::std::endl;

	associative_container_add_primitive (pSrcContainer, nNumEntries, rLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	get_begin (pSrcContainer, sIterBegin);
	get_end (pSrcContainer, sIterEnd);

	nSize = pSrcContainer->size ();

	switch (eTest)
	{
		case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART:
		{
			::std::advance<_t_iterator, int> (sIterBegin, nArg);
			::std::advance<_t_iterator, int> (sIterEnd, 0 - nArg);
			
			pDestContainer->insert (sIterBegin, sIterEnd);

			break;
		}

		case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT:
		{
			pDestContainer->insert (sIterBegin, sIterEnd);

			get_begin (pSrcContainer, rIterA);
			get_begin (pSrcContainer, rIterB);

			while (rIterA != sIterEnd)
			{
				pDestContainer->insert (rIterA, rIterB);

				rIterA++;
				rIterB++;

				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: equivalent or same test 1: container size changed!" << ::std::endl;

					exit (-1);
				}
			}

			get_begin (pDestContainer, rIterA);
			get_begin (pDestContainer, rIterB);

			get_end (pDestContainer, sIterEnd);

			while (rIterA != sIterEnd)
			{
				pDestContainer->insert (rIterA, rIterB);
		
				rIterA++;
				rIterB++;

				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: equivalent or same test 2: container size changed!" << ::std::endl;

					exit (-1);
				}
			}

			break;
		}

		case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE	:
		{
			for (ui32 = 0; ui32 < nNumInstances; ui32++)
			{
				pDestContainer->insert (sIterBegin, sIterEnd);
			}

			get_begin (pDestContainer, sIterBegin);
			get_end (pDestContainer, sIterEnd);

			::std::advance<_t_iterator, int> (sIterBegin, nArg);
			::std::advance<_t_iterator, int> (sIterEnd, 0 - nArg);

			if (!is_const_iterator (pDestContainer, sIterBegin))
			{
				if (!is_reverse_iterator (pDestContainer, sIterBegin))
				{
					typename _t_dest_container::test_iterator				sIter;

					pDestContainer->insert_self_reference (sIterBegin, sIterEnd, sIter);
				}
				else
				{
					typename _t_dest_container::test_reverse_iterator		sRIter;

					pDestContainer->insert_self_reference (sIterBegin, sIterEnd, sRIter);
				}
			}
			else
			{
				if (!is_reverse_iterator (pDestContainer, sIterBegin))
				{
					typename _t_dest_container::test_const_iterator			sCIter;

					pDestContainer->insert_self_reference (sIterBegin, sIterEnd, sCIter);
				}
				else
				{
					typename _t_dest_container::test_const_reverse_iterator	sCRIter;

					pDestContainer->insert_self_reference (sIterBegin, sIterEnd, sCRIter);
				}
			}

			break;
		}

		case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN:
		{
			container_assign (pContainer, sIterBegin, sIterEnd);

			get_begin (pContainer, sExtIterBegin);
			get_end (pContainer, sExtIterEnd);
	
			::std::advance<_t_ext_iterator, int> (sExtIterBegin, nArg);
			::std::advance<_t_ext_iterator, int> (sExtIterEnd, 0 - nArg);

			pDestContainer->insert (sExtIterBegin, sExtIterEnd);

			break;
		}

		case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN:
		{
			container_assign (pContainer, sIterBegin, sIterEnd);

			get_begin (pContainer, rExtIterA);
			get_begin (pContainer, rExtIterB);

			get_end (pContainer, sExtIterEnd);

			nSize = pDestContainer->size ();
		
			while (rExtIterA != sExtIterEnd)
			{
				pDestContainer->insert (rExtIterA, rExtIterB);
		
				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: empty equivalent or same extern test (A == B): container size changed!" << ::std::endl;

					exit (-1);
				}

				pDestContainer->insert (rExtIterB, rExtIterA);
		
				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: empty equivalent or same extern test (B == A): container size changed!" << ::std::endl;

					exit (-1);
				}

				pDestContainer->insert (rExtIterA, rExtIterA);
		
				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: empty equivalent or same extern test (A == A): container size changed!" << ::std::endl;

					exit (-1);
				}

				pDestContainer->insert (rExtIterB, rExtIterB);
		
				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: empty equivalent or same extern test (B == B): container size changed!" << ::std::endl;

					exit (-1);
				}

				rExtIterA++;
				rExtIterB++;
			}

			pDestContainer->insert (sIterBegin, sIterEnd);

			nSize = pDestContainer->size ();

			get_begin (pContainer, rExtIterA);
			get_begin (pContainer, rExtIterB);

			while (rExtIterA != sExtIterEnd)
			{
				pDestContainer->insert (rExtIterA, rExtIterB);
		
				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: equivalent or same extern test (A == B): container size changed!" << ::std::endl;

					exit (-1);
				}

				pDestContainer->insert (rExtIterB, rExtIterA);
		
				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: equivalent or same extern test (B == A): container size changed!" << ::std::endl;

					exit (-1);
				}

				pDestContainer->insert (rExtIterA, rExtIterA);
		
				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: equivalent or same extern test (A == A): container size changed!" << ::std::endl;

					exit (-1);
				}

				pDestContainer->insert (rExtIterB, rExtIterB);
		
				if (nSize != pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: equivalent or same extern test (B == B): container size changed!" << ::std::endl;

					exit (-1);
				}

				rExtIterA++;
				rExtIterB++;
			}

			break;
		}

		default:
		{
			::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorEx: unknown test!" << ::std::endl;

			exit (-1);

			break;
		}
	}

	pContainer->clear ();
	pDestContainer->clear ();
	pSrcContainer->clear ();
}

template <class _t_dest_container, class _t_src_container, class _t_data, class _t_dest_iterator, class _t_src_iterator>
void TestBTreeKeySortSTLifInsertViaIteratorPairEx (btreetest_keysort_stl_if_insert_via_iterator_e eTest, const char *pszTestTitle, int nArg, uint32_t nNumInstances, _t_dest_container *pDestContainer, _t_src_container *pSrcContainer, _t_data &rKeySortPairData, _t_dest_iterator &rDestIter, _t_src_iterator &rSrcIter, uint32_t nNumEntries)
{
	_t_src_iterator		sSrcIterBegin;
	_t_src_iterator		sSrcIterBeginB;
	_t_src_iterator		sSrcIterEnd;
	_t_dest_iterator	sDestIterBegin;
	_t_dest_iterator	sDestIterBeginB;
	_t_dest_iterator	sDestIterEnd;
	size_t				nSize;
	uint32_t			ui32;

	::std::cout << pszTestTitle << ::std::endl;

	for (ui32 = 0; ui32 < nNumEntries; ui32++)
	{
		_t_data 	sKeySortPairData (generate_rand32 (), {generate_rand32 (), g_nDebug});

		g_nDebug++;

		pSrcContainer->insert (sKeySortPairData);

		::std::cout << ui32 << " / " << nNumEntries << "\r";
	}

	::std::cout << ui32 << " / " << nNumEntries << ::std::endl << ::std::flush;

	get_begin (pSrcContainer, sSrcIterBegin);
	get_end (pSrcContainer, sSrcIterEnd);

	nSize = (size_t) pSrcContainer->size ();

	switch (eTest)
	{
		case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART:
		{
			::std::advance<_t_src_iterator, int> (sSrcIterBegin, nArg);
			::std::advance<_t_src_iterator, int> (sSrcIterEnd, 0 - nArg);
			
			pDestContainer->template insert <_t_src_iterator> (sSrcIterBegin, sSrcIterEnd);

			break;
		}

		case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT:
		{
			pDestContainer->template insert <_t_src_iterator> (sSrcIterBegin, sSrcIterEnd);

			while (sSrcIterBegin != sSrcIterEnd)
			{
				pDestContainer->template insert <_t_src_iterator> (sSrcIterBegin, sSrcIterBegin);

				::std::advance<_t_src_iterator, int> (sSrcIterBegin, 1);

				if (nSize != (size_t) pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorPairEx: equivalent or same test 1: container size changed!" << ::std::endl;

					exit (-1);
				}
			}

			get_begin (pSrcContainer, sSrcIterBegin);
			get_begin (pSrcContainer, sSrcIterBeginB);

			while (sSrcIterBegin != sSrcIterEnd)
			{
				pDestContainer->template insert <_t_src_iterator> (sSrcIterBegin, sSrcIterBeginB);

				::std::advance<_t_src_iterator, int> (sSrcIterBegin, 1);
				::std::advance<_t_src_iterator, int> (sSrcIterBeginB, 1);

				if (nSize != (size_t) pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorPairEx: equivalent or same test 2: container size changed!" << ::std::endl;

					exit (-1);
				}
			}

			get_begin (pSrcContainer, sSrcIterBegin);
			get_begin (pSrcContainer, sSrcIterBeginB);

			while (sSrcIterBegin != sSrcIterEnd)
			{
				pDestContainer->template insert <_t_src_iterator> (sSrcIterBeginB, sSrcIterBegin);

				::std::advance<_t_src_iterator, int> (sSrcIterBegin, 1);
				::std::advance<_t_src_iterator, int> (sSrcIterBeginB, 1);

				if (nSize != (size_t) pDestContainer->size ())
				{
					::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorPairEx: equivalent or same test 3: container size changed!" << ::std::endl;

					exit (-1);
				}
			}
			
			break;
		}

		default:
		{
			::std::cerr << "TestBTreeKeySortSTLifInsertViaIteratorPairEx: unknown test!" << ::std::endl;

			exit (-1);

			break;
		}
	}

	pSrcContainer->clear ();
	pDestContainer->clear ();
}

template <class _t_container, class _t_pair_container>
void TestBTreeKeySortSTLifInsertViaIterator (_t_container *pContainer, _t_pair_container *pPairContainer, uint32_t nNumEntries, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nNodeSize)
{
	typedef ::std::list<typename _t_container::value_type>						list_t;
	typedef ::std::vector<typename _t_container::value_type>						vector_t;
	typedef	::std::map<uint32_t, keySortMap_t>											map_t;
	typedef ::std::multimap<uint32_t, keySortMap_t>										mmap_t;

	typedef typename _t_container::const_iterator										citer_t;
	typedef typename _t_container::const_reverse_iterator								criter_t;

	typedef typename list_t::iterator													itlist_t;
	typedef typename list_t::const_iterator												citlist_t;
	typedef typename list_t::reverse_iterator											ritlist_t;
	typedef typename list_t::const_reverse_iterator										critlist_t;

	typedef typename vector_t::iterator													itvec_t;
	typedef typename vector_t::const_iterator											citvec_t;
	typedef typename vector_t::reverse_iterator											ritvec_t;
	typedef typename vector_t::const_reverse_iterator									critvec_t;

	typedef CBTreeKeySort<keySortEntry_t, uint32_t, size_t>								CBTreeKeySort_VectorFeed_t;

	typedef typename _t_pair_container::const_iterator									citer_pair_t;
	typedef typename _t_pair_container::const_reverse_iterator							criter_pair_t;

	typedef typename map_t::iterator													itmap_t;
	typedef typename map_t::const_iterator												citmap_t;
	typedef typename map_t::reverse_iterator											ritmap_t;
	typedef typename map_t::const_reverse_iterator										critmap_t;

	typedef typename mmap_t::iterator													itmmap_t;
	typedef typename mmap_t::const_iterator												citmmap_t;
	typedef typename mmap_t::reverse_iterator											ritmmap_t;
	typedef typename mmap_t::const_reverse_iterator										critmmap_t;
	
	_t_container								sContainerTarget (*pContainer);
	uint32_t									nLastKey = 0;
	citer_t										sCIterA, sCIterB;
	criter_t									sCRIterA, sCRIterB;

	list_t										sList;
	vector_t									sVector;

	itlist_t									sItListBegin, sItListEnd;
	citlist_t									sCItListBegin, sCItListEnd;
	ritlist_t									sRItListBegin, sRItListEnd;
	critlist_t									sCRItListBegin, sCRItListEnd;
	itvec_t										sItVecBegin, sItVecEnd;
	citvec_t									sCItVecBegin, sCItVecEnd;
	ritvec_t									sRItVecBegin, sRItVecEnd;
	critvec_t									sCRItVecBegin, sCRItVecEnd;
	CBTreeIOpropertiesRAM						sRAMproperties;
	_t_pair_container							sKeySortTargetPair (*pPairContainer);
	keySortPair_t								sKeySortPairData;
	citer_pair_t								sCIterTargetPair, sCIterPair;
	criter_pair_t								sCRIterTargetPair, sCRIterPair;
	map_t										sMap;
	mmap_t										sMMap;
	itmap_t										sItMap;
	citmap_t									sCItMap;
	ritmap_t									sRItMap;
	critmap_t									sCRItMap;
	itmmap_t									sItMMap;
	citmmap_t									sCItMMap;
	ritmmap_t									sRItMMap;
	critmmap_t									sCRItMMap;

	
	cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeySort<>:: insert<_t_iterator> (_t_iterator, _t_iterator)" << endl;

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_obj::citer_t> (citer_t, citer_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_obj::criter_t> (criter_t, criter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCRIterA, sCRIterB, sCRItListBegin, sCRItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<_t_obj::citer_t> (citer_t == citer_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<_t_obj::criter_t> (criter_t == criter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCRIterA, sCRIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_obj::citer_t> (>citer_t, citer_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_obj::criter_t> (>criter_t, criter_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sList, sCRIterA, sCRIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::citer_t> (target::citer_t, target::citer_t)", 0, 1, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::criter_t> (target::criter_t, target::criter_t)", 0, 1, &sContainerTarget, pContainer, &sList, sCRIterA, sCRIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::citer_t> x 4 (>target::citer_t, target::citer_t<)", (int) (nNumEntries / 4), 4, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, "target::insert<_t_obj::criter_t> x 4 (>target::criter_t, target::criter_t<)", (int) (nNumEntries / 4), 4, &sContainerTarget, pContainer, &sList, sCRIterA, sCRIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::iter_t> (iter_t, iter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sItListBegin, sItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::citer_t> (citer_t, citer_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::riter_t> (riter_t, riter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sRItListBegin, sRItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::criter_t> (criter_t, criter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCRItListBegin, sCRItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::iter_t> (>iter_t, iter_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sItListBegin, sItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::citer_t> (>citer_t, citer_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::riter_t> (>riter_t, riter_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sRItListBegin, sRItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<list::criter_t> (>criter_t, criter_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCRItListBegin, sCRItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<list::iter_t> (iter_t == iter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sItListBegin, sItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<list::citer_t> (citer_t == citer_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCItListBegin, sCItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<list::riter_t> (riter_t == riter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sRItListBegin, sRItListEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<list::criter_t> (criter_t == criter_t)", 0, 0, &sContainerTarget, pContainer, &sList, sCIterA, sCIterB, sCRItListBegin, sCRItListEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::iter_t> (iter_t, iter_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sItVecBegin, sItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::citer_t> (citer_t, citer_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sCItVecBegin, sCItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::riter_t> (riter_t, riter_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sRItVecBegin, sRItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::criter_t> (criter_t, criter_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sCRItVecBegin, sCRItVecEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::iter_t> (>iter_t, iter_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sItVecBegin, sItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::citer_t> (>citer_t, citer_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sCItVecBegin, sCItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::riter_t> (>riter_t, riter_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sRItVecBegin, sRItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, "target::insert<vector::criter_t> (>criter_t, criter_t<)", (int) (nNumEntries / 4), 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sCRItVecBegin, sCRItVecEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<vector::iter_t> (iter_t == iter_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sItVecBegin, sItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<vector::citer_t> (citer_t == citer_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sCItVecBegin, sCItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<vector::riter_t> (riter_t == riter_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sRItVecBegin, sRItVecEnd, nNumEntries, nLastKey);
	TestBTreeKeySortSTLifInsertViaIteratorEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, "target::insert<vector::criter_t> (criter_t == criter_t)", 0, 0, &sContainerTarget, pContainer, &sVector, sCIterA, sCIterB, sCRItVecBegin, sCRItVecEnd, nNumEntries, nLastKey);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::citer_t> (citer_t, citer_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCIterTargetPair, sCIterPair, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::criter_t> (criter_t, criter_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCRIterTargetPair, sCRIterPair, nNumEntries);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<_t_container<pair>::citer_t> (citer_t == citer_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCIterTargetPair, sCIterPair, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<_t_container<pair>::criter_t> (criter_t == criter_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCRIterTargetPair, sCRIterPair, nNumEntries);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::citer_t> (>citer_t, citer_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCIterTargetPair, sCIterPair, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::criter_t> (>criter_t, criter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCRIterTargetPair, sCRIterPair, nNumEntries);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::citer_t> (target::citer_t, target::citer_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCIterTargetPair, sCIterPair, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::criter_t> (target::criter_t, target::criter_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCRIterTargetPair, sCRIterPair, nNumEntries);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<_t_container<pair>::citer_t> (target::citer_t == target::citer_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCIterTargetPair, sCIterPair, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<_t_container<pair>::criter_t> (target::criter_t == target::criter_t)", 0, 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCRIterTargetPair, sCRIterPair, nNumEntries);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::citer_t> (>target::citer_t, target::citer_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCIterTargetPair, sCIterPair, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<_t_container<pair>::criter_t> (>target::criter_t, target::criter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, pPairContainer, sKeySortPairData, sCRIterTargetPair, sCRIterPair, nNumEntries);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::iter_t> (iter_t, iter_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sItMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::citer_t> (citer_t, citer_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sCItMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::riter_t> (riter_t, riter_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sRItMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::criter_t> (criter_t, criter_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sCRItMap, nNumEntries);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::map::iter_t> (iter_t == iter_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sItMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::map::citer_t> (citer_t == citer_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sCItMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::map::riter_t> (riter_t == riter_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sRItMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::map::criter_t> (criter_t == criter_t)", 0, 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sCRItMap, nNumEntries);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::iter_t> (>iter_t, iter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sItMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::citer_t> (>citer_t, citer_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sCItMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::riter_t> (>riter_t, riter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sRItMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::map::criter_t> (>criter_t, criter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMap, sKeySortPairData, sCIterTargetPair, sCRItMap, nNumEntries);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::iter_t> (iter_t, iter_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sItMMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::citer_t> (citer_t, citer_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sCItMMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::riter_t> (riter_t, riter_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sRItMMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::criter_t> (criter_t, criter_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sCRItMMap, nNumEntries);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::multimap::iter_t> (iter_t == iter_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sItMMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::multimap::citer_t> (citer_t == citer_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sCItMMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::multimap::riter_t> (riter_t == riter_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sRItMMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, "target::insert<std::multimap::criter_t> (criter_t == criter_t)", 0, 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sCRItMMap, nNumEntries);

	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::iter_t> (>iter_t, iter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sItMMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::citer_t> (>citer_t, citer_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sCItMMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::riter_t> (>riter_t, riter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sRItMMap, nNumEntries);
	TestBTreeKeySortSTLifInsertViaIteratorPairEx (BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, "target::insert<std::multimap::criter_t> (>criter_t, criter_t<)", (int) (nNumEntries / 4), 0, &sKeySortTargetPair, &sMMap, sKeySortPairData, sCIterTargetPair, sCRItMMap, nNumEntries);
}

template <class _t_container, class _t_pair_container>
void TestBTreeKeySortSTLifSwap (_t_container *pContainer, _t_pair_container *pPairContainer, uint32_t nNumEntries)
{
	typedef typename _t_container::const_iterator						citer_t;

	typedef typename _t_pair_container::const_iterator					citer_pair_t;
	
	uint32_t						nLastKey = 0;
	_t_container					sContainerTarget (*pContainer);
	_t_container					sContainerRef (*pContainer);
	_t_container					sContainerTargetRef (*pContainer);
	_t_pair_container				sContainerPairTarget (*pPairContainer);
	_t_pair_container				sContainerPairRef (*pPairContainer);
	_t_pair_container				sContainerPairTargetRef (*pPairContainer);
	uint32_t						i;

	cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeysort<>:: swap ()" << endl;

	associative_container_add_primitive (pContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	sContainerRef.template insert<citer_t> (pContainer->cbegin (), pContainer->cend ());

	if ((*pContainer) != sContainerRef)
	{
		cerr << "ERROR: Unexpected keysort mismatch!" << endl;

		exit (-1);
	}

	associative_container_add_primitive (&sContainerTarget, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	sContainerTargetRef.template insert<citer_t> (sContainerTarget.cbegin (), sContainerTarget.cend ());

	if (sContainerTarget != sContainerTargetRef)
	{
		cerr << "ERROR: Unexpected target mismatch!" << endl;

		exit (-1);
	}

	pContainer->swap (sContainerTarget);

	if ((*pContainer) != sContainerTargetRef)
	{
		cerr << "ERROR: array mismatches target reference!" << endl;

		exit (-1);
	}

	if (sContainerTarget != sContainerRef)
	{
		cerr << "ERROR: target mismatches array reference!" << endl;

		exit (-1);
	}

	sContainerTarget.swap (*pContainer);

	if ((*pContainer) != sContainerRef)
	{
		cerr << "ERROR: array mismatches array reference!" << endl;

		exit (-1);
	}

	if (sContainerTarget != sContainerTargetRef)
	{
		cerr << "ERROR: target mismatches target reference!" << endl;

		exit (-1);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		keySortPair_t			sEntryPair (generate_rand32 (), {generate_rand32 (), g_nDebug});
		
		g_nDebug++;

		pPairContainer->insert (sEntryPair);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sContainerPairRef.template insert<citer_pair_t> (pPairContainer->cbegin (), pPairContainer->cend ());

	if (((_t_pair_container &) *pPairContainer) != sContainerPairRef)
	{
		cerr << "ERROR: Unexpected keysort mismatch! (pair)" << endl;

		exit (-1);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		keySortPair_t			sEntryPair (generate_rand32 (), {generate_rand32 (), g_nDebug});

		g_nDebug++;

		sContainerPairTarget.insert (sEntryPair);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sContainerPairTargetRef.template insert<citer_pair_t> (sContainerPairTarget.cbegin (), sContainerPairTarget.cend ());

	if (sContainerPairTarget != sContainerPairTargetRef)
	{
		cerr << "ERROR: Unexpected target mismatch! (pair)" << endl;

		exit (-1);
	}

	pPairContainer->swap (sContainerPairTarget);

	if (((_t_pair_container &) *pPairContainer) != sContainerPairTargetRef)
	{
		cerr << "ERROR: array mismatches target reference! (pair)" << endl;

		exit (-1);
	}

	if (sContainerPairTarget != sContainerPairRef)
	{
		cerr << "ERROR: target mismatches array reference! (pair)" << endl;

		exit (-1);
	}

	sContainerPairTarget.swap (((_t_pair_container &) *pPairContainer));

	if (((_t_pair_container &) *pPairContainer) != sContainerPairRef)
	{
		cerr << "ERROR: array mismatches array reference! (pair)" << endl;

		exit (-1);
	}

	if (sContainerPairTarget != sContainerPairTargetRef)
	{
		cerr << "ERROR: target mismatches target reference! (pair)" << endl;

		exit (-1);
	}
}

template <class _t_container, class _t_pair_container>
void TestBTreeKeySortSTLifFind (_t_container *pContainer, _t_pair_container *pPairContainer, uint32_t nNumEntries)
{
	typedef typename _t_container::const_iterator				citer_t;

	typedef typename _t_pair_container::const_iterator			citer_pair_t;
	
	uint32_t						nLastKey = 1;
	uint32_t						i;
	citer_t							sCIter;
	citer_t							sCIterRslt;
	citer_pair_t					sCItPair;
	citer_pair_t					sCItPairRslt;

	cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeysort<>:: find ()" << endl;

	associative_container_add_primitive (pContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_ASCEND);

	for (sCIter = pContainer->cbegin (); sCIter != pContainer->cend (); sCIter++)
	{
		sCIterRslt = pContainer->find (get_entry_key (*sCIter));

		if (sCIter != sCIterRslt)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifFind: ERROR: iterator mismatch!" << endl;

			exit (-1);
		}
	}

	sCIterRslt = pContainer->find (0);

	if (sCIterRslt != pContainer->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on lower bound check!" << endl;

		exit (-1);
	}

	sCIterRslt = pContainer->find (nNumEntries + 1);

	if (sCIterRslt != pContainer->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on upper bound check!" << endl;

		exit (-1);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		keySortPair_t		sEntryPair (i + 1, {generate_rand32 (), g_nDebug});
		
		g_nDebug++;

		pPairContainer->insert (sEntryPair);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	for (sCItPair = pPairContainer->cbegin (); sCItPair != pPairContainer->cend (); sCItPair++)
	{
		sCItPairRslt = pPairContainer->find (get_entry_key (*sCItPair));

		if (sCItPair != sCItPairRslt)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifFind: ERROR: iterator mismatch! (pair)" << endl;

			exit (-1);
		}
	}

	sCItPairRslt = pPairContainer->find (0);

	if (sCItPairRslt != pPairContainer->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on lower bound check! (pair)" << endl;

		exit (-1);
	}

	sCItPairRslt = pPairContainer->find (nNumEntries + 1);

	if (sCItPairRslt != pPairContainer->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on upper bound check! (pair)" << endl;

		exit (-1);
	}
}

template <class _t_container, class _t_pair_container>
void TestBTreeKeySortSTLifLowerBoundUpperBound (_t_container *pContainer, _t_pair_container *pPairContainer, uint32_t nNumEntries, uint32_t nInstance)
{
	typedef typename _t_container::const_iterator			citer_t;

	typedef typename _t_pair_container::const_iterator		citer_pair_t;
	
	uint32_t						nLastKey;
	uint32_t						i;
	uint32_t						j;
	citer_t							sCIterLower;
	citer_t							sCIterUpper;
	citer_pair_t					sCItPairLower;
	citer_pair_t					sCItPairUpper;

	cout << "exercises methods compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeysort<>:: lower_bound () upper_bound () count ()" << endl;

	for (i = 0; i < nInstance; i++)
	{
		nLastKey = 1;

		associative_container_add_primitive (pContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_ASCEND);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sCIterLower = pContainer->lower_bound (i + 1);
		sCIterUpper = pContainer->upper_bound (i + 1);

		::std::advance (sCIterLower, (int) nInstance);

		if (sCIterLower != sCIterUpper)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: boundary mismatch!" << endl;

			exit (-1);
		}

		if (pContainer->count (i + 1) != nInstance)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: count () returned with unexpected value!" << endl;

			exit (-1);
		}
	}

	sCIterLower = pContainer->lower_bound (0);

	if (sCIterLower != pContainer->cbegin ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: lower_bound () didn't return cbegin () when testing for lowest key!" << endl;

		exit (-1);
	}

	sCIterUpper = pContainer->upper_bound (nNumEntries + 1);

	if (sCIterUpper != pContainer->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: upper_bound () didn't return cend () when testing for lowest key!" << endl;

		exit (-1);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		for (j = 0; j < nInstance; j++)
		{
			keySortPair_t			sEntryPair (i + 1, {generate_rand32 (), g_nDebug});

			g_nDebug++;

			pPairContainer->insert (sEntryPair);
		}

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sCItPairLower = pPairContainer->lower_bound (i + 1);
		sCItPairUpper = pPairContainer->upper_bound (i + 1);

		::std::advance (sCItPairLower, (int) nInstance);

		if (sCItPairLower != sCItPairUpper)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: boundary mismatch! (pair)" << endl;

			exit (-1);
		}

		if (pPairContainer->count (i + 1) != nInstance)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: count () returned with unexpected value! (pair)" << endl;

			exit (-1);
		}
	}

	sCItPairLower = pPairContainer->lower_bound (0);

	if (sCItPairLower != pPairContainer->cbegin ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: lower_bound () didn't return cbegin () when testing for lowest key! (pair)" << endl;

		exit (-1);
	}

	sCItPairUpper = pPairContainer->upper_bound (nNumEntries + 1);

	if (sCItPairUpper != pPairContainer->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: upper_bound () didn't return cend () when testing for lowest key! (pair)" << endl;

		exit (-1);
	}
}

template <class _t_container, class _t_pair_container>
void TestBTreeKeySortSTLifEraseViaIterator (_t_container *pContainer, _t_pair_container *pPairContainer, uint32_t nNumEntries)
{
	typedef typename _t_container::const_iterator					citer_t;

	typedef typename _t_container::size_type						size_type;

	typedef typename _t_pair_container::const_iterator				citer_pair_t;

	typedef typename _t_pair_container::size_type					size_type_pair;
	
	uint32_t									nLastKey = 0;
	citer_t										sCIter;
	citer_t										sCIterRslt;
	size_type									nIter;
	size_type									nIterRslt;
	uint32_t									i;
	citer_pair_t								sCIterPair;
	citer_pair_t								sCIterPairRslt;
	size_type_pair								nIterPair;
	size_type_pair								nIterPairRslt;

	cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeySort<>:: erase (const_iterator)" << endl;

	associative_container_add_primitive (pContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);

	while (pContainer->size () > 0)
	{
		sCIter = pContainer->cbegin ();

		::std::advance (sCIter, (int) (generate_rand32 () % pContainer->size ()));

		nIter = ::std::distance (pContainer->cbegin (), sCIter);

		sCIterRslt = pContainer->erase (sCIter);

		nIterRslt = std::distance (pContainer->cbegin (), sCIterRslt);

		if (nIter != nIterRslt)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifEraseViaIterator: returned iterator mismatches!" << endl;

			exit (-1);
		}
	}

	for (i = 0; i < nNumEntries; i++)
	{
		keySortMap_t 		sKeySortMap = {generate_rand32 (), g_nDebug};
		keySortPair_t		sKeySortPairData (generate_rand32 (), sKeySortMap);

		g_nDebug++;

		pPairContainer->insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	while (pPairContainer->size () > 0)
	{
		sCIterPair = pPairContainer->cbegin ();

		::std::advance (sCIterPair, (int) (generate_rand32 () % pPairContainer->size ()));

		nIterPair = ::std::distance (pPairContainer->cbegin (), sCIterPair);

		sCIterPairRslt = pPairContainer->erase (sCIterPair);

		nIterPairRslt = ::std::distance (pPairContainer->cbegin (), sCIterPairRslt);

		if (nIterPair != nIterPairRslt)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifEraseViaIterator: returned iterator (pair type) mismatches!" << endl;

			exit (-1);
		}
	}
}

template <class _t_container, class _t_pair_container>
void TestBTreeKeySortSTLifEraseViaKey (_t_container *pContainer, _t_pair_container *pPairContainer, uint32_t nNumEntries, uint32_t nInstances)
{
	typedef typename _t_container::const_iterator										citer_t;

	typedef typename _t_container::size_type											size_type;

	typedef typename _t_pair_container::const_iterator									citer_pair_t;
	
	uint32_t									nLastKey;
	citer_t										sCIter;
	uint32_t									i;
	size_type									nRslt;
	uint32_t									j;
	citer_pair_t								sCIterPair;
	uint32_t									nKey;

	cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeySort<>:: erase (const _t_key)" << endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		associative_container_add_primitive (pContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);
	}

	while (pContainer->size () > 0)
	{
		sCIter = pContainer->cbegin ();

		::std::advance (sCIter, (int) (generate_rand32 () % pContainer->size ()));

		nKey = (*sCIter).first;

		nRslt = pContainer->erase (nKey);

		if (nRslt != nInstances)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifEraseViaKey: unexpected number of items deleted!" << endl;

			exit (-1);
		}
	}

	for (i = 0; i < nNumEntries; i++)
	{
		nKey = generate_rand32 ();

		for (j = 0; j < nInstances; j++)
		{
			keySortMap_t					sKeySortMap = {generate_rand32 (), g_nDebug};
			keySortPair_t					sKeySortPairData (nKey, sKeySortMap);

			g_nDebug++;

			pPairContainer->insert (sKeySortPairData);
		}

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	while (pPairContainer->size () > 0)
	{
		sCIterPair = pPairContainer->cbegin ();

		::std::advance (sCIterPair, (int) (generate_rand32 () % pPairContainer->size ()));

		nKey = (*sCIterPair).first;

		nRslt = pPairContainer->erase (nKey);

		if (nRslt != nInstances)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifEraseViaKey: unexpected number of items deleted! (pair)" << endl;

			exit (-1);
		}
	}
}

template <class _t_container, class _t_pair_container>
void TestBTreeKeySortSTLifEraseViaIteratorMultiple (_t_container *pContainer, _t_pair_container *pPairContainer, uint32_t nNumEntries, uint32_t nInstances)
{
	typedef typename _t_container::const_iterator				citer_t;

	typedef typename _t_container::size_type					size_type;

	typedef typename _t_pair_container::const_iterator			citer_pair_t;

	typedef typename _t_pair_container::size_type				size_type_pair;
	
	uint32_t									nDebug = 0;
	uint32_t									nLastKey;
	citer_t										sCIterBegin;
	citer_t										sCIterEnd;
	citer_t										sCIterRslt;
	size_type									nIterBegin;
	size_type									nIterRslt;
	uint32_t									i;
	uint32_t									j;
	citer_pair_t								sCIterPairBegin;
	citer_pair_t								sCIterPairEnd;
	citer_pair_t								sCIterPairRslt;
	size_type_pair								nIterPairBegin;
	size_type_pair								nIterPairRslt;
	uint32_t									nKey;

	cout << "exercises method compatible to STL interface CBTreeBaseDefaults<>::CBTreeKeySort<>:: erase (const_iterator, const_iterator)" << endl;

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		associative_container_add_primitive (pContainer, nNumEntries, nLastKey, BTREETEST_KEY_GENERATION_RANDOM);
	}

	while (pContainer->size () > 0)
	{
		sCIterBegin = pContainer->cbegin ();
		sCIterEnd = pContainer->cend ();

		if (pContainer->size () > 1)
		{
			::std::advance (sCIterBegin, (int) (generate_rand32 () % (pContainer->size () / 2)));
			::std::advance (sCIterEnd, 0 - ((int) (generate_rand32 () % (pContainer->size () / 2))));
		}

		nIterBegin = ::std::distance (pContainer->cbegin (), sCIterBegin);

		sCIterRslt = pContainer->erase (sCIterBegin, sCIterEnd);

		nIterRslt = ::std::distance (pContainer->cbegin (), sCIterRslt);

		if (nIterRslt != nIterBegin)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifEraseViaIteratorMultiple: returned iterator mismatches!" << endl;

			exit (-1);
		}
	}

	for (i = 0; i < nNumEntries; i++)
	{
		nKey = generate_rand32 ();

		for (j = 0; j < nInstances; j++)
		{
			keySortMap_t				sKeySortMap = {generate_rand32 (), g_nDebug};
			keySortPair_t				sKeySortPairData (nKey, sKeySortMap);

			g_nDebug++;

			pPairContainer->insert (sKeySortPairData);
		}

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	while (pPairContainer->size () > 0)
	{
		sCIterPairBegin = pPairContainer->cbegin ();
		sCIterPairEnd = pPairContainer->cend ();

		::std::advance (sCIterPairBegin, (int) (generate_rand32 () % (pPairContainer->size () / 2)));
		::std::advance (sCIterPairEnd, 0 - (int) (generate_rand32 () % (pPairContainer->size () / 2)));

		nIterPairBegin = ::std::distance (pPairContainer->cbegin (), sCIterPairBegin);

		sCIterPairRslt = pPairContainer->erase (sCIterPairBegin, sCIterPairEnd);

		nIterPairRslt = ::std::distance (pPairContainer->cbegin (), sCIterPairRslt);

		if (nIterPairRslt != nIterPairBegin)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifEraseViaKey: returned iterator (pair type) mismatches!" << endl;

			exit (-1);
		}
	}
}

void TestBTreeKeySort (uint32_t nTest, uint32_t nNodeSize, uint32_t nPageSize, CBTreeKeySortTestWrapper<keySortEntry_t, keySortEntry_t, keysort_reference_t> *pKeySortTestWrapper, CBTreeKeySortTestWrapper<keySortPair_t, keySortPair_t, keysort_reference_t> *pKeySortPairTestWrapper)
{
	cout << "b-tree keysort test bench selected" << endl;

	switch (nTest)
	{
	case BTREETEST_KEYSORT_BASIC_ASCEND			:
		{
			TestBTreeKeySortBasic (pKeySortTestWrapper, 512, 256, 3, BTREETEST_KEY_GENERATION_ASCEND, BTREETEST_KEY_GENERATION_ASCEND);
			TestBTreeKeySortBasic (pKeySortPairTestWrapper, 512, 256, 3, BTREETEST_KEY_GENERATION_ASCEND, BTREETEST_KEY_GENERATION_ASCEND);

			break;
		}

	case BTREETEST_KEYSORT_BASIC_ASCEND_SMALL	:
		{
			TestBTreeKeySortBasic (pKeySortTestWrapper, 16, 0, 0, BTREETEST_KEY_GENERATION_ASCEND, BTREETEST_KEY_GENERATION_ASCEND);
			TestBTreeKeySortBasic (pKeySortPairTestWrapper, 16, 0, 0, BTREETEST_KEY_GENERATION_ASCEND, BTREETEST_KEY_GENERATION_ASCEND);

			break;
		}

	case BTREETEST_KEYSORT_BASIC_DESCEND			:
		{
			TestBTreeKeySortBasic (pKeySortTestWrapper, 512, 256, 3, BTREETEST_KEY_GENERATION_DESCEND, BTREETEST_KEY_GENERATION_DESCEND);
			TestBTreeKeySortBasic (pKeySortPairTestWrapper, 512, 256, 3, BTREETEST_KEY_GENERATION_DESCEND, BTREETEST_KEY_GENERATION_DESCEND);

			break;
		}

	case BTREETEST_KEYSORT_BASIC_DESCEND_SMALL	:
		{
			TestBTreeKeySortBasic (pKeySortTestWrapper, 16, 0, 0, BTREETEST_KEY_GENERATION_DESCEND, BTREETEST_KEY_GENERATION_DESCEND);
			TestBTreeKeySortBasic (pKeySortPairTestWrapper, 16, 0, 0, BTREETEST_KEY_GENERATION_DESCEND, BTREETEST_KEY_GENERATION_DESCEND);

			break;
		}

	case BTREETEST_KEYSORT_BASIC_RANDOM		:
		{
			TestBTreeKeySortBasic (pKeySortTestWrapper, 512, 256, 3, BTREETEST_KEY_GENERATION_RANDOM, BTREETEST_KEY_GENERATION_RANDOM);
			TestBTreeKeySortBasic (pKeySortPairTestWrapper, 512, 256, 3, BTREETEST_KEY_GENERATION_RANDOM, BTREETEST_KEY_GENERATION_RANDOM);

			break;
		}

	case BTREETEST_KEYSORT_BASIC_RANDOM_SMALL	:
		{
			TestBTreeKeySortBasic (pKeySortTestWrapper, 16, 0, 0, BTREETEST_KEY_GENERATION_RANDOM, BTREETEST_KEY_GENERATION_RANDOM);
			TestBTreeKeySortBasic (pKeySortPairTestWrapper, 16, 0, 0, BTREETEST_KEY_GENERATION_RANDOM, BTREETEST_KEY_GENERATION_RANDOM);

			break;
		}

	case BTREETEST_KEYSORT_CLEAR			:
		{
			TestBTreeKeySortRemoveAll (pKeySortTestWrapper, 256);
			TestBTreeKeySortRemoveAll (pKeySortPairTestWrapper, 256);

			break;
		}

	case BTREETEST_KEYSORT_REPLACE			:
		{
			TestBTreeKeySortMultiRemove (pKeySortTestWrapper, 64);
			TestBTreeKeySortMultiRemove (pKeySortPairTestWrapper, 64);

			break;
		}

	case BTREETEST_KEYSORT_UNLOAD				:
		{
			TestBTreeKeySortUnload (pKeySortTestWrapper, 128, 128, 128, 128);
			TestBTreeKeySortUnload (pKeySortPairTestWrapper, 128, 128, 128, 128);

			break;
		}

	case BTREETEST_KEYSORT_REMOVE_INSTANCES		:
		{
			TestBTreeKeySortRemoveInstance (pKeySortTestWrapper, 64, 3);
			TestBTreeKeySortRemoveInstance (pKeySortPairTestWrapper, 64, 3);

			break;
		}

	case BTREETEST_KEYSORT_CODE_COVERAGE_DETERMINE_POSITION	:
		{
			TestBTreeKeySortCCdeterminePosition (pKeySortTestWrapper, nNodeSize);
			TestBTreeKeySortCCdeterminePosition (pKeySortPairTestWrapper, nNodeSize);

			break;
		}

	case BTREETEST_KEYSORT_CODE_COVERAGE_FIND_FIRST_KEY		:
		{
			TestBTreeKeySortCCfindFirstKey (pKeySortTestWrapper, nNodeSize);
			TestBTreeKeySortCCfindFirstKey (pKeySortPairTestWrapper, nNodeSize);

			break;
		}

	case BTREETEST_KEYSORT_HTML_OUTPUT						:
		{
			TestBTreeKeySortHTMLoutput (pKeySortTestWrapper, 64);
			TestBTreeKeySortHTMLoutput (pKeySortPairTestWrapper, 64);

			break;
		}

	case BTREETEST_KEYSORT_COPY_CONSTRUCTOR					:
		{
			TestBTreeKeySortCopyConstructor (pKeySortTestWrapper, 64);
			TestBTreeKeySortCopyConstructor (pKeySortPairTestWrapper, 64);

			break;
		}

	case BTREETEST_KEYSORT_OPERATOR_OVERLOAD_ASSIGN			:
		{
			TestBTreeKeySortOperatorOverloadAssign (pKeySortTestWrapper, 64);
			TestBTreeKeySortOperatorOverloadAssign (pKeySortPairTestWrapper, 64);

			break;
		}

	case BTREETEST_KEYSORT_OPERATOR_OVERLOAD_ASSIGN_MULTI_INSTANCE	:
		{
			TestBTreeKeySortOperatorOverloadAssignMultiInstance (pKeySortTestWrapper, 64, 32, 5);
			TestBTreeKeySortOperatorOverloadAssignMultiInstance (pKeySortPairTestWrapper, 64, 32, 5);

			break;
		}

	case BTREETEST_KEYSORT_GET_INIT_POS_OF_KEY_ON_LEAF_NODE	:
		{
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize * 2);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize * 3);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize * 4);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize * 5);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize * 6);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize * 7);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortTestWrapper, nNodeSize * 8);

			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize * 2);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize * 3);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize * 4);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize * 5);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize * 6);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize * 7);
			TestBTreeKeySortLowerBoundOnLeafNode (pKeySortPairTestWrapper, nNodeSize * 8);

			break;
		}
	case BTREETEST_KEYSORT_SERLIALIZE_ALL					:
		{
			TestBTreeKeySortSerialize (pKeySortTestWrapper, 1, 128, 128);
			TestBTreeKeySortSerialize (pKeySortPairTestWrapper, 1, 128, 128);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_HALF_NODE				:
		{
			TestBTreeKeySortSerialize (pKeySortTestWrapper, 1, 128, 64);
			TestBTreeKeySortSerialize (pKeySortPairTestWrapper, 1, 128, 64);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_PART_NODE				:
		{
			TestBTreeKeySortSerialize (pKeySortTestWrapper, 1, 128, nNodeSize - 1);
			TestBTreeKeySortSerialize (pKeySortPairTestWrapper, 1, 128, nNodeSize - 1);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_INCOMPLETE				:
		{
			TestBTreeKeySortSerializeIncomplete (pKeySortTestWrapper, nNodeSize, 128, 128, 64);
			TestBTreeKeySortSerializeIncomplete (pKeySortPairTestWrapper, nNodeSize, 128, 128, 64);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_MULTI_INSTANCE_ALL		:
		{
			TestBTreeKeySortSerialize (pKeySortTestWrapper, 2, 64, 128);
			TestBTreeKeySortSerialize (pKeySortPairTestWrapper, 2, 64, 128);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_MULTI_INSTANCE_HALF_NODE	:
		{
			TestBTreeKeySortSerialize (pKeySortTestWrapper, 2, 64, 64);
			TestBTreeKeySortSerialize (pKeySortPairTestWrapper, 2, 64, 64);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_MULTI_INSTANCE_PART_NODE	:
		{
			TestBTreeKeySortSerialize (pKeySortTestWrapper, 2, 64, nNodeSize - 1);
			TestBTreeKeySortSerialize (pKeySortPairTestWrapper, 2, 64, nNodeSize - 1);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_INSERT:
		{
			TestBTreeKeySortSTLifInsert (pKeySortTestWrapper, 64);
			TestBTreeKeySortSTLifInsert (pKeySortPairTestWrapper, 64);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR	:
		{
			CBTreeIOpropertiesRAM		sRAMproperties;
			CBTreeIOpropertiesFile		sFileProperties ("./");
			bayerTreeCacheDescription_t	sCacheDesc = {nPageSize};

			TestBTreeKeySortSTLifInsertViaIterator (pKeySortTestWrapper, pKeySortPairTestWrapper, 64, &sCacheDesc, nNodeSize);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_ERASE_VIA_ITERATOR	:
		{
			TestBTreeKeySortSTLifEraseViaIterator (pKeySortTestWrapper, pKeySortPairTestWrapper, 64);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_ERASE_VIA_KEY:
		{
			TestBTreeKeySortSTLifEraseViaKey (pKeySortTestWrapper, pKeySortPairTestWrapper, 64, 5);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE	:
		{
			TestBTreeKeySortSTLifEraseViaIteratorMultiple (pKeySortTestWrapper, pKeySortPairTestWrapper, 64, 5);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_SWAP	:
		{
			TestBTreeKeySortSTLifSwap (pKeySortTestWrapper, pKeySortPairTestWrapper, 128);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_FIND	:
		{
			TestBTreeKeySortSTLifFind (pKeySortTestWrapper, pKeySortPairTestWrapper, 128);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_LOWER_BOUND_UPPER_BOUND	:
		{
			TestBTreeKeySortSTLifLowerBoundUpperBound (pKeySortTestWrapper, pKeySortPairTestWrapper, 64, 5);

			break;
		}

	default									:
		{
			cerr << "ERROR: TestBTreeKeySort: Unknown test or test not specified!" << endl;

			break;
		}
	}
}
