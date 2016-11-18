/************************************************************
**
** file:	btreedefaults.cpp
** author:	Andreas Steffens
** license:	LGPL v3
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

template<class _t_data, class _t_sizetype>
CBTreeDefaults<_t_data, _t_sizetype>::CBTreeDefaults ()
	:	CBTreeIf<_t_data, _t_sizetype> ()
{
#if defined (BTREE_ITERATOR_REGISTRATION)
	
	m_psIterRegister = new typename ::std::set<iterator *> ();
	m_psCIterRegister = new typename ::std::set<const_iterator *> ();
//	m_psRIterRegister = new typename ::std::set<reverse_iterator *> ();
//	m_psCRIterRegister = new typename ::std::set<const_reverse_iterator *> ();

	BTREE_ASSERT (m_psIterRegister != NULL, "CBTreeDefaults<>::CBTreeDefaults (): insufficient memory! (m_psIterRegister)");
	BTREE_ASSERT (m_psCIterRegister != NULL, "CBTreeDefaults<>::CBTreeDefaults (): insufficient memory! (m_psCIterRegister)");
//	BTREE_ASSERT (m_psRIterRegister != NULL, "CBTreeDefaults<>::CBTreeDefaults (): insufficient memory! (m_psRIterRegister)");
//	BTREE_ASSERT (m_psCRIterRegister != NULL, "CBTreeDefaults<>::CBTreeDefaults (): insufficient memory! (m_psCRIterRegister)");

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

template<class _t_data, class _t_sizetype>
CBTreeDefaults<_t_data, _t_sizetype>::CBTreeDefaults (const CBTreeDefaults<_t_data, _t_sizetype> &rBT)
	:	CBTreeIf<_t_data, _t_sizetype> ()
{
#if defined (BTREE_ITERATOR_REGISTRATION)

	m_psIterRegister = new typename ::std::set<iterator *> ();
	m_psCIterRegister = new typename ::std::set<const_iterator *> ();
//	m_psRIterRegister = new typename ::std::set<reverse_iterator *> ();
//	m_psCRIterRegister = new typename ::std::set<const_reverse_iterator *> ();

	BTREE_ASSERT (m_psIterRegister != NULL, "CBTreeDefaults<>::CBTreeDefaults (const CBTreeDefaults<_t_data, _t_sizetype> &): insufficient memory! (m_psIterRegister)");
	BTREE_ASSERT (m_psCIterRegister != NULL, "CBTreeDefaults<>::CBTreeDefaults (const CBTreeDefaults<_t_data, _t_sizetype> &): insufficient memory! (m_psCIterRegister)");
//	BTREE_ASSERT (m_psRIterRegister != NULL, "CBTreeDefaults<>::CBTreeDefaults (const CBTreeDefaults<_t_data, _t_sizetype> &): insufficient memory! (m_psRIterRegister)");
//	BTREE_ASSERT (m_psCRIterRegister != NULL, "CBTreeDefaults<>::CBTreeDefaults (const CBTreeDefaults<_t_data, _t_sizetype> &): insufficient memory! (m_psCRIterRegister)");

#endif

#if defined (USE_PERFORMANCE_COUNTERS)
	m_nHitCtr = 0;
	m_nMissCtr = 0;
#endif
}

/*

Destructor

*/

