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

#include "testbench/common/btreetestcommon.h"
#include "testbench/application_classes/regression/btreetestarray.h"
#include "testbench/primitives/btreearraytestprimitive.h"

#include "btreeiofile.h"

typedef enum
{
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
	BTREETEST_ARRAY_ITER_CONST_SWAP, 
	BTREETEST_ARRAY_ITER_CC_SAME_INSTANCE_COMPARE = 2000, 
	BTREETEST_ARRAY_ITER_CC_IS_TIME_STAMP_UP_TO_DATE_AFTER_RE_ASSIGNMENT, 
	BTREETEST_ARRAY_ITER_CC_UPDATE_TIME_STAMP_RE_REGISTER, 
	BTREETEST_ARRAY_ITER_CC_ASSIGN_UNINITIALIZED_INSTANCE, 
	BTREETEST_ARRAY_ITER_CC_ALL_BEGIN_END_METHODS
} btreetest_array_iter_e;

template<class _t_sizetype>
void TestBTreeArrayIter (uint32_t nTest, uint32_t nNodeSize, uint32_t nPageSize);

#endif // BTREEARRAYITERTESTBENCH_H
