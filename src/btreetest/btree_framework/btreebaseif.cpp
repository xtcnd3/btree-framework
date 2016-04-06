/************************************************************
**
** file:	btreebaseif.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree frameworks base class
** on the low level interface level.
**
************************************************************/

#ifndef	BTREEBASEIF_CPP
#define	BTREEBASEIF_CPP

#include "btreebaseif.h"

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::CBTreeBaseIf (const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize)
	:	CBTreeDefaults<_t_data, _t_sizetype> (psCacheDescription)
	,	m_nRootNode (0ULL)
	,	m_nTreeSize (0ULL)
	,	m_nNodeSize (nNodeSize)
	,	m_nGrowBy (64UL)
	,	m_nAutoShrinkThreshold (nNodeSize * 2)
	,	m_nAutoShrinkCounter (0)
{
	m_nRebuildFIFOSize = 0UL;
	m_pRebuildFIFO = NULL;
	m_nRebuildFIFOreadPos = 0ULL;
	m_nRebuildFIFOwritePos = 0ULL;

	m_nNextAlloc = 0ULL;

#if defined (_DEBUG)
	m_pNodeTrace = NULL;
	m_nNodeTraceDepth = 0UL;
	m_nNodeTraceDepthMax = 0UL;
#endif

	setup_pools ();
}

