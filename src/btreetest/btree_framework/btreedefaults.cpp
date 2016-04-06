/************************************************************
**
** file:	btreedefaults.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree frameworks base class
** on the iterator interface level.
**
************************************************************/

#ifndef	BTREEDEFAULTS_CPP
#define	BTREEDEFAULTS_CPP

#include "btreedefaults.h"

template <class _t_data, class _t_sizetype>
CBTreeDefaults<_t_data, _t_sizetype>::CBTreeDefaults (const bayerTreeCacheDescription_t *psCacheDescription)
	:	CBTreeIf<_t_data, _t_sizetype> ()
	,	m_sCacheDescription (*psCacheDescription)
{
#if defined (BTREE_ITERATOR_REGISTRATION)
	m_psIterRegister = new typename ::std::set<const_iterator *> ();

	BTREE_ASSERT (m_psIterRegister != NULL, "CBTreeDefaults<>::CBTreeDefaults (bayerTreeCacheDescription_t *): insufficient memory!");
#endif

#if defined (USE_PERFORMANCE_COUNTERS)
	m_nHitCtr = 0;
	m_nMissCtr = 0;
#endif
}

/*

Copy Constructor

rBT		-	reference to CBTreeDefaults instance to be copied

*/

template <class _t_data, class _t_sizetype>
CBTreeDefaults<_t_data, _t_sizetype>::CBTreeDefaults (const CBTreeDefaults<_t_data, _t_sizetype> &rBT)
	:	CBTreeIf<_t_data, _t_sizetype> ()
{
#if defined (BTREE_ITERATOR_REGISTRATION)
	m_psIterRegister = new typename ::std::set<const_iterator *> ();

	BTREE_ASSERT (m_psIterRegister != NULL, "CBTreeDefaults<>::CBTreeDefaults (const CBTreeDefaults<_t_data, _t_sizetype> &): insufficient memory!");
#endif

	m_sCacheDescription = rBT.m_sCacheDescription;

#if defined (USE_PERFORMANCE_COUNTERS)
	m_nHitCtr = 0;
	m_nMissCtr = 0;
#endif
}

/*

Destructor

*/

template <class _t_data, class _t_sizetype>
CBTreeDefaults<_t_data, _t_sizetype>::~CBTreeDefaults ()
{
#if defined (BTREE_ITERATOR_REGISTRATION)

	typename ::std::set<const_iterator *>::iterator		sIter = m_psIterRegister->begin ();

	for (; sIter != m_psIterRegister->end (); sIter++)
	{
		(*sIter)->detach ();
	}

	delete m_psIterRegister;

#endif
}

/*

get_performance_counters - get performance counters

The information released by this routine can be used to identify performance lags in a CBTreeDefaults instance.

rHitCtrs	- is a refernce to an array of cache hit counters
rMissCtrs	- is a refernce to an array of cache missed counters

Each array contains the following elements:
PERFCTR_SUPER_BLOCK_ADDR		- super block address generator TLB
PERFCTR_BLOCK_ADDR				- block address generator TLB
PERFCTR_RESERVATION_VECTOR_ADDR	- reservation vector address generator TLB
PERFCTR_NODE_ADDR				- node address generator TLB
PERFCTR_SUBNODE_ADDR			- sub-node data address generator TLB
PERFCTR_SERIAL_VECTOR_ADDR		- serial information vector address generator TLB
PERFCTR_DATA_ADDR				- data address generator TLB
PERFCTR_FILE_CACHE				- file cache of internal CFileMem instance
PERFCTR_NODEDATA_DATA			- node information data cache
PERFCTR_SUBNODE_DATA			- sub-node information data cache
PERFCTR_DATA					- top level data cache

Note: TLB means translation look aside buffer.

NB: The defined USE_PERFORMANCE_COUNTERS needs to be defined, otherwise only unset values are returned.

*/

template <class _t_data, class _t_sizetype>
void CBTreeDefaults<_t_data, _t_sizetype>::get_performance_counters (uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR])
{
#if defined (USE_PERFORMANCE_COUNTERS)
	if (m_pData != NULL)
	{
		m_pData->get_performance_counters (rHitCtrs, rMissCtrs);

		rHitCtrs[PERFCTR_DATA] = m_nHitCtr;
		rMissCtrs[PERFCTR_DATA] = m_nMissCtr;
	}
	else
	{
		memset ((void *) &rHitCtrs, 0, sizeof (rHitCtrs));
		memset ((void *) &rMissCtrs, 0, sizeof (rMissCtrs));
	}
#else
	memset ((void *) &rHitCtrs, 0, sizeof (rHitCtrs));
	memset ((void *) &rMissCtrs, 0, sizeof (rMissCtrs));
#endif
}

