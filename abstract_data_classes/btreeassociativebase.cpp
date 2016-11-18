/************************************************************
**
** file:	btreeassociativebase.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** STL base class, containing a common sub-set of methods
** shared between all STL interchangeable container classes.
**
************************************************************/

#ifndef BTREEASSOCAITIVEBASE_CPP
#define	BTREEASSOCAITIVEBASE_CPP

#include "abstract_data_classes/btreeassociativebase.h"

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::CBTreeAssociativeBase
	(_t_datalayerproperties &rDataLayerProperties, typename _t_datalayerproperties::sub_node_iter_type nNodeSize)
	:	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>
		(
			rDataLayerProperties, 
			nNodeSize
		)
{
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::CBTreeAssociativeBase
	(const CBTreeAssociativeBase_t &rBT, bool bAssign)
	:	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>
	(
		dynamic_cast<const CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties> &> (rBT), 
		bAssign
	)
{
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::~CBTreeAssociativeBase ()
{
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties> &
	CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::operator=
		(const CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties> &rBT)
{
	// if this is not the same instance as the referenced b-tree (rBT) ...
	if (this != &rBT)
	{
		CBTreeAssociative_t			&rAssociativeThis = dynamic_cast<CBTreeAssociative_t &> (*this);
		const CBTreeAssociative_t	&rAssociativerBT = dynamic_cast<const CBTreeAssociative_t &> (rBT);

		rAssociativeThis = rAssociativerBT;
	}

	return (*this);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename _t_datalayerproperties::size_type CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::max_size () const
{
	size_type		nRetval = (size_type) -1;

	nRetval &= (size_type) ~0x1;
	
	return (nRetval);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::lower_bound (const _t_key &rKey)
{
	return (CBTreeAssociative_t::lower_bound (rKey));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::const_iterator
	CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::lower_bound (const _t_key &rKey) const
{
	return (CBTreeAssociative_t::lower_bound (rKey));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::upper_bound (const _t_key &rKey)
{
	return (CBTreeAssociative_t::upper_bound (rKey));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::const_iterator
	CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::upper_bound (const _t_key &rKey) const
{
	return (CBTreeAssociative_t::upper_bound (rKey));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
int CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::comp (const _t_key &rKey0, const _t_key &rKey1) const
{
	typename ::std::is_arithmetic<_t_key>::type		sCompareSelect;

	return (CBTreeAssociativeBase_t::comp (rKey0, rKey1, sCompareSelect));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
int CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::comp (const _t_key &rKey0, const _t_key &rKey1, ::std::true_type)
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

template<class _t_data, class _t_key, class _t_datalayerproperties>
int CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::comp (const _t_key &rKey0, const _t_key &rKey1, ::std::false_type)
{
	return (::memcmp ((void *) &rKey0, (void *) &rKey1, sizeof (_t_key)));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::_swap (CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties> &rContainer)
{
	CBTreeAssociative_t		&rAssociativeContainer = dynamic_cast<CBTreeAssociative_t &> (rContainer);

	CBTreeAssociative_t::_swap (rAssociativeContainer);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
template<class _t_iterator>
void CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::_insert_unique (_t_iterator sItFirst, _t_iterator sItLast)
{
	_t_iterator		sIt;
	bool			bSelfReverse;
	bool			bSelfReference = CBTreeAssociativeBase_t::test_self_reference_of_iterator_to_this (sItFirst, sItLast, bSelfReverse, NULL, NULL);

	if (!bSelfReference)
	{
		for (sIt = sItFirst; sIt != sItLast; sIt++)
		{
			CBTreeAssociativeBase_t::_insert_unique ((value_type) (*sIt));
		}
	}
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::_insert_unique (const typename CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::value_type &rData)
{
	iterator			sIter;
	key_type			sKey;
	key_type			*psKey = this->extract_key (&sKey, rData);

	size_type			nPresent = CBTreeAssociativeBase_t::count (*psKey);

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

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::_insert_unique (typename CBTreeAssociativeBase_t::const_iterator sCIterHint, const typename CBTreeAssociativeBase_t::value_type &rData)
{
	bool				bFallBack = true;
	node_iter_type		nNode;
	sub_node_iter_type	nSubPos;
	size_type			nPresent = 0;

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
				node_iter_type		nDummyNode;
				sub_node_iter_type	nDummySubPos;

				if (this->find_oneKey (psIterState->nNode, *sPos.pKey, nDummyNode, nDummySubPos))
				{
					return (this->end ());
				}

				size_type		nDiff = size_type (nSubPos - psIterState->nSubPos);

				::std::advance (sCIterHint, nDiff);

				bFallBack = this->insert_via_iterator (sCIterHint, nNode, nSubPos);
			}
		}
	}

	if (bFallBack)
	{
		return (this->_insert_unique (rData));
	}
	else
	{
		value_type	*psData = this->get_data (nNode, nSubPos);

		*psData = ::std::move (rData);

		iterator_state_t	sIterState;

		sIterState.nAssociatedPos = sCIterHint.get_pos ();
		sIterState.nNode = nNode;
		sIterState.nSubPos = nSubPos;

		return (iterator (this, sIterState.nAssociatedPos, (void *) &sIterState, this->get_time_stamp ()));
	}
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
template<class ..._t_va_args>
typename CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::_emplace_unique (_t_va_args && ... rrArgs)
{
	size_type					nRetval;
	iterator_state_t			sIterState;
	position_t					sPos;
	value_type					*psInput = new value_type (::std::forward<_t_va_args> (rrArgs) ...);
	
	this->create_root ();

	// convert object to a position structure for internal use
	//
	// nInstance is left to any (~0), since it will be ignored but 
	// has to be set to not turn up on any valgrind report and this
	// object will be appended to any set of entries that share
	// the same key
	sPos.nInstance = ~0;
	sPos.pKey = this->extract_key (this->m_pAddToNodeKey, *psInput);

	if (CBTreeAssociativeBase_t::count (*sPos.pKey) != size_type (0))
	{
		delete psInput;

		return (this->end ());
	}

	sIterState.nAssociatedPos = 0;

	// allocate space for one more entry
	nRetval = this->add_to_node (sPos, this->m_nRootNode, 0, sIterState.nNode, sIterState.nSubPos, &sIterState.nAssociatedPos);

	BTREE_ASSERT (nRetval == 1, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::emplace (_t_va_args && ...): Failed to create new entry!");

	// insert data by moving it in place
	value_type	*pData = this->get_data (sIterState.nNode, sIterState.nSubPos);

	*pData = ::std::move (*psInput);

	delete psInput;

	// create iterator pointing at location of new entry to be returned
	iterator		sRetval (this, sIterState.nAssociatedPos, &sIterState, this->get_time_stamp (), false);

	return (sRetval);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
template<class ..._t_va_args>
typename CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::_emplace_hint_unique (typename CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::const_iterator sCIterHint, _t_va_args && ... rrArgs)
{
	iterator_state_t			sIterState;
	position_t					sPos;
	value_type					*psInput = new value_type (::std::forward<_t_va_args> (rrArgs) ...);
	bool						bFallBack = true;
	sub_node_iter_type			nSubPos;
	
	this->create_root ();

	// convert object to a position structure for internal use
	//
	// nInstance is left to any (~0), since it will be ignored but 
	// has to be set to not turn up on any valgrind report and this
	// object will be appended to any set of entries that share
	// the same key
	sPos.nInstance = ~0;
	sPos.pKey = this->extract_key (this->m_pAddToNodeKey, *psInput);

	// if hint iterator is referring to a valid location ...
	if (sCIterHint.is_evaluated ())
	{
		iterator_state_t		*psIterState = (iterator_state_t *) sCIterHint.get_iterator_state ();

		// ... and the operation is going to take place on a leaf node ...
		if (this->is_leaf (psIterState->nNode))
		{
			nSubPos = this->find_next_sub_pos (psIterState->nNode, sPos);

			// ... and it was possible to confirm the hint was correct by making
			// sure the operation will take place within the node in question
			if ((nSubPos != 0) && (nSubPos != this->get_data_count (psIterState->nNode)))
			{
				node_iter_type		nDummyNode;
				sub_node_iter_type	nDummySubPos;

				if (this->find_oneKey (psIterState->nNode, *sPos.pKey, nDummyNode, nDummySubPos))
				{
					delete psInput;

					return (this->end ());
				}

				size_type		nDiff = size_type (nSubPos - psIterState->nSubPos);

				::std::advance (sCIterHint, nDiff);

				// ... then attempt to accelerate the allocation operation
				bFallBack = this->insert_via_iterator (sCIterHint, sIterState.nNode, sIterState.nSubPos);

				sIterState.nAssociatedPos = psIterState->nAssociatedPos;
			}
		}
	}

	// if the accelerated allocation failed...
	if (bFallBack)
	{
		return (this->_emplace_unique (::std::forward<_t_va_args> (rrArgs) ...));
	}
	else
	{
		// insert data by moving it in place
		value_type	*pData = this->get_data (sIterState.nNode, sIterState.nSubPos);

		*pData = ::std::move (*psInput);

		delete psInput;

		// create iterator pointing at location of new entry to be returned
		iterator		sRetval (this, sIterState.nAssociatedPos, &sIterState, this->get_time_stamp (), false);

		return (sRetval);
	}
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
template<class _t_iterator>
typename ::std::pair<_t_iterator, _t_iterator>
	CBTreeAssociativeBase<_t_data, _t_key, _t_datalayerproperties>::_equal_range_unique (const key_type &rKey) const
{
	typedef typename ::std::pair<_t_iterator, _t_iterator>		local_equal_range_type;

	iterator_state_t		sIterState;

	if (this->_find (rKey, sIterState.nNode, sIterState.nSubPos, sIterState.nAssociatedPos))
	{
		_t_iterator		sIterLower (this, sIterState.nAssociatedPos, &sIterState, this->get_time_stamp (), false);
		_t_iterator		sIterUpper (sIterLower);

		::std::advance (sIterUpper, 1);

		local_equal_range_type	sRetval (sIterLower, sIterUpper);

		return (sRetval);
	}
	else
	{
		_t_iterator		sIter (this, sIterState.nAssociatedPos, &sIterState, this->get_time_stamp (), false);
		
		local_equal_range_type	sRetval (sIter, sIter);

		return (sRetval);
	}
}

#endif // BTREEASSOCAITIVEBASE_CPP
