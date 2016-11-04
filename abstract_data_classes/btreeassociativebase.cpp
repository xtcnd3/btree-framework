/************************************************************
**
** file:	btreeassociativebase.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** STL base class, containing a common sub-set of methods
** shared between all STL interchangeable container classes.
**
************************************************************/

#ifndef BTREEASSOCAITIVEBASE_CPP
#define	BTREEASSOCAITIVEBASE_CPP

#include "abstract_data_classes/btreeassociativebase.h"

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::CBTreeAssociativeBase
	(_t_datalayerproperties &rDataLayerProperties, typename _t_datalayerproperties::sub_node_iter_type nNodeSize)
	:	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>
		(
			rDataLayerProperties, 
			nNodeSize
		)
{
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::CBTreeAssociativeBase
	(const CBTreeAssociativeBase_t &rBT, bool bAssign)
	:	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>
	(
		dynamic_cast<const CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties> &> (rBT), 
		bAssign
	)
{
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::~CBTreeAssociativeBase ()
{
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties> &
	CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::operator=
		(const CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties> &rBT)
{
	// if this is not the same instance as the referenced b-tree (rBT) ...
	if (this != &rBT)
	{
		CBTreeAssociative_t			&rAssociativeThis = dynamic_cast<CBTreeAssociative_t &> (*this);
		const CBTreeAssociative_t	&rAssociativerBT = dynamic_cast<const CBTreeAssociative_t &> (rBT);

		rAssociativeThis = rAssociativerBT;
	}

	return (*this);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename _t_datalayerproperties::size_type CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::max_size () const
{
	size_type		nRetval = (size_type) -1;

	nRetval &= (size_type) ~0x1;
	
	return (nRetval);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::lower_bound (const _t_key &rKey)
{
	return (CBTreeAssociative_t::lower_bound (rKey));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::const_iterator
	CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::lower_bound (const _t_key &rKey) const
{
	return (CBTreeAssociative_t::lower_bound (rKey));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::upper_bound (const _t_key &rKey)
{
	return (CBTreeAssociative_t::upper_bound (rKey));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::const_iterator
	CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::upper_bound (const _t_key &rKey) const
{
	return (CBTreeAssociative_t::upper_bound (rKey));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
int CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::comp (const _t_key &rKey0, const _t_key &rKey1) const
{
	typename ::std::is_arithmetic<_t_key>::type		sCompareSelect;

	return (CBTreeAssociativeBase_t::comp (rKey0, rKey1, sCompareSelect));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
int CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::comp (const _t_key &rKey0, const _t_key &rKey1, ::std::true_type)
{
	if (rKey0 < rKey1)
	{
		return (-1);
	}
	else if (rKey0 == rKey1)
	{
		return (0);
	}
	else
	{
		return (1);
	}
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
int CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::comp (const _t_key &rKey0, const _t_key &rKey1, ::std::false_type)
{
	return (::memcmp ((void *) &rKey0, (void *) &rKey1, sizeof (_t_key)));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::_swap (CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties> &rContainer)
{
	CBTreeAssociative_t		&rAssociativeContainer = dynamic_cast<CBTreeAssociative_t &> (rContainer);

	CBTreeAssociative_t::_swap (rAssociativeContainer);
}

#endif // BTREEASSOCAITIVEBASE_CPP
