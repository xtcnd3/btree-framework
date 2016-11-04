/************************************************************
**
** file:	btreeiter.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** iterator classes.
**
************************************************************/

#ifndef	BTREEITER_CPP
#define	BTREEITER_CPP

#include "btreeiter.h"

template<class _ti_container>
CBTreeConstIterator<_ti_container>::CBTreeConstIterator ()
	:	::std::iterator< ::std::random_access_iterator_tag, typename _ti_container::value_type, typename _ti_container::difference_type, typename _ti_container::pointer, typename _ti_container::reference> ()
	,	m_pContainer (NULL)
	,	m_nPos (~0x0)
	,	m_pExternState (NULL)
{
	this->invalidate_time_stamp ();
}

template<class _ti_container>
CBTreeConstIterator<_ti_container>::CBTreeConstIterator (const _ti_container *pContainer, size_type nPos, const bool bRegister)
	:	::std::iterator< ::std::random_access_iterator_tag, typename _ti_container::value_type, typename _ti_container::difference_type, typename _ti_container::pointer, typename _ti_container::reference> ()
	,	m_nPos (nPos)
	,	m_pExternState (NULL)
	,	m_pContainer ((_ti_container *) pContainer)
{
	m_pExternState = malloc (m_pContainer->get_iter_state_size ());

	BTREE_ASSERT (m_pExternState != NULL, "CBTreeConstIterator<_ti_container>::CBTreeConstIterator (const _ti_container *, size_type, bool): insufficient memory!");

	m_pContainer->reset_iter_state (m_pExternState);

	this->invalidate_time_stamp ();

	if (bRegister)
	{
		this->register_iterator (this->m_pContainer);
	}

//	this->evaluate ();
}

template<class _ti_container>
CBTreeConstIterator<_ti_container>::CBTreeConstIterator
	(
		const _ti_container *pContainer, 
		const size_type nPos, 
		const void *pExternState, 
		const btree_time_stamp_t sTimeStamp, 
		const bool bRegister
	)
	:	::std::iterator< ::std::random_access_iterator_tag, typename _ti_container::value_type, typename _ti_container::difference_type, typename _ti_container::pointer, typename _ti_container::reference> ()
	,	m_nPos (nPos)
	,	m_pExternState (NULL)
	,	m_pContainer ((_ti_container *) pContainer)
{
	m_pExternState = malloc (m_pContainer->get_iter_state_size ());

	BTREE_ASSERT (m_pExternState != NULL, "CBTreeConstIterator<_ti_container>::CBTreeConstIterator (const _ti_container *, size_type, void *, btree_time_stamp_t, bool): insufficient memory!");

	memcpy (m_pExternState, pExternState, m_pContainer->get_iter_state_size ());

	m_sTimeStamp.sTimeStamp = sTimeStamp;
	m_sTimeStamp.pContainer = this->m_pContainer;
	
	if (bRegister)
	{
		this->register_iterator (this->m_pContainer);
	}
}


template<class _ti_container>
CBTreeConstIterator<_ti_container>::CBTreeConstIterator (const CBTreeConstIterator &rIter, const bool bRegister)
	:	::std::iterator< ::std::random_access_iterator_tag, typename _ti_container::value_type, typename _ti_container::difference_type, typename _ti_container::pointer, typename _ti_container::reference> ()
	,	m_pContainer (rIter.m_pContainer)
	,	m_nPos (rIter.m_nPos)
	,	m_sTimeStamp (rIter.m_sTimeStamp)
	,	m_pExternState (NULL)
{
	if (m_pContainer != NULL)
	{
		m_pExternState = malloc (m_pContainer->get_iter_state_size ());

		BTREE_ASSERT (m_pExternState != NULL, "CBTreeConstIterator<_ti_container>::CBTreeConstIterator (const CBTreeConstIterator &, bool): insufficient memory!");

		memcpy (m_pExternState, rIter.m_pExternState, m_pContainer->get_iter_state_size ());

		if (bRegister)
		{
			this->register_iterator (this->m_pContainer);
		}
	}
}

template<class _ti_container>
CBTreeConstIterator<_ti_container>::CBTreeConstIterator (const CBTreeIterator<_ti_container> &rIter, const bool bRegister)
	:	::std::iterator< ::std::random_access_iterator_tag, typename _ti_container::value_type, typename _ti_container::difference_type, typename _ti_container::pointer, typename _ti_container::reference> ()
	,	m_nPos (rIter.m_nPos)
	,	m_pExternState (NULL)
	,	m_pContainer (rIter.m_pContainer)
{
	if (this->m_pContainer != NULL)
	{
		m_pExternState = malloc (m_pContainer->get_iter_state_size ());

		BTREE_ASSERT (m_pExternState != NULL, "CBTreeConstIterator<_ti_container>::CBTreeConstIterator (const CBTreeIterator &, bool): insufficient memory!");

		m_pContainer->reset_iter_state (m_pExternState);

		if (bRegister)
		{
			this->register_iterator (this->m_pContainer);
		}
	}

	this->invalidate_time_stamp ();
}

template<class _ti_container>
CBTreeConstIterator<_ti_container>::~CBTreeConstIterator ()
{
	this->unregister_iterator (this->m_pContainer);

	if (m_pExternState != NULL)
	{
		free (m_pExternState);
	}
}

