/************************************************************
**
** file:	btreearraytestprimitive.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains primitive code for the b-tree framework's
** test bench to support array data class testing.
**
************************************************************/

#ifndef	BTREEARRAYTESTPRIMITIVE_H
#define	BTREEARRAYTESTPRIMITIVE_H

#include <stdint.h>

#include <iostream>

#include "testbench/common/btreetestcommon.h"
#include "testbench/application_classes/regression/btreetestarray.h"

typedef enum
{
	BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN, 
	BTREETEST_ARRAY_PRIMITIVE_SEEK_END, 
	BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM
} btreetest_array_primitive_seek_e;

template<class _t_container>
void arrayPrim_add (_t_container *pContainer, typename _t_container::size_type nEntries, btreetest_array_primitive_seek_e eWhere);

template<class _t_container>
void arrayPrim_remove (_t_container *pContainer, typename _t_container::size_type nEntries, btreetest_array_primitive_seek_e eWhere);

template<class _t_container>
void arrayPrim_replace (_t_container *pContainer, typename _t_container::size_type nEntries, btreetest_array_primitive_seek_e eWhere);

#include "btreearraytestprimitive.cpp"

#endif // BTREEARRAYTESTPRIMITIVE_H
