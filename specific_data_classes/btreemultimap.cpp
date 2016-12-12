/************************************************************
**
** file:	btreemultimap.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** multi map data class, which has an interface interchangeable
** with ::std::multimap.
**
************************************************************/

#ifndef BTREEMULTIMAP_CPP
#define	BTREEMULTIMAP_CPP

#include "specific_data_classes/btreemultimap.h"

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::CBTreeMultiMap
	(const _t_datalayerproperties &rDataLayerProperties, const typename _t_datalayerproperties::sub_node_iter_type nNodeSize)
	:	CBTreeAssociativeBase<typename CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::value_type, _t_keytype, _t_datalayerproperties>
		(
			rDataLayerProperties, 
			nNodeSize
		)
{
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::CBTreeMultiMap
	(const CBTreeMultiMap_t &rContainer, const bool bAssign)
	:	CBTreeAssociativeBase<typename CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::value_type, _t_keytype, _t_datalayerproperties>
	(
		dynamic_cast<const CBTreeAssociativeBase<typename CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::value_type, _t_keytype, _t_datalayerproperties> &> (rContainer), 
		bAssign
	)
{
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::CBTreeMultiMap
	(CBTreeMultiMap_t &&rRhsContainer)
	:	CBTreeAssociativeBase<typename CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::value_type, _t_keytype, _t_datalayerproperties>
	(
		dynamic_cast<CBTreeAssociativeBase<typename CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::value_type, _t_keytype, _t_datalayerproperties> &&> (rRhsContainer)
	)
{
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::~CBTreeMultiMap ()
{
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
void CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::swap (typename CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::CBTreeAssociativeIf_t &rContainerIf)
{
	CBTreeMultiMap_t		&rContainer = dynamic_cast<CBTreeMultiMap_t &> (rContainerIf);

	this->swap (rContainer);
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
void CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::swap (typename CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::CBTreeMultiMap_t &rContainer)
{
	if (this != &rContainer)
	{
		this->_swap (rContainer);
	}
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties> &CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::operator=
	(const CBTreeMultiMap_t &rContainer)
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

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties> &CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::operator=
	(CBTreeMultiMap_t &&rRhsContainer)
{
	CBTreeAssociativeBase_t		&rAssociativeBaseThis = dynamic_cast<CBTreeAssociativeBase_t &> (*this);
	CBTreeAssociativeBase_t		&rAssociativeBaseContainer = dynamic_cast<CBTreeAssociativeBase_t &> (rRhsContainer);
		
	rAssociativeBaseThis = ::std::move (rAssociativeBaseContainer);

	return (*this);
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
typename CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::key_type *CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::extract_key (typename CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::key_type *pKey, const typename CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::value_type &rData) const
{
	pKey;

	return ((typename ::std::remove_const<_t_keytype>::type *) &(rData.first));
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
void CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::_swap (typename CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>::CBTreeMultiMap_t &rContainer)
{
	CBTreeAssociativeBase_t		&rSTLbaseContainerContainer = dynamic_cast <CBTreeAssociativeBase_t &> (rContainer);
	
	CBTreeAssociativeBase_t::_swap (rSTLbaseContainerContainer);
}

#endif // BTREEMULTIMAP_CPP
