/************************************************************
**
** file:	btreemaptestbench.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for the map application class.
**
************************************************************/

#ifndef BTREEMAPTESTBENCH_H
#define	BTREEMAPTESTBENCH_H

#include <stdint.h>

#include <map>
#include <list>
#include <vector>

#include "btreetestmap.h"

#include "btreemultimapprimitives.h"

#include "btreetestcommon.h"
#include "btreemap.h"

typedef enum
{
	BTREETEST_MAP_ASSIGNMENT_OPERATOR, 
	BTREETEST_MAP_STL_IF_INSERT, 
	BTREETEST_MAP_STL_IF_INSERT_VIA_ITERATOR, 
	BTREETEST_MAP_STL_IF_ERASE_VIA_ITERATOR, 
	BTREETEST_MAP_STL_IF_ERASE_VIA_KEY, 
	BTREETEST_MAP_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE, 
	BTREETEST_MAP_STL_IF_KEY_COMP, 
	BTREETEST_MAP_STL_IF_VALUE_COMP, 
	BTREETEST_MAP_STL_IF_SWAP, 
	BTREETEST_MAP_STL_IF_FIND, 
	BTREETEST_MAP_STL_IF_LOWER_BOUND_UPPER_BOUND
} btreetest_map_t;

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeSTLmap (uint32_t nTestNum, _t_subnodeiter nNodeSize, _t_datalayerproperties sDataProperties, bayerTreeCacheDescription_t &sCacheDesc, int argc, char **argv);

#endif // !BTREEMAPTESTBENCH_H
