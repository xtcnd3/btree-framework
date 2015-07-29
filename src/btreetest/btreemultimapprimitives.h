/************************************************************
**
** file:	btreemultimapprimitives.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains primitive code for the b-tree framework's
** test bench to support multi map application class testing.
**
************************************************************/

#ifndef BTREEMULTIMAPPRIMITIVES_H
#define	BTREEMULTIMAPPRIMITIVES_H

#include <stdint.h>

#include "btreetestcommon.h"
#include "btreemultimap.h"

typedef enum
{
	BTREETEST_MULTIMAP_PRIMITIVE_DESCEND_KEY, 
	BTREETEST_MULTIMAP_PRIMITIVE_ASCEND_KEY, 
	BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY, 
	BTREETEST_MULTIMAP_PRIMITIVE_CONST_KEY
} btreetest_multimap_primitive_seek_e;

template<class _t_obj, class _t_value>
void multiMapPrim_add (_t_obj *pClMM, uint32_t nEntries, uint32_t &nFromWhereOrSeed, btreetest_multimap_primitive_seek_e eWhere);

#endif // !BTREEMULTIMAPPRIMITIVES_H

#include "btreemultimapprimitives.cpp"
