/************************************************************
**
** file:	btreebaseif.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree frameworks base class
** on the low level interface level.
**
************************************************************/

#ifndef	BTREEBASEIF_CPP
#define	BTREEBASEIF_CPP

#include "btreebaseif.h"

template<class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::CBTreeBaseIf (const _t_subnodeiter nNodeSize)
	:	CBTreeDefaults<_t_data, _t_sizetype> ()
{
}

/*

Copy Constructor

rContainer		-	reference to CBTreeBaseDefaults instance to be copied

*/

template<class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::CBTreeBaseIf (const CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> &rContainer)
	:	CBTreeDefaults<_t_data, _t_sizetype> (dynamic_cast <const CBTreeDefaults<_t_data, _t_sizetype> &> (rContainer))
{
}

template<class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::CBTreeBaseIf (CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> &&rRhsContainer)
	:	CBTreeDefaults<_t_data, _t_sizetype> (dynamic_cast <CBTreeDefaults<_t_data, _t_sizetype> &&> (rRhsContainer))
{
}

/*

Destructor

*/

template<class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::~CBTreeBaseIf ()
{
}

#endif // BTREEBASEIF_CPP
