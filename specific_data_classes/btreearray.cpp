/************************************************************
**
** file:	btreearray.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's array data class.
**
************************************************************/

#ifndef	BTREEARRAY_CPP
#define	BTREEARRAY_CPP

#include "btreearray.h"

template<class _t_data, class _t_datalayerproperties>
CBTreeArray<_t_data, _t_datalayerproperties>::CBTreeArray
	(
		_t_datalayerproperties &rDataLayerProperties, 
		typename _t_datalayerproperties::sub_node_iter_type nNodeSize
	)
	:	CBTreeBaseDefaults <CBTreeArrayPos<typename _t_datalayerproperties::size_type>, _t_data, _t_datalayerproperties>
	(
		rDataLayerProperties, 
		nNodeSize
	)
	,	CBTreeArrayIf <_t_data, typename _t_datalayerproperties::size_type> ()
	,	m_pClAccessWrapper (NULL)
	,	m_psReturnData (NULL)
{
	CBTreeArrayIf_t	&rBtrArray = dynamic_cast <CBTreeArrayIf_t &> (*this);

	m_pClAccessWrapper = new CBTreeArrayAccessWrapper<value_type, typename _t_datalayerproperties::size_type> (rBtrArray);

	BTREE_ASSERT (m_pClAccessWrapper != NULL, "CBTreeArray<_t_data, _t_datalayerproperties>::CBTreeArray (_t_datalayerproperties &, sub_node_iter_type): insufficient memory!");

	m_psReturnData = new value_type ();

	BTREE_ASSERT (m_psReturnData != NULL, "CBTreeArray<_t_data, _t_datalayerproperties>::CBTreeArray (_t_datalayerproperties &, sub_node_iter_type): insufficient memory!");
}
						
template<class _t_data, class _t_datalayerproperties>
CBTreeArray<_t_data, _t_datalayerproperties>::CBTreeArray (const CBTreeArray<_t_data, _t_datalayerproperties> &rBT, bool bAssign)
	:	CBTreeBaseDefaults<CBTreeArrayPos<typename _t_datalayerproperties::size_type>, _t_data, _t_datalayerproperties>
	(
		dynamic_cast <const CBTreeBaseDefaults<CBTreeArrayPos<typename _t_datalayerproperties::size_type>, _t_data, _t_datalayerproperties> &> (rBT)
	)
	,	CBTreeArrayIf <_t_data, typename _t_datalayerproperties::size_type> ()
	,	m_pClAccessWrapper (NULL)
	,	m_psReturnData (NULL)
{
	CBTreeArrayIf_t	&rBtrArray = dynamic_cast <CBTreeArrayIf_t &> (*this);

	m_pClAccessWrapper = new CBTreeArrayAccessWrapper<value_type, typename _t_datalayerproperties::size_type> (rBtrArray);

	BTREE_ASSERT (m_pClAccessWrapper != NULL, "CBTreeArray<_t_data, _t_datalayerproperties>::CBTreeArray (CBTreeArray<> &, bool): insufficient memory!");

	m_psReturnData = new value_type ();

	BTREE_ASSERT (m_psReturnData != NULL, "CBTreeArray<_t_data, _t_datalayerproperties>::CBTreeArray (CBTreeArray<> &, bool): insufficient memory!");
	
	if (bAssign)
	{
		_assign (rBT);
	}
}

template<class _t_data, class _t_datalayerproperties>
CBTreeArray<_t_data, _t_datalayerproperties>::CBTreeArray
	(CBTreeArray<_t_data, _t_datalayerproperties> &&rRhBT)
	:	CBTreeBaseDefaults<CBTreeArrayPos<typename _t_datalayerproperties::size_type>, _t_data, _t_datalayerproperties>
	(
		dynamic_cast <CBTreeBaseDefaults<CBTreeArrayPos<typename _t_datalayerproperties::size_type>, _t_data, _t_datalayerproperties> &&> (rRhBT)
	)
	,	CBTreeArrayIf <_t_data, typename _t_datalayerproperties::size_type> ()
	,	m_pClAccessWrapper (NULL)
	,	m_psReturnData (NULL)
{
	CBTreeArray_t::swap (&rRhBT);
}

template<class _t_data, class _t_datalayerproperties>
CBTreeArray<_t_data, _t_datalayerproperties>::~CBTreeArray ()
{
	if (m_psReturnData != NULL)
	{
		delete m_psReturnData;
	}
	
	if (m_pClAccessWrapper != NULL)
	{
		delete m_pClAccessWrapper;
	}
}

template<class _t_data, class _t_datalayerproperties>
template<class _t_iterator>
void CBTreeArray<_t_data, _t_datalayerproperties>::assign (_t_iterator sItFirst, _t_iterator sItLast)
{
	this->create_root ();

	size_type	nDiff;
	size_type	nAppendix;
	bool		bSelfReverse;
	bool		bSelfReference = test_self_reference_of_iterator_to_this (sItFirst, sItLast, nDiff, nAppendix, bSelfReverse);

	if (bSelfReference)
	{
		if (bSelfReverse)
		{
			nAppendix = this->size () - nAppendix - nDiff;
		}

		size_type		i;
		size_type		nPos = this->size () - nAppendix;
		const_iterator	sCIterStart;
		const_iterator	sCIterEnd;

		if (this->size () > nPos)
		{
			sCIterEnd = this->cend ();

			sCIterStart = sCIterEnd;

			::std::advance (sCIterStart, 0 - nAppendix);

			CBTreeArray_t::erase (sCIterStart, sCIterEnd);
		}

		nPos -= nDiff;

		if (this->size () > nPos)
		{
			sCIterStart = this->cbegin ();

			sCIterEnd = sCIterStart;

			::std::advance (sCIterEnd, nPos);

			CBTreeArray_t::erase (sCIterStart, sCIterEnd);
		}

		if (bSelfReverse)
		{
			iterator			sIter = this->begin ();
			reverse_iterator	sRIter = this->rbegin ();

			value_type			sEntry;

			for (i = 0; i < (nDiff / 2); i++)
			{
				sEntry = *sIter;

				*sIter = (value_type)(*sRIter);

				*sRIter = sEntry;

				sIter++;
				sRIter++;
			}
		}

		return;
	}

	// remove all data in this list
	CBTreeArray_t::clear ();

	CBTreeArray_t::insert<_t_iterator> (this->cbegin (), sItFirst, sItLast);
}