template<class _ti_container>
void CBTreeConstIterator<_ti_container>::detach ()
{
	this->invalidate_time_stamp ();

	this->m_pContainer = NULL;
}

template<class _ti_container>
void CBTreeConstIterator<_ti_container>::swap (CBTreeConstIterator &rRight)
{
	if (this != &rRight)
	{
		if (this->m_pContainer != rRight.m_pContainer)
		{ // this also works if any container pointers are NULL
			this->unregister_iterator (this->m_pContainer);
			rRight.unregister_iterator (rRight.m_pContainer);

			fast_swap (m_pContainer, rRight.m_pContainer);
		}

		fast_swap (m_nPos, rRight.m_nPos);
		fast_swap (m_pExternState, rRight.m_pExternState);
		fast_swap (m_sTimeStamp, rRight.m_sTimeStamp);

		if (this->m_pContainer != rRight.m_pContainer)
		{ // this also works if any container pointers are NULL
			this->register_iterator (this->m_pContainer);
			rRight.register_iterator (rRight.m_pContainer);
		}
	}
}

template<class _ti_container>
CBTreeConstIterator<_ti_container>& CBTreeConstIterator<_ti_container>::operator= (const CBTreeConstIterator<_ti_container> &rIter)
{
	assign (rIter);

	return (*this);
}

template<class _ti_container>
CBTreeConstIterator<_ti_container>& CBTreeConstIterator<_ti_container>::operator= (const CBTreeIterator<_ti_container> &rIter)
{
	CBTreeConstIterator<_ti_container>		sCIter (rIter);

	*this = sCIter;

	return (*this);
}

template<class _ti_container>
bool CBTreeConstIterator<_ti_container>::operator== (const CBTreeConstIterator<_ti_container> &rIter) const
{
	if (&rIter == this)
	{
		return (true);
	}

	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator==: Cannot compare if target iterator is not associated with a container!");
	BTREE_ASSERT (rIter.m_pContainer != NULL, "CBTreeConstIterator::operator==: Cannot compare if external iterator is not associated with a container!");

	return (get_pos () == rIter.get_pos ());
}

template<class _ti_container>
bool CBTreeConstIterator<_ti_container>::operator!= (const CBTreeConstIterator<_ti_container> &rIter) const
{
	if (&rIter == this)
	{
		return (false);
	}

	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator!=: Cannot compare if target iterator is not associated with a container!");
	BTREE_ASSERT (rIter.m_pContainer != NULL, "CBTreeConstIterator::operator!=: Cannot compare if external iterator is not associated with a container!");

	return ( ! (*this == rIter));
}

template<class _ti_container>
typename CBTreeConstIterator<_ti_container>::value_type& CBTreeConstIterator<_ti_container>::operator* ()
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator*: Indirection not possible if iterator is not associated with a container!");
	BTREE_ASSERT (m_pExternState != NULL, "CBTreeConstIterator::operator*: Indirection not possible if iterator has not an initialized external state!");

	BTREE_ASSERT_EXCEPT (m_nPos < m_pContainer->size (), ::std::out_of_range, "CBTreeConstIterator::operator*: Cannot dereference if iterator is out of range!");

	this->sync ();

	m_sData = *(this->m_pContainer->get_iter_data (m_pExternState));

	return m_sData;
}

template<class _ti_container>
typename CBTreeConstIterator<_ti_container>::value_type* CBTreeConstIterator<_ti_container>::operator-> ()
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator->: Cannot dereference if iterator is not associated with a container!");
	BTREE_ASSERT (m_pExternState != NULL, "CBTreeConstIterator::operator->: Cannot dereference if iterator has not an initialized external state!");

	return (&**this);
}

template<class _ti_container>
CBTreeConstIterator<_ti_container>& CBTreeConstIterator<_ti_container>::operator++ ()
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::++operator: Cannot increment if iterator is not associated with a container!");

	increment_by (1);
	
	return (*this);
}

template<class _ti_container>
CBTreeConstIterator<_ti_container> CBTreeConstIterator<_ti_container>::operator++ (int)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator++: Cannot increment if iterator is not associated with a container!");

	CBTreeConstIterator		sTemp (*this);

	increment_by (1);
	
	return (sTemp);
}

template<class _ti_container>
CBTreeConstIterator<_ti_container>& CBTreeConstIterator<_ti_container>::operator-- ()
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::--operator: Cannot increment if iterator is not associated with a container!");

	decrement_by (1);
	
	return (*this);
}

template<class _ti_container>
CBTreeConstIterator<_ti_container> CBTreeConstIterator<_ti_container>::operator-- (int)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator--: Cannot increment if iterator is not associated with a container!");

	CBTreeConstIterator		sTemp (*this);

	decrement_by (1);
	
	return (sTemp);
}
/*
template<class _ti_container>
CBTreeConstIterator<_ti_container>& CBTreeConstIterator<_ti_container>::operator+= (const int nOperand)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator+= (int): Cannot increment if iterator is not associated with a container!");

	if (nOperand > 0)
	{
		return (this->operator+= ((size_type) nOperand));
	}
	else
	{
		return (this->operator-= ((size_type) (0 - nOperand)));
	}

	return (*this);
}
*/
template<class _ti_container>
CBTreeConstIterator<_ti_container>& CBTreeConstIterator<_ti_container>::operator+= (const typename CBTreeConstIterator<_ti_container>::size_type nOperand)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator+= (size_type): Cannot increment if target iterator is not associated with a container!");
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator+= (size_type): Cannot increment if external iterator is not associated with a container!");

	this->increment_by (nOperand);

	return (*this);
}

