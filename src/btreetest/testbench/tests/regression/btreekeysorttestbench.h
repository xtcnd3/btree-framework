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
#include <map>

#include "btreeioram.h"
#include "btreeiofile.h"

#include "testbench/primitives/btreeprimitivedefinitions.h"
#include "testbench/primitives/btreekeysorttestprimitive.h"
#include "testbench/primitives/btreecommonprimitives.h"
#include "testbench/common/btreetestcommon.h"
#include "testbench/application_classes/regression/btreetestkeysort.h"

#include "testbench/wrapper_classes/btreekeysorttestwrapper.h"

typedef ::std::multimap<uint32_t, keySortMap_t>				keysort_reference_t;

typedef enum
{
	BTREETEST_KEYSORT_BASIC_ASCEND, 
	BTREETEST_KEYSORT_BASIC_ASCEND_SMALL, 
	BTREETEST_KEYSORT_BASIC_DESCEND, 
	BTREETEST_KEYSORT_BASIC_DESCEND_SMALL, 
	BTREETEST_KEYSORT_BASIC_RANDOM, 
	BTREETEST_KEYSORT_BASIC_RANDOM_SMALL, 
	BTREETEST_KEYSORT_CLEAR, 
	BTREETEST_KEYSORT_REPLACE, 
	BTREETEST_KEYSORT_UNLOAD, 
	BTREETEST_KEYSORT_REMOVE_INSTANCES, 
	BTREETEST_KEYSORT_HTML_OUTPUT, 
	BTREETEST_KEYSORT_COPY_CONSTRUCTOR, 
	BTREETEST_KEYSORT_OPERATOR_OVERLOAD_ASSIGN, 
	BTREETEST_KEYSORT_OPERATOR_OVERLOAD_ASSIGN_MULTI_INSTANCE, 
	BTREETEST_KEYSORT_SERLIALIZE_ALL, 
	BTREETEST_KEYSORT_SERLIALIZE_HALF_NODE, 
	BTREETEST_KEYSORT_SERLIALIZE_PART_NODE, 
	BTREETEST_KEYSORT_SERLIALIZE_INCOMPLETE, 
	BTREETEST_KEYSORT_SERLIALIZE_MULTI_INSTANCE_ALL, 
	BTREETEST_KEYSORT_SERLIALIZE_MULTI_INSTANCE_HALF_NODE, 
	BTREETEST_KEYSORT_SERLIALIZE_MULTI_INSTANCE_PART_NODE, 
	BTREETEST_KEYSORT_STL_IF_INSERT = 1000, 
	BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR, 
	BTREETEST_KEYSORT_STL_IF_ERASE_VIA_ITERATOR, 
	BTREETEST_KEYSORT_STL_IF_ERASE_VIA_KEY, 
	BTREETEST_KEYSORT_STL_IF_ERASE_VIA_ITERATOR_MULTIPLE, 
	BTREETEST_KEYSORT_STL_IF_SWAP, 
	BTREETEST_KEYSORT_STL_IF_FIND, 
	BTREETEST_KEYSORT_STL_IF_LOWER_BOUND_UPPER_BOUND, 
	BTREETEST_KEYSORT_CODE_COVERAGE_DETERMINE_POSITION = 2000, 
	BTREETEST_KEYSORT_CODE_COVERAGE_FIND_FIRST_KEY, 
	BTREETEST_KEYSORT_CODE_COVERAGE_GET_INIT_POS_OF_KEY_ON_LEAF_NODE, 
	BTREETEST_KEYSORT_CODE_COVERAGE_SET_ITER_DATA, 
	BTREETEST_KEYSORT_CODE_COVERAGE_OPERATOR_OVERLOAD_COMPARE
} btreetest_keysort_e;

typedef enum
{
	BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART, 
	BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT, 
	BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SELF_REFERENCE, 
	BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_PART_EXTERN, 
	BTREETEST_KEYSORT_STL_IF_INSERT_VIA_ITERATOR_SAME_OR_EQUIVALENT_EXTERN, 
} btreetest_keysort_stl_if_insert_via_iterator_e;

template<class _t_container, class _t_pair_container>
void TestBTreeKeySort (uint32_t nTest, uint32_t nNodeSize, uint32_t nPageSize, _t_container *pKeySortTestWrapper, _t_pair_container *pKeySortPairTestWrapper);

#endif // BTREEARRAYTESTBENCH_H