template<class _t_data, class _t_sizetype>
CBTreeDefaults<_t_data, _t_sizetype>::~CBTreeDefaults ()
{
#if defined (BTREE_ITERATOR_REGISTRATION)

	if (m_psIterRegister != NULL)
	{
		auto	sCIter = m_psIterRegister->cbegin ();

		for (; sCIter != m_psIterRegister->cend (); sCIter++)
		{
			(*sCIter)->detach ();
		}

		delete m_psIterRegister;
	}

	if (m_psCIterRegister != NULL)
	{
		auto	sCIter = m_psCIterRegister->cbegin ();

		for (; sCIter != m_psCIterRegister->cend (); sCIter++)
		{
			(*sCIter)->detach ();
		}

		delete m_psCIterRegister;
	}

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

template<class _t_data, class _t_sizetype>
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

template<class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator CBTreeDefaults<_t_data, _t_sizetype>::cbegin () const
{
	typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator			sIter (this, 0, false);

	return (sIter);
}

template<class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator CBTreeDefaults<_t_data, _t_sizetype>::cend () const
{
	typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator			sIter (this, this->size (), false);

	return (sIter);
}

template<class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_reverse_iterator CBTreeDefaults<_t_data, _t_sizetype>::crbegin () const
{
	typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator			sIter (this, this->size (), false);
	typename CBTreeDefaults<_t_data, _t_sizetype>::const_reverse_iterator	sRIter (sIter);

	return (sRIter);
}

template<class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_reverse_iterator CBTreeDefaults<_t_data, _t_sizetype>::crend () const
{
	typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator			sIter (this, 0, false);
	typename CBTreeDefaults<_t_data, _t_sizetype>::const_reverse_iterator	sRIter (sIter);

	return (sRIter);
}

template<class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::iterator CBTreeDefaults<_t_data, _t_sizetype>::begin ()
{
	CBTreeDefaults_t::iterator		sIter (this, 0);

	return (sIter);
}

template<class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::iterator CBTreeDefaults<_t_data, _t_sizetype>::end ()
{
	CBTreeDefaults_t::iterator		sIter (this, this->size ());

	return (sIter);
}

template<class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::reverse_iterator CBTreeDefaults<_t_data, _t_sizetype>::rbegin ()
{
	CBTreeDefaults_t::iterator			sIter (this, this->size (), false);
	CBTreeDefaults_t::reverse_iterator	sRIter (sIter);

	return (sRIter);
}

template<class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::reverse_iterator CBTreeDefaults<_t_data, _t_sizetype>::rend ()
{
	CBTreeDefaults_t::iterator			sIter (this, 0, false);
	CBTreeDefaults_t::reverse_iterator	sRIter (sIter);

	return (sRIter);
}

template<class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator CBTreeDefaults<_t_data, _t_sizetype>::begin () const
{
	return (this->cbegin ());
}

template<class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_iterator CBTreeDefaults<_t_data, _t_sizetype>::end () const
{
	return (this->cend ());
}

template<class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_reverse_iterator CBTreeDefaults<_t_data, _t_sizetype>::rbegin () const
{
	return (this->crbegin ());
}

template<class _t_data, class _t_sizetype>
typename CBTreeDefaults<_t_data, _t_sizetype>::const_reverse_iterator CBTreeDefaults<_t_data, _t_sizetype>::rend () const
{
	return (this->crend ());
}

template<class _t_data, class _t_sizetype>
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

template<class _t_data, class _t_sizetype>
void CBTreeDefaults<_t_data, _t_sizetype>::register_iterator (iterator *pIter)
{
#if defined (BTREE_ITERATOR_REGISTRATION)

	if (m_psIterRegister->count (pIter) == 0)
	{
		m_psIterRegister->insert (pIter);
	}

#endif
}

template<class _t_data, class _t_sizetype>
void CBTreeDefaults<_t_data, _t_sizetype>::register_iterator (const_iterator *pCIter)
{
#if defined (BTREE_ITERATOR_REGISTRATION)

	if (m_psCIterRegister->count (pCIter) == 0)
	{
		m_psCIterRegister->insert (pCIter);
	}
#endif
}

//template<class _t_data, class _t_sizetype>
//void CBTreeDefaults<_t_data, _t_sizetype>::register_iterator (reverse_iterator *pRIter)
//{
//#if defined (BTREE_ITERATOR_REGISTRATION)
//
//	if (m_psRIterRegister->count (pRIter) == 0)
//	{
//		m_psRIterRegister->insert (pRIter);
//	}
//
//#endif
//}
//
//template<class _t_data, class _t_sizetype>
//void CBTreeDefaults<_t_data, _t_sizetype>::register_iterator (const_reverse_iterator *pCRIter)
//{
//#if defined (BTREE_ITERATOR_REGISTRATION)
//
//	if (m_psCRIterRegister->count (pCRIter) == 0)
//	{
//		m_psCRIterRegister->insert (pCRIter);
//	}
//
//#endif
//}

template<class _t_data, class _t_sizetype>
void CBTreeDefaults<_t_data, _t_sizetype>::unregister_iterator (iterator *pIter)
{
#if defined (BTREE_ITERATOR_REGISTRATION)

	auto	nSize = m_psIterRegister->erase (pIter);

#endif
}

template<class _t_data, class _t_sizetype>
void CBTreeDefaults<_t_data, _t_sizetype>::unregister_iterator (const_iterator *pCIter)
{
#if defined (BTREE_ITERATOR_REGISTRATION)

	auto	nSize = m_psCIterRegister->erase (pCIter);

#endif
}

//template<class _t_data, class _t_sizetype>
//void CBTreeDefaults<_t_data, _t_sizetype>::unregister_iterator (reverse_iterator *pRIter)
//{
//#if defined (BTREE_ITERATOR_REGISTRATION)
//
//	m_psRIterRegister->erase (pRIter);
//
//#endif
//}
//
//template<class _t_data, class _t_sizetype>
//void CBTreeDefaults<_t_data, _t_sizetype>::unregister_iterator (const_reverse_iterator *pCRIter)
//{
//#if defined (BTREE_ITERATOR_REGISTRATION)
//
//	m_psCRIterRegister->erase (pCRIter);
//
//#endif
//}

template<class _t_data, class _t_sizetype>
void CBTreeDefaults<_t_data, _t_sizetype>::_swap
	(
		CBTreeDefaults<_t_data, _t_sizetype> &rBT
	)
{
#if defined (USE_PERFORMANCE_COUNTERS)

	fast_swap (this->m_nHitCtr, rBT.m_nHitCtr);
	fast_swap (this->m_nMissCtr, rBT.m_nMissCtr);

#endif

	CBTreeSuper	&rSuper = dynamic_cast <CBTreeSuper &> (rBT);

	CBTreeSuper::_swap (rSuper);
}

#endif // BTREEDEFAULTS_CPP

