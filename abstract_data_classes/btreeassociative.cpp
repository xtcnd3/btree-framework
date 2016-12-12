/************************************************************
**
** file:	btreeassociative.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** abstract associative container interface class.
**
************************************************************/

#ifndef	BTREEASSOCIATIVE_CPP
#define	BTREEASSOCIATIVE_CPP

#include "abstract_data_classes/btreeassociative.h"

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::CBTreeAssociative
	(
		const _t_datalayerproperties &rDataLayerProperties, 
		const typename _t_datalayerproperties::sub_node_iter_type nNodeSize
	)
	:	CBTreeBaseDefaults<typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::position_t, _t_data, _t_datalayerproperties>
	(
		rDataLayerProperties, 
		nNodeSize
	)
{
#if defined (_DEBUG)
	m_pbShortLiveKeyInUse = new bool (false);

	BTREE_ASSERT (m_pbShortLiveKeyInUse != NULL, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::CBTreeAssociative (_t_datalayerproperties &, sub_node_iter_type): insufficient memory!");
#endif

	m_pRemoveKey = NULL;
	m_pTempRemoveKey = NULL;
	m_pInstancesNewKey = NULL;
	m_pTempFindFirstKeyNewKey = NULL;
	m_pAddToNodeKey = NULL;
	m_pGetNewKey = NULL;

	m_ppShortLiveKey = new _t_key *;

	BTREE_ASSERT (m_ppShortLiveKey != NULL, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::CBTreeAssociative (_t_datalayerproperties &, sub_node_iter_type) (m_ppShortLiveKey): insufficient memory!");

	*m_ppShortLiveKey = NULL;

	m_ppTempFindFirstKeyKey = new _t_key *;

	BTREE_ASSERT (m_ppTempFindFirstKeyKey != NULL, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::CBTreeAssociative (_t_datalayerproperties &, sub_node_iter_type) (m_ppTempFindFirstKeyKey): insufficient memory!");

	*m_ppTempFindFirstKeyKey = NULL;
	
	// allocate all variable size temporary buffers
	vbufferAllocate (m_ppShortLiveKey);
	vbufferAllocate (m_ppTempFindFirstKeyKey);

	vbufferAllocate (&m_pRemoveKey);
	vbufferAllocate (&m_pTempRemoveKey);
	vbufferAllocate (&m_pInstancesNewKey);
	vbufferAllocate (&m_pTempFindFirstKeyNewKey);
	vbufferAllocate (&m_pAddToNodeKey);
	vbufferAllocate (&m_pGetNewKey);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::CBTreeAssociative (const CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties> &rContainer, const bool bAssign)
	:	CBTreeBaseDefaults<typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::position_t, _t_data, _t_datalayerproperties>
	(
		dynamic_cast <const CBTreeBaseDefaults<typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::position_t, _t_data, _t_datalayerproperties> &> (rContainer)
	)
#if defined (_DEBUG)
	,	m_pbShortLiveKeyInUse (NULL)
#endif
	,	m_pRemoveKey (NULL)
	,	m_pTempRemoveKey (NULL)
	,	m_pInstancesNewKey (NULL)
	,	m_pTempFindFirstKeyNewKey (NULL)
	,	m_pAddToNodeKey (NULL)
	,	m_pGetNewKey (NULL)
	,	m_ppShortLiveKey (NULL)
	,	m_ppTempFindFirstKeyKey (NULL)
{
#if defined (_DEBUG)

	m_pbShortLiveKeyInUse = new bool (false);

	BTREE_ASSERT (m_pbShortLiveKeyInUse != NULL, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::CBTreeAssociative (CBTreeAssociative<> &): insufficient memory!");

#endif

	m_ppShortLiveKey = new _t_key *;

	BTREE_ASSERT (m_ppShortLiveKey != NULL, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::CBTreeAssociative (CBTreeAssociative<> &) (m_ppShortLiveKey): insufficient memory!");

	*m_ppShortLiveKey = NULL;
	
	m_ppTempFindFirstKeyKey = new _t_key *;

	BTREE_ASSERT (m_ppTempFindFirstKeyKey != NULL, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::CBTreeAssociative (CBTreeAssociative<> &) (m_ppTempFindFirstKeyKey): insufficient memory!");

	*m_ppTempFindFirstKeyKey = NULL;
	
	// allocate all variable size temporary buffers
	vbufferAllocate (m_ppShortLiveKey);
	vbufferAllocate (m_ppTempFindFirstKeyKey);
	
	vbufferAllocate (&m_pRemoveKey);
	vbufferAllocate (&m_pTempRemoveKey);
	vbufferAllocate (&m_pInstancesNewKey);
	vbufferAllocate (&m_pTempFindFirstKeyNewKey);
	vbufferAllocate (&m_pAddToNodeKey);
	vbufferAllocate (&m_pGetNewKey);

	if (bAssign)
	{
		this->_assign (rContainer);
	}
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::CBTreeAssociative (CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties> &&rRhsContainer)
	:	CBTreeBaseDefaults<typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::position_t, _t_data, _t_datalayerproperties>
	(
		dynamic_cast <CBTreeBaseDefaults<typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::position_t, _t_data, _t_datalayerproperties> &&> (rRhsContainer)
	)
#if defined (_DEBUG)
	,	m_pbShortLiveKeyInUse (NULL)
#endif
	,	m_pRemoveKey (NULL)
	,	m_pTempRemoveKey (NULL)
	,	m_pInstancesNewKey (NULL)
	,	m_pTempFindFirstKeyNewKey (NULL)
	,	m_pAddToNodeKey (NULL)
	,	m_pGetNewKey (NULL)
	,	m_ppShortLiveKey (NULL)
	,	m_ppTempFindFirstKeyKey (NULL)
{
#if defined (_DEBUG)

	fast_swap (this->m_pbShortLiveKeyInUse, rRhsContainer.m_pbShortLiveKeyInUse);

#endif

	fast_swap (this->m_pRemoveKey, rRhsContainer.m_pRemoveKey);
	fast_swap (this->m_pTempRemoveKey, rRhsContainer.m_pTempRemoveKey);
	fast_swap (this->m_pInstancesNewKey, rRhsContainer.m_pInstancesNewKey);
	fast_swap (this->m_pTempFindFirstKeyNewKey, rRhsContainer.m_pTempFindFirstKeyNewKey);
	fast_swap (this->m_pAddToNodeKey, rRhsContainer.m_pAddToNodeKey);
	fast_swap (this->m_pGetNewKey, rRhsContainer.m_pGetNewKey);
	fast_swap (this->m_ppShortLiveKey, rRhsContainer.m_ppShortLiveKey);
	fast_swap (this->m_ppTempFindFirstKeyKey, rRhsContainer.m_ppTempFindFirstKeyKey);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::~CBTreeAssociative ()
{
	vbufferDeallocate (m_ppShortLiveKey);
	vbufferDeallocate (m_ppTempFindFirstKeyKey);
	vbufferDeallocate (&m_pRemoveKey);
	vbufferDeallocate (&m_pTempRemoveKey);
	vbufferDeallocate (&m_pInstancesNewKey);
	vbufferDeallocate (&m_pTempFindFirstKeyNewKey);
	vbufferDeallocate (&m_pAddToNodeKey);
	vbufferDeallocate (&m_pGetNewKey);

	if (m_ppShortLiveKey != NULL)
	{
		delete m_ppShortLiveKey;
	}

	if (m_ppTempFindFirstKeyKey != NULL)
	{
		delete m_ppTempFindFirstKeyKey;
	}

#if defined (_DEBUG)

	if (m_pbShortLiveKeyInUse != NULL)
	{
		delete m_pbShortLiveKeyInUse;
	}

#endif
}

/*

insert - insert data

rData		- data set to be added

This method inserts one entry.

If the method was successful, then the return value is an iterator reflecting the position of
the newly added entry, otherwise the method asserts and throws an ::std::run_time_error expection.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::insert
	(
		const _t_data &rData
	)
{
	size_type					nRetval;
	iterator_state_t			sIterState;
	position_t					sPos;
	
	this->create_root ();

	// convert object to a position structure for internal use
	//
	// nInstance is left to any (~0), since it will be ignored but 
	// has to be set to not turn up on any valgrind report and this
	// object will be appended to any set of entries that share
	// the same key
	sPos.nInstance = ~0;
	sPos.pKey = extract_key (m_pAddToNodeKey, rData);

	sIterState.nAssociatedPos = 0;

	// allocate space for one more data entry
	nRetval = this->add_to_node (sPos, this->m_nRootNode, 0, sIterState.nNode, sIterState.nSubPos, &sIterState.nAssociatedPos);

	BTREE_ASSERT (nRetval == 1, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::insert (const _t_data &): Failed to create new entry!");

	// insert data by constructing it in place
	value_type	*pData = this->get_data (sIterState.nNode, sIterState.nSubPos);

	new (pData) value_type (rData);

	// create iterator pointing at location of new entry to be returned
	iterator		sRetval (this, sIterState.nAssociatedPos, &sIterState, this->get_time_stamp (), false);

	return (sRetval);
}

/*

insert - insert data

sCIterHint	- indicates the location where the new entry is going to be inserted
rData		- data set to be added

This method inserts one entry, using the input iterator as a hint as to where the new data entry
is going to be put.

If the method was successful, then the return value is an iterator reflecting the position of
the newly added entry, otherwise the method asserts and throws an ::std::run_time_error expection.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::insert
	(
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::const_iterator sCIterHint, 
		const typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::value_type &rData
	)
{
	iterator_state_t			sIterState;
	iterator_state_t			*psIterState;
	position_t					sPos;
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
	sPos.pKey = extract_key (m_pAddToNodeKey, rData);

	// if hint iterator is referring to a valid location ...
	if (sCIterHint.is_evaluated ())
	{
		psIterState = (iterator_state_t *) sCIterHint.get_iterator_state ();

		// ... and the operation is going to take place on a leaf node ...
		if (this->is_leaf (psIterState->nNode))
		{
			nSubPos = this->find_next_sub_pos (psIterState->nNode, sPos);

			// ... and it was possible to confirm the hint was correct by making
			// sure the operation will take place within the node in question
			if ((nSubPos != 0) && (nSubPos != this->get_data_count (psIterState->nNode)))
			{
				// ... then attempt to accelerate the allocation operation
				size_type		nDiff = size_type (nSubPos - psIterState->nSubPos);

				::std::advance (sCIterHint, nDiff);

				bFallBack = this->insert_via_iterator (sCIterHint, sIterState.nNode, sIterState.nSubPos);
			}
		}
	}

	// if the accelerated allocation failed...
	if (bFallBack)
	{
		// ... then use the regular allocation
		sIterState.nAssociatedPos = 0;

		size_type	nRetval = this->add_to_node (sPos, this->m_nRootNode, 0, sIterState.nNode, sIterState.nSubPos, &sIterState.nAssociatedPos);

		BTREE_ASSERT (nRetval == 1, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::emplace_hint (const_iterator, _t_va_args && ...): Failed to create new entry!");
	}
	else
	{
		// otherwise update the external pointer of the iterator to be returned
		sIterState.nAssociatedPos = sCIterHint.get_pos ();
	}

	// insert data by constructing it in place
	value_type	*pData = this->get_data (sIterState.nNode, sIterState.nSubPos);

	*pData = rData;

	// create iterator pointing at location of new entry to be returned
	iterator		sRetval (this, sIterState.nAssociatedPos, &sIterState, this->get_time_stamp (), false);

	return (sRetval);
}

/*

insert - insert data

sItFirst	- specifies reference to first entry to be transfered
sItLast		- specifies reference to one entry past the final entry to be transfered

This method will insert one or more data items to the target container, according to
the input iterators. In case sItFirst equals sItLast, the call is ineffective neither
the stored data nor the state of the target container is being altered.
The method iterates element by element from sItFirst onwards until sItLast has been
found. In case sItLast is not being found and the container associated with sItFirst
goes beyond its last element, then an ::std::out_of_range exception is thrown.

If the method was not successful for any other reason, then ::std::run_time_error
expection is thrown.

Note: The method is capable to self-reference, which means if the input iterators are
associated with the target container, then the method copies the content specified by the
range to a temporary container and then inserts the data from that temporary container
into the target container.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::insert
	(
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::iterator sItFirst, 
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::iterator sItLast
	)
{
	this->template insert <iterator> (sItFirst, sItLast);
}

/*

insert - insert data

sItFirst	- specifies reference to first entry to be transfered
sItLast		- specifies reference to one entry past the final entry to be transfered

This method will insert one or more data items to the target container, according to
the input iterators. In case sItFirst equals sItLast, the call is ineffective neither
the stored data nor the state of the target container is being altered.
The method iterates element by element from sItFirst onwards until sItLast has been
found. In case sItLast is not being found and the container associated with sItFirst
goes beyond its last element, then an ::std::out_of_range exception is thrown.

If the method was not successful for any other reason, then ::std::run_time_error
expection is thrown.

Note: The method is capable to self-reference, which means if the input iterators are
associated with the target container, then the method copies the content specified by the
range to a temporary container and then inserts the data from that temporary container
into the target container.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::insert
	(
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::const_iterator sItFirst, 
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::const_iterator sItLast
	)
{
	this->template insert <const_iterator> (sItFirst, sItLast);
}

/*

insert - insert data

sItFirst	- specifies reference to first entry to be transfered
sItLast		- specifies reference to one entry past the final entry to be transfered

This method will insert one or more data items to the target container, according to
the input iterators. In case sItFirst equals sItLast, the call is ineffective neither
the stored data nor the state of the target container is being altered.
The method iterates element by element from sItFirst onwards until sItLast has been
found. In case sItLast is not being found and the container associated with sItFirst
goes beyond its last element, then an ::std::out_of_range exception is thrown.

If the method was not successful for any other reason, then ::std::run_time_error
expection is thrown.

Note: The method is capable to self-reference, which means if the input iterators are
associated with the target container, then the method copies the content specified by the
range to a temporary container and then inserts the data from that temporary container
into the target container.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::insert
	(
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::reverse_iterator sItFirst, 
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::reverse_iterator sItLast
	)
{
	this->template insert <reverse_iterator> (sItFirst, sItLast);
}

/*

insert - insert data

sItFirst	- specifies reference to first entry to be transfered
sItLast		- specifies reference to one entry past the final entry to be transfered

This method will insert one or more data items to the target container, according to
the input iterators. In case sItFirst equals sItLast, the call is ineffective neither
the stored data nor the state of the target container is being altered.
The method iterates element by element from sItFirst onwards until sItLast has been
found. In case sItLast is not being found and the container associated with sItFirst
goes beyond its last element, then an ::std::out_of_range exception is thrown.

If the method was not successful for any other reason, then ::std::run_time_error
expection is thrown.

Note: The method is capable to self-reference, which means if the input iterators are
associated with the target container, then the method copies the content specified by the
range to a temporary container and then inserts the data from that temporary container
into the target container.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::insert
	(
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::const_reverse_iterator sItFirst, 
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::const_reverse_iterator sItLast
	)
{
	this->template insert <const_reverse_iterator> (sItFirst, sItLast);
}

/*

insert - insert data

sItFirst	- specifies reference to first entry to be transfered
sItLast		- specifies reference to one entry past the final entry to be transfered

This method will insert one or more data items to the target container, according to
the input iterators. In case sItFirst equals sItLast, the call is ineffective neither
the stored data nor the state of the target container is being altered.
The method iterates element by element from sItFirst onwards until sItLast has been
found. In case sItLast is not being found and the container associated with sItFirst
goes beyond its last element, then an ::std::out_of_range exception is thrown.

If the method was not successful for any other reason, then ::std::run_time_error
expection is thrown.

Note: The method is capable to self-reference, which means if the input iterators are
associated with the target container, then the method copies the content specified by the
range to a temporary container and then inserts the data from that temporary container
into the target container.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
template<class _t_iterator>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	bool			bSelfReverse;
	_t_iterator		sIt;
	const_iterator	*psItFirst;
	const_iterator	*psItLast;

	if (sItFirst == sItLast)
	{
		return;
	}

	// if the input iterators are referring to this container ...
	if (this->test_self_reference_of_iterator_to_this (sItFirst, sItLast, bSelfReverse, &psItFirst, &psItLast))
	{
		// ... then switch to self reference mode

		// by, in reverse order, copying data block by block from this container to a temporary container, to
		// then insert those blocks again 
		const_iterator						sIt = (const_iterator) (*psItLast);
		const_iterator						sItLower;
		const_iterator						sItUpper;
		value_type							sData;
		key_type							sKey;
		key_type							*psKey;
		key_type							sFirstKey;
		key_type							sLastKey;
		key_type							*psFirstKey;
		key_type							*psLastKey;
		typename ::std::list<_t_data>		sList;

		// determine key of initial block
		sData = *((const_iterator) (*psItFirst));

		psFirstKey = this->extract_key (&sFirstKey, sData);

		// determine key of final block
		(*psItLast)--;

		sData = *((const_iterator) (*psItLast));

		psLastKey = this->extract_key (&sLastKey, sData);

		(*psItLast)++;

		// in reverse order ...
		while (sIt != ((const_iterator) (*psItFirst)))
		{
			sIt--;

			// determine the key value of the current block
			sData = *sIt;

			psKey = this->extract_key (&sKey, sData);

			// deteremine the range of the current block
			sItLower = this->lower_bound (sKey);
			sItUpper = this->upper_bound (sKey);

			// if it is the initial block ...
			if (this->comp (*psKey, *psFirstKey) == 0)
			{
				// ... then use the first position given by the transfer parameter instead of the determined block
				sItLower = ((const_iterator) (*psItFirst));
			}

			// if it is the final block ...
			if (this->comp (*psKey, *psLastKey) == 0)
			{
				// ... then use the last position given by the transfer parameter instead of the determined block
				sItUpper = ((const_iterator) (*psItLast));
			}

			// clear temporary buffer
			sList.clear ();

			// insert block into temporary buffer
			sList.insert (sList.begin (), sItLower, sItUpper);

			// re-insert data into self referenced container
			CBTreeAssociative_t::insert (sList.cbegin (), sList.cend ());

			// if the end has been reached prematurely ...
			if (sItLower == ((const_iterator) (*psItFirst)))
			{
				// ... then end transfer
				break;
			}

			// go to next block
			sIt = sItLower;
		}

		delete (psItFirst);
		delete (psItLast);
	}
	else
	{
		// otherwise transfer data one by one
#if defined (_DEBUG)

		uint32_t	nDebug = 0;

#endif

		for (sIt = sItFirst; sIt != sItLast; sIt++)
		{
#if defined (_DEBUG)

			size_type		nSize = this->size ();

#endif

			CBTreeAssociative_t::insert ((const value_type &) *sIt);

#if defined (_DEBUG)

			nDebug++;

			BTREE_ASSERT ((nSize + 1) == this->size (), "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::insert<_t_iterator>: unexpected container size!");

#endif
		}
	}
}

/*

emplace - insert data by constructing it in place

rData		- specifies the data to be insered as a move constructor argument

The method constructs a new entry in place based on the move constructor
argument, which increases the container size by one.

If the method is successful, then an iterator referring to the newly inserted
data entry is returned. Otherwise, an ::std::run_time_error exception is thrown.

Note: If value_type doesn't provide a move constructor, then the copy constructor
is invoked.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::emplace
	(
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::value_type &&rData
	)
{
	// method provides the actual code for the virtual emplace (value_type &&) of CBTreeAssociativeIf
	return (this->template emplace<value_type &&> (::std::forward<value_type> (rData)));
}

/*

emplace_hint - insert data by constructing it in place

sCIterHint	- indicates the location where the new data is going to be inserted
rData		- specifies the data to be insered as a move constructor argument

The method constructs a new entry in place based on the move constructor
argument, which increases the container size by one, while trying to use sCIterHint
as an indicator where that insertion has to take place, for an accelerated operation.
If the position displayed by sCIterHint is insufficient to be used, then the
method ignores sCIterHint and falls back to its emplace () equivalent.

If the method is successful, then an iterator referring to the newly inserted
data entry is returned. Otherwise, an ::std::run_time_error exception is thrown.

Note: If value_type doesn't provide a move constructor, then the copy constructor
is invoked.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::emplace_hint
	(
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::const_iterator sCIterHint, 
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::value_type &&rData
	)
{
	// method provides the actual code for the virtual emplace_hint (const_iterator, value_type &&) of CBTreeAssociativeIf
	return (this->template emplace_hint<value_type &&> (sCIterHint, ::std::forward<value_type> (rData)));
}

/*

emplace - insert data by constructing it in place

rData		- specifies the constructor arguments for a new data entry

The method constructs a new entry in place based on the constructor
arguments, which increases the container size by one.

If the method is successful, then an iterator referring to the newly inserted
data entry is returned. Otherwise, an ::std::run_time_error exception is thrown.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
template<class ..._t_va_args>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::emplace (_t_va_args && ... rrArgs)
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
	sPos.pKey = extract_key (m_pAddToNodeKey, *psInput);

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

/*

emplace_hint - insert data by constructing it in place

sCIterHint	- indicates the location where the new data is going to be inserted
rData		- specifies the constructor arguments for a new data entry

The method constructs a new entry in place based on the constructor
arguments, which increases the container size by one, while trying to use sCIterHint
as an indicator where that insertion has to take place, for an accelerated operation.
If the position displayed by sCIterHint is insufficient to be used, then the
method ignores sCIterHint and falls back to its emplace () equivalent.

If the method is successful, then an iterator referring to the newly inserted
data entry is returned. Otherwise, an ::std::run_time_error exception is thrown.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
template<class ..._t_va_args>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::emplace_hint
	(
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::const_iterator sCIterHint, 
		_t_va_args && ... rrArgs
	)
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
	sPos.pKey = extract_key (m_pAddToNodeKey, *psInput);

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
		// ... then use the regular allocation
		sIterState.nAssociatedPos = 0;

		size_type	nRetval = this->add_to_node (sPos, this->m_nRootNode, 0, sIterState.nNode, sIterState.nSubPos, &sIterState.nAssociatedPos);

		BTREE_ASSERT (nRetval == 1, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::emplace_hint (const_iterator, _t_va_args && ...): Failed to create new entry!");
	}

	// insert data by moving it in place
	value_type	*pData = this->get_data (sIterState.nNode, sIterState.nSubPos);

	*pData = ::std::move (*psInput);

	delete psInput;

	// create iterator pointing at location of new entry to be returned
	iterator		sRetval (this, sIterState.nAssociatedPos, &sIterState, this->get_time_stamp (), false);

	return (sRetval);
}

/*

erase - removes one entry from the container

sCIterPos		- specifies iterator referring to an entry to be removed

This method removes the data entry assoctiated with the location sCIterPos
is referring to from the container. This means, the container size is reduced
by one.

If successful the method returns an iterator associated with the location
of the removed entry. Otherwise, an ::std::run_time_error exception is thrown.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::erase
	(
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::const_iterator sCIterPos
	)
{
	typedef typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::iterator		iter_t;

	this->erase_via_reference (sCIterPos, false);

	return (iter_t (this, sCIterPos.get_pos (), false));
}

/*

erase - removes all entries using the same key

key		- specifies the key which is extracted from the parameter

Every entry using the same as the key extracted from the parameter is removed.

The method returns number of removed entries.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename _t_datalayerproperties::size_type CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::erase (const _t_key &rKey)
{
	position_t			sPos;
	size_type			nRetval = (size_type) 0;

	if (!this->empty ())
	{
		// convert object to a position structure for internal use
		sPos.pKey = m_pRemoveKey;
		*m_pRemoveKey = rKey;

		// nInstance is left to any (~0), since any entry using the same
		// key will be removed
		sPos.nInstance = ~0;

		// for as long as any instnce using the same key is left ...
		while (count (rKey) > (size_type) 0)
		{
			// ... continue removing those entries
			nRetval += this->remove_from_node (sPos, this->m_nRootNode, 0);
		}
	}

	return (nRetval);
}

/*

erase - removes a range of entries from the container

sCIterFirst		- specifies iterator referring to the initial entry to be removed
sCIterLast		- specifies iterator referring one past to the final entry to be removed

This method removes all data entries between the location referred to by sCIterFirst and 
before sCIterLast. If sCIterFirst and sCIterLast refer to the same location, then the call
is ineffective and neither is an entry removed nor the state of the container changed.
In case sCIterLast refers to a location before sCIterFirst, then any data past sCIterFirst
is removed and an ::std::run_time_error exception is thrown.

If successful the method returns an iterator associated with the initial location
of the removed range. Otherwise, an ::std::run_time_error exception is thrown.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::erase
	(
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::const_iterator sCIterFirst, 
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::const_iterator sCIterLast
	)
{
	
	difference_type		nDiff = ::std::distance (sCIterFirst, sCIterLast);
	difference_type		i;

	// remove nDiff entries one by one always from the first position
	for (i = 0; i < nDiff; i++)
	{
		// which allows to use the accelarated remove method, since the iterator in use is almost always valid
		this->erase_via_reference (sCIterFirst);
	}

	return (iterator (this, sCIterFirst.get_pos (), false));
}

/*

find - finds any entry containing a specified key

rKey		- specifies key value of any element to be found

This method finds an entry which has a key value equivalent to the input key
value 'rKey', so any exists. If there is more than one entry containing
equivalent keys, then any of those is returned. If an entry with a matching
key value is not present, then an iterator equivalent to this->end () is
returned.

If successful the method returns an iterator associated with any entry
containing a key value same as the input key value. Otherwise, an iterator
past the last entry (value equivalent to this->end ()) is returned.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::iterator
CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::find
	(
		const _t_key &rKey
	)
{
	iterator_state_t		sIterState;
	
	// if key has been found ...
	if (CBTreeAssociative::_find (rKey, sIterState.nNode, sIterState.nSubPos, sIterState.nAssociatedPos))
	{
		// ... then return iterator referring to initial location
		return (iterator (this, sIterState.nAssociatedPos, (void *) &sIterState, this->get_time_stamp (), false));
	}
	else
	{
		// otherwise return state of failure
		return (this->end ());
	}
}

/*

find - finds any entry containing a specified key

rKey		- specifies key value of any element to be found

This method finds an entry which has a key value equivalent to the input key
value 'rKey', so any exists. If there is more than one entry containing
equivalent keys, then any of those is returned. If an entry with a matching
key value is not present, then a const_iterator equivalent to this->cend () is
returned.

If successful the method returns an const_iterator associated with any entry
containing a key value same as the input key value. Otherwise, a const_iterator
past the last entry (value equivalent to this->cend ()) is returned.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::const_iterator
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::find
	(
		const _t_key &rKey
	) const
{
	iterator_state_t		sIterState;
	
	// if key has been found ...
	if (CBTreeAssociative::_find (rKey, sIterState.nNode, sIterState.nSubPos, sIterState.nAssociatedPos))
	{
		// ... then return iterator referring to initial location
		return (const_iterator (this, sIterState.nAssociatedPos, (void *) &sIterState, this->get_time_stamp (), false));
	}
	else
	{
		// otherwise return state of failure
		return (this->cend ());
	}
}

/*

lower_bound - finds initial entry containing a specified key

rKey		- specifies key value of initial element to be found

This method finds the first entry which has a key value equivalent to the input key
value 'rKey', so any exists. If an entry with a matching key value is not 
present, then an iterator referring to an entry containing a key value
deemed to be the next greater key value is returned.

If successful the method returns an iterator associated with the initial entry
containing a key value same as the input key value. Otherwise, a iterator pointing
to an entry containing a key value what is deemed to be the next greater key value is
returned. If such an entry doesn't exist, then an iterator past the last entry
(value equivalent to this->end ()) is returned.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::lower_bound
	(
		const _t_key &rKey
	)
{
	iterator_state_t		sIterState;
	
	CBTreeAssociative_t::_lower_bound (rKey, sIterState.nNode, sIterState.nSubPos, sIterState.nAssociatedPos);
	
	return (iterator (this, sIterState.nAssociatedPos, (void *) &sIterState, this->get_time_stamp (), false));
}

/*

lower_bound - finds initial entry containing a specified key

rKey		- specifies key value of initial element to be found

This method finds the first entry which has a key value equivalent to the input key
value 'rKey', so any exists. If an entry with a matching key value is not 
present, then a const_iterator referring to an entry containing a key value
deemed to be the next greater key value is returned.

If successful the method returns a const_iterator associated with the initial entry
containing a key value same as the input key value. Otherwise, a const_iterator pointing
to an entry containing a key value what is deemed to be the next greater key value is
returned. If such an entry doesn't exist, then a const_iterator past the last entry
(value equivalent to this->cend ()) is returned.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::const_iterator
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::lower_bound
	(
		const _t_key &rKey
	) const
{
	iterator_state_t		sIterState;
	
	CBTreeAssociative_t::_lower_bound (rKey, sIterState.nNode, sIterState.nSubPos, sIterState.nAssociatedPos);
	
	return (const_iterator (this, sIterState.nAssociatedPos, (void *) &sIterState, this->get_time_stamp (), false));
}

/*

upper_bound - finds initial entry containing the next greater key

rKey		- specifies the next lesser key value of initial element to be found

This method finds the first entry which has a key value equivalent to the next greater
input key value 'rKey', so any exists. If an entry with a matching key value is not 
present, then an iterator referring to an entry containing a key value
deemed to be the nearest greater key value is returned.

If successful the method returns an iterator associated with the initial entry
containing the next greater key value than the input key value. Otherwise, an
iterator pointing to an entry containing a key value what is deemed to be the
nearest greater key value is returned. If such an entry doesn't exist, then an
iterator past the last entry (value equivalent to this->end ()) is returned.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::upper_bound
	(
		const _t_key &rKey
	)
{
	iterator_state_t		sIterState;
	
	// if key in question is the last key ...
	if (CBTreeAssociative_t::_upper_bound (rKey, sIterState.nNode, sIterState.nSubPos, sIterState.nAssociatedPos))
	{
		// ... then return iterator equivalent to end ()
		return (iterator (this, sIterState.nAssociatedPos, false));
	}
	else
	{
		// otherwise generate iterator referring to next key
		return (iterator (this, sIterState.nAssociatedPos, (void *) &sIterState, this->get_time_stamp (), false));
	}
}

/*

upper_bound - finds initial entry containing the next greater key

rKey		- specifies the next lesser key value of initial element to be found

This method finds the first entry which has a key value equivalent to the next greater
input key value 'rKey', so any exists. If an entry with a matching key value is not 
present, then a const_iterator referring to an entry containing a key value
deemed to be the nearest greater key value is returned.

If successful the method returns a const_iterator associated with the initial entry
containing the next greater key value than the input key value. Otherwise, a
const_iterator pointing to an entry containing a key value what is deemed to be the
nearest greater key value is returned. If such an entry doesn't exist, then a
const_iterator past the last entry (value equivalent to this->end ()) is returned.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::const_iterator
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::upper_bound
	(
		const _t_key &rKey
	) const
{
	iterator_state_t		sIterState;
	
	// if key in question is the last key ...
	if (CBTreeAssociative_t::_upper_bound (rKey, sIterState.nNode, sIterState.nSubPos, sIterState.nAssociatedPos))
	{
		// ... then return iterator equivalent to end ()
		return (const_iterator (this, sIterState.nAssociatedPos, false));
	}
	else
	{
		// otherwise generate iterator referring to next key
		return (const_iterator (this, sIterState.nAssociatedPos, (void *) &sIterState, this->get_time_stamp (), false));
	}
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::equal_range_type
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::equal_range
	(
		const key_type &rKey
	)
{
	return (this->template _equal_range <iterator> (rKey));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::equal_range_const_type
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::equal_range
	(
		const key_type &rKey
	) const
{
	return (this->template _equal_range <const_iterator> (rKey));
}

/*

get_next_key - get next key

rKey		- specifies key prior to key to be returned
rNextKey	- specifies reference to return key
bBounce		- specifies reference to bounce return value

This method searches for the next key value relative to the key value rKey. If the method was
successful then bBounce is false and rNextKey contains the next key value relative to rKey, otherwise
bBounce is true and rNextKey contains an undefined value.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::get_next_key (const _t_key &rKey, _t_key &rNextKey, bool &bBounce) const
{
	node_iter_type					nNode, nNextNode;
	sub_node_iter_type				nSub, nNextSub;
	key_type						*psNextKey;
	
	bBounce = false;
	
	// if data not present ...
	if (this->empty ())
	{
		// ... then flag bounce since search for not successful and return
		bBounce = true;
		
		return;
	}
	
	nNode = this->m_nRootNode;
	
	// trace into tree structure until entry of interest has been found
	while (1)
	{
		// determine sub-position of current node that has the next greater key value
		nSub = this->get_firstSubPos (nNode, rKey, true);
		
		// if sub-position is not referring to the last sub-node of the current node ...
		if (nSub < this->get_data_count (nNode))
		{
			// ... then use sub-position directly
			psNextKey = this->extract_key (&rNextKey, nNode, nSub);

			// if the current node is a leaf node ...
			if (this->is_leaf (nNode))
			{
				// ... then walk through the node until an entry containing a different key has been discovered
				while (this->comp (rKey, *psNextKey) == 0)
				{
					// go to next data entry
					this->move_next (nNode, nSub, nNextNode, nNextSub, bBounce);

					// if end of container has been reached ...
					if (bBounce)
					{
						// ... then terminate operation
						return;
					}

					// determine key of next entry
					psNextKey = this->extract_key (&rNextKey, nNextNode, nNextSub);

					// update potential return value
					nNode = nNextNode;
					nSub = nNextSub;
				}

				// tracing further from a leaf node is not possible - terminate operation
				break;
			}
			else
			{
				// otherwise trace further into tree structure

				// if a matching key has been found within an inner node
				if (this->comp (rKey, *psNextKey) == 0)
				{
					// determine following leave node position
					this->move_next (nNode, nSub, nNextNode, nNextSub, bBounce);

					if (bBounce)
					{
						return;
					}

					// update return value
					nNode = nNextNode;
					nSub = nNextSub;
				}
				else
				{
					//  otherwise trace into next node
					nNode = *(this->get_sub_node (nNode, nSub));
				}
			}
		}
		else
		{
			// otherwise trace into last leave node
			nSub--;

			this->move_next (nNode, nSub, nNextNode, nNextSub, bBounce);

			if (bBounce)
			{
				return;
			}

			// determine if the next key is within the current inner node or on the least leave node
			psNextKey = this->extract_key (&rNextKey, nNode, nSub);

			nNode = nNextNode;
			nSub = nNextSub;

			// if the inner node key is greater than the key in question ...
			if (this->comp (rKey, *psNextKey) > 0)
			{
				psNextKey = this->extract_key (&rNextKey, nNextNode, nNextSub);

				// ... and the first key on the leave node is smaller than the key in question
				if (this->comp (rKey, *psNextKey) < 0)
				{
					// ... then the initial key on the leave node is the next key
					break;
				}
			}
		}
	}

	rNextKey = *psNextKey;
}

/*

get_prev_key - get previous key

rKey		- specifies key next to key to be returned
rNextKey	- specifies reference to return key
bBounce		- specifies reference to bounce return value

This method searches for the previous key value relative to the key value rKey. If the method was
successful then bBounce is false and rNextKey contains the previous key value relative to rKey, otherwise
bBounce is true and rNextKey contains an undefined value.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::get_prev_key (const _t_key &rKey, _t_key &rPrevKey, bool &bBounce) const
{
	node_iter_type					nNode, nPrevNode;
	sub_node_iter_type				nSub, nPrevSub;
	
	bBounce = false;
	
	// if data not present ...
	if (this->empty ())
	{
		// ... then flag bounce since search for not successful and return
		bBounce = true;
		
		return;
	}

	// if key is present ...
	if (this->find_oneKey (this->m_nRootNode, rKey, nNode, nSub))
	{
		// ... then look for initial data entry associated with that key value
		this->find_firstKey (nNode, nSub, nPrevNode, nPrevSub);

		nNode = nPrevNode;
		nSub = nPrevSub;
	}

	// go to previous data entry
	this->move_prev (nNode, nSub, nPrevNode, nPrevSub, bBounce);
				
	// if there is no data prior to the already found initial entry ...
	if (bBounce)
	{
		// ... then abort operation
		return;
	}
	
	// otherwise return key value prior to input key value
	rPrevKey = *(this->extract_key (&rPrevKey, nPrevNode, nPrevSub));
}

/*

count - get number of instances of one key

key			- contains on object specifying the key which is to be extracted from this parameter

This method determines the number entries containing the same key as the object of parameter key.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename _t_datalayerproperties::size_type CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::count (const _t_key &rKey) const
{
	size_type					nRetval = (size_type) 0;
	node_iter_type				nNode = this->m_nRootNode, nNodeSeen;
	sub_node_iter_type			nSubPos, nSubPosSeen;
	bool						bBounce;

	// if no data is present ...
	if (this->empty ())
	{
		// ... then return
		return (nRetval);
	}

	// if not a single entry can be found using the same key as pkey is pointing at ...
	if (!find_oneKey (this->m_nRootNode, rKey, nNode, nSubPos))
	{
		// ... then return
		return (nRetval);
	}

	// find first node / sub-node position where the key in question has been 'seen'
	find_firstKey (nNode, nSubPos, nNodeSeen, nSubPosSeen);

	while (1)
	{
		// increment return value on any entry with a matching key
		nRetval++;

		// determine next position in b-tree
		this->move_next (nNodeSeen, nSubPosSeen, nNode, nSubPos, bBounce);

		// if the last seen position also was the final entry ...
		if (bBounce)
		{
			// ... then abort
			break;
		}

		// extract key of next entry
		key_type	*psKey = extract_key (m_pInstancesNewKey, nNode, nSubPos);

		// if key is no longer matching ...
		if (this->comp (*psKey, rKey) != 0)
		{
			// ... then abort
			break;
		}

		// make next position the last seen position
		nNodeSeen = nNode;
		nSubPosSeen = nSubPos;
	}

	return (nRetval);
}

/*

key_compare - get key compare object

This method returns an object with an overloaded call operator, which accepts two key values
as an input. Said operator returns true if the left hand key value is deemed lesser than the
right hand key value, otherwise false.

Note: Unlike other STL implementations, the returned object is only valid for as long as
the container instance employed to create it has not been destroyed.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::key_compare CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::key_comp () const
{
	key_compare		sRetval;

	sRetval.pThis = dynamic_cast <CBTreeAssociativeIf_t *> ((CBTreeAssociative_t *) this);

	return (sRetval);
}

/*

value_compare - get value compare object

This method returns an object with an overloaded call operator, which accepts two entries of type
value_type as an input. Said operator returns true if the left hand value is deemed lesser than the
right hand value, otherwise false.

Note: Unlike other STL implementations, the returned object is only valid for as long as
the container instance employed to create it has not been destroyed.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::value_compare CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::value_comp () const
{
	value_compare		sRetval;

	sRetval.pThis = dynamic_cast <CBTreeAssociativeIf_t *> ((CBTreeAssociative_t *) this);

	return (sRetval);
}
	
/*

serialize - serialize

The serialize method reads out data items from position nFrom to position nTo - 1 and writes the
return data into an array pData is poiting at. It is assumed by this method that the array is large enough
to contain all data that will potentially be written into it, otherwise memory regions not associated with
pData get overwritten.

nStart		- specifies the initial position to be serialized
nLen		- specifies number of entries to be serialized
pData		- pointer to array where result will be copied to

This method returns the number of items that have been written to the array pData is pointing at.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename _t_datalayerproperties::size_type CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::serialize (const typename _t_datalayerproperties::size_type nStart, const typename _t_datalayerproperties::size_type nLen, _t_data *pData) const
{
	position_t			sPos;
	key_type			sKey;
	key_type			*psKey;
	value_type			sData;

	/*
	**	The actual serialize process is done by CBTreeBaseDefaults::serialize.
	**	All this method does is to convert nFrom and nTo into the template parameter _ti_pos (CBTreeKeySortPos <size_type, _t_key>)
	**	and nLen. This has to be done, since CBTreeBaseDefaults::serialize has no concept of what an size_type in terms of a position is.
	*/

	get_at (nStart, sData);

	psKey = extract_key (&sKey, sData);

	// if initial position is requested or the key is unique ...
	if ((nStart == 0) || (count (*psKey) == 1))
	{
		// ... then initial instance of key is implied
		sPos.nInstance = 0;
	}
	else
	{
		// ... otherwise determine instance
		size_type		nInit = get_init_pos_of_key (*psKey);

		sPos.nInstance = nStart - nInit;
	}

	sPos.pKey = psKey;

	return (CBTreeBaseDefaults_t::serialize (sPos, nLen, pData));
}

/*

operator= - assignment operator overload

rContainer		- is a reference to a b-tree that will have its content be assigned to this instance

The result is a reference to this instance.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties> &CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::operator=
	(const CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties> &rContainer)
{
	// if this is not the same instance as the referenced b-tree (rContainer) ...
	if (this != &rContainer)
	{
		CBTreeBaseDefaults_t		&rBaseThis = dynamic_cast<CBTreeBaseDefaults_t &> (*this);
		const CBTreeBaseDefaults_t	&rBaseContainer = dynamic_cast<const CBTreeBaseDefaults_t &> (rContainer);
		
		rBaseThis = rBaseContainer;
	}

	return (*this);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties> &CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::operator=
	(CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties> &&rRhsContainer)
{
	CBTreeBaseDefaults_t		&rBaseThis = dynamic_cast<CBTreeBaseDefaults_t &> (*this);
	CBTreeBaseDefaults_t		&rBaseContainer = dynamic_cast<CBTreeBaseDefaults_t &> (rRhsContainer);
		
	rBaseThis = ::std::move (rBaseContainer);

	return (*this);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
bool CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::operator== (const typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::CBTreeAssociativeIf_t &rRhs) const
{
	typedef typename ::operator_test_scope::has_equal_operator<_t_data>::type			has_equal_operator;

	if (this == &rRhs)
	{
		return (true);
	}

	if (this->size () != rRhs.size ())
	{
		return (false);
	}

	has_equal_operator	sSelectHasEqualOperator;

	const_iterator		sCIterThis = this->cbegin ();
	const_iterator		sCIterThisEnd = this->cend ();

	const_iterator		sCIterRhs = rRhs.cbegin ();

	while (sCIterThis < sCIterThisEnd)
	{
		if (!this->compare_entry (*sCIterThis, *sCIterRhs, sSelectHasEqualOperator))
		{
			return (false);
		}

		::std::advance (sCIterThis, 1);
		::std::advance (sCIterRhs, 1);
	}

	return (true);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
bool CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::operator!= (const typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::CBTreeAssociativeIf_t &rRhs) const
{
	return (!(*this == rRhs));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::_assign
	(const typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::CBTreeIf_t &rContainer)
{
	// remove all data from target container
	this->clear ();

	/*
	**	Although a b-tree guarantees that data is stored balanced; this happens under the assumption that 
	**	data entries are stored and removed in random order. In case data is stored or removed in order, the
	**	tree will become partially unblanced. To prevent this the data is not copied in order, but in a
	**	skewed fashion. The skew width is determined by the target tree's maximum node size (t * 2) and target
	**	tree's future depth (based on the source tree's size) combined, since that ensures an equal distribution
	**	of all data throughout the entire target tree.
	*/
	
	// determine initial part of skew width
	const CBTreeAssociative_t&		rAssociative = dynamic_cast<const CBTreeAssociative_t &> (rContainer);
	size_type						nSkewWidth = this->get_node_max_sub_nodes ();
	size_type						nEntries = 1;
	size_type						i;
	const_iterator					sCIter;
	const_iterator					sCIterEnd = rAssociative.cend ();
	size_type						nAverageNodeSize = this->get_node_max_sub_nodes ();

	nAverageNodeSize *= 3;
	nAverageNodeSize /= 4;

	// combine skew width with eventual target tree depth (log t of n, while t is node size and n is tree size)
	for (nSkewWidth--; nEntries <= rAssociative.size (); nSkewWidth++)
	{
		// on average every new layer allows for t times current tree size more entries
		nEntries *= nAverageNodeSize;
	}

	// copy data using skew width
	for (i = 0; i < nSkewWidth; i++)
	{
		sCIter = rAssociative.cbegin ();

		::std::advance (sCIter, i);

		for (; sCIter < sCIterEnd; ::std::advance (sCIter, nSkewWidth))
		{
			key_type	sKey;
			key_type	*psKey;
			value_type	sData (*sCIter);

			psKey = this->extract_key (&sKey, sData);

			size_type	nNumKeyPresentInDest = this->count (*psKey);

			if (nNumKeyPresentInDest == 0)
			{
				size_type	nNumKeyPresentInSrc = rAssociative.count (*psKey);

				if (nNumKeyPresentInSrc == 1)
				{
					this->insert (sData);
				}
				else
				{
					const_iterator	sCIterSrcLower = rAssociative.lower_bound (*psKey);
					const_iterator	sCIterSrcUpper = rAssociative.upper_bound (*psKey);

					this->insert (sCIterSrcLower, sCIterSrcUpper);

					// if the previous insert copied all remaining data ...
					if (this->size () == rAssociative.size ())
					{
						// ... then abort early
						i = nSkewWidth;

						break;
					}
				}
			}
		}
	}
}

/*

get_at - get at

nPos		- specifies the the linear position
rData		- reference to where the returned entry is put


This method returns true is the access was successful, otherwise false.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
bool CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::get_at (const typename _t_datalayerproperties::size_type nPos, _t_data &rData) const
{
	bool				retval = false;
	node_iter_type		nNode;
	sub_node_iter_type	nSubPos;

	// if data layer is present ...
	if (!this->empty ())
	{
		// ... and the linear position does not exceed the size of this tree ...
		if (nPos < this->size ())
		{
			// ... then obtain of the data entry at the linear position in question
			this->convert_pos_to_container_pos (this->m_nRootNode, nPos, nNode, nSubPos);

			rData = *(this->get_data (nNode, nSubPos));

			retval = true;
		}
	}

	return (retval);
}

/*

get_init_pos_of_key - obtain first linear position of one key

key			- contains on object specifying the key which is to be extracted

If a number of entries is sharing the same key, this method is to be used to obtain the initial
position of those entries. However, the key must exists, otherwise the result is undefined.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename _t_datalayerproperties::size_type CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::get_init_pos_of_key (const _t_key &rKey) const
{
	size_type						retval = (size_type) ~0;
	node_iter_type					nNode, fstNode;
	node_iter_type					*pnNode;
	sub_node_iter_type				sub, fstSub;
	uint32_t						comp = 0;
	bool							bContinue;
	node_t							*psNodeDesc;

	// if data layer not present ...
	if (this->empty ())
	{
		// ... then return failure
		return (retval);
	}

	// if any entry using that key exists ...
	if (find_oneKey (this->m_nRootNode, rKey, nNode, sub))
	{
		// ... then find first entry sharing the same key and find its linear position within the tree
		find_firstKey (nNode, sub, fstNode, fstSub);

		nNode = fstNode;
		sub = fstSub;

		retval = (size_type) 0;

		// move from parent node to parent node until the root node has been found and therewith the linear position
		do
		{
			// get description of current node
			psNodeDesc = this->get_node (nNode);

			bContinue = (nNode != this->m_nRootNode);

			// combine the number of entries in the current node with the result so far
			retval += (sub_node_iter_type) (sub + comp);

			// if this is an inner node ...
			if (!this->is_leaf (nNode))
			{
				// ... then add the number of all entries of previous or left sub nodes
				for (; sub != ((sub_node_iter_type) ~0); sub--)
				{
					pnNode = this->get_sub_node (nNode, sub);

					retval += this->get_max_index (*pnNode);
				}
			}

			// if the current node is not the root node ...
			if (bContinue)
			{
				// ... move to the parent node, which during the next iteration will be the current node
				sub = this->find_sub_node_offset (psNodeDesc->nParent, nNode) - 1;
				nNode = psNodeDesc->nParent;

				comp = 1;
			}
		} while (bContinue);
	}

	return (retval);
}

/*

determine_position - determine position

This method tries to determine whether or not a data item at position sPos is in the
node nNode or in one of its sub-nodes. If the return value bFound is true, then nSubData
points at the data item the caller is interested in, otherwise nSubPos must be used to
determine next sub-node and this method's return value ought to be used as parameter sPos
for the next call to determine_position.

This method is used to step into a tree to determine the position of a specific data item
to then further process it.

sPos		- specifies the absolute linear position of the data entry
nNode		- specifies the node to step into
nSubPos		- returns the sub-position in this node to determine the next node to step into, if bFound is false
nSubData	- returns the sub-position of the data item in this node
bFound		- return true if the sought data item is in node nNode, otherwise false

If bFound is false, then this method returns the parameter sPos for the next call to
determine_position, otherwise it return sPos again.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::position_t CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::determine_position (typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::position_t sPos, typename _t_datalayerproperties::node_iter_type nNode, typename _t_datalayerproperties::sub_node_iter_type &nSubPos, typename _t_datalayerproperties::sub_node_iter_type &nSubData, bool &bFound) const
{
	size_type		nInstancePos;
	int				triCmpRslt;
	node_t			*psNodeDesc;

	psNodeDesc = this->get_node (nNode);

	// if node is a leaf node ...
	if (this->is_leaf (nNode))
	{
		// ... then convert sPos to a linear within this node
		sub_node_iter_type		uBase, ui32;

		nSubData = 0UL;

		// since this is a leaf node and stepping into another sub-node is not possible, it is simply assumed that the data is to be found in this node
		bFound = true;

		// binary sub-divide the data in this node until the correct sub-data position (nSubData) has been idenfied
		for (uBase = 0UL, ui32 = this->get_data_count (*psNodeDesc) / 2; ui32 > 0; ui32 >>= 1)
		{
			allocateShortLiveKey ();
			{
				// test for correct position in sub-division
				for (nSubData = uBase; nSubData < this->get_data_count (*psNodeDesc); nSubData += ui32)
				{
					triCmpRslt = this->comp (*(extract_key (*m_ppShortLiveKey, nNode, nSubData)), *(sPos.pKey));
					// if the key of the current sub-position is smaller than the sought key ...
					if (triCmpRslt < 0)
					{
						// ... then advance to next sub-division
						uBase = nSubData;
					}
					// if current key is equal to sought key ...
					else if (triCmpRslt == 0)
					{
						// ... and it is key's instance of interest ...
						if ((sPos.nInstance == (size_type) (~0)) || (sPos.nInstance == get_instancePos (nNode, nSubData)))
						{
							// ... then abort search
							ui32 = 0UL;

							break;
						}
					}
					else
					{
						// if the current key is bigger than the sought key, then abort and sub-divide further
						break;
					}
				}
			}
			freeShortLiveKey ();
		}
	}
	else
	{
		// if this is an inner node, then try to find data item within this node or identify the next sub-node
		bFound = false;

		// find nearest sub-position of key of interest in this node
		nSubData = nSubPos = get_firstSubPos (nNode, *(sPos.pKey));

		// if the nearest sub-position is not the last position ...
		if (nSubPos < this->get_data_count (*psNodeDesc))
		{
			allocateShortLiveKey ();
			{
				triCmpRslt = this->comp (*(sPos.pKey), *(extract_key (*m_ppShortLiveKey, nNode, nSubData)));

				// if key of interest matches key at data sub-position ...
				if (triCmpRslt == 0)
				{
					// ... and it is key's instance of interest ...
					if ((sPos.nInstance == (size_type) (~0)) || (get_instancePos (nNode, nSubPos) == sPos.nInstance))
					{
						// ... then flag the find
						bFound = true;
					}
				}
			}
			freeShortLiveKey ();
		}

		// if data has not been found yet ...
		if (!bFound)
		{
			// ... and if a specific key instance is being sought ...
			if (sPos.nInstance != (size_type) ~0)
			{
				// ... then test for other instances that share the same key and if any of those has the correct instance
				allocateShortLiveKey ();
				{
					// walk through node until the end of the node has been reached or a different key has been found
					for (; ((nSubPos < this->get_data_count (*psNodeDesc)) && (this->comp (*(sPos.pKey), *(extract_key (*m_ppShortLiveKey, nNode, nSubPos))) == 0)); nSubPos++, nSubData++)
					{
						nInstancePos = get_instancePos (nNode, nSubPos);

						// if the found instance is bigger than the one of interest ...
						if (nInstancePos > sPos.nInstance)
						{
							// ... then the correct instance must be in the previous sub-node and the search in this node is aborted
							break;
						}
						// if the correct instance has been found ...
						else if (nInstancePos == sPos.nInstance)
						{
							// ... then flag the find and abort search
							bFound = true;

							break;
						}
					}
				}
				freeShortLiveKey ();
			}

			// if data sub-position exceeds node data size as a result of the previous loop ...
			if (nSubData >= this->get_data_count (*psNodeDesc))
			{
				// ... then limit data sub-position to node's data size
				nSubData -= 1;
			}
		}
	}

	return (sPos);
}

/*

rebuild_node - rebuild node after modification

This method is called to re-instate the integrity of this node within the b-tree. If a
node is split, merged or an entry is inserted or removed, then not only the size of a node
needs updating. Also the maximum index (nMaxIdx), which displays the size of this node
as well as any sub-nodes of this node combined, has to be re-calculated.

nNode		- specifies the node to be updated
triMod		- specifies a tri-state fast update modifier
				+ if set to zero, then fully re-calculate nMaxIdx
				+ if not set to zero, then add triMod to this node's nMaxIdx

Note:	The parameter triMod is usually set 1 or -1 if one entry is inserted or removed
		respectively. This avoids the requirement to look at any sub-node's nMaxIdx, which
		as a result avoids to have the sub-node descriptor cache modified to a dis-advantage.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::rebuild_node (const typename _t_datalayerproperties::node_iter_type nNode, const int32_t triMod, typename _t_datalayerproperties::sub_node_iter_type nSubStart)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::rebuild_node: requested node exceeds tree size");
#endif

	node_iter_type					*pnSubNode;
	node_t							*psNodeDesc;
	size_type						nIdx = (size_type) 0;
	sub_node_iter_type				ui32;
	sub_node_iter_type				nSubPosOffset;
	size_type						*pnSerVector;
	int								nLocalTriMod = triMod;

	psNodeDesc = this->get_node (nNode);

	// if nNode is a leaf node ...
	if (this->is_leaf (nNode))
	{
		// ... then use the arithmetic negative size value of this node to determine nMaxIdx
		nIdx = ((sub_node_iter_type) (0 - psNodeDesc->nNumData));

		// Note:	If a node is a leaf node, then the size is displayed as a negative
		//			value, which appears as a 2's complement value, since the size actually is unsigned.
	}
	else
	{
		const sub_node_iter_type						nNumData = this->get_data_count (*psNodeDesc);

		pnSerVector = this->get_serVector (nNode);

		// if nMaxIdx has been determined (already set up node) and the fast modifier is set ...
		if ((psNodeDesc->nMaxIdx != 0) && (triMod != 0))
		{
			// ... then use fast modifier to update nMaxIdx and serial vector
			
			// initialise index variable being employed to populate serial vector
			if (nSubStart == 0)
			{
				nIdx = 0;
			}
			else
			{
				nSubPosOffset = nSubStart - 1;
				nIdx = pnSerVector[nSubPosOffset] - nSubPosOffset;
			}

			// seek point where serial vector mismatches expected sequence based on sub-node indexes
			for (ui32 = nSubStart; ui32 < sub_node_iter_type (nNumData - 1); ui32++)
			{
				pnSubNode = this->get_sub_node (nNode, ui32);

				nIdx += this->get_max_index (*pnSubNode);

				if (pnSerVector[ui32] != (nIdx + ui32))
				{
					break;
				}
			}

			// update remaining serial vector part by using fast modifier
			for (; ui32 < sub_node_iter_type (nNumData - 1); ui32++)
			{
				pnSerVector[ui32] += triMod;
			}

			nIdx = psNodeDesc->nMaxIdx + triMod;

			if (ui32 == 0)
			{
				pnSubNode = this->get_sub_node (nNode, 0);

				pnSerVector[0] = this->get_max_index (*pnSubNode);
			}
			else if (ui32 < nNumData)
			{
				pnSubNode = this->get_sub_node (nNode, nNumData);

				const size_type	nLastIdx = this->get_max_index (*pnSubNode);

				pnSubNode = this->get_sub_node (nNode, nNumData - 1);

				const size_type	nSecondLastIdx = this->get_max_index (*pnSubNode);
				size_type			nTrialingIdx = nLastIdx;

				nTrialingIdx += nSecondLastIdx;

				nTrialingIdx += 2;

				if (nIdx != (pnSerVector[ui32 - 1] + nTrialingIdx))
				{
					nLocalTriMod = 0;
				}
				else
				{
					// code cannot be sure if the last entry had valid data in the first place
					// re-build last entry from all other values
					pnSerVector[ui32] = nIdx - nLastIdx - 1;

					// the above code doesn't account for new entries
					// if new entry has been created, which is not in the last place ...
					if (pnSerVector[ui32] < pnSerVector[ui32 - 1])
					{
						// ..then fall back to slow full re-build mode
						nLocalTriMod = 0;
					}
				}
			}
		}
		
		// if nMaxIdx has not been determined yet (newly created node) or the fast modifier is not set ...
		if ((psNodeDesc->nMaxIdx == 0) || (nLocalTriMod == 0))
		{
			this->m_pData->set_cacheFreeze (true);
			{
				// ... then fully re-calculate nMaxIdx and serial vector
				for (ui32 = 0UL, nIdx = 0; ui32 < this->get_sub_node_count (*psNodeDesc); ui32++)
				{
					pnSubNode = this->get_sub_node (nNode, ui32);

					nIdx += this->get_max_index (*pnSubNode);

					if (ui32 < this->get_data_count (nNode))
					{
						pnSerVector[ui32] = nIdx + ui32;
					}
				}
			}
			this->m_pData->set_cacheFreeze (false);

			nIdx += this->get_data_count (*psNodeDesc);
		}
	}

	// write result back
	psNodeDesc->nMaxIdx = nIdx;
}

/*

generate_prev_position - generate previous position

This method returns the previous position (CBTreeBaseDefaults::_ti_pos = CBTreeKeySortPos<size_type, _t_key>)
based on the parameter sPos and the node / sub-node tuple associated with sPos.

nNode		- specifies the node of the node / sub-position tuple of which the previous position will be generated from
nSub		- specifies the sub_position of the node / sub-position tuple of which the previous position will be generated from
sPos		- class specific position associated with node / sub-node tuple

The return value is the previous linear position of sPos (nNode, nSub) in terms of how data is
presented to the application. Given that the b-tree classes are supposed to act like a linear
list to calling the application, this pseudo code displays what generate_prev_position does:

return_value = (*this)[sPos (this, nNode, nSub) - 1];

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::position_t CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::generate_prev_position (const typename _t_datalayerproperties::node_iter_type nNode, const typename _t_datalayerproperties::sub_node_iter_type nSub, typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::position_t sPos) const
{
	node_iter_type		nPrevNode;
	sub_node_iter_type	nPrevSubPos;
	bool				bBounce;
	value_type			*psData;

	// get previous position within tree
	this->move_prev (nNode, nSub, nPrevNode, nPrevSubPos, bBounce);

#if defined (_DEBUG)
	BTREE_ASSERT (!this->is_leaf (nNode), "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::generate_prev_position: This method cannot be used on leaf nodes!");
	BTREE_ASSERT (!bBounce, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::generate_prev_position: unexpected bounce!");
#endif

	psData = this->get_data (nPrevNode, nPrevSubPos);

	// get key of previous position
	key_type	*psKey = extract_key (m_pTempRemoveKey, *psData);

	*m_pTempRemoveKey = *psKey;

	sPos.pKey = m_pTempRemoveKey;
	sPos.nInstance = get_instancePos (nPrevNode, nPrevSubPos);

	return (sPos);
}

/*

generate_next_position - generate next position

This method returns the next position (CBTreeBaseDefaults::_ti_pos = CBTreeKeySortPos<size_type, _t_key>)
based on the parameter sPos and the node / sub-node tuple associated with sPos.

nNode		- specifies the node of the node / sub-position tuple of which the next position will be generated from
nSub		- specifies the sub_position of the node / sub-position tuple of which the next position will be generated from
sPos		- class specific position associated with node / sub-node tuple

The return value is the next linear position of sPos (nNode, nSub) in terms of how data is
presented to the application. Given that the b-tree classes are supposed to act like a linear
list to calling the application, this pseudo code displays what generate_prev_position does:

return_value = (*this)[sPos (this, nNode, nSub) + 1];

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::position_t CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::generate_next_position (typename _t_datalayerproperties::node_iter_type nNode, typename _t_datalayerproperties::sub_node_iter_type nSub, typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::position_t sPos) const
{
	node_iter_type		nNextNode;
	sub_node_iter_type	nNextSubPos;
	bool				bBounce;
	value_type			*psData;

	// get previous position within tree
	this->move_next (nNode, nSub, nNextNode, nNextSubPos, bBounce);

#if defined (_DEBUG)
	BTREE_ASSERT (!this->is_leaf (nNode), "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::generate_next_position: This method cannot be used on leaf nodes!");
	BTREE_ASSERT (!bBounce, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::generate_next_position: unexpected bounce!");
#endif

	psData = this->get_data (nNextNode, nNextSubPos);

	// get key of previous position
	key_type	*psKey = extract_key (m_pTempRemoveKey, *psData);

	*m_pTempRemoveKey = *psKey;

	sPos.pKey = m_pTempRemoveKey;
	sPos.nInstance = get_instancePos (nNextNode, nNextSubPos);

	return (sPos);
}

/*

comp - compare keys

This is the default method to compare two keys.

pkey0		- first key to compare
pkey1		- second key to compare

The return value is zero if both keys are matching, negative if the first key is smaller than
the second key and positive if the first key is bigger than the second key.

Note:	This method is application specific. By default the entire data set is deemed to be its key.
		If the key needs to be interpreted from the data set or not the entire data set is the, than
		a class inheriting from CBTreeKeySort needs to be created, replacing this method.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
int CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::comp (const _t_key &rKey0, const _t_key &rKey1) const
{
	return (memcmp ((void *) &rKey0, (void *) &rKey1, sizeof (_t_key)));
}

/*

extract_key - extract key

This is the default method to convert the dataset associated with node / sub-node into a key.

nNode		- node parameter of node / sub-node tuple
nEntry		- sub-node parameter of node / sub-node tuple
obj			- dataset to be converted

The return value is pKey again. This enables extract_key to act as a parameter for other methods.

Note:	This method is application specific. By default the entire data set is deemed to be its key.
		If the key needs to be interpreted from the data set or not the entire data set is the, than
		a class inheriting from CBTreeKeySort needs to be created, replacing this method.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
_t_key *CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::extract_key (_t_key *pKey, const typename _t_datalayerproperties::node_iter_type nNode, const typename _t_datalayerproperties::sub_node_iter_type nEntry) const
{
	value_type			*psData;

#if defined (_DEBUG)
	BTREE_ASSERT (nEntry < this->get_data_count (nNode), "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::extract_key (_t_key *, const node_iter_type, const sub_node_iter_type): requested entry exceeds nodes data size");
#endif

	// obtain dataset
	psData = this->get_data (nNode, nEntry);

	// convert dataset to key
	return (extract_key (pKey, *psData));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
_t_key *CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::extract_key (_t_key *pKey, const _t_data &rData) const
{
	typename ::std::is_convertible<_t_data, _t_key>::type	sExtractSelect;

	return (CBTreeAssociative_t::extract_key (pKey, rData, sExtractSelect));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
_t_key *CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::extract_dataset_as_key (_t_key *pKey, const _t_data &rData, typename ::std::true_type) const
{
	pKey;

	return (&rData);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
_t_key *CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::extract_dataset_as_key (_t_key *pKey, const _t_data &rData, typename ::std::false_type) const
{
	typename ::std::is_convertible<_t_data, _t_key *>::type	sExtractSelect;	// extract by reference or conversion
	
	return (CBTreeAssociative_t::extract_key_directly (pKey, rData, sExtractSelect));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
bool CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::compare_entry (const value_type &rLhs, const value_type &rRhs, typename ::std::true_type) const
{
	return (rLhs == rRhs);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
bool CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::compare_entry (const value_type &rLhs, const value_type &rRhs, typename ::std::false_type) const
{
	int		triRslt = memcmp ((void *) &rLhs, (void *) &rRhs, sizeof (rLhs));

	return (triRslt == 0);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
_t_key *CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::extract_key_directly (_t_key *pKey, const _t_data &rData, typename ::std::true_type) const
{
	pKey;

	return ((_t_key *) rData);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
_t_key *CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::extract_key_directly (_t_key *pKey, const _t_data &rData, typename ::std::false_type) const
{
	typename ::std::is_convertible<_t_data, _t_key>::type	sExtractSelect;

	return (CBTreeAssociative_t::extract_key (pKey, rData, sExtractSelect));
}

/*

extract_key - extract key

This is the default method to convert a dataset into a key.

pKey		- pointer to location where the extracted key will be written to
obj			- dataset to be converted

The return value is pKey again. This enables extract_key to act as a parameter for other methods.

Note:	This method is application specific. By default the entire data set is deemed to be its key.
		If the key needs to be interpreted from the data set or not the entire data set is the, than
		a class inheriting from CBTreeKeySort needs to be created, replacing this method.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
_t_key *CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::extract_key (_t_key *pKey, const _t_data &rData, typename ::std::true_type) const
{
	value_type	sData = rData;

	*pKey = (_t_key) sData;

	return (pKey);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
_t_key *CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::extract_key (_t_key *pKey, const _t_data &rData, typename ::std::false_type) const
{
#if defined (_DEBUG)

	static int	nCallDepth = 0;

	// if this assertion is hit, then _t_data cannot be cast to _t_key and this or the deriving class has not provided its version of extract_key ()
	BTREE_ASSERT (nCallDepth == 0, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::extract_key (_t_key *, const _t_data &, typename ::std::false_type): Call depth not zero");

	nCallDepth++;

#endif

	key_type	*pRslt;

	pRslt = this->extract_key (pKey, rData);

#if defined (_DEBUG)

	nCallDepth--;

#endif

	return (pRslt);
}

/*

get_instancePos - get instance position

The method get_instancePos extracts the key of sub-position (nSub) in node (nNode) and returns
the number of linear entries before the given entry, which share the same key.

nNode		- node parameter of node / sub-node tuple
nSub		- sub-node parameter of node / sub-node tuple

The return value is zero if no other entry shares the same key or it is the initial instance of
an entry streak sharing the same key, otherwise the number of entries before the given instance
sharing the same key is returned.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename _t_datalayerproperties::size_type CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::get_instancePos (typename _t_datalayerproperties::node_iter_type nNode, typename _t_datalayerproperties::sub_node_iter_type nSub) const
{
	size_type					nRetval = (size_type) 0;
	bool						bBounce;
	node_iter_type				nFromNode = nNode;
	sub_node_iter_type			nFromSub = nSub;
	node_iter_type				*pnNode;

	// determine one key of the key raw in question
	key_type	*psKeyInit = extract_key (*m_ppTempFindFirstKeyKey, nFromNode, nFromSub);

	// continue searching for the initial position of key raw until either the beginning of the list or a different key has been found
	while (true)
	{
		// update output position
		nNode = nFromNode;
		nSub = nFromSub;

		if (	this->is_leaf (nNode) || 
				(nSub == 0) || 
				(this->comp (*psKeyInit, *(extract_key (m_pTempFindFirstKeyNewKey, nNode, nSub - 1))) != 0)
			)
		{
			// move to previous position
			this->move_prev (nNode, nSub, nFromNode, nFromSub, bBounce);
		}
		else
		{
			bBounce = false;

			nFromSub = nSub - 1;

			pnNode = this->get_sub_node (nNode, nSub);

			nRetval += this->get_max_index (*pnNode);
		}

		// if beginning the of the list has been reached ...
		if (bBounce)
		{
			// ... then abort searching
			break;
		}

		key_type	*psKey = extract_key (m_pTempFindFirstKeyNewKey, nFromNode, nFromSub);

		// if a different key has been found ...
		if (this->comp (*psKey, *psKeyInit) != 0)
		{
			// ... then abort searching
			break;
		}

		nRetval++;
	}

	return (nRetval);
}

/*

get_firstSubPos - get first sub-position

To find the sub-position of the nearest greater or equal key of what pKey is pointing at
within a node, this method ought to be used.

nNode		- specifies the node in which the nearest greater or equal key is to be sought in
pKey		- points at key to be sought

The value returned is the sub-position, which has the nearest greater or equal key compared to *pKey.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename _t_datalayerproperties::sub_node_iter_type CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::get_firstSubPos (typename _t_datalayerproperties::node_iter_type nNode, _t_key const &rKey, bool bReverse) const
{
	node_t						*psNodeDesc;
	sub_node_iter_type			ui32;
	sub_node_iter_type			nMinPos, nMaxPos;
	int							triCmpRslt = ~0x0;

	// load node descriptor
	psNodeDesc = this->get_node (nNode);

	// set range up
	nMinPos = 0UL;

	nMaxPos = this->get_data_count (*psNodeDesc);

	// until first sub postion has been found ...
	while (nMinPos != nMaxPos)
	{
		// sub divide range and determine average middle position
		ui32 = (nMinPos + nMaxPos) / 2;

		allocateShortLiveKey ();
		{
			// get key of current middle position
			key_type	*psKey = extract_key (*m_ppShortLiveKey, nNode, ui32);

			triCmpRslt = this->comp (rKey, *psKey);

			// if search key is smaller than or equal to middle key ...
			if (triCmpRslt <= 0)
			{
				// ... then range maximum assumes middle position
				nMaxPos = ui32;
			}
			else
			{
				// ... otherwise, range minimum assumes next position after middle position
				nMinPos = ui32 + 1;
			}
		}
		freeShortLiveKey ();
	}

	if (bReverse)
	{
		if (triCmpRslt == 0)
		{
			// set range up
			nMaxPos = this->get_data_count (*psNodeDesc);

			// until last equal sub postion has been found ...
			while ((nMinPos + 1) != nMaxPos)
			{
				// sub divide range and determine average middle position
				ui32 = (nMinPos + nMaxPos) / 2;

				allocateShortLiveKey ();

				// get key of current middle position
				key_type	*psKey = extract_key (*m_ppShortLiveKey, nNode, ui32);

				triCmpRslt = this->comp (rKey, *psKey);

				// if search key is smaller than or equal to middle key ...
				if (triCmpRslt < 0)
				{
					// ... then range maximum assumes middle position
					nMaxPos = ui32;
				}
				else
				{
					// ... otherwise, range minimum assumes next position after middle position
					nMinPos = ui32;
				}

				freeShortLiveKey ();
			}
		}
	}

	return (nMinPos);
}

/*

find_next_sub_pos - find next sub-position

To find the sub-position of the nearest greater key of what pKey is pointing at
within a node, this method ought to be used.

nNode		- specifies the node in which the nearest greater key is to be sought in
pKey		- points at key to be sought

The value returned is the sub-position, which has the nearest greater key compared to *pKey.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename _t_datalayerproperties::sub_node_iter_type CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::find_next_sub_pos (const typename _t_datalayerproperties::node_iter_type nNode, typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::position_t &sPos) const
{
	node_t						*psNodeDesc;
	sub_node_iter_type			ui32;
	sub_node_iter_type			nMinPos, nMaxPos;

	// load node descriptor
	psNodeDesc = this->get_node (nNode);

	// set range up
	nMinPos = 0UL;

	nMaxPos = this->get_data_count (*psNodeDesc);

	// until nearest sub postion has been found ...
	while (nMinPos != nMaxPos)
	{
		// sub divide range and determine average middle position
		ui32 = (nMinPos + nMaxPos) / 2;

		allocateShortLiveKey ();

		// get key of current middle position
		key_type	*psKey = extract_key (*m_ppShortLiveKey, nNode, ui32);

		// if search key is smaller than middle key ...
		if (this->comp (*(sPos.pKey), *psKey) < 0)
		{
			// ... then range maximum assumes middle position
			nMaxPos = ui32;
		}
		else
		{
			// ... otherwise, range minimum assumes next position after middle position
			nMinPos = ui32 + 1;
		}

		freeShortLiveKey ();
	}

	return (nMinPos);
}

/*

find_oneKey - find one key

To track down into the tree to find any location, which has the same key as pKey is
pointing at, this method is to be used.

nStartNode	- specifies the node to start the tracking from
pkey		- specifies the pointer to the key in question
nNode		- returns the node in which an equal key has been found
nSub		- returns the sub-position within the node in which an equal key has been found

If it was possible to find any matching key, then true is returned, otherwise false.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
bool CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::find_oneKey (const node_iter_type nStartNode, const _t_key &rKey, typename _t_datalayerproperties::node_iter_type &nNode, typename _t_datalayerproperties::sub_node_iter_type &nSub, typename _t_datalayerproperties::size_type *pnPos) const
{
	uint32_t	bFound;

	nNode = nStartNode;

	if (pnPos != NULL)
	{
		(*pnPos) = 0;
	}

	// continue until one node has been found that contains *pkey
	while (1)
	{
		bFound = 0;

		// determine current node's sub position that has the nearest smaller or equal key
		nSub = get_firstSubPos (nNode, rKey);

		// load node descriptor
		node_t	*psNodeDesc;
			
		psNodeDesc = this->get_node (nNode);

		// if key can be found in current node ...
		if (nSub < this->get_data_count (*psNodeDesc))
		{
			// ... then test if key is part of the current node

			allocateShortLiveKey ();

			// extract key from node's sub postition
			key_type	*psKey = extract_key (*m_ppShortLiveKey, nNode, nSub);

			// if extracted key is equal to searched key ...
			if (this->comp (*psKey, rKey) == 0)
			{
				// ... then abort searching
				bFound = 1;
			}

			freeShortLiveKey ();
		}

		if (pnPos != NULL)
		{
			sub_node_iter_type	i;

			if (!this->is_leaf (nNode))
			{
				for (i = 0; i < (nSub + bFound); i++)
				{
					node_iter_type		*pnSubNode = this->get_sub_node (nNode, i);

					(*pnPos) += this->get_max_index (*pnSubNode);
				}
			}

			(*pnPos) += nSub;
		}

		if (bFound != 0)
		{
			break;
		}

		// if the current node is a leaf node ...
		if (this->is_leaf (nNode))
		{
			// ... then abort searching and return failure
			return (false);
		}

		// determine sub node for next iteration
		nNode = *(this->get_sub_node (nNode, nSub));
	}

	return (true);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
bool CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::find_nextKey (const key_type &rKey, const node_iter_type nFromNode, const sub_node_iter_type nFromSub, node_iter_type &rnNode, sub_node_iter_type &rnSub, size_type *pnPos) const
{
	if (pnPos != NULL)
	{
		if (*pnPos >= this->size ())
		{
			return (false);
		}
	}

	key_type		*psKey;
	bool			bBounce;

	this->move_next (nFromNode, nFromSub, rnNode, rnSub, bBounce);

	if (bBounce)
	{
		if (pnPos != NULL)
		{
			*pnPos = this->size ();
		}

		return (false);
	}

	if (pnPos != NULL)
	{
		(*pnPos)++;
	}

	node_iter_type		nThisNode;
	sub_node_iter_type	nThisSub;

	while (1)
	{
		
		nThisNode = rnNode;
		nThisSub = rnSub;

		sub_node_iter_type	nMinPos = nThisSub;
		sub_node_iter_type	nMaxPos = this->get_data_count (nThisNode);

		psKey = this->extract_key (*this->m_ppShortLiveKey, nThisNode, nMaxPos - 1);

		if (this->comp (rKey, *psKey) == 0)
		{
			this->move_next (nThisNode, nMaxPos - 1, rnNode, rnSub, bBounce);

			if (bBounce)
			{
				if (pnPos != NULL)
				{
					*pnPos = this->size ();
				}

				return (false);
			}
			else
			{
				if (pnPos != NULL)
				{
					if (this->is_leaf (nThisNode))
					{
						*pnPos += nMaxPos - nMinPos;
					}
					else
					{
						if (rnSub > 0)
						{
							size_type	*pnSerVector = this->get_serVector (rnNode);

							*pnPos += pnSerVector[nMaxPos - 1] - pnSerVector[nMinPos];
						}

						(*pnPos)++;
					}
				}

				continue;
			}
		}

		while (nMinPos != nMaxPos)
		{
			sub_node_iter_type	nMidPos = (nMinPos + nMaxPos) / 2;

			psKey = this->extract_key (*this->m_ppShortLiveKey, nThisNode, nMidPos);

			if (this->comp (rKey, *psKey) < 0)
			{
				nMaxPos = nMidPos;
			}
			else
			{
				nMinPos = nMidPos + 1;
			}
		}

		if (this->is_leaf (nThisNode))
		{
			rnNode = nThisNode;
			rnSub = nMinPos;

			if (pnPos != NULL)
			{
				*pnPos += (nMinPos - nThisSub);
			}

			return (true);
		}
		else
		{
			this->move_prev (nThisNode, nMinPos, rnNode, rnSub, bBounce);

#if defined (_DEBUG)

			BTREE_ASSERT (!bBounce, "CBTreeAssociative<>::find_nextKey (): ERROR: move_prev ought not to return with bounce flagged on an inner node!");

#endif

			if ((pnPos != NULL) && (nThisSub != nMinPos))
			{
				size_type	*pnSerVector = this->get_serVector (nThisNode);

				*pnPos += pnSerVector[nMinPos] - pnSerVector[nThisSub];
			}

			psKey = this->extract_key (*this->m_ppShortLiveKey, rnNode, rnSub);

			if (this->comp (rKey, *psKey) == 0)
			{
				rnNode = nThisNode;
				rnSub = nMinPos;

				return (true);
			}

			if (pnPos != NULL)
			{
				*pnPos -= rnSub + 1;
			}

			rnSub = 0;
		}
	}
}

/*

find_firstKey - find first key

This method extracts the key from the sub-position nFromSub within node nFromNode and then rewinds
until the first location, that has the same key as the one extracted, has been found. 

nFromNode	- specifies node from where to rewind from
nFromSub	- specifies the sub-position within nFromNode
nNode		- returns the node where the first key has been found
nSub		- returns the sub-position within nNode where the first key has been found

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename _t_datalayerproperties::size_type CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::find_firstKey (typename _t_datalayerproperties::node_iter_type nFromNode, typename _t_datalayerproperties::sub_node_iter_type nFromSub, typename _t_datalayerproperties::node_iter_type &nNode, typename _t_datalayerproperties::sub_node_iter_type &nSub) const
{
	size_type			nRetval = 0;
	size_type			nStepSize = 0;
	node_iter_type		*pnSubNode;
	bool				bBounce;
	
	// determine one key of the key raw in question
	key_type	*psKeyInit = extract_key (*m_ppTempFindFirstKeyKey, nFromNode, nFromSub);

	// continue searching for the initial position of key raw until either the beginning of the list or a different key has been found
	while (1)
	{
		// update output position
		nNode = nFromNode;
		nSub = nFromSub;

		nRetval += nStepSize;

		if (	this->is_leaf (nNode) || 
				(nSub == 0) || 
				(this->comp (*psKeyInit, *(extract_key (m_pTempFindFirstKeyNewKey, nNode, nSub - 1))) != 0)
			)
		{
			// move to previous position
			this->move_prev (nNode, nSub, nFromNode, nFromSub, bBounce);

			nStepSize = 1;
		}
		else
		{
			bBounce = false;

			pnSubNode = this->get_sub_node (nFromNode, nFromSub);

			nFromSub = nSub - 1;

			nStepSize = this->get_max_index (*pnSubNode);
			nStepSize += 1;
		}

		// if beginning the of the list has been reached or a different key has been found ...
		if (bBounce)
		{
			// ... then abort searching
			break;
		}

		key_type	*psKey = extract_key (m_pTempFindFirstKeyNewKey, nFromNode, nFromSub);

		if (this->comp (*psKey, *psKeyInit) != 0)
		{
			break;
		}
	}

	return (nRetval);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::erase_via_reference
	(
		typename CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::const_iterator &rCIterPos, 
		bool bReEvaluate
	)
{
	bool			bFallBack = true;
	
	// if the iterator is associated with this btree ...
	if (this == rCIterPos.get_container ())
	{
		// ... and if iterator ready for use ...
		if (rCIterPos.is_evaluated ())
		{
			iterator_state_t	*psIterState = (iterator_state_t *) rCIterPos.get_iterator_state ();

			// ... and if the node to be modified is a leaf node ...
			if (this->is_leaf (psIterState->nNode))
			{
				// ... and if the node to be modified has enough entries left ...
				if (this->get_data_count (psIterState->nNode) > (this->get_node_max_data_size () / 2))
				{
					// ... then an additional entry can be added fast
					node_iter_type			nNode;

					bFallBack = false;

					// insert entry into leaf node
					this->remove_data_from_leaf (psIterState->nNode, psIterState->nSubPos);

					// rebuild all parent node's integrity by back-tracing to the root node
					nNode = psIterState->nNode;

					while (nNode != this->m_nRootNode)
					{
						nNode = this->get_parent (nNode);

						this->rebuild_node (nNode, -1);
					}

					// update time stamp
					this->update_time_stamp ();

					if (psIterState->nSubPos < this->get_data_count (psIterState->nNode))
					{
						// overwrite iterator time stamp directly, without having the iterator to evaluate again
						rCIterPos.set_time_stamp (this);

						bReEvaluate = false;
					}
				}
			}
		}
	}

	// if fast erase was not possible ...
	if (bFallBack)
	{
		position_t				sPos;
		size_type				nPos = rCIterPos.get_pos ();
		node_iter_type			nNode;
		sub_node_iter_type		nSubPos;

		this->convert_pos_to_container_pos (this->m_nRootNode, nPos, nNode, nSubPos);

		*m_pRemoveKey = *(this->extract_key (m_pRemoveKey, nNode, nSubPos));

		sPos.pKey = m_pRemoveKey;
		sPos.nInstance = this->get_instancePos (nNode, nSubPos);

		this->remove_from_node (sPos, this->m_nRootNode, 0);
	}

	// iterator reference is used for more than one access ...
	if (bReEvaluate)
	{
		// ... then prepare iterator for next access
		rCIterPos.sync ();
	}
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
bool CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::_find (const _t_key &rKey, typename _t_datalayerproperties::node_iter_type &rnNode, typename _t_datalayerproperties::sub_node_iter_type &rnSub, typename _t_datalayerproperties::size_type &rnPos) const
{
	return (this->find_oneKey (this->m_nRootNode, rKey, rnNode, rnSub, &rnPos));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::_lower_bound (const _t_key &rKey, typename _t_datalayerproperties::node_iter_type &rnNode, typename _t_datalayerproperties::sub_node_iter_type &rnSub, typename _t_datalayerproperties::size_type &rnPos) const
{
	node_iter_type			nRsltNode;
	sub_node_iter_type		nRsltSub;

	if (this->find_oneKey (this->m_nRootNode, rKey, rnNode, rnSub, &rnPos))
	{
		rnPos -= this->find_firstKey (rnNode, rnSub, nRsltNode, nRsltSub);

		rnNode = nRsltNode;
		rnSub = nRsltSub;
	}
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
bool CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::_upper_bound (const _t_key &rKey, typename _t_datalayerproperties::node_iter_type &rnNode, typename _t_datalayerproperties::sub_node_iter_type &rnSub, typename _t_datalayerproperties::size_type &rnPos) const
{
	key_type			sKey;
	bool				bBounce;

	this->get_next_key (rKey, sKey, bBounce);

	if (bBounce)
	{
		rnPos = this->size ();
	}
	else
	{
		CBTreeAssociative_t::_lower_bound (sKey, rnNode, rnSub, rnPos);
	}

	return (bBounce);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
template<class _t_iterator>
typename ::std::pair<_t_iterator, _t_iterator>
	CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::_equal_range
	(
		const key_type &rKey
	) const
{
	typedef typename ::std::pair<_t_iterator, _t_iterator>		local_equal_range_type;
	node_iter_type			nRsltNode;
	sub_node_iter_type		nRsltSub;
	iterator_state_t		sIterState;
	size_type				nPos;

	sIterState.nAssociatedPos = size_type (0);

	if (this->find_oneKey (this->m_nRootNode, rKey, sIterState.nNode, sIterState.nSubPos, &sIterState.nAssociatedPos))
	{
		nPos = sIterState.nAssociatedPos;

		sIterState.nAssociatedPos -= this->find_firstKey (sIterState.nNode, sIterState.nSubPos, nRsltNode, nRsltSub);

		fast_swap (sIterState.nNode, nRsltNode);
		fast_swap (sIterState.nSubPos, nRsltSub);

		_t_iterator		sIterLower (this, sIterState.nAssociatedPos, (void *) &sIterState, this->get_time_stamp (), false);

		sIterState.nAssociatedPos = nPos;

		this->find_nextKey (rKey, nRsltNode, nRsltSub, sIterState.nNode, sIterState.nSubPos, &sIterState.nAssociatedPos);

		_t_iterator		sIterUpper (this, sIterState.nAssociatedPos, (void *) &sIterState, this->get_time_stamp (), false);

		local_equal_range_type	sRetval (sIterLower, sIterUpper);

		return (sRetval);
	}
	else
	{
		_t_iterator		sIter (this, sIterState.nAssociatedPos, (void *) &sIterState, this->get_time_stamp (), false);

		local_equal_range_type	sRetval (sIter, sIter);

		return (sRetval);
	}
}

/*

vbufferAllocate - allocate variable size buffer

This method de-allocates the buffer *pp if that is required and allocates a new buffer
of size of _t_key.

Note:	The intention is to be able to allocate and de-allocate temporary key buffers as soon as
		unload is called and to have them reinstated when needed. This has not been implemented
		yet and therefore vbufferAllocate and vbufferDeallocate seem like overkill.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::vbufferAllocate (_t_key **pp)
{
	if (*pp != NULL)
	{
		vbufferDeallocate (pp);
	}

	*pp = new _t_key;

	BTREE_ASSERT (*pp != NULL, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::vbufferAllocate: insufficient memory");
}

/*

vbufferDeallocate - de-allocate variable size buffer

This method de-allocates the buffer *pp if that is required.

Note:	The intention is to be able to allocate and de-allocate temporary key buffers as soon as
		unload is called and to have them reinstated when needed. This has not been implemented
		yet and therefore vbufferAllocate and vbufferDeallocate seem like overkill.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::vbufferDeallocate (_t_key **pp)
{
	if (pp != NULL)
	{
		if (*pp != NULL)
		{
			delete *pp;
		}

		*pp = NULL;
	}
}

/*

allocateShortLiveKey - allocate short live key

This method tests if the member variable m_bShortLiveKeyInUse is de-asserted, if it is not
then the program terminates, otherwise it asserts m_bShortLiveKeyInUse.

Note:	The intention is to make sure that m_pShortLiveKey is not used in one method and then
		used again in another method which is called by the method before. So, code
		sections were m_pShortLiveKey gets employed are surrounded by allocateShortLiveKey
		and freeShortLiveKey. It has debug purposes only and is not part of the final binary.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::allocateShortLiveKey () const
{
#if defined (_DEBUG)
	BTREE_ASSERT (!(*m_pbShortLiveKeyInUse), "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::allocateShortLiveKey: DEBUG: short live key in use!");

	*m_pbShortLiveKeyInUse = true;
#endif
}

/*

freeShortLiveKey - free short live key

This method tests if the member variable m_bShortLiveKeyInUse is asserted, if it is not
then the program terminates, otherwise it de-asserts m_bShortLiveKeyInUse.

Note:	The intention is to make sure that m_pShortLiveKey is not used in one method and then
		used again in another method which is called by the method before. So, code
		sections were m_pShortLiveKey gets employed are surrounded by allocateShortLiveKey
		and freeShortLiveKey. It has debug purposes only and is not part of the final binary.

*/

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::freeShortLiveKey () const
{
#if defined (_DEBUG)
	BTREE_ASSERT (*(m_pbShortLiveKeyInUse), "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::freeShortLiveKey: DEBUG: short live key was already de-allocated!");

	*m_pbShortLiveKeyInUse = false;
#endif
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
bool CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::show_data (std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const typename _t_datalayerproperties::node_iter_type nNode, const typename _t_datalayerproperties::sub_node_iter_type nSubPos) const
{
	return (true);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
bool CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::show_node (std::ofstream &ofs, const typename _t_datalayerproperties::node_iter_type nNode, const typename _t_datalayerproperties::sub_node_iter_type nSubPos) const
{
	std::stringstream			buf;
	value_type					*psData;
	node_iter_type				nNeightbourNode;
	sub_node_iter_type			nNeightbourSub;
	int							nErrCnt = 0;
	std::stringstream			aszMsg;
	char						*pColorInit = (char *) "";
	char						*pColorExit = (char *) "";
	node_iter_type				nPrevNode;
	sub_node_iter_type			nPrevSub;
	node_iter_type				nNextNode;
	sub_node_iter_type			nNextSub;
	bool						bBounce;
	key_type					sKey;
	key_type					*psNextKey;
	key_type					*psPrevKey;
	key_type					*psKey;

	psData = this->get_data (nNode, nSubPos);

	if (this->is_leaf (nNode))
	{
		if (!this->show_data (ofs, buf, aszMsg, nNode, nSubPos))
		{
			if (!ofs.is_open ())
			{
				return (false);
			}

			nErrCnt++;
		}

		try
		{
			this->move_prev (nNode, nSubPos, nPrevNode, nPrevSub, bBounce);

			if (bBounce == false)
			{
				value_type		*psPrevData = this->get_data (nPrevNode, nPrevSub);
				key_type		sPrevKey;
				key_type		sKey;

				psPrevKey = this->extract_key (&sPrevKey, *psPrevData);
				psKey = this->extract_key (&sKey, *psData);

				if (this->comp (*psPrevKey, *psKey) > 0)
				{
					if (!ofs.is_open ())
					{
						return (false);
					}

					nErrCnt++;

					aszMsg << "<br>prev: integrity error";
				}

				this->move_next (nPrevNode, nPrevSub, nNextNode, nNextSub, bBounce);

				if (bBounce)
				{
					if (!ofs.is_open ())
					{
						return (false);
					}

					nErrCnt++;

					aszMsg << "<br>prev: unexpected bBounce";
				}
				else
				{
					if ((nNextNode != nNode) || (nNextSub != nSubPos))
					{
						if (!ofs.is_open ())
						{
							return (false);
						}

						nErrCnt++;

						aszMsg << "<br>prev: broken link";
					}
				}
			}

			this->allocateShortLiveKey ();
			{
				this->move_prev (nNode, nSubPos, nNeightbourNode, nNeightbourSub, bBounce);

				if (bBounce == false)
				{
					psPrevKey = this->extract_key (*(this->m_ppShortLiveKey), nNeightbourNode, nNeightbourSub);

					psKey = this->extract_key (&sKey, *psData);

					if (this->comp (*psPrevKey, *psKey) == 0)
					{
						if (this->get_instancePos (nNode, nSubPos) != (this->get_instancePos (nNeightbourNode, nNeightbourSub) + 1))
						{
							if (!ofs.is_open ())
							{
								this->freeShortLiveKey ();
								
								return (false);
							}

							nErrCnt++;

							aszMsg << "<br>instance position error!";
						}
					}
				}

				this->move_next (nNode, nSubPos, nNeightbourNode, nNeightbourSub, bBounce);

				if (bBounce == false)
				{
					psPrevKey = this->extract_key (*(this->m_ppShortLiveKey), nNeightbourNode, nNeightbourSub);

					psKey = this->extract_key (&sKey, *psData);

					if (this->comp (*psPrevKey, *psKey) == 0)
					{
						if (this->get_instancePos (nNode, nSubPos) != (this->get_instancePos (nNeightbourNode, nNeightbourSub) - 1))
						{
							if (!ofs.is_open ())
							{
								this->freeShortLiveKey ();
								
								return (false);
							}

							nErrCnt++;

							aszMsg << "<br>instance position error!";
						}
					}
				}
			}
			this->freeShortLiveKey ();
		}
		catch (::std::exception *pE)
		{
			if (!ofs.is_open ())
			{
				return (false);
			}

			nErrCnt++;

			aszMsg << "<br>prev: link walk crashed (" << pE->what () << ")";
		}

		try
		{
			this->move_next (nNode, nSubPos, nNextNode, nNextSub, bBounce);

			if (bBounce == false)
			{
				value_type		*psNextData = this->get_data (nNextNode, nNextSub);
				key_type		sNextKey;
				key_type		sKey;

				psNextKey = this->extract_key (&sNextKey, *psNextData);
				psKey = this->extract_key (&sKey, *psData);

				if (this->comp (*psNextKey, *psKey) < 0)
				{
					if (!ofs.is_open ())
					{
						return (false);
					}

					nErrCnt++;

					aszMsg << "<br>next: integrity error";
				}

				this->move_prev (nNextNode, nNextSub, nPrevNode, nPrevSub, bBounce);

				if (bBounce)
				{
					if (!ofs.is_open ())
					{
						return (false);
					}

					nErrCnt++;

					aszMsg << "<br>next: unexpected bBounce";
				}
				else
				{
					if ((nPrevNode != nNode) || (nPrevSub != nSubPos))
					{
						if (!ofs.is_open ())
						{
							return (false);
						}

						nErrCnt++;

						aszMsg << "<br>next: broken link";
					}
				}
			}
		}
		catch (::std::exception *pE)
		{
			if (!ofs.is_open ())
			{
				return (false);
			}

			nErrCnt++;

			aszMsg << "<br>next: link walk crashed (" << pE->what () << ")";
		}

		if (nErrCnt == 0)
		{
			pColorInit = (char *) "";
			pColorExit = (char *) "";
		}
		else
		{
			pColorInit = (char *) "<font color=\"#FF0000\">";
			pColorExit = (char *) "</font>";
		}
		
		if (ofs.is_open ())
		{
			ofs << pColorInit << buf.str ().c_str () << pColorExit << aszMsg.str ().c_str ();
		}
	}
	else
	{
		if (!this->show_data (ofs, buf, aszMsg, nNode, nSubPos))
		{
			if (!ofs.is_open ())
			{
				return (false);
			}

			nErrCnt++;
		}

		if (ofs.is_open ())
		{
			ofs << buf.str ().c_str () << ::std::endl;
		}
	}

	return (true);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::_swap
	(
		CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties> &rContainer
	)
{
	fast_swap (m_ppShortLiveKey, rContainer.m_ppShortLiveKey);
	fast_swap (m_ppTempFindFirstKeyKey, rContainer.m_ppTempFindFirstKeyKey);
	fast_swap (m_pRemoveKey, rContainer.m_pRemoveKey);
	fast_swap (m_pTempRemoveKey, rContainer.m_pTempRemoveKey);
	fast_swap (m_pInstancesNewKey, rContainer.m_pInstancesNewKey);
	fast_swap (m_pTempFindFirstKeyNewKey, rContainer.m_pTempFindFirstKeyNewKey);
	fast_swap (m_pAddToNodeKey, rContainer.m_pAddToNodeKey);
	fast_swap (m_pGetNewKey, rContainer.m_pGetNewKey);

#if defined (_DEBUG)

	fast_swap <bool *> (m_pbShortLiveKeyInUse, rContainer.m_pbShortLiveKeyInUse);

#endif

	CBTreeBaseDefaults_t	&rKeySortBase = dynamic_cast <CBTreeBaseDefaults_t &> (rContainer);

	CBTreeBaseDefaults_t::_swap (rKeySortBase);
}

#endif // BTREEASSOCIATIVE_CPP
