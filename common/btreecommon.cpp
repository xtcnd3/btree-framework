/************************************************************
**
** file:	btreecommon.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains common code for the b-tree framework.
**
************************************************************/

#ifndef BTREECOMMON_CPP
#define BTREECOMMON_CPP

#include "common/btreecommon.h"

template<class _t_type, typename _t_intrinsic>
void xor_swap (_t_type &rLhs, _t_type &rRhs)
{
	_t_intrinsic		*pnLeft = (_t_intrinsic *) &rLhs;
	_t_intrinsic		*pnRight = (_t_intrinsic *) &rRhs;

	(*pnLeft) = (*pnLeft) ^ (*pnRight);
	(*pnRight) = (*pnLeft) ^ (*pnRight);
	(*pnLeft) = (*pnLeft) ^ (*pnRight);
}

template<class _t_type>
void fast_swap (_t_type &rLhs, _t_type &rRhs)
{
	switch (sizeof (_t_type))
	{
	case	sizeof (uint8_t)	:	
		{
			xor_swap<_t_type, uint8_t> (rLhs, rRhs);

			break;
		}

	case	sizeof (uint16_t)	:	
		{
			xor_swap<_t_type, uint16_t> (rLhs, rRhs);

			break;
		}

	case	sizeof (uint32_t)	:	
		{
			xor_swap<_t_type, uint32_t> (rLhs, rRhs);

			break;
		}

	case	sizeof (uint64_t)	:	
		{
			xor_swap<_t_type, uint64_t> (rLhs, rRhs);

			break;
		}

#if defined (__local_uint128_t)

	case	sizeof (__local_uint128_t)	:	
		{
			xor_swap<_t_type, __local_uint128_t> (rLhs, rRhs);

			break;
		}

#endif

	default		:
		{
			size_t			nRemain = sizeof (_t_type);
			size_t			nStepSize;
			uint8_t			*psLhs = (uint8_t *) &rLhs;
			uint8_t			*psRhs = (uint8_t *) &rRhs;

			while (nRemain > 0)
			{
#if defined (__local_uint128_t)

				if (nRemain >= sizeof (__local_uint128_t))
				{
					nStepSize = sizeof (__local_uint128_t);

					xor_swap<__local_uint128_t, __local_uint128_t> (*((__local_uint128_t *) psLhs), *((__local_uint128_t *) psRhs));
				}
				else if (nRemain >= sizeof (uint64_t))
#else
				if (nRemain >= sizeof (uint64_t))
#endif
				{
					nStepSize = sizeof (uint64_t);

					xor_swap<uint64_t, uint64_t> (*((uint64_t *) psLhs), *((uint64_t *) psRhs));
				}
				else if (nRemain >= sizeof (uint32_t))
				{
					nStepSize = sizeof (uint32_t);

					xor_swap<uint32_t, uint32_t> (*((uint32_t *) psLhs), *((uint32_t *) psRhs));
				}
				else if (nRemain >= sizeof (uint16_t))
				{
					nStepSize = sizeof (uint16_t);

					xor_swap<uint16_t, uint16_t> (*((uint16_t *) psLhs), *((uint16_t *) psRhs));
				}
				else if (nRemain >= sizeof (uint8_t))
				{
					nStepSize = sizeof (uint8_t);

					xor_swap<uint8_t, uint8_t> (*((uint8_t *) psLhs), *((uint8_t *) psRhs));
				}

				nRemain -= nStepSize;

				psLhs += nStepSize;
				psRhs += nStepSize;
			}

			break;
		}
	}
}

#endif // BTREECOMMON_CPP
