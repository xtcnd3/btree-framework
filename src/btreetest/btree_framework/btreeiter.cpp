/************************************************************
**
** file:	btreeiter.cpp
** author:	Andreas Steffens
** license:	GPL v2
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

template <class _ti_container>
CBTreeBaseConstIterator<_ti_container>::CBTreeBaseConstIterator ()
	:	::std::iterator< ::std::random_access_iterator_tag, data_t, sizetype_t> ()
	,	m_pContainer (NULL)
	,	m_nPos (~0x0)
	,	m_nAssociatedPos (~0x0)
	,	m_nNode (~0x0)
	,	m_nSub (~0x0)
{
	this->invalidate_time_stamp ();
}

template <class _ti_container>
CBTreeBaseConstIterator<_ti_container>::CBTreeBaseConstIterator (_ti_container *pContainer, sizetype_t nPos, bool bRegister)
	:	::std::iterator< ::std::random_access_iterator_tag, data_t, sizetype_t> ()
	,	m_pContainer (pContainer)
	,	m_nPos (nPos)
	,	m_nAssociatedPos (~0x0)
	,	m_nNode (~0x0)
	,	m_nSub (~0x0)
{
	this->invalidate_time_stamp ();

	if (bRegister)
	{
		this->register_iterator (this->m_pContainer);
	}

//	this->evaluate ();
}

template <class _ti_container>
CBTreeBaseConstIterator<_ti_container>::CBTreeBaseConstIterator
	(
		_ti_container *pContainer, 
		sizetype_t nPos, 
		nodeiter_t nNode, 
		subnodeiter_t nSub, 
		btree_time_stamp_t sTimeStamp, 
		bool bRegister
	)
	:	::std::iterator< ::std::random_access_iterator_tag, data_t, sizetype_t> ()
	,	m_pContainer (pContainer)
	,	m_nPos (nPos)
	,	m_nAssociatedPos (nPos)
	,	m_nNode (nNode)
	,	m_nSub (nSub)
{
	m_sTimeStamp.sTimeStamp = sTimeStamp;
	m_sTimeStamp.pContainer = pContainer;
	
	if (bRegister)
	{
		this->register_iterator (this->m_pContainer);
	}
}


template <class _ti_container>
CBTreeBaseConstIterator<_ti_container>::CBTreeBaseConstIterator (const CBTreeBaseConstIterator &rIter, bool bRegister)
	:	::std::iterator< ::std::random_access_iterator_tag, data_t, sizetype_t> ()
	,	m_pContainer (rIter.m_pContainer)
	,	m_nPos (rIter.m_nPos)
	,	m_nAssociatedPos (rIter.m_nAssociatedPos)
	,	m_nNode (rIter.m_nNode)
	,	m_nSub (rIter.m_nSub)
	,	m_sTimeStamp (rIter.m_sTimeStamp)
{
	if (bRegister)
	{
		this->register_iterator (this->m_pContainer);
	}
}

template <class _ti_container>
CBTreeBaseConstIterator<_ti_container>::~CBTreeBaseConstIterator ()
{
	this->unregister_iterator (this->m_pContainer);
}

template <class _ti_container>
void CBTreeBaseConstIterator<_ti_container>::detach ()
{
	this->invalidate_time_stamp ();

	this->m_pContainer = NULL;
}

template <class _ti_container>
void CBTreeBaseConstIterator<_ti_container>::swap (CBTreeBaseConstIterator &rRight)
{
	if (this != &rRight)
	{
		if (this->m_pContainer != rRight.m_pContainer)
		{ // this also works if both b-tree pointers are NULL
			this->unregister_iterator (this->m_pContainer);
			rRight.unregister_iterator (rRight.m_pContainer);

			fast_swap <_ti_container *> (m_pContainer, rRight.m_pContainer);
		}

		fast_swap <sizetype_t> (m_nPos, rRight.m_nPos);
		fast_swap <sizetype_t> (m_nAssociatedPos, rRight.m_nAssociatedPos);
		fast_swap <nodeiter_t> (m_nNode, rRight.m_nNode);
		fast_swap <subnodeiter_t> (m_nSub, rRight.m_nSub);
		//fast_swap <data_t> (m_sData, rRight.m_sData);
		fast_swap <btree_iter_last_access_t> (m_sTimeStamp, rRight.m_sTimeStamp);

		if (this->m_pContainer != rRight.m_pContainer)
		{ // this also works if both b-tree pointers are NULL
			this->register_iterator (this->m_pContainer);
			rRight.register_iterator (rRight.m_pContainer);
		}
	}
}

template <class _ti_container>
CBTreeBaseConstIterator<_ti_container>& CBTreeBaseConstIterator<_ti_container>::operator= (const CBTreeBaseConstIterator<_ti_container> &rIter)
{
	assign (rIter);

	return (*this);
}

template <class _ti_container>
bool CBTreeBaseConstIterator<_ti_container>::operator== (const CBTreeBaseConstIterator<_ti_container> &rIter)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator==: Cannot compare if target iterator is not associated with a b-tree!");
	BTREE_ASSERT (rIter.m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator==: Cannot compare if external iterator is not associated with a b-tree!");

	if (&rIter == this)
	{
		return (true);
	}

	return (get_pos () == rIter.get_pos ());
}

template <class _ti_container>
bool CBTreeBaseConstIterator<_ti_container>::operator!= (const CBTreeBaseConstIterator<_ti_container> &rIter)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator!=: Cannot compare if target iterator is not associated with a b-tree!");
	BTREE_ASSERT (rIter.m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator!=: Cannot compare if external iterator is not associated with a b-tree!");

	return ( ! (*this == rIter));
}

template <class _ti_container>
typename CBTreeBaseConstIterator<_ti_container>::data_t& CBTreeBaseConstIterator<_ti_container>::operator* ()
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator*: Indirection not possible if iterator is not associated with a b-tree!");

	BTREE_ASSERT_EXCEPT (m_nPos < m_pContainer->size (), ::std::out_of_range, "ERROR: CBTreeBaseConstIterator::operator*: Cannot dereference if iterator is out of range!");

	this->sync ();

	m_sData = *(this->m_pContainer->get_data (m_nNode, m_nSub));

	return m_sData;
}

template <class _ti_container>
typename CBTreeBaseConstIterator<_ti_container>::data_t* CBTreeBaseConstIterator<_ti_container>::operator-> ()
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator->: Cannot dereference if iterator is not associated with a b-tree!");

	return (&**this);
}

template <class _ti_container>
CBTreeBaseConstIterator<_ti_container>& CBTreeBaseConstIterator<_ti_container>::operator++ ()
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::++operator: Cannot increment if iterator is not associated with a b-tree!");

	increment_by (1);
	
	return (*this);
}

template <class _ti_container>
CBTreeBaseConstIterator<_ti_container> CBTreeBaseConstIterator<_ti_container>::operator++ (int)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator++: Cannot increment if iterator is not associated with a b-tree!");

	CBTreeBaseConstIterator		sTemp (*this);

	increment_by (1);
	
	return (sTemp);
}

template <class _ti_container>
CBTreeBaseConstIterator<_ti_container>& CBTreeBaseConstIterator<_ti_container>::operator-- ()
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::--operator: Cannot increment if iterator is not associated with a b-tree!");

	decrement_by (1);
	
	return (*this);
}

template <class _ti_container>
CBTreeBaseConstIterator<_ti_container> CBTreeBaseConstIterator<_ti_container>::operator-- (int)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator--: Cannot increment if iterator is not associated with a b-tree!");

	CBTreeBaseConstIterator		sTemp (*this);

	decrement_by (1);
	
	return (sTemp);
}

template <class _ti_container>
CBTreeBaseConstIterator<_ti_container>& CBTreeBaseConstIterator<_ti_container>::operator+= (int nOperand)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator+= (int): Cannot increment if iterator is not associated with a b-tree!");

	if (nOperand > 0)
	{
		return (this->operator+= ((sizetype_t) nOperand));
	}
	else
	{
		return (this->operator-= ((sizetype_t) (0 - nOperand)));
	}

	return (*this);
}

template <class _ti_container>
CBTreeBaseConstIterator<_ti_container>& CBTreeBaseConstIterator<_ti_container>::operator+= (sizetype_t nOperand)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator+= (sizetype_t): Cannot increment if iterator is not associated with a b-tree!");

	this->increment_by (nOperand);

	return (*this);
}

template <class _ti_container>
CBTreeBaseConstIterator<_ti_container>& CBTreeBaseConstIterator<_ti_container>::operator+= (const CBTreeBaseConstIterator<_ti_container> &rIter)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator+= (const CBTreeBaseConstIterator &): Cannot increment if iterator is not associated with a b-tree!");

	this->increment_by (rIter.m_nPos);

	return (*this);
}

template <class _ti_container>
CBTreeBaseConstIterator<_ti_container>& CBTreeBaseConstIterator<_ti_container>::operator-= (int nOperand)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator-= (int): Cannot decrement if iterator is not associated with a b-tree!");

	if (nOperand > 0)
	{
		return (this->operator-= ((sizetype_t) nOperand));
	}
	else
	{
		return (this->operator+= ((sizetype_t) (0 - nOperand)));
	}

	return (*this);
}

template <class _ti_container>
CBTreeBaseConstIterator<_ti_container>& CBTreeBaseConstIterator<_ti_container>::operator-= (sizetype_t nOperand)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator-= (sizetype_t): Cannot decrement if iterator is not associated with a b-tree!");

	this->decrement_by (nOperand);

	return (*this);
}

template <class _ti_container>
CBTreeBaseConstIterator<_ti_container>& CBTreeBaseConstIterator<_ti_container>::operator-= (const CBTreeBaseConstIterator<_ti_container> &rIter)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator-= (const CBTreeBaseConstIterator &): Cannot decrement if iterator is not associated with a b-tree!");

	this->decrement_by (rIter.m_nPos);

	return (*this);
}

template <class _ti_container>
const CBTreeBaseConstIterator<_ti_container> CBTreeBaseConstIterator<_ti_container>::operator+ (int nOperand) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator+ (int): Cannot increment if iterator is not associated with a b-tree!");

	CBTreeBaseConstIterator_t		nResult = *this;

	nResult += nOperand;

	return (nResult);
}

template <class _ti_container>
const CBTreeBaseConstIterator<_ti_container> CBTreeBaseConstIterator<_ti_container>::operator+ (sizetype_t nOperand) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator+ (sizetype_t): Cannot increment if iterator is not associated with a b-tree!");

	CBTreeBaseConstIterator_t		nResult = *this;

	nResult += nOperand;

	return (nResult);
}

template <class _ti_container>
const CBTreeBaseConstIterator<_ti_container> CBTreeBaseConstIterator<_ti_container>::operator+ (const CBTreeBaseConstIterator<_ti_container> &rIter) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator+ (const CBTreeBaseConstIterator &): Cannot increment if iterator is not associated with a b-tree!");

	CBTreeBaseConstIterator_t		nResult = *this;

	nResult += rIter;
	
	return (nResult);
}

template <class _ti_container>
const CBTreeBaseConstIterator<_ti_container> CBTreeBaseConstIterator<_ti_container>::operator- (int nOperand) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator- (int): Cannot decrement if iterator is not associated with a b-tree!");

	CBTreeBaseConstIterator_t		nResult = *this;

	nResult -= nOperand;

	return (nResult);
}

template <class _ti_container>
const CBTreeBaseConstIterator<_ti_container> CBTreeBaseConstIterator<_ti_container>::operator- (sizetype_t nOperand) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator- (sizetype_t): Cannot decrement if iterator is not associated with a b-tree!");

	CBTreeBaseConstIterator_t		nResult = *this;

	nResult -= nOperand;

	return (nResult);
}

template <class _ti_container>
const typename CBTreeBaseConstIterator<_ti_container>::sizetype_t CBTreeBaseConstIterator<_ti_container>::operator- (const CBTreeBaseConstIterator<_ti_container> &rIter) const
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::(sizetype_t)operator- (const CBTreeBaseConstIterator &): Cannot decrement if iterator is not associated with a b-tree!");
	
	sizetype_t		nResult;
	
	nResult = this->m_nPos - rIter.m_nPos;
	
	return (nResult);
}

template <class _ti_container>
bool CBTreeBaseConstIterator<_ti_container>::operator< (const CBTreeBaseConstIterator<_ti_container> &rIter)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator<: Cannot compare if target iterator is not associated with a b-tree!");
	BTREE_ASSERT (rIter.m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator<: Cannot compare if external iterator is not associated with a b-tree!");

	return (this->m_nPos < rIter.m_nPos);
}

template <class _ti_container>
bool CBTreeBaseConstIterator<_ti_container>::operator<= (const CBTreeBaseConstIterator<_ti_container> &rIter)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator<=: Cannot compare if target iterator is not associated with a b-tree!");
	BTREE_ASSERT (rIter.m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator<=: Cannot compare if external iterator is not associated with a b-tree!");

	return (this->m_nPos <= rIter.m_nPos);
}

template <class _ti_container>
bool CBTreeBaseConstIterator<_ti_container>::operator> (const CBTreeBaseConstIterator<_ti_container> &rIter)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator>: Cannot compare if target iterator is not associated with a b-tree!");
	BTREE_ASSERT (rIter.m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator>: Cannot compare if external iterator is not associated with a b-tree!");

	return (this->m_nPos > rIter.m_nPos);
}

template <class _ti_container>
bool CBTreeBaseConstIterator<_ti_container>::operator>= (const CBTreeBaseConstIterator<_ti_container> &rIter)
{
	BTREE_ASSERT (this->m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator>=: Cannot compare if target iterator is not associated with a b-tree!");
	BTREE_ASSERT (rIter.m_pContainer != NULL, "ERROR: CBTreeBaseConstIterator::operator>=: Cannot compare if external iterator is not associated with a b-tree!");

	return (this->m_nPos >= rIter.m_nPos);
}

template <class _ti_container>
typename CBTreeBaseConstIterator<_ti_container>::data_t& CBTreeBaseConstIterator<_ti_container>::operator[] (const int nPos)
{
	typedef typename	::CBTreeBaseConstIterator<_ti_container>		iter_t;

	iter_t		nIter ((iter_t)*this);

	nIter += nPos;

	m_sSubScriptResult = *nIter;

	return (m_sSubScriptResult);
}

template <class _ti_container>
typename CBTreeBaseConstIterator<_ti_container>::data_t& CBTreeBaseConstIterator<_ti_container>::operator[] (const typename CBTreeBaseConstIterator<_ti_container>::sizetype_t nPos)
{
	typedef typename	::CBTreeBaseConstIterator<_ti_container>		iter_t;

	iter_t		nIter ((iter_t)*this);

	nIter += nPos;

	m_sSubScriptResult = *nIter;

	return (m_sSubScriptResult);
}

template <class _ti_container>
_ti_container* CBTreeBaseConstIterator<_ti_container>::get_container ()
{
	return (this->m_pContainer);
}

template <class _ti_container>
bool CBTreeBaseConstIterator<_ti_container>::is_btree_valid ()
{
	return (this->m_pContainer != NULL);
}

template <class _ti_container>
bool CBTreeBaseConstIterator<_ti_container>::is_evaluated ()
{
	if (this->is_time_stamp_up_to_data ())
	{
		if (this->m_nPos == this->m_nAssociatedPos)
		{
			return (true);
		}
	}

	return (false);
}

template <class _ti_container>
void CBTreeBaseConstIterator<_ti_container>::sync ()
{
	if (!this->is_time_stamp_up_to_data ())
	{
		this->evaluate ();

		this->update_time_stamp ();
	}
	else
	{
		this->evaluate_by_seek ();
	}
}

template <class _ti_container>
typename CBTreeBaseConstIterator<_ti_container>::sizetype_t CBTreeBaseConstIterator<_ti_container>::get_pos ()
{
	return (this->m_nPos);
}

template <class _ti_container>
typename CBTreeBaseConstIterator<_ti_container>::nodeiter_t CBTreeBaseConstIterator<_ti_container>::get_node ()
{
	return (this->m_nNode);
}

template <class _ti_container>
typename CBTreeBaseConstIterator<_ti_container>::subnodeiter_t CBTreeBaseConstIterator<_ti_container>::get_sub_pos ()
{
	return (this->m_nSub);
}

template <class _ti_container>
void CBTreeBaseConstIterator<_ti_container>::set_time_stamp (_ti_container *pContainer)
{
	BTREE_ASSERT (pContainer == this->m_pContainer, "CBTreeBaseConstIterator<>::set_time_stamp (_ti_container *): Cannot set time stamp if input parameter is not associated with this iterator!");

	this->m_sTimeStamp.pContainer = pContainer;
	this->m_sTimeStamp.sTimeStamp = pContainer->get_time_stamp ();
}

template <class _ti_container>
void CBTreeBaseConstIterator<_ti_container>::increment_by (typename CBTreeBaseConstIterator<_ti_container>::sizetype_t nOperand)
{
	m_nPos += nOperand;
}

template <class _ti_container>
void CBTreeBaseConstIterator<_ti_container>::decrement_by (typename CBTreeBaseConstIterator<_ti_container>::sizetype_t nOperand)
{
	m_nPos -= nOperand;
}

template <class _ti_container>
bool CBTreeBaseConstIterator<_ti_container>::out_of_range ()
{
	return (m_nPos >= m_pContainer->size ());
}

template <class _ti_container>
void CBTreeBaseConstIterator<_ti_container>::evaluate ()
{
	if (m_nPos > m_pContainer->size ())
	{
		return;
	}

	evaluate_from (m_pContainer->m_nRootNode, m_nPos);
}

template <class _ti_container>
void CBTreeBaseConstIterator<_ti_container>::evaluate_from (typename CBTreeBaseConstIterator<_ti_container>::nodeiter_t nFromNode, typename CBTreeBaseConstIterator<_ti_container>::sizetype_t nFromPos)
{
	if (m_pContainer->empty ())
	{
		return;
	}

	m_pContainer->convert_pos_to_container_pos (nFromNode, nFromPos, m_nNode, m_nSub);

	m_nAssociatedPos = m_nPos;
}

template <class _ti_container>
void CBTreeBaseConstIterator<_ti_container>::evaluate_by_seek ()
{
	if (m_nAssociatedPos == m_nPos)
	{
		return;
	}

	if (this->out_of_range ())
	{
		return;
	}

	CBTreeBaseConstIterator_t::nodeiter_t		nNode;
	CBTreeBaseConstIterator_t::subnodeiter_t	nSub;
	CBTreeBaseConstIterator_t::subnodeiter_t	i;
	CBTreeBaseConstIterator_t::sizetype_t		nSize;
	bool										bBounce = false;
	CBTreeBaseConstIterator_t::sizetype_t		nOperand;

	nNode = m_nNode;
	nSub = m_nSub;
	
	if (m_nAssociatedPos < m_nPos)
	{
		nOperand = m_nPos - m_nAssociatedPos;

		m_nAssociatedPos = m_nPos;

		switch (nOperand)
		{
		case	2	:	
			{
				m_pContainer->move_next (nNode, nSub, m_nNode, m_nSub, bBounce);

				BTREE_ASSERT (!bBounce, "ERROR: CBTreeBaseConstIterator::evaluate_by_seek (+2): Unexpected bounce!");

				nNode = m_nNode;
				nSub = m_nSub;
			}

		case	1	:
			{
				m_pContainer->move_next (nNode, nSub, m_nNode, m_nSub, bBounce);

				BTREE_ASSERT (!bBounce, "ERROR: CBTreeBaseConstIterator::evaluate_by_seek (+1): Unexpected bounce!");

				break;
			}

		default		:
			{
				if ((m_pContainer->is_leaf (m_nNode)) && (((CBTreeBaseConstIterator_t::sizetype_t) (m_nSub + nOperand)) < ((CBTreeBaseConstIterator_t::sizetype_t) m_pContainer->get_data_count (m_nNode))))
				{
					m_nSub += (CBTreeBaseConstIterator_t::subnodeiter_t) nOperand;
				}
				else if (nOperand >= m_pContainer->size ())
				{
					evaluate ();
				}
				else
				{
					if (m_pContainer->is_leaf (nNode))
					{
						nOperand -= m_pContainer->get_data_count (nNode) - m_nSub;

						nSub = m_pContainer->get_data_count (nNode) - 1;

						m_pContainer->move_next (nNode, nSub, m_nNode, m_nSub, bBounce);

						BTREE_ASSERT (!bBounce, "ERROR: CBTreeBaseConstIterator::increment_by: Unexpected bounce!");

						nNode = m_nNode;
						nSub = m_nSub;
					}

					while (1)
					{
						nodeiter_t		nSubNode = nNode;

						for (nSize = 0, i = nSub; i < m_pContainer->get_data_count (nNode); i++)
						{
							if (nSize == nOperand)
							{
								m_nSub = i;

								return;
							}

							nSubNode = *(m_pContainer->get_sub_node (nNode, i + 1));

							sizetype_t		nMaxIdx = m_pContainer->get_max_index (nSubNode);

							nSize++;
							nSize += nMaxIdx;

							if (nSize > nOperand)
							{
								nSize -= nMaxIdx;

								nOperand -= nSize;

								break;
							}
						}

						if (i < m_pContainer->get_data_count (nNode))
						{
							nNode = nSubNode;

							break;
						}
						else
						{
							m_nNode = m_pContainer->get_parent (nNode);
							m_nSub = m_pContainer->find_sub_node_offset (m_nNode, nNode);

							nOperand -= nSize;

							nNode = m_nNode;
							nSub = m_nSub;
						}
					}

					evaluate_from (nNode, nOperand);
				}

				break;
			}
		}
	}
	else
	{
		nOperand = m_nAssociatedPos - m_nPos;

		m_nAssociatedPos = m_nPos;

		switch (nOperand)
		{
		case	2	:	
			{
				m_pContainer->move_prev (nNode, nSub, m_nNode, m_nSub, bBounce);

				BTREE_ASSERT (!bBounce, "ERROR: CBTreeBaseConstIterator::evaluate_by_seek (-2): Unexpected bounce!");

				nNode = m_nNode;
				nSub = m_nSub;
			}

		case	1	:
			{
				m_pContainer->move_prev (nNode, nSub, m_nNode, m_nSub, bBounce);

				BTREE_ASSERT (!bBounce, "ERROR: CBTreeBaseConstIterator::evaluate_by_seek (-1): Unexpected bounce!");

				break;
			}

		default		:
			{
				if ((m_pContainer->is_leaf (m_nNode)) && ((((CBTreeBaseConstIterator_t::sizetype_t) m_nSub) >= nOperand)))
				{
					m_nSub -= (CBTreeBaseConstIterator_t::subnodeiter_t) nOperand;
				}
				else if (nOperand >= m_pContainer->size ())
				{
					evaluate ();
				}
				else
				{
					if (m_pContainer->is_leaf (nNode))
					{
						nOperand -= m_nSub + 1;

						nSub = 0;

						m_pContainer->move_prev (nNode, nSub, m_nNode, m_nSub, bBounce);

						BTREE_ASSERT (!bBounce, "ERROR: CBTreeBaseConstIterator::decrement_by: Unexpected bounce!");

						nNode = m_nNode;
						nSub = m_nSub;

						if (nOperand == 0)
						{
							return;
						}
					}

					while (1)
					{
						CBTreeBaseConstIterator_t::nodeiter_t	nSubNode = nNode;
						bool									bEvaluate = false;

						for (nSize = 0, i = nSub + 1; i > 0; )
						{
							i--;

							nSubNode = *(m_pContainer->get_sub_node (nNode, i));

							CBTreeBaseConstIterator_t::sizetype_t	nMaxIdx = m_pContainer->get_max_index (nSubNode);

							nSize += nMaxIdx;
							nSize++;

							if ((nSize == nOperand) && (i > 0))
							{
								m_nSub = i - 1;

								return;
							}

							if (nSize > nOperand)
							{
								nSize--;

								nOperand = nSize - nOperand;

								bEvaluate = true;

								break;
							}
						}

						if (bEvaluate)
						{
							nNode = nSubNode;

							break;
						}
						else
						{
							do
							{
								m_nNode = m_pContainer->get_parent (nNode);
								m_nSub = m_pContainer->find_sub_node_offset (m_nNode, nNode);

								nNode = m_nNode;
							} while (m_nSub == 0);

							m_nSub--;

							nOperand -= nSize;

							nNode = m_nNode;
							nSub = m_nSub;

							if (nOperand == 0)
							{
								return;
							}
						}
					}

					evaluate_from (nNode, nOperand);
				}

				break;
			}
		}
	}
}

template <class _ti_container>
typename CBTreeBaseConstIterator<_ti_container>::sizetype_t CBTreeBaseConstIterator<_ti_container>::get_pos () const
{
	return (m_nPos);
}

template <class _ti_container>
bool CBTreeBaseConstIterator<_ti_container>::is_time_stamp_up_to_data ()
{
	if (this->m_sTimeStamp.pContainer == NULL)
	{
		return (false);
	}

	if (this->m_sTimeStamp.pContainer != this->m_pContainer)
	{
		return (false);
	}

	if (this->m_sTimeStamp.sTimeStamp != this->m_pContainer->get_time_stamp ())
	{
		return (false);
	}

	return (true);
}

template <class _ti_container>
void CBTreeBaseConstIterator<_ti_container>::invalidate_time_stamp ()
{
	this->m_sTimeStamp.pContainer = NULL;
	this->m_sTimeStamp.sTimeStamp.nAccCtr = 0;
	this->m_sTimeStamp.sTimeStamp.sTime = 0;
}

template <class _ti_container>
void CBTreeBaseConstIterator<_ti_container>::update_time_stamp ()
{
	if (this->m_sTimeStamp.pContainer != NULL)
	{
		if (this->m_sTimeStamp.pContainer != this->m_pContainer)
		{
			this->unregister_iterator (this->m_sTimeStamp.pContainer);

			this->register_iterator (this->m_pContainer);
		}
	}

	this->m_sTimeStamp.pContainer = this->m_pContainer;

	if (this->m_pContainer != NULL)
	{
		this->m_sTimeStamp.sTimeStamp = this->m_pContainer->get_time_stamp ();
	}
}

template <class _ti_container>
float CBTreeBaseConstIterator<_ti_container>::get_average_node_hops (sizetype_t nHopDistance) const
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

template <class _ti_container>
void CBTreeBaseConstIterator<_ti_container>::assign (const CBTreeBaseConstIterator<_ti_container> &rIter)
{
	if (this != &rIter)
	{
		bool	bReRegister = false;

		if (m_pContainer != rIter.m_pContainer)
		{
			if (m_pContainer != NULL)
			{
				this->unregister_iterator (this->m_pContainer);

				bReRegister = true;
			}
		}

		m_pContainer = rIter.m_pContainer;
		m_nPos = rIter.m_nPos;
		m_nAssociatedPos = rIter.m_nAssociatedPos;
		m_nNode = rIter.m_nNode;
		m_nSub = rIter.m_nSub;
		m_sTimeStamp = rIter.m_sTimeStamp;

		if ((bReRegister) || (m_pContainer == rIter.m_pContainer))
		{
			if (m_pContainer != NULL)
			{
				this->register_iterator (this->m_pContainer);
			}
		}
	}
}

template <class _ti_container>
void CBTreeBaseConstIterator<_ti_container>::register_iterator (_ti_container *pContainer)
{
	if (pContainer != NULL)
	{
		pContainer->register_iterator (reinterpret_cast<typename _ti_container::const_iterator * const> (this));
	}
}

template <class _ti_container>
void CBTreeBaseConstIterator<_ti_container>::unregister_iterator (_ti_container *pContainer)
{
	if (pContainer != NULL)
	{
		pContainer->unregister_iterator (reinterpret_cast<typename _ti_container::const_iterator * const> (this));
	}
}

template <class _ti_container>
typename ::CBTreeBaseConstIterator<_ti_container> operator+
(
	typename _ti_container::sizetype_t						nLhs, 
	const typename ::CBTreeBaseConstIterator<_ti_container>	&nRhs
)
{
	typename ::CBTreeBaseConstIterator<_ti_container>		nResult (nRhs);

	nResult += nLhs;

	return (nResult);
}

template <class _ti_container>
typename ::CBTreeBaseConstIterator<_ti_container> operator+
(
	int nLhs, 
	const typename ::CBTreeBaseConstIterator<_ti_container> &nRhs
)
{
	typename ::CBTreeBaseConstIterator<_ti_container>		nResult (nRhs);

	nResult += nLhs;

	return (nResult);
}

template <class _ti_container>
CBTreeBaseIterator<_ti_container>::CBTreeBaseIterator ()
	:	CBTreeBaseConstIterator<_ti_container> ()
{
}

template <class _ti_container>
CBTreeBaseIterator<_ti_container>::CBTreeBaseIterator (_ti_container *pContainer, sizetype_t nPos, bool bRegister)
	:	CBTreeBaseConstIterator<_ti_container> (pContainer, nPos, bRegister)
{
}

template <class _ti_container>
CBTreeBaseIterator<_ti_container>::CBTreeBaseIterator (const CBTreeBaseIterator<_ti_container> &rIter, bool bRegister)
	:	CBTreeBaseConstIterator<_ti_container> (rIter, bRegister)
{
}

template <class _ti_container>
CBTreeBaseIterator<_ti_container>::CBTreeBaseIterator (const CBTreeBaseReverseIterator<CBTreeBaseIterator<_ti_container> > &rRIter, bool bRegister)
	:	CBTreeBaseConstIterator<_ti_container> (rRIter.base ())
{
}

template <class _ti_container>
CBTreeBaseIterator<_ti_container>::CBTreeBaseIterator (const CBTreeBaseConstIterator<_ti_container> &rIter, bool bRegister)
	:	CBTreeBaseConstIterator<_ti_container> (rIter, bRegister)
{
}

template <class _ti_container>
CBTreeBaseIterator<_ti_container>::~CBTreeBaseIterator ()
{
}

template <class _ti_container>
const typename CBTreeBaseIterator<_ti_container>::data_t & CBTreeBaseIterator<_ti_container>::operator* () const
{
	CBTreeBaseConstIterator<_ti_container>			&rCThis (*this);

	return (&**rCThis);
}

template <class _ti_container>
CBTreeBaseIteratorSubScriptWrapper<_ti_container> CBTreeBaseIterator<_ti_container>::operator* ()
{
	return (CBTreeBaseIteratorSubScriptWrapper<_ti_container> (*this, (sizetype_t) 0));
}

template <class _ti_container>
CBTreeBaseIterator<_ti_container>& CBTreeBaseIterator<_ti_container>::operator= (const CBTreeBaseIterator<_ti_container> &rIter)
{
	this->assign (rIter);

	return (*this);
}

template <class _ti_container>
CBTreeBaseIteratorSubScriptWrapper<_ti_container> CBTreeBaseIterator<_ti_container>::operator[] (const int nPos)
{
	return (CBTreeBaseIteratorSubScriptWrapper<_ti_container> (*this, (sizetype_t) nPos));
}

template <class _ti_container>
CBTreeBaseIteratorSubScriptWrapper<_ti_container> CBTreeBaseIterator<_ti_container>::operator[] (const sizetype_t nPos)
{
	return (CBTreeBaseIteratorSubScriptWrapper<_ti_container> (*this, nPos));
}

template <class _ti_container>
void CBTreeBaseIterator<_ti_container>::set (const data_t &rData)
{
	this->sync ();

	BTREE_ASSERT_EXCEPT (!this->out_of_range (), ::std::out_of_range, "ERROR: CBTreeBase<>::CBTreeBaseIterator<>::set (const data_t &): Cannot dereference if iterator is out of range!");

	this->m_pContainer->set_data (this->m_nNode, this->m_nSub, rData);
}

template <class _ti_container>
typename ::CBTreeBaseIterator<_ti_container> operator+
(
	typename _ti_container::sizetype_t					nLhs, 
	const typename ::CBTreeBaseIterator<_ti_container>	&nRhs
)
{
	typename ::CBTreeBaseIterator<_ti_container>		nResult (nRhs);

	nResult += nLhs;

	return (nResult);
}

template <class _ti_container>
typename ::CBTreeBaseIterator<_ti_container> operator+
(
	int nLhs, 
	const typename ::CBTreeBaseIterator<_ti_container> &nRhs
)
{
	typename ::CBTreeBaseIterator<_ti_container>		nResult (nRhs);

	nResult += nLhs;

	return (nResult);
}

template <class _ti_container>
CBTreeBaseIteratorSubScriptWrapper<_ti_container>::CBTreeBaseIteratorSubScriptWrapper
(
	const CBTreeBaseIterator<_ti_container>									&rInstance, 
	typename CBTreeBaseIteratorSubScriptWrapper<_ti_container>::sizetype_t	nPos
)
	:	m_nOffset (rInstance)
	,	m_nPos (nPos)
{
	m_nOffset += m_nPos;
}

template <class _ti_container>
CBTreeBaseIteratorSubScriptWrapper<_ti_container>::~CBTreeBaseIteratorSubScriptWrapper ()
{
}

template <class _ti_container>
CBTreeBaseIteratorSubScriptWrapper<_ti_container> &
CBTreeBaseIteratorSubScriptWrapper<_ti_container>::operator= (const typename CBTreeBaseIteratorSubScriptWrapper<_ti_container>::data_t &rData)
{
	m_nOffset.set (rData);

	return (*this);
}

template <class _ti_container>
CBTreeBaseIteratorSubScriptWrapper<_ti_container>::operator const typename CBTreeBaseIteratorSubScriptWrapper<_ti_container>::data_t & ()
{
	m_sData = *(dynamic_cast <CBTreeBaseConstIterator<_ti_container> &> (m_nOffset));

	return (m_sData);
}

template <class _t_iterator>
CBTreeBaseConstReverseIterator<_t_iterator>::CBTreeBaseConstReverseIterator ()
	:	::std::reverse_iterator<_t_iterator> ()
{
}

template <class _t_iterator>
CBTreeBaseConstReverseIterator<_t_iterator>::CBTreeBaseConstReverseIterator (const _t_iterator &rIter)
	:	::std::reverse_iterator<_t_iterator> (rIter)
{
}

template <class _t_iterator>
CBTreeBaseConstReverseIterator<_t_iterator>::CBTreeBaseConstReverseIterator (const CBTreeBaseConstReverseIterator<_t_iterator> &rIter)
	:	::std::reverse_iterator<_t_iterator> (rIter)
{
}

template <class _t_iterator>
CBTreeBaseConstReverseIterator<_t_iterator>::~CBTreeBaseConstReverseIterator	()
{
}

template <class _t_iterator>
void CBTreeBaseConstReverseIterator<_t_iterator>::swap (CBTreeBaseConstReverseIterator &rRight)
{
	CBTreeBaseConstReverseIterator<_t_iterator>		sTmp (rRight);

	rRight = *this;

	*this = sTmp;
}

template <class _t_iterator>
CBTreeBaseConstReverseIterator<_t_iterator>& CBTreeBaseConstReverseIterator<_t_iterator>::operator= (const reverse_iterator<_t_iterator> &rIter)
{
	CBTreeBaseConstReverseIterator<_t_iterator>		sIter (rIter.base ());

	*this = sIter;

	return (*this);
}

template <class _t_iterator>
bool CBTreeBaseConstReverseIterator<_t_iterator>::operator== (const CBTreeBaseConstReverseIterator<_t_iterator> &rIter)
{
	return (this->base () == rIter.base ());
}

template <class _t_iterator>
bool CBTreeBaseConstReverseIterator<_t_iterator>::operator!= (const CBTreeBaseConstReverseIterator<_t_iterator> &rIter)
{
	return ( ! (*this == rIter) );
}

template <class _t_iterator>
typename CBTreeBaseConstReverseIterator<_t_iterator>::data_t& CBTreeBaseConstReverseIterator<_t_iterator>::operator* ()
{
	_t_iterator		nIter (this->base ());

	nIter--;

	m_sSubScriptResult = *nIter;

	return (m_sSubScriptResult);
}

template <class _t_iterator>
typename CBTreeBaseConstReverseIterator<_t_iterator>::data_t* CBTreeBaseConstReverseIterator<_t_iterator>::operator-> ()
{
	return (&**this);
}

	
template <class _t_iterator>
typename CBTreeBaseConstReverseIterator<_t_iterator>::data_t& CBTreeBaseConstReverseIterator<_t_iterator>::operator[] (const int nPos)
{	
	typedef typename	::CBTreeBaseConstReverseIterator<_t_iterator>		iter_t;

	iter_t		nIter ((iter_t)*this);

	nIter += nPos;

	m_sSubScriptResult = *nIter;

	return (m_sSubScriptResult);
}

template <class _t_iterator>
typename CBTreeBaseConstReverseIterator<_t_iterator>::data_t& CBTreeBaseConstReverseIterator<_t_iterator>::operator[] (const typename CBTreeBaseConstReverseIterator<_t_iterator>::sizetype_t nPos)
{
	typedef typename	::CBTreeBaseConstReverseIterator<_t_iterator>		iter_t;

	iter_t		nIter ((iter_t)*this);

	nIter += nPos;

	m_sSubScriptResult = *nIter;

	return (m_sSubScriptResult);
}	

template <class _t_iterator>
CBTreeBaseReverseIterator<_t_iterator>::CBTreeBaseReverseIterator ()
	:	CBTreeBaseConstReverseIterator<_t_iterator> ()
{
}

template <class _t_iterator>
CBTreeBaseReverseIterator<_t_iterator>::CBTreeBaseReverseIterator (const _t_iterator &rIter)
	:	CBTreeBaseConstReverseIterator<_t_iterator> (rIter)
{
}

template <class _t_iterator>
CBTreeBaseReverseIterator<_t_iterator>::CBTreeBaseReverseIterator (const CBTreeBaseReverseIterator<_t_iterator> &rIter)
	:	CBTreeBaseConstReverseIterator<_t_iterator> (rIter)
{
}

template <class _t_iterator>
CBTreeBaseReverseIterator<_t_iterator>::~CBTreeBaseReverseIterator ()
{
}

template <class _t_iterator>
const typename CBTreeBaseReverseIterator<_t_iterator>::data_t & CBTreeBaseReverseIterator<_t_iterator>::operator* () const
{
	CBTreeBaseConstReverseIterator<_t_iterator>			&rCThis (*this);

	return (*rCThis);
}

template <class _t_iterator>
CBTreeBaseIteratorSubScriptWrapper<typename _t_iterator::container_t> CBTreeBaseReverseIterator<_t_iterator>::operator* ()
{
	CBTreeBaseReverseIterator<_t_iterator>	sRIter (*this);

	++sRIter;

	CBTreeBaseIterator<container_t>	sIter (sRIter, false);

	return (CBTreeBaseIteratorSubScriptWrapper<container_t> (sIter, (sizetype_t) 0));
}

template <class _t_iterator>
CBTreeBaseReverseIterator<_t_iterator>& CBTreeBaseReverseIterator<_t_iterator>::operator= (const reverse_iterator<_t_iterator> &rIter)
{
	CBTreeBaseReverseIterator<_t_iterator>		sIter (rIter.base ());

	*this = sIter;

	return (*this);
}

template <class _t_iterator>
CBTreeBaseReverseIterator<_t_iterator>& CBTreeBaseReverseIterator<_t_iterator>::operator= (const data_t &rData)
{
	_t_iterator				sIter (this->base ());

	sIter++;

	sIter = rData;

	return (*this);
}

template <class _t_iterator>
void CBTreeBaseReverseIterator<_t_iterator>::set (const data_t &rData)
{
	this->sync ();

	BTREE_ASSERT_EXCEPT (!this->out_of_range (), ::std::out_of_range, "ERROR: CBTreeBase<>::CBTreeBaseIterator<>::set (const data_t &): Cannot dereference if iterator is out of range!");

	this->m_pContainer->set_data (this->m_nNode, this->m_nSub, rData);
}

template <class _t_iterator>
CBTreeBaseIteratorSubScriptWrapper<typename _t_iterator::container_t> CBTreeBaseReverseIterator<_t_iterator>::operator[] (const int nPos)
{
	int		nRevPos = 0 - nPos;

	CBTreeBaseReverseIterator<_t_iterator>	sRIter (*this);

	++sRIter;

	CBTreeBaseIterator<container_t>	sIter (sRIter, false);
	
	return (CBTreeBaseIteratorSubScriptWrapper<container_t> (sIter, (sizetype_t) nRevPos));
}

template <class _t_iterator>
CBTreeBaseIteratorSubScriptWrapper<typename _t_iterator::container_t> CBTreeBaseReverseIterator<_t_iterator>::operator[] (const typename _t_iterator::container_t::sizetype_t nPos)
{
	typename _t_iterator::sizetype_t		nRevPos = nPos - 1;

	nRevPos = ~nRevPos;

	CBTreeBaseReverseIterator<_t_iterator>	sRIter (*this);

	++sRIter;

	CBTreeBaseIterator<container_t>	sIter (sRIter, false);

	return (CBTreeBaseIteratorSubScriptWrapper<container_t> (sIter, nRevPos));
}

#endif	// BTREEITER_CPP