template<class _t_data, class _t_datalayerproperties>
void CBTreeArray<_t_data, _t_datalayerproperties>::assign (typename _t_datalayerproperties::size_type nNewSize, const _t_data& rVal)
{
	// remove all data in this list
	CBTreeArray_t::clear ();

	this->create_root ();

	/*
	**	Although a b-tree guarantees that data is stored balanced; this happens under the assumption that 
	**	data entries are stored and removed in random order. In case data is stored or removed in order, the
	**	tree will become partially unblanced. To prevent this the data is not copied in order, but in a
	**	skewed fashion. The skew width is determined by the target tree's maximum node size (t * 2) and target
	**	tree's future depth (based on the source tree's size) combined, since that ensures an equal distribution
	**	of all data throughout the entire target tree.
	*/
	
	// determine initial part of skew width
	size_type			nSkewWidth = this->get_node_max_sub_nodes ();
	size_type			nEntries = 1;
	size_type			i;
	size_type			j;
	size_type			nOffset;
	size_type			nAverageNodeSize = this->get_node_max_sub_nodes ();
	const_iterator		sCIter;

	nAverageNodeSize *= 3;
	nAverageNodeSize /= 4;

	// combine skew width with eventual target tree depth (log t of n, while t is node size and n is tree size)
	for (nSkewWidth--; nEntries <= nNewSize; nSkewWidth++)
	{
		// on average every new layer allows for t times current tree size more entries
		nEntries *= nAverageNodeSize;
	}

	// copy data using skew width
	for (i = 0; i < nSkewWidth; i++)
	{
		sCIter = this->cbegin ();

		::std::advance (sCIter, i);

		for (j = i, nOffset = 0; j < nNewSize; j += nSkewWidth, nOffset++)
		{
			CBTreeArray_t::insert (sCIter, rVal);

			::std::advance (sCIter, i + 1);
		}
	}
}


template<class _t_data, class _t_datalayerproperties>
void CBTreeArray<_t_data, _t_datalayerproperties>::push_back (const _t_data &rData)
{
	const_iterator			sCIter = this->cend ();
	const_iterator			sCIterRslt;

	sCIterRslt = CBTreeArray_t::insert (sCIter, rData);

#if defined (_DEBUG)

	BTREE_ASSERT (sCIter == sCIterRslt, "CBTreeArray<>::push_back (const value_type &): insert_at failed to add new entry!");

#endif
}

template<class _t_data, class _t_datalayerproperties>
void CBTreeArray<_t_data, _t_datalayerproperties>::pop_back ()
{
	BTREE_ASSERT ((!this->empty ()) , "CBTreeArray<>::pop_back (): method tried to remove one entry on an empty container!");

	const_iterator			sCIter = this->cend ();
	const_iterator			sCIterRslt;

	sCIter--;

	sCIterRslt = CBTreeArray_t::erase (sCIter);

#if defined (_DEBUG)

	BTREE_ASSERT (sCIter == sCIterRslt, "CBTreeArray<>::pop_back (): remove_at failed to remove entry on container not being empty!");

#endif
}

template<class _t_data, class _t_datalayerproperties>
typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator
	CBTreeArray<_t_data, _t_datalayerproperties>::insert
	(
		typename CBTreeArray<_t_data, _t_datalayerproperties>::const_iterator sCIterPos, 
		const _t_data& rData
	)
{
	this->create_root ();

	this->insert_via_reference (sCIterPos, rData, false, false);

	iterator		sRetval (this, false);

	sRetval = this->begin ();

	sRetval += sCIterPos.get_pos ();

	return (sRetval);
}

template<class _t_data, class _t_datalayerproperties>
typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator
	CBTreeArray<_t_data, _t_datalayerproperties>::insert
	(
		typename CBTreeArray<_t_data, _t_datalayerproperties>::const_iterator sCIterPos, 
		const typename _t_datalayerproperties::size_type nLen, 
		const _t_data& rData
	)
{
	this->create_root ();

	size_type		i;
	iterator		sRetval (this, sCIterPos.get_pos (), false);

	for (i = 0; i < nLen; i++)
	{
		this->insert_via_reference (sCIterPos, rData, false, true);
	}

	return (sRetval);
}

template<class _t_data, class _t_datalayerproperties>
template<class _t_iterator>
typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator
	CBTreeArray<_t_data, _t_datalayerproperties>::insert
	(
		typename CBTreeArray<_t_data, _t_datalayerproperties>::const_iterator sCIterPos, 
		_t_iterator sItFirst, 
		_t_iterator sItLast
	)
{
	typedef typename CBTreeArray<_t_data, _t_datalayerproperties>::const_iterator	citer_t;
	typedef typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator			iter_t;

	this->create_root ();

	if (sItFirst == sItLast)
	{
		iter_t		sRetval (this, false);

		sRetval = this->begin ();

		sRetval += sCIterPos.get_pos ();

		return (sRetval);
	}

	size_type	nDiff;
	size_type	nAppendix;
	bool		bSelfReverse;
	bool		bSelfReference = test_self_reference_of_iterator_to_this (sItFirst, sItLast, nDiff, nAppendix, bSelfReverse);

	if (bSelfReference)
	{
		size_type		nBlockSize = ::std::min<size_type> (8192, nDiff);
		size_type		nAllocSize = nBlockSize;
		value_type		*pBlock;
		size_type		i;
		size_type		j;
		size_type		nStart;
		size_type		nEnd;
		size_type		nPos = sCIterPos.get_pos ();
		size_type		nLenPriorToInsert;
		size_type		nLenPastInsert;
		size_type		nChunkSize;
		citer_t			sCIterTarget = sCIterPos;

		nStart = this->size () - nDiff - nAppendix;
		nEnd = this->size () - nAppendix;

		if (bSelfReverse)
		{
			nStart = this->size () - nStart;
			nEnd = this->size () - nEnd;

			nStart = nEnd ^ nStart;
			nEnd = nEnd ^ nStart;
			nStart = nEnd ^ nStart;
		}
		
		if ((nStart < nPos) && (nEnd < nPos))
		{
			nLenPriorToInsert = nDiff;
			nLenPastInsert = 0;
		}
		else if ((nStart >= nPos) && (nEnd > nPos))
		{
			nLenPriorToInsert = 0;
			nLenPastInsert = nDiff;
		}
		else
		{
			nLenPriorToInsert = nPos - nStart;
			nLenPastInsert = nEnd - nPos;
		}

		do
		{
			nBlockSize = nAllocSize;

			pBlock = (value_type *) calloc ((size_t) nAllocSize, sizeof (value_type));

			nAllocSize /= 2;

			BTREE_ASSERT (nAllocSize > 0, "CBTreeArray<>::insert<_t_iterator> (const_iterator, _t_iterator, _t_iterator): insufficient memory!");
		} while (pBlock == NULL);

		if (!bSelfReverse)
		{
			for (i = 0; i < nLenPriorToInsert; i += nChunkSize)
			{
				nChunkSize = ::std::min<size_type> (nBlockSize, nLenPriorToInsert - i);

				this->serialize (nStart + i, nChunkSize, pBlock);

				for (j = nChunkSize; j > 0;)
				{
					j--;

					this->insert_via_reference (sCIterTarget, pBlock[j], false);
				}

				sCIterTarget += nChunkSize;
			}

			nLenPriorToInsert *= 2;

			for (i = 0; i < nLenPastInsert; i += nChunkSize)
			{
				nChunkSize = ::std::min<size_type> (nBlockSize, nLenPastInsert - i);

				this->serialize (nLenPriorToInsert + nStart + i, nChunkSize, pBlock);

				for (j = nChunkSize; j > 0;)
				{
					j--;

					this->insert_via_reference (sCIterTarget, pBlock[j], false);
				}

				nLenPriorToInsert += nChunkSize;
				sCIterTarget += nChunkSize;
			}
		}
		else
		{
			for (i = 0; i < nLenPriorToInsert; i += nChunkSize)
			{
				nChunkSize = ::std::min<size_type> (nBlockSize, nLenPriorToInsert - i);

				this->serialize (nStart + i, nChunkSize, pBlock);

				for (j = 0; j < nChunkSize; j++)
				{
					this->insert_via_reference (sCIterTarget, pBlock[j], false);
				}
			}

			nLenPriorToInsert *= 2;

			for (i = 0; i < nLenPastInsert; i += nChunkSize)
			{
				nChunkSize = ::std::min<size_type> (nBlockSize, nLenPastInsert - i);

				this->serialize (nLenPriorToInsert + nStart + i, nChunkSize, pBlock);

				for (j = 0; j < nChunkSize; j++)
				{
					this->insert_via_reference (sCIterTarget, pBlock[j], false);
				}

				nLenPriorToInsert += nChunkSize;
			}
		}

		free ((void *) pBlock);

		iter_t		sRetval (this, false);

		sRetval = this->begin ();

		sRetval += sCIterPos.get_pos ();

		return (sRetval);
	}
	else
	{
		typename::std::iterator_traits<_t_iterator>::iterator_category	sIterCategory;

		return (this->insert_via_iterator_tag (sCIterPos, sItFirst, sItLast, sIterCategory));
	}
}

