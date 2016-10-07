/************************************************************
**
** file:	btreeset.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** set data class, which has an interface interchangeable
** with ::std::set.
**
************************************************************/

#ifndef BTREESET_CPP
#define	BTREESET_CPP

#include "specific_data_classes/btreeset.h"

template<class _t_keytype, class _t_datalayerproperties>
CBTreeSet<_t_keytype, _t_datalayerproperties>::CBTreeSet
	(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, typename _t_datalayerproperties::sub_node_iter_type nNodeSize)
	:	CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties>
		(
			rDataLayerProperties, 
			psCacheDescription, 
			nNodeSize
		)
{
}

template<class _t_keytype, class _t_datalayerproperties>
CBTreeSet<_t_keytype, _t_datalayerproperties>::CBTreeSet
	(const CBTreeSet_t &rBT, bool bAssign)
	:	CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties>
	(
		dynamic_cast<const CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties> &> (rBT), 
		bAssign
	)
{
}

template<class _t_keytype, class _t_datalayerproperties>
CBTreeSet<_t_keytype, _t_datalayerproperties>::~CBTreeSet ()
{
}

template<class _t_keytype, class _t_datalayerproperties>
template<class _t_iterator>
void CBTreeSet<_t_keytype, _t_datalayerproperties>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	_t_iterator		sIt;
	bool			bSelfReverse;
	bool			bSelfReference = CBTreeSet_t::test_self_reference_of_iterator_to_this (sItFirst, sItLast, bSelfReverse, NULL, NULL);

	if (!bSelfReference)
	{
		for (sIt = sItFirst; sIt != sItLast; sIt++)
		{
			CBTreeSet_t::insert ((value_type) (*sIt));
		}
	}
}

template<class _t_keytype, class _t_datalayerproperties>
typename CBTreeSet<_t_keytype, _t_datalayerproperties>::iterator
	CBTreeSet<_t_keytype, _t_datalayerproperties>::insert (const typename CBTreeSet<_t_keytype, _t_datalayerproperties>::value_type &rData)
{
	iterator			sIter;

	size_type			nPresent = CBTreeAssociativeBase_t::count (rData);

	if (nPresent == 0)
	{
		sIter = CBTreeAssociative_t::insert (rData);
	}
	else
	{
		sIter = this->cend ();
	}

	return (sIter);
}

template<class _t_keytype, class _t_datalayerproperties>
void CBTreeSet<_t_keytype, _t_datalayerproperties>::swap (typename CBTreeSet<_t_keytype, _t_datalayerproperties>::CBTreeAssociativeIf_t &rContainerIf)
{
	CBTreeSet_t		&rContainer = dynamic_cast<CBTreeSet_t &> (rContainerIf);

	this->swap (rContainer);
}

template<class _t_keytype, class _t_datalayerproperties>
void CBTreeSet<_t_keytype, _t_datalayerproperties>::swap (typename CBTreeSet<_t_keytype, _t_datalayerproperties>::CBTreeSet_t &rContainer)
{
	if (this != &rContainer)
	{
		this->_swap (rContainer);
	}
}

//template<class _t_keytype, class _t_datalayerproperties>
//typename CBTreeSet<_t_keytype, _t_datalayerproperties>::key_compare
//	CBTreeSet<_t_keytype, _t_datalayerproperties>::key_comp () const
//{
//	key_compare		sRslt;
//
//	sRslt.pThis = (CBTreeAssociativeIf_t *) (this);
//
//	return (sRslt);
//}
//
//template<class _t_keytype, class _t_datalayerproperties>
//typename CBTreeSet<_t_keytype, _t_datalayerproperties>::value_compare
//	CBTreeSet<_t_keytype, _t_datalayerproperties>::value_comp () const
//{
//	value_compare		sRslt;
//
//	sRslt.pThis = (CBTreeAssociativeIf_t *) (this);
//
//	return (sRslt);
//}

template<class _t_keytype, class _t_datalayerproperties>
void CBTreeSet<_t_keytype, _t_datalayerproperties>::_swap (typename CBTreeSet<_t_keytype, _t_datalayerproperties>::CBTreeSet_t &rContainer)
{
	CBTreeAssociativeBase_t		&rSTLbaseContainerContainer = dynamic_cast <CBTreeAssociativeBase_t &> (rContainer);
	
	CBTreeAssociativeBase_t::_swap (rSTLbaseContainerContainer);
}

#endif // BTREESET_CPP
