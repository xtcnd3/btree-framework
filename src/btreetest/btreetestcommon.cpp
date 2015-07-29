/************************************************************
**
** file:	btreetestcommon.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** common code.
**
************************************************************/

#include "btreetestcommon.h"

uint32_t generate_rand32 ()
{
	uint32_t	nRetval = rand ();

	nRetval <<= 15;
	nRetval |= rand ();

	nRetval <<= 15;
	nRetval |= rand ();

	return (nRetval);
}

uint64_t generate_rand64 ()
{
	uint64_t	nRetval = (uint64_t) rand ();

	nRetval <<= 15ULL;
	nRetval |= (uint64_t) rand ();

	nRetval <<= 15ULL;
	nRetval |= (uint64_t) rand ();

	nRetval <<= 15ULL;
	nRetval |= (uint64_t) rand ();

	nRetval <<= 15ULL;
	nRetval |= (uint64_t) rand ();

	return (nRetval);
}