template<class _t_data, class _t_datalayerproperties>
typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator
	CBTreeArray<_t_data, _t_datalayerproperties>::emplace
	(
		typename CBTreeArray<_t_data, _t_datalayerproperties>::const_iterator sCIterPos, 
		typename CBTreeArray<_t_data, _t_datalayerproperties>::value_type &&rData
	)
{
	return (this->template emplace<value_type &&> (sCIterPos, ::std::forward<value_type> (rData)));
}

template<class _t_data, class _t_datalayerproperties>
typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator
	CBTreeArray<_t_data, _t_datalayerproperties>::emplace_back
	(
		typename CBTreeArray<_t_data, _t_datalayerproperties>::value_type &&rData
	)
{
	return (this->template emplace_back<value_type &&> (::std::forward<value_type> (rData)));
}

template<class _t_data, class _t_datalayerproperties>
template<class ..._t_va_args>
typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator
	CBTreeArray<_t_data, _t_datalayerproperties>::emplace
	(
		typename CBTreeArray<_t_data, _t_datalayerproperties>::const_iterator sCIterPos, 
		_t_va_args && ... rrArgs
	)
{
	iterator_state_t			sIterState;
	iterator_state_t			*psIterState;
	position_t					sPos;
	bool						bFallBack = true;
	sub_node_iter_type			nSubPos;
	
	this->create_root ();

	if (sCIterPos.is_evaluated ())
	{
		psIterState = (iterator_state_t *) sCIterPos.get_iterator_state ();

		if (this->is_leaf (psIterState->nNode))
		{
			sPos.nIndex = size_type (psIterState->nSubPos);

			nSubPos = this->find_next_sub_pos (psIterState->nNode, sPos);

			size_type		nDiff = size_type (nSubPos - psIterState->nSubPos);

			::std::advance (sCIterPos, nDiff);

			bFallBack = this->insert_via_iterator (sCIterPos, sIterState.nNode, sIterState.nSubPos);
		}
	}

	if (bFallBack)
	{
		sPos.nIndex = sCIterPos.get_pos ();

		size_type	nRetval = this->add_to_node (sPos, this->m_nRootNode, 0, sIterState.nNode, sIterState.nSubPos, &sIterState.nAssociatedPos);

		BTREE_ASSERT (nRetval == 1, "CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>::emplace_hint (const_iterator, _t_va_args && ...): Failed to create new entry!");
	}
	else
	{
		sIterState.nAssociatedPos = psIterState->nAssociatedPos;
	}

	value_type	*psData = this->get_data (sIterState.nNode, sIterState.nSubPos);

	new (psData) value_type (::std::forward<_t_va_args> (rrArgs) ...);

	iterator		sRetval (this, sIterState.nAssociatedPos, &sIterState, this->get_time_stamp (), false);

	return (sRetval);
}

template<class _t_data, class _t_datalayerproperties>
template<class ..._t_va_args>
typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator
	CBTreeArray<_t_data, _t_datalayerproperties>::emplace_back
	(
		_t_va_args && ... rrArgs
	)
{
	const_iterator		sCIterEnd = this->cend ();

	return (this->emplace (sCIterEnd, ::std::forward<_t_va_args> (rrArgs) ...));
}

template<class _t_data, class _t_datalayerproperties>
typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator
	CBTreeArray<_t_data, _t_datalayerproperties>::erase
	(
		typename CBTreeArray<_t_data, _t_datalayerproperties>::const_iterator sCIterPos
	)
{
	typedef typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator			iter_t;

	this->erase_via_reference (sCIterPos, false);

	iter_t		sRetval (this, false);

	sRetval = this->begin ();

	sRetval += sCIterPos.get_pos ();

	return (sRetval);
}

template<class _t_data, class _t_datalayerproperties>
typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator
	CBTreeArray<_t_data, _t_datalayerproperties>::erase
	(
		typename CBTreeArray<_t_data, _t_datalayerproperties>::const_iterator sCIterFirst,
		typename CBTreeArray<_t_data, _t_datalayerproperties>::const_iterator sCIterLast
	)
{
	typedef typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator			iter_t;

	iter_t		sRetval (this, false);

	sRetval = this->begin ();

	sRetval += sCIterFirst.get_pos ();

	if (sCIterFirst == sCIterLast)
	{
		return (sRetval);
	}

	size_type		nDiff = sCIterLast - sCIterFirst;
	size_type		i;

	for (i = 0; i < nDiff; i++)
	{
		this->erase_via_reference (sCIterFirst);
	}

	return (sRetval);
}