template<class _ti_container>
CBTreeConstIterator<_ti_container>& CBTreeConstIterator<_ti_container>::operator+= (const CBTreeConstIterator<_ti_container> &rIter)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator+= (const CBTreeConstIterator &): Cannot increment if target iterator is not associated with a container!");
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator+= (const CBTreeConstIterator &): Cannot increment if external iterator is not associated with a container!");

	this->increment_by (rIter.m_nPos);

	return (*this);
}
/*
template<class _ti_container>
CBTreeConstIterator<_ti_container>& CBTreeConstIterator<_ti_container>::operator-= (const int nOperand)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator-= (int): Cannot decrement if iterator is not associated with a container!");

	if (nOperand > 0)
	{
		return (this->operator-= ((size_type) nOperand));
	}
	else
	{
		return (this->operator+= ((size_type) (0 - nOperand)));
	}

	return (*this);
}
*/
template<class _ti_container>
CBTreeConstIterator<_ti_container>& CBTreeConstIterator<_ti_container>::operator-= (const typename CBTreeConstIterator<_ti_container>::size_type nOperand)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator-= (size_type): Cannot decrement if target iterator is not associated with a container!");
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator-= (size_type): Cannot decrement if external iterator is not associated with a container!");

	this->decrement_by (nOperand);

	return (*this);
}

template<class _ti_container>
CBTreeConstIterator<_ti_container>& CBTreeConstIterator<_ti_container>::operator-= (const CBTreeConstIterator<_ti_container> &rIter)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator-= (const CBTreeConstIterator &): Cannot decrement if target iterator is not associated with a container!");
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator-= (const CBTreeConstIterator &): Cannot decrement if external iterator is not associated with a container!");

	this->decrement_by (rIter.m_nPos);

	return (*this);
}
/*
template<class _ti_container>
const CBTreeConstIterator<_ti_container> CBTreeConstIterator<_ti_container>::operator+ (const int nOperand) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator+ (int): Cannot increment if iterator is not associated with a container!");

	CBTreeBaseConstIterator_t		nResult = *this;

	nResult += nOperand;

	return (nResult);
}
*/
template<class _ti_container>
const CBTreeConstIterator<_ti_container> CBTreeConstIterator<_ti_container>::operator+ (const typename CBTreeConstIterator<_ti_container>::size_type nOperand) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator+ (size_type): Cannot increment if target iterator is not associated with a container!");
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator+ (size_type): Cannot increment if external iterator is not associated with a container!");

	CBTreeBaseConstIterator_t		nResult = *this;

	nResult += nOperand;

	return (nResult);
}

template<class _ti_container>
const CBTreeConstIterator<_ti_container> CBTreeConstIterator<_ti_container>::operator+ (const CBTreeConstIterator<_ti_container> &rIter) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator+ (const CBTreeConstIterator &): Cannot increment if target iterator is not associated with a container!");
	BTREE_ASSERT (rIter.m_pContainer != NULL, "CBTreeConstIterator::operator+ (const CBTreeConstIterator &): Cannot increment if external iterator is not associated with a container!");

	CBTreeBaseConstIterator_t		nResult = *this;

	nResult += rIter;
	
	return (nResult);
}
/*
template<class _ti_container>
const CBTreeConstIterator<_ti_container> CBTreeConstIterator<_ti_container>::operator- (const int nOperand) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator- (int): Cannot decrement if iterator is not associated with a container!");

	CBTreeBaseConstIterator_t		nResult = *this;

	nResult -= nOperand;

	return (nResult);
}
*/
template<class _ti_container>
const CBTreeConstIterator<_ti_container> CBTreeConstIterator<_ti_container>::operator- (const typename CBTreeConstIterator<_ti_container>::size_type nOperand) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator- (size_type): Cannot decrement if target iterator is not associated with a container!");
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator- (size_type): Cannot decrement if external iterator is not associated with a container!");

	CBTreeBaseConstIterator_t		nResult = *this;

	nResult -= nOperand;

	return (nResult);
}

template<class _ti_container>
const typename CBTreeConstIterator<_ti_container>::size_type CBTreeConstIterator<_ti_container>::operator- (const CBTreeConstIterator<_ti_container> &rIter) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::(size_type)operator- (const CBTreeConstIterator &): Cannot decrement if target iterator is not associated with a container!");
	BTREE_ASSERT (rIter.m_pContainer != NULL, "CBTreeConstIterator::(size_type)operator- (const CBTreeConstIterator &): Cannot decrement if external iterator is not associated with a container!");
	
	size_type		nResult;
	
	nResult = this->m_nPos - rIter.m_nPos;
	
	return (nResult);
}

