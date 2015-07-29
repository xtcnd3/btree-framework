/************************************************************
**
** file:	btreetestcommon.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** common code.
**
************************************************************/

#ifndef	BTREETESTCOMMON_H
#define	BTREETESTCOMMON_H

#include <stdint.h>
#include <stdlib.h>

uint32_t generate_rand32 ();
uint64_t generate_rand64 ();

#endif // BTREETESTCOMMON_H
