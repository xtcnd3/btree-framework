/************************************************************
**
** file:	btreemap.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** map data class, which has an interface interchangeable with
** ::std::map.
**
************************************************************/

#ifndef BTREEMAP_CPP
#define	BTREEMAP_CPP

#include "specific_data_classes/btreemap.h"

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::CBTreeMap
	(_t_datalayerproperties &rDataLayerProperties, typename _t_datalayerproperties::sub_node_iter_type nNodeSize)
	:	CBTreeAssociativeBase<typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::value_type, _t_keytype, _t_datalayerproperties>
		(
			rDataLayerProperties, 
			nNodeSize
		)
{
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::CBTreeMap
	(const CBTreeMap_t &rBT, bool bAssign)
	:	CBTreeAssociativeBase<typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::value_type, _t_keytype, _t_datalayerproperties>
	(
		dynamic_cast<const CBTreeAssociativeBase<typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::value_type, _t_keytype, _t_datalayerproperties> &> (rBT), 
		bAssign
	)
{
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::~CBTreeMap ()
{
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
template<class _t_iterator>
void CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	CBTreeAssociativeBase_t::template _insert_unique <_t_iterator> (sItFirst, sItLast);
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::insert (const typename CBTreeMap_t::value_type &rData)
{
	return (CBTreeAssociativeBase_t::_insert_unique (rData));
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::insert (typename CBTreeMap_t::const_iterator sCIterHint, const typename CBTreeMap_t::value_type &rData)
{
	return (CBTreeAssociativeBase_t::_insert_unique (sCIterHint, rData));
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::emplace (typename CBTreeMap_t::value_type && rrData)
{
	return (CBTreeAssociativeBase_t::_emplace_unique (::std::forward<value_type> (rrData)));
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::emplace_hint (const_iterator sCIterHint, typename CBTreeMap_t::value_type && rrData)
{
	return (CBTreeAssociativeBase_t::_emplace_hint_unique (sCIterHint, ::std::forward<value_type> (rrData)));
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
template<class ..._t_va_args>
typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::emplace (_t_va_args && ... rrArgs)
{
	return (CBTreeAssociativeBase_t::_emplace_unique (::std::forward<_t_va_args> (rrArgs) ...));
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
template<class ..._t_va_args>
typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::emplace_hint (const_iterator sCIterHint, _t_va_args && ... rrArgs)
{
	return (CBTreeAssociativeBase_t::_emplace_hint_unique (sCIterHint, ::std::forward<_t_va_args> (rrArgs) ...));
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::equal_range_type
	CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::equal_range (const key_type &rKey)
{
	return (CBTreeAssociativeBase_t::template _equal_range_unique <iterator> (rKey));
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::equal_range_const_type
	CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::equal_range (const key_type &rKey) const
{
	return (CBTreeAssociativeBase_t::template _equal_range_unique <const_iterator> (rKey));
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
void CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::swap (typename CBTreeMap_t::CBTreeAssociativeIf_t &rContainerIf)
{
	CBTreeMap_t		&rContainer = dynamic_cast<CBTreeMap_t &> (rContainerIf);

	this->swap (rContainer);
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
void CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::swap (CBTreeMap_t &rContainer)
{
	if (this != &rContainer)
	{
		this->_swap (rContainer);
	}
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::key_type *CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::extract_key (typename CBTreeMap_t::key_type *pKey, const typename CBTreeMap_t::value_type &rData) const
{
	pKey;

	return ((typename ::std::remove_const<_t_keytype>::type *) &(rData.first));
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
void CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::_swap (CBTreeMap_t &rContainer)
{
	CBTreeAssociativeBase_t		&rSTLbaseContainerContainer = dynamic_cast <CBTreeAssociativeBase_t &> (rContainer);
	
	CBTreeAssociativeBase_t::_swap (rSTLbaseContainerContainer);
}

#endif // BTREEMAP_CPP
