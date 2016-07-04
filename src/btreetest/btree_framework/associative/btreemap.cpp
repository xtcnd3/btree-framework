/************************************************************
**
** file:	btreemap.cpp
** author:	Andreas Steffens
** license:	GPL v2
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

#include "./associative/btreemap.h"

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::CBTreeMap
	(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, typename _t_datalayerproperties::sub_node_iter_type nNodeSize)
	:	CBTreeAssociativeBase<typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::value_type, _t_keytype, _t_datalayerproperties>
		(
			rDataLayerProperties, 
			psCacheDescription, 
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
	_t_iterator		sIt;
	bool			bSelfReverse;
	bool			bSelfReference = CBTreeMap_t::test_self_reference_of_iterator_to_this (sItFirst, sItLast, bSelfReverse, NULL, NULL);

	if (!bSelfReference)
	{
		for (sIt = sItFirst; sIt != sItLast; sIt++)
		{
			CBTreeMap_t::insert ((value_type) (*sIt));
		}
	}
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::insert (const typename CBTreeMap_t::value_type &rData)
{
	iterator			sIter;

	size_type			nPresent = CBTreeAssociativeBase_t::count (rData.first);

	if (nPresent == 0)
	{
		sIter = CBTreeAssociative_t::insert (rData);
	}
	else
	{
		sIter = this->end ();
	}

	return (sIter);
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
void CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::swap (typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::CBTreeAssociativeIf_t &rContainerIf)
{
	CBTreeMap_t		&rContainer = dynamic_cast<CBTreeMap_t &> (rContainerIf);

	this->swap (rContainer);
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
void CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::swap (typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::CBTreeMap_t &rContainer)
{
	if (this != &rContainer)
	{
		this->_swap (rContainer);
	}
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::key_compare
	CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::key_comp () const
{
	key_compare		sRslt;

	sRslt.pThis = (CBTreeAssociativeIf_t *) (this);

	return (sRslt);
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::value_compare
	CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::value_comp () const
{
	value_compare		sRslt;

	sRslt.pThis = (CBTreeAssociativeIf_t *) (this);

	return (sRslt);
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::key_type *CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::extract_key (typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::key_type *pKey, const typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::value_type &rData) const
{
	*pKey = rData.first;

	return (pKey);
}

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties>
void CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::_swap (typename CBTreeMap<_t_keytype, _t_maptype, _t_datalayerproperties>::CBTreeMap_t &rContainer)
{
	CBTreeAssociativeBase_t		&rSTLbaseContainerContainer = dynamic_cast <CBTreeAssociativeBase_t &> (rContainer);
	
	CBTreeAssociativeBase_t::_swap (rSTLbaseContainerContainer);
}

#endif // BTREEMAP_CPP