template<class _ti_container>
bool CBTreeConstIterator<_ti_container>::operator< (const CBTreeConstIterator<_ti_container> &rIter) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator<: Cannot compare if target iterator is not associated with a container!");
	BTREE_ASSERT (rIter.m_pContainer != NULL, "CBTreeConstIterator::operator<: Cannot compare if external iterator is not associated with a container!");

	return (this->m_nPos < rIter.m_nPos);
}

template<class _ti_container>
bool CBTreeConstIterator<_ti_container>::operator<= (const CBTreeConstIterator<_ti_container> &rIter) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator<=: Cannot compare if target iterator is not associated with a container!");
	BTREE_ASSERT (rIter.m_pContainer != NULL, "CBTreeConstIterator::operator<=: Cannot compare if external iterator is not associated with a container!");

	return (this->m_nPos <= rIter.m_nPos);
}

template<class _ti_container>
bool CBTreeConstIterator<_ti_container>::operator> (const CBTreeConstIterator<_ti_container> &rIter) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator>: Cannot compare if target iterator is not associated with a container!");
	BTREE_ASSERT (rIter.m_pContainer != NULL, "CBTreeConstIterator::operator>: Cannot compare if external iterator is not associated with a container!");

	return (this->m_nPos > rIter.m_nPos);
}

template<class _ti_container>
bool CBTreeConstIterator<_ti_container>::operator>= (const CBTreeConstIterator<_ti_container> &rIter) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeConstIterator::operator>=: Cannot compare if target iterator is not associated with a container!");
	BTREE_ASSERT (rIter.m_pContainer != NULL, "CBTreeConstIterator::operator>=: Cannot compare if external iterator is not associated with a container!");

	return (this->m_nPos >= rIter.m_nPos);
}

template<class _ti_container>
typename CBTreeConstIterator<_ti_container>::value_type& CBTreeConstIterator<_ti_container>::operator[] (const int nPos)
{
	typedef typename	::CBTreeConstIterator<_ti_container>		iter_t;

	iter_t		nIter ((iter_t)*this);

	nIter += nPos;

	m_sSubScriptResult = *nIter;

	return (m_sSubScriptResult);
}

template<class _ti_container>
typename CBTreeConstIterator<_ti_container>::value_type& CBTreeConstIterator<_ti_container>::operator[] (const typename CBTreeConstIterator<_ti_container>::size_type nPos)
{
	typedef typename	::CBTreeConstIterator<_ti_container>		iter_t;

	iter_t		nIter ((iter_t)*this);

	nIter += nPos;

	m_sSubScriptResult = *nIter;

	return (m_sSubScriptResult);
}

template<class _ti_container>
_ti_container* CBTreeConstIterator<_ti_container>::get_container ()
{
	return (this->m_pContainer);
}

template<class _ti_container>
bool CBTreeConstIterator<_ti_container>::is_btree_valid ()
{
	return (this->m_pContainer != NULL);
}

template<class _ti_container>
bool CBTreeConstIterator<_ti_container>::is_evaluated ()
{
	if (this->is_time_stamp_up_to_date ())
	{
		BTREE_ASSERT (this->m_pExternState != NULL, "CBTreeConstIterator<_ti_container>::is_evaluated (): Cannot test if iterator evaluated if external state has not been initialized!");

		return (m_pContainer->is_iter_pos_evaluated (this->m_pExternState, this->m_nPos));
	}

	return (false);
}

template<class _ti_container>
void CBTreeConstIterator<_ti_container>::sync ()
{
	if (!this->is_time_stamp_up_to_date ())
	{
		this->evaluate ();

		this->update_time_stamp ();
	}
	else
	{
		this->evaluate_by_seek ();
	}
}

template<class _ti_container>
typename CBTreeConstIterator<_ti_container>::size_type CBTreeConstIterator<_ti_container>::get_pos ()
{
	return (this->m_nPos);
}

template<class _ti_container>
void *CBTreeConstIterator<_ti_container>::get_iterator_state ()
{
	BTREE_ASSERT (this->m_pExternState != NULL, "CBTreeConstIterator<_ti_container>::get_iterator_state (): An external state that has not been initialized ought not to be returned!");

	return (m_pExternState);
}

template<class _ti_container>
void CBTreeConstIterator<_ti_container>::set_time_stamp (_ti_container *pContainer)
{
	BTREE_ASSERT (pContainer == this->m_pContainer, "CBTreeConstIterator<>::set_time_stamp (_ti_container *): Cannot set time stamp if input parameter is not associated with this iterator!");

	this->m_sTimeStamp.pContainer = pContainer;
	this->m_sTimeStamp.sTimeStamp = pContainer->get_time_stamp ();
}

template<class _ti_container>
void CBTreeConstIterator<_ti_container>::increment_by (typename CBTreeConstIterator<_ti_container>::size_type nOperand)
{
	m_nPos += nOperand;
}

template<class _ti_container>
void CBTreeConstIterator<_ti_container>::decrement_by (typename CBTreeConstIterator<_ti_container>::size_type nOperand)
{
	m_nPos -= nOperand;
}

template<class _ti_container>
bool CBTreeConstIterator<_ti_container>::out_of_range ()
{
	return (m_nPos >= m_pContainer->size ());
}

