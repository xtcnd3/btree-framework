/************************************************************
**
** file:	btreecommon.h
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains common declarations for the b-tree framework.
**
************************************************************/

#ifndef BTREECOMMON_H
#define BTREECOMMON_H

#include <stdint.h>

#if defined(__GNUC__) || defined(__GNUG__)
 #include <cxxabi.h>
#endif

#include <stdexcept>

#include <chrono>

#define	BTREE_ASSERT(c,m)			if (!(c)) throw (new ::std::runtime_error (m))
#define	BTREE_ASSERT_EXCEPT(c,e,m)	if (!(c)) throw (new (e) ((m)))

typedef struct btree_time_stamp_s
{
	::std::chrono::high_resolution_clock::time_point	sTime;
	uint32_t											nAccCtr;

	bool			operator== (const struct btree_time_stamp_s &rTimeStamp) const
	{
		return ((sTime == rTimeStamp.sTime) && (nAccCtr == rTimeStamp.nAccCtr));
	}

	bool			operator!= (const struct btree_time_stamp_s &rTimeStamp) const
	{
		return ((sTime != rTimeStamp.sTime) || (nAccCtr != rTimeStamp.nAccCtr));
	}
} btree_time_stamp_t;

template<class _t>
struct has_compare_operator_equal
{
    template<class _ti>
    static auto test(_ti*) -> decltype(std::declval<_ti>() == std::declval<_ti>());

    template<typename>
    static auto test(...) -> std::false_type;

    using type = typename std::is_same<bool, decltype(test<_t>(0))>::type;
};

#if defined (uint128_t)

	typedef uint128_t			__local_uint128_t;

#elif defined (__int128)

	typedef unsigned __int128	__local_uint128_t;

#elif defined (__uint128_t)

	typedef __uint128_t			__local_uint128_t;
	
#endif

template<class _t_type, typename _t_intrinsic>
void				xor_swap		(_t_type &rLhs, _t_type &rRhs);

template<class _t_type>
void				fast_swap		(_t_type &rLhs, _t_type &rRhs);

namespace operator_test_scope
{
	struct doesnt_exist {};

	template<class _t_data, class _t_cdata>
	doesnt_exist operator== (const _t_data &, const _t_cdata &);

	template<class _t_data, class _t_cdata = _t_data>
	struct has_equal_operator
	{
		typedef typename ::std::is_same<typename ::std::is_same<decltype (*(_t_data *)(0) == *(_t_cdata *)(0)), doesnt_exist>::type, ::std::false_type>::type		type;
	};
}

template<class _t>
void get_typename (_t &rType, ::std::string &rString)
{
#if defined(__GNUC__) || defined(__GNUG__)

	int		nStatus;
	char	*pszTypeid = abi::__cxa_demangle (typeid (_t).name (), 0, 0, &nStatus);

	if (pszTypeid != NULL)
	{
		rString = pszTypeid;
		
		free ((void *) pszTypeid);
	}
	else
	{
		rString = typeid (_t).name ();
	}
	
#else

	rString = typeid (_t).name ();
	
#endif
}

#endif // BTREECOMMON_H

#include "btreecommon.cpp"

