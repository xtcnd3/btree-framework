/************************************************************
**
** file:	btreemultisettestbench.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for the multi set application class.
**
************************************************************/

#ifndef BTREEMULTISETTESTBENCH_H
#define	BTREEMULTISETTESTBENCH_H

#include <stdint.h>

#include <map>
#include <list>
#include <vector>

#include "btreetestmultiset.h"

#include "btreemultisetprimitives.h"

#include "btreetestcommon.h"
#include "./associative/btreemultiset.h"

typedef enum
{
	BTREETEST_MULTISET_ASSIGNMENT_OPERATOR, 
	BTREETEST_MULTISET_STL_IF_INSERT, 
	BTREETEST_MULTISET_STL_IF_INSERT_VIA_ITERATOR, 
	BTREETEST_MULTISET_STL_IF_ERASE_VIA_ITERATOR, 
	BTREETEST_MULTISET_STL_IF_ERASE_VIA_KEY, 
	BTREETEST_MULTISET_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE, 
	BTREETEST_MULTISET_STL_IF_KEY_COMP, 
	BTREETEST_MULTISET_STL_IF_VALUE_COMP, 
	BTREETEST_MULTISET_STL_IF_SWAP, 
	BTREETEST_MULTISET_STL_IF_FIND, 
	BTREETEST_MULTISET_STL_IF_LOWER_BOUND_UPPER_BOUND
} btreetest_multiset_t;

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeSTLmultiSet (uint32_t nTestNum, _t_subnodeiter nNodeSize, _t_datalayerproperties sDataProperties, bayerTreeCacheDescription_t &sCacheDesc, int argc, char **argv);

#endif // !BTREEMULTISETTESTBENCH_H