template<class _ti_container>
void CBTreeConstIterator<_ti_container>::evaluate ()
{
	if (m_nPos > m_pContainer->size ())
	{
		return;
	}

	BTREE_ASSERT (this->m_pExternState != NULL, "CBTreeConstIterator<_ti_container>::evaluate (): An external state that has not been initialized ought not to be used by a container instance!");

	m_pContainer->evaluate_iter (this->m_pExternState, m_nPos);
}

template<class _ti_container>
void CBTreeConstIterator<_ti_container>::evaluate_by_seek ()
{
	BTREE_ASSERT (this->m_pExternState != NULL, "CBTreeConstIterator<_ti_container>::evaluate_by_seek (): An external state that has not been initialized ought not to be used by a container instance!");

	m_pContainer->evaluate_iter_by_seek (this->m_pExternState, m_nPos);
}

template<class _ti_container>
typename CBTreeConstIterator<_ti_container>::size_type CBTreeConstIterator<_ti_container>::get_pos () const
{
	return (this->m_nPos);
}

template<class _ti_container>
bool CBTreeConstIterator<_ti_container>::is_time_stamp_up_to_date ()
{
	if (this->m_sTimeStamp.pContainer == NULL)
	{
		return (false);
	}

#if defined (_DEBUG)

	BTREE_ASSERT (this->m_sTimeStamp.pContainer == this->m_pContainer, "CBTreeConstIterator<_ti_container>::is_time_stamp_up_to_date: ERROR: The time stamp container ought to be synchronized at this point!");

#endif
	
//	if (this->m_sTimeStamp.pContainer != this->m_pContainer)
//	{
//		return (false);
//	}

	return (this->m_sTimeStamp.sTimeStamp == this->m_pContainer->get_time_stamp ());
}

template<class _ti_container>
void CBTreeConstIterator<_ti_container>::invalidate_time_stamp ()
{
	this->m_sTimeStamp.pContainer = NULL;
	this->m_sTimeStamp.sTimeStamp.nAccCtr = 0;
	this->m_sTimeStamp.sTimeStamp.sTime -= this->m_sTimeStamp.sTimeStamp.sTime.time_since_epoch ();
}

template<class _ti_container>
void CBTreeConstIterator<_ti_container>::update_time_stamp ()
{
#if defined (_DEBUG)

	if (this->m_sTimeStamp.pContainer != NULL)
	{
		BTREE_ASSERT (this->m_sTimeStamp.pContainer == this->m_pContainer, "CBTreeConstIterator<_ti_container>::update_time_stamp: ERROR: The time stamp container ought to be synchronized at this point!");

//		if (this->m_sTimeStamp.pContainer != this->m_pContainer)
//		{
//			this->unregister_iterator (this->m_sTimeStamp.pContainer);
//
//			this->register_iterator (this->m_pContainer);
//		}
	}

#endif

	this->m_sTimeStamp.pContainer = this->m_pContainer;

	if (this->m_pContainer != NULL)
	{
		this->m_sTimeStamp.sTimeStamp = this->m_pContainer->get_time_stamp ();
	}
}

template<class _ti_container>
float CBTreeConstIterator<_ti_container>::get_average_node_hops (size_type nHopDistance) const
{
	float		fRetval = 0.0f;
	float		fRangeIncPerDepth;
	float		fHopDistance = (float) nHopDistance;
	float		fRange = 1.0;
	float		fPrevRange;

	fRangeIncPerDepth = (float) m_pContainer->get_node_max_sub_nodes ();
	fRangeIncPerDepth *= 0.75f;
	
	do
	{
		fRetval += 1.0f;

		fPrevRange = fRange;

		fRange *= fRangeIncPerDepth;
	} while (fRange < fHopDistance);

	fRetval += (fHopDistance - fPrevRange) / (fRange - fPrevRange);

	return (fRetval);
}

template<class _ti_container>
void CBTreeConstIterator<_ti_container>::assign (const CBTreeConstIterator<_ti_container> &rIter)
{
	if (this != &rIter)
	{
		if (m_pContainer != rIter.m_pContainer)
		{
			if (m_pContainer != NULL)
			{
				this->unregister_iterator (m_pContainer);
			}
		}

		if (rIter.m_pContainer != NULL)
		{
			if (m_pExternState == NULL)
			{
				m_pExternState = malloc (rIter.m_pContainer->get_iter_state_size ());

				BTREE_ASSERT (m_pExternState != NULL, "CBTreeConstIterator<_ti_container>::assign (const CBTreeConstIterator<_ti_container> &): insufficient memory!");
			}
			else
			{
				m_pExternState = realloc (m_pExternState, rIter.m_pContainer->get_iter_state_size ());

				BTREE_ASSERT (m_pExternState != NULL, "CBTreeConstIterator<_ti_container>::assign (const CBTreeConstIterator<_ti_container> &): insufficient memory (realloc)!");
			}

#if defined (_DEBUG)

			BTREE_ASSERT (rIter.m_pExternState != NULL, "CBTreeConstIterator<_ti_container>::assign: ERROR: Since the source iterator has a container associated with it, also the source iterator's external state ought to be set!");

#endif

//			if (rIter.m_pExternState != NULL)
			{
				memcpy (m_pExternState, rIter.m_pExternState, rIter.m_pContainer->get_iter_state_size ());

				m_sTimeStamp = rIter.m_sTimeStamp;
			}
//			else
//			{
//				rIter.m_pContainer->reset_iter_state (m_pExternState);
//
//				invalidate_time_stamp ();
//			}
		}

		if (m_pContainer != rIter.m_pContainer)
		{
			if (rIter.m_pContainer != NULL)
			{
				this->register_iterator (rIter.m_pContainer);
			}
		}

		m_pContainer = rIter.m_pContainer;
		m_nPos = rIter.m_nPos;
	}
}