template<class _t_data, class _t_datalayerproperties>
void CBTreeArray<_t_data, _t_datalayerproperties>::swap (CBTreeArrayIf<_t_data, typename _t_datalayerproperties::size_type> &rArrayIf)
{
	CBTreeArray_t		&rArray = dynamic_cast <CBTreeArray_t &> (rArrayIf);

	this->swap (rArray);
}

template<class _t_data, class _t_datalayerproperties>
void CBTreeArray<_t_data, _t_datalayerproperties>::swap (CBTreeArray<_t_data, _t_datalayerproperties> &rArray)
{
	if (this != &rArray)
	{
		this->_swap (rArray);
	}
}

/*

clear - remove all

This method removes all data by dstroying the data layer.

If successfully this method returns true, otherwise false.

*/
template<class _t_data, class _t_datalayerproperties>
void CBTreeArray<_t_data, _t_datalayerproperties>::clear ()
{
	CBTreeBaseDefaults_t::clear ();
}

/*

serialize - serialize

The serialize method reads out data items from position nFrom to position nTo - 1 and writes the
return data into an array pData is poiting at. It is assumed by this method that the array is large enough
to contain all data that will potentially be written into it, otherwise memory regions not associated with
pData get overwritten.

nStart		- specifies position of initial entry to be serialized
nLen		- specifies number of entries to be serialized
pData		- pointer to array where the serialized data will be copied to

This method returns the number of items that have been written to the array pData is pointing at.

*/

template<class _t_data, class _t_datalayerproperties>
typename _t_datalayerproperties::size_type CBTreeArray<_t_data, _t_datalayerproperties>::serialize (const typename _t_datalayerproperties::size_type nStart, const typename _t_datalayerproperties::size_type nLen, _t_data *pData) const
{
	position_t		sPos;

	/*
	**	The actual serialize process is done by CBTreeBaseDefaults::serialize.
	**	All this method does is to convert nFrom and nTo into the template parameter _ti_pos (CBTreeArrayPos <size_type>)
	**	and nLen. This has to be done, since CBTreeBaseDefaults::serialize has no concept of what an size_type in terms of a position is.
	*/

	sPos.nIndex = nStart;

	return (CBTreeBaseDefaults_t::serialize (sPos, nLen, pData));
}

/*

operator= - assignment operator overload

rBT		- is a reference to a b-tree that will have its content be assigned to this instance

The result is a reference to this instance.

*/

template<class _t_data, class _t_datalayerproperties>
CBTreeArray<_t_data, _t_datalayerproperties> &CBTreeArray<_t_data, _t_datalayerproperties>::operator=
	(const CBTreeArray<_t_data, _t_datalayerproperties> &rBT)
{
	// if this is not the same instance as the referenced b-tree (rBT) ...
	if (this != &rBT)
	{
		// ... then destroy this data layer
		CBTreeArray_t::clear ();

		// and copy all data from rBT to this instance
		CBTreeArray_t::_assign (rBT);
	}

	return (*this);
}

template<class _t_data, class _t_datalayerproperties>
CBTreeArray<_t_data, _t_datalayerproperties> &CBTreeArray<_t_data, _t_datalayerproperties>::operator=
	(CBTreeArray<_t_data, _t_datalayerproperties> &&rRhBT)
{
	// if this is not the same instance as the referenced b-tree (rBT) ...
	if (this != &&rRhBT)
	{
		// ... then destroy this data layer
		this->clear ();

		// and swap all data from reference of right-hand container with this instance
		swap (&rRhBT);
	}

	return (*this);
}

/*

operator[] - subscription operator overload (write)

nPos	- specifies the linear position of the entry to be written to

The result is an instance to CBTreeArrayAccessWrapper, which will manage the write.

Note:	The class CBTreeArrayAccessWrapper has a cast operator, which cast CBTreeArrayAccessWrapper
		to value_type. This way the compiler can distinguish between read and write
		accesses, when subscriptions are employed.

*/

template<class _t_data, class _t_datalayerproperties>
CBTreeArrayAccessWrapper<_t_data, typename _t_datalayerproperties::size_type>
	CBTreeArray<_t_data, _t_datalayerproperties>::operator[] (const typename _t_datalayerproperties::size_type nPos)
{
	BTREE_ASSERT_EXCEPT (nPos < this->size (), ::std::out_of_range, "CBTreeArray<>::operator[] (const size_type): nPos exceeds range!");

	m_pClAccessWrapper->set (nPos);

	return (*m_pClAccessWrapper);
}

template<class _t_data, class _t_datalayerproperties>
const _t_data& CBTreeArray<_t_data, _t_datalayerproperties>::operator[] (const typename _t_datalayerproperties::size_type nPos) const
{
	BTREE_ASSERT_EXCEPT (nPos < this->size (), ::std::out_of_range, "CBTreeArray<>::operator[] (const size_type) const: nPos exceeds range!");

	bool	bRslt = CBTreeArray_t::get_at (nPos, *m_psReturnData);

#if defined (_DEBUG)

	BTREE_ASSERT (bRslt, "CBTreeArray<>::operator[] (const size_type) const: get_at failed!");

#endif

	return (*m_psReturnData);
}

template<class _t_data, class _t_datalayerproperties>
bool CBTreeArray<_t_data, _t_datalayerproperties>::operator==
	(
		const CBTreeArrayIf<_t_data, typename _t_datalayerproperties::size_type> &rRhs
	) const
{
	if (this == &rRhs)
	{
		return (true);
	}

	if (this->size () != rRhs.size ())
	{
		return (false);
	}

	const_iterator		sCIterBegin;
	const_iterator		sCIterEnd;
	const_iterator		sCIterLhs;
	const_iterator		sCIterRhs;

	sCIterBegin = this->cbegin ();
	sCIterEnd = this->cend ();

	sCIterRhs = rRhs.cbegin ();

	for (sCIterLhs = sCIterBegin; sCIterLhs < sCIterEnd; sCIterLhs++, sCIterRhs++)
	{
		typename has_compare_operator_equal<value_type>::type		sSelectCompareOperatorPresent;

		if (!this->compare_value_type ((value_type) *sCIterLhs, (value_type) *sCIterRhs, sSelectCompareOperatorPresent))
		{
			return (false);
		}
	}

	return (true);
}

template<class _t_data, class _t_datalayerproperties>
bool CBTreeArray<_t_data, _t_datalayerproperties>::operator!=
	(
		const CBTreeArrayIf<_t_data, typename _t_datalayerproperties::size_type> &rRhs
	) const
{
	return (! (*this == rRhs) );
}