/*

Copy Constructor

rBT		-	reference to CBTreeBaseDefaults instance to be copied

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::CBTreeBaseIf (const CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> &rBT)
	:	CBTreeDefaults<_t_data, _t_sizetype> (dynamic_cast <const CBTreeDefaults<_t_data, _t_sizetype> &> (rBT))
	,	m_nAutoShrinkThreshold (rBT.m_nAutoShrinkThreshold)
	,	m_nAutoShrinkCounter (0)
{
	m_nRootNode = 0ULL;
	m_nTreeSize = 0ULL;

	m_nNodeSize = rBT.m_nNodeSize;

	m_nGrowBy = rBT.m_nGrowBy;

	m_nRebuildFIFOSize = 0UL;
	m_pRebuildFIFO = NULL;
	m_nRebuildFIFOreadPos = 0ULL;
	m_nRebuildFIFOwritePos = 0ULL;

	m_nNextAlloc = 0ULL;

#if defined (_DEBUG)
	m_pNodeTrace = NULL;
	m_nNodeTraceDepth = 0UL;
	m_nNodeTraceDepthMax = 0UL;
#endif

	setup_pools ();
}

/*

Destructor

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::~CBTreeBaseIf ()
{
	if (m_pRebuildFIFO != NULL)
	{
		delete [] m_pRebuildFIFO;
	}

#if defined (_DEBUG)

	if (m_pNodeTrace != NULL)
	{
		delete [] m_pNodeTrace;
	}

#endif
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> &CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::operator=
	(const CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> &rBT)
{
	// if this is not the same instance as the referenced b-tree (rBT) ...
	if (this != &rBT)
	{
		CBTreeDefaults_t			&rBaseIterThis = dynamic_cast<CBTreeDefaults_t &> (*this);
		const CBTreeDefaults_t	&rBaseIterBT = dynamic_cast<const CBTreeDefaults_t &> (rBT);
		
		rBaseIterThis = rBaseIterBT;
	}

	return (*this);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
void CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::convert_pos_to_container_pos (_t_nodeiter nNode, _t_sizetype nPos, _t_nodeiter &rRsltNode, _t_subnodeiter &rRsltSubPos) const
{
	_t_subnodeiter							nMax;
	_t_subnodeiter							nMin;
	_t_subnodeiter							nMid;
	bool									isLeaf = is_leaf (nNode);
	_t_sizetype								nMaxIdx;
	_t_nodeiter								*pnSubNode;
	_t_sizetype								*pnSerVector;

	// if nNode is inner node ...
	if (!isLeaf)
	{
		// ... then search for the next sub node or find data in current node
		
		// if the linear position is smaller than the node size parameter ...
		if (nPos < (this->m_nNodeSize - 1))
		{
			// ... then the resulting location is guaranteed to be in the initial sub-node
			nMax = 0;

			// Hence, searching is skipped.
		}
		else
		{
			// ... otherwise prepare for searching
			pnSerVector = this->get_serVector (nNode);

			nMax = this->get_data_count (nNode);
		}

		nMin = 0;

		while (nMax != nMin)
		{
			nMid = (nMax + nMin) / 2;

			if (nMid >= this->get_data_count (nNode))
			{
				nMaxIdx = this->get_max_index (nNode);
			}
			else
			{
				nMaxIdx = pnSerVector[nMid];
			}

			if (nPos == nMaxIdx)
			{ // if data is in current node, then return with result immediately

				rRsltNode = nNode;
				rRsltSubPos = nMid;
				
				return;
			}

			if (nPos < nMaxIdx)
			{
				nMax = nMid;
			}
			else
			{
				nMin = nMid + 1;
			}
		}

		if (nMin > 0)
		{
			if (nMin >= this->get_data_count (nNode))
			{
				nPos -= pnSerVector[this->get_data_count (nNode) - 1] + 1;
			}
			else
			{
				nPos -= pnSerVector[nMin - 1] + 1;
			}
		}

		pnSubNode = get_sub_node (nNode, nMin);

		convert_pos_to_container_pos (*pnSubNode, nPos, rRsltNode, rRsltSubPos);
	}
	else
	{
#if defined (_DEBUG)
		BTREE_ASSERT (nPos < 0x80000000ULL, "CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::convert_pos_to_container_pos: nodes sub element addressing exceeds possible node size");
#endif
		// ... otherwise return with result here

		rRsltNode = nNode;
		rRsltSubPos = (_t_subnodeiter) (nPos);
	}
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
_t_subnodeiter CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::get_node_max_sub_nodes () const
{// returns the maximum number of sub node entries per node
	return (m_nNodeSize * 2);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
_t_subnodeiter CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::get_node_max_data_size () const
{// returns the maximum number of data entries per node
	return ((m_nNodeSize * 2) - 1);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
void CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::rebuild_FIFO_put (_t_nodeiter nNode)
{
#if defined (_DEBUG)
	BTREE_ASSERT (rebuild_FIFO_fillstate () <= m_nRebuildFIFOSize, "CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::rebuild_FIFO_put: FIFO overflow");
#endif

	// if rebuild FIFO is full ...
	if (rebuild_FIFO_fillstate () == m_nRebuildFIFOSize)
	{
		// ... then allocate more space, copy current content to new space and destroy old space
		uint32_t		u;
		uint32_t		oldFillState = rebuild_FIFO_fillstate ();
		uint32_t		newRebuildFIFOsize = (m_nRebuildFIFOSize == 0) ? 2 : m_nRebuildFIFOSize << 1;
		_t_nodeiter		*pNew = new _t_nodeiter [newRebuildFIFOsize];

		BTREE_ASSERT (pNew != NULL, "CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::rebuild_FIFO_put: insufficient memory");

		// copy rebuild FIFO content to new space
		for (u = 0UL; rebuild_FIFO_fillstate () > 0UL; u++)
		{
			pNew[u] = rebuild_FIFO_get ();
		}

		// update FIFO state according to new space
		m_nRebuildFIFOwritePos = oldFillState;
		m_nRebuildFIFOreadPos = 0UL;

		// if old space is valid ...
		if (m_pRebuildFIFO != NULL)
		{
			// ... then delete old space
			delete [] m_pRebuildFIFO;
		}

		// assign new space and maximum size to member variables
		m_pRebuildFIFO = pNew;
		m_nRebuildFIFOSize = newRebuildFIFOsize;
	}

	// put node into FIFO and increment write pointer
	m_pRebuildFIFO[(m_nRebuildFIFOwritePos++ & (m_nRebuildFIFOSize - 1UL))] = nNode;
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
_t_nodeiter CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::rebuild_FIFO_peek ()
{
#if defined (_DEBUG)
	BTREE_ASSERT (m_nRebuildFIFOreadPos < m_nRebuildFIFOwritePos, "CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::rebuild_FIFO_peek: rebuild FIFO underrun");
#endif
	
	// return current node
	return (m_pRebuildFIFO[m_nRebuildFIFOreadPos & (m_nRebuildFIFOSize - 1UL)]);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
_t_nodeiter CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::rebuild_FIFO_get ()
{
#if defined (_DEBUG)
	BTREE_ASSERT (m_nRebuildFIFOreadPos < m_nRebuildFIFOwritePos, "CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::rebuild_FIFO_get: rebuild FIFO underrun");
#endif

	// if both the read and write pointer are exceeding the FIFO space ...
	while ((m_nRebuildFIFOreadPos > m_nRebuildFIFOSize) && (m_nRebuildFIFOwritePos > m_nRebuildFIFOSize))
	{
		// ... then rewind both until at least one is within FIFO space again
		m_nRebuildFIFOreadPos -= m_nRebuildFIFOSize;
		m_nRebuildFIFOwritePos -= m_nRebuildFIFOSize;
	}

	// return current node and increment read pointer
	return (m_pRebuildFIFO[m_nRebuildFIFOreadPos++ & (m_nRebuildFIFOSize - 1UL)]);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
uint32_t CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::rebuild_FIFO_fillstate ()
{
	// return number of items in rebuild FIFO
	// for this to work the write position must always be bigger than the read position
	return (m_nRebuildFIFOwritePos - m_nRebuildFIFOreadPos);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
void CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::attache_traced_node (_t_nodeiter nNode)
{
#if defined (_DEBUG)
	// only valid in debug

	// if node trace buffer not allocate ...
	if (m_pNodeTrace == NULL)
	{
		// ... then set minimum size and allocate trace buffer space
		m_nNodeTraceDepthMax = 1UL;

		m_pNodeTrace = new _t_nodeiter [m_nNodeTraceDepthMax];

		BTREE_ASSERT (m_pNodeTrace != NULL, "CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::attache_traced_node: insufficient memory!");
	}

	// if trace buffer is full ...
	if (m_nNodeTraceDepth >= m_nNodeTraceDepthMax)
	{
		// ... then allocate more a larger space, copy current buffer to larger space and de-allocte current buffer
		_t_nodeiter			*pNodeTraceTemp;
		uint32_t			nNodeTraceDepthTemp = m_nNodeTraceDepthMax;
		uint32_t			ui32;

		// allocte space twice as large than current space
		m_nNodeTraceDepthMax <<= 1UL;

		pNodeTraceTemp = new _t_nodeiter [m_nNodeTraceDepthMax];

		BTREE_ASSERT (m_pNodeTrace != NULL, "CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::attache_traced_node: insufficient memory!");

		// copy content to new space
		for (ui32 = 0; ui32 < nNodeTraceDepthTemp; ui32++)
		{
			pNodeTraceTemp[ui32] = m_pNodeTrace[ui32];
		}

		// de-allocate current trace buffer
		delete [] m_pNodeTrace;

		// assign new buffer as current buffer
		m_pNodeTrace = pNodeTraceTemp;
	}

	// put node into trace buffer and update pointer
	m_pNodeTrace[m_nNodeTraceDepth] = nNode;

	m_nNodeTraceDepth++;
#endif
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
void CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::detache_traced_node ()
{
#if defined (_DEBUG)
	// if trace buffer present ...
	if (m_pNodeTrace != NULL)
	{
		// ... then detach current node
		m_nNodeTraceDepth--;
	}
#endif
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
float CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::get_average_access_depth ()
{
	if (this->empty ())
	{
		return (0.0f);
	}

	if (this->is_leaf (this->m_nRootNode))
	{
		return (1.0f);
	}

	float		fRetval = 1.0f;
	float		fRangeIncPerDepth;
	float		fSize = (float) this->size ();
	float		fRange = 1.0;
	float		fPrevRange;

	fRangeIncPerDepth = (float) this->get_node_max_sub_nodes ();
	fRangeIncPerDepth *= 0.75f;

	do
	{
		fRetval += 1.0f;

		fPrevRange = fRange;

		fRange *= fRangeIncPerDepth;
	} while (fRange < fSize);

	fRetval -= (fRange - fSize) / (fRange - fPrevRange);

	return (fRetval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
uint32_t CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::get_iter_state_size () const
{
	return ((uint32_t) sizeof (iterator_state_t));
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
void CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::reset_iter_state (void *pState) const
{
	iterator_state_t	*pIterState = (iterator_state_t *) pState;

	pIterState->nAssociatedPos = ~0x0;
	pIterState->nNode = ~0x0;
	pIterState->nSubPos = ~0x0;
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
void CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::evaluate_iter (void *pState, _t_sizetype nOffsetPos) const
{
	iterator_state_t	*pIterState = (iterator_state_t *) pState;

	if (nOffsetPos >= this->size ())
	{
		return;
	}

	this->convert_pos_to_container_pos (this->m_nRootNode, nOffsetPos, pIterState->nNode, pIterState->nSubPos);

	pIterState->nAssociatedPos = nOffsetPos;
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
void CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::evaluate_iter_by_seek (void *pState, _t_sizetype nNewPos) const
{
	iterator_state_t	*pIterState = (iterator_state_t *) pState;

	if (pIterState->nAssociatedPos == nNewPos)
	{
		return;
	}

	if (nNewPos >= this->size ())
	{
		return;
	}

	_t_nodeiter			nNode = pIterState->nNode;
	_t_subnodeiter		nSub = pIterState->nSubPos;
	_t_subnodeiter		i;
	_t_sizetype			nSize;
	bool				bBounce = false;
	_t_sizetype			nOperand;

	if (pIterState->nAssociatedPos < nNewPos)
	{
		nOperand = nNewPos - pIterState->nAssociatedPos;

		pIterState->nAssociatedPos = nNewPos;

		switch (nOperand)
		{
		case	2	:	
			{
				move_next (nNode, nSub, pIterState->nNode, pIterState->nSubPos, bBounce);

				BTREE_ASSERT (!bBounce, "CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::evaluate_iter_by_seek (+2): Unexpected bounce!");

				nNode = pIterState->nNode;
				nSub = pIterState->nSubPos;
			}

		case	1	:
			{
				move_next (nNode, nSub, pIterState->nNode, pIterState->nSubPos, bBounce);

				BTREE_ASSERT (!bBounce, "CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::evaluate_iter_by_seek (+1): Unexpected bounce!");

				break;
			}

		default		:
			{
				if ((is_leaf (pIterState->nNode)) && (((_t_sizetype) (pIterState->nSubPos + nOperand)) < ((_t_sizetype) get_data_count (pIterState->nNode))))
				{
					pIterState->nSubPos += (_t_subnodeiter) nOperand;
				}
				else
				{
					if (is_leaf (nNode))
					{
						nOperand -= this->get_data_count (nNode) - pIterState->nSubPos;

						nSub = this->get_data_count (nNode) - 1;

						this->move_next (nNode, nSub, pIterState->nNode, pIterState->nSubPos, bBounce);

						BTREE_ASSERT (!bBounce, "CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::evaluate_iter_by_seek (+): Unexpected bounce!");

						nNode = pIterState->nNode;
						nSub = pIterState->nSubPos;
					}

					while (1)
					{
						_t_nodeiter		nSubNode = nNode;

						for (nSize = 0, i = nSub; i < this->get_data_count (nNode); i++)
						{
							if (nSize == nOperand)
							{
								pIterState->nSubPos = i;

								return;
							}

							nSubNode = *(this->get_sub_node (nNode, i + 1));

							_t_sizetype		nMaxIdx = this->get_max_index (nSubNode);

							nSize++;
							nSize += nMaxIdx;

							if (nSize > nOperand)
							{
								nSize -= nMaxIdx;

								nOperand -= nSize;

								break;
							}
						}

						if (i < this->get_data_count (nNode))
						{
							nNode = nSubNode;

							break;
						}
						else
						{
							pIterState->nNode = this->get_parent (nNode);
							pIterState->nSubPos = this->find_sub_node_offset (pIterState->nNode, nNode);

							nOperand -= nSize;

							nNode = pIterState->nNode;
							nSub = pIterState->nSubPos;
						}
					}

					this->convert_pos_to_container_pos (nNode, nOperand, pIterState->nNode, pIterState->nSubPos);
				}

				break;
			}
		}
	}
	else
	{
		nOperand = pIterState->nAssociatedPos - nNewPos;

		pIterState->nAssociatedPos = nNewPos;

		switch (nOperand)
		{
		case	2	:	
			{
				move_prev (nNode, nSub, pIterState->nNode, pIterState->nSubPos, bBounce);

				BTREE_ASSERT (!bBounce, "CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::evaluate_iter_by_seek (-2): Unexpected bounce!");

				nNode = pIterState->nNode;
				nSub = pIterState->nSubPos;
			}

		case	1	:
			{
				move_prev (nNode, nSub, pIterState->nNode, pIterState->nSubPos, bBounce);

				BTREE_ASSERT (!bBounce, "CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::evaluate_iter_by_seek (-1): Unexpected bounce!");

				break;
			}

		default		:
			{
				if ((this->is_leaf (pIterState->nNode)) && ((((_t_sizetype) pIterState->nSubPos) >= nOperand)))
				{
					pIterState->nSubPos -= (_t_subnodeiter) nOperand;
				}
				else
				{
					if (this->is_leaf (nNode))
					{
						nOperand -= pIterState->nSubPos + 1;

						nSub = 0;

						this->move_prev (nNode, nSub, pIterState->nNode, pIterState->nSubPos, bBounce);

						BTREE_ASSERT (!bBounce, "CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::evaluate_iter_by_seek (-): Unexpected bounce!");

						nNode = pIterState->nNode;
						nSub = pIterState->nSubPos;

						if (nOperand == 0)
						{
							return;
						}
					}

					while (1)
					{
						_t_nodeiter	nSubNode = nNode;
						bool		bEvaluate = false;

						for (nSize = 0, i = nSub + 1; i > 0; )
						{
							i--;

							nSubNode = *(this->get_sub_node (nNode, i));

							_t_sizetype	nMaxIdx = this->get_max_index (nSubNode);

							nSize += nMaxIdx;
							nSize++;

							if ((nSize == nOperand) && (i > 0))
							{
								pIterState->nSubPos = i - 1;

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
								pIterState->nNode = this->get_parent (nNode);
								pIterState->nSubPos = this->find_sub_node_offset (pIterState->nNode, nNode);

								nNode = pIterState->nNode;
							} while (pIterState->nSubPos == 0);

							pIterState->nSubPos--;

							nOperand -= nSize;

							nNode = pIterState->nNode;
							nSub = pIterState->nSubPos;

							if (nOperand == 0)
							{
								return;
							}
						}
					}

					this->convert_pos_to_container_pos (nNode, nOperand, pIterState->nNode, pIterState->nSubPos);
				}

				break;
			}
		}
	}
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
bool CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::is_iter_pos_evaluated (void *pState, _t_sizetype nPos) const
{
	iterator_state_t	*pIterState = (iterator_state_t *) pState;

	return (pIterState->nAssociatedPos == nPos);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
_t_data* CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::get_iter_data (void *pState) const
{
	iterator_state_t	*pIterState = (iterator_state_t *) pState;

	return (this->get_data (pIterState->nNode, pIterState->nSubPos));
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
void CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::set_iter_data (void *pState, const _t_data &rData)
{
	iterator_state_t	*pIterState = (iterator_state_t *) pState;

	_t_data		*pData = this->get_data (pIterState->nNode, pIterState->nSubPos);

	*pData = rData;
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
void CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::_swap
	(
		CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> &rBT
	)
{
	fast_swap<_t_nodeiter> (this->m_nRootNode, rBT.m_nRootNode);
	fast_swap<_t_nodeiter> (this->m_nTreeSize, rBT.m_nTreeSize);
	fast_swap<_t_subnodeiter> (this->m_nNodeSize, rBT.m_nNodeSize);
	fast_swap<uint32_t> (this->m_nGrowBy, rBT.m_nGrowBy);
	fast_swap<uint32_t> (this->m_nPoolIDnodeDesc, rBT.m_nPoolIDnodeDesc);
	fast_swap<uint32_t> (this->m_nPoolIDnodeMaintenance, rBT.m_nPoolIDnodeMaintenance);
	fast_swap<uint32_t> (this->m_nPoolIDdata, rBT.m_nPoolIDdata);
	fast_swap<uint32_t> (this->m_nPoolIDsubNodes, rBT.m_nPoolIDsubNodes);
	fast_swap<uint32_t> (this->m_nPoolIDserialVector, rBT.m_nPoolIDserialVector);
	fast_swap<_t_nodeiter *> (this->m_pRebuildFIFO, rBT.m_pRebuildFIFO);
	fast_swap<uint32_t> (this->m_nRebuildFIFOSize, rBT.m_nRebuildFIFOSize);
	fast_swap<uint32_t> (this->m_nRebuildFIFOreadPos, rBT.m_nRebuildFIFOreadPos);
	fast_swap<uint32_t> (this->m_nRebuildFIFOwritePos, rBT.m_nRebuildFIFOwritePos);
	fast_swap<uint32_t> (this->m_nAutoShrinkThreshold, rBT.m_nAutoShrinkThreshold);
	fast_swap<uint32_t> (this->m_nAutoShrinkCounter, rBT.m_nAutoShrinkCounter);

#if defined (_DEBUG)

	fast_swap<_t_nodeiter *> (this->m_pNodeTrace, rBT.m_pNodeTrace);
	fast_swap<uint32_t> (this->m_nNodeTraceDepth, rBT.m_nNodeTraceDepth);
	fast_swap<uint32_t> (this->m_nNodeTraceDepthMax, rBT.m_nNodeTraceDepthMax);

#endif

	fast_swap<_t_nodeiter> (this->m_nNextAlloc, rBT.m_nNextAlloc);

	CBTreeDefaults_t	&rIterIf = dynamic_cast <CBTreeDefaults_t &> (rBT);

	CBTreeDefaults_t::_swap (rIterIf);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter>
void CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::setup_pools ()
{
	CBTreeIOperBlockPoolDesc_t		sPoolDesc;

	sPoolDesc.nTotalSize = sizeof (node_t);
	sPoolDesc.nEntrySize = sizeof (node_t);

	m_nPoolIDnodeDesc = this->create_dataPool (&sPoolDesc);

	sPoolDesc.nTotalSize = sizeof (node_maintainence_t);
	sPoolDesc.nEntrySize = sizeof (node_maintainence_t);

	m_nPoolIDnodeMaintenance = this->create_dataPool (&sPoolDesc);

	sPoolDesc.nTotalSize = sizeof (_t_data) * get_node_max_data_size ();
	sPoolDesc.nEntrySize = sizeof (_t_data);

	m_nPoolIDdata = this->create_dataPool (&sPoolDesc);

	sPoolDesc.nTotalSize = (uint32_t)(sizeof (_t_nodeiter) * get_node_max_sub_nodes ());
	sPoolDesc.nEntrySize = (uint32_t)(sizeof (_t_nodeiter));

	m_nPoolIDsubNodes = this->create_dataPool (&sPoolDesc);

	sPoolDesc.nTotalSize = sizeof (_t_sizetype) * this->get_node_max_data_size ();
	sPoolDesc.nEntrySize = sizeof (_t_sizetype);

	m_nPoolIDserialVector = this->create_dataPool (&sPoolDesc);
}

#endif // BTREEBASEIF_CPP
