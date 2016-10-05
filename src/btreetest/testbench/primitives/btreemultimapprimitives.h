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

#include <type_traits>

#include "testbench/application_classes/regression/btreetestmap.h"
#include "testbench/application_classes/regression/btreetestmultimap.h"

#include "specific_data_classes/btreemultimap.h"

typedef CBTreePairTest<const uint32_t, mapMap_t>										mapPair_t;

typedef CBTreePairTest<const uint32_t, multiMapMap_t>									multiMapPair_t;

typedef enum
{
	BTREETEST_MULTIMAP_PRIMITIVE_DESCEND_KEY, 
	BTREETEST_MULTIMAP_PRIMITIVE_ASCEND_KEY, 
	BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY, 
	BTREETEST_MULTIMAP_PRIMITIVE_CONST_KEY
} btreetest_multimap_primitive_seek_e;

template<class _t_container>
void multiMapPrim_add (_t_container *pClMM, typename _t_container::size_type nEntries, uint32_t &nFromWhereOrSeed, btreetest_multimap_primitive_seek_e eWhere);

#endif // !BTREEMULTIMAPPRIMITIVES_H

#include "btreemultimapprimitives.cpp"