template<class _t_data, class _t_datalayerproperties>
void CBTreeArray<_t_data, _t_datalayerproperties>::_assign
	(const CBTreeIf<_t_data, typename _t_datalayerproperties::size_type> &rContainer)
{
	// remove all data from container
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
	size_type				nSkewWidth = this->get_node_max_sub_nodes ();
	size_type				nEntries = 1;
	size_type				i;
	value_type				sData;
	size_type				nAverageNodeSize = this->get_node_max_sub_nodes ();
	const_iterator			sCIter;
	const_iterator			sCIterSrc;
	const_iterator			sCIterSrcEnd;

	nAverageNodeSize *= 3;
	nAverageNodeSize /= 4;

	// combine skew width with eventual target tree depth (log t of n, while t is node size and n is tree size)
	for (nSkewWidth--; nEntries <= rContainer.size (); nSkewWidth++)
	{
		// on average every new layer allows for t times current tree size more entries
		nEntries *= nAverageNodeSize;
	}

	sCIterSrcEnd = rContainer.cend ();

	// copy data using skew width
	for (i = 0; i < nSkewWidth; i++)
	{
		sCIter = this->cbegin ();

		::std::advance (sCIter, i);

		sCIterSrc = rContainer.cbegin ();

		for (::std::advance (sCIterSrc, i); sCIterSrc < sCIterSrcEnd; ::std::advance (sCIterSrc, nSkewWidth))
		{
			sData = *sCIterSrc;

			this->insert (sCIter, sData);

			::std::advance (sCIter, i + 1);
		}
	}
}

/*

set_at - set data at specified position

nPos		- specifies the position of the entry to be set with new data
rData		- contains the data to replace the entry data at the specified position

The entry at the specified position gets replaced with the data of the parameter obj.

If successful true is returned, otherwise false.

*/

template<class _t_data, class _t_datalayerproperties>
bool CBTreeArray<_t_data, _t_datalayerproperties>::set_at (const typename _t_datalayerproperties::size_type nPos, const _t_data &rData)
{
	bool				retval = false;
	node_iter_type		nNode;
	sub_node_iter_type	nSubPos;
	value_type			*psData;
	
	this->create_root ();

	if (nPos < this->size ())
	{
		this->convert_pos_to_container_pos (this->m_nRootNode, nPos, nNode, nSubPos);

		psData = this->get_data (nNode, nSubPos);

		*psData = rData;

		retval = true;
	}
	
	return (retval);
}

/*

get_at - retrieves data from specified position

nPos			- specifies the position of the entry to be retrieved

The entry at the specified position is returned.

If successful true is returned, otherwise false.

*/

