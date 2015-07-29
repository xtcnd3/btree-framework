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

#include "btreemap.h"

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeMap
	(_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize)
	:	CBTreeKeySort<::std::pair<_t_keytype, _t_maptype>, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
		(
			rDataLayerProperties, 
			psCacheDescription, 
			nNodeSize
		)
{
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeMap
	(CBTreeMap_t &rBT, bool bAssign)
	:	CBTreeKeySort<::std::pair<_t_keytype, _t_maptype>, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		dynamic_cast<CBTreeKeySort<::std::pair<_t_keytype, _t_maptype>, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &> (rBT), 
		bAssign
	)
{
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeMap ()
{
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_keytype *CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::extract_key (_t_keytype *pKey, const value_t &rData)
{
	*pKey = rData.first;

	return (pKey);
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &
	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator=
		(CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT)
{
	// if this is not the same instance as the referenced b-tree (rBT) ...
	if (this != &rBT)
	{
		CBTreeKeySort_t	&rKeySortThis = dynamic_cast<CBTreeKeySort_t &> (*this);
		CBTreeKeySort_t	&rKeySortrBT = dynamic_cast<CBTreeKeySort_t &> (rBT);

		rKeySortThis = rKeySortrBT;
	}

	return (*this);
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::begin ()
{
	return (iterator (this, 0));
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::end ()
{
	return (iterator (this, this->size ()));
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rbegin ()
{
	iterator			sIter (this, 0, false);

	return (reverse_iterator (sIter));
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rend ()
{
	iterator			sIter (this, this->size (), false);

	return (reverse_iterator (sIter));
}

//template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
//typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
//	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::cbegin ()
//{
//
//}
//
//template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
//typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
//	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::cend ()
//{
//
//}
//
//template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
//typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator
//	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::crbegin ()
//{
//
//}
//
//template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
//typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator
//	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::crend ()
//{
//
//}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::max_size ()
{
	_t_sizetype		nRetval = (_t_sizetype) -1;

	nRetval &= (_t_sizetype) ~0x1;
	
	return (nRetval);
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
template <class _t_iterator>
void CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	_t_iterator		sIt;
	bool			bSelfReverse;
	bool			bSelfReference = CBTreeMap_t::template test_self_reference_of_iterator_to_this<_t_iterator> (sItFirst, sItLast, bSelfReverse, NULL, NULL);

	if (!bSelfReference)
	{
		for (sIt = sItFirst; sIt != sItLast; sIt++)
		{
			CBTreeMap_t::insert ((value_t) (*sIt));
		}
	}
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename ::std::pair<typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator, bool>
	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (const typename CBTreeMap_t::value_t &rData)
{
	insert_rslt_t		sRetVal;

	_t_sizetype			nPresent = CBTreeKeySort_t::count (rData.first);

	const_iterator		sCIterKS;

	if (nPresent == 0)
	{
		sCIterKS = CBTreeKeySort_t::insert (rData);

		sRetVal.second = true;
	}
	else
	{
		sCIterKS = CBTreeKeySort_t::find (rData.first);

		sRetVal.second = false;
	}

	iterator			sIter (this, 0, false);

	sIter = this->begin ();
	sIter += sCIterKS;

	sRetVal.first = sIter;

	return (sRetVal);
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterPos
	)
{
	const_iterator		sCIterKS = CBTreeKeySort_t::erase (sCIterPos);

	iterator						sIter (this, 0, false);

	sIter = this->begin ();
	sIter += sCIterKS;

	return (sIter);
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (const _t_keytype &rKey)
{
	return (CBTreeKeySort_t::erase (rKey));
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterFirst, 
		typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterLast
	)
{
	const_iterator		sCIterKS = CBTreeKeySort_t::erase (sCIterFirst, sCIterLast);

	iterator						sIter (this, 0, false);

	sIter = this->begin ();
	sIter += sCIterKS;

	return (sIter);
}

//template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
//void CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap (CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rKeySort)
//{
//
//}

//template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
//void CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ()
//{
//
//}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::key_compare
	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::key_comp () const
{
	key_compare		sRslt;

	return (sRslt);
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::value_compare
	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::value_comp () const
{
	value_compare		sRslt;

	return (sRslt);
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find (const _t_keytype &rKey)
{
	const_iterator		sCIterKS = CBTreeKeySort_t::find (rKey);

	iterator						sIter (this, 0, false);

	sIter = this->begin ();
	sIter += sCIterKS;

	return (sIter);
}

//template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
//_t_sizetype CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::count (const _t_key &rKey)
//{
//
//}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (const _t_keytype &rKey)
{
	const_iterator		sCIterKS = CBTreeKeySort_t::lower_bound (rKey);

	iterator						sIter (this, 0, false);

	sIter = this->begin ();
	sIter += sCIterKS;

	return (sIter);
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (const _t_keytype &rKey)
{
	const_iterator		sCIterKS = CBTreeKeySort_t::upper_bound (rKey);

	iterator						sIter (this, 0, false);

	sIter = this->begin ();
	sIter += sCIterKS;

	return (sIter);
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
int CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::comp (const _t_keytype &rKey0, const _t_keytype &rKey1)
{
	typename ::std::is_arithmetic<_t_keytype>::type		sCompareSelect;

	return (CBTreeMap_t::comp (rKey0, rKey1, sCompareSelect));
}

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
int CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::comp (const _t_keytype &rKey0, const _t_keytype &rKey1, ::std::true_type)
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

template <class _t_keytype, class _t_maptype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
int CBTreeMap<_t_keytype, _t_maptype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::comp (const _t_keytype &rKey0, const _t_keytype &rKey1, ::std::false_type)
{
	return (::memcmp ((void *) &rKey0, (void *) &rKey1, sizeof (_t_keytype)));
}

#endif // BTREEMAP_CPP