template<class _ti_container>
void CBTreeConstIterator<_ti_container>::register_iterator (_ti_container *pContainer)
{
	if (pContainer != NULL)
	{
		pContainer->register_iterator (this);
	}
}

template<class _ti_container>
void CBTreeConstIterator<_ti_container>::unregister_iterator (_ti_container *pContainer)
{
	if (pContainer != NULL)
	{
		pContainer->unregister_iterator (this);
	}
}

template<class _ti_container>
typename ::CBTreeConstIterator<_ti_container> operator+
(
	typename _ti_container::size_type						nLhs, 
	const typename ::CBTreeConstIterator<_ti_container>	&nRhs
)
{
	typename ::CBTreeConstIterator<_ti_container>		nResult (nRhs);

	nResult += nLhs;

	return (nResult);
}

template<class _ti_container>
typename ::CBTreeConstIterator<_ti_container> operator+
(
	int nLhs, 
	const typename ::CBTreeConstIterator<_ti_container> &nRhs
)
{
	typename ::CBTreeConstIterator<_ti_container>		nResult (nRhs);

	nResult += nLhs;

	return (nResult);
}

template<class _ti_container>
CBTreeIterator<_ti_container>::CBTreeIterator ()
	:	CBTreeConstIterator<_ti_container> ()
{
}

template<class _ti_container>
CBTreeIterator<_ti_container>::CBTreeIterator (const _ti_container *pContainer, const size_type nPos, const bool bRegister)
	:	CBTreeConstIterator<_ti_container> (pContainer, nPos, bRegister)
{
}

template<class _ti_container>
CBTreeIterator<_ti_container>::CBTreeIterator (const _ti_container *pContainer, const size_type nPos, const void *pExternState, const btree_time_stamp_t sTimeStamp, const bool bRegister)
	:	CBTreeConstIterator<_ti_container> (pContainer, nPos, pExternState, sTimeStamp, bRegister)
{
}

template<class _ti_container>
CBTreeIterator<_ti_container>::CBTreeIterator (const CBTreeIterator<_ti_container> &rIter, const bool bRegister)
	:	CBTreeConstIterator<_ti_container> (rIter, bRegister)
{
}

template<class _ti_container>
CBTreeIterator<_ti_container>::CBTreeIterator (const CBTreeReverseIterator<CBTreeIterator<_ti_container> > &rRIter, const bool bRegister)
	:	CBTreeConstIterator<_ti_container> (rRIter.base ())
{
}

template<class _ti_container>
CBTreeIterator<_ti_container>::CBTreeIterator (const CBTreeConstIterator<_ti_container> &rIter, const bool bRegister)
	:	CBTreeConstIterator<_ti_container> (rIter, bRegister)
{
}

template<class _ti_container>
CBTreeIterator<_ti_container>::~CBTreeIterator ()
{
}

template<class _ti_container>
const typename CBTreeIterator<_ti_container>::value_type & CBTreeIterator<_ti_container>::operator* () const
{
	CBTreeConstIterator<_ti_container>			&rCThis (*this);

	return (&**rCThis);
}

template<class _ti_container>
CBTreeIteratorSubScriptWrapper<_ti_container> CBTreeIterator<_ti_container>::operator* ()
{
	return (CBTreeIteratorSubScriptWrapper<_ti_container> (*this, (size_type) 0));
}

template<class _ti_container>
CBTreeIterator<_ti_container>& CBTreeIterator<_ti_container>::operator= (const CBTreeIterator<_ti_container> &rIter)
{
	this->assign (rIter);

	return (*this);
}

template<class _ti_container>
CBTreeIteratorSubScriptWrapper<_ti_container> CBTreeIterator<_ti_container>::operator[] (const int nPos)
{
	return (CBTreeIteratorSubScriptWrapper<_ti_container> (*this, (size_type) nPos));
}

template<class _ti_container>
CBTreeIteratorSubScriptWrapper<_ti_container> CBTreeIterator<_ti_container>::operator[] (const size_type nPos)
{
	return (CBTreeIteratorSubScriptWrapper<_ti_container> (*this, nPos));
}

template<class _ti_container>
void CBTreeIterator<_ti_container>::set (const value_type &rData)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "CBTreeIterator<_ti_container>::set (const value_type &): Cannot set iterator data if container is not set!");
	BTREE_ASSERT (this->m_pExternState != NULL, "CBTreeIterator<_ti_container>::set (const value_type &): Cannot set iterator data if external state is not initialized!");

	this->sync ();

	BTREE_ASSERT_EXCEPT (!this->out_of_range (), ::std::out_of_range, "CBTreeIterator<_ti_container>::set (const value_type &): Cannot dereference if iterator is out of range!");

	this->m_pContainer->set_iter_data (this->m_pExternState, rData);
}

