/************************************************************
**
** file:	btreekeysorttestprimitive.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains primitive code for the b-tree framework's
** test bench to support key sort data class testing.
**
************************************************************/

#include "btreekeysorttestprimitive.h"

void keySortPrim_add (CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *pClKeySort, uint32_t nEntries, uint32_t &nDebug, uint32_t &nFromWhereOrSeed, btreetest_keysort_primitive_seek_e eWhere)
{
	uint32_t		ui32;
	keySortEntry_t	sData;

	if (eWhere == BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY)
	{
		srand (nFromWhereOrSeed);
	}

	for (ui32 = 0; ui32 < nEntries; ui32++)
	{
		cout << "insert: " << ui32 << " / " << nEntries << "\r" << flush;

		sData.nDebug = nDebug;

		nDebug++;

		switch (eWhere)
		{
		case BTREETEST_KEYSORT_PRIMITIVE_DESCEND_KEY		:
			{
				sData.nKey = nFromWhereOrSeed;

				nFromWhereOrSeed--;
				
				break;
			}

		case BTREETEST_KEYSORT_PRIMITIVE_ASCEND_KEY		:
			{
				sData.nKey = nFromWhereOrSeed;

				nFromWhereOrSeed++;
				
				break;
			}

		case BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY	:
			{
				sData.nKey = generate_rand32 ();

				break;
			}

		case BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY	:
			{
				sData.nKey = nFromWhereOrSeed;

				break;
			}

		default									:
			{
				cerr << "ERROR: keySortPrim_add: eWhere corrupted or not set!" << endl;

				exit (-1);

				break;
			}
		}

		sData.nData = generate_rand32 ();

		pClKeySort->insert_tb (sData);
	}

	cout << "insert: " << ui32 << " / " << nEntries << endl;
}

void keySortPrim_remove (CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *pClKeySort, uint32_t nEntries, uint32_t nInstance, uint32_t &nFromWhereOrSeed, btreetest_keysort_primitive_seek_e eWhere)
{
	uint32_t		ui32;
	keySortEntry_t	sKey;

	sKey.nDebug = 0;
	
	if (eWhere == BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY)
	{
		srand (nFromWhereOrSeed);
	}

	for (ui32 = 0; ui32 < nEntries; ui32++)
	{
		cout << "remove: " << ui32 << " / " << nEntries << "\r" << flush;

		switch (eWhere)
		{
		case BTREETEST_KEYSORT_PRIMITIVE_DESCEND_KEY		:
			{
				nFromWhereOrSeed--;

				sKey.nKey = nFromWhereOrSeed;
				
				break;
			}

		case BTREETEST_KEYSORT_PRIMITIVE_ASCEND_KEY		:
			{
				nFromWhereOrSeed++;

				sKey.nKey = nFromWhereOrSeed;
				
				break;
			}

		case BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY	:
			{
				sKey.nKey = generate_rand32 ();

				break;
			}

		case BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY	:
			{
				sKey.nKey = nFromWhereOrSeed;

				break;
			}

		default									:
			{
				cerr << "ERROR: keySortPrim_remove: eWhere corrupted or not set!" << endl;

				exit (-1);

				break;
			}
		}

		sKey.nData = generate_rand32 ();
		
		if (nInstance == ~0x0)
		{
			pClKeySort->erase_tb (sKey);
		}
		else
		{
			pClKeySort->erase_tb (sKey, (uint64_t) nInstance);
		}
	}

	cout << "remove: " << ui32 << " / " << nEntries << endl;
}

bool keySortPrim_compare (CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *pClKeySort0, CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *pClKeySort1)
{
	if (pClKeySort0 == pClKeySort1)
	{
		return (true);
	}

	if (pClKeySort0->size () != pClKeySort1->size ())
	{
		return (false);
	}

	uint64_t			i;
	keySortEntry_t		sData0;
	keySortEntry_t		sData1;

	for (i = 0; i < pClKeySort0->size (); i++)
	{
		pClKeySort0->get_at (i, sData0);
		pClKeySort1->get_at (i, sData1);

		if ((sData0.nKey != sData1.nKey) || (sData0.nData != sData1.nData))
		{
			return (false);
		}
	}

	return (true);
}
