/************************************************************
**
** file:	btreekeysorttestprimitive.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains primitive code for the b-tree framework's
** test bench to support key sort data class testing.
**
************************************************************/

#ifndef	BTREEKEYSORTTESTPRIMITIVE_H
#define	BTREEKEYSORTTESTPRIMITIVE_H

#include <stdint.h>

#include "testbench/common/btreetestcommon.h"
#include "testbench/application_classes/regression/btreetestkeysort.h"

#include "testbench/primitives/btreeprimitivedefinitions.h"

typedef CBTreePairTest<const uint32_t, keySortMap_t>										keySortPair_t;

/*** keySortEntry_t ******************/

void container_data_reset (keySortEntry_t &rEntry);
void container_data_set (keySortEntry_t &rEntry, uint32_t nDebug, uint32_t &nFromWhereOrSeed, btreetest_key_generation_e eGenerator);

uint32_t get_entry_key (const keySortEntry_t &rEntry);
uint32_t get_entry_data (const keySortEntry_t &rEntry);
uint32_t get_entry_debug (const keySortEntry_t &rEntry);

void set_entry_key (keySortEntry_t &rEntry, uint32_t nKey);
void set_entry_data (keySortEntry_t &rEntry, uint32_t nData);
void set_entry_debug (keySortEntry_t &rEntry, uint32_t nDebug);

/*** _t_pair *************************/

template<class _t_pair>
void container_data_reset (_t_pair &rEntry)
{
	set_entry_key (rEntry, 0);
	set_entry_data (rEntry, 0);
	set_entry_debug (rEntry, 0);
}

template<class _t_pair>
void container_data_set (_t_pair &rEntry, uint32_t nDebug, uint32_t &nFromWhereOrSeed, btreetest_key_generation_e eGenerator)
{
	switch (eGenerator)
	{
		case BTREETEST_KEY_GENERATION_DESCEND	:
		{
			set_entry_key (rEntry, nFromWhereOrSeed);

			nFromWhereOrSeed--;
			
			break;
		}

		case BTREETEST_KEY_GENERATION_ASCEND	:
		{
			set_entry_key (rEntry, nFromWhereOrSeed);

			nFromWhereOrSeed++;
			
			break;
		}

		case BTREETEST_KEY_GENERATION_RANDOM	:
		{
			set_entry_key (rEntry, generate_rand32 ());

			break;
		}

		case BTREETEST_KEY_GENERATION_CONST		:
		{
			set_entry_key (rEntry, nFromWhereOrSeed);

			break;
		}

		default									:
		{
			::std::cerr << "ERROR: container_data_set<keySortEntry_>: eGenerator corrupted or not set!" << ::std::endl;

			exit (-1);

			break;
		}
	}

	set_entry_data (rEntry, generate_rand32 ());
	set_entry_debug (rEntry, nDebug);
}

template<class _t_pair>
uint32_t get_entry_key (const _t_pair &rEntry)
{
	return (rEntry.first);
}

template<class _t_pair>
uint32_t get_entry_data (const _t_pair &rEntry)
{
	return (rEntry.second.nData);
}

template<class _t_pair>
uint32_t get_entry_debug (const _t_pair &rEntry)
{
	return (rEntry.second.nDebug);
}

template<class _t_pair>
void set_entry_key (_t_pair &rEntry, uint32_t nKey)
{
	(typename ::std::remove_const<typename _t_pair::first_type>::type &) rEntry.first = nKey;
}

template<class _t_pair>
void set_entry_data (_t_pair &rEntry, uint32_t nData)
{
	rEntry.second.nData = nData;
}

template<class _t_pair>
void set_entry_debug (_t_pair &rEntry, uint32_t nDebug)
{
	rEntry.second.nDebug = nDebug;
}

/*** uint32_t ************************/

void container_data_reset (uint32_t &rEntry);
void container_data_set (uint32_t &rEntry, uint32_t nDebug, uint32_t &nFromWhereOrSeed, btreetest_key_generation_e eGenerator);

uint32_t get_entry_key (const uint32_t &rEntry);
uint32_t get_entry_data (const uint32_t &rEntry);
uint32_t get_entry_debug (const uint32_t &rEntry);

void set_entry_key (uint32_t &rEntry, uint32_t nKey);
void set_entry_data (uint32_t &rEntry, uint32_t nData);
void set_entry_debug (uint32_t &rEntry, uint32_t nDebug);

bool is_entry_not_equal_to_entry (const uint32_t &rLhsEntry, const uint32_t &rRhsEntry);

/*** generic *************************/

template<class _t_dest_data, class _t_src_data>
void entry_conversion (_t_dest_data &rDstData, const _t_src_data &rSrcData)
{
	set_entry_key (rDstData, get_entry_key (rSrcData));
	set_entry_data (rDstData, get_entry_data (rSrcData));
	set_entry_debug (rDstData, get_entry_debug (rSrcData));
}

template<class _t_lhs_data, class _t_rhs_data>
bool is_entry_not_equal_to_entry (const _t_lhs_data &rLhsEntry, const _t_rhs_data &rRhsEntry)
{
	return ((get_entry_key (rLhsEntry) != get_entry_key (rRhsEntry)) ||
			(get_entry_data (rLhsEntry) != get_entry_data (rRhsEntry)));
}

#endif // BTREEKEYSORTTESTPRIMITIVE_H
