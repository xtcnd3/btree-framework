/************************************************************
**
** file:	btreemultisetprimitives.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains primitive code for the b-tree framework's
** test bench to support multi set application class testing.
**
************************************************************/

#ifndef BTREEMULTISETPRIMITIVES_H
#define	BTREEMULTISETPRIMITIVES_H

#include <stdint.h>

#include "btreetestcommon.h"
#include "btreemultiset.h"

typedef enum
{
	BTREETEST_MULTISET_PRIMITIVE_DESCEND_KEY, 
	BTREETEST_MULTISET_PRIMITIVE_ASCEND_KEY, 
	BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY, 
	BTREETEST_MULTISET_PRIMITIVE_CONST_KEY
} btreetest_multiset_primitive_seek_e;

template<class _t_obj, class _t_value>
void multiSetPrim_add (_t_obj *pClMS, uint32_t nEntries, uint32_t &nFromWhereOrSeed, btreetest_multiset_primitive_seek_e eWhere);

#endif // !BTREEMULTIMAPPRIMITIVES_H

#include "btreemultisetprimitives.cpp"