template<class _ti_container>
typename ::CBTreeIterator<_ti_container> operator+
(
	typename _ti_container::size_type					nLhs, 
	const typename ::CBTreeIterator<_ti_container>	&nRhs
)
{
	typename ::CBTreeIterator<_ti_container>		nResult (nRhs);

	nResult += nLhs;

	return (nResult);
}

template<class _ti_container>
typename ::CBTreeIterator<_ti_container> operator+
(
	int nLhs, 
	const typename ::CBTreeIterator<_ti_container> &nRhs
)
{
	typename ::CBTreeIterator<_ti_container>		nResult (nRhs);

	nResult += nLhs;

	return (nResult);
}

template<class _ti_container>
CBTreeIteratorSubScriptWrapper<_ti_container>::CBTreeIteratorSubScriptWrapper
(
	const CBTreeIterator<_ti_container>									&rInstance, 
	typename CBTreeIteratorSubScriptWrapper<_ti_container>::size_type	nPos
)
	:	m_nOffset (rInstance)
	,	m_nPos (nPos)
{
	m_nOffset += m_nPos;
}

template<class _ti_container>
CBTreeIteratorSubScriptWrapper<_ti_container>::~CBTreeIteratorSubScriptWrapper ()
{
}

template<class _ti_container>
CBTreeIteratorSubScriptWrapper<_ti_container> &
CBTreeIteratorSubScriptWrapper<_ti_container>::operator= (const typename CBTreeIteratorSubScriptWrapper<_ti_container>::value_type &rData)
{
	m_nOffset.set (rData);

	return (*this);
}

template<class _ti_container>
CBTreeIteratorSubScriptWrapper<_ti_container>::operator const typename CBTreeIteratorSubScriptWrapper<_ti_container>::value_type & ()
{
	m_sData = *(dynamic_cast <CBTreeConstIterator<_ti_container> &> (m_nOffset));

	return (m_sData);
}

template<class _t_iterator>
CBTreeConstReverseIterator<_t_iterator>::CBTreeConstReverseIterator ()
	:	::std::reverse_iterator<_t_iterator> ()
{
}

template<class _t_iterator>
CBTreeConstReverseIterator<_t_iterator>::CBTreeConstReverseIterator (const _t_iterator &rIter)
	:	::std::reverse_iterator<_t_iterator> (rIter)
{
}

template<class _t_iterator>
CBTreeConstReverseIterator<_t_iterator>::CBTreeConstReverseIterator (const CBTreeConstReverseIterator<_t_iterator> &rIter)
	:	::std::reverse_iterator<_t_iterator> (rIter)
{
}

template<class _t_iterator>
CBTreeConstReverseIterator<_t_iterator>::CBTreeConstReverseIterator (const CBTreeReverseIterator<CBTreeIterator<typename _t_iterator::container_t> > &rIter)
	:	::std::reverse_iterator<_t_iterator> (rIter)
{
}

template<class _t_iterator>
CBTreeConstReverseIterator<_t_iterator>::~CBTreeConstReverseIterator	()
{
}

template<class _t_iterator>
void CBTreeConstReverseIterator<_t_iterator>::swap (CBTreeConstReverseIterator &rRight)
{
#if defined (BTREE_ITERATOR_REGISTRATION)

	CBTreeConstReverseIterator<_t_iterator>		sRIterRtoL (rRight);
	CBTreeConstReverseIterator<_t_iterator>		sRIterLtoR (*this);

	*this = sRIterRtoL;
	rRight = sRIterLtoR;

#else

	fast_swap (*this, rRight);

#endif
}

template<class _t_iterator>
CBTreeConstReverseIterator<_t_iterator>& CBTreeConstReverseIterator<_t_iterator>::operator= (const ::std::reverse_iterator<_t_iterator> &rIter)
{
	CBTreeConstReverseIterator<_t_iterator>		sIter (rIter.base ());

	*this = sIter;

	return (*this);
}

template<class _t_iterator>
CBTreeConstReverseIterator<_t_iterator>& CBTreeConstReverseIterator<_t_iterator>::operator=
	(
		const CBTreeReverseIterator<CBTreeIterator <typename _t_iterator::container_t> > &rIter
	)
{
	CBTreeConstReverseIterator<_t_iterator>		sIter (rIter.base ());

	*this = sIter;

	return (*this);
}

template<class _t_iterator>
bool CBTreeConstReverseIterator<_t_iterator>::operator== (const CBTreeConstReverseIterator<_t_iterator> &rIter) const
{
	if (&rIter == this)
	{
		return (true);
	}

	return (this->base () == rIter.base ());
}

template<class _t_iterator>
bool CBTreeConstReverseIterator<_t_iterator>::operator!= (const CBTreeConstReverseIterator<_t_iterator> &rIter) const
{
	if (&rIter == this)
	{
		return (false);
	}

	return ( ! (*this == rIter) );
}

