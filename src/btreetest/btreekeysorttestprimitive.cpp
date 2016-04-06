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

#ifndef	BTREEKEYSORTTESTPRIMITIVE_CPP
#define	BTREEKEYSORTTESTPRIMITIVE_CPP

#include "btreekeysorttestprimitive.h"

void container_data_reset (keySortEntry_t &rData)
{
	rData.nKey = 0;
	rData.nData = 0;
	rData.nDebug = 0;
}

void container_data_set (keySortEntry_t &rData, uint32_t nDebug, uint32_t &nFromWhereOrSeed, btreetest_key_generation_e eGenerator)
{
	rData.nDebug = nDebug;

	switch (eGenerator)
	{
		case BTREETEST_KEY_GENERATION_DESCEND	:
		{
			rData.nKey = nFromWhereOrSeed;

			nFromWhereOrSeed--;
			
			break;
		}

		case BTREETEST_KEY_GENERATION_ASCEND	:
		{
			rData.nKey = nFromWhereOrSeed;

			nFromWhereOrSeed++;
			
			break;
		}

		case BTREETEST_KEY_GENERATION_RANDOM	:
		{
			rData.nKey = generate_rand32 ();

			break;
		}

		case BTREETEST_KEY_GENERATION_CONST		:
		{
			rData.nKey = nFromWhereOrSeed;

			break;
		}

		default									:
		{
			cerr << "ERROR: container_data_set<keySortEntry_>: eGenerator corrupted or not set!" << endl;

			exit (-1);

			break;
		}
	}

	rData.nData = generate_rand32 ();
}

uint32_t get_entry_key (const keySortEntry_t &rData)
{
	return (rData.nKey);
}

uint32_t get_entry_data (const keySortEntry_t &rData)
{
	return (rData.nData);
}

uint32_t get_entry_debug (const keySortEntry_t &rData)
{
	return (rData.nDebug);
}

void set_entry_key (keySortEntry_t &rEntry, uint32_t nKey)
{
	rEntry.nKey = nKey;
}

void set_entry_data (keySortEntry_t &rEntry, uint32_t nData)
{
	rEntry.nData = nData;
}

void set_entry_debug (keySortEntry_t &rEntry, uint32_t nDebug)
{
	rEntry.nDebug = nDebug;
}

#endif // BTREEKEYSORTTESTPRIMITIVE_CPP
