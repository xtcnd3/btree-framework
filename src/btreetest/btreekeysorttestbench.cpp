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

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortBasic (_t_obj *pClKeySort, uint32_t nMaxEntries, uint32_t nVariation, uint32_t nRepetitions, btreetest_keysort_primitive_seek_e eWhereAdd, btreetest_keysort_primitive_seek_e eWhereRemove)
{
	uint32_t			nDebug = 0;
	uint32_t			ui32;
	uint32_t			nLastKey;
	uint32_t			nLastKeyInit;
	_t_objprim			*pClKeySortPrim;

	cout << "basic test adds data at ";

	switch (eWhereAdd)
	{
	case BTREETEST_KEYSORT_PRIMITIVE_DESCEND_KEY	:
		{
			cout << "the beginning ";

			nLastKeyInit = ~0x0;

			break;
		}

	case BTREETEST_KEYSORT_PRIMITIVE_ASCEND_KEY		:
		{
			cout << "the end ";

			nLastKeyInit = 0x0;

			break;
		}

	case BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY		:
		{
			cout << "a random position ";

			nLastKeyInit = 0x0;

			break;
		}

	case BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY		:
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

	switch (eWhereRemove)
	{
	case BTREETEST_KEYSORT_PRIMITIVE_DESCEND_KEY	:
		{
			cout << "the beginning";

			break;
		}

	case BTREETEST_KEYSORT_PRIMITIVE_ASCEND_KEY		:
		{
			cout << "the end";

			break;
		}

	case BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY		:
		{
			cout << "a random position";

			break;
		}

	case BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY		:
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

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	nLastKey = nLastKeyInit;

	keySortPrim_add (pClKeySortPrim, nMaxEntries, nDebug, nLastKey, eWhereAdd);

	for (ui32 = 0; ui32 < nRepetitions; ui32++)
	{
		cout << "cycle: " << ui32 + 1 << endl;

		nLastKey = nLastKeyInit;

		keySortPrim_remove (pClKeySortPrim, nVariation, ~0x0, nLastKey, eWhereRemove);

		nLastKey = nLastKeyInit;

		keySortPrim_add (pClKeySortPrim, nVariation, nDebug, nLastKey, eWhereAdd);
	}

	nLastKey = nLastKeyInit;

	keySortPrim_remove (pClKeySortPrim, nMaxEntries, ~0x0, nLastKey, eWhereRemove);

	cout << "finished..." << endl;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortRemoveAll (_t_obj *pClKeySort, uint32_t nEntries)
{
	uint32_t			nDebug = 0;
	uint32_t			nLastKey = 0;
	_t_objprim			*pClKeySortPrim;

	cout << "test inserts data and empties the data container by calling clear" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	pClKeySort->clear ();
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortMultiRemove (_t_obj *pClKeySort, uint32_t nEntries)
{
	uint32_t			nDebug = 0;
	uint32_t			nLastKey = 0;
	_t_objprim			*pClKeySortPrim;

	cout << "test removes key sets" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	keySortPrim_remove (pClKeySortPrim, nEntries, ~0x0, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortUnload (_t_obj *pClKeySort, uint32_t nInitEntries, uint32_t nAddEntries, uint32_t nRemoveEntries, uint32_t nExitEntries)
{
	uint32_t			nDebug = 0;
	uint32_t			nLastKey = 0;
	_t_objprim			*pClKeySortPrim;

	cout << "test uses unload after every operation" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	cout << "unloading..." << endl;

	pClKeySort->unload ();

	keySortPrim_add (pClKeySortPrim, nInitEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	cout << "unloading..." << endl;
	
	pClKeySort->unload ();

	nLastKey++;

	keySortPrim_add (pClKeySortPrim, nAddEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	cout << "unloading..." << endl;
	
	pClKeySort->unload ();

	keySortPrim_remove (pClKeySortPrim, nRemoveEntries, ~0x0, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	cout << "unloading..." << endl;

	pClKeySort->unload ();

	nLastKey--;

	keySortPrim_remove (pClKeySortPrim, nExitEntries, ~0x0, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
	
	cout << "unloading..." << endl;
	
	pClKeySort->unload ();

	keySortPrim_add (pClKeySortPrim, nInitEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	cout << "unloading..." << endl;
	
	pClKeySort->unload ();

	cout << "remove all" << endl;

	pClKeySort->clear ();

	cout << "unloading..." << endl;
	
	pClKeySort->unload ();
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortRemoveInstance (_t_obj *pClKeySort, uint32_t nEntries, uint32_t nInstances)
{
	uint32_t			ui32;
	uint32_t			nDebug = 0;
	uint32_t			nLastKey = 0;
	_t_objprim			*pClKeySortPrim;

	cout << "test removes key sets" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	for (ui32 = 0; ui32 < nInstances; ui32++)
	{
		keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
	}

	keySortPrim_remove (pClKeySortPrim, nEntries, 2, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	keySortPrim_remove (pClKeySortPrim, nEntries, 0, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	keySortPrim_remove (pClKeySortPrim, nEntries, ~0x0, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void BTreeKeySortMulti (_t_obj **ppClKeySort, uint32_t nNumArray, uint32_t nAddEntries, uint32_t nAddMultiple, uint32_t nRemoveEntries, uint32_t nRemoveMultiple)
{
	uint32_t				nDebug = 0;
	uint32_t				ui32, uj32;
	bool					bError = false;
	_t_objprim				**pClKeySortPrim;
	std::stringstream		aszArrayName;

	pClKeySortPrim = new _t_objprim * [nNumArray];

	if (pClKeySortPrim == NULL)
	{
		cerr << "ERROR: insufficient memory!" << endl;

		exit (-1);
	}

	for (ui32 = 0; ui32 < nNumArray; ui32++)
	{
		pClKeySortPrim[ui32] = dynamic_cast <_t_objprim *> (ppClKeySort[ui32]);
	}

	for (ui32 = 0; ui32 < nAddMultiple; ui32++)
	{
		nDebug = 0;

		for (uj32 = 0; uj32 < nNumArray; uj32++)
		{
			keySortPrim_add (pClKeySortPrim[uj32], nAddEntries, nDebug, ui32, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
		}

		for (uj32 = 1; uj32 < nNumArray; uj32++)
		{
			if ((* (ppClKeySort[0])) != (* (ppClKeySort[uj32])))
			{
				cerr << "ERROR: array " << uj32 << " mismatches master array" << endl;

				bError = true;
			}
		}

		if (bError)
		{
			for (uj32 = 0; uj32 < nNumArray; uj32++)
			{
				aszArrayName.clear ();
				aszArrayName << "array" << aszArrayName.width (2) << aszArrayName.fill ('0') << uj32 << ".html" << aszArrayName.width (0) << aszArrayName.fill (' ');

				cerr << "outputting " << aszArrayName.str ().c_str () << endl;

				ppClKeySort[ui32]->show_integrity (aszArrayName.str ().c_str ());
			}

			exit (-1);
		}
	}

	for (ui32 = 0; ui32 < nRemoveMultiple; ui32++)
	{
		for (uj32 = 0; uj32 < nNumArray; uj32++)
		{
			keySortPrim_remove (pClKeySortPrim[uj32], nRemoveEntries, 0, ui32, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
		}

		for (uj32 = 1; uj32 < nNumArray; uj32++)
		{
			if ((* (ppClKeySort[0])) != (* (ppClKeySort[uj32])))
			{
				cerr << "ERROR: array " << uj32 << " mismatches master array" << endl;

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

				ppClKeySort[ui32]->show_integrity (aszArrayName.str ().c_str ());
			}

			exit (-1);
		}
	}

	delete [] pClKeySortPrim;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortMultiNodeSizes (_t_obj *pClMaster, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nFromNodeSize, uint32_t nToNodeSize, uint32_t nAddEntries, uint32_t nAddMultiple, uint32_t nRemoveEntries, uint32_t nRemoveMultiple)
{
	uint32_t		nDebug = 0;
	uint32_t		ui32;
	uint32_t		nNumInst;
	bool			bError = false;
	_t_obj			**ppClKeySort;

	cout << "test exercises several instances using different node size settings" << endl;

	nNumInst = nToNodeSize - nFromNodeSize + 2;

	ppClKeySort = new _t_obj * [nNumInst];

	if (ppClKeySort == NULL)
	{
		cerr << "ERROR: insufficient memory!" << endl;

		exit (-1);
	}

	ppClKeySort[0] = pClMaster;
	
	for (ui32 = nFromNodeSize; ui32 <= nToNodeSize; ui32++)
	{
		ppClKeySort[ui32 - nFromNodeSize + 1] = new _t_obj (rDataLayerProperties, psCacheDescription, ui32, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));

		if (ppClKeySort[ui32 - nFromNodeSize + 1] == NULL)
		{
			cerr << "ERROR: insufficient memory!" << endl;

			exit (-1);
		}
	}

	BTreeKeySortMulti<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (ppClKeySort, nNumInst, nAddEntries, nAddMultiple, nRemoveEntries, nRemoveMultiple);

	for (ui32 = nFromNodeSize; ui32 <= nToNodeSize; ui32++)
	{
		delete ppClKeySort[ui32 - nFromNodeSize + 1];
	}

	delete [] ppClKeySort;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortMultiCacheSize (_t_obj *pClMaster, uint32_t nNumInst, uint32_t nNumMultiCacheSizes, bayerTreeCacheDescription_t *psMultiCacheDesc, _t_datalayerproperties **ppMultiCacheSizeDataLayerProperties, uint32_t nNodeSize, uint32_t nAddEntries, uint32_t nAddMultiple, uint32_t nRemoveEntries, uint32_t nRemoveMultiple)
{
	uint32_t						nDebug = 0;
	uint32_t						ui32;
	bool							bError = false;
	_t_obj							**ppClKeySort;
	
	cout << "test exercises several instances using different cache settings" << endl;

	ppClKeySort = new _t_obj * [nNumInst + 1];

	if (ppClKeySort == NULL)
	{
		cerr << "ERROR: insufficient memory!" << endl;

		exit (-1);
	}

	ppClKeySort[0] = pClMaster;
	
	for (ui32 = 0; ui32 < nNumMultiCacheSizes; ui32++)
	{
		ppClKeySort[ui32 + 1] = new _t_obj (*(ppMultiCacheSizeDataLayerProperties[ui32]), &(psMultiCacheDesc[ui32]), nNodeSize, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));

		if (ppClKeySort[ui32 + 1] == NULL)
		{
			cerr << "ERROR: insufficient memory!" << endl;

			exit (-1);
		}
	}

	BTreeKeySortMulti<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (ppClKeySort, nNumInst + 1, nAddEntries, nAddMultiple, nRemoveEntries, nRemoveMultiple);

	for (ui32 = 0; ui32 < nNumMultiCacheSizes; ui32++)
	{
		delete ppClKeySort[ui32 + 1];
	}

	delete [] ppClKeySort;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortMultiTemplateParams (_t_obj *pClMaster, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nNumMultiTemplateParams, _t_objprim **ppClKeySortPrim, uint32_t nNodeSize, uint32_t nAddEntries, uint32_t nAddMultiple, uint32_t nRemoveEntries, uint32_t nRemoveMultiple)
{
	const uint32_t		nNumInst = nNumMultiTemplateParams + 1;
	uint32_t			nDebug = 0;
	uint32_t			ui32;
	uint32_t			uj32;
	bool				bError = false;
	_t_objprim			**ppClKeySortPrimAll;

	cout << "test exercises several instances using different template parameter settings" << endl;

	ppClKeySortPrimAll = new _t_objprim * [nNumInst];

	if (ppClKeySortPrimAll == NULL)
	{
		cerr << "ERROR: insufficient memory!";

		exit (-1);
	}

	ppClKeySortPrimAll[0] = dynamic_cast <_t_objprim *> (pClMaster);

	for (ui32 = 0; ui32 < nNumMultiTemplateParams; ui32++)
	{
		ppClKeySortPrimAll[ui32 + 1] = ppClKeySortPrim[ui32];
	}

	for (ui32 = 0; ui32 < nAddMultiple; ui32++)
	{
		nDebug = 0;

		for (uj32 = 0; uj32 < nNumInst; uj32++)
		{
			keySortPrim_add (ppClKeySortPrimAll[uj32], nAddEntries, nDebug, ui32, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
		}

		for (uj32 = 1; uj32 < nNumInst; uj32++)
		{
			if (!keySortPrim_compare (ppClKeySortPrimAll[0], ppClKeySortPrimAll[uj32]))
			{
				cerr << "ERROR: array " << uj32 << " mismatches master array!" << endl;

				bError = true;
			}
		}

		if (bError)
		{
			for (ui32 = 0; ui32 < nNumInst; ui32++)
			{
				_t_obj			*pClKeySort;
				std::stringstream	strFileName;

				pClKeySort = dynamic_cast <_t_obj *> (ppClKeySortPrimAll[ui32]);

				strFileName << "keysort" << ui32 << ".html";

				pClKeySort->show_integrity (strFileName.str ().c_str ());
			}


			break;
		}
	}

	if (!bError)
	{
		for (ui32 = 0; ui32 < nRemoveMultiple; ui32++)
		{
			for (uj32 = 0; uj32 < nNumInst; uj32++)
			{
				keySortPrim_remove (ppClKeySortPrimAll[uj32], nRemoveEntries, 0, ui32, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
			}

			for (uj32 = 1; uj32 < nNumInst; uj32++)
			{
				if (!keySortPrim_compare (ppClKeySortPrimAll[0], ppClKeySortPrimAll[uj32]))
				{
					cerr << "ERROR: array " << uj32 << " mismatches master array!" << endl;

					bError = true;
				}
			}

			if (bError)
			{
				for (ui32 = 0; ui32 < nNumInst; ui32++)
				{
					_t_obj			*pClKeySort;
					std::stringstream	strFileName;

					pClKeySort = dynamic_cast <_t_obj *> (ppClKeySortPrimAll[ui32]);

					strFileName << "keysort" << ui32 << ".html";

					pClKeySort->show_integrity (strFileName.str ().c_str ());
				}


				break;
			}
		}
	}

	delete [] ppClKeySortPrimAll;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortCCdeterminePosition (_t_obj *pClKeySort, uint32_t nNodeSize)
{
	uint32_t			nKey = 0xABCDEF98;
	uint32_t			nDebug = 0;
	uint32_t			nEntries;
	uint32_t			nInst;
	_t_objprim			*pClKeySortPrim;

	cout << "test exercises code coverage regarding determinePosition" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	nEntries = nNodeSize * 2 + (nNodeSize * 2) - 1;

	keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nKey, BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY);

	nInst = nNodeSize * 2 - 1;

	keySortPrim_remove (pClKeySortPrim, 1, nInst, nKey, BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY);
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortCCfindFirstKey (_t_obj *pClKeySort, uint32_t nNodeSize)
{
	uint32_t			nKey = 0xABCDEF98;
	uint32_t			nDebug = 0;
	uint32_t			nEntries;
	uint32_t			nInst;
	uint32_t			ui32;
	_t_objprim			*pClKeySortPrim;
	keySortEntry_t		sEntry;
	keySortEntry_t		sKey;

	cout << "test exercises code coverage regarding findFirstKey" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	for (ui32 = 0, nEntries = 1; ui32 < 6; ui32++)
	{
		nEntries *= nNodeSize;
	}

	keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nKey, BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY);

	sKey.nKey = nKey;
	sKey.nDebug = 0;

	while (pClKeySort->size () > 0)
	{
		pClKeySort->get (sKey, 0, &sEntry);

		nInst = generate_rand32 () % pClKeySort->size ();
		nInst /= 2;

		keySortPrim_remove (pClKeySortPrim, 1, nInst, nKey, BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY);
	}
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortCCget (_t_obj *pClKeySort, uint32_t nEntries, uint32_t nInstances)
{
	uint32_t			nDebug = 0;
	uint32_t			nLastKey = 0;
	uint32_t			ui32;
	_t_objprim			*pClKeySortPrim;
	keySortEntry_t		sEntry;
	keySortEntry_t		*pEntries;
	uint64_t			nCount64;
	uint64_t			ui64;

	cout << "test exercises code coverage regarding get" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	for (ui32 = 0; ui32 < nInstances; ui32++)
	{
		keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
	}

	for (ui32 = 0; ui32 < (nEntries * nInstances); )
	{
		pClKeySort->get_at ((uint64_t) ui32, sEntry);

		nCount64 = pClKeySort->count (sEntry);

		pEntries = new keySortEntry_t [(uint32_t) nCount64];

		if (pEntries == NULL)
		{
			cerr << "ERROR: insufficient memory!" << endl;

			exit (-1);
		}

		pClKeySort->get (sEntry, pEntries); 

		for (ui64 = 0; ui64 < nCount64; ui64++)
		{
			pClKeySort->get (sEntry, ui64, &sEntry);

			if ((sEntry.nKey != pEntries[ui64].nKey) ||
				(sEntry.nDebug != pEntries[ui64].nDebug))
			{
				cerr << "ERROR: data mismatch" << endl;
				cerr << "single key: " << std::hex << cerr.width (8) << cerr.fill ('0') << sEntry.nKey << std::dec << cerr.width (0) << cerr.fill (' ') << " debug: " << sEntry.nDebug << endl;
				cerr << " array key: " << std::hex << cerr.width (8) << cerr.fill ('0') << pEntries[ui64].nKey << std::dec << cerr.width (0) << cerr.fill (' ') << " debug: " << pEntries[ui64].nDebug << endl;
				cerr << "instance: " << ui64 << endl;
				cerr << "outputting mismatch.html..." << endl;

				pClKeySort->show_integrity ("mismatch.html");

				exit (-1);
			}

			if (ui32 != (uint32_t) pClKeySort->get_init_pos_of_key (sEntry))
			{
				cerr << "ERROR: get_init_pos_of_key failed" << endl;
				cerr << "counter: " << ui32 << endl;
				cerr << "function: " << pClKeySort->get_init_pos_of_key (sEntry) << endl;
				cerr << "outputting init_pos_of_key.html..." << endl;

				pClKeySort->show_integrity ("init_pos_of_key.html");

				exit (-1);
			}
		}

		delete [] pEntries;

		ui32 += (uint32_t) nCount64;
	}
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortHTMLoutput (_t_obj *pClKeySort, uint32_t nEntries)
{
	uint32_t			nDebug = 0;
	_t_objprim			*pClKeySortPrim;
	uint32_t			nLastKey = 0;

	cout << "tests if instance is left in undefined state after HTML output" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	nLastKey++;

	pClKeySort->show_integrity ("test_keysort_output_0000.html");

	keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	nLastKey++;

	pClKeySort->show_integrity ("test_keysort_output_0001.html");

	keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortCopyConstructorTest (_t_obj *pClRef, _t_obj sClCopy)
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
void TestBTreeKeySortCopyConstructor (_t_obj *pClKeySort, uint32_t nEntries)
{
	uint32_t			nDebug = 0;
	uint32_t			nLastKey = 0;
	_t_objprim			*pClKeySortPrim;

	cout << "exercises array type copy constructor" << endl;

	TestBTreeKeySortCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, *pClKeySort);

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	TestBTreeKeySortCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, *pClKeySort);

	nLastKey++;
	
	keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	TestBTreeKeySortCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, *pClKeySort);
	
	nLastKey++;
	
	keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	TestBTreeKeySortCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, *pClKeySort);

	pClKeySort->unload ();

	TestBTreeKeySortCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, *pClKeySort);

	nLastKey = 0;

	keySortPrim_remove (pClKeySortPrim, nEntries, ~0, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	TestBTreeKeySortCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, *pClKeySort);

	nLastKey++;

	keySortPrim_remove (pClKeySortPrim, nEntries, ~0, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	TestBTreeKeySortCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, *pClKeySort);

	pClKeySort->clear ();

	TestBTreeKeySortCopyConstructorTest<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, *pClKeySort);
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortCompare (_t_obj *pClRef, _t_obj *pClCopy)
{
	if (*pClCopy != *pClRef)
	{
		cerr << endl << "ERROR: copied instance mismatches reference!" << endl;
		cerr << "outputting reference to comp_reference.html" << endl;

		pClRef->show_integrity ("comp_reference.html");

		cerr << "outputting copied instance to comp_copy.html" << endl;

		pClCopy->show_integrity ("comp_copy.html");

		exit (-1);
	}
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortOperatorOverloadAssign (_t_obj *pClKeySort, uint32_t nEntries)
{
	uint32_t			nDebug = 0;
	uint32_t			nLastKey = 0;
	_t_objprim			*pClKeySortPrim;
	_t_obj				*pClTarget;
	_t_objprim			*pClTargetPrim;

	cout << "exercises keysort type operator=" << endl;

	pClTarget = new _t_obj (*pClKeySort);

	if (pClTarget == NULL)
	{
		cerr << "ERROR: Insufficient memory!" << endl;

		exit (-1);
	}

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);
	pClTargetPrim = dynamic_cast <_t_objprim *> (pClTarget);

	*pClTarget = *pClKeySort;

	keySortPrim_compare (pClTargetPrim, pClKeySortPrim);
	TestBTreeKeySortCompare<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClTarget, pClKeySort);
	
	keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	*pClTarget = *pClKeySort;

	keySortPrim_compare (pClTargetPrim, pClKeySortPrim);
	TestBTreeKeySortCompare<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClTarget, pClKeySort);
	
	nLastKey++;
	
	keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	*pClTarget = *pClKeySort;

	keySortPrim_compare (pClTargetPrim, pClKeySortPrim);
	TestBTreeKeySortCompare<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClTarget, pClKeySort);
	
	nLastKey++;
	
	keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	*pClTarget = *pClKeySort;

	keySortPrim_compare (pClTargetPrim, pClKeySortPrim);
	TestBTreeKeySortCompare<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClTarget, pClKeySort);
	
	pClKeySort->unload ();

	*pClTarget = *pClKeySort;

	keySortPrim_compare (pClTargetPrim, pClKeySortPrim);
	TestBTreeKeySortCompare<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClTarget, pClKeySort);
	
	nLastKey = 0;
	
	keySortPrim_remove (pClKeySortPrim, nEntries, ~0, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	*pClTarget = *pClKeySort;

	keySortPrim_compare (pClTargetPrim, pClKeySortPrim);
	TestBTreeKeySortCompare<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClTarget, pClKeySort);
	
	nLastKey += 2;
	
	keySortPrim_remove (pClKeySortPrim, nEntries, ~0, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	*pClTarget = *pClKeySort;

	keySortPrim_compare (pClTargetPrim, pClKeySortPrim);
	TestBTreeKeySortCompare<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClTarget, pClKeySort);
	
	pClKeySort->clear ();

	*pClTarget = *pClKeySort;

	keySortPrim_compare (pClTargetPrim, pClKeySortPrim);
	TestBTreeKeySortCompare<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClTarget, pClKeySort);
	
	delete pClTarget;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortOperatorOverloadAssignMultiInstance (_t_obj *pClKeySort, uint32_t nOuterEntries, uint32_t nInnerEntries, uint32_t nMulti)
{
	uint32_t			nDebug = 0;
	uint32_t			nLastKey = 0;
	uint32_t			ui32;
	uint32_t			nEntries;
	_t_objprim			*pClKeySortPrim;
	_t_obj				*pClTarget;
	_t_objprim			*pClTargetPrim;

	cout << "exercises keysort type operator= while more than one instance per key is in use" << endl;

	pClTarget = new _t_obj (*pClKeySort);

	if (pClTarget == NULL)
	{
		cerr << "ERROR: Insufficient memory!" << endl;

		exit (-1);
	}

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);
	pClTargetPrim = dynamic_cast <_t_objprim *> (pClTarget);

	*pClTarget = *pClKeySort;

	keySortPrim_compare (pClTargetPrim, pClKeySortPrim);
	TestBTreeKeySortCompare<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClTarget, pClKeySort);
	
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

		keySortPrim_add (pClKeySortPrim, nEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY);

		nLastKey++;

		*pClTarget = *pClKeySort;

		keySortPrim_compare (pClTargetPrim, pClKeySortPrim);
		TestBTreeKeySortCompare<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClTarget, pClKeySort);
	}

	pClKeySort->unload ();

	*pClTarget = *pClKeySort;

	keySortPrim_compare (pClTargetPrim, pClKeySortPrim);
	TestBTreeKeySortCompare<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClTarget, pClKeySort);

	nLastKey = 0;
	
	keySortPrim_remove (pClKeySortPrim, nOuterEntries, 0, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY);

	nLastKey++;

	*pClTarget = *pClKeySort;

	keySortPrim_compare (pClTargetPrim, pClKeySortPrim);
	TestBTreeKeySortCompare<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClTarget, pClKeySort);
	
	keySortPrim_remove (pClKeySortPrim, nInnerEntries, 0, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY);

	*pClTarget = *pClKeySort;

	keySortPrim_compare (pClTargetPrim, pClKeySortPrim);
	TestBTreeKeySortCompare<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClTarget, pClKeySort);
	
	pClKeySort->clear ();

	*pClTarget = *pClKeySort;

	keySortPrim_compare (pClTargetPrim, pClKeySortPrim);
	TestBTreeKeySortCompare<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClTarget, pClKeySort);
	
	delete pClTarget;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortGetInitPosOfKeyOnLeafNode (_t_obj *pClKeySort, uint32_t nNodeSize)
{
	uint32_t			nDebug = 0;
	uint32_t			nLastKey = 0;
	uint64_t			nRslt;
	keySortEntry_t		sKey;
	_t_objprim			*pClKeySortPrim;
	
	cout << "exercises get_init_pos_of_key on leaf node" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	keySortPrim_add (pClKeySortPrim, nNodeSize * 2 - 1, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY);

	sKey.nKey = nLastKey;
	sKey.nData = 0;
	sKey.nDebug = 0;

	nRslt = pClKeySort->get_init_pos_of_key (sKey);

	if (nRslt != 0)
	{
		cerr << "ERROR: TestBTreeKeySortGetInitPosOfKeyOnLeafNode: pClKeySort->get_init_pos_of_key did not return with value 0 after t * 2 - 1 entries of the same have been added to key sort list" << endl;

		exit (-1);
	}

	pClKeySort->clear ();

	keySortPrim_add (pClKeySortPrim, nNodeSize - 1, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY);

	nLastKey += 2;

	keySortPrim_add (pClKeySortPrim, nNodeSize - 1, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY);

	sKey.nKey = nLastKey;
	sKey.nData = 0;
	sKey.nDebug = 0;

	nRslt = pClKeySort->get_init_pos_of_key (sKey);

	if (nRslt != (nNodeSize - 1))
	{
		cerr << "ERROR: TestBTreeKeySortGetInitPosOfKeyOnLeafNode: pClKeySort->get_init_pos_of_key did not return with value t - 1 after two entries of different kind have been added to key sort list" << endl;

		exit (-1);
	}

	nLastKey -= 2;

	sKey.nKey = nLastKey;
	sKey.nData = 0;
	sKey.nDebug = 0;

	nRslt = pClKeySort->get_init_pos_of_key (sKey);

	if (nRslt != 0)
	{
		cerr << "ERROR: TestBTreeKeySortGetInitPosOfKeyOnLeafNode: pClKeySort->get_init_pos_of_key did not return with value 0 after two entries of different kind have been added to key sort list" << endl;

		exit (-1);
	}

	nLastKey++;

	keySortPrim_add (pClKeySortPrim, 2, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY);

	sKey.nKey = nLastKey;
	sKey.nData = 0;
	sKey.nDebug = 0;

	nRslt = pClKeySort->get_init_pos_of_key (sKey);

	if (nRslt != (nNodeSize - 1))
	{
		cerr << "ERROR: TestBTreeKeySortGetInitPosOfKeyOnLeafNode: pClKeySort->get_init_pos_of_key did not return with value t - 1 after four entries of different kind have been added to key sort list" << endl;

		exit (-1);
	}

	nLastKey--;

	keySortPrim_add (pClKeySortPrim, nNodeSize * 2 - 1, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY);

	sKey.nKey = nLastKey;
	sKey.nData = 0;
	sKey.nDebug = 0;

	nRslt = pClKeySort->get_init_pos_of_key (sKey);

	if (nRslt != 0)
	{
		cerr << "ERROR: TestBTreeKeySortGetInitPosOfKeyOnLeafNode: pClKeySort->get_init_pos_of_key did not return with value 0 after key 0 was extended" << endl;

		exit (-1);
	}

	nLastKey += 3;

	sKey.nKey = nLastKey;
	sKey.nData = 0;
	sKey.nDebug = 0;

	nRslt = pClKeySort->get_init_pos_of_key (sKey);

	if (nRslt != -1)
	{
		cerr << "ERROR: TestBTreeKeySortGetInitPosOfKeyOnLeafNode: pClKeySort->get_init_pos_of_key did not return with value -1 when programmed with absent key" << endl;

		exit (-1);
	}
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortSerialize (_t_obj *pClKeySort, uint32_t nInstance, uint32_t nSize, uint32_t nWindowSize)
{
	uint32_t			nDebug = 0;
	uint32_t			nLastKey = 0;
	uint32_t			ui32, uj32;
	uint64_t			nRslt;
	keySortEntry_t		*pnData;
	_t_objprim			*pClKeySortPrim;
	keySortEntry_t		sKey;
	
	cout << "exercises serialize method" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	pnData = new keySortEntry_t [nWindowSize];

	if (pnData == NULL)
	{
		cerr << "ERROR: TestBTreeKeySortSerialize: insufficient memory!" << endl;

		exit (-1);
	}
	
	for (ui32 = 0; ui32 < nInstance; ui32++)
	{
		keySortPrim_add (pClKeySortPrim, nSize, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

		nLastKey = 0;
	}

	for (ui32 = 0; ui32 < ((nSize * nInstance) - nWindowSize + 1); ui32++)
	{
		nRslt = pClKeySort->serialize (ui32, nWindowSize, pnData);

		cout << "testing from " << ui32 << " to " << ui32 + nWindowSize << endl;

		if ((uint32_t) nRslt != nWindowSize)
		{
			cerr << "ERROR: TestBTreeKeySortSerialize: size mismatch!" << endl;
			cerr << "iteration: " << ui32 << endl;
			cerr << "window size: " << nWindowSize << endl;
			cerr << "returned size: " << nRslt << endl;

			exit (-1);
		}

		for (uj32 = 0; uj32 < nWindowSize; uj32++)
		{
			pClKeySort->get_at (ui32 + uj32, sKey);

			if (memcmp ((void *) &(pnData[uj32]), (void *) &sKey, sizeof (sKey)) != 0)
			{
				cerr << "ERROR: TestBTreeKeySortSerialize: data mismatch!" << endl;
				cerr << "iteration: " << ui32 << endl;
				cerr << "position: " << uj32 << endl;
				cerr << "key: " << pnData[uj32].nKey << " " << sKey.nKey << endl;
				cerr << "data: " << pnData[uj32].nData << " " << sKey.nData << endl;
				cerr << "debug: " << pnData[uj32].nDebug << " " << sKey.nDebug << endl;

				exit (-1);
			}
		}
	}

	delete [] pnData;
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySortSerializeIncomplete (_t_obj *pClKeySort, uint32_t nNodeSize, uint32_t nSize, uint32_t nWindowSize, uint32_t nOffset)
{
	uint32_t			nDebug = 0;
	uint32_t			nLastKey = 0;
	uint64_t			nRslt;
	keySortEntry_t		*pnData;
	_t_objprim			*pClKeySortPrim;
	
	cout << "exercises serialize method with an incomplete transfer" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	pnData = new keySortEntry_t [nWindowSize];

	if (pnData == NULL)
	{
		cerr << "ERROR: TestBTreeKeySortSerialize: insufficient memory!" << endl;

		exit (-1);
	}
	
	keySortPrim_add (pClKeySortPrim, nSize, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	cout << "testing from " << nOffset << " to " << nWindowSize + nOffset << endl;

	nRslt = pClKeySort->serialize (nOffset, nWindowSize, pnData);

	if ((uint32_t) nRslt != (nSize - nOffset))
	{
		cerr << "ERROR: TestBTreeKeySortSerialize: size mismatch!" << endl;
		cerr << "expected size: " << nSize - nOffset << endl;
		cerr << "returned size: " << nRslt << endl;

		exit (-1);
	}

	delete [] pnData;
}

template <class _t_obj>
void TestBTreeKeySortSTLifInsert (_t_obj *pClKeySort, uint32_t nNumEntries)
{
	typedef typename _t_obj::const_iterator		citer_t;

	citer_t				sCIter;
	citer_t				sCIterRd;
	uint32_t			i;
	keySortEntry_t		sEntry;
	uint32_t			nCnt;

	cout << "exercises method compatible to STL interface CBTreeBase<>::CBTreeKeySort<>:: insert (const _t_data &)" << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		cout << i << " / " << nNumEntries << "\r";

		do
		{
			sEntry.nKey = generate_rand32 ();
		} while (pClKeySort->count (sEntry.nKey) != 0);
		
		sEntry.nDebug = i;
		sEntry.nData = generate_rand32 ();

		sCIter = pClKeySort->insert (sEntry);

		sCIterRd = sCIter;

		for (nCnt = 0; sCIterRd != pClKeySort->cbegin (); nCnt++)
		{
			if (((keySortEntry_t)(*sCIterRd)).nKey > sEntry.nKey)
			{
				break;
			}

			sCIterRd--;
		}

		if (sCIterRd != pClKeySort->cbegin ())
		{
			cerr << endl;
			cerr << "ERROR: TestBTreeKeySortSTLinsert: returned key doesn't fulfill required sort condition! (<=)" << endl;
			cerr << "new entry key: " << sEntry.nKey << endl;
			cerr << " returned key: " << ((keySortEntry_t)(*sCIterRd)).nKey << endl;

			exit (-1);
		}

		sCIterRd = sCIter;

		for (nCnt = 0; sCIterRd != pClKeySort->cend (); nCnt++)
		{
			if (((keySortEntry_t)(*sCIterRd)).nKey < sEntry.nKey)
			{
				break;
			}

			sCIterRd++;
		}

		if (sCIterRd != pClKeySort->cend ())
		{
			cerr << endl;
			cerr << "ERROR: TestBTreeKeySortSTLinsert: returned key doesn't fulfill required sort condition! (>=)" << endl;
			cerr << "new entry key: " << sEntry.nKey << endl;
			cerr << " returned key: " << ((keySortEntry_t)(*sCIterRd)).nKey << endl;

			exit (-1);
		}
	}

	cout << i << " / " << nNumEntries << "\n";
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties>
void TestBTreeKeySortSTLifInsertViaIterator (_t_obj *pClKeySort, uint32_t nNumEntries, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nNodeSize)
{
	typedef typename _t_obj::const_iterator												citer_t;
	typedef typename _t_obj::const_reverse_iterator										criter_t;

	typedef typename ::std::list<keySortEntry_t>::iterator								itlist_t;
	typedef typename ::std::list<keySortEntry_t>::const_iterator						citlist_t;
	typedef typename ::std::list<keySortEntry_t>::reverse_iterator						ritlist_t;
	typedef typename ::std::list<keySortEntry_t>::const_reverse_iterator				critlist_t;

	typedef typename ::std::vector<keySortEntry_t>::iterator							itvec_t;
	typedef typename ::std::vector<keySortEntry_t>::const_iterator						citvec_t;
	typedef typename ::std::vector<keySortEntry_t>::reverse_iterator					ritvec_t;
	typedef typename ::std::vector<keySortEntry_t>::const_reverse_iterator				critvec_t;

	typedef CBTreeKeySort<keySortEntry_t, uint32_t, size_t>								CBTreeKeySort_VectorFeed_t;

	typedef CBTreePairTest<uint32_t, keySortMap_t>										keySortPair_t;

	typedef CBTreeKeySortTest<keySortPair_t, uint32_t, typename _t_obj::sizetype_t, typename _t_obj::nodeiter_t, typename _t_obj::subnodeiter_t, typename _t_obj::datalayerproperties_t, typename _t_obj::datalayer_t>
																						CBTreeKeySortTest_Pair_t;

	typedef typename CBTreeKeySortTest_Pair_t::const_iterator							citer_pair_t;
	typedef typename CBTreeKeySortTest_Pair_t::const_reverse_iterator					criter_pair_t;

	typedef typename ::std::map<uint32_t, keySortMap_t>::iterator						itmap_t;
	typedef typename ::std::map<uint32_t, keySortMap_t>::const_iterator					citmap_t;
	typedef typename ::std::map<uint32_t, keySortMap_t>::reverse_iterator				ritmap_t;
	typedef typename ::std::map<uint32_t, keySortMap_t>::const_reverse_iterator			critmap_t;

	typedef typename ::std::multimap<uint32_t, keySortMap_t>::iterator					itmmap_t;
	typedef typename ::std::multimap<uint32_t, keySortMap_t>::const_iterator			citmmap_t;
	typedef typename ::std::multimap<uint32_t, keySortMap_t>::reverse_iterator			ritmmap_t;
	typedef typename ::std::multimap<uint32_t, keySortMap_t>::const_reverse_iterator	critmmap_t;
	
	_t_obj										sKeySortTarget (*pClKeySort);
	uint32_t									nDebug = 0;
	uint32_t									nLastKey = 0;
	_t_objprim									*pClKeySortPrim;
	citer_t										sCIterA, sCIterB;
	criter_t									sCRIterA, sCRIterB;
	::std::list<keySortEntry_t>					sList;
	::std::vector<keySortEntry_t>				sVector;
	itlist_t									sItListA, sItListB;
	citlist_t									sCItListA, sCItListB;
	ritlist_t									sRItListA, sRItListB;
	critlist_t									sCRItListA, sCRItListB;
	itvec_t										sItVecA, sItVecB;
	citvec_t									sCItVecA, sCItVecB;
	ritvec_t									sRItVecA, sRItVecB;
	critvec_t									sCRItVecA, sCRItVecB;
	CBTreeIOpropertiesRAM						sRAMproperties;
	CBTreeKeySort_VectorFeed_t					sKeySortVectorFeed (sRAMproperties, psCacheDescription, nNodeSize);
	CBTreeKeySortTest_Pair_t					sKeySortPair (rDataLayerProperties, psCacheDescription, nNodeSize);
	CBTreeKeySortTest_Pair_t					sKeySortTargetPair (sKeySortPair);
	keySortPair_t								sKeySortPairData;
	uint32_t									i;
	citer_pair_t								sCIterPairA, sCIterPairB;
	criter_pair_t								sCRIterPairA, sCRIterPairB;
	::std::map<uint32_t, keySortMap_t>			sMap;
	itmap_t										sItMapA, sItMapB;
	citmap_t									sCItMapA, sCItMapB;
	ritmap_t									sRItMapA, sRItMapB;
	critmap_t									sCRItMapA, sCRItMapB;
	::std::multimap<uint32_t, keySortMap_t>		sMMap;
	itmmap_t									sItMMapA, sItMMapB;
	citmmap_t									sCItMMapA, sCItMMapB;
	ritmmap_t									sRItMMapA, sRItMMapB;
	critmmap_t									sCRItMMapA, sCRItMMapB;
	
	cout << "exercises method compatible to STL interface CBTreeBase<>::CBTreeKeySort<>:: insert<_t_iterator> (_t_iterator, _t_iterator)" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	cout << "target::insert<_t_obj::citer_t> (citer_t, citer_t)" << endl;

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sKeySortTarget.template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<_t_obj::criter_t> (criter_t, criter_t)" << endl;

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sKeySortTarget.template insert <criter_t> (pClKeySort->crbegin (), pClKeySort->crend ());

	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<_t_obj::citer_t> (citer_t == citer_t)" << endl;

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sCIterA = pClKeySort->cbegin ();
	sCIterB = pClKeySort->cbegin ();

	while (sCIterA != pClKeySort->cend ())
	{
		sKeySortTarget.template insert <citer_t> (sCIterA, sCIterB);

		sCIterA++;
		sCIterB++;
	}

	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<_t_obj::criter_t> (criter_t == criter_t)" << endl;

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sCRIterA = pClKeySort->crbegin ();
	sCRIterB = pClKeySort->crbegin ();

	while (sCRIterA != pClKeySort->crend ())
	{
		sKeySortTarget.template insert <criter_t> (sCRIterA, sCRIterB);

		sCRIterA++;
		sCRIterB++;
	}

	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<_t_obj::citer_t> (>citer_t, citer_t<)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sCIterA = pClKeySort->cbegin ();
	sCIterB = pClKeySort->cend ();

	sCIterA += (int) (nNumEntries / 4);
	sCIterB -= (int) (nNumEntries / 4);

	sKeySortTarget.template insert <citer_t> (sCIterA, sCIterB);

	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<_t_obj::criter_t> (>criter_t, criter_t<)" << endl;

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sCRIterA = pClKeySort->crbegin ();
	sCRIterB = pClKeySort->crend ();

	sCRIterA += (int) (nNumEntries / 4);
	sCRIterB -= (int) (nNumEntries / 4);

	sKeySortTarget.template insert <criter_t> (sCRIterA, sCRIterB);

	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<_t_obj::citer_t> (target::citer_t, target::citer_t)" << endl;

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	pClKeySort->template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	pClKeySort->clear ();

	cout << "target::insert<_t_obj::criter_t> (target::criter_t, target::criter_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	pClKeySort->template insert <criter_t> (pClKeySort->crbegin (), pClKeySort->crend ());

	pClKeySort->clear ();

	cout << "target::insert<_t_obj::citer_t> (target::citer_t == target::citer_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sCIterA = pClKeySort->cbegin ();
	sCIterB = pClKeySort->cbegin ();

	while (sCIterA != pClKeySort->cend ())
	{
		pClKeySort->template insert <citer_t> (sCIterA, sCIterB);

		sCIterA++;
		sCIterB++;
	}

	pClKeySort->clear ();

	cout << "target::insert<_t_obj::criter_t> (target::criter_t == target::criter_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sCRIterA = pClKeySort->crbegin ();
	sCRIterB = pClKeySort->crbegin ();

	while (sCRIterA != pClKeySort->crend ())
	{
		pClKeySort->template insert <criter_t> (sCRIterA, sCRIterB);

		sCRIterA++;
		sCRIterB++;
	}

	pClKeySort->clear ();

	cout << "target::insert<_t_obj::citer_t> (>target::citer_t, target::citer_t<)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	pClKeySort->template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());
	pClKeySort->template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());
	pClKeySort->template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());
	pClKeySort->template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	sCIterA = pClKeySort->cbegin ();
	sCIterB = pClKeySort->cend ();

	sCIterA += (int) (nNumEntries / 4);
	sCIterB -= (int) (nNumEntries / 4);

	pClKeySort->template insert <citer_t> (sCIterA, sCIterB);

	pClKeySort->clear ();

	cout << "target::insert<_t_obj::criter_t> (>target::criter_t, target::criter_t<)" << endl;

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	pClKeySort->template insert <criter_t> (pClKeySort->crbegin (), pClKeySort->crend ());
	pClKeySort->template insert <criter_t> (pClKeySort->crbegin (), pClKeySort->crend ());
	pClKeySort->template insert <criter_t> (pClKeySort->crbegin (), pClKeySort->crend ());
	pClKeySort->template insert <criter_t> (pClKeySort->crbegin (), pClKeySort->crend ());

	sCRIterA = pClKeySort->crbegin ();
	sCRIterB = pClKeySort->crend ();

	sCRIterA += (int) (nNumEntries / 4);
	sCRIterB -= (int) (nNumEntries / 4);

	pClKeySort->template insert <criter_t> (sCRIterA, sCRIterB);

	pClKeySort->clear ();

	cout << "target::insert<list::iter_t> (iter_t, iter_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClKeySort->cbegin (), pClKeySort->cend ());

	sKeySortTarget.template insert <itlist_t> (sList.begin (), sList.end ());

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<list::citer_t> (citer_t, citer_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClKeySort->cbegin (), pClKeySort->cend ());

	sKeySortTarget.template insert <citlist_t> (sList.cbegin (), sList.cend ());

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<list::riter_t> (riter_t, riter_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClKeySort->cbegin (), pClKeySort->cend ());

	sKeySortTarget.template insert <ritlist_t> (sList.rbegin (), sList.rend ());

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<list::criter_t> (criter_t, criter_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClKeySort->cbegin (), pClKeySort->cend ());

	sKeySortTarget.template insert <critlist_t> (sList.crbegin (), sList.crend ());

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<list::iter_t> (iter_t == iter_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClKeySort->cbegin (), pClKeySort->cend ());

	sItListA = sList.begin ();
	sItListB = sList.begin ();

	while (sItListA != sList.end ())
	{
		sKeySortTarget.template insert <itlist_t> (sItListA, sItListB);

		sItListA++;
		sItListB++;
	}

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<list::citer_t> (citer_t == citer_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClKeySort->cbegin (), pClKeySort->cend ());

	sCItListA = sList.cbegin ();
	sCItListB = sList.cbegin ();

	while (sCItListA != sList.cend ())
	{
		sKeySortTarget.template insert <citlist_t> (sCItListA, sCItListB);

		sCItListA++;
		sCItListB++;
	}

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<list::riter_t> (riter_t == riter_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClKeySort->cbegin (), pClKeySort->cend ());

	sRItListA = sList.rbegin ();
	sRItListB = sList.rbegin ();

	while (sRItListA != sList.rend ())
	{
		sKeySortTarget.template insert <ritlist_t> (sRItListA, sRItListB);

		sRItListA++;
		sRItListB++;
	}

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<list::criter_t> (criter_t == criter_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClKeySort->cbegin (), pClKeySort->cend ());

	sCRItListA = sList.crbegin ();
	sCRItListB = sList.crbegin ();

	while (sCRItListA != sList.crend ())
	{
		sKeySortTarget.template insert <critlist_t> (sCRItListA, sCRItListB);

		sCRItListA++;
		sCRItListB++;
	}

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<list::iter_t> (>iter_t, iter_t<)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClKeySort->cbegin (), pClKeySort->cend ());

	sItListA = sList.begin ();
	sItListB = sList.end ();

	::std::advance<itlist_t, int> (sItListA, nNumEntries / 4);
	::std::advance<itlist_t, int> (sItListB, 0 - (nNumEntries / 4));

	sKeySortTarget.template insert <itlist_t> (sItListA, sItListB);

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<list::citer_t> (>citer_t, citer_t<)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClKeySort->cbegin (), pClKeySort->cend ());

	sCItListA = sList.cbegin ();
	sCItListB = sList.cend ();

	::std::advance<citlist_t, int> (sCItListA, nNumEntries / 4);
	::std::advance<citlist_t, int> (sCItListB, 0 - (nNumEntries / 4));

	sKeySortTarget.template insert <citlist_t> (sCItListA, sCItListB);

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<list::riter_t> (>riter_t, riter_t<)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClKeySort->cbegin (), pClKeySort->cend ());

	sRItListA = sList.rbegin ();
	sRItListB = sList.rend ();

	::std::advance<ritlist_t, int> (sRItListA, nNumEntries / 4);
	::std::advance<ritlist_t, int> (sRItListB, 0 - (nNumEntries / 4));

	sKeySortTarget.template insert <ritlist_t> (sRItListA, sRItListB);

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<list::criter_t> (>criter_t, criter_t<)" << endl;

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sList.template insert <citer_t> (sList.begin (), pClKeySort->cbegin (), pClKeySort->cend ());

	sCRItListA = sList.crbegin ();
	sCRItListB = sList.crend ();

	::std::advance<critlist_t, int> (sCRItListA, nNumEntries / 4);
	::std::advance<critlist_t, int> (sCRItListB, 0 - (nNumEntries / 4));

	sKeySortTarget.template insert <critlist_t> (sCRItListA, sCRItListB);

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

		cout << "target::insert<vector::iter_t> (iter_t, iter_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sKeySortVectorFeed.template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	sVector.template insert <typename CBTreeKeySort_VectorFeed_t::const_iterator> (sVector.begin (), sKeySortVectorFeed.cbegin (), sKeySortVectorFeed.cend ());

	sKeySortTarget.template insert <itvec_t> (sVector.begin (), sVector.end ());

	sVector.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<vector::citer_t> (citer_t, citer_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sKeySortVectorFeed.template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	sVector.template insert <typename CBTreeKeySort_VectorFeed_t::const_iterator> (sVector.begin (), sKeySortVectorFeed.cbegin (), sKeySortVectorFeed.cend ());

	sKeySortTarget.template insert <citvec_t> (sVector.cbegin (), sVector.cend ());

	sVector.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<vector::riter_t> (riter_t, riter_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sKeySortVectorFeed.template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	sVector.template insert <typename CBTreeKeySort_VectorFeed_t::const_iterator> (sVector.begin (), sKeySortVectorFeed.cbegin (), sKeySortVectorFeed.cend ());

	sKeySortTarget.template insert <ritvec_t> (sVector.rbegin (), sVector.rend ());

	sVector.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<vector::criter_t> (criter_t, criter_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sKeySortVectorFeed.template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	sVector.template insert <typename CBTreeKeySort_VectorFeed_t::const_iterator> (sVector.begin (), sKeySortVectorFeed.cbegin (), sKeySortVectorFeed.cend ());

	sKeySortTarget.template insert <critvec_t> (sVector.crbegin (), sVector.crend ());

	sVector.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<vector::iter_t> (iter_t == iter_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sKeySortVectorFeed.template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	sVector.template insert <typename CBTreeKeySort_VectorFeed_t::const_iterator> (sVector.begin (), sKeySortVectorFeed.cbegin (), sKeySortVectorFeed.cend ());

	sItVecA = sVector.begin ();
	sItVecB = sVector.begin ();

	while (sItVecA != sVector.end ())
	{
		sKeySortTarget.template insert <itvec_t> (sItVecA, sItVecB);

		sItVecA++;
		sItVecB++;
	}

	sVector.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<vector::citer_t> (citer_t == citer_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sKeySortVectorFeed.template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	sVector.template insert <typename CBTreeKeySort_VectorFeed_t::const_iterator> (sVector.begin (), sKeySortVectorFeed.cbegin (), sKeySortVectorFeed.cend ());

	sCItVecA = sVector.cbegin ();
	sCItVecB = sVector.cbegin ();

	while (sCItVecA != sVector.cend ())
	{
		sKeySortTarget.template insert <citvec_t> (sCItVecA, sCItVecB);

		sCItVecA++;
		sCItVecB++;
	}

	sVector.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<vector::riter_t> (riter_t == riter_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sKeySortVectorFeed.template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	sVector.template insert <typename CBTreeKeySort_VectorFeed_t::const_iterator> (sVector.begin (), sKeySortVectorFeed.cbegin (), sKeySortVectorFeed.cend ());

	sRItVecA = sVector.rbegin ();
	sRItVecB = sVector.rbegin ();

	while (sRItVecA != sVector.rend ())
	{
		sKeySortTarget.template insert <ritvec_t> (sRItVecA, sRItVecB);

		sRItVecA++;
		sRItVecB++;
	}

	sVector.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<vector::criter_t> (criter_t == criter_t)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sKeySortVectorFeed.template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	sVector.template insert <typename CBTreeKeySort_VectorFeed_t::const_iterator> (sVector.begin (), sKeySortVectorFeed.cbegin (), sKeySortVectorFeed.cend ());

	sCRItVecA = sVector.crbegin ();
	sCRItVecB = sVector.crbegin ();

	while (sCRItVecA != sVector.crend ())
	{
		sKeySortTarget.template insert <critvec_t> (sCRItVecA, sCRItVecB);

		sCRItVecA++;
		sCRItVecB++;
	}

	sVector.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<vector::iter_t> (>iter_t, iter_t<)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sKeySortVectorFeed.template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	sVector.template insert <typename CBTreeKeySort_VectorFeed_t::const_iterator> (sVector.begin (), sKeySortVectorFeed.cbegin (), sKeySortVectorFeed.cend ());

	sItVecA = sVector.begin ();
	sItVecB = sVector.end ();

	::std::advance<itvec_t, int> (sItVecA, nNumEntries / 4);
	::std::advance<itvec_t, int> (sItVecB, 0 - (nNumEntries / 4));

	sKeySortTarget.template insert <itvec_t> (sItVecA, sItVecB);

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<vector::citer_t> (>citer_t, citer_t<)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sKeySortVectorFeed.template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	sVector.template insert <typename CBTreeKeySort_VectorFeed_t::const_iterator> (sVector.begin (), sKeySortVectorFeed.cbegin (), sKeySortVectorFeed.cend ());

	sCItVecA = sVector.cbegin ();
	sCItVecB = sVector.cend ();

	::std::advance<citvec_t, int> (sCItVecA, nNumEntries / 4);
	::std::advance<citvec_t, int> (sCItVecB, 0 - (nNumEntries / 4));

	sKeySortTarget.template insert <citvec_t> (sCItVecA, sCItVecB);

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<vector::riter_t> (>riter_t, riter_t<)" << endl;
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sKeySortVectorFeed.template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	sVector.template insert <typename CBTreeKeySort_VectorFeed_t::const_iterator> (sVector.begin (), sKeySortVectorFeed.cbegin (), sKeySortVectorFeed.cend ());

	sRItVecA = sVector.rbegin ();
	sRItVecB = sVector.rend ();

	::std::advance<ritvec_t, int> (sRItVecA, nNumEntries / 4);
	::std::advance<ritvec_t, int> (sRItVecB, 0 - (nNumEntries / 4));

	sKeySortTarget.template insert <ritvec_t> (sRItVecA, sRItVecB);

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<vector::criter_t> (>criter_t, criter_t<)" << endl;

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sKeySortVectorFeed.template insert <citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	sVector.template insert <typename CBTreeKeySort_VectorFeed_t::const_iterator> (sVector.begin (), sKeySortVectorFeed.cbegin (), sKeySortVectorFeed.cend ());

	sCRItVecA = sVector.crbegin ();
	sCRItVecB = sVector.crend ();

	::std::advance<critvec_t, int> (sCRItVecA, nNumEntries / 4);
	::std::advance<critvec_t, int> (sCRItVecB, 0 - (nNumEntries / 4));

	sKeySortTarget.template insert <critvec_t> (sCRItVecA, sCRItVecB);

	sList.clear ();
	pClKeySort->clear ();
	sKeySortTarget.clear ();

	cout << "target::insert<_t_obj<pair>::citer_t> (citer_t, citer_t)" << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sKeySortPair.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sKeySortTargetPair.template insert<citer_pair_t> (sKeySortPair.cbegin (), sKeySortPair.cend ());

	sKeySortPair.clear ();
	sKeySortTargetPair.clear ();

	cout << "target::insert<_t_obj<pair>::criter_t> (criter_t, criter_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sKeySortPair.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sKeySortTargetPair.template insert<criter_pair_t> (sKeySortPair.crbegin (), sKeySortPair.crend ());

	sKeySortPair.clear ();
	sKeySortTargetPair.clear ();

	cout << "target::insert<_t_obj<pair>::citer_t> (citer_t == citer_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sKeySortPair.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCIterPairA = sKeySortPair.cbegin ();
	sCIterPairB = sKeySortPair.cbegin ();

	while (sCIterPairA != sKeySortPair.cend ())
	{
		sKeySortTargetPair.template insert <citer_pair_t> (sCIterPairA, sCIterPairB);

		sCIterPairA++;
		sCIterPairB++;
	}

	sKeySortPair.clear ();
	sKeySortTargetPair.clear ();

	cout << "target::insert<_t_obj<pair>::criter_t> (criter_t == criter_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sKeySortPair.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCRIterPairA = sKeySortPair.crbegin ();
	sCRIterPairB = sKeySortPair.crbegin ();

	while (sCRIterPairA != sKeySortPair.crend ())
	{
		sKeySortTargetPair.template insert <criter_pair_t> (sCRIterPairA, sCRIterPairB);

		sCRIterPairA++;
		sCRIterPairB++;
	}

	sKeySortPair.clear ();
	sKeySortTargetPair.clear ();

	cout << "target::insert<_t_obj<pair>::citer_t> (>citer_t, citer_t<)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sKeySortPair.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCIterPairA = sKeySortPair.cbegin ();
	sCIterPairB = sKeySortPair.cend ();

	sCIterPairA += (int) (nNumEntries / 4);
	sCIterPairB -= (int) (nNumEntries / 4);

	sKeySortTargetPair.template insert <citer_pair_t> (sCIterPairA, sCIterPairB);

	sKeySortPair.clear ();
	sKeySortTargetPair.clear ();

	cout << "target::insert<_t_obj<pair>::criter_t> (>criter_t, criter_t<)" << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sKeySortPair.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCRIterPairA = sKeySortPair.crbegin ();
	sCRIterPairB = sKeySortPair.crend ();

	sCRIterPairA += (int) (nNumEntries / 4);
	sCRIterPairB -= (int) (nNumEntries / 4);

	sKeySortTargetPair.template insert <criter_pair_t> (sCRIterPairA, sCRIterPairB);

	sKeySortPair.clear ();
	sKeySortTargetPair.clear ();

	cout << "target::insert<_t_obj<pair>::citer_t> (target::citer_t, target::citer_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sKeySortPair.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sKeySortPair.template insert<citer_pair_t> (sKeySortPair.cbegin (), sKeySortPair.cend ());

	sKeySortPair.clear ();

	cout << "target::insert<_t_obj<pair>::criter_t> (target::criter_t, target::criter_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sKeySortPair.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sKeySortPair.template insert<criter_pair_t> (sKeySortPair.crbegin (), sKeySortPair.crend ());

	sKeySortPair.clear ();

	cout << "target::insert<_t_obj<pair>::citer_t> (target::citer_t == target::citer_t)" << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sKeySortPair.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCIterPairA = sKeySortPair.cbegin ();
	sCIterPairB = sKeySortPair.cbegin ();

	while (sCIterPairA != sKeySortPair.cend ())
	{
		sKeySortPair.template insert <citer_pair_t> (sCIterPairA, sCIterPairB);

		sCIterPairA++;
		sCIterPairB++;
	}

	sKeySortPair.clear ();

	cout << "target::insert<_t_obj<pair>::criter_t> (target::criter_t == target::criter_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sKeySortPair.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCRIterPairA = sKeySortPair.crbegin ();
	sCRIterPairB = sKeySortPair.crbegin ();

	while (sCRIterPairA != sKeySortPair.crend ())
	{
		sKeySortPair.template insert <criter_pair_t> (sCRIterPairA, sCRIterPairB);

		sCRIterPairA++;
		sCRIterPairB++;
	}

	sKeySortPair.clear ();

	cout << "target::insert<_t_obj<pair>::citer_t> (>target::citer_t, target::citer_t<)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sKeySortPair.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCIterPairA = sKeySortPair.cbegin ();
	sCIterPairB = sKeySortPair.cend ();

	sCIterPairA += (int) (nNumEntries / 4);
	sCIterPairB -= (int) (nNumEntries / 4);

	sKeySortPair.template insert <citer_pair_t> (sCIterPairA, sCIterPairB);

	sKeySortPair.clear ();

	cout << "target::insert<_t_obj<pair>::criter_t> (>target::criter_t, target::criter_t<)" << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sKeySortPair.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCRIterPairA = sKeySortPair.crbegin ();
	sCRIterPairB = sKeySortPair.crend ();

	sCRIterPairA += (int) (nNumEntries / 4);
	sCRIterPairB -= (int) (nNumEntries / 4);

	sKeySortPair.template insert <criter_pair_t> (sCRIterPairA, sCRIterPairB);

	sKeySortPair.clear ();

	cout << "target::insert<map::iter_t> (iter_t, iter_t)" << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sKeySortPair.template insert<itmap_t> (sMap.begin (), sMap.end ());

	sKeySortPair.clear ();

	cout << "target::insert<map::citer_t> (citer_t, citer_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sKeySortPair.template insert<citmap_t> (sMap.cbegin (), sMap.cend ());

	sKeySortPair.clear ();

	cout << "target::insert<map::riter_t> (riter_t, riter_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sKeySortPair.template insert<ritmap_t> (sMap.rbegin (), sMap.rend ());

	sKeySortPair.clear ();

	cout << "target::insert<map::criter_t> (criter_t, criter_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sKeySortPair.template insert<critmap_t> (sMap.crbegin (), sMap.crend ());

	sKeySortPair.clear ();

	cout << "target::insert<map::iter_t> (iter_t == iter_t)" << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sItMapA = sMap.begin ();
	sItMapB = sMap.begin ();

	while (sItMapA != sMap.end ())
	{
		sKeySortPair.template insert <itmap_t> (sItMapA, sItMapB);

		sItMapA++;
		sItMapB++;
	}

	sKeySortPair.clear ();
	
	cout << "target::insert<map::citer_t> (citer_t == citer_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCItMapA = sMap.cbegin ();
	sCItMapB = sMap.cbegin ();

	while (sCItMapA != sMap.cend ())
	{
		sKeySortPair.template insert <citmap_t> (sCItMapA, sCItMapB);

		sCItMapA++;
		sCItMapB++;
	}

	sKeySortPair.clear ();
	
	cout << "target::insert<map::riter_t> (riter_t == riter_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sRItMapA = sMap.rbegin ();
	sRItMapB = sMap.rbegin ();

	while (sRItMapA != sMap.rend ())
	{
		sKeySortPair.template insert <ritmap_t> (sRItMapA, sRItMapB);

		sRItMapA++;
		sRItMapB++;
	}

	sKeySortPair.clear ();
	
	cout << "target::insert<map::criter_t> (criter_t == criter_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCRItMapA = sMap.crbegin ();
	sCRItMapB = sMap.crbegin ();

	while (sCRItMapA != sMap.crend ())
	{
		sKeySortPair.template insert <critmap_t> (sCRItMapA, sCRItMapB);

		sCRItMapA++;
		sCRItMapB++;
	}

	sKeySortPair.clear ();
	
	cout << "target::insert<map::iter_t> (>iter_t, iter_t<)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sItMapA = sMap.begin ();
	sItMapB = sMap.end ();

	::std::advance<itmap_t, int> (sItMapA, (int) (nNumEntries / 4));
	::std::advance<itmap_t, int> (sItMapB, (int) (0 - (nNumEntries / 4)));

	sKeySortPair.template insert <itmap_t> (sItMapA, sItMapB);

	sKeySortPair.clear ();

	cout << "target::insert<map::citer_t> (>citer_t, citer_t<)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCItMapA = sMap.cbegin ();
	sCItMapB = sMap.cend ();

	::std::advance<citmap_t, int> (sCItMapA, (int) (nNumEntries / 4));
	::std::advance<citmap_t, int> (sCItMapB, (int) (0 - (nNumEntries / 4)));

	sKeySortPair.template insert <citmap_t> (sCItMapA, sCItMapB);

	sKeySortPair.clear ();

	cout << "target::insert<map::riter_t> (>riter_t, riter_t<)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sRItMapA = sMap.rbegin ();
	sRItMapB = sMap.rend ();

	::std::advance<ritmap_t, int> (sRItMapA, (int) (nNumEntries / 4));
	::std::advance<ritmap_t, int> (sRItMapB, (int) (0 - (nNumEntries / 4)));

	sKeySortPair.template insert <ritmap_t> (sRItMapA, sRItMapB);

	sKeySortPair.clear ();

	cout << "target::insert<map::criter_t> (>criter_t, criter_t<)" << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCRItMapA = sMap.crbegin ();
	sCRItMapB = sMap.crend ();

	::std::advance<critmap_t, int> (sCRItMapA, (int) (nNumEntries / 4));
	::std::advance<critmap_t, int> (sCRItMapB, (int) (0 - (nNumEntries / 4)));

	sKeySortPair.template insert <critmap_t> (sCRItMapA, sCRItMapB);

	sKeySortPair.clear ();

		cout << "target::insert<multimap::iter_t> (iter_t, iter_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sKeySortPair.template insert<itmmap_t> (sMMap.begin (), sMMap.end ());

	sKeySortPair.clear ();

	cout << "target::insert<multimap::citer_t> (citer_t, citer_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sKeySortPair.template insert<citmmap_t> (sMMap.cbegin (), sMMap.cend ());

	sKeySortPair.clear ();

	cout << "target::insert<multimap::riter_t> (riter_t, riter_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sKeySortPair.template insert<ritmmap_t> (sMMap.rbegin (), sMMap.rend ());

	sKeySortPair.clear ();

	cout << "target::insert<multimap::criter_t> (criter_t, criter_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sKeySortPair.template insert<critmmap_t> (sMMap.crbegin (), sMMap.crend ());

	sKeySortPair.clear ();

	cout << "target::insert<multimap::iter_t> (iter_t == iter_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sItMMapA = sMMap.begin ();
	sItMMapB = sMMap.begin ();

	while (sItMMapA != sMMap.end ())
	{
		sKeySortPair.template insert <itmmap_t> (sItMMapA, sItMMapB);

		sItMMapA++;
		sItMMapB++;
	}

	sKeySortPair.clear ();
	
	cout << "target::insert<multimap::citer_t> (citer_t == citer_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCItMMapA = sMMap.cbegin ();
	sCItMMapB = sMMap.cbegin ();

	while (sCItMMapA != sMMap.cend ())
	{
		sKeySortPair.template insert <citmmap_t> (sCItMMapA, sCItMMapB);

		sCItMMapA++;
		sCItMMapB++;
	}

	sKeySortPair.clear ();
	
	cout << "target::insert<multimap::riter_t> (riter_t == riter_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sRItMMapA = sMMap.rbegin ();
	sRItMMapB = sMMap.rbegin ();

	while (sRItMMapA != sMMap.rend ())
	{
		sKeySortPair.template insert <ritmmap_t> (sRItMMapA, sRItMMapB);

		sRItMMapA++;
		sRItMMapB++;
	}

	sKeySortPair.clear ();
	
	cout << "target::insert<multimap::criter_t> (criter_t == criter_t)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCRItMMapA = sMMap.crbegin ();
	sCRItMMapB = sMMap.crbegin ();

	while (sCRItMMapA != sMMap.crend ())
	{
		sKeySortPair.template insert <critmmap_t> (sCRItMMapA, sCRItMMapB);

		sCRItMMapA++;
		sCRItMMapB++;
	}

	sKeySortPair.clear ();
	
	cout << "target::insert<multimap::iter_t> (>iter_t, iter_t<)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sItMMapA = sMMap.begin ();
	sItMMapB = sMMap.end ();

	::std::advance<itmmap_t, int> (sItMMapA, (int) (nNumEntries / 4));
	::std::advance<itmmap_t, int> (sItMMapB, (int) (0 - (nNumEntries / 4)));

	sKeySortPair.template insert <itmmap_t> (sItMMapA, sItMMapB);

	sKeySortPair.clear ();

	cout << "target::insert<multimap::citer_t> (>citer_t, citer_t<)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCItMMapA = sMMap.cbegin ();
	sCItMMapB = sMMap.cend ();

	::std::advance<citmmap_t, int> (sCItMMapA, (int) (nNumEntries / 4));
	::std::advance<citmmap_t, int> (sCItMMapB, (int) (0 - (nNumEntries / 4)));

	sKeySortPair.template insert <citmmap_t> (sCItMMapA, sCItMMapB);

	sKeySortPair.clear ();

	cout << "target::insert<multimap::riter_t> (>riter_t, riter_t<)" << endl;
	
	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sRItMMapA = sMMap.rbegin ();
	sRItMMapB = sMMap.rend ();

	::std::advance<ritmmap_t, int> (sRItMMapA, (int) (nNumEntries / 4));
	::std::advance<ritmmap_t, int> (sRItMMapB, (int) (0 - (nNumEntries / 4)));

	sKeySortPair.template insert <ritmmap_t> (sRItMMapA, sRItMMapB);

	sKeySortPair.clear ();

	cout << "target::insert<multimap::criter_t> (>criter_t, criter_t<)" << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sMMap.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sCRItMMapA = sMMap.crbegin ();
	sCRItMMapB = sMMap.crend ();

	::std::advance<critmmap_t, int> (sCRItMMapA, (int) (nNumEntries / 4));
	::std::advance<critmmap_t, int> (sCRItMMapB, (int) (0 - (nNumEntries / 4)));

	sKeySortPair.template insert <critmmap_t> (sCRItMMapA, sCRItMMapB);

	sKeySortPair.clear ();
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties>
void TestBTreeKeySortSTLifSwap (_t_obj *pClKeySort, uint32_t nNumEntries, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nNodeSize)
{
	typedef typename _t_obj::const_iterator			citer_t;

	typedef CBTreePairTest<uint32_t, keySortMap_t>										keySortPair_t;

	typedef CBTreeKeySortTest<keySortPair_t, uint32_t, typename _t_obj::sizetype_t, typename _t_obj::nodeiter_t, typename _t_obj::subnodeiter_t, typename _t_obj::datalayerproperties_t, typename _t_obj::datalayer_t>
																						CBTreeKeySortTest_Pair_t;

	typedef typename CBTreeKeySortTest_Pair_t::const_iterator							citer_pair_t;
	
	_t_objprim						*pClKeySortPrim;
	_t_objprim						*pClKeySortTargetPrim;
	uint32_t						nDebug = 0;
	uint32_t						nLastKey = 0;
	_t_obj							sClKeySortTarget (*pClKeySort);
	_t_obj							sClKeySortRef (*pClKeySort);
	_t_obj							sClKeySortTargetRef (*pClKeySort);
	CBTreeKeySortTest_Pair_t		sClKeySortPair (rDataLayerProperties, psCacheDescription, nNodeSize);
	CBTreeKeySortTest_Pair_t		sClKeySortPairTarget (sClKeySortPair);
	CBTreeKeySortTest_Pair_t		sClKeySortPairRef (sClKeySortPair);
	CBTreeKeySortTest_Pair_t		sClKeySortPairTargetRef (sClKeySortPair);
	keySortPair_t					sEntryPair;
	uint32_t						i;

	cout << "exercises method compatible to STL interface CBTreeBase<>::CBTreeKeysort<>:: swap ()" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);
	pClKeySortTargetPrim = dynamic_cast <_t_objprim *> (&sClKeySortTarget);

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sClKeySortRef.template insert<citer_t> (pClKeySort->cbegin (), pClKeySort->cend ());

	if ((*pClKeySort) != sClKeySortRef)
	{
		cerr << "ERROR: Unexpected keysort mismatch!" << endl;

		exit (-1);
	}

	keySortPrim_add (pClKeySortTargetPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	sClKeySortTargetRef.template insert<citer_t> (sClKeySortTarget.cbegin (), sClKeySortTarget.cend ());

	if (sClKeySortTarget != sClKeySortTargetRef)
	{
		cerr << "ERROR: Unexpected target mismatch!" << endl;

		exit (-1);
	}

	pClKeySort->swap (sClKeySortTarget);

	if ((*pClKeySort) != sClKeySortTargetRef)
	{
		cerr << "ERROR: array mismatches target reference!" << endl;

		exit (-1);
	}

	if (sClKeySortTarget != sClKeySortRef)
	{
		cerr << "ERROR: target mismatches array reference!" << endl;

		exit (-1);
	}

	sClKeySortTarget.swap (*pClKeySort);

	if ((*pClKeySort) != sClKeySortRef)
	{
		cerr << "ERROR: array mismatches array reference!" << endl;

		exit (-1);
	}

	if (sClKeySortTarget != sClKeySortTargetRef)
	{
		cerr << "ERROR: target mismatches target reference!" << endl;

		exit (-1);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sEntryPair.first = generate_rand32 ();
		sEntryPair.second.nData = generate_rand32 ();
		sEntryPair.second.nDebug = nDebug;

		nDebug++;

		sClKeySortPair.insert (sEntryPair);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sClKeySortPairRef.template insert<citer_pair_t> (sClKeySortPair.cbegin (), sClKeySortPair.cend ());

	if (sClKeySortPair != sClKeySortPairRef)
	{
		cerr << "ERROR: Unexpected keysort mismatch! (pair)" << endl;

		exit (-1);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sEntryPair.first = generate_rand32 ();
		sEntryPair.second.nData = generate_rand32 ();
		sEntryPair.second.nDebug = nDebug;

		nDebug++;

		sClKeySortPairTarget.insert (sEntryPair);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	sClKeySortPairTargetRef.template insert<citer_pair_t> (sClKeySortPairTarget.cbegin (), sClKeySortPairTarget.cend ());

	if (sClKeySortPairTarget != sClKeySortPairTargetRef)
	{
		cerr << "ERROR: Unexpected target mismatch! (pair)" << endl;

		exit (-1);
	}

	sClKeySortPair.swap (sClKeySortPairTarget);

	if (sClKeySortPair != sClKeySortPairTargetRef)
	{
		cerr << "ERROR: array mismatches target reference! (pair)" << endl;

		exit (-1);
	}

	if (sClKeySortPairTarget != sClKeySortPairRef)
	{
		cerr << "ERROR: target mismatches array reference! (pair)" << endl;

		exit (-1);
	}

	sClKeySortPairTarget.swap (sClKeySortPair);

	if (sClKeySortPair != sClKeySortPairRef)
	{
		cerr << "ERROR: array mismatches array reference! (pair)" << endl;

		exit (-1);
	}

	if (sClKeySortPairTarget != sClKeySortPairTargetRef)
	{
		cerr << "ERROR: target mismatches target reference! (pair)" << endl;

		exit (-1);
	}
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties>
void TestBTreeKeySortSTLifFind (_t_obj *pClKeySort, uint32_t nNumEntries, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nNodeSize)
{
	typedef typename _t_obj::const_iterator			citer_t;

	typedef CBTreePairTest<uint32_t, keySortMap_t>										keySortPair_t;

	typedef CBTreeKeySortTest<keySortPair_t, uint32_t, typename _t_obj::sizetype_t, typename _t_obj::nodeiter_t, typename _t_obj::subnodeiter_t, typename _t_obj::datalayerproperties_t, typename _t_obj::datalayer_t>
																						CBTreeKeySortTest_Pair_t;

	typedef typename CBTreeKeySortTest_Pair_t::const_iterator							citer_pair_t;
	
	_t_objprim						*pClKeySortPrim;
	uint32_t						nDebug = 0;
	uint32_t						nLastKey = 1;
	CBTreeKeySortTest_Pair_t		sClKeySortPair (rDataLayerProperties, psCacheDescription, nNodeSize);
	keySortPair_t					sEntryPair;
	uint32_t						i;
	citer_t							sCIter;
	citer_t							sCIterRslt;
	citer_pair_t					sCItPair;
	citer_pair_t					sCItPairRslt;

	cout << "exercises method compatible to STL interface CBTreeBase<>::CBTreeKeysort<>:: find ()" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);
	
	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_ASCEND_KEY);

	for (sCIter = pClKeySort->cbegin (); sCIter != pClKeySort->cend (); sCIter++)
	{
		sCIterRslt = pClKeySort->find ((*sCIter).nKey);

		if (sCIter != sCIterRslt)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifFind: ERROR: iterator mismatch!" << endl;

			exit (-1);
		}
	}

	sCIterRslt = pClKeySort->find (0);

	if (sCIterRslt != pClKeySort->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on lower bound check!" << endl;

		exit (-1);
	}

	sCIterRslt = pClKeySort->find (nNumEntries + 1);

	if (sCIterRslt != pClKeySort->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on upper bound check!" << endl;

		exit (-1);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sEntryPair.first = i + 1;
		sEntryPair.second.nData = generate_rand32 ();
		sEntryPair.second.nDebug = nDebug;

		nDebug++;

		sClKeySortPair.insert (sEntryPair);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	for (sCItPair = sClKeySortPair.cbegin (); sCItPair != sClKeySortPair.cend (); sCItPair++)
	{
		sCItPairRslt = sClKeySortPair.find ((*sCItPair).first);

		if (sCItPair != sCItPairRslt)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifFind: ERROR: iterator mismatch! (pair)" << endl;

			exit (-1);
		}
	}

	sCItPairRslt = sClKeySortPair.find (0);

	if (sCItPairRslt != sClKeySortPair.cend ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on lower bound check! (pair)" << endl;

		exit (-1);
	}

	sCItPairRslt = sClKeySortPair.find (nNumEntries + 1);

	if (sCItPairRslt != sClKeySortPair.cend ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifFind: ERROR: find () didn't return cend () on upper bound check! (pair)" << endl;

		exit (-1);
	}
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties>
void TestBTreeKeySortSTLifLowerBoundUpperBound (_t_obj *pClKeySort, uint32_t nNumEntries, uint32_t nInstance, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nNodeSize)
{
	typedef typename _t_obj::const_iterator			citer_t;

	typedef CBTreePairTest<uint32_t, keySortMap_t>										keySortPair_t;

	typedef CBTreeKeySortTest<keySortPair_t, uint32_t, typename _t_obj::sizetype_t, typename _t_obj::nodeiter_t, typename _t_obj::subnodeiter_t, typename _t_obj::datalayerproperties_t, typename _t_obj::datalayer_t>
																						CBTreeKeySortTest_Pair_t;

	typedef typename CBTreeKeySortTest_Pair_t::const_iterator							citer_pair_t;
	
	_t_objprim						*pClKeySortPrim;
	uint32_t						nDebug = 0;
	uint32_t						nLastKey;
	CBTreeKeySortTest_Pair_t		sClKeySortPair (rDataLayerProperties, psCacheDescription, nNodeSize);
	keySortPair_t					sEntryPair;
	uint32_t						i;
	uint32_t						j;
	citer_t							sCIterLower;
	citer_t							sCIterUpper;
	citer_pair_t					sCItPairLower;
	citer_pair_t					sCItPairUpper;

	cout << "exercises methods compatible to STL interface CBTreeBase<>::CBTreeKeysort<>:: lower_bound () upper_bound () count ()" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);
	
	for (i = 0; i < nInstance; i++)
	{
		nLastKey = 1;

		keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_ASCEND_KEY);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sCIterLower = pClKeySort->lower_bound (i + 1);
		sCIterUpper = pClKeySort->upper_bound (i + 1);

		sCIterLower += (int) nInstance;

		if (sCIterLower != sCIterUpper)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: boundary mismatch!" << endl;

			exit (-1);
		}

		if (pClKeySort->count (i + 1) != nInstance)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: count () returned with unexpected value!" << endl;

			exit (-1);
		}
	}

	sCIterLower = pClKeySort->lower_bound (0);

	if (sCIterLower != pClKeySort->cbegin ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: lower_bound () didn't return cbegin () when testing for lowest key!" << endl;

		exit (-1);
	}

	sCIterUpper = pClKeySort->upper_bound (nNumEntries + 1);

	if (sCIterUpper != pClKeySort->cend ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: upper_bound () didn't return cend () when testing for lowest key!" << endl;

		exit (-1);
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sEntryPair.first = i + 1;

		for (j = 0; j < nInstance; j++)
		{
			sEntryPair.second.nData = generate_rand32 ();
			sEntryPair.second.nDebug = nDebug;

			nDebug++;

			sClKeySortPair.insert (sEntryPair);
		}

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	for (i = 0; i < nNumEntries; i++)
	{
		sCItPairLower = sClKeySortPair.lower_bound (i + 1);
		sCItPairUpper = sClKeySortPair.upper_bound (i + 1);

		sCItPairLower += (int) nInstance;

		if (sCItPairLower != sCItPairUpper)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: boundary mismatch! (pair)" << endl;

			exit (-1);
		}

		if (sClKeySortPair.count (i + 1) != nInstance)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: count () returned with unexpected value! (pair)" << endl;

			exit (-1);
		}
	}

	sCItPairLower = sClKeySortPair.lower_bound (0);

	if (sCItPairLower != sClKeySortPair.cbegin ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: lower_bound () didn't return cbegin () when testing for lowest key! (pair)" << endl;

		exit (-1);
	}

	sCItPairUpper = sClKeySortPair.upper_bound (nNumEntries + 1);

	if (sCItPairUpper != sClKeySortPair.cend ())
	{
		cerr << endl;
		cerr << "TestBTreeKeySortSTLifLowerBoundUpperBound: ERROR: upper_bound () didn't return cend () when testing for lowest key! (pair)" << endl;

		exit (-1);
	}
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties>
void TestBTreeKeySortSTLifEraseViaIterator (_t_obj *pClKeySort, uint32_t nNumEntries, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nNodeSize)
{
	typedef typename _t_obj::const_iterator												citer_t;

	typedef CBTreePairTest<uint32_t, keySortMap_t>										keySortPair_t;

	typedef CBTreeKeySortTest<keySortPair_t, uint32_t, typename _t_obj::sizetype_t, typename _t_obj::nodeiter_t, typename _t_obj::subnodeiter_t, typename _t_obj::datalayerproperties_t, typename _t_obj::datalayer_t>
																						CBTreeKeySortTest_Pair_t;

	typedef typename CBTreeKeySortTest_Pair_t::const_iterator							citer_pair_t;
	
	uint32_t									nDebug = 0;
	uint32_t									nLastKey = 0;
	_t_objprim									*pClKeySortPrim;
	citer_t										sIter;
	citer_t										sIterRslt;
	CBTreeKeySortTest_Pair_t					sKeySortPair (rDataLayerProperties, psCacheDescription, nNodeSize);
	uint32_t									i;
	citer_pair_t								sIterPair;
	citer_pair_t								sIterPairRslt;
	keySortPair_t								sKeySortPairData;

	cout << "exercises method compatible to STL interface CBTreeBase<>::CBTreeKeySort<>:: erase (const_iterator)" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

	while (pClKeySort->size () > 0)
	{
		sIter = pClKeySort->cbegin ();

		sIter += (int) (generate_rand32 () % pClKeySort->size ());

		sIterRslt = pClKeySort->erase (sIter);

		if (sIter != sIterRslt)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifEraseViaIterator: returned iterator mismatches!" << endl;

			exit (-1);
		}
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();
		sKeySortPairData.second.nData = generate_rand32 ();
		sKeySortPairData.second.nDebug = nDebug;

		nDebug++;

		sKeySortPair.insert (sKeySortPairData);

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	while (sKeySortPair.size () > 0)
	{
		sIterPair = sKeySortPair.cbegin ();

		sIterPair += (int) (generate_rand32 () % sKeySortPair.size ());

		sIterPairRslt = sKeySortPair.erase (sIterPair);

		if (sIterPair != sIterPairRslt)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifEraseViaIterator: returned iterator (pair type) mismatches!" << endl;

			exit (-1);
		}
	}
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties>
void TestBTreeKeySortSTLifEraseViaKey (_t_obj *pClKeySort, uint32_t nNumEntries, uint32_t nInstances, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nNodeSize)
{
	typedef typename _t_obj::const_iterator												citer_t;

	typedef CBTreePairTest<uint32_t, keySortMap_t>										keySortPair_t;

	typedef CBTreeKeySortTest<keySortPair_t, uint32_t, typename _t_obj::sizetype_t, typename _t_obj::nodeiter_t, typename _t_obj::subnodeiter_t, typename _t_obj::datalayerproperties_t, typename _t_obj::datalayer_t>
																						CBTreeKeySortTest_Pair_t;

	typedef typename CBTreeKeySortTest_Pair_t::const_iterator							citer_pair_t;
	
	uint32_t									nDebug = 0;
	uint32_t									nLastKey;
	_t_objprim									*pClKeySortPrim;
	citer_t										sIter;
	uint32_t									i;
	typename _t_obj::sizetype_t					nRslt;
	uint32_t									j;
	CBTreeKeySortTest_Pair_t					sKeySortPair (rDataLayerProperties, psCacheDescription, nNodeSize);
	citer_pair_t								sIterPair;
	keySortPair_t								sKeySortPairData;

	cout << "exercises method compatible to STL interface CBTreeBase<>::CBTreeKeySort<>:: erase (const _t_key)" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
	}

	while (pClKeySort->size () > 0)
	{
		sIter = pClKeySort->cbegin ();

		sIter += (int) (generate_rand32 () % pClKeySort->size ());

		nRslt = pClKeySort->erase ((*sIter).nKey);

		if (nRslt != nInstances)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifEraseViaKey: unexpected number of items deleted!" << endl;

			exit (-1);
		}
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();

		for (j = 0; j < nInstances; j++)
		{
			sKeySortPairData.second.nData = generate_rand32 ();
			sKeySortPairData.second.nDebug = nDebug;

			nDebug++;

			sKeySortPair.insert (sKeySortPairData);
		}

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	while (sKeySortPair.size () > 0)
	{
		sIterPair = sKeySortPair.cbegin ();

		sIterPair += (int) (generate_rand32 () % sKeySortPair.size ());

		nRslt = sKeySortPair.erase ((*sIterPair).first);

		if (nRslt != nInstances)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifEraseViaKey: unexpected number of items deleted! (pair)" << endl;

			exit (-1);
		}
	}
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties>
void TestBTreeKeySortSTLifEraseViaIteratorMultiple (_t_obj *pClKeySort, uint32_t nNumEntries, uint32_t nInstances, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nNodeSize)
{
	typedef typename _t_obj::const_iterator												citer_t;

	typedef CBTreePairTest<uint32_t, keySortMap_t>										keySortPair_t;

	typedef CBTreeKeySortTest<keySortPair_t, uint32_t, typename _t_obj::sizetype_t, typename _t_obj::nodeiter_t, typename _t_obj::subnodeiter_t, typename _t_obj::datalayerproperties_t, typename _t_obj::datalayer_t>
																						CBTreeKeySortTest_Pair_t;

	typedef typename CBTreeKeySortTest_Pair_t::const_iterator							citer_pair_t;
	
	uint32_t									nDebug = 0;
	uint32_t									nLastKey;
	_t_objprim									*pClKeySortPrim;
	citer_t										sIterBegin;
	citer_t										sIterEnd;
	citer_t										sIterRslt;
	uint32_t									i;
	uint32_t									j;
	CBTreeKeySortTest_Pair_t					sKeySortPair (rDataLayerProperties, psCacheDescription, nNodeSize);
	citer_pair_t								sIterPairBegin;
	citer_pair_t								sIterPairEnd;
	citer_pair_t								sIterPairRslt;
	keySortPair_t								sKeySortPairData;

	cout << "exercises method compatible to STL interface CBTreeBase<>::CBTreeKeySort<>:: erase (const_iterator, const_iterator)" << endl;

	pClKeySortPrim = dynamic_cast <_t_objprim *> (pClKeySort);

	for (i = 0; i < nInstances; i++)
	{
		nLastKey = 0;

		keySortPrim_add (pClKeySortPrim, nNumEntries, nDebug, nLastKey, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);
	}

	while (pClKeySort->size () > 0)
	{
		sIterBegin = pClKeySort->cbegin ();
		sIterEnd = pClKeySort->cend ();

		sIterBegin += (int) (generate_rand32 () % (pClKeySort->size () / 2));
		sIterEnd -= (int) (generate_rand32 () % (pClKeySort->size () / 2));

		sIterRslt = pClKeySort->erase (sIterBegin, sIterEnd);

		if (sIterRslt != sIterBegin)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifEraseViaIteratorMultiple: returned iterator mismatches!" << endl;

			exit (-1);
		}
	}

	for (i = 0; i < nNumEntries; i++)
	{
		sKeySortPairData.first = generate_rand32 ();

		for (j = 0; j < nInstances; j++)
		{
			sKeySortPairData.second.nData = generate_rand32 ();
			sKeySortPairData.second.nDebug = nDebug;

			nDebug++;

			sKeySortPair.insert (sKeySortPairData);
		}

		cout << i << " / " << nNumEntries << "\r";
	}

	cout << i << " / " << nNumEntries << endl;

	while (sKeySortPair.size () > 0)
	{
		sIterPairBegin = sKeySortPair.cbegin ();
		sIterPairEnd = sKeySortPair.cend ();

		sIterPairBegin += (int) (generate_rand32 () % (sKeySortPair.size () / 2));
		sIterPairEnd -= (int) (generate_rand32 () % (sKeySortPair.size () / 2));

		sIterPairRslt = sKeySortPair.erase (sIterPairBegin, sIterPairEnd);

		if (sIterPairRslt != sIterPairBegin)
		{
			cerr << endl;
			cerr << "TestBTreeKeySortSTLifEraseViaKey: returned iterator (pair type) mismatches!" << endl;

			exit (-1);
		}
	}
}

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySort (uint32_t nTest, uint32_t nNodeSize, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t &sCacheDesc, uint32_t nNumMultiCacheSizes, bayerTreeCacheDescription_t *psMultiCacheDesc, uint32_t nNumCacheProperties, _t_datalayerproperties **ppMultiCacheSizeDataLayerProperties, uint32_t nNumMultiTemplateParams, _t_objprim **ppClKeySortsPrim, int argc, char **argv)
{
	_t_obj									*pClKeySort;

	cout << "b-tree keysort test bench selected" << endl;
	
	pClKeySort = new _t_obj (rDataLayerProperties, &sCacheDesc, nNodeSize, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));

	if (pClKeySort == NULL)
	{
		cerr << "ERROR: Insufficient memory!" << endl;

		return;
	}

	switch (nTest)
	{
	case BTREETEST_KEYSORT_BASIC_ASCEND			:
		{
			TestBTreeKeySortBasic<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 512, 256, 3, BTREETEST_KEYSORT_PRIMITIVE_ASCEND_KEY, BTREETEST_KEYSORT_PRIMITIVE_ASCEND_KEY);

			break;
		}

	case BTREETEST_KEYSORT_BASIC_ASCEND_SMALL	:
		{
			TestBTreeKeySortBasic<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 16, 0, 0, BTREETEST_KEYSORT_PRIMITIVE_ASCEND_KEY, BTREETEST_KEYSORT_PRIMITIVE_ASCEND_KEY);

			break;
		}

	case BTREETEST_KEYSORT_BASIC_DESCEND			:
		{
			TestBTreeKeySortBasic<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 512, 256, 3, BTREETEST_KEYSORT_PRIMITIVE_DESCEND_KEY, BTREETEST_KEYSORT_PRIMITIVE_DESCEND_KEY);

			break;
		}

	case BTREETEST_KEYSORT_BASIC_DESCEND_SMALL	:
		{
			TestBTreeKeySortBasic<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 16, 0, 0, BTREETEST_KEYSORT_PRIMITIVE_DESCEND_KEY, BTREETEST_KEYSORT_PRIMITIVE_DESCEND_KEY);

			break;
		}

	case BTREETEST_KEYSORT_BASIC_RANDOM		:
		{
			TestBTreeKeySortBasic<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 512, 256, 3, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

			break;
		}

	case BTREETEST_KEYSORT_BASIC_RANDOM_SMALL	:
		{
			TestBTreeKeySortBasic<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 16, 0, 0, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY, BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY);

			break;
		}

	case BTREETEST_KEYSORT_clear			:
		{
			TestBTreeKeySortRemoveAll<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 256);

			break;
		}

	case BTREETEST_KEYSORT_REPLACE			:
		{
			TestBTreeKeySortMultiRemove<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 64);

			break;
		}

	case BTREETEST_KEYSORT_UNLOAD				:
		{
			TestBTreeKeySortUnload<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 128, 128, 128, 128);

			break;
		}

	case BTREETEST_KEYSORT_REMOVE_INSTANCES		:
		{
			TestBTreeKeySortRemoveInstance<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 64, 3);

			break;
		}

	case BTREETEST_KEYSORT_MULTI_NODESIZES				:
		{
			TestBTreeKeySortMultiNodeSizes<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, rDataLayerProperties, &sCacheDesc, 3, 16, 64, 4, 64, 4);

			break;
		}

	case BTREETEST_KEYSORT_MULTI_CACHE_SETTINGS			:
		{
			TestBTreeKeySortMultiCacheSize<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, nNumCacheProperties, nNumMultiCacheSizes, psMultiCacheDesc, ppMultiCacheSizeDataLayerProperties, nNodeSize, 64, 4, 64, 4);

			break;
		}

	case BTREETEST_KEYSORT_MULTI_TEMPLATE_PARAMETERS	:
		{
			TestBTreeKeySortMultiTemplateParams<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, &sCacheDesc, nNumMultiTemplateParams, ppClKeySortsPrim, nNodeSize, 64, 4, 64, 4);

			break;
		}

	case BTREETEST_KEYSORT_CODE_COVERAGE_DETERMINE_POSITION	:
		{
			TestBTreeKeySortCCdeterminePosition<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, nNodeSize);

			break;
		}

	case BTREETEST_KEYSORT_CODE_COVERAGE_FIND_FIRST_KEY		:
		{
			TestBTreeKeySortCCfindFirstKey<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, nNodeSize);

			break;
		}

	case BTREETEST_KEYSORT_CODE_COVERAGE_GET				:
		{
			TestBTreeKeySortCCget<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 64, 3);

			break;
		}

	case BTREETEST_KEYSORT_HTML_OUTPUT						:
		{
			TestBTreeKeySortHTMLoutput<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 64);

			break;
		}

	case BTREETEST_KEYSORT_COPY_CONSTRUCTOR					:
		{
			TestBTreeKeySortCopyConstructor<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 64);

			break;
		}

	case BTREETEST_KEYSORT_OPERATOR_OVERLOAD_ASSIGN			:
		{
			TestBTreeKeySortOperatorOverloadAssign<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 64);

			break;
		}

	case BTREETEST_KEYSORT_OPERATOR_OVERLOAD_ASSIGN_MULTI_INSTANCE	:
		{
			TestBTreeKeySortOperatorOverloadAssignMultiInstance<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 64, 32, 5);

			break;
		}

	case BTREETEST_KEYSORT_GET_INIT_POS_OF_KEY_ON_LEAF_NODE	:
		{
			TestBTreeKeySortGetInitPosOfKeyOnLeafNode<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, nNodeSize);

			break;
		}
	case BTREETEST_KEYSORT_SERLIALIZE_ALL					:
		{
			TestBTreeKeySortSerialize<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 1, 128, 128);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_HALF_NODE				:
		{
			TestBTreeKeySortSerialize<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 1, 128, 64);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_PART_NODE				:
		{
			TestBTreeKeySortSerialize<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 1, 128, nNodeSize - 1);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_INCOMPLETE				:
		{
			TestBTreeKeySortSerializeIncomplete<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, nNodeSize, 128, 128, 64);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_MULTI_INSTANCE_ALL		:
		{
			TestBTreeKeySortSerialize<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 2, 64, 128);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_MULTI_INSTANCE_HALF_NODE	:
		{
			TestBTreeKeySortSerialize<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 2, 64, 64);

			break;
		}

	case BTREETEST_KEYSORT_SERLIALIZE_MULTI_INSTANCE_PART_NODE	:
		{
			TestBTreeKeySortSerialize<_t_obj, _t_objprim, _t_datalayerproperties, _t_datalayer> (pClKeySort, 2, 64, nNodeSize - 1);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_INSERT:
		{
			TestBTreeKeySortSTLifInsert<_t_obj> (pClKeySort, 64);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR	:
		{
			TestBTreeKeySortSTLifInsertViaIterator<_t_obj, _t_objprim, _t_datalayerproperties> (pClKeySort, 64, rDataLayerProperties, &sCacheDesc, nNodeSize);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_ERASE_VIA_ITERATOR	:
		{
			TestBTreeKeySortSTLifEraseViaIterator<_t_obj, _t_objprim, _t_datalayerproperties> (pClKeySort, 64, rDataLayerProperties, &sCacheDesc, nNodeSize);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_ERASE_VIA_KEY:
		{
			TestBTreeKeySortSTLifEraseViaKey<_t_obj, _t_objprim, _t_datalayerproperties> (pClKeySort, 64, 5, rDataLayerProperties, &sCacheDesc, nNodeSize);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE	:
		{
			TestBTreeKeySortSTLifEraseViaIteratorMultiple<_t_obj, _t_objprim, _t_datalayerproperties> (pClKeySort, 64, 5, rDataLayerProperties, &sCacheDesc, nNodeSize);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_SWAP	:
		{
			TestBTreeKeySortSTLifSwap<_t_obj, _t_objprim, _t_datalayerproperties> (pClKeySort, 128, rDataLayerProperties, &sCacheDesc, nNodeSize);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_FIND	:
		{
			TestBTreeKeySortSTLifFind<_t_obj, _t_objprim, _t_datalayerproperties> (pClKeySort, 128, rDataLayerProperties, &sCacheDesc, nNodeSize);

			break;
		}

	case BTREETEST_KEYSORT_STL_IF_LOWER_BOUND_UPPER_BOUND	:
		{
			TestBTreeKeySortSTLifLowerBoundUpperBound<_t_obj, _t_objprim, _t_datalayerproperties> (pClKeySort, 64, 5, rDataLayerProperties, &sCacheDesc, nNodeSize);

			break;
		}

	default									:
		{
			cerr << "ERROR: TestBTreeArray: Unknown test or test not specified!" << endl;

			break;
		}
	}

	delete pClKeySort;
}
