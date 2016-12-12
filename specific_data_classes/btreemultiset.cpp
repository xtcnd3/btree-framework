/************************************************************
**
** file:	btreemultiset.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** multi set data class, which has an interface interchangeable
** with ::std::multiset.
**
************************************************************/

#ifndef BTREEMULTISET_CPP
#define	BTREEMULTISET_CPP

#include "specific_data_classes/btreemultiset.h"

template<class _t_keytype, class _t_datalayerproperties>
CBTreeMultiSet<_t_keytype, _t_datalayerproperties>::CBTreeMultiSet
	(const _t_datalayerproperties &rDataLayerProperties, const typename _t_datalayerproperties::sub_node_iter_type nNodeSize)
	:	CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties>
		(
			rDataLayerProperties, 
			nNodeSize
		)
{
}

template<class _t_keytype, class _t_datalayerproperties>
CBTreeMultiSet<_t_keytype, _t_datalayerproperties>::CBTreeMultiSet
	(const CBTreeMultiSet_t &rContainer, const bool bAssign)
	:	CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties>
	(
		dynamic_cast<const CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties> &> (rContainer), 
		bAssign
	)
{
}

template<class _t_keytype, class _t_datalayerproperties>
CBTreeMultiSet<_t_keytype, _t_datalayerproperties>::CBTreeMultiSet
	(CBTreeMultiSet_t &&rRhsContainer)
	:	CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties>
	(
		dynamic_cast<CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties> &&> (rRhsContainer)
	)
{
}

template<class _t_keytype, class _t_datalayerproperties>
CBTreeMultiSet<_t_keytype, _t_datalayerproperties>::~CBTreeMultiSet ()
{
}

template<class _t_keytype, class _t_datalayerproperties>
void CBTreeMultiSet<_t_keytype, _t_datalayerproperties>::swap (typename CBTreeMultiSet<_t_keytype, _t_datalayerproperties>::CBTreeAssociativeIf_t &rContainerIf)
{
	CBTreeMultiSet_t		&rContainer = dynamic_cast<CBTreeMultiSet_t &> (rContainerIf);

	this->swap (rContainer);
}

template<class _t_keytype, class _t_datalayerproperties>
void CBTreeMultiSet<_t_keytype, _t_datalayerproperties>::swap (typename CBTreeMultiSet<_t_keytype, _t_datalayerproperties>::CBTreeMultiSet_t &rContainer)
{
	if (this != &rContainer)
	{
		this->_swap (rContainer);
	}
}

template<class _t_keytype, class _t_datalayerproperties>
CBTreeMultiSet<_t_keytype, _t_datalayerproperties> &CBTreeMultiSet<_t_keytype, _t_datalayerproperties>::operator=
	(const CBTreeMultiSet_t &rContainer)
{
	// if this is not the same instance as the referenced b-tree (rContainer) ...
	if (this != &rContainer)
	{
		CBTreeAssociativeBase_t			&rAssociativeBaseThis = dynamic_cast<CBTreeAssociativeBase_t &> (*this);
		const CBTreeAssociativeBase_t	&rAssociativeBaseContainer = dynamic_cast<const CBTreeAssociativeBase_t &> (rContainer);
		
		rAssociativeBaseThis = rAssociativeBaseContainer;
	}

	return (*this);
}

template<class _t_keytype, class _t_datalayerproperties>
CBTreeMultiSet<_t_keytype, _t_datalayerproperties> &CBTreeMultiSet<_t_keytype, _t_datalayerproperties>::operator=
	(CBTreeMultiSet_t &&rRhsContainer)
{
	CBTreeAssociativeBase_t		&rAssociativeBaseThis = dynamic_cast<CBTreeAssociativeBase_t &> (*this);
	CBTreeAssociativeBase_t		&rAssociativeBaseContainer = dynamic_cast<CBTreeAssociativeBase_t &> (rRhsContainer);
		
	rAssociativeBaseThis = ::std::move (rAssociativeBaseContainer);

	return (*this);
}

template<class _t_keytype, class _t_datalayerproperties>
void CBTreeMultiSet<_t_keytype, _t_datalayerproperties>::_swap (typename CBTreeMultiSet<_t_keytype, _t_datalayerproperties>::CBTreeMultiSet_t &rContainer)
{
	CBTreeAssociativeBase_t		&rSTLbaseContainerContainer = dynamic_cast <CBTreeAssociativeBase_t &> (rContainer);
	
	CBTreeAssociativeBase_t::_swap (rSTLbaseContainerContainer);
}

#endif // BTREEMULTISET_CPP
