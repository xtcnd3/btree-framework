/************************************************************
**
** file:	btreearrayitertestbench.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for the array iterator data class.
**
************************************************************/

#ifndef	BTREEARRAYITERTESTBENCH_H
#define	BTREEARRAYITERTESTBENCH_H

#include <stdint.h>

#include <iostream>
#include <typeinfo>
#include <vector>

#if defined(__GNUC__) || defined(__GNUG__)
 #include <cxxabi.h>
#endif

#include "btreetestcommon.h"
#include "btreetestarray.h"
#include "btreearraytestprimitive.h"

typedef enum
{
	BTREETEST_ARRAY_ITER_CONST_ASCEND, 
	BTREETEST_ARRAY_ITER_CONST_ASCEND_SMALL, 
	BTREETEST_ARRAY_ITER_CONST_DESCEND, 
	BTREETEST_ARRAY_ITER_CONST_DESCEND_SMALL, 
	BTREETEST_ARRAY_ITER_CONST_CMPLX_ASCEND, 
	BTREETEST_ARRAY_ITER_CONST_CMPLX_ASCEND_SMALL, 
	BTREETEST_ARRAY_ITER_CONST_CMPLX_DESCEND, 
	BTREETEST_ARRAY_ITER_CONST_CMPLX_DESCEND_SMALL, 
	BTREETEST_ARRAY_ITER_CONST_NODESIZE_VS_STEPSIZE_ASCEND, 
	BTREETEST_ARRAY_ITER_CONST_NODESIZE_VS_STEPSIZE_DESCEND, 
	BTREETEST_ARRAY_ITER_ASCEND, 
	BTREETEST_ARRAY_ITER_ASCEND_SMALL, 
	BTREETEST_ARRAY_ITER_DESCEND, 
	BTREETEST_ARRAY_ITER_DESCEND_SMALL, 
	BTREETEST_ARRAY_ITER_CMPLX_ASCEND, 
	BTREETEST_ARRAY_ITER_CMPLX_ASCEND_SMALL, 
	BTREETEST_ARRAY_ITER_CMPLX_DESCEND, 
	BTREETEST_ARRAY_ITER_CMPLX_DESCEND_SMALL, 
	BTREETEST_ARRAY_ITER_NODESIZE_VS_STEPSIZE_ASCEND, 
	BTREETEST_ARRAY_ITER_NODESIZE_VS_STEPSIZE_DESCEND, 
	BTREETEST_ARRAY_ITER_BI_DIRECTIONAL_DEREFERENCE, 
	BTREETEST_ARRAY_ITER_BI_DIRECTIONAL_SUBSCRIPTOR, 
	BTREETEST_ARRAY_ITER_BI_DIRECTIONAL_SUBSCRIPTOR_INT, 
	BTREETEST_ARRAY_ITER_COMPOUND, 
	BTREETEST_ARRAY_ITER_COMPOUND_INT, 
	BTREETEST_ARRAY_ITER_COMPOUND_ITER, 
	BTREETEST_ARRAY_ITER_ARITHMETIC_OPERATORS, 
	BTREETEST_ARRAY_ITER_COMPARE_OPERATORS, 
	BTREETEST_ARRAY_ITER_SWAP, 
	BTREETEST_ARRAY_ITER_CONST_SWAP
} btreetest_array_iter_e;

template <class _t_obj, class _t_objprim, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void TestBTreeArrayIter (uint32_t nTest, _t_subnodeiter nNodeSize, _t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t &sCacheDesc, int argc, char **argv);

#endif // BTREEARRAYITERTESTBENCH_H
