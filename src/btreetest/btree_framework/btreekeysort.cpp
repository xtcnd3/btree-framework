/************************************************************
**
** file:	btreekeysort.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** key sort (ordered associative) data class.
**
************************************************************/

#ifndef	BTREEKEYSORT_CPP
#define	BTREEKEYSORT_CPP

#include "btreekeysort.h"

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySort
	(
		_t_datalayerproperties &rDataLayerProperties, 
		const bayerTreeCacheDescription_t *psCacheDescription, 
		_t_subnodeiter nNodeSize
	)
	:	CBTreeAssociative <_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		rDataLayerProperties, 
		psCacheDescription, 
		nNodeSize
	)
{
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySort (CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign)
	:	CBTreeAssociative <_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		dynamic_cast <CBTreeAssociative <_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &> (rBT), bAssign
	)
{
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeKeySort ()
{
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap (CBTreeAssociativeIf<_t_data, _t_key, _t_sizetype> &rContainerIf)
{
	CBTreeKeySort_t		&rContainer = dynamic_cast<CBTreeKeySort_t &> (rContainerIf);

	this->swap (rContainer);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap 
	(
		CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rContainer
	)
{
	if (this != &rContainer)
	{
		this->_swap (rContainer);
	}
}

/*

clear - remove all

This method tell the instance in question to destroy the underlying data layer and
reset all size counts to zero.

The method returns true if it was successful, otherwise false.
If false is returned the data was destroyed already.

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ()
{
	// since the compiler cannot know if you want to call CBTreeKeySortDataIf::clear or CBTreeBaseDefaults::clear the code has to be explicit
	CBTreeBase_t::clear ();
}

/*

operator= - assignment operator overload

rBT		- is a reference to a b-tree that will have its content be assigned to this instance

The result is a reference to this instance.

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator=
	(const CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT)
{
	// if this is not the same instance as the referenced b-tree (rBT) ...
	if (this != &rBT)
	{
		// ... then destroy this data layer
		this->clear ();

		// and copy all data from rBT to this instance
		this->assign (rBT);
	}

	return (*this);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_iter_data (void *pState, const _t_data &rData)
{
	iterator_state_t	*pIterState = (iterator_state_t *) pState;
	const_iterator		sCIter (this, pIterState->nAssociatedPos, pState, this->get_time_stamp (), false);

	_t_data		*pData = this->get_data (pIterState->nNode, pIterState->nSubPos);
	_t_key		sKeyContainer;
	_t_key		sKeyNew;

	this->extract_key (&sKeyContainer, *pData);
	this->extract_key (&sKeyNew, rData);

	if (this->comp (sKeyContainer, sKeyNew) == 0)
	{
		*pData = rData;
	}
	else
	{
		this->erase (sCIter);

		this->insert (rData);
	}
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::_swap
	(
		CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rContainer
	)
{
	//CBTreeAssociative_t	&rThisAssociative = dynamic_cast <CBTreeAssociative_t &> (*this);
	CBTreeAssociative_t	&rKeySortAssociative = dynamic_cast <CBTreeAssociative_t &> (rContainer);

	CBTreeAssociative_t::_swap (rKeySortAssociative);
}

#endif // BTREEKEYSORT_CPP
