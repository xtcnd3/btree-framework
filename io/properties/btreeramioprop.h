/************************************************************
**
** file:	btreeramioprop.h
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** RAM data layer properties class.
**
************************************************************/

#ifndef BTREEIORAMPROP_H
#define BTREEIORAMPROP_H

#include <stdint.h>

#include <type_traits>

#include "common/btreecommon.h"
#include "io/properties/base/btreeioprop.h"
#include "btreeioram.h"

template<class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_addresstype = uint64_t, class _t_offsettype = uint32_t>
class CBTreeIOpropertiesRAM : public CBTreeIOproperties<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
{
public:

	typedef CBTreeRAMIO<CBTreeIOpropertiesRAM<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype> >
															data_layer_type;

	typedef _t_sizetype										size_type;
	typedef _t_nodeiter										node_iter_type;
	typedef _t_subnodeiter									sub_node_iter_type;
	typedef _t_addresstype									address_type;
	typedef _t_offsettype									offset_type;

	typedef ::std::false_type								has_latency;

					CBTreeIOpropertiesRAM<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
												();

					CBTreeIOpropertiesRAM<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
												(CBTreeIOpropertiesRAM &rBT);

					~CBTreeIOpropertiesRAM<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
												();

protected:

};

#endif // BTREEIORAMPROP_H

#include "btreeramioprop.cpp"
