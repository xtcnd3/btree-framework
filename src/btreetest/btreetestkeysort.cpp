/************************************************************
**
** file:	btreetestkeysort.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains key sort application class code derived
** from the b-tree framework's key sort data class to allow
** for the key sort data class being tested by having
** the key sort application class act as a wrapper.
**
************************************************************/

#ifndef	BTREETESTKEYSORT_CPP
#define	BTREETESTKEYSORT_CPP

#include "btreetestkeysort.h"

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySortTest
	(
		_t_datalayerproperties &rDataLayerProperties, 
		bayerTreeCacheDescription_t *psCacheDescription, 
		_t_subnodeiter nNodeSize, 
		uint32_t nKeyOffset, 
		uint32_t nKeySize
	)
	:	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		rDataLayerProperties, 
		psCacheDescription, 
		nNodeSize
	)
	,	CBTreeKeySortDataIf <_t_data, _t_key, _t_sizetype> ()
	,	m_nKeyOffset (nKeyOffset)
	,	m_nKeySize (nKeySize)
{
	m_pClRefData = new ::std::multimap<uint32_t, keySortMap_t> ();

	BTREE_ASSERT (m_pClRefData != NULL, "CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySortTest: insufficient memory!");
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySortTest (CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT)
	:	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		rBT, false
	)
	,	CBTreeKeySortDataIf <_t_data, _t_key, _t_sizetype> ()
	,	m_nKeyOffset (rBT.m_nKeyOffset)
	,	m_nKeySize (rBT.m_nKeySize)
{
	m_pClRefData = new ::std::multimap<uint32_t, keySortMap_t> ();

	BTREE_ASSERT (m_pClRefData != NULL, "CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySortTest: insufficient memory!");

	this->assign (rBT);

//	m_pClRefData->insert<::std::multimap<uint32_t, keySortMap_t>::const_iterator> (rBT.m_pClRefData->cbegin (), rBT.m_pClRefData->cend ());

	test ();
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeKeySortTest ()
{
	delete m_pClRefData;
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_key* CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::extract_key (_t_key *pKey, const _t_data &rData)
{
	*pKey = rData.first;

	return (pKey);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
template <class _t_iterator>
void CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	this->template insert_via_iterator<_t_iterator> (sItFirst, sItLast);

	test ();
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert
	(
		const _t_data &rData
	)
{
	const_iterator							sCIter;

	this->m_pClRefData->insert (rData);
	
	sCIter = CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (rData);

	test ();

	return (sCIter);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterPos
	)
{
	typedef typename ::std::multimap<_t_key, keySortMap_t>::iterator		itmmap_t;

	_t_key			sKey = (*sCIterPos).first;
	itmmap_t		sItMMapLower = m_pClRefData->lower_bound (sKey);
	itmmap_t		sItMMapUpper = m_pClRefData->upper_bound (sKey);
	itmmap_t		sItMMap;

	for (sItMMap = sItMMapLower; sItMMap != sItMMapUpper; sItMMap++)
	{
		if ((*sItMMap).second.nData == (*sCIterPos).second.nData)
		{
			if ((*sItMMap).second.nDebug == (*sCIterPos).second.nDebug)
			{
				m_pClRefData->erase (sItMMap);

				break;
			}
		}
	}

	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (sCIterPos);

	test ();

	return (sCIterPos);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (const _t_key &rKey)
{
	_t_sizetype		nRetval;

	m_pClRefData->erase (rKey);

	nRetval = CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (rKey);

	test ();

	return (nRetval);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterFirst, 
		typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterLast
	)
{
	typedef typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator		citer_t;
	typedef typename ::std::multimap<_t_key, keySortMap_t>::iterator																						itmmap_t;

	citer_t		sCIterRetval;
	citer_t		sCIter;
	_t_key		sKey;
	itmmap_t	sItMMapLower;
	itmmap_t	sItMMapUpper;
	itmmap_t	sItMMap;

	for (sCIter = sCIterFirst; sCIter != sCIterLast; sCIter++)
	{
		sKey = (*sCIter).first;

		sItMMapLower = m_pClRefData->lower_bound (sKey);
		sItMMapUpper = m_pClRefData->upper_bound (sKey);

		for (sItMMap = sItMMapLower; sItMMap != sItMMapUpper; sItMMap++)
		{
			if ((*sItMMap).second.nData == (*sCIter).second.nData)
			{
				if ((*sItMMap).second.nDebug == (*sCIter).second.nDebug)
				{
					m_pClRefData->erase (sItMMap);

					break;
				}
			}
		}
	}

	sCIterRetval = CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (sCIterFirst, sCIterLast);

	test ();

	return (sCIterRetval);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap
	(
		CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rKeySort
	)
{
	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>	&rBtrKeySort = 
		dynamic_cast <CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &> (rKeySort);

	m_pClRefData->swap (*(rKeySort.m_pClRefData));

	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap (rBtrKeySort);

	test ();
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find (const _t_key &rKey)
{
	return (CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find (rKey));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (const _t_key &rKey)
{
	return (CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (rKey));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (const _t_key &rKey)
{
	return (CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (rKey));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert_tb (const _t_data &rData)
{
	_t_sizetype								nRslt;

	m_pClRefData->insert (rData);

	nRslt = CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert_tb (rData);

	test ();

	return (nRslt);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase_tb (const _t_key &rKey)
{
	typedef ::std::multimap<uint32_t, keySortMap_t>::const_iterator		citer_mmap_t;

	citer_mmap_t							sItMMapLower;
	citer_mmap_t							sItMMapUpper;
	citer_mmap_t							sItMMap;
	const_iterator							sCIterLower;
	const_iterator							sCIterUpper;
	const_iterator							sCIter;
	_t_sizetype								nRslt;
	
	sItMMapLower = m_pClRefData->lower_bound (rKey);
	sItMMapUpper = m_pClRefData->upper_bound (rKey);

	m_pClRefData->erase (sItMMapLower, sItMMapUpper);

	sCIterLower = CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (rKey);
	sCIterUpper = CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (rKey);	

	nRslt = sCIterUpper - sCIterLower;

	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (sCIterLower, sCIterUpper);

	test ();

	return (nRslt);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase_tb (const _t_key &rKey, const _t_sizetype instance)
{
	typedef ::std::multimap<uint32_t, keySortMap_t>::const_iterator		citer_mmap_t;

	::std::pair<uint32_t, keySortMap_t>		sValue;
	keySortMap_t							sMap;
	citer_mmap_t							sItMMapLower;
	citer_mmap_t							sItMMapUpper;
	citer_mmap_t							sItMMap;
	const_iterator							sCIterLower;
	const_iterator							sCIterUpper;
	const_iterator							sCIter;
	_t_sizetype								i;
	_t_data									sEntry;
	bool									bDeleted = false;

	sCIterLower = CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (rKey);
	sCIterUpper = CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (rKey);

	for (sCIter = sCIterLower, i = 0; (sCIter != sCIterUpper) && (i < instance); sCIter++, i++);
	
	BTREE_ASSERT (i == instance, "CBTreeKeySortTest<>::erase_tb (const _t_key &, const _t_sizetype): ERROR: Failed to find instance!");

	sEntry = *sCIter;

	sItMMapLower = m_pClRefData->lower_bound (rKey);
	sItMMapUpper = m_pClRefData->upper_bound (rKey);

	for (sItMMap = sItMMapLower; sItMMap != sItMMapUpper; sItMMap++)
	{
		sMap = (*sItMMap).second;

		if (sMap.nData == sEntry.second.nData)
		{
			if (sMap.nDebug == sEntry.second.nDebug)
			{
				m_pClRefData->erase (sItMMap);

				bDeleted = true;

				break;
			}
		}
	}

	BTREE_ASSERT (bDeleted, "CBTreeKeySortTest<>::erase_tb (const _t_key &, const _t_sizetype): ERROR: multi map instance failed to delete!");
	
	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (sCIter);

	test ();

	return (1);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ()
{
	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ();

	m_pClRefData->clear ();
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get (const _t_key &rKey, _t_data *pObj)
{
	return (CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get (rKey, pObj));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get (const _t_key &rKey, _t_sizetype nInstance, _t_data *pObj)
{
	return (CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get (rKey, nInstance, pObj));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::count (const _t_key &rKey)
{
	return (CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::count (rKey));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_init_pos_of_key (const _t_key &rKey)
{
	return (CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_init_pos_of_key (rKey));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_at (const _t_sizetype nPos, _t_data &rData)
{
	return (CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_at (nPos, rData));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator=
	(CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT)
{
	if (this != &rBT)
	{
		CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		*pThisKeySort;
		CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		*pRefKeySort;

		pThisKeySort = dynamic_cast <CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *> (this);
		pRefKeySort = dynamic_cast <CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *> (&rBT);

		*pThisKeySort = *pRefKeySort;

		test ();
	}

	return (*this);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator== (CBTreeKeySortTest &rKeySort)
{
	if (this == &rKeySort)
	{
		return (true);
	}

	if (this->size () != rKeySort.size ())
	{
		return (false);
	}

	CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>							*pKeySetTest;

	pKeySetTest = new CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (*(this->m_pClDataLayerProperties), &(this->m_sCacheDescription), this->m_nNodeSize, this->m_nKeyOffset, this->m_nKeySize);

	BTREE_ASSERT (pKeySetTest != NULL, "CBTreeKeySortTest::operator== (const CBTreeKeySortTest &): insufficient memory!");

	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		*pKeySort = dynamic_cast <CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *> (&rKeySort);
	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		*pKeySet;
	_t_sizetype																											i;
	_t_sizetype																											j;
	_t_sizetype																											k;
	_t_sizetype																											nKeySetSize;
	_t_data																												sThisData;
	_t_data																												sData;
	_t_data																												sKey;

	pKeySet = dynamic_cast <CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *> (pKeySetTest);

	for (i = 0; i < this->size (); i += nKeySetSize)
	{
		this->get_at (i, sKey);

		nKeySetSize = pKeySort->get (sKey.first, *pKeySet);

		if (nKeySetSize != this->count (sKey.first))
		{
			break;
		}

		for (j = 0; j < nKeySetSize; j++)
		{
			this->get_at (i + j, sThisData);

			for (k = 0; k < pKeySet->size (); k++)
			{
				pKeySet->get_at (k, sData);

				if ((sThisData.first == sData.first) && (sThisData.second.nData == sData.second.nData))
				{
					pKeySet->erase_tb (sKey.first, k);

					break;
				}
			}
		}

		if (pKeySet->size () != 0)
		{
			break;
		}
	}

	delete pKeySetTest;

	return (i >= this->size ());
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator!= (CBTreeKeySortTest &rKeySort)
{
	return ( ! (*this == rKeySort));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
int CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::comp (const _t_key &rKey0, const _t_key &rKey1)
{
	if (rKey0 < rKey1)
	{
		return (-1);
	}
	else if (rKey0 > rKey1)
	{
		return (1);
	}

	return (0);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_key* CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::extract_key (_t_key *pKey, const _t_nodeiter nNode, const _t_subnodeiter nEntry)
{
	pKey = CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::extract_key (pKey, nNode, nEntry);

	return (pKey);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::test ()
{
	typedef ::std::multimap<uint32_t, keySortMap_t>::const_iterator		citer_mmap_t;

	::std::multimap<uint32_t, keySortMap_t>		sMMap;
	uint32_t									nKey;
	uint32_t									nNextKey;
	bool										bBounce;
	_t_sizetype									nTotalCount = 0;
	_t_data										sEntry;
	::std::pair<uint32_t, keySortMap_t>			sValue;
	citer_mmap_t								sItMMapLower;
	citer_mmap_t								sItMMapUpper;
	citer_mmap_t								sItMMap;
	const_iterator								sCIterBegin;
	const_iterator								sCIterEnd;
	const_iterator								sCIterLower;
	const_iterator								sCIterUpper;
	const_iterator								sCIter;
	bool										bDeleted;
	
	if (!this->test_integrity ())
	{
		cerr << endl;
		cerr << "integrity test failed" << endl;

		cerr << "creating integrity.html..." << endl;

		this->show_integrity ("integrity.html");

		cerr << "finished!" << endl;

		exit (-1);
	}

	sCIterBegin = CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::cbegin ();
	sCIterEnd = CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::cend ();

	sCIter = sCIterBegin;

	if (this->size () > 0)
	{
		this->extract_key (&nKey, ((_t_data) (*sCIter)));
	}

	while (sCIter != sCIterEnd)
	{
		if (m_pClRefData->count (nKey) != this->count (nKey))
		{
			cerr << endl;
			cerr << "number of instances mismatches" << endl;
			cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << nKey << endl;
			cerr << std::setfill (' ') << std::dec << std::setw (0);

			cerr << "count: " << this->count (nKey) << endl;
			cerr << "reference: " << m_pClRefData->count (nKey) << endl;
			
			cerr << "creating count.html..." << endl;

			this->show_integrity ("count.html");

			cerr << "finished!" << endl;

			exit (-1);
		}

		if (this->count (nKey) == 1)
		{
			sItMMapLower = m_pClRefData->lower_bound (nKey);

			sCIterLower = CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (nKey);

			sValue = *sItMMapLower;

			sEntry = ((_t_data) (*sCIterLower));

			if ((sEntry.second.nData != sValue.second.nData) || (sEntry.second.nDebug != sValue.second.nDebug))
			{
				cerr << endl;
				cerr << "data mismatches" << endl;
				cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << sEntry.first << endl;
				cerr << "data: " << sEntry.second.nData << endl;

				cerr << std::setfill (' ') << std::dec << std::setw (0);

				cerr << "debug: " << sEntry.second.nDebug << endl;
				cerr << "reference" << endl;

				cerr << "data: " << std::setfill ('0') << std::hex << std::setw (8) << sValue.second.nData << endl;

				cerr << std::setfill (' ') << std::dec << std::setw (0);

				cerr << "debug: " << sValue.second.nDebug << endl;
				
				cerr << "creating data.html..." << endl;

				this->show_integrity ("data.html");

				cerr << "finished!" << endl;

				exit (-1);
			}
		}
		else
		{
			sItMMapLower = m_pClRefData->lower_bound (nKey);
			sItMMapUpper = m_pClRefData->upper_bound (nKey);

			sMMap.insert<citer_mmap_t> (sItMMapLower, sItMMapUpper);

			sCIterLower = CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (nKey);
			sCIterUpper = CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (nKey);

			for (sCIter = sCIterLower; sCIter != sCIterUpper; sCIter++)
			{
				sEntry = ((_t_data) (*sCIter));

				bDeleted = false;

				for (sItMMap = sMMap.cbegin (); sItMMap != sMMap.cend (); sItMMap++)
				{
					sValue = *sItMMap;

					if (sEntry.second.nData == sValue.second.nData)
					{
						if (sEntry.second.nDebug == sValue.second.nDebug)
						{
							sMMap.erase (sItMMap);

							bDeleted = true;

							break;
						}
					}
				}

				if (!bDeleted)
				{
					cerr << endl;
					cerr << "number of instances mismatches" << endl;
					cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << sEntry.first << endl;
					cerr << "data: " << sEntry.second.nData << endl;

					cerr << std::setfill (' ') << std::dec << std::setw (0);

					cerr << "debug: " << sEntry.second.nDebug << endl;
					cerr << "Instance not found in reference!" << endl;

					cerr << "creating error.html..." << endl;

					this->show_integrity ("error.html");

					cerr << "finished!" << endl;

					exit (-1);
				}
			}

			if (sMMap.size () != 0)
			{
				cerr << endl;
				cerr << "number of instances mismatches" << endl;
				cerr << "the following entries are still present in reference:" << endl;

				for (sItMMap = sMMap.cbegin (); sItMMap != sMMap.cend (); sItMMap++)
				{
					sValue = *sItMMap;

					cerr << "key: ";

					cerr << std::setfill ('0') << std::hex << std::setw (8);
					{
						cerr << sValue.first << " ";
						cerr << "data: " << flush;
						cerr << sValue.second.nData << " ";
					}
					cerr << std::setfill (' ') << std::dec << std::setw (0);

					cerr << "debug: " << sValue.second.nDebug << endl;
				}

				cerr << "creating error.html..." << endl;

				this->show_integrity ("error.html");

				cerr << "finished!" << endl;

				exit (-1);
			}
		}

		CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_next_key (nKey, nNextKey, bBounce);

		if (bBounce)
		{
			break;
		}

		nKey = nNextKey;
	}
	
	if (m_pClRefData->size () != this->size ())
	{
		cerr << endl;
		cerr << "size mismatches" << endl;
		cerr << "size: " << this->size () << endl;
		cerr << "reference size: " << m_pClRefData->size () << endl;

		cerr << "creating size.html..." << endl;

		this->show_integrity ("size.html");

		cerr << "finished!" << endl;

		exit (-1);
	}
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::showdata (std::ofstream &ofs, const _t_nodeiter nNode, const _t_subnodeiter nSubPos)
{
	std::stringstream		buf;
	uint64_t				ntab = 0ULL, notab = 0ULL;
	uint32_t				rData;
	_t_data					data;
	bool					bError = false;
	_t_nodeiter				nNeightbourNode;
	_t_subnodeiter			nNeightbourSub;
	int						nErrCnt = 0;
	std::stringstream		aszMsg;
	char					*pColorInit = (char *) "";
	char					*pColorExit = (char *) "";
	_t_nodeiter				nPrevNode;
	_t_subnodeiter			nPrevSub;
	_t_nodeiter				nNextNode;
	_t_subnodeiter			nNextSub;
	bool					bBounce;

	if (this->is_leaf (nNode))
	{
		try
		{
			data = this->get_data (nNode, nSubPos);

			rData = data.first;
			rData = (rData >> 16) | (rData << 16);
			rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);
			
			buf.clear ();
			buf << "<table border=\"0\" cellspacing=\"1\" width=\"220\"><tr><td>";
			buf << "key: " << hex << setfill ('0') << setw (8) << rData << "<br>";
			buf << "data: " << data.second.nData << dec << "<br>";
			buf << "debug: " << data.second.nDebug << "<br>";
			buf << "instance: " << this->get_instancePos (nNode, nSubPos);
			buf << "</td>";

			_t_sizetype		nDiff = this->lower_bound (data.first) - this->cbegin ();
			_t_sizetype		nOffset = nDiff + this->get_instancePos (nNode, nSubPos);

			buf << "<td align=\"top\">";

			if (nOffset < m_pClRefData->size ())
			{
				::std::multimap<uint32_t, keySortMap_t>::const_iterator		sItMMap;

				sItMMap = m_pClRefData->cbegin ();

				::std::advance< ::std::multimap<uint32_t, keySortMap_t>::const_iterator, _t_sizetype> (sItMMap, nDiff);

				rData = (*sItMMap).first;
				rData = (rData >> 16) | (rData << 16);
				rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);

				if (data.first != (*sItMMap).first)
				{
					buf << "<font color=\"#FF0000\">";
				}
				else
				{
					buf << "<font color=\"#00BB00\">";
				}
				
				buf << "key: " << hex << setfill ('0') << setw (8) << rData << "<br>";
				buf << "</font>";

				if (data.second.nData != (*sItMMap).second.nData)
				{
					buf << "<font color=\"#AAAA00\">";
				}
				else
				{
					buf << "<font color=\"#00BB00\">";
				}
				
				buf << "data: " << (*sItMMap).second.nData << dec << "<br>";
				buf << "</font>";

				if (data.second.nDebug != (*sItMMap).second.nDebug)
				{
					buf << "<font color=\"#AAAA00\">";
				}
				else
				{
					buf << "<font color=\"#00BB00\">";
				}
				
				buf << "debug: " << (*sItMMap).second.nDebug << "<br>";
				buf << "</font>";
				buf << "-";
			}
			else
			{
				buf << "<font color=\"#FF0000\">";
				buf << "reference<br>";
				buf << "out of<br>";
				buf << "range";
				buf << "</font>";
			}

			buf << "</td></tr>";
			buf << "</table>" << endl;
		}
		catch (exception *pE)
		{
			if (!ofs.is_open ())
			{
				return (false);
			}

			nErrCnt++;

			aszMsg.clear ();
			aszMsg << "<br>data: corruption (" << pE->what () << ")";

			rData = data.first;
			rData = (rData >> 16) | (rData << 16);
			rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);
			
			buf.clear ();
			buf << "key: " << hex << setfill ('0') << setw (8) << rData << dec << "<br>debug: " << data.second.nDebug << "<br>instance: ---";
		}

		try
		{
			this->move_prev (nNode, nSubPos, nPrevNode, nPrevSub, bBounce);

			if (bBounce == false)
			{
				_t_data	prevData = this->get_data (nPrevNode, nPrevSub);
				_t_key	sPrevKey;
				_t_key	sKey;

				this->extract_key (&sPrevKey, prevData);
				this->extract_key (&sKey, data);

				if (this->comp (sPrevKey, sKey) > 0)
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
					this->extract_key (this->m_pShortLiveKey, nNeightbourNode, nNeightbourSub);

					if (this->comp (*(this->m_pShortLiveKey), (uint32_t) (((char *)&data)[this->m_nKeyOffset])) == 0)
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
					this->extract_key (this->m_pShortLiveKey, nNeightbourNode, nNeightbourSub);

					if (this->comp (*(this->m_pShortLiveKey), (uint32_t) (((char *)&data)[this->m_nKeyOffset])) == 0)
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
		catch (exception *pE)
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
				_t_data	nextData = this->get_data (nNextNode, nNextSub);
				_t_key	sNextKey;
				_t_key	sKey;

				this->extract_key (&sNextKey, nextData);
				this->extract_key (&sKey, data);

				if (this->comp (sNextKey, sKey) < 0)
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
		catch (exception *pE)
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
		try
		{
			data = this->get_data (nNode, nSubPos);

			rData = data.first;
			rData = (rData >> 16) | (rData << 16);
			rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);
			
			buf.clear ();
			buf << "<table border=\"0\" cellspacing=\"1\" width=\"220\"><tr><td>";
			buf << "key: " << hex << setfill ('0') << setw (8) << rData << "<br>";
			buf << "data: " << data.second.nData << dec << "<br>";
			buf << "debug: " << data.second.nDebug << "<br>";
			buf << "instance: " << this->get_instancePos (nNode, nSubPos);
			buf << "</td>";

			_t_sizetype		nDiff = this->lower_bound (data.first) - this->cbegin ();
			_t_sizetype		nOffset = nDiff + this->get_instancePos (nNode, nSubPos);

			buf << "<td align=\"top\">";

			if (nOffset < m_pClRefData->size ())
			{
				::std::multimap<uint32_t, keySortMap_t>::const_iterator		sItMMap;

				sItMMap = m_pClRefData->cbegin ();

				::std::advance< ::std::multimap<uint32_t, keySortMap_t>::const_iterator, _t_sizetype> (sItMMap, nDiff);

				rData = (*sItMMap).first;
				rData = (rData >> 16) | (rData << 16);
				rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);

				if (data.first != (*sItMMap).first)
				{
					buf << "<font color=\"#FF0000\">";
				}
				else
				{
					buf << "<font color=\"#00BB00\">";
				}
				
				buf << "key: " << hex << setfill ('0') << setw (8) << rData << "<br>";
				buf << "</font>";

				if (data.second.nData != (*sItMMap).second.nData)
				{
					buf << "<font color=\"#AAAA00\">";
				}
				else
				{
					buf << "<font color=\"#00BB00\">";
				}
				
				buf << "data: " << (*sItMMap).second.nData << dec << "<br>";
				buf << "</font>";

				if (data.second.nDebug != (*sItMMap).second.nDebug)
				{
					buf << "<font color=\"#AAAA00\">";
				}
				else
				{
					buf << "<font color=\"#00BB00\">";
				}
				
				buf << "debug: " << (*sItMMap).second.nDebug << "<br>";
				buf << "</font>";
				buf << "-";
			}
			else
			{
				buf << "<font color=\"#FF0000\">";
				buf << "reference<br>";
				buf << "out of<br>";
				buf << "range";
				buf << "</font>";
			}

			buf << "</td></tr>";
			buf << "</table>" << endl;
		}
		catch (exception *pE)
		{
			if (!ofs.is_open ())
			{
				return (false);
			}

			nErrCnt++;

			aszMsg.clear ();
			aszMsg << "<br>data: corruption (" << pE->what () << ")";

			rData = data.first;
			rData = (rData >> 16) | (rData << 16);
			rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);
			
			buf.clear ();
			buf << "key: " << hex << setfill ('0') << setw (8) << rData << dec << "<br>debug: " << data.second.nDebug << "<br>instance: ---";
		}

		if (ofs.is_open ())
		{
			ofs << buf.str ().c_str () << endl;
		}
	}

	return (true);
}

/******************************************************************/

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySortTest
	(
		_t_datalayerproperties &rDataLayerProperties, 
		bayerTreeCacheDescription_t *psCacheDescription, 
		_t_subnodeiter nNodeSize, 
		uint32_t nKeyOffset, 
		uint32_t nKeySize
	)
	:	CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		rDataLayerProperties, 
		psCacheDescription, 
		nNodeSize
	)
	,	CBTreeKeySortDataIf <keySortEntry_t, uint32_t, _t_sizetype> ()
	,	m_nKeyOffset (nKeyOffset)
	,	m_nKeySize (nKeySize)
{
	m_pClRefData = new ::std::multimap<uint32_t, keySortMap_t> ();

	BTREE_ASSERT (m_pClRefData != NULL, "CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySortTest: insufficient memory!");
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySortTest
	(
		CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT
	)
	:	CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		rBT, false
	)
	,	CBTreeKeySortDataIf <keySortEntry_t, uint32_t, _t_sizetype> ()
	,	m_nKeyOffset (rBT.m_nKeyOffset)
	,	m_nKeySize (rBT.m_nKeySize)
{
	m_pClRefData = new ::std::multimap<uint32_t, keySortMap_t> ();

	BTREE_ASSERT (m_pClRefData != NULL, "CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySortTest: insufficient memory!");

	this->assign (rBT);

//	m_pClRefData->insert<::std::multimap<uint32_t, keySortMap_t>::const_iterator> (rBT.m_pClRefData->cbegin (), rBT.m_pClRefData->cend ());

	test ();
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeKeySortTest ()
{
	delete m_pClRefData;
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
int CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::comp (const uint32_t &rKey0, const uint32_t &rKey1)
{
	if (rKey0 < rKey1)
	{
		return (-1);
	}
	else if (rKey0 > rKey1)
	{
		return (1);
	}

	return (0);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
template <class _t_iterator>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	this->template insert_via_iterator<_t_iterator> (sItFirst, sItLast);

	test ();
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert
	(
		const keySortEntry_t &rData
	)
{
	::std::pair<uint32_t, keySortMap_t>		sValue;
	const_iterator							sCIter;

	sValue.first = rData.nKey;
	sValue.second.nData = rData.nData;
	sValue.second.nDebug = rData.nDebug;

	this->m_pClRefData->insert (sValue);
	
	sCIter = CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (rData);

	test ();

	return (sCIter);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterPos
	)
{
	typedef typename ::std::multimap<uint32_t, keySortMap_t>::iterator		itmmap_t;

	uint32_t		sKey = (*sCIterPos).nKey;
	itmmap_t		sItMMapLower = m_pClRefData->lower_bound (sKey);
	itmmap_t		sItMMapUpper = m_pClRefData->upper_bound (sKey);
	itmmap_t		sItMMap;

	for (sItMMap = sItMMapLower; sItMMap != sItMMapUpper; sItMMap++)
	{
		if ((*sItMMap).second.nData == (*sCIterPos).nData)
		{
			if ((*sItMMap).second.nDebug == (*sCIterPos).nDebug)
			{
				m_pClRefData->erase (sItMMap);

				break;
			}
		}
	}

	CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (sCIterPos);

	test ();

	return (sCIterPos);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (const uint32_t &rKey)
{
	_t_sizetype		nRetval;

	m_pClRefData->erase (rKey);

	nRetval = CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (rKey);

	test ();

	return (nRetval);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterFirst, 
		typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterLast
	)
{
	typedef typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator		citer_t;
	typedef typename ::std::multimap<uint32_t, keySortMap_t>::iterator																						itmmap_t;

	citer_t		sCIterRetval;
	citer_t		sCIter;
	uint32_t	sKey;
	itmmap_t	sItMMapLower;
	itmmap_t	sItMMapUpper;
	itmmap_t	sItMMap;

	for (sCIter = sCIterFirst; sCIter != sCIterLast; sCIter++)
	{
		sKey = (*sCIter).nKey;

		sItMMapLower = m_pClRefData->lower_bound (sKey);
		sItMMapUpper = m_pClRefData->upper_bound (sKey);

		for (sItMMap = sItMMapLower; sItMMap != sItMMapUpper; sItMMap++)
		{
			if ((*sItMMap).second.nData == (*sCIter).nData)
			{
				if ((*sItMMap).second.nDebug == (*sCIter).nDebug)
				{
					m_pClRefData->erase (sItMMap);

					break;
				}
			}
		}
	}

	sCIterRetval = CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (sCIterFirst, sCIterLast);

	test ();

	return (sCIterRetval);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap
	(
		CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rKeySort
	)
{
	CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>	&rBtrKeySort = 
		dynamic_cast <CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &> (rKeySort);

	m_pClRefData->swap (*(rKeySort.m_pClRefData));

	CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap (rBtrKeySort);

	test ();
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find (const uint32_t &rKey)
{
	return (CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find (rKey));
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (const uint32_t &rKey)
{
	return (CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (rKey));
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (const uint32_t &rKey)
{
	return (CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (rKey));
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert_tb (const keySortEntry_t &rData)
{
	::std::pair<uint32_t, keySortMap_t>		sValue;
	_t_sizetype								nRslt;
	uint32_t								nKey = rData.nKey;
	keySortMap_t							sMap;

	sMap.nData = rData.nData;
	sMap.nDebug = rData.nDebug;

	sValue.first = nKey;
	sValue.second = sMap;

	m_pClRefData->insert (sValue);

	nRslt = CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert_tb (rData);

	this->test ();

	return (nRslt);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase_tb (const uint32_t &rKey)
{
	typedef ::std::multimap<uint32_t, keySortMap_t>::const_iterator		citer_mmap_t;

	citer_mmap_t							sItMMapLower;
	citer_mmap_t							sItMMapUpper;
	citer_mmap_t							sItMMap;
	const_iterator							sCIterLower;
	const_iterator							sCIterUpper;
	const_iterator							sCIter;
	_t_sizetype								nRslt;
	
	sItMMapLower = m_pClRefData->lower_bound (rKey);
	sItMMapUpper = m_pClRefData->upper_bound (rKey);

	m_pClRefData->erase (sItMMapLower, sItMMapUpper);

	sCIterLower = CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (rKey);
	sCIterUpper = CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (rKey);	

	nRslt = sCIterUpper - sCIterLower;

	CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (sCIterLower, sCIterUpper);

	test ();

	return (nRslt);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase_tb (const uint32_t &rKey, const _t_sizetype instance)
{
	typedef ::std::multimap<uint32_t, keySortMap_t>::const_iterator		citer_mmap_t;

	::std::pair<uint32_t, keySortMap_t>		sValue;
	keySortMap_t							sMap;
	citer_mmap_t							sItMMapLower;
	citer_mmap_t							sItMMapUpper;
	citer_mmap_t							sItMMap;
	const_iterator							sCIterLower;
	const_iterator							sCIterUpper;
	const_iterator							sCIter;
	_t_sizetype								i;
	keySortEntry_t							sEntry;
	bool									bDeleted = false;

	sCIterLower = CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (rKey);
	sCIterUpper = CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (rKey);

	for (sCIter = sCIterLower, i = 0; (sCIter != sCIterUpper) && (i < instance); sCIter++, i++);
	
	BTREE_ASSERT (i == instance, "CBTreeKeySortTest<>::erase_tb (const _t_key &, const _t_sizetype): ERROR: Failed to find instance!");

	sEntry = *sCIter;

	sItMMapLower = m_pClRefData->lower_bound (rKey);
	sItMMapUpper = m_pClRefData->upper_bound (rKey);

	for (sItMMap = sItMMapLower; sItMMap != sItMMapUpper; sItMMap++)
	{
		sMap = (*sItMMap).second;

		if (sMap.nData == sEntry.nData)
		{
			if (sMap.nDebug == sEntry.nDebug)
			{
				m_pClRefData->erase (sItMMap);

				bDeleted = true;

				break;
			}
		}
	}

	BTREE_ASSERT (bDeleted, "CBTreeKeySortTest<>::erase_tb (const _t_key &, const _t_sizetype): ERROR: multi map instance failed to delete!");
	
	CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (sCIter);

	test ();

	return (1);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ()
{
	CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ();

	m_pClRefData->clear ();
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator=
	(CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT)
{
	if (this != &rBT)
	{
		CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		*pThisKeySort;
		CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		*pRefKeySort;

		pThisKeySort = dynamic_cast <CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *> (this);
		pRefKeySort = dynamic_cast <CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *> (&rBT);

		*pThisKeySort = *pRefKeySort;

		test ();
	}

	return (*this);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator== (CBTreeKeySortTest &rKeySort)
{
	if (this == &rKeySort)
	{
		return (true);
	}

	if (this->size () != rKeySort.size ())
	{
		return (false);
	}

	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>							*pKeySetTest;

	pKeySetTest = new CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (*(this->m_pClDataLayerProperties), &(this->m_sCacheDescription), this->m_nNodeSize, this->m_nKeyOffset, this->m_nKeySize);

	BTREE_ASSERT (pKeySetTest != NULL, "CBTreeKeySortTest::operator== (const CBTreeKeySortTest &): insufficient memory!");

	CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		*pKeySort = dynamic_cast <CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *> (&rKeySort);
	CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		*pKeySet;
	_t_sizetype																													i;
	_t_sizetype																													j;
	_t_sizetype																													k;
	_t_sizetype																													nKeySetSize;
	keySortEntry_t																												sThisData;
	keySortEntry_t																												sData;
	keySortEntry_t																												sKey;

	pKeySet = dynamic_cast <CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *> (pKeySetTest);

	for (i = 0; i < this->size (); i += nKeySetSize)
	{
		this->get_at (i, sKey);

		nKeySetSize = pKeySort->get (sKey, *pKeySet);

		if (nKeySetSize != this->count (sKey))
		{
			break;
		}

		for (j = 0; j < nKeySetSize; j++)
		{
			this->get_at (i + j, sThisData);

			for (k = 0; k < pKeySet->size (); k++)
			{
				pKeySet->get_at (k, sData);

				if ((sThisData.nKey == sData.nKey) && (sThisData.nData == sData.nData))
				{
					pKeySet->erase_tb (sKey, k);

					break;
				}
			}
		}

		if (pKeySet->size () != 0)
		{
			break;
		}
	}

	delete pKeySetTest;

	return (i >= this->size ());
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator!= (CBTreeKeySortTest &rKeySort)
{
	return ( ! (*this == rKeySort));
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::test ()
{
	typedef ::std::multimap<uint32_t, keySortMap_t>::const_iterator		citer_mmap_t;

	::std::multimap<uint32_t, keySortMap_t>		sMMap;
	uint32_t									nKey;
	uint32_t									nNextKey;
	bool										bBounce;
	_t_sizetype									nTotalCount = 0;
	keySortEntry_t								sEntry;
	::std::pair<uint32_t, keySortMap_t>			sValue;
	citer_mmap_t								sItMMapLower;
	citer_mmap_t								sItMMapUpper;
	citer_mmap_t								sItMMap;
	const_iterator								sCIterBegin;
	const_iterator								sCIterEnd;
	const_iterator								sCIterLower;
	const_iterator								sCIterUpper;
	const_iterator								sCIter;
	bool										bDeleted;
	
	if (!this->test_integrity ())
	{
		cerr << endl;
		cerr << "integrity test failed" << endl;

		cerr << "creating integrity.html..." << endl;

		this->show_integrity ("integrity.html");

		cerr << "finished!" << endl;

		exit (-1);
	}

	sCIterBegin = CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::cbegin ();
	sCIterEnd = CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::cend ();

	sCIter = sCIterBegin;

	if (this->size () > 0)
	{
		nKey = ((keySortEntry_t) (*sCIter)).nKey;
	}

	while (sCIter != sCIterEnd)
	{
		if (m_pClRefData->count (nKey) != this->count (nKey))
		{
			cerr << endl;
			cerr << "number of instances mismatches" << endl;
			cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << nKey << endl;
			cerr << std::setfill (' ') << std::dec << std::setw (0);

			cerr << "count: " << this->count (nKey) << endl;
			cerr << "reference: " << m_pClRefData->count (nKey) << endl;
			
			cerr << "creating count.html..." << endl;

			this->show_integrity ("count.html");

			cerr << "finished!" << endl;

			exit (-1);
		}

		if (this->count (nKey) == 1)
		{
			sItMMapLower = m_pClRefData->lower_bound (nKey);

			sCIterLower = CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (nKey);

			sValue = *sItMMapLower;

			sEntry = ((keySortEntry_t) (*sCIterLower));

			if ((sEntry.nData != sValue.second.nData) || (sEntry.nDebug != sValue.second.nDebug))
			{
				cerr << endl;
				cerr << "data mismatches" << endl;
				cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << sEntry.nKey << endl;
				cerr << "data: " << sEntry.nData << endl;

				cerr << std::setfill (' ') << std::dec << std::setw (0);

				cerr << "debug: " << sEntry.nDebug << endl;
				cerr << "reference" << endl;

				cerr << "data: " << std::setfill ('0') << std::hex << std::setw (8) << sValue.second.nData << endl;

				cerr << std::setfill (' ') << std::dec << std::setw (0);

				cerr << "debug: " << sValue.second.nDebug << endl;
				
				cerr << "creating data.html..." << endl;

				this->show_integrity ("data.html");

				cerr << "finished!" << endl;

				exit (-1);
			}
		}
		else
		{
			sItMMapLower = m_pClRefData->lower_bound (nKey);
			sItMMapUpper = m_pClRefData->upper_bound (nKey);

			sMMap.insert<citer_mmap_t> (sItMMapLower, sItMMapUpper);

			sCIterLower = CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (nKey);
			sCIterUpper = CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (nKey);

			for (sCIter = sCIterLower; sCIter != sCIterUpper; sCIter++)
			{
				sEntry = ((keySortEntry_t) (*sCIter));

				bDeleted = false;

				for (sItMMap = sMMap.cbegin (); sItMMap != sMMap.cend (); sItMMap++)
				{
					sValue = *sItMMap;

					if (sEntry.nData == sValue.second.nData)
					{
						if (sEntry.nDebug == sValue.second.nDebug)
						{
							sMMap.erase (sItMMap);

							bDeleted = true;

							break;
						}
					}
				}

				if (!bDeleted)
				{
					cerr << endl;
					cerr << "number of instances mismatches" << endl;
					cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << sEntry.nKey << endl;
					cerr << "data: " << sEntry.nData << endl;

					cerr << std::setfill (' ') << std::dec << std::setw (0);

					cerr << "debug: " << sEntry.nDebug << endl;
					cerr << "Instance not found in reference!" << endl;

					cerr << "creating error.html..." << endl;

					this->show_integrity ("error.html");

					cerr << "finished!" << endl;

					exit (-1);
				}
			}

			if (sMMap.size () != 0)
			{
				cerr << endl;
				cerr << "number of instances mismatches" << endl;
				cerr << "the following entries are still present in reference:" << endl;

				for (sItMMap = sMMap.cbegin (); sItMMap != sMMap.cend (); sItMMap++)
				{
					sValue = *sItMMap;

					cerr << "key: ";

					cerr << std::setfill ('0') << std::hex << std::setw (8);
					{
						cerr << sValue.first << " ";
						cerr << "data: " << flush;
						cerr << sValue.second.nData << " ";
					}
					cerr << std::setfill (' ') << std::dec << std::setw (0);

					cerr << "debug: " << sValue.second.nDebug << endl;
				}

				cerr << "creating error.html..." << endl;

				this->show_integrity ("error.html");

				cerr << "finished!" << endl;

				exit (-1);
			}
		}

		CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_next_key (nKey, nNextKey, bBounce);

		if (bBounce)
		{
			break;
		}

		nKey = nNextKey;
	}
	
	if (m_pClRefData->size () != this->size ())
	{
		cerr << endl;
		cerr << "size mismatches" << endl;
		cerr << "size: " << this->size () << endl;
		cerr << "reference size: " << m_pClRefData->size () << endl;

		cerr << "creating size.html..." << endl;

		this->show_integrity ("size.html");

		cerr << "finished!" << endl;

		exit (-1);
	}
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::showdata (std::ofstream &ofs, const _t_nodeiter nNode, const _t_subnodeiter nSubPos)
{
	std::stringstream		buf;
	uint64_t				ntab = 0ULL, notab = 0ULL;
	uint32_t				rData;
	keySortEntry_t			data;
	bool					bError = false;
	_t_nodeiter				nNeightbourNode;
	_t_subnodeiter			nNeightbourSub;
	int						nErrCnt = 0;
	std::stringstream		aszMsg;
	char					*pColorInit = (char *) "";
	char					*pColorExit = (char *) "";
	_t_nodeiter				nPrevNode;
	_t_subnodeiter			nPrevSub;
	_t_nodeiter				nNextNode;
	_t_subnodeiter			nNextSub;
	bool					bBounce;

	if (this->is_leaf (nNode))
	{
		try
		{
			data = this->get_data (nNode, nSubPos);

			rData = data.nKey;
			rData = (rData >> 16) | (rData << 16);
			rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);
			
			buf.clear ();
			buf << "<table border=\"0\" cellspacing=\"1\" width=\"220\"><tr><td>";
			buf << "key: " << hex << setfill ('0') << setw (8) << rData << "<br>";
			buf << "data: " << data.nData << dec << "<br>";
			buf << "debug: " << data.nDebug << "<br>";
			buf << "instance: " << this->get_instancePos (nNode, nSubPos);
			buf << "</td>";

			_t_sizetype		nDiff = this->lower_bound (data.nKey) - this->cbegin ();
			_t_sizetype		nOffset = nDiff + this->get_instancePos (nNode, nSubPos);

			buf << "<td align=\"top\">";

			if (nOffset < m_pClRefData->size ())
			{
				::std::multimap<uint32_t, keySortMap_t>::const_iterator		sItMMap;

				sItMMap = m_pClRefData->cbegin ();

				::std::advance< ::std::multimap<uint32_t, keySortMap_t>::const_iterator, _t_sizetype> (sItMMap, nDiff);

				rData = (*sItMMap).first;
				rData = (rData >> 16) | (rData << 16);
				rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);

				if (data.nKey != (*sItMMap).first)
				{
					buf << "<font color=\"#FF0000\">";
				}
				else
				{
					buf << "<font color=\"#00BB00\">";
				}
				
				buf << "key: " << hex << setfill ('0') << setw (8) << rData << "<br>";
				buf << "</font>";

				if (data.nData != (*sItMMap).second.nData)
				{
					buf << "<font color=\"#AAAA00\">";
				}
				else
				{
					buf << "<font color=\"#00BB00\">";
				}
				
				buf << "data: " << (*sItMMap).second.nData << dec << "<br>";
				buf << "</font>";

				if (data.nDebug != (*sItMMap).second.nDebug)
				{
					buf << "<font color=\"#AAAA00\">";
				}
				else
				{
					buf << "<font color=\"#00BB00\">";
				}
				
				buf << "debug: " << (*sItMMap).second.nDebug << "<br>";
				buf << "</font>";
				buf << "-";
			}
			else
			{
				buf << "<font color=\"#FF0000\">";
				buf << "reference<br>";
				buf << "out of<br>";
				buf << "range";
				buf << "</font>";
			}

			buf << "</td></tr>";
			buf << "</table>" << endl;
		}
		catch (exception *pE)
		{
			if (!ofs.is_open ())
			{
				return (false);
			}

			nErrCnt++;

			aszMsg.clear ();
			aszMsg << "<br>data: corruption (" << pE->what () << ")";

			rData = data.nKey;
			rData = (rData >> 16) | (rData << 16);
			rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);
			
			buf.clear ();
			buf << "key: " << hex << setfill ('0') << setw (8) << rData << dec << "<br>debug: " << data.nDebug << "<br>instance: ---";
		}

		try
		{
			this->move_prev (nNode, nSubPos, nPrevNode, nPrevSub, bBounce);

			if (bBounce == false)
			{
				keySortEntry_t prevData = this->get_data (nPrevNode, nPrevSub);

				if (this->comp (prevData.nKey, data.nKey) > 0)
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
					this->extract_key (this->m_pShortLiveKey, nNeightbourNode, nNeightbourSub);

					if (this->comp (*(this->m_pShortLiveKey), (uint32_t) (((char *)&data)[this->m_nKeyOffset])) == 0)
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
					this->extract_key (this->m_pShortLiveKey, nNeightbourNode, nNeightbourSub);

					if (this->comp (*(this->m_pShortLiveKey), (uint32_t) (((char *)&data)[this->m_nKeyOffset])) == 0)
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
		catch (exception *pE)
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
				keySortEntry_t nextData = this->get_data (nNextNode, nNextSub);

				if (this->comp (nextData.nKey, data.nKey) < 0)
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
		catch (exception *pE)
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
		try
		{
			data = this->get_data (nNode, nSubPos);

			rData = data.nKey;
			rData = (rData >> 16) | (rData << 16);
			rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);
			
			buf.clear ();
			buf << "<table border=\"0\" cellspacing=\"1\" width=\"220\"><tr><td>";
			buf << "key: " << hex << setfill ('0') << setw (8) << rData << "<br>";
			buf << "data: " << data.nData << dec << "<br>";
			buf << "debug: " << data.nDebug << "<br>";
			buf << "instance: " << this->get_instancePos (nNode, nSubPos);
			buf << "</td>";

			_t_sizetype		nDiff = this->lower_bound (data.nKey) - this->cbegin ();
			_t_sizetype		nOffset = nDiff + this->get_instancePos (nNode, nSubPos);

			buf << "<td align=\"top\">";

			if (nOffset < m_pClRefData->size ())
			{
				::std::multimap<uint32_t, keySortMap_t>::const_iterator		sItMMap;

				sItMMap = m_pClRefData->cbegin ();

				::std::advance< ::std::multimap<uint32_t, keySortMap_t>::const_iterator, _t_sizetype> (sItMMap, nDiff);

				rData = (*sItMMap).first;
				rData = (rData >> 16) | (rData << 16);
				rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);

				if (data.nKey != (*sItMMap).first)
				{
					buf << "<font color=\"#FF0000\">";
				}
				else
				{
					buf << "<font color=\"#00BB00\">";
				}
				
				buf << "key: " << hex << setfill ('0') << setw (8) << rData << "<br>";
				buf << "</font>";

				if (data.nData != (*sItMMap).second.nData)
				{
					buf << "<font color=\"#AAAA00\">";
				}
				else
				{
					buf << "<font color=\"#00BB00\">";
				}
				
				buf << "data: " << (*sItMMap).second.nData << dec << "<br>";
				buf << "</font>";

				if (data.nDebug != (*sItMMap).second.nDebug)
				{
					buf << "<font color=\"#AAAA00\">";
				}
				else
				{
					buf << "<font color=\"#00BB00\">";
				}
				
				buf << "debug: " << (*sItMMap).second.nDebug << "<br>";
				buf << "</font>";
				buf << "-";
			}
			else
			{
				buf << "<font color=\"#FF0000\">";
				buf << "reference<br>";
				buf << "out of<br>";
				buf << "range";
				buf << "</font>";
			}

			buf << "</td></tr>";
			buf << "</table>" << endl;
		}
		catch (exception *pE)
		{
			if (!ofs.is_open ())
			{
				return (false);
			}

			nErrCnt++;

			aszMsg.clear ();
			aszMsg << "<br>data: corruption (" << pE->what () << ")";

			rData = data.nKey;
			rData = (rData >> 16) | (rData << 16);
			rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);
			
			buf.clear ();
			buf << "key: " << hex << setfill ('0') << setw (8) << rData << dec << "<br>debug: " << data.nDebug << "<br>instance: ---";
		}

		if (ofs.is_open ())
		{
			ofs << buf.str ().c_str () << endl;
		}
	}

	return (true);
}

#endif // BTREETESTKEYSORT_CPP
