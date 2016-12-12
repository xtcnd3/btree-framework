/************************************************************
**
** file:	btreeramioprop.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** RAM data layer properties class.
**
************************************************************/

#ifndef BTREEIORAMPROP_CPP
#define BTREEIORAMPROP_CPP

#include "io/properties/btreeramioprop.h"

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeIOpropertiesRAM<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeIOpropertiesRAM ()
	:	CBTreeIOproperties<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype> ()
{
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeIOpropertiesRAM<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeIOpropertiesRAM (const CBTreeIOpropertiesRAM &rProperties)
	:	CBTreeIOproperties<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype> ()
{
	rProperties;
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeIOpropertiesRAM<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::~CBTreeIOpropertiesRAM ()
{
}

#endif // BTREEIORAMPROP_CPP
