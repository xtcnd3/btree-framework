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
CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::CBTreeBaseIf (_t_subnodeiter nNodeSize)
	:	CBTreeDefaults<_t_data, _t_sizetype> ()
{
}

/*

Copy Constructor

rBT		-	reference to CBTreeBaseDefaults instance to be copied

*/

template<class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::CBTreeBaseIf (const CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> &rBT)
	:	CBTreeDefaults<_t_data, _t_sizetype> (dynamic_cast <const CBTreeDefaults<_t_data, _t_sizetype> &> (rBT))
{
}

/*

Destructor

*/

template<class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::~CBTreeBaseIf ()
{
}

template<class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> &CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::operator=
	(const CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> &rBT)
{
	// if this is not the same instance as the referenced b-tree (rBT) ...
	if (this != &rBT)
	{
		CBTreeDefaults_t			&rBaseIterThis = dynamic_cast<CBTreeDefaults_t &> (*this);
		const CBTreeDefaults_t	&rBaseIterBT = dynamic_cast<const CBTreeDefaults_t &> (rBT);
		
		rBaseIterThis = rBaseIterBT;
	}

	return (*this);
}

#endif // BTREEBASEIF_CPP
