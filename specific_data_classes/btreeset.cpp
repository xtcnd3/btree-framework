/************************************************************
**
** file:	btreeset.cpp
** author:	Andreas Steffens
** license:	LGPL v3
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
	(_t_datalayerproperties &rDataLayerProperties, typename _t_datalayerproperties::sub_node_iter_type nNodeSize)
	:	CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties>
		(
			rDataLayerProperties, 
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
typename CBTreeSet<_t_keytype, _t_datalayerproperties>::iterator
	CBTreeSet<_t_keytype, _t_datalayerproperties>::insert (typename CBTreeSet_t::const_iterator sCIterHint, const typename CBTreeSet_t::value_type &rData)
{
	bool				bFallBack = true;
	node_iter_type		nNode;
	sub_node_iter_type	nSubPos;

	if (sCIterHint.is_evaluated ())
	{
		iterator_state_t	*psIterState = (iterator_state_t *) sCIterHint.get_iterator_state ();

		if (this->is_leaf (psIterState->nNode))
		{
			position_t			sPos;

			sPos.nInstance = ~0;
			sPos.pKey = this->extract_key (this->m_pAddToNodeKey, rData);

			nSubPos = this->find_next_sub_pos (psIterState->nNode, sPos);

			if ((nSubPos != 0) && (nSubPos != this->get_data_count (psIterState->nNode)))
			{
				size_type		nDiff = size_type (nSubPos - psIterState->nSubPos);

				::std::advance (sCIterHint, nDiff);

				bFallBack = this->insert_via_iterator (sCIterHint, nNode, nSubPos);
			}
		}
	}

	if (bFallBack)
	{
		return (this->insert (rData));
	}
	else
	{
		value_type	*psData = this->get_data (nNode, nSubPos);

		*psData = ::std::move (rData);

		return (iterator (sCIterHint));
	}
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

template<class _t_keytype, class _t_datalayerproperties>
void CBTreeSet<_t_keytype, _t_datalayerproperties>::_swap (typename CBTreeSet<_t_keytype, _t_datalayerproperties>::CBTreeSet_t &rContainer)
{
	CBTreeAssociativeBase_t		&rSTLbaseContainerContainer = dynamic_cast <CBTreeAssociativeBase_t &> (rContainer);
	
	CBTreeAssociativeBase_t::_swap (rSTLbaseContainerContainer);
}

#endif // BTREESET_CPP