template <class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator CBTreeDefaults<_t_data, _t_sizetype>::cbegin () const
{
	typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator			sIter (this, 0, false);

	return (sIter);
}

template <class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator CBTreeDefaults<_t_data, _t_sizetype>::cend () const
{
	typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator			sIter (this, this->size (), false);

	return (sIter);
}

template <class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_reverse_iterator CBTreeDefaults<_t_data, _t_sizetype>::crbegin () const
{
	typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator			sIter (this, this->size (), false);
	typename CBTreeDefaults<_t_data, _t_sizetype>::const_reverse_iterator	sRIter (sIter);

	return (sRIter);
}

template <class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_reverse_iterator CBTreeDefaults<_t_data, _t_sizetype>::crend () const
{
	typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator			sIter (this, 0, false);
	typename CBTreeDefaults<_t_data, _t_sizetype>::const_reverse_iterator	sRIter (sIter);

	return (sRIter);
}

template <class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::iterator CBTreeDefaults<_t_data, _t_sizetype>::begin ()
{
	CBTreeDefaults_t::iterator		sIter (this, 0);

	return (sIter);
}

template <class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::iterator CBTreeDefaults<_t_data, _t_sizetype>::end ()
{
	CBTreeDefaults_t::iterator		sIter (this, this->size ());

	return (sIter);
}

template <class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::reverse_iterator CBTreeDefaults<_t_data, _t_sizetype>::rbegin ()
{
	CBTreeDefaults_t::iterator			sIter (this, this->size (), false);
	CBTreeDefaults_t::reverse_iterator	sRIter (sIter);

	return (sRIter);
}

template <class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::reverse_iterator CBTreeDefaults<_t_data, _t_sizetype>::rend ()
{
	CBTreeDefaults_t::iterator			sIter (this, 0, false);
	CBTreeDefaults_t::reverse_iterator	sRIter (sIter);

	return (sRIter);
}

template <class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator CBTreeDefaults<_t_data, _t_sizetype>::begin () const
{
	return (this->cbegin ());
}

template <class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator CBTreeDefaults<_t_data, _t_sizetype>::end () const
{
	return (this->cend ());
}

template <class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_reverse_iterator CBTreeDefaults<_t_data, _t_sizetype>::rbegin () const
{
	return (this->crbegin ());
}

template <class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_reverse_iterator CBTreeDefaults<_t_data, _t_sizetype>::rend () const
{
	return (this->crend ());
}

template <class _t_data, class _t_sizetype>
CBTreeDefaults<_t_data, _t_sizetype> &CBTreeDefaults<_t_data, _t_sizetype>::operator=
	(const CBTreeDefaults<_t_data, _t_sizetype> &rBT)
{
	// if this is not the same instance as the referenced b-tree (rBT) ...
	if (this != &rBT)
	{
		CBTreeIf_t			&rIterTraitsThis = dynamic_cast<CBTreeIf_t &> (*this);
		const CBTreeIf_t	&rIterTraitsBT = dynamic_cast<const CBTreeIf_t &> (rBT);
		
		rIterTraitsThis = rIterTraitsBT;
	}

	return (*this);
}

template <class _t_data, class _t_sizetype>
void CBTreeDefaults<_t_data, _t_sizetype>::register_iterator (const_iterator *pIter)
{
#if defined (BTREE_ITERATOR_REGISTRATION)

	if (m_psIterRegister->count (pIter) == 0)
	{
		m_psIterRegister->insert (pIter);
	}

#endif
}

template <class _t_data, class _t_sizetype>
void CBTreeDefaults<_t_data, _t_sizetype>::unregister_iterator (const_iterator *pIter)
{
#if defined (BTREE_ITERATOR_REGISTRATION)

	m_psIterRegister->erase (pIter);

#endif
}

template <class _t_data, class _t_sizetype>
void CBTreeDefaults<_t_data, _t_sizetype>::_swap
	(
		CBTreeDefaults<_t_data, _t_sizetype> &rBT
	)
{
	fast_swap<bayerTreeCacheDescription_t> (this->m_sCacheDescription, rBT.m_sCacheDescription);

#if defined (USE_PERFORMANCE_COUNTERS)

	fast_swap<uint64_t> (this->m_nHitCtr, rBT.m_nHitCtr);
	fast_swap<uint64_t> (this->m_nMissCtr, rBT.m_nMissCtr);

#endif

	CBTreeSuper	&rSuper = dynamic_cast <CBTreeSuper &> (rBT);

	CBTreeSuper::_swap (rSuper);
}

#endif // BTREEDEFAULTS_CPP