template<class _t_data, class _t_datalayerproperties>
bool CBTreeArray<_t_data, _t_datalayerproperties>::get_at (const typename _t_datalayerproperties::size_type nPos, _t_data &rData) const
{
	bool				retval = false;
	node_iter_type		nNode;
	sub_node_iter_type	nSubPos;

	if (!this->empty ())
	{
		if (nPos < this->size ())
		{
			this->convert_pos_to_container_pos (this->m_nRootNode, nPos, nNode, nSubPos);

			rData = *(this->get_data (nNode, nSubPos));

			retval = true;
		}
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

template<class _t_data, class _t_datalayerproperties>
CBTreeArrayPos<typename _t_datalayerproperties::size_type> CBTreeArray<_t_data, _t_datalayerproperties>::determine_position (CBTreeArrayPos<typename _t_datalayerproperties::size_type> sPos, typename _t_datalayerproperties::node_iter_type nNode, typename _t_datalayerproperties::sub_node_iter_type &nSubPos, typename _t_datalayerproperties::sub_node_iter_type &nSubData, bool &bFound) const
{
	size_type			nMaxIdx;
	node_iter_type		*pnNode;
	node_t				*psNodeDesc;

	if (this->is_leaf (nNode))
	{
		nSubData = (sub_node_iter_type) sPos.nIndex;

		bFound = true;
	}
	else
	{
		bFound = false;

		psNodeDesc = this->get_node (nNode);

		// determine which subnode (nSubPos) is to enter
		// sPos will return as the number of data items before the selected sub node, including data from this node
		nSubData = nSubPos = this->find_next_sub_pos (nNode, sPos); // <- note: sPos is an input/output parameter

		// determine size of sub node
		pnNode = this->get_sub_node (nNode, nSubPos);

		nMaxIdx = this->get_max_index (*pnNode);

#if defined (_DEBUG)
		BTREE_ASSERT (sPos.nIndex <= nMaxIdx, "CBTreeArray<>::determine_position: position exceeds sub node size");
#endif

		// if the offset inside the next sub node exceeds the next sub node's size by one ...
		if (sPos.nIndex == nMaxIdx)
		{
			// ... then the data to be deleted exists in the current node
			bFound = true;
		}

		// if select sub-node is outside of the current node ...
		if (nSubPos >= this->get_data_count (*psNodeDesc))
		{
			// ... then correct to most right sub-node
			nSubData--;
		}
	}

	return (sPos);
}

/*

rebuild_node - update maximum index

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

template<class _t_data, class _t_datalayerproperties>
void CBTreeArray<_t_data, _t_datalayerproperties>::rebuild_node (const typename _t_datalayerproperties::node_iter_type nNode, const int32_t triMod, typename _t_datalayerproperties::sub_node_iter_type nSubStart)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeArray<_t_data, _t_datalayerproperties>::rebuild_node: requested node exceeds tree size");
#endif

	node_iter_type				*pnSubNode;
	node_t						*psNodeDesc;
	size_type					nIdx = (size_type) 0;
	sub_node_iter_type			ui32;
	sub_node_iter_type			nSubPosOffset;
	size_type					*pnSerVector;

	psNodeDesc = this->get_node (nNode);

	if (this->is_leaf (nNode))
	{
		nIdx = ((sub_node_iter_type) (0 - psNodeDesc->nNumData));
	}
	else
	{
		pnSerVector = this->get_serVector (nNode);

		if (nSubStart == 0)
		{
			nSubPosOffset = 0;
			nIdx = 0;
		}
		else
		{
			nSubPosOffset = nSubStart - 1;
			nIdx = pnSerVector[nSubPosOffset] - nSubPosOffset;
		}

		this->m_pData->set_cacheFreeze (true);
		{
			for (ui32 = nSubStart; ui32 < this->get_sub_node_count (*psNodeDesc); ui32++)
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

	psNodeDesc->nMaxIdx = nIdx;
}

/*

generate_prev_position - generate previous position

This method returns the previous position (CBTreeBaseDefaults::_ti_pos = CBTreeArrayPos<size_type>)
based on the parameter sPos and the node / sub-node tuple associated with sPos.

nNode		- specifies the node of the node / sub-position tuple of which the previous position will be generated from
nSub		- specifies the sub_position of the node / sub-position tuple of which the previous position will be generated from
sPos		- class specific position associated with node / sub-node tuple

The return value is the previous linear position of sPos (nNode, nSub) in terms of how data is
presented to the application. Given that the b-tree classes are supposed to act like a linear
list to calling the application, this pseudo code displays what generate_prev_position does:

return_value = (*this)[sPos (this, nNode, nSub) - 1];

*/

template<class _t_data, class _t_datalayerproperties>
CBTreeArrayPos<typename _t_datalayerproperties::size_type> CBTreeArray<_t_data, _t_datalayerproperties>::generate_prev_position (const typename _t_datalayerproperties::node_iter_type nNode, const typename _t_datalayerproperties::sub_node_iter_type nSub, CBTreeArrayPos<typename _t_datalayerproperties::size_type> sPos)
{
#if defined (_DEBUG)
	BTREE_ASSERT (!this->is_leaf (nNode), "CBTreeArray<_t_data, _t_datalayerproperties>::generate_prev_position: This method cannot be used on leaf nodes!");
	BTREE_ASSERT (sPos.nIndex > 0, "CBTreeArray<_t_data, _t_datalayerproperties>::generate_prev_position: Cannot generate previous position from position 0!");
#endif

	sPos.nIndex--;

	return (sPos);
}

/*

generate_next_position - generate next position

This method returns the next position (CBTreeBaseDefaults::_ti_pos = CBTreeArrayPos<size_type>)
based on the parameter sPos and the node / sub-node tuple associated with sPos.

nNode		- specifies the node of the node / sub-position tuple of which the next position will be generated from
nSub		- specifies the sub_position of the node / sub-position tuple of which the next position will be generated from
sPos		- class specific position associated with node / sub-node tuple

The return value is the next linear position of sPos (nNode, nSub) in terms of how data is
presented to the application. Given that the b-tree classes are supposed to act like a linear
list to calling the application, this pseudo code displays what generate_prev_position does:

return_value = (*this)[sPos (this, nNode, nSub) + 1];

*/

template<class _t_data, class _t_datalayerproperties>
CBTreeArrayPos<typename _t_datalayerproperties::size_type> CBTreeArray<_t_data, _t_datalayerproperties>::generate_next_position (typename _t_datalayerproperties::node_iter_type nNode, typename _t_datalayerproperties::sub_node_iter_type nSub, CBTreeArrayPos<typename _t_datalayerproperties::size_type> sPos)
{
#if defined (_DEBUG)
	BTREE_ASSERT (!this->is_leaf (nNode), "CBTreeArray<_t_data, _t_datalayerproperties>::generate_next_position: This method cannot be used on leaf nodes!");
#endif

	sPos.nIndex++;

	return (sPos);
}

/*

find_next_sub_pos - find next sub-position

To find the sub-position of the nearest greater offset of what sPos is displaying
within a node, this method ought to be used.

nNode		- specifies the node in which the nearest greater offset is to be sought in
sPos		- specifes the offset

The value returned is the sub-position, which has the nearest greater offset displayed by sPos.

*/

template<class _t_data, class _t_datalayerproperties>
typename _t_datalayerproperties::sub_node_iter_type CBTreeArray<_t_data, _t_datalayerproperties>::find_next_sub_pos (const typename _t_datalayerproperties::node_iter_type nNode, CBTreeArrayPos<typename _t_datalayerproperties::size_type> &sPos) const
{
	// sPos is an input output parameter
	// input	- absolute index within this node
	// output	- if sub node was found then index of sub node, otherwise zero

	node_t					*psNodeDesc;
	sub_node_iter_type		nRetval;
	size_type				nDiff;
	size_type				*pnSerVector;
	uint8_t					*pDiff = (uint8_t *) &nDiff;

	pDiff += sizeof (nDiff) - 1;

	// node is a leave node ...
	if (this->is_leaf (nNode))
	{
#if defined (_DEBUG)

		psNodeDesc = this->get_node (nNode);

		BTREE_ASSERT (sPos.nIndex <= this->get_data_count (*psNodeDesc), "CBTreeArray<_t_data, _t_datalayerproperties>::find_next_sub_pos (): linear position exceeds node's data size! (root node)");

#endif

		// ... then return immediately with input index
		nRetval = (sub_node_iter_type) sPos.nIndex;

		sPos.nIndex = (size_type) 0;
	}
	else
	{
		psNodeDesc = this->get_node (nNode);

		// load node's serial vector
		pnSerVector = this->get_serVector (nNode);

		sub_node_iter_type	ui32, nMinPos, nMaxPos;

		// set range up
		nMinPos = 0;

		nMaxPos = this->get_data_count (*psNodeDesc);

		// until nearest sub postion has been found ...
		while (nMinPos != nMaxPos)
		{
			// sub divide range and determine average middle position
			ui32 = (nMinPos + nMaxPos) / 2;

			// determine difference between current middle position's index and input index
			nDiff = pnSerVector[ui32] - sPos.nIndex;

			// if current middle position's index is beyond input index ...
			if ((*pDiff) & 0x80)
			{
				// ... then range minimum assumes next position after middle position
				nMinPos = ui32 + 1;
			}
			else
			{
				// ... otherwise range maximum assumes middle position
				nMaxPos = ui32;
			}
		}

		nRetval = nMinPos;

		// if returned sub position is not the initial position ...
		if (nMinPos > 0)
		{
			// ... then subtract sub position's serial vector entry from input index
			sPos.nIndex -= pnSerVector[nMinPos - 1] + 1;

			// Note: The initial sub node's position in the serial vector list would always be zero and is therefore not part of the serial vector.
		}
	}

	return (nRetval);
}

template<class _t_data, class _t_datalayerproperties>
bool CBTreeArray<_t_data, _t_datalayerproperties>::show_data (std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const typename _t_datalayerproperties::node_iter_type nNode, const typename _t_datalayerproperties::sub_node_iter_type nSubPos) const
{
	return (true);
}

template<class _t_data, class _t_datalayerproperties>
bool CBTreeArray<_t_data, _t_datalayerproperties>::show_node (std::ofstream &ofs, const typename _t_datalayerproperties::node_iter_type nNode, const typename _t_datalayerproperties::sub_node_iter_type nSubPos) const
{
	std::stringstream		buf;
	std::stringstream		aszMsg;

	if (!this->show_data (ofs, buf, aszMsg, nNode, nSubPos))
	{
		if (!ofs.is_open ())
		{
			return (false);
		}
	}

	if (ofs.is_open ())
	{
		ofs << buf.str ().c_str () << aszMsg.str ().c_str ();
	}
	
	return (true);
}

template<class _t_data, class _t_datalayerproperties>
template<class _t_iterator>
typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator
	CBTreeArray<_t_data, _t_datalayerproperties>::insert_via_iterator_tag
	(
		typename CBTreeArray<_t_data, _t_datalayerproperties>::const_iterator sCIterPos, 
		_t_iterator sItFirst, 
		_t_iterator sItLast, 
		::std::forward_iterator_tag
	)
{
	typedef typename CBTreeArray<_t_data, _t_datalayerproperties>::const_iterator	citer_t;
	typedef typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator			iter_t;

	iter_t				sRetval (this, false);
	citer_t				sCIterTarget (this, false);
	value_type			sData;
	
	_t_iterator			sIt;

	sCIterTarget = sCIterPos;

	for (sIt = sItFirst; sIt != sItLast; sIt++)
	{
		sData = *sIt;

		CBTreeArray_t::insert (sCIterTarget, sData);

		sCIterTarget++;
	}
	
	sRetval = this->begin ();

	sRetval += sCIterPos.get_pos ();

	return (sRetval);
}

template<class _t_data, class _t_datalayerproperties>
template<class _t_iterator>
typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator
	CBTreeArray<_t_data, _t_datalayerproperties>::insert_via_iterator_tag
	(
		typename CBTreeArray<_t_data, _t_datalayerproperties>::const_iterator sCIterPos, 
		_t_iterator sItFirst, 
		_t_iterator sItLast, 
		::std::random_access_iterator_tag
	)
{
	typedef typename CBTreeArray<_t_data, _t_datalayerproperties>::const_iterator	citer_t;
	typedef typename CBTreeArray<_t_data, _t_datalayerproperties>::iterator			iter_t;

	/*
	**	Although a b-tree guarantees that data is stored balanced; this happens under the assumption that 
	**	data entries are stored and removed in random order. In case data is stored or removed in order, the
	**	tree will become partially unblanced. To prevent this the data is not copied in order, but in a
	**	skewed fashion. The skew width is determined by the target tree's maximum node size (t * 2) and target
	**	tree's future depth (based on the source tree's size) combined, since that ensures an equal distribution
	**	of all data throughout the entire target tree.
	*/

	// determine initial part of skew width
	iter_t									sRetval (this, false);
	_t_iterator								sIt;
	citer_t									sCIterTarget (this, false);
	size_type								nNewSize = (size_type) ::std::distance <_t_iterator> (sItFirst, sItLast);
	typename _t_iterator::difference_type	nNewSizeCounterChk = ::std::distance <_t_iterator> (sItFirst, sItLast);
	size_type								nSkewWidth = this->get_node_max_sub_nodes ();
	size_type								nEntries = 1;
	size_type								i;
	size_type								j;
	size_type								nOffset;
	value_type								sData;
	size_type								nAverageNodeSize = this->get_node_max_sub_nodes ();

	BTREE_ASSERT (nNewSize == (size_type) nNewSizeCounterChk, "CBTreeArray<>::insert_via_iterator_tag (::std::random_access_iterator_tag): Insertion distance cannot be displayed by local size type!");
	BTREE_ASSERT ((typename _t_iterator::difference_type) nNewSize == nNewSizeCounterChk, "CBTreeArray<>::insert_via_iterator_tag (::std::random_access_iterator_tag): Insertion distance cannot be displayed by local size type!");

	nAverageNodeSize *= 3;
	nAverageNodeSize /= 4;

	// combine skew width with eventual target tree depth (log t of n, while t is node size and n is tree size)
	for (nSkewWidth--; nEntries <= nNewSize; nSkewWidth++)
	{
		// on average every new layer allows for t times current tree size more entries
		nEntries *= nAverageNodeSize;
	}

	// in extreme cases the skew width may exceed the number of entries to be
	// inserted, which is problematic if the source is an MS ::std::vector<_t_data>
	// 
	// said vector container cannot handle situations in which an iterator exceeds
	// the container's total size
	// 
	// if the skew width is greater than the number of data entries to be inserted ...
	if (nSkewWidth > nNewSize)
	{
		// ... then restrict the skew width
		nSkewWidth = nNewSize;
	}

	// copy data using skew width
	for (i = 0; i < nSkewWidth; i++)
	{
		sIt = sItFirst;

		::std::advance (sIt, (typename _t_iterator::difference_type) i);

		for (j = i, nOffset = 0; j < nNewSize; nOffset++)
		{
			sData = *sIt;

			sCIterTarget = sCIterPos;

			sCIterTarget += i + (nOffset * (i + 1));

			CBTreeArray_t::insert (sCIterTarget, sData);

			j += nSkewWidth;

			if (j < nNewSize)
			{
				// some containers (like ::std::vector) cannot operate outside their range
				::std::advance (sIt, (typename _t_iterator::difference_type) nSkewWidth);
			}
		}
	}

	sRetval = this->begin ();

	sRetval += sCIterPos.get_pos ();

	return (sRetval);
}

template<class _t_data, class _t_datalayerproperties>
void CBTreeArray<_t_data, _t_datalayerproperties>::insert_via_reference (const_iterator &rCIterPos, const _t_data& rData, bool bCopyNotMove, bool bReEvaluate)
{
	node_iter_type					nNode;
	sub_node_iter_type				nSubPos;

	bool				bFallBack = this->insert_via_iterator (rCIterPos, nNode, nSubPos);
	
	// if fast insert was not possible ...
	if (bFallBack)
	{
		// ... then perform actual insert
		position_t						sPos;
		iterator_state_t				*psIterState = (iterator_state_t *) rCIterPos.get_iterator_state ();
	
		this->create_root ();

		sPos.nIndex = rCIterPos.get_pos ();

		this->add_to_node (sPos, this->m_nRootNode, 0, nNode, nSubPos);

		// iterator reference is used for more than one access ...
		if (bReEvaluate)
		{
			// ... then prepare iterator for next access
			psIterState->nNode = nNode;
			psIterState->nSubPos = nSubPos;

			rCIterPos.set_time_stamp (this);
		}
	}

	value_type	*psData = this->get_data (nNode, nSubPos);

	if (bCopyNotMove)
	{
		new (psData) value_type (rData);
	}
	else
	{
		*psData = ::std::move (rData);
	}
}

template<class _t_data, class _t_datalayerproperties>
void CBTreeArray<_t_data, _t_datalayerproperties>::erase_via_reference (const_iterator &rCIterPos, bool bReEvaluate)
{
	bool			bFallBack = true;
	
	// if the iterator is associated with this btree ...
	if (this == rCIterPos.get_container ())
	{
		// ... and the iterator is ready for use ...
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

					// overwrite iterator time stamp directly, without having the iterator to evaluate again
					rCIterPos.set_time_stamp (this);
				}
			}
		}
	}

	// if fast erase was not possible ...
	if (bFallBack)
	{
		// ... then perform actual erase
		position_t			sPos;

		if (this->m_pData != NULL)
		{
			sPos.nIndex = rCIterPos.get_pos ();
	
#if defined (_DEBUG)

			BTREE_ASSERT (this->empty () == false, "CBTreeArray<_t_data, _t_datalayerproperties>::erase_via_reference: cannot remove entry from empty list");
			BTREE_ASSERT (sPos.nIndex < this->size (), "CBTreeArray<_t_data, _t_datalayerproperties>::erase_via_reference: position exceeds maximum entries");

#endif

			this->remove_from_node (sPos, this->m_nRootNode, 0);
		}

		// iterator reference is used for more than one access ...
		if (bReEvaluate)
		{
			// ... then prepare iterator for next access
			rCIterPos.sync ();
		}
	}
}

