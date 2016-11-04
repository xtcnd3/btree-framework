/************************************************************
**
** file:	btreekeysort.cpp
** author:	Andreas Steffens
** license:	LGPL v3
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

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>::CBTreeKeySort
	(
		_t_datalayerproperties &rDataLayerProperties, 
		typename _t_datalayerproperties::sub_node_iter_type nNodeSize
	)
	:	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>
	(
		rDataLayerProperties, 
		nNodeSize
	)
{
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>::CBTreeKeySort (CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties> &rBT, bool bAssign)
	:	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>
	(
		dynamic_cast <CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties> &> (rBT), bAssign
	)
{
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>::~CBTreeKeySort ()
{
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>::swap (CBTreeAssociativeIf<_t_data, _t_key, typename _t_datalayerproperties::size_type> &rContainerIf)
{
	CBTreeKeySort_t		&rContainer = dynamic_cast<CBTreeKeySort_t &> (rContainerIf);

	this->swap (rContainer);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>::swap 
	(
		CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties> &rContainer
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

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>::clear ()
{
	// since the compiler cannot know if you want to call CBTreeKeySortDataIf::clear or CBTreeBaseDefaults::clear the code has to be explicit
	CBTreeBaseDefaults_t::clear ();
}

/*

operator= - assignment operator overload

rBT		- is a reference to a b-tree that will have its content be assigned to this instance

The result is a reference to this instance.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties> &CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>::operator=
	(const CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties> &rBT)
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

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>::set_iter_data (void *pState, const typename CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>::value_type &rData)
{
	iterator_state_t	*pIterState = (iterator_state_t *) pState;
	const_iterator		sCIter (this, pIterState->nAssociatedPos, pState, this->get_time_stamp (), false);

	_t_data		*pData = this->get_data (pIterState->nNode, pIterState->nSubPos);
	_t_key		sKeyContainer;
	_t_key		sKeyNew;
	_t_key		*psKeyContainer;
	_t_key		*psKeyNew;

	psKeyContainer = this->extract_key (&sKeyContainer, *pData);
	psKeyNew = this->extract_key (&sKeyNew, rData);

	if (this->comp (*psKeyContainer, *psKeyNew) == 0)
	{
		*pData = rData;
	}
	else
	{
		CBTreeKeySort_t::erase (sCIter);

		CBTreeKeySort_t::insert (rData);
	}
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>::_swap
	(
		CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties> &rContainer
	)
{
	CBTreeAssociative_t	&rKeySortAssociative = dynamic_cast <CBTreeAssociative_t &> (rContainer);

	CBTreeAssociative_t::_swap (rKeySortAssociative);
}

#endif // BTREEKEYSORT_CPP
