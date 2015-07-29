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

#include "btreetestcommon.h"
#include "btreetestkeysort.h"

typedef enum
{
	BTREETEST_KEYSORT_PRIMITIVE_DESCEND_KEY, 
	BTREETEST_KEYSORT_PRIMITIVE_ASCEND_KEY, 
	BTREETEST_KEYSORT_PRIMITIVE_RANDOM_KEY, 
	BTREETEST_KEYSORT_PRIMITIVE_CONST_KEY
} btreetest_keysort_primitive_seek_e;

void keySortPrim_add (CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *pClKeySort, uint32_t nEntries, uint32_t &nDebug, uint32_t &nFromWhereOrSeed, btreetest_keysort_primitive_seek_e eWhere);
void keySortPrim_remove (CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *pClKeySort, uint32_t nEntries, uint32_t nInstance, uint32_t &nFromWhereOrSeed, btreetest_keysort_primitive_seek_e eWhere);
bool keySortPrim_compare (CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *pClKeySort0, CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *pClKeySort1);

#endif // BTREEKEYSORTTESTPRIMITIVE_H
