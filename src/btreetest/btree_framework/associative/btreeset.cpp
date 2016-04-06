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

#include "./associative/btreeset.h"

template <class _t_keytype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeSet
	(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize)
	:	CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
		(
			rDataLayerProperties, 
			psCacheDescription, 
			nNodeSize
		)
{
}

template <class _t_keytype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeSet
	(const CBTreeSet_t &rBT, bool bAssign)
	:	CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		dynamic_cast<const CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &> (rBT), 
		bAssign
	)
{
}

template <class _t_keytype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeSet ()
{
}

template <class _t_keytype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
template <class _t_iterator>
void CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	_t_iterator		sIt;
	bool			bSelfReverse;
	bool			bSelfReference = CBTreeSet_t::template test_self_reference_of_iterator_to_this<_t_iterator> (sItFirst, sItLast, bSelfReverse, NULL, NULL);

	if (!bSelfReference)
	{
		for (sIt = sItFirst; sIt != sItLast; sIt++)
		{
			CBTreeSet_t::insert ((value_t) (*sIt));
		}
	}
}

template <class _t_keytype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (const typename CBTreeSet_t::value_t &rData)
{
	iterator			sIter;

	_t_sizetype			nPresent = CBTreeAssociativeBase_t::count (rData);

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

template <class _t_keytype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap (CBTreeAssociativeIf<_t_keytype, _t_keytype, _t_sizetype> &rContainerIf)
{
	CBTreeSet_t		&rContainer = dynamic_cast<CBTreeSet_t &> (rContainerIf);

	this->swap (rContainer);
}

template <class _t_keytype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap (CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rContainer)
{
	if (this != &rContainer)
	{
		this->_swap (rContainer);
	}
}

template <class _t_keytype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::key_compare
	CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::key_comp () const
{
	key_compare		sRslt;

	sRslt.pThis = (CBTreeAssociativeIf_t *) (this);

	return (sRslt);
}

template <class _t_keytype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::value_compare
	CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::value_comp () const
{
	value_compare		sRslt;

	sRslt.pThis = (CBTreeAssociativeIf_t *) (this);

	return (sRslt);
}

template <class _t_keytype, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::_swap (CBTreeSet<_t_keytype, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rContainer)
{
	CBTreeAssociativeBase_t		&rSTLbaseContainerContainer = dynamic_cast <CBTreeAssociativeBase_t &> (rContainer);
	
	CBTreeAssociativeBase_t::_swap (rSTLbaseContainerContainer);
}

#endif // BTREESET_CPP
