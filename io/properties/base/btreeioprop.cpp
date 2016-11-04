/************************************************************
**
** file:	btreeioprop.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** abstract data layer properties class.
**
************************************************************/

#ifndef BTREEIOPROP_CPP
#define BTREEIOPROP_CPP

#include "io/properties/base/btreeioprop.h"

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeIOproperties<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeIOproperties ()
{
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeIOproperties<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::~CBTreeIOproperties ()
{
}

#endif // BTREEIOPROP_CPP
