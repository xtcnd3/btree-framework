/************************************************************
**
** file:	btreemultimap.cpp
** author:	Andreas Steffens
** license:	GPL v2
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

#include "./associative/btreemultimap.h"

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeMultiMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeMultiMap
	(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize)
	:	CBTreeAssociativeBase<::std::pair<_t_keytype, _t_maptype>, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
		(
			rDataLayerProperties, 
			psCacheDescription, 
			nNodeSize
		)
{
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeMultiMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeMultiMap
	(const CBTreeMultiMap_t &rBT, bool bAssign)
	:	CBTreeAssociativeBase<::std::pair<_t_keytype, _t_maptype>, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		dynamic_cast<const CBTreeAssociativeBase<::std::pair<_t_keytype, _t_maptype>, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &> (rBT), 
		bAssign
	)
{
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeMultiMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeMultiMap ()
{
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeMultiMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap (CBTreeAssociativeIf< ::std::pair<_t_keytype, _t_maptype>, _t_keytype, _t_sizetype> &rContainerIf)
{
	CBTreeMultiMap_t		&rContainer = dynamic_cast<CBTreeMultiMap_t &> (rContainerIf);

	this->swap (rContainer);
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeMultiMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap (CBTreeMultiMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rContainer)
{
	if (this != &rContainer)
	{
		this->_swap (rContainer);
	}
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeMultiMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::key_compare
	CBTreeMultiMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::key_comp () const
{
	key_compare		sRslt;

	sRslt.pThis = (CBTreeAssociativeIf_t *) (this);

	return (sRslt);
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeMultiMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::value_compare
	CBTreeMultiMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::value_comp () const
{
	value_compare		sRslt;

	sRslt.pThis = (CBTreeAssociativeIf_t *) (this);

	return (sRslt);
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_keytype *CBTreeMultiMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::extract_key (_t_keytype *pKey, const value_t &rData) const
{
	*pKey = rData.first;

	return (pKey);
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeMultiMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::_swap (CBTreeMultiMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rContainer)
{
	CBTreeAssociativeBase_t		&rSTLbaseContainerContainer = dynamic_cast <CBTreeAssociativeBase_t &> (rContainer);
	
	CBTreeAssociativeBase_t::_swap (rSTLbaseContainerContainer);
}

#endif // BTREEMULTIMAP_CPP
