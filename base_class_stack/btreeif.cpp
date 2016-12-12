/************************************************************
**
** file:	btreeif.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree frameworks base class
** on the iterator interface level.
**
************************************************************/

#ifndef	BTREEBASEITERIF_CPP
#define	BTREEBASEITERIF_CPP

#include "btreeif.h"

template<class _t_data, class _t_sizetype>
CBTreeIf<_t_data, _t_sizetype>::CBTreeIf ()
	:	CBTreeSuper ()
{
}

template<class _t_data, class _t_sizetype>
CBTreeIf<_t_data, _t_sizetype>::CBTreeIf (const CBTreeIf<_t_data, _t_sizetype> &rContainer)
	:	CBTreeSuper ()
{
}

template<class _t_data, class _t_sizetype>
CBTreeIf<_t_data, _t_sizetype>::CBTreeIf (CBTreeIf<_t_data, _t_sizetype> &&rRhsContainer)
	: CBTreeSuper (dynamic_cast <CBTreeSuper &&> (rRhsContainer))
{
}

template<class _t_data, class _t_sizetype>
CBTreeIf<_t_data, _t_sizetype>::~CBTreeIf ()
{
}

/*

operator= - assignment operator overload

rContainer		- is a reference of a container that will have its content be assigned to this instance

The result is a reference to this instance.

*/

template<class _t_data, class _t_sizetype>
CBTreeIf<_t_data, _t_sizetype> &
	CBTreeIf<_t_data, _t_sizetype>::operator=
	(const CBTreeIf<_t_data, _t_sizetype> &rContainer)
{
	// if this is not the same instance as the right hand side container ...
	if (this != &rContainer)
	{
		// ... then assign all data from right hand side to left hand side container
		this->_assign (rContainer);
	}

	return (*this);
}

#endif // BTREEBASEITERIF_CPP
