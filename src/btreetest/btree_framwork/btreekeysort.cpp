/************************************************************
**
** file:	btreekeysort.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** key sort (ordered associative) data class.
**
************************************************************/

#ifndef	BTREEKEYSORT_CPP
#define	BTREEKEYSORT_CPP

#include "btreekeysort.h"

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySort
	(
		_t_datalayerproperties &rDataLayerProperties, 
		bayerTreeCacheDescription_t *psCacheDescription, 
		_t_subnodeiter nNodeSize
	)
	:	CBTreeBase <CBTreeKeySortPos<_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		rDataLayerProperties, 
		psCacheDescription, 
		nNodeSize
	)
	,	CBTreeKeySortDataIf<_t_data, _t_key, _t_sizetype> ()
{
#if defined (_DEBUG)
	m_bShortLiveKeyInUse = false;
#endif

	m_pShortLiveKey = NULL;
	m_pRemoveKey = NULL;
	m_pTempRemoveKey = NULL;
	m_pInstancesNewKey = NULL;
	m_pTempFindFirstKeyKey = NULL;
	m_pTempFindFirstKeyNewKey = NULL;
	m_pAddToNodeKey = NULL;
	m_pGetNewKey = NULL;
	
	// allocate all variable size temporary buffers
	vbufferAllocate (&m_pShortLiveKey);
	vbufferAllocate (&m_pRemoveKey);
	vbufferAllocate (&m_pTempRemoveKey);
	vbufferAllocate (&m_pInstancesNewKey);
	vbufferAllocate (&m_pTempFindFirstKeyKey);
	vbufferAllocate (&m_pTempFindFirstKeyNewKey);
	vbufferAllocate (&m_pAddToNodeKey);
	vbufferAllocate (&m_pGetNewKey);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySort (CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign)
	:	CBTreeBase <CBTreeKeySortPos<_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		dynamic_cast <CBTreeBase <CBTreeKeySortPos<_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &> (rBT)
	)
	,	CBTreeKeySortDataIf<_t_data, _t_key, _t_sizetype> ()
{
#if defined (_DEBUG)
	m_bShortLiveKeyInUse = false;
#endif

	m_pShortLiveKey = NULL;
	m_pRemoveKey = NULL;
	m_pTempRemoveKey = NULL;
	m_pInstancesNewKey = NULL;
	m_pTempFindFirstKeyKey = NULL;
	m_pTempFindFirstKeyNewKey = NULL;
	m_pAddToNodeKey = NULL;
	m_pGetNewKey = NULL;
	
	// allocate all variable size temporary buffers
	vbufferAllocate (&m_pShortLiveKey);
	vbufferAllocate (&m_pRemoveKey);
	vbufferAllocate (&m_pTempRemoveKey);
	vbufferAllocate (&m_pInstancesNewKey);
	vbufferAllocate (&m_pTempFindFirstKeyKey);
	vbufferAllocate (&m_pTempFindFirstKeyNewKey);
	vbufferAllocate (&m_pAddToNodeKey);
	vbufferAllocate (&m_pGetNewKey);

	if (bAssign)
	{
		assign (rBT);
	}
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeKeySort ()
{
	vbufferDeallocate (&m_pShortLiveKey);
	vbufferDeallocate (&m_pRemoveKey);
	vbufferDeallocate (&m_pTempRemoveKey);
	vbufferDeallocate (&m_pInstancesNewKey);
	vbufferDeallocate (&m_pTempFindFirstKeyKey);
	vbufferDeallocate (&m_pTempFindFirstKeyNewKey);
	vbufferDeallocate (&m_pAddToNodeKey);
	vbufferDeallocate (&m_pGetNewKey);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_key *CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::extract_key (_t_key *pKey, const _t_data &rData)
{
	typename ::std::is_convertible<_t_data, _t_key>::type	sExtractSelect;

	return (CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::extract_key (pKey, rData, sExtractSelect));
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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_key *CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::extract_key (_t_key *pKey, const _t_data &rData, typename ::std::true_type)
{
	_t_data		sData = rData;

	*pKey = (_t_key) sData;

	return (pKey);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_key *CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::extract_key (_t_key *pKey, const _t_data &rData, typename ::std::false_type)
{
#if defined (_DEBUG)

	static int	nCallDepth = 0;

	// if this assertion is hit, then _t_data cannot be cast to _t_key and this or the deriving class has not provided its version of extract_key ()
	BTREE_ASSERT (nCallDepth == 0, "CBTreeKeySort<>::extract_key (_t_key *, const _t_data &, typename ::std::false_type): ERROR: Call depth not zero");

	nCallDepth++;

#endif

	_t_key		*pRslt;

	pRslt = this->extract_key (pKey, rData);

#if defined (_DEBUG)

	nCallDepth--;

#endif

	return (pRslt);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
template <class _t_iterator>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	bool			bSelfReverse;
	_t_iterator		sIt;
	const_iterator	*psItFirst;
	const_iterator	*psItLast;

	if (sItFirst == sItLast)
	{
		return;
	}

	if (this->test_self_reference_of_iterator_to_this (sItFirst, sItLast, bSelfReverse, &psItFirst, &psItLast))
	{
		const_iterator						sIt = (const_iterator) (*psItLast);
		const_iterator						sItLower;
		const_iterator						sItUpper;
		_t_data								sData;
		_t_key								sKey;
		_t_key								sPrevKey;
		bool								bBounce;
		_t_key								sFirstKey;
		_t_key								sLastKey;
		typename ::std::list<_t_data>		sList;

		sData = *((const_iterator) (*psItFirst));

		this->extract_key (&sFirstKey, sData);

		(*psItLast)--;

		sData = *((const_iterator) (*psItLast));

		this->extract_key (&sLastKey, sData);

		(*psItLast)++;

		while (sIt != ((const_iterator) (*psItFirst)))
		{
			sIt--;

			sData = *sIt;

			this->extract_key (&sKey, sData);

			sItLower = CBTreeKeySort_t::lower_bound (sKey);
			sItUpper = CBTreeKeySort_t::upper_bound (sKey);

			if (this->comp (sKey, sFirstKey) == 0)
			{
				sItLower = ((const_iterator) (*psItFirst));
			}

			if (this->comp (sKey, sLastKey) == 0)
			{
				sItUpper = ((const_iterator) (*psItLast));
			}

			sList.clear ();

			sList.template insert <const_iterator> (sList.begin (), sItLower, sItUpper);

			CBTreeKeySort_t::insert<typename ::std::list<_t_data>::const_iterator> (sList.cbegin (), sList.cend ());

			this->get_prev_key (sKey, sPrevKey, bBounce);

			if (bBounce)
			{
				break;
			}

			sKey = sPrevKey;

			sIt = sItLower;
		}

		delete (psItFirst);
		delete (psItLast);
	}
	else
	{
		for (sIt = sItFirst; sIt != sItLast; sIt++)
		{
			CBTreeKeySort_t::insert ((const _t_data &) *sIt);
		}
	}
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert
	(
		const _t_data &rData
	)
{
	_t_sizetype								nPos = 0;
	_t_sizetype								nRetval;
	CBTreeKeySortPos<_t_sizetype, _t_key>	sPos;
	
	this->create_root ();

	// convert object to a position structure for internal use
	//
	// nInstance is left to any (~0), since it will be ignored but 
	// has to be set to not turn up on any valgrind report and this
	// object will be appended to any set of entries that share
	// the same key
	sPos.nInstance = ~0;
	sPos.pKey = extract_key (m_pAddToNodeKey, rData);

	nRetval = this->add_to_node (sPos, rData, this->m_nRootNode, 0, &nPos);

	BTREE_ASSERT (nRetval == 1, "CBTreeKeySort<>::add_to_node: ERROR: Failed to create new entry!");

	CBTreeKeySort_t::const_iterator		sRetval (this, nPos, false);

	return (sRetval);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterPos
	)
{
	this->erase_via_reference (sCIterPos, false);

	return (sCIterPos);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (const _t_key &rKey)
{
	return (CBTreeKeySort_t::erase_tb (rKey));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterFirst, 
		typename CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterLast
	)
{
	_t_sizetype		nRetval = sCIterFirst - this->cbegin ();
	_t_sizetype		nDiff = sCIterLast - sCIterFirst;
	_t_sizetype		i;

	for (i = 0; i < nDiff; i++)
	{
		this->erase_via_reference (sCIterFirst);
	}

	return (const_iterator (this, nRetval));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap 
	(
		CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rKeySort
	)
{
	if (this != &rKeySort)
	{
		fast_swap <_t_key *> (m_pShortLiveKey, rKeySort.m_pShortLiveKey);
		fast_swap <_t_key *> (m_pRemoveKey, rKeySort.m_pRemoveKey);
		fast_swap <_t_key *> (m_pTempRemoveKey, rKeySort.m_pTempRemoveKey);
		fast_swap <_t_key *> (m_pInstancesNewKey, rKeySort.m_pInstancesNewKey);
		fast_swap <_t_key *> (m_pTempFindFirstKeyKey, rKeySort.m_pTempFindFirstKeyKey);
		fast_swap <_t_key *> (m_pTempFindFirstKeyNewKey, rKeySort.m_pTempFindFirstKeyNewKey);
		fast_swap <_t_key *> (m_pAddToNodeKey, rKeySort.m_pAddToNodeKey);
		fast_swap <_t_key *> (m_pGetNewKey, rKeySort.m_pGetNewKey);

#if defined (_DEBUG)

		fast_swap <bool> (m_bShortLiveKeyInUse, rKeySort.m_bShortLiveKeyInUse);

#endif

		CBTreeBase_t	&rThisBase = dynamic_cast <CBTreeBase_t &> (*this);
		CBTreeBase_t	&rKeySortBase = dynamic_cast <CBTreeBase_t &> (rKeySort);

		rThisBase.swap (rKeySortBase);
	}
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find
	(
		const _t_key &rKey
	)
{
	typedef typename CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator		citer_t;
	_t_nodeiter			nNode;
	_t_subnodeiter		nSub;
	_t_sizetype			nPos;

	if (this->find_oneKey (rKey, nNode, nSub, &nPos))
	{
		return (citer_t (this, nPos, nNode, nSub, this->get_time_stamp (), false));
	}
	else
	{
		return (this->cend ());
	}
}

/*

insert - insert data

rData		- data set to be added

This method inserts one entry.

If the method was successful, then the return value is the position of the newly added
entry, otherwise the return value has all bits set ((_t_sizetype) ~0).

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert_tb (const _t_data &rData)
{
	_t_sizetype								nRetval = (_t_sizetype) ~0;
	CBTreeKeySortPos<_t_sizetype, _t_key>	sPos;
	
	this->create_root ();

	// convert object to a position structure for internal use
	//
	// nInstance is left to any (~0), since it will be ignored but 
	// has to be set to not turn up on any valgrind report and this
	// object will be appended to any set of entries that share
	// the same key
	sPos.nInstance = ~0;
	sPos.pKey = extract_key (m_pAddToNodeKey, rData);

	nRetval = this->add_to_node (sPos, rData, this->m_nRootNode, 0);
	
	return (nRetval);
}

/*

erase - removes all entries using the same key

key		- specifies the key which is extracted from the parameter

Every entry using the same as the key extracted from the parameter is removed.

The method returns number of removed entries.

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase_tb (const _t_key &rKey)
{
	CBTreeKeySortPos<_t_sizetype, _t_key>	sPos;
	_t_sizetype								nRetval = (_t_sizetype) 0;

	if (this->m_pData != NULL)
	{
		// convert object to a position structure for internal use
		sPos.pKey = m_pRemoveKey;
		*m_pRemoveKey = rKey;

		// nInstance is left to any (~0), since any entry using the same
		// key will be removed
		sPos.nInstance = ~0;

		// for as long as any instnce using the same key is left ...
		while (count (rKey) > (_t_sizetype) 0)
		{
			// ... continue removing those entries
			nRetval += this->remove_from_node (sPos, this->m_nRootNode, 0);
		}
	}

	return (nRetval);
}

/*

erase - removes one entry

key			- specifies the key which is extracted from this parameter
instance	- specifies the instance of the data set using the same key as extracted from the previous parameter

The specified instance of the entry which uses the same as the one extracted the parameter key is removed.

The method returns number of removed entries.

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase_tb (const _t_key &rKey, const _t_sizetype nInstance)
{
	CBTreeKeySortPos<_t_sizetype, _t_key>		sPos;
	_t_sizetype									retval = (_t_sizetype) 0;

#if defined (_DEBUG)
	BTREE_ASSERT (this->empty () == false, "CBTreeBase<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase_tb (_t_data, uint64_t): cannot remove entry from empty list");
#endif

	// if data layer is present ...
	if (this->m_pData != NULL)
	{
		// ... and nInstance is not set to any ...
		if (nInstance != (_t_sizetype) ~0)
		{
			// ... and nInstance exceeds the number of entries using the key of the entry to be removed ...
			if (nInstance >= count (rKey))
			{
				// ... then return failure
				return ((_t_sizetype) 0);
			}
		}

		// convert object to a position structure for internal use
		*m_pRemoveKey = rKey;
		sPos.pKey = m_pRemoveKey;
		sPos.nInstance = nInstance;

		retval = this->remove_from_node (sPos, this->m_nRootNode, 0);
	}

	return (retval);
}

/*

clear - remove all

This method tell the instance in question to destroy the underlying data layer and
reset all size counts to zero.

The method returns true if it was successful, otherwise false.
If false is returned the data was destroyed already.

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ()
{
	// since the compiler cannot know if you want to call CBTreeKeySortDataIf::clear or CBTreeBase::clear the code has to be explicit
	CBTreeBase<CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ();
}

/*

get - get all entries using the same key

key			- contains an object specifying the key which is to be extracted from this parameter
pObj		- specifies a pointer to an array where the entries will be copied at

This method extracts the key from the object of the first parameter (key) and copies
all entries employing the same key to an array pObj is pointing at. This method assumes the
memory space pObj is pointing at is sufficient enough to hold all entries to be copied.
The method count ought to be used to determine the size of the array pObj.

The method returns the number of entries copied to pObj.

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get (const _t_key &rKey, _t_data *pObj)
{
	_t_sizetype				retval = (_t_sizetype) 0;
	_t_nodeiter				nNode = this->m_nRootNode, nodeSeen;
	_t_subnodeiter			subPos, subPosSeen;
	bool					bBounce;

	// if data layer not present ...
	if (this->m_pData == NULL)
	{
		// ... then return failure
		return (retval);
	}

	// if no entry with that is preset ...
	if (!find_oneKey (rKey, nNode, subPos))
	{
		// ... then return failure
		return (retval);
	}

	// find the first entry of sharing the same key and make it the "seen" position
	find_firstKey (nNode, subPos, nodeSeen, subPosSeen);

	while (true)
	{
		// copy data of current "seen" position to output array
		pObj[retval] = this->get_data (nodeSeen, subPosSeen);

		retval++;

		// move to next entry
		this->move_next (nodeSeen, subPosSeen, nNode, subPos, bBounce);

		// if this was the last entry in this tree ...
		if (bBounce)
		{
			// ... then abort procure
			break;
		}

		// extract key of next position
		extract_key (m_pGetNewKey, nNode, subPos);

		// if next key is not equal to current key ...
		if (comp (*m_pGetNewKey, rKey) != 0)
		{
			// ... then abort procedure
			break;
		}

		// otherwise make the next position the new "seen" position
		nodeSeen = nNode;
		subPosSeen = subPos;
	}

	return (retval);
}

/*

get - get all entries using the same key

rKey		- contains an object specifying the key which is to be extracted from this parameter
rDestData	- specifies a reference to a CBTreeKeySort instance where the entries will be insert to

This method extracts the key from the object of the first parameter (rKey) and inserts
all entries sharing the same key into the instance referred to by rDestData. Any data
contained by rDestData will be destroyed in the process.
The method count is not needed to be called, since rDestData manages its own space.

The method returns the number of entries copied to rDestData.

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get (const _t_key &rKey, CBTreeKeySortDataIf<_t_data, _t_key, _t_sizetype> &rDestData)
{
	_t_sizetype		nRslt;
	_t_sizetype		i;
	_t_data			sData;

	// remove previous content of rBT
	rDestData.clear ();

	// obtain number of instances to be copied
	nRslt = this->count (rKey);

	// copy data from this instance and insert it into rBT
	for (i = 0; i < nRslt; i++)
	{
		this->get (rKey, i, &sData);

		rDestData.insert_tb (sData);
	}

	return (nRslt);
}

/*

get - get one specific instance using key

key			- contains an object specifying the key which is to be extracted from this parameter
nInstance	- specifies the instance of the entry sharing the same key as parameter "key"
pObj		- specifies a pointer where the entry will be copied to

This method extracts the key from the object of the first parameter (key) and copies
the instance of entry sharing the same key to the location pObj is pointing at.
The method count may be used to determine how many instances of the key in question exist.

The method returns true if it was successful, otherwise false.

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get (const _t_key &rKey, _t_sizetype nInstance, _t_data *pObj)
{
	bool					retval = false;
	_t_nodeiter				nNode = this->m_nRootNode, nodeSeen;
	_t_subnodeiter			subPos, subPosSeen;
	bool					bBounce;
	_t_sizetype				u64;

	// if data layer not present ...
	if (this->m_pData == NULL)
	{
		// ... then return failure
		return (retval);
	}

	// if no entry with that is preset ...
	if (!find_oneKey (rKey, nNode, subPos))
	{
		// ... then return failure
		return (retval);
	}

	// find the first entry of sharing the same key and make it the "seen" position
	find_firstKey (nNode, subPos, nodeSeen, subPosSeen);

	// walk through key streak until the instance in question has been found
	for (u64 = 0ULL; u64 < nInstance; u64++)
	{
		// determine next position
		this->move_next (nodeSeen, subPosSeen, nNode, subPos, bBounce);

		// if next position doesn't exist ...
		if (bBounce)
		{
			// ... then abort procedure
			break;
		}

		// extract key of next position
		extract_key (m_pGetNewKey, nNode, subPos);

		// if key of next postion is not equal with current key ...
		if (comp (*m_pGetNewKey, rKey) != 0)
		{
			// ... then abort procedure
			break;
		}

		// assume next position as current position
		nodeSeen = nNode;
		subPosSeen = subPos;
	}

	// if procedure had been aborted ...
	if (u64 < nInstance)
	{
		// ... then return failure
		return (retval);
	}

	retval = true;

	// obtain data instance
	*pObj = this->get_data (nodeSeen, subPosSeen);

#if defined (_DEBUG)
	BTREE_ASSERT (nInstance == get_instancePos (nodeSeen, subPosSeen), "CBTreeBase<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get (_t_data, uint64_t, _t_data *): comparison of external and internal instance position mismatch!");
#endif

	// return success
	return (retval);
}

/*

get_init_pos_of_key - obtain first linear position of one key

key			- contains on object specifying the key which is to be extracted

If a number of entries is sharing the same key, this method is to be used to obtain the initial
position of those entries. However, the key must exists, otherwise the result is undefined.

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_init_pos_of_key (const _t_key &rKey)
{
	_t_sizetype					retval = (_t_sizetype) ~0;
	_t_nodeiter					nNode, fstNode;
	_t_subnodeiter				sub, fstSub;
	uint32_t					comp = 0;
	bool						bContinue;
	node_t						sNodeDesc;

	// if data layer not present ...
	if (this->m_pData == NULL)
	{
		// ... then return failure
		return (retval);
	}

	// if any entry using that key exists ...
	if (find_oneKey (rKey, nNode, sub))
	{
		// ... then find first entry sharing the same key and find its linear position within the tree
		find_firstKey (nNode, sub, fstNode, fstSub);

		nNode = fstNode;
		sub = fstSub;

		retval = (_t_sizetype) 0;

		// move from parent node to parent node until the root node has been found and therewith the linear position
		do
		{
			// get description of current node
			this->get_node (nNode, sNodeDesc);

			bContinue = (nNode != this->m_nRootNode);

			// combine the number of entries in the current node with the result so far
			retval += sub + comp;

			// if this is an inner node ...
			if (!this->is_leaf (nNode))
			{
				// ... then add the number of all entries of previous or left sub nodes
				for (; sub != ~0; sub--)
				{
					retval += this->get_max_index (this->get_sub_node (nNode, sub));
				}
			}

			// if the current node is not the root node ...
			if (bContinue)
			{
				// ... move to the parent node, which during the next iteration will be the current node
				sub = this->find_sub_node_offset (sNodeDesc.nParent, nNode) - 1;
				nNode = sNodeDesc.nParent;

				comp = 1;
			}
		} while (bContinue);
	}

	return (retval);
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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_next_key (const _t_key &rKey, _t_key &rNextKey, bool &bBounce)
{
	_t_nodeiter					nNode, nNextNode;
	_t_subnodeiter				nSub, nNextSub;
	
	bBounce = false;
	
	// if data not present ...
	if (this->empty ())
	{
		// ... then flag bounce since search for not successful and return
		bBounce = true;
		
		return;
	}
	
	nNode = this->m_nRootNode;
	
	while (1)
	{
		nSub = this->get_firstSubPos (nNode, rKey, true);
		
		if (nSub < this->get_data_count (nNode))
		{
			this->extract_key (&rNextKey, nNode, nSub);

			if (this->is_leaf (nNode))
			{
				while (this->comp (rKey, rNextKey) == 0)
				{
					this->move_next (nNode, nSub, nNextNode, nNextSub, bBounce);

					if (bBounce)
					{
						return;
					}

					this->extract_key (&rNextKey, nNextNode, nNextSub);

					nNode = nNextNode;
					nSub = nNextSub;
				}

				break;
			}
			else
			{
				if (this->comp (rKey, rNextKey) == 0)
				{
					this->move_next (nNode, nSub, nNextNode, nNextSub, bBounce);

					if (bBounce)
					{
						return;
					}

					nNode = nNextNode;
					nSub = nNextSub;
				}
				else
				{
					nNode = this->get_sub_node (nNode, nSub);
				}
			}
		}
		else
		{
			nSub--;

			this->move_next (nNode, nSub , nNextNode, nNextSub, bBounce);

			if (bBounce)
			{
				return;
			}

			this->extract_key (&rNextKey, nNode, nSub);

			nNode = nNextNode;
			nSub = nNextSub;

			if (this->comp (rKey, rNextKey) > 0)
			{
				this->extract_key (&rNextKey, nNextNode, nNextSub);

				if (this->comp (rKey, rNextKey) < 0)
				{
					return;
				}
			}
		}
	}
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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_prev_key (const _t_key &rKey, _t_key &rPrevKey, bool &bBounce)
{
	_t_nodeiter					nNode, nPrevNode;
	_t_subnodeiter				nSub, nPrevSub;
	
	bBounce = false;
	
	// if data not present ...
	if (this->empty ())
	{
		// ... then flag bounce since search for not successful and return
		bBounce = true;
		
		return;
	}

	if (this->find_oneKey (rKey, nNode, nSub))
	{
		this->find_firstKey (nNode, nSub, nPrevNode, nPrevSub);

		nNode = nPrevNode;
		nSub = nPrevSub;
	}

	this->move_prev (nNode, nSub, nPrevNode, nPrevSub, bBounce);
				
	if (bBounce)
	{
		return;
	}
	
	this->extract_key (&rPrevKey, nPrevNode, nPrevSub);
}

/*

count - get number of instances of one key

key			- contains on object specifying the key which is to be extracted from this parameter

This method determines the number entries containing the same key as the object of parameter key.

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::count (const _t_key &rKey)
{
	_t_sizetype				nRetval = (_t_sizetype) 0;
	_t_nodeiter				nNode = this->m_nRootNode, nNodeSeen;
	_t_subnodeiter			nSubPos, nSubPosSeen;
	bool					bBounce;

	// if no data is present ...
	if (this->empty ())
	{
		// ... then return
		return (nRetval);
	}

	// if not a single entry can be found using the same key as pkey is pointing at ...
	if (!find_oneKey (rKey, nNode, nSubPos))
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
		extract_key (m_pInstancesNewKey, nNode, nSubPos);

		// if key is no longer matching ...
		if (this->comp (*m_pInstancesNewKey, rKey) != 0)
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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (const _t_key &rKey)
{
	typedef typename CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator		citer_t;

	_t_nodeiter			nNode;
	_t_subnodeiter		nSub;
	_t_sizetype			nPos;
	_t_nodeiter			nRsltNode;
	_t_subnodeiter		nRsltSub;
	

	if (!this->find_oneKey (rKey, nNode, nSub, &nPos))
	{
		return (citer_t (this, nPos, nNode, nSub, this->get_time_stamp (), false));
	}

	nPos -= this->find_firstKey (nNode, nSub, nRsltNode, nRsltSub);

	return (citer_t (this, nPos, nRsltNode, nRsltSub, this->get_time_stamp (), false));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator 
	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (const _t_key &rKey)
{
	_t_key				sKey;
	bool				bBounce;

	this->get_next_key (rKey, sKey, bBounce);

	if (bBounce)
	{
		return (this->cend ());
	}

	return (this->lower_bound (sKey));
}

/*

get_at - get at

nPos		- specifies the the linear position
rData		- reference to where the returned entry is put



This method returns true is the access was successful, otherwise false.

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_at (const _t_sizetype nPos, _t_data &rData)
{
	bool			retval = false;
	_t_nodeiter		nNode;
	_t_subnodeiter	nSubPos;

	// if data layer is present ...
	if (this->m_pData != NULL)
	{
		// ... and the linear position does not exceed the size of this tree ...
		if (nPos < this->size ())
		{
			// ... then obtain of the data entry at the linear position in question
			this->convert_pos_to_container_pos (this->m_nRootNode, nPos, nNode, nSubPos);

			rData = this->get_data (nNode, nSubPos);

			retval = true;
		}
	}

	return (retval);
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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::serialize (const _t_sizetype nStart, const _t_sizetype nLen, _t_data *pData)
{
	CBTreeKeySortPos <_t_sizetype, _t_key>	sPos;
	_t_key									sKey;
	_t_data									sData;

	/*
	**	The actual serialize process is done by CBTreeBase::serialize.
	**	All this method does is to convert nFrom and nTo into the template parameter _ti_pos (CBTreeKeySortPos <_t_sizetype, _t_key>)
	**	and nLen. This has to be done, since CBTreeBase::serialize has no concept of what an _t_sizetype in terms of a position is.
	*/

	get_at (nStart, sData);

	extract_key (&sKey, sData);

	// if initial position is requested or the key is unique ...
	if ((nStart == 0) || (count (sKey) == 1))
	{
		// ... then initial instance of key is implied
		sPos.nInstance = 0;
	}
	else
	{
		// ... otherwise determine instance
		_t_sizetype		nInit = get_init_pos_of_key (sKey);

		sPos.nInstance = nStart - nInit;
	}

	sPos.pKey = &sKey;

	return (CBTreeBase<CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::serialize (sPos, nLen, pData));
}

/*

operator= - assignment operator overload

rBT		- is a reference to a b-tree that will have its content be assigned to this instance

The result is a reference to this instance.

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator=
	(CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT)
{
	// if this is not the same instance as the referenced b-tree (rBT) ...
	if (this != &rBT)
	{
		// ... then destroy this data layer
		this->clear ();

		// and copy all data from rBT to this instance
		assign (rBT);
	}

	return (*this);
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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortPos<_t_sizetype, _t_key> CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::determine_position (CBTreeKeySortPos<_t_sizetype, _t_key> sPos, _t_nodeiter nNode, _t_subnodeiter &nSubPos, _t_subnodeiter &nSubData, bool &bFound)
{
	_t_sizetype		nInstancePos;
	int				triCmpRslt;
	node_t			sNodeDesc;

	this->get_node (nNode, sNodeDesc);

	// if node is a leaf node ...
	if (this->is_leaf (nNode))
	{
		// ... then convert sPos to a linear within this node
		_t_subnodeiter		uBase, ui32;

		nSubData = 0UL;

		// since this is a leaf node and stepping into another sub-node is not possible, it is simply assumed that the data is to be found in this node
		bFound = true;

		// binary sub-divide the data in this node until the correct sub-data position (nSubData) has been idenfied
		for (uBase = 0UL, ui32 = this->get_data_count (sNodeDesc) / 2; ui32 > 0; ui32 >>= 1)
		{
			allocateShortLiveKey ();
			{
				// test for correct position in sub-division
				for (nSubData = uBase; nSubData < this->get_data_count (sNodeDesc); nSubData += ui32)
				{
					// if the key of the current sub-position is smaller than the sought key ...
					if (this->comp (*(extract_key (m_pShortLiveKey, nNode, nSubData)), *(sPos.pKey)) < 0)
					{
						// ... then advance to next sub-division
						uBase = nSubData;
					}
					// if current key is equal to sought key ...
					else if (this->comp (*(extract_key (m_pShortLiveKey, nNode, nSubData)), *(sPos.pKey)) == 0)
					{
						// ... and it is key's instance of interest ...
						if ((sPos.nInstance == ~0ULL) || (sPos.nInstance == get_instancePos (nNode, nSubData)))
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
		if (nSubPos < this->get_data_count (sNodeDesc))
		{
			allocateShortLiveKey ();
			{
				triCmpRslt = this->comp (*(sPos.pKey), *(extract_key (m_pShortLiveKey, nNode, nSubData)));

				// if key of interest matches key at data sub-position ...
				if (triCmpRslt == 0)
				{
					// ... and it is key's instance of interest ...
					if ((get_instancePos (nNode, nSubPos) == sPos.nInstance) || (sPos.nInstance == ~0ULL))
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
			if (sPos.nInstance != ~0ULL)
			{
				// ... then test for other instances that share the same key and if any of those has the correct instance
				allocateShortLiveKey ();
				{
					// walk through node until the end of the node has been reached or a different key has been found
					for (; ((nSubPos < this->get_data_count (sNodeDesc)) && (this->comp (*(sPos.pKey), *(extract_key (m_pShortLiveKey, nNode, nSubPos))) == 0)); nSubPos++, nSubData++)
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
			if (nSubData >= this->get_data_count (sNodeDesc))
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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rebuild_node (const _t_nodeiter nNode, const int32_t triMod, _t_subnodeiter nSubStart)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeKeySort<>::rebuild_node: requested node exceeds tree size");
#endif

	_t_nodeiter									nSubNode;
	node_t										sNodeDesc;
	_t_sizetype									nIdx = (_t_sizetype) 0;
	_t_subnodeiter								ui32;
	_t_subnodeiter								nSubPosOffset;
	int											nLocalTriMod = triMod;

	this->get_node (nNode, sNodeDesc);

	// if nNode is a leaf node ...
	if (this->is_leaf (nNode))
	{
		// ... then use the arithmetic negative size value of this node to determine nMaxIdx
		nIdx = ((_t_subnodeiter) (0 - sNodeDesc.nNumData));

		// Note:	If a node is a leaf node, then the size is displayed as a negative
		//			value, which appears as a 2's complement value, since the size actually is unsigned.
	}
	else
	{
		const _t_subnodeiter						nNumData = this->get_data_count (sNodeDesc);

		this->get_serVector (nNode, this->m_pSerVector);

		// if nMaxIdx has been determined (already set up node) and the fast modifier is set ...
		if ((sNodeDesc.nMaxIdx != 0) && (triMod != 0))
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
				nIdx = this->m_pSerVector[nSubPosOffset] - nSubPosOffset;
			}

			// seek point where serial vector mismatches expected sequence based on sub-node indexes
			for (ui32 = nSubStart; ui32 < (nNumData - 1); ui32++)
			{
				nSubNode = this->get_sub_node (nNode, ui32);
				nIdx += this->get_max_index (nSubNode);

				if (this->m_pSerVector[ui32] != (nIdx + ui32))
				{
					break;
				}
			}

			// update remaining serial vector part by using fast modifier
			for (; ui32 < (nNumData - 1); ui32++)
			{
				this->m_pSerVector[ui32] += triMod;
			}

			nIdx = sNodeDesc.nMaxIdx + triMod;

			if (ui32 == 0)
			{
				this->m_pSerVector[0] = this->get_max_index (this->get_sub_node (nNode, 0));
			}
			else if (ui32 < nNumData)
			{
				const _t_sizetype	nLastIdx = this->get_max_index (this->get_sub_node (nNode, nNumData));
				const _t_sizetype	nSecondLastIdx = this->get_max_index (this->get_sub_node (nNode, nNumData - 1));
				_t_sizetype			nTrialingIdx = nLastIdx;

				nTrialingIdx += nSecondLastIdx;

				nTrialingIdx += 2;

				if (nIdx != (this->m_pSerVector[ui32 - 1] + nTrialingIdx))
				{
					nLocalTriMod = 0;
				}
				else
				{
					// code cannot be sure if the last entry had valid data in the first place
					// re-build last entry from all other values
					this->m_pSerVector[ui32] = nIdx - nLastIdx - 1;

					// the above code doesn't account for new entries
					// if new entry has been created, which is not in the last place ...
					if (this->m_pSerVector[ui32] < this->m_pSerVector[ui32 - 1])
					{
						// ..then fall back to slow full re-build mode
						nLocalTriMod = 0;
					}
				}
			}
		}
		
		// if nMaxIdx has not been determined yet (newly created node) or the fast modifier is not set ...
		if ((sNodeDesc.nMaxIdx == 0) || (nLocalTriMod == 0))
		{
			this->m_pData->set_cacheFreeze (true);
			{
				// ... then fully re-calculate nMaxIdx and serial vector
				for (ui32 = 0UL, nIdx = 0; ui32 < this->get_sub_node_count (sNodeDesc); ui32++)
				{
					nSubNode = this->get_sub_node (nNode, ui32);
					nIdx += this->get_max_index (nSubNode);

					if (ui32 < this->get_data_count (nNode))
					{
						this->m_pSerVector[ui32] = nIdx + ui32;
					}
				}
			}
			this->m_pData->set_cacheFreeze (false);

			nIdx += this->get_data_count (sNodeDesc);
		}

		this->set_serVector (nNode, this->m_pSerVector);
	}

	// write result back
	sNodeDesc.nMaxIdx = nIdx;

	this->set_node (nNode, sNodeDesc);
}

/*

generate_prev_position - generate previous position

This method returns the previous position (CBTreeBase::_ti_pos = CBTreeKeySortPos<_t_sizetype, _t_key>)
based on the parameter sPos and the node / sub-node tuple associated with sPos.

nNode		- specifies the node of the node / sub-position tuple of which the previous position will be generated from
nSub		- specifies the sub_position of the node / sub-position tuple of which the previous position will be generated from
sPos		- class specific position associated with node / sub-node tuple

The return value is the previous linear position of sPos (nNode, nSub) in terms of how data is
presented to the application. Given that the b-tree classes are supposed to act like a linear
list to calling the application, this pseudo code displays what generate_prev_position does:

return_value = (*this)[sPos (this, nNode, nSub) - 1];

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortPos<_t_sizetype, _t_key> CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::generate_prev_position (const _t_nodeiter nNode, const _t_subnodeiter nSub, CBTreeKeySortPos<_t_sizetype, _t_key> sPos)
{
	_t_nodeiter		nPrevNode;
	_t_subnodeiter	nPrevSubPos;
	bool			bBounce;
	_t_data			sData;

	// get previous position within tree
	this->move_prev (nNode, nSub, nPrevNode, nPrevSubPos, bBounce);

#if defined (_DEBUG)
	BTREE_ASSERT (!this->is_leaf (nNode), "CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::generate_prev_position: This method cannot be used on leaf nodes!");
	BTREE_ASSERT (!bBounce, "CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::generate_prev_position: unexpected bounce!");
#endif

	sData = this->get_data (nPrevNode, nPrevSubPos);

	// get key of previous position
	m_pTempRemoveKey = extract_key (m_pTempRemoveKey, sData);

	sPos.pKey = m_pTempRemoveKey;
	sPos.nInstance = get_instancePos (nPrevNode, nPrevSubPos);

	return (sPos);
}

/*

generate_next_position - generate next position

This method returns the next position (CBTreeBase::_ti_pos = CBTreeKeySortPos<_t_sizetype, _t_key>)
based on the parameter sPos and the node / sub-node tuple associated with sPos.

nNode		- specifies the node of the node / sub-position tuple of which the next position will be generated from
nSub		- specifies the sub_position of the node / sub-position tuple of which the next position will be generated from
sPos		- class specific position associated with node / sub-node tuple

The return value is the next linear position of sPos (nNode, nSub) in terms of how data is
presented to the application. Given that the b-tree classes are supposed to act like a linear
list to calling the application, this pseudo code displays what generate_prev_position does:

return_value = (*this)[sPos (this, nNode, nSub) + 1];

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortPos<_t_sizetype, _t_key> CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::generate_next_position (_t_nodeiter nNode, _t_subnodeiter nSub, CBTreeKeySortPos<_t_sizetype, _t_key> sPos)
{
	_t_nodeiter		nNextNode;
	_t_subnodeiter	nNextSubPos;
	bool			bBounce;
	_t_data			sData;

	// get previous position within tree
	this->move_next (nNode, nSub, nNextNode, nNextSubPos, bBounce);

#if defined (_DEBUG)
	BTREE_ASSERT (!this->is_leaf (nNode), "CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::generate_prev_position: This method cannot be used on leaf nodes!");
	BTREE_ASSERT (!bBounce, "CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::generate_prev_position: unexpected bounce!");
#endif

	sData = this->get_data (nNextNode, nNextSubPos);

	// get key of previous position
	m_pTempRemoveKey = extract_key (m_pTempRemoveKey, sData);

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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
int CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::comp (const _t_key &rKey0, const _t_key &rKey1)
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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_key *CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::extract_key (_t_key *pKey, const _t_nodeiter nNode, const _t_subnodeiter nEntry)
{
	_t_data				sData;

#if defined (_DEBUG)
	BTREE_ASSERT (nEntry < this->get_data_count (nNode), "CBTreeBase<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::peek_key (void *, uint64_t, uint32_t): requested entry exceeds nodes data size");
#endif

	// obtain dataset
	sData = this->get_data (nNode, nEntry);

	// convert dataset to key
	return (extract_key (pKey, sData));
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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_instancePos (_t_nodeiter nNode, _t_subnodeiter nSub)
{
	_t_sizetype				nRetval = (_t_sizetype) 0;
	bool					bBounce;
	_t_nodeiter				nFromNode = nNode;
	_t_subnodeiter			nFromSub = nSub;

	// determine one key of the key raw in question
	m_pTempFindFirstKeyKey = extract_key (m_pTempFindFirstKeyKey, nFromNode, nFromSub);

	// continue searching for the initial position of key raw until either the beginning of the list or a different key has been found
	while (true)
	{
		// update output position
		nNode = nFromNode;
		nSub = nFromSub;

		if (	this->is_leaf (nNode) || 
				(nSub == 0) || 
				(this->comp (*m_pTempFindFirstKeyKey, *(extract_key (m_pTempFindFirstKeyNewKey, nNode, nSub - 1))) != 0)
			)
		{
			// move to previous position
			this->move_prev (nNode, nSub, nFromNode, nFromSub, bBounce);
		}
		else
		{
			bBounce = false;

			nFromSub = nSub - 1;

			nRetval += this->get_max_index (this->get_sub_node (nNode, nSub));
		}

		// if beginning the of the list has been reached ...
		if (bBounce)
		{
			// ... then abort searching
			break;
		}

		extract_key (m_pTempFindFirstKeyNewKey, nFromNode, nFromSub);

		// if a different key has been found ...
		if (this->comp (*m_pTempFindFirstKeyNewKey, *m_pTempFindFirstKeyKey) != 0)
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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_firstSubPos (_t_nodeiter nNode, _t_key const &rKey, bool bReverse)
{
	node_t					sNodeDesc;
	_t_subnodeiter			ui32;
	_t_subnodeiter			nMinPos, nMaxPos;
	int						triCmpRslt = ~0x0;

	// load node descriptor
	this->get_node (nNode, sNodeDesc);

	// set range up
	nMinPos = 0UL;

	nMaxPos = this->get_data_count (sNodeDesc);

	// until first sub postion has been found ...
	while (nMinPos != nMaxPos)
	{
		// sub divide range and determine average middle position
		ui32 = (nMinPos + nMaxPos) / 2;

		allocateShortLiveKey ();

		// get key of current middle position
		m_pShortLiveKey = extract_key (m_pShortLiveKey, nNode, ui32);

		triCmpRslt = this->comp (rKey, *m_pShortLiveKey);

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

		freeShortLiveKey ();
	}

	if (bReverse)
	{
		if (triCmpRslt == 0)
		{
			// set range up
			nMaxPos = this->get_data_count (sNodeDesc);

			// until last equal sub postion has been found ...
			while ((nMinPos + 1) != nMaxPos)
			{
				// sub divide range and determine average middle position
				ui32 = (nMinPos + nMaxPos) / 2;

				allocateShortLiveKey ();

				// get key of current middle position
				m_pShortLiveKey = extract_key (m_pShortLiveKey, nNode, ui32);

				triCmpRslt = this->comp (rKey, *m_pShortLiveKey);

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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find_next_sub_pos (const _t_nodeiter nNode, CBTreeKeySortPos<_t_sizetype, _t_key> &sPos)
{
	node_t					sNodeDesc;
	_t_subnodeiter			ui32;
	_t_subnodeiter			nMinPos, nMaxPos;

	// load node descriptor
	this->get_node (nNode, sNodeDesc);

	// set range up
	nMinPos = 0UL;

	nMaxPos = this->get_data_count (sNodeDesc);

	// until nearest sub postion has been found ...
	while (nMinPos != nMaxPos)
	{
		// sub divide range and determine average middle position
		ui32 = (nMinPos + nMaxPos) / 2;

		allocateShortLiveKey ();

		// get key of current middle position
		m_pShortLiveKey = extract_key (m_pShortLiveKey, nNode, ui32);

		// if search key is smaller than middle key ...
		if (this->comp (*(sPos.pKey), *m_pShortLiveKey) < 0)
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

pkey		- specifies the pointer to the key in question
nNode		- returns the node in which an equal key has been found
nSub		- returns the sub-position within the node in which an equal key has been found

If it was possible to find any matching key, then true is returned, otherwise false.

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find_oneKey (const _t_key &rKey, _t_nodeiter &nNode, _t_subnodeiter &nSub, _t_sizetype *pnPos)
{
	uint32_t	bFound;

	nNode = this->m_nRootNode;

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
		node_t	sNodeDesc;
			
		this->get_node (nNode, sNodeDesc);

		// if key can be found in current node ...
		if (nSub < this->get_data_count (sNodeDesc))
		{
			// ... then test if key is part of the current node

			allocateShortLiveKey ();

			// extract key from node's sub postition
			m_pShortLiveKey = extract_key (m_pShortLiveKey, nNode, nSub);

			// if extracted key is equal to searched key ...
			if (this->comp (*m_pShortLiveKey, rKey) == 0)
			{
				// ... then abort searching
				bFound = 1;
			}

			freeShortLiveKey ();
		}

		if (pnPos != NULL)
		{
			_t_subnodeiter	i;

			if (!this->is_leaf (nNode))
			{
				for (i = 0; i < (nSub + bFound); i++)
				{
					_t_nodeiter		nSubNode = this->get_sub_node (nNode, i);

					(*pnPos) += this->get_max_index (nSubNode);
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
		nNode = this->get_sub_node (nNode, nSub);
	}

	return (true);
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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find_firstKey (_t_nodeiter nFromNode, _t_subnodeiter nFromSub, _t_nodeiter &nNode, _t_subnodeiter &nSub)
{
	_t_sizetype		nRetval = 0;
	_t_sizetype		nStepSize = 0;
	_t_nodeiter		nSubNode;
	bool			bBounce;
	
	// determine one key of the key raw in question
	m_pTempFindFirstKeyKey = extract_key (m_pTempFindFirstKeyKey, nFromNode, nFromSub);

	// continue searching for the initial position of key raw until either the beginning of the list or a different key has been found
	while (1)
	{
		// update output position
		nNode = nFromNode;
		nSub = nFromSub;

		nRetval += nStepSize;

		if (	this->is_leaf (nNode) || 
				(nSub == 0) || 
				(this->comp (*m_pTempFindFirstKeyKey, *(extract_key (m_pTempFindFirstKeyNewKey, nNode, nSub - 1))) != 0)
			)
		{
			// move to previous position
			this->move_prev (nNode, nSub, nFromNode, nFromSub, bBounce);

			nStepSize = 1;
		}
		else
		{
			bBounce = false;

			nSubNode = this->get_sub_node (nFromNode, nFromSub);

			nFromSub = nSub - 1;

			nStepSize = this->get_max_index (nSubNode);
			nStepSize += 1;
		}

		// if beginning the of the list has been reached or a different key has been found ...
		if (bBounce)
		{
			// ... then abort searching
			break;
		}

		extract_key (m_pTempFindFirstKeyNewKey, nFromNode, nFromSub);

		if (this->comp (*m_pTempFindFirstKeyNewKey, *m_pTempFindFirstKeyKey) != 0)
		{
			break;
		}
	}

	return (nRetval);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase_via_reference
	(
		typename CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator &rCIterPos, 
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
			// ... and if the node to be modified is a leaf node ...
			if (this->is_leaf (rCIterPos.get_node ()))
			{
				// ... and if the node to be modified has enough entries left ...
				if (this->get_data_count (rCIterPos.get_node ()) > (this->get_node_max_data_size () / 2))
				{
					// ... then an additional entry can be added fast
					_t_nodeiter			nNode;

					bFallBack = false;

					// insert entry into leaf node
					this->remove_data_from_leaf (rCIterPos.get_node (), rCIterPos.get_sub_pos ());

					// rebuild all parent node's integrity by back-tracing to the root node
					nNode = rCIterPos.get_node ();

					while (nNode != this->m_nRootNode)
					{
						nNode = this->get_parent (nNode);

						this->rebuild_node (nNode, -1);
					}

					// update time stamp
					this->update_time_stamp ();

					if (rCIterPos.get_sub_pos () < this->get_data_count (rCIterPos.get_node ()))
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
		CBTreeKeySortPos<_t_sizetype, _t_key>	sPos;
		_t_sizetype								nPos = rCIterPos.get_pos ();
		_t_nodeiter								nNode;
		_t_subnodeiter							nSubPos;

		this->convert_pos_to_container_pos (this->m_nRootNode, nPos, nNode, nSubPos);

		this->extract_key (m_pRemoveKey, nNode, nSubPos);

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

/*

vbufferAllocate - allocate variable size buffer

This method de-allocates the buffer *pp if that is required and allocates a new buffer
of size of _t_key.

Note:	The intention is to be able to allocate and de-allocate temporary key buffers as soon as
		unload is called and to have them reinstated when needed. This has not been implemented
		yet and therefore vbufferAllocate and vbufferDeallocate seem like overkill.

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::vbufferAllocate (_t_key **pp)
{
	if (*pp != NULL)
	{
		vbufferDeallocate (pp);
	}

	*pp = new _t_key;

	BTREE_ASSERT (*pp != NULL, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayer>::vbufferAllocate: insufficient memory");
}

/*

vbufferDeallocate - de-allocate variable size buffer

This method de-allocates the buffer *pp if that is required.

Note:	The intention is to be able to allocate and de-allocate temporary key buffers as soon as
		unload is called and to have them reinstated when needed. This has not been implemented
		yet and therefore vbufferAllocate and vbufferDeallocate seem like overkill.

*/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::vbufferDeallocate (_t_key **pp)
{
	if (*pp != NULL)
	{
		delete *pp;
	}

	*pp = NULL;
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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::allocateShortLiveKey ()
{
#if defined (_DEBUG)
	BTREE_ASSERT (!m_bShortLiveKeyInUse, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayer>::allocateShortLiveKey: DEBUG: short live key in use!");

	m_bShortLiveKeyInUse = true;
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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::freeShortLiveKey ()
{
#if defined (_DEBUG)
	BTREE_ASSERT (m_bShortLiveKeyInUse, "CBTreeBase<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayer>::freeShortLiveKey: DEBUG: short live key was already de-allocated!");

	m_bShortLiveKeyInUse = false;
#endif
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::showdata (std::ofstream &ofs, const _t_nodeiter nNode, const _t_subnodeiter nSubPos)
{
	return (true);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::assign
	(CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT)
{
	// if the source contains no data ...
	if (rBT.size () == 0)
	{
		// ... then remove all data in this list
		this->clear ();

		return;
	}
	
	/*
	**	Although a b-tree guarantees that data is stored balanced; this happens under the assumption that 
	**	data entries are stored and removed in random order. In case data is stored or removed in order, the
	**	tree will become partially unblanced. To prevent this the data is not copied in order, but in a
	**	skewed fashion. The skew width is determined by the target tree's maximum node size (t * 2) and target
	**	tree's future depth (based on the source tree's size) combined, since that ensures an equal distribution
	**	of all data throughout the entire target tree.
	*/
	
	// determine initial part of skew width
	_t_sizetype			nSkewWidth = this->m_nNodeSize * 2;
	_t_sizetype			nEntries = 1;
	_t_sizetype			i;
	_t_sizetype			j;
	_t_data				sData;

	// combine skew width with eventual target tree depth (log t of n, while t is node size and n is tree size)
	for (nSkewWidth--; nEntries <= rBT.size (); nSkewWidth++)
	{
		// on average every new layer allows for t times current tree size more entries
		nEntries *= this->m_nNodeSize;
	}

	// copy data using skew width
	for (i = 0; i < nSkewWidth; i++)
	{
		for (j = i; j < rBT.size (); j += nSkewWidth)
		{
			rBT.get_at (j, sData);

			insert_tb (sData);
		}
	}
}

#endif // BTREEKEYSORT_CPP

