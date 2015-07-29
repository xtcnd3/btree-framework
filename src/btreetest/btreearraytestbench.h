/************************************************************
**
** file:	btreearraytestbench.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for the array data class.
**
************************************************************/

#ifndef	BTREEARRAYTESTBENCH_H
#define	BTREEARRAYTESTBENCH_H

#include <stdint.h>

#include <iostream>
#include <sstream>
#include <string>

#include <list>
#include <vector>

#include "btreetestcommon.h"
#include "btreetestarray.h"
#include "btreearraytestprimitive.h"

typedef enum
{
	BTREETEST_ARRAY_BASIC_FIFO, 
	BTREETEST_ARRAY_BASIC_FIFO_SMALL, 
	BTREETEST_ARRAY_BASIC_LILO, 
	BTREETEST_ARRAY_BASIC_LILO_SMALL, 
	BTREETEST_ARRAY_BASIC_RANDOM, 
	BTREETEST_ARRAY_BASIC_RANDOM_SMALL, 
	BTREETEST_ARRAY_clear, 
	BTREETEST_ARRAY_REPLACE, 
	BTREETEST_ARRAY_UNLOAD, 
	BTREETEST_ARRAY_MULTI_NODESIZES, 
	BTREETEST_ARRAY_MULTI_CACHE_SETTINGS, 
	BTREETEST_ARRAY_MULTI_TEMPLATE_PARAMETERS, 
	BTREETEST_ARRAY_HTML_OUTPUT, 
	BTREETEST_ARRAY_COPY_CONSTRUCTOR, 
	BTREETEST_ARRAY_OPERATOR_OVERLOAD_ASSIGN, 
	BTREETEST_ARRAY_OPERATOR_OVERLOAD_SUBSCRIPT, 
	BTREETEST_ARRAY_SERLIALIZE_ALL, 
	BTREETEST_ARRAY_SERLIALIZE_HALF_NODE, 
	BTREETEST_ARRAY_SERLIALIZE_PART_NODE, 
	BTREETEST_ARRAY_SERLIALIZE_INCOMPLETE, 
	BTREETEST_ARRAY_CODE_COVERAGE_ROOT_MERGE_WITH_LOWER_RIGHT, 
	BTREETEST_ARRAY_SELF_REFERENCE_VIA_SUBSCRIPT_WRAPPER, 
	BTREETEST_ARRAY_INTRINSIC_DATA_TYPE, 
	BTREETEST_ARRAY_STL_IF_AT_FRONT_BACK,
	BTREETEST_ARRAY_STL_IF_ASSIGN_FILL, 
	BTREETEST_ARRAY_STL_IF_ASSIGN_VIA_ITERATOR, 
	BTREETEST_ARRAY_STL_IF_PUSH_BACK_POP_BACK, 
	BTREETEST_ARRAY_STL_IF_INSERT_SINGLE, 
	BTREETEST_ARRAY_STL_IF_INSERT_MULTIPLE, 
	BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR, 
	BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_ITER,
	BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_CITER,
	BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_RITER,
	BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_CRITER,
	BTREETEST_ARRAY_STL_IF_ERASE_SINGLE, 
	BTREETEST_ARRAY_STL_IF_ERASE_MULTIPLE, 
	BTREETEST_ARRAY_STL_IF_SWAP
} btreetest_array_e;

template <class _t_obj, class _t_objprim, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArray (uint32_t nTest, uint32_t nNodeSize, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t &sCacheDesc, uint32_t nNumMultiCacheSizes, bayerTreeCacheDescription_t *psMultiCacheDesc, uint32_t nNumCacheProperties, _t_datalayerproperties **ppMultiCacheSizeDataLayerProperties, uint32_t nNumMultiTemplateParams, _t_objprim **ppClArraysPrim, int argc, char **argv);

#endif // BTREEARRAYTESTBENCH_H
