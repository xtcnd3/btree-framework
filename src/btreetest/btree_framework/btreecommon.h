/************************************************************
**
** file:	btreecommon.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains common declarations for the b-tree framework.
**
************************************************************/

#ifndef BTREECOMMON_H
#define BTREECOMMON_H

#include <stdint.h>

#include <stdexcept>

#include <ctime>

#define	BTREE_ASSERT(c,m)			if (!(c)) throw (new ::std::runtime_error (m))
#define	BTREE_ASSERT_EXCEPT(c,e,m)	if (!(c)) throw (new (e) ((m)))

typedef struct btree_time_stamp_s
{
	::std::clock_t	sTime;
	uint32_t		nAccCtr;

	bool			operator== (const struct btree_time_stamp_s &rTimeStamp)
	{
		return ((sTime == rTimeStamp.sTime) && (nAccCtr == rTimeStamp.nAccCtr));
	}

	bool			operator!= (const struct btree_time_stamp_s &rTimeStamp)
	{
		return ((sTime != rTimeStamp.sTime) || (nAccCtr != rTimeStamp.nAccCtr));
	}
} btree_time_stamp_t;

#if defined (uint128_t)

	typedef uint128_t			__local_uint128_t;

#elif defined (__int128)

	typedef unsigned __int128	__local_uint128_t;

#elif defined (__uint128_t)

	typedef __uint128_t			__local_uint128_t;
	
#endif


template <class _t_type, typename _t_intrinsic>
void				xor_swap		(_t_type &rLhs, _t_type &rRhs);

template <class _t_type>
void				fast_swap		(_t_type &rLhs, _t_type &rRhs);

#endif // BTREECOMMON_H

#include "btreecommon.cpp"