template<class _t_data, class _t_datalayerproperties>
void CBTreeArray<_t_data, _t_datalayerproperties>::_swap (CBTreeArray<_t_data, _t_datalayerproperties> &rArray)
{
	CBTreeBaseDefaults_t	&rArrayBase = dynamic_cast <CBTreeBaseDefaults_t &> (rArray);

	CBTreeBaseDefaults_t::_swap (rArrayBase);
}

template<class _t_data, class _t_datalayerproperties>
bool CBTreeArray<_t_data, _t_datalayerproperties>::compare_value_type (const value_type & rLhs, const value_type & rRhs, ::std::true_type) const
{
	return (rLhs == rRhs);
}

template<class _t_data, class _t_datalayerproperties>
bool CBTreeArray<_t_data, _t_datalayerproperties>::compare_value_type (const value_type & rLhs, const value_type & rRhs, ::std::false_type) const
{
#if defined (_MSC_VER)

	throw new ::std::bad_function_call ("CBTreeArray<_t_data, _t_datalayerproperties>::compare_value_type (const value_type &, const value_type &, ::std::false_type) const: ERROR: value_type is not out fitted with a compare operator!");

#else

	throw new ::std::bad_function_call ();

#endif

	return (false);
}

/**********************************************************************************************/

/*

The class' CBTreeArrayAccessWrapper purpose is to help the compiler to distinguish between
CBTreeArray::operator[] reads and writes, since the afore mentioned operator is
helping that. 
It is required to have that distingtion, because dependent on what datalayer type is
in use the returned reference might be a reference to a cache line, which needs
to be managed in terms of unloading unwanted data residing in that cache line and
reloading the actual data in question. On a higher level this is managed by get_at
and set_at, thus the two different routes.

To clearify, this is how reads work:
(const value_type &) = ((*this)[(const _t_sizetype) nOffset]) (value_type sData; get_at (nOffset, sData); return (sData))

This is how writes work:
1) (CBTreeArrayAccessWrapper) = CBTreeArray::operator[const _t_sizetype nOffset] creates an instance of CBTreeArrayAccessWrapper
	The constructor of CBTreeArrayAccessWrapper takes *this as a reference (m_rInstance)
	and the parameter nOffset becoming (m_nPos).
2) CBTreeArrayAccessWrapper::operator= (const value_type &rData) takes the reference to the data to be written
	and uses the instance of CBTreeArray and the copy of nOffset, which have been taken by the
	constructor before, to call set_at (m_rInstance.set_at (m_nPos, rData))

*/

