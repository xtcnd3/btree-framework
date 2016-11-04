/************************************************************
**
** file:	btreeioprop.h
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** abstract data layer properties class.
**
************************************************************/

#ifndef BTREEIOPROP_H
#define BTREEIOPROP_H

#include <stdint.h>

#include "common/btreecommon.h"

template<class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_addresstype = uint64_t, class _t_offsettype = uint32_t>
class CBTreeIOproperties
{
public:

									CBTreeIOproperties<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
															();

									~CBTreeIOproperties<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
															();

};

#endif // BTREEIOPROP_H

#include "btreeioprop.cpp"