template<class _t_iterator>
typename CBTreeConstReverseIterator<_t_iterator>::value_type& CBTreeConstReverseIterator<_t_iterator>::operator* ()
{
	_t_iterator		nIter (this->base ());

	nIter--;

	m_sSubScriptResult = *nIter;

	return (m_sSubScriptResult);
}

template<class _t_iterator>
typename CBTreeConstReverseIterator<_t_iterator>::value_type* CBTreeConstReverseIterator<_t_iterator>::operator-> ()
{
	return (&**this);
}

	
template<class _t_iterator>
typename CBTreeConstReverseIterator<_t_iterator>::value_type& CBTreeConstReverseIterator<_t_iterator>::operator[] (const int nPos)
{	
	typedef typename	::CBTreeConstReverseIterator<_t_iterator>		iter_t;

	iter_t		nIter ((iter_t)*this);

	nIter += nPos;

	m_sSubScriptResult = *nIter;

	return (m_sSubScriptResult);
}

template<class _t_iterator>
typename CBTreeConstReverseIterator<_t_iterator>::value_type& CBTreeConstReverseIterator<_t_iterator>::operator[] (const typename CBTreeConstReverseIterator<_t_iterator>::size_type nPos)
{
	typedef typename	::CBTreeConstReverseIterator<_t_iterator>		iter_t;

	iter_t		nIter ((iter_t)*this);

	nIter += nPos;

	m_sSubScriptResult = *nIter;

	return (m_sSubScriptResult);
}	

template<class _t_iterator>
CBTreeReverseIterator<_t_iterator>::CBTreeReverseIterator ()
	:	CBTreeConstReverseIterator<_t_iterator> ()
{
}

template<class _t_iterator>
CBTreeReverseIterator<_t_iterator>::CBTreeReverseIterator (const _t_iterator &rIter)
	:	CBTreeConstReverseIterator<_t_iterator> (rIter)
{
}

template<class _t_iterator>
CBTreeReverseIterator<_t_iterator>::CBTreeReverseIterator (const CBTreeReverseIterator<_t_iterator> &rIter)
	:	CBTreeConstReverseIterator<_t_iterator> (rIter)
{
}

template<class _t_iterator>
CBTreeReverseIterator<_t_iterator>::~CBTreeReverseIterator ()
{
}

template<class _t_iterator>
const typename CBTreeReverseIterator<_t_iterator>::value_type & CBTreeReverseIterator<_t_iterator>::operator* () const
{
	CBTreeConstReverseIterator<_t_iterator>			&rCThis (*this);

	return (*rCThis);
}

template<class _t_iterator>
CBTreeIteratorSubScriptWrapper<typename _t_iterator::container_t> CBTreeReverseIterator<_t_iterator>::operator* ()
{
	CBTreeReverseIterator<_t_iterator>	sRIter (*this);

	++sRIter;

	CBTreeIterator<container_t>	sIter (sRIter, false);

	return (CBTreeIteratorSubScriptWrapper<container_t> (sIter, (size_type) 0));
}

template<class _t_iterator>
CBTreeReverseIterator<_t_iterator>& CBTreeReverseIterator<_t_iterator>::operator= (const ::std::reverse_iterator<_t_iterator> &rIter)
{
	CBTreeReverseIterator<_t_iterator>		sIter (rIter.base ());

	*this = sIter;

	return (*this);
}

template<class _t_iterator>
CBTreeReverseIterator<_t_iterator>& CBTreeReverseIterator<_t_iterator>::operator= (const value_type &rData)
{
	_t_iterator				sIter (this->base ());

	sIter++;

	sIter = rData;

	return (*this);
}

template<class _t_iterator>
CBTreeReverseIterator<_t_iterator>::operator const CBTreeConstReverseIterator<CBTreeConstIterator <typename _t_iterator::container_t> > &	()
{
	const CBTreeConstReverseIterator<CBTreeConstIterator <typename _t_iterator::container_t> > &rRetIter = dynamic_cast <const CBTreeConstReverseIterator<CBTreeConstIterator <typename _t_iterator::container_t> > &> ((CBTreeReverseIterator<_t_iterator>) *this);

	return (rRetIter);
}

template<class _t_iterator>
CBTreeIteratorSubScriptWrapper<typename _t_iterator::container_t> CBTreeReverseIterator<_t_iterator>::operator[] (const int nPos)
{
	int		nRevPos = 0 - nPos;

	CBTreeReverseIterator<_t_iterator>	sRIter (*this);

	++sRIter;

	CBTreeIterator<container_t>	sIter (sRIter, false);
	
	return (CBTreeIteratorSubScriptWrapper<container_t> (sIter, (size_type) nRevPos));
}

template<class _t_iterator>
CBTreeIteratorSubScriptWrapper<typename _t_iterator::container_t> CBTreeReverseIterator<_t_iterator>::operator[] (const typename _t_iterator::container_t::size_type nPos)
{
	typename _t_iterator::size_type		nRevPos = nPos - 1;

	nRevPos = ~nRevPos;

	CBTreeReverseIterator<_t_iterator>	sRIter (*this);

	++sRIter;

	CBTreeIterator<container_t>	sIter (sRIter, false);

	return (CBTreeIteratorSubScriptWrapper<container_t> (sIter, nRevPos));
}

#endif	// BTREEITER_CPP
