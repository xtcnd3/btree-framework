/************************************************************
**
** file:	btreeprimitivedefinitions.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains definitions for the b-tree framework
** test bench's common primitives.
**
************************************************************/

#ifndef BTREEPRIMITIVEDEFINITIONS_H
#define BTREEPRIMITIVEDEFINITIONS_H

typedef enum
{
	BTREETEST_KEY_GENERATION_DESCEND, 
	BTREETEST_KEY_GENERATION_ASCEND, 
	BTREETEST_KEY_GENERATION_RANDOM, 
	BTREETEST_KEY_GENERATION_CONST
} btreetest_key_generation_e;

#endif // BTREEPRIMITIVEDEFINITIONS_H