template<class _t_data, class _t_sizetype>
CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype>::CBTreeArrayConstAccessWrapper
	(
		CBTreeArrayIf<_t_data, _t_sizetype>	&rInstance
	)
	:	m_rInstance (rInstance)
	,	m_nPos (0)
{
}

template<class _t_data, class _t_sizetype>
CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype>::~CBTreeArrayConstAccessWrapper ()
{
}

template<class _t_data, class _t_sizetype>
void CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype>::set (const _t_sizetype nPos)
{
	this->m_nPos = nPos;
}

template<class _t_data, class _t_sizetype>
_t_data & CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype>::operator* ()
{
	bool		bRslt;

	bRslt = m_rInstance.get_at (this->m_nPos, this->m_sData);

	BTREE_ASSERT (bRslt, "CBTreeArrayAccessWrapper<>::operator* (): get_at was not successful!");

	return (this->m_sData);
}

template<class _t_data, class _t_sizetype>
_t_data & CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype>::operator-> ()
{
	return (*this);
}

template<class _t_data, class _t_sizetype>
CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype>::operator const _t_data& ()
{
	bool		bRslt;

	bRslt = m_rInstance.get_at (this->m_nPos, this->m_sData);

	BTREE_ASSERT (bRslt, "CBTreeArrayConstAccessWrapper<>::operator const _t_data& (): get_at was not successful!");

	return (this->m_sData);
}

/************************************************************/

template<class _t_data, class _t_sizetype>
CBTreeArrayAccessWrapper<_t_data, _t_sizetype>::CBTreeArrayAccessWrapper
	(
		CBTreeArrayIf<_t_data, _t_sizetype>	&rInstance
	)
	:	CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype> (rInstance)
{
}

template<class _t_data, class _t_sizetype>
CBTreeArrayAccessWrapper<_t_data, _t_sizetype>::~CBTreeArrayAccessWrapper ()
{
}

template<class _t_data, class _t_sizetype>
CBTreeArrayAccessWrapper<_t_data, _t_sizetype> &CBTreeArrayAccessWrapper<_t_data, _t_sizetype>::operator=
	(const _t_data &rData)
{
	bool		bRslt;

	bRslt = this->m_rInstance.set_at (this->m_nPos, rData);

	BTREE_ASSERT (bRslt, "CBTreeArrayAccessWrapper<>::operator= (const _t_data &): set_at was not successful!");

	return (*this);
}

template<class _t_data, class _t_sizetype>
CBTreeArrayAccessWrapper<_t_data, _t_sizetype> &CBTreeArrayAccessWrapper<_t_data, _t_sizetype>::operator=
	(const CBTreeArrayAccessWrapper<_t_data, _t_sizetype> &rData)
{
	if (this != &rData)
	{
		_t_data		sDataTmp;
		bool		bRslt;

		bRslt = rData.m_rInstance.get_at (rData.m_nPos, sDataTmp);

		BTREE_ASSERT (bRslt, "CBTreeArrayAccessWrapper<>::operator= (const CBTreeArrayAccessWrapper<> &): get_at was not successful!");

		*this = sDataTmp;
	}

	return (*this);
}

#endif // BTREEARRAY_CPP
