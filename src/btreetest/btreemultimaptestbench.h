/************************************************************
**
** file:	btreemultimaptestbench.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for the multi map application class.
**
************************************************************/

#ifndef BTREEMULTIMAPTESTBENCH_H
#define	BTREEMULTIMAPTESTBENCH_H

#include <stdint.h>

#include <map>
#include <list>
#include <vector>

#include "btreetestmultimap.h"

#include "btreemultimapprimitives.h"

#include "btreetestcommon.h"
#include "btreemultimap.h"

typedef enum
{
	BTREETEST_MULTIMAP_ASSIGNMENT_OPERATOR, 
	BTREETEST_MULTIMAP_STL_IF_INSERT, 
	BTREETEST_MULTIMAP_STL_IF_INSERT_VIA_ITERATOR, 
	BTREETEST_MULTIMAP_STL_IF_ERASE_VIA_ITERATOR, 
	BTREETEST_MULTIMAP_STL_IF_ERASE_VIA_KEY, 
	BTREETEST_MULTIMAP_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE, 
	BTREETEST_MULTIMAP_STL_IF_KEY_COMP, 
	BTREETEST_MULTIMAP_STL_IF_VALUE_COMP, 
	BTREETEST_MULTIMAP_STL_IF_SWAP, 
	BTREETEST_MULTIMAP_STL_IF_FIND, 
	BTREETEST_MULTIMAP_STL_IF_LOWER_BOUND_UPPER_BOUND
} btreetest_multimap_t;

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeSTLmultiMap (uint32_t nTestNum, _t_subnodeiter nNodeSize, _t_datalayerproperties sDataProperties, bayerTreeCacheDescription_t &sCacheDesc, int argc, char **argv);

#endif // !BTREEMULTIMAPTESTBENCH_H
