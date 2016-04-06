/************************************************************
**
** file:	btreearraytestprimitive.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains primitive code for the b-tree framework's
** test bench to support array data class testing.
**
************************************************************/

#include "btreearraytestprimitive.h"

void arrayPrim_add (CBTreeArrayIf<arrayEntry_t> *pClArray, uint32_t nEntries, btreetest_array_primitive_seek_e eWhere)
{
	typedef			CBTreeArrayIf<arrayEntry_t>::const_iterator			citer_t;

	uint64_t		nPos;
	uint32_t		ui32;
	arrayEntry_t	sData;
	citer_t			sCIter;

	for (ui32 = 0; ui32 < nEntries; ui32++)
	{
		cout << "insert: " << ui32 << " / " << nEntries << "\r" << flush;

		sData.nDebug = g_nDebug;
		sData.nData = generate_rand32 ();

		g_nDebug++;

		switch (eWhere)
		{
		case BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN		:
			{
				nPos = 0;

				break;
			}

		case BTREETEST_ARRAY_PRIMITIVE_SEEK_END		:
			{
				nPos = pClArray->size ();
				
				break;
			}

		case BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM	:
			{
				nPos = generate_rand64 () % (pClArray->size () + 1);

				break;
			}

		default									:
			{
				cerr << "ERROR: arrayPrim_add: eWhere corrupted or not set!" << endl;

				exit (-1);

				break;
			}
		}

		sCIter = pClArray->cbegin ();

		::std::advance<citer_t, uint64_t> (sCIter, nPos);

		pClArray->insert (sCIter, sData);
	}

	cout << "insert: " << ui32 << " / " << nEntries << endl;
}

void arrayPrim_remove (CBTreeArrayIf<arrayEntry_t> *pClArray, uint32_t nEntries, btreetest_array_primitive_seek_e eWhere)
{
	typedef			CBTreeArrayIf<arrayEntry_t>::const_iterator			citer_t;

	uint64_t		nPos;
	uint32_t		ui32;
	citer_t			sCIter;
	
	for (ui32 = 0; ui32 < nEntries; ui32++)
	{
		cout << "remove: " << ui32 << " / " << nEntries << "\r" << flush;

		switch (eWhere)
		{
		case BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN		:
			{
				nPos = 0;

				break;
			}

		case BTREETEST_ARRAY_PRIMITIVE_SEEK_END		:
			{
				nPos = pClArray->size ();
				nPos--;
				
				break;
			}

		case BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM	:
			{
				nPos = generate_rand64 () % pClArray->size ();

				break;
			}

		default									:
			{
				cerr << "ERROR: arrayPrim_remove: eWhere corrupted or not set!" << endl;

				exit (-1);

				break;
			}
		}

		sCIter = pClArray->cbegin ();

		::std::advance<citer_t, uint64_t> (sCIter, nPos);

		pClArray->erase (sCIter);
	}

	cout << "remove: " << ui32 << " / " << nEntries << endl;
}

void arrayPrim_replace (CBTreeArrayIf<arrayEntry_t> *pClArray, uint32_t nEntries, btreetest_array_primitive_seek_e eWhere)
{
	uint64_t		nPos;
	uint32_t		ui32;
	arrayEntry_t	sData;

	for (ui32 = 0; ui32 < nEntries; ui32++)
	{
		cout << "replace: " << ui32 << " / " << nEntries << "\r" << flush;

		sData.nDebug = g_nDebug;
		sData.nData = generate_rand32 ();

		g_nDebug++;

		switch (eWhere)
		{
		case BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN		:
			{
				nPos = 0;

				break;
			}

		case BTREETEST_ARRAY_PRIMITIVE_SEEK_END		:
			{
				nPos = pClArray->size ();
				nPos--;
				
				break;
			}

		case BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM	:
			{
				nPos = generate_rand64 () % pClArray->size ();

				break;
			}

		default									:
			{
				cerr << "ERROR: arrayPrim_replace: eWhere corrupted or not set!" << endl;

				exit (-1);

				break;
			}
		}

		pClArray->at (nPos) = sData;
	}

	cout << "replace: " << ui32 << " / " << nEntries << endl;
}

bool arrayPrim_compare (CBTreeArrayIf<arrayEntry_t> *pClArray0, CBTreeArrayIf<arrayEntry_t> *pClArray1)
{
	uint64_t		ui64;

	if (pClArray0 == pClArray1)
	{
		return (true);
	}

	if (pClArray0->size () != pClArray1->size ())
	{
		return (false);
	}

	for (ui64 = 0; ui64 < pClArray0->size (); ui64++)
	{
		arrayEntry_t	sData0;
		arrayEntry_t	sData1;

		sData0 = pClArray0->at (ui64);
		sData1 = pClArray1->at (ui64);

		if (sData0.nData != sData1.nData)
		{
			return (false);
		}
	}

	return (true);
}
