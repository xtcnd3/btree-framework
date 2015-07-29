/************************************************************
**
** file:	btreekeysorttestbench.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for the key sort data class.
**
************************************************************/

#ifndef	BTREEKEYSORTTESTBENCH_H
#define	BTREEKEYSORTTESTBENCH_H

#include <stdio.h>
#include <stdint.h>

#include <iostream>
#include <sstream>
#include <string>

#include <list>
#include <vector>

#include "btreeioram.h"
#include "btreeiofile.h"

#include "btreetestcommon.h"
#include "btreetestkeysort.h"
#include "btreekeysorttestprimitive.h"

typedef enum
{
	BTREETEST_KEYSORT_BASIC_ASCEND, 
	BTREETEST_KEYSORT_BASIC_ASCEND_SMALL, 
	BTREETEST_KEYSORT_BASIC_DESCEND, 
	BTREETEST_KEYSORT_BASIC_DESCEND_SMALL, 
	BTREETEST_KEYSORT_BASIC_RANDOM, 
	BTREETEST_KEYSORT_BASIC_RANDOM_SMALL, 
	BTREETEST_KEYSORT_clear, 
	BTREETEST_KEYSORT_REPLACE, 
	BTREETEST_KEYSORT_UNLOAD, 
	BTREETEST_KEYSORT_REMOVE_INSTANCES, 
	BTREETEST_KEYSORT_MULTI_NODESIZES, 
	BTREETEST_KEYSORT_MULTI_CACHE_SETTINGS, 
	BTREETEST_KEYSORT_MULTI_TEMPLATE_PARAMETERS, 
	BTREETEST_KEYSORT_CODE_COVERAGE_DETERMINE_POSITION, 
	BTREETEST_KEYSORT_CODE_COVERAGE_FIND_FIRST_KEY, 
	BTREETEST_KEYSORT_CODE_COVERAGE_GET, 
	BTREETEST_KEYSORT_HTML_OUTPUT, 
	BTREETEST_KEYSORT_COPY_CONSTRUCTOR, 
	BTREETEST_KEYSORT_OPERATOR_OVERLOAD_ASSIGN, 
	BTREETEST_KEYSORT_OPERATOR_OVERLOAD_ASSIGN_MULTI_INSTANCE, 
	BTREETEST_KEYSORT_GET_INIT_POS_OF_KEY_ON_LEAF_NODE, 
	BTREETEST_KEYSORT_SERLIALIZE_ALL, 
	BTREETEST_KEYSORT_SERLIALIZE_HALF_NODE, 
	BTREETEST_KEYSORT_SERLIALIZE_PART_NODE, 
	BTREETEST_KEYSORT_SERLIALIZE_INCOMPLETE, 
	BTREETEST_KEYSORT_SERLIALIZE_MULTI_INSTANCE_ALL, 
	BTREETEST_KEYSORT_SERLIALIZE_MULTI_INSTANCE_HALF_NODE, 
	BTREETEST_KEYSORT_SERLIALIZE_MULTI_INSTANCE_PART_NODE, 
	BTREETEST_KEYSORT_STL_IF_INSERT, 
	BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR, 
	BTREETEST_KEYSORT_STL_IF_ERASE_VIA_ITERATOR, 
	BTREETEST_KEYSORT_STL_IF_ERASE_VIA_KEY, 
	BTREETEST_KEYSORT_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE, 
	BTREETEST_KEYSORT_STL_IF_SWAP, 
	BTREETEST_KEYSORT_STL_IF_FIND, 
	BTREETEST_KEYSORT_STL_IF_LOWER_BOUND_UPPER_BOUND
} btreetest_keysort_e;

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeKeySort (uint32_t nTest, uint32_t nNodeSize, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t &sCacheDesc, uint32_t nNumMultiCacheSizes, bayerTreeCacheDescription_t *psMultiCacheDesc, uint32_t nNumCacheProperties, _t_datalayerproperties **ppMultiCacheSizeDataLayerProperties, uint32_t nNumMultiTemplateParams, _t_objprim **ppClKeySortsPrim, int argc, char **argv);

#endif // BTREEARRAYTESTBENCH_H
