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
		const bayerTreeCacheDescription_t *psCacheDescription, 
		_t_subnodeiter nNodeSize, 
		::std::multimap<const _t_key, keySortMap_t> *pClRefData
	)
	:	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		rDataLayerProperties, 
		psCacheDescription, 
		nNodeSize
	)
	,	m_pClRefData (pClRefData)
	,	m_bAtomicTesting (true)
{
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySortTest
	(
		CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, 
		bool bAssign
	)
	:	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> (rBT, false)
	,	m_pClRefData (NULL)
	,	m_bAtomicTesting (false)
{
	if (bAssign)
	{
		this->_assign (rBT);
	}
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeKeySortTest ()
{
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
template <class _t_iterator>
void CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	this->template insert_via_iterator<_t_iterator> (sItFirst, sItLast);

	test ();
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert
	(
		const _t_data &rData
	)
{
	iterator					sIter;

	sIter = CBTreeKeySort_t::insert (rData);

	test ();

	return (sIter);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterPos
	)
{
	typedef typename reference_t::iterator		itmmap_t;

	_t_key			sKey = (*sCIterPos).first;
	itmmap_t		sItMMapLower = m_pClRefData->lower_bound (sKey);
	itmmap_t		sItMMapUpper = m_pClRefData->upper_bound (sKey);
	itmmap_t		sItMMap;
	iterator		sIter;
	/*
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
	*/
	sIter = CBTreeKeySort_t::erase (sCIterPos);

	test ();

	return (sIter);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (const _t_key &rKey)
{
	_t_sizetype		nRetval;

	//m_pClRefData->erase (rKey);

	nRetval = CBTreeKeySort_t::erase (rKey);

	test ();

	return (nRetval);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterFirst, 
		typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterLast
	)
{
	typedef typename reference_t::iterator			itmmap_t;

	const_iterator		sCIterRetval;
	const_iterator		sCIter;
//	_t_key				sKey;
	itmmap_t			sItMMapLower;
	itmmap_t			sItMMapUpper;
	itmmap_t			sItMMap;
	iterator			sIter;
	/*
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
	*/
	sIter = CBTreeKeySort_t::erase (sCIterFirst, sCIterLast);

	test ();

	return (sIter);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap
	(
		CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rKeySort
	)
{
	CBTreeKeySort_t	&rBtrKeySort = dynamic_cast <CBTreeKeySort_t &> (rKeySort);

	m_pClRefData->swap (*(rKeySort.m_pClRefData));

	CBTreeKeySort_t::swap (rBtrKeySort);

	test ();
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find (const _t_key &rKey)
{
	return (CBTreeKeySort_t::find (rKey));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (const _t_key &rKey)
{
	return (CBTreeKeySort_t::lower_bound (rKey));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (const _t_key &rKey) const
{
	return (CBTreeKeySort_t::lower_bound (rKey));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (const _t_key &rKey)
{
	return (CBTreeKeySort_t::upper_bound (rKey));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (const _t_key &rKey) const
{
	return (CBTreeKeySort_t::upper_bound (rKey));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ()
{
	CBTreeKeySort_t::clear ();

	test ();
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::count (const _t_key &rKey) const
{
	return (CBTreeKeySort_t::count (rKey));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_data CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data_reference
	(
		typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator &rCIter
	)
{
	typedef typename reference_t::const_iterator		mmap_citer_t;

	return (this->template get_data_reference<const_iterator, mmap_citer_t> (rCIter));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_data CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data_reference
	(
		typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator &rCRIter
	)
{
	typedef typename reference_t::const_reverse_iterator		mmap_criter_t;

	return (this->template get_data_reference<const_reverse_iterator, mmap_criter_t> (rCRIter));
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator=
	(const CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT)
{
	if (this != &rBT)
	{
		CBTreeKeySort_t			&rThisKeySort;
		const CBTreeKeySort_t	&rBTKeySort;

		rThisKeySort = dynamic_cast <CBTreeKeySort_t &> (*this);
		rBTKeySort = dynamic_cast <const CBTreeKeySort_t &> (rBT);

		rThisKeySort = rBTKeySort;

		test ();
	}

	return (*this);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::test () const
{
	if (!m_bAtomicTesting)
	{
		return;
	}

	typedef ::std::pair<const _t_key, keySortMap_t>			value_t;

	typedef typename reference_t::const_iterator			citer_mmap_t;

	reference_t									sMMap;
	uint32_t									nKey;
	uint32_t									nNextKey;
	bool										bBounce;
	_t_sizetype									nTotalCount = 0;
	_t_data										sEntry;
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

	sCIterBegin = CBTreeKeySort_t::cbegin ();
	sCIterEnd = CBTreeKeySort_t::cend ();

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

			sCIterLower = CBTreeKeySort_t::lower_bound (nKey);

			value_t						sValue ((value_t) *sItMMapLower);

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

			sCIterLower = CBTreeKeySort_t::lower_bound (nKey);
			sCIterUpper = CBTreeKeySort_t::upper_bound (nKey);

			for (sCIter = sCIterLower; sCIter != sCIterUpper; sCIter++)
			{
				sEntry = ((_t_data) (*sCIter));

				bDeleted = false;

				for (sItMMap = sMMap.cbegin (); sItMMap != sMMap.cend (); sItMMap++)
				{
					value_t				sValue ((value_t) *sItMMap);

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
					value_t						sValue ((value_t) *sItMMap);

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

		CBTreeKeySort_t::get_next_key (nKey, nNextKey, bBounce);

		if (bBounce)
		{
			break;
		}

		nKey = nNextKey;
	}

	if ((m_pClRefData == NULL) && (!this->empty ()))
	{
		cerr << endl;
		cerr << "reference not set while data container not empty" << endl;
		cerr << "size: " << this->size () << endl;

		exit (-1);
	}
	
	if ((m_pClRefData != NULL) && (m_pClRefData->size () != this->size ()))
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
void CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_reference (typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reference_t *pReference)
{
	m_pClRefData = pReference;
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_atomic_testing (bool bEnable)
{
	m_bAtomicTesting = bEnable;
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
int CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::comp (const _t_key &rKey0, const _t_key &rKey1) const
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
_t_key* CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::extract_key (_t_key *pKey, const _t_nodeiter nNode, const _t_subnodeiter nEntry) const
{
	pKey = CBTreeKeySort_t::extract_key (pKey, nNode, nEntry);

	return (pKey);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_key* CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::extract_key (_t_key *pKey, const _t_data &rData) const
{
	*pKey = rData.first;

	return (pKey);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
template<class _t_iterator, class _t_ref_iterator>
_t_data CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data_reference (_t_iterator &rIter)
{
	_t_iterator			sIterBegin;
	_t_ref_iterator		sMMapCIter;

	get_begin (this, sIterBegin);
	
	get_begin (this->m_pClRefData, sMMapCIter);

	_t_sizetype			nOffset = ::std::distance<_t_iterator> (sIterBegin, rIter);

	::std::advance<_t_ref_iterator, _t_sizetype> (sMMapCIter, nOffset);

	return ((_t_data) *sMMapCIter);
}

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::show_data (std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const
{
	typedef typename reference_t::const_iterator		mmap_citer_t;

	_t_data					*psData;
	uint32_t				rData;

	try
	{
		psData = this->get_data (nNode, nSubPos);

		rData = psData->first;
		rData = (rData >> 16) | (rData << 16);
		rData = ((rData >> 8) & 0xFF00FF) | ((rData << 8) & 0xFF00FF00);
		
		rstrData.clear ();
		rstrData << "<table border=\"0\" cellspacing=\"1\" width=\"220\"><tr><td>";
		rstrData << "key: " << hex << setfill ('0') << setw (8) << rData << "<br>";
		rstrData << "data: " << psData->second.nData << dec << "<br>";
		rstrData << "debug: " << psData->second.nDebug << "<br>";
		rstrData << "instance: " << this->get_instancePos (nNode, nSubPos);
		rstrData << "</td>";

		_t_sizetype		nDiff = this->lower_bound (psData->first) - this->cbegin ();
		_t_sizetype		nOffset = nDiff + this->get_instancePos (nNode, nSubPos);

		rstrData << "<td align=\"top\">";

		if (nOffset < m_pClRefData->size ())
		{
			mmap_citer_t		sItMMap;

			sItMMap = m_pClRefData->cbegin ();

			::std::advance (sItMMap, nDiff);

			rData = (*sItMMap).first;
			rData = (rData >> 16) | (rData << 16);
			rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);

			if (psData->first != (*sItMMap).first)
			{
				rstrData << "<font color=\"#FF0000\">";
			}
			else
			{
				rstrData << "<font color=\"#00BB00\">";
			}
			
			rstrData << "key: " << hex << setfill ('0') << setw (8) << rData << "<br>";
			rstrData << "</font>";

			if (psData->second.nData != (*sItMMap).second.nData)
			{
				rstrData << "<font color=\"#AAAA00\">";
			}
			else
			{
				rstrData << "<font color=\"#00BB00\">";
			}
			
			rstrData << "data: " << (*sItMMap).second.nData << dec << "<br>";
			rstrData << "</font>";

			if (psData->second.nDebug != (*sItMMap).second.nDebug)
			{
				rstrData << "<font color=\"#AAAA00\">";
			}
			else
			{
				rstrData << "<font color=\"#00BB00\">";
			}
			
			rstrData << "debug: " << (*sItMMap).second.nDebug << "<br>";
			rstrData << "</font>";
			rstrData << "-";
		}
		else
		{
			rstrData << "<font color=\"#FF0000\">";
			rstrData << "reference<br>";
			rstrData << "out of<br>";
			rstrData << "range";
			rstrData << "</font>";
		}

		rstrData << "</td></tr>";
		rstrData << "</table>" << endl;
	}
	catch (exception *pE)
	{
		if (!ofs.is_open ())
		{
			return (false);
		}

		rszMsg.clear ();
		rszMsg << "<br>data: corruption (" << pE->what () << ")";

		rData = psData->first;
		rData = (rData >> 16) | (rData << 16);
		rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);
		
		rstrData.clear ();
		rstrData << "key: " << hex << setfill ('0') << setw (8) << rData << dec << "<br>debug: " << psData->second.nDebug << "<br>instance: ---";
	}

	return (true);
}

/******************************************************************/

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySortTest
	(
		_t_datalayerproperties &rDataLayerProperties, 
		const bayerTreeCacheDescription_t *psCacheDescription, 
		_t_subnodeiter nNodeSize, 
		typename ::std::multimap<const uint32_t, keySortMap_t> *pClRefData
	)
	:	CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		rDataLayerProperties, 
		psCacheDescription, 
		nNodeSize
	)
	,	m_pClRefData (pClRefData)
	,	m_bAtomicTesting (true)
{
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySortTest
	(
		CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, 
		bool bAssign
	)
	:	CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		rBT, false
	)
	,	m_pClRefData (NULL)
	,	m_bAtomicTesting (false)
{
	if (bAssign)
	{
		this->_assign (rBT);
	}
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeKeySortTest ()
{
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
template <class _t_iterator>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	this->template insert_via_iterator<_t_iterator> (sItFirst, sItLast);

	test ();
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert
	(
		const keySortEntry_t &rData
	)
{
	iterator								sIter;

	sIter = CBTreeKeySort_t::insert (rData);

	test ();

	return (sIter);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterPos
	)
{
	iterator	sIter = CBTreeKeySort_t::erase (sCIterPos);

	test ();

	return (sIter);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (const uint32_t &rKey)
{
	_t_sizetype		nRetval;

	nRetval = CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (rKey);

	test ();

	return (nRetval);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterFirst, 
		typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterLast
	)
{
	iterator			sIter;

	sIter = CBTreeKeySort_t::erase (sCIterFirst, sCIterLast);

	return (sIter);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap
	(
		CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rKeySort
	)
{
	CBTreeKeySort_t	&rBtrKeySort = dynamic_cast <CBTreeKeySort_t &> (rKeySort);

	CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap (rBtrKeySort);

	test ();
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find (const uint32_t &rKey)
{
	return (CBTreeKeySort_t::find (rKey));
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (const uint32_t &rKey)
{
	return (CBTreeKeySort_t::lower_bound (rKey));
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (const uint32_t &rKey) const
{
	return (CBTreeKeySort_t::lower_bound (rKey));
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (const uint32_t &rKey)
{
	return (CBTreeKeySort_t::upper_bound (rKey));
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (const uint32_t &rKey) const
{
	return (CBTreeKeySort_t::upper_bound (rKey));
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ()
{
	CBTreeKeySort_t::clear ();
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
keySortEntry_t CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data_reference
	(
		typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator &rCIter
	)
{
	typedef typename ::std::multimap<const uint32_t, keySortMap_t>::const_iterator		mmap_citer_t;

	return (this->template get_data_reference<const_iterator, mmap_citer_t> (rCIter));
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
keySortEntry_t CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data_reference
	(
		typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator &rCRIter
	)
{
	typedef typename ::std::multimap<const uint32_t, keySortMap_t>::const_reverse_iterator		mmap_criter_t;

	return (this->template get_data_reference<const_reverse_iterator, mmap_criter_t> (rCRIter));
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator=
	(const CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT)
{
	if (this != &rBT)
	{
		CBTreeKeySort_t			&rThisKeySort = dynamic_cast <CBTreeKeySort_t &> (*this);
		const CBTreeKeySort_t	&rBTKeySort = dynamic_cast <const CBTreeKeySort_t &> (rBT);

		rThisKeySort = rBTKeySort;

		test ();
	}

	return (*this);
}

//template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
//bool CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator== (const CBTreeKeySortTest &rKeySort) const
//{
//	typedef typename CBTreeKeySort_t::const_iterator		keysort_citer_t;
//
//	if (this == &rKeySort)
//	{
//		return (true);
//	}
//
//	if (this->size () != rKeySort.size ())
//	{
//		return (false);
//	}
//
//	CBTreeKeySortTest_t							*pKeySetTest;
//
//	pKeySetTest = new CBTreeKeySortTest_t (*(this->m_pClDataLayerProperties), &(this->m_sCacheDescription), this->m_nNodeSize, NULL);
//
//	BTREE_ASSERT (pKeySetTest != NULL, "CBTreeKeySortTest::operator== (const CBTreeKeySortTest &): insufficient memory!");
//
//	const CBTreeKeySort_t	*pKeySort = dynamic_cast <const CBTreeKeySort_t *> (&rKeySort);
//	CBTreeKeySort_t			*pKeySet;
//	_t_sizetype				i;
//	_t_sizetype				j;
//	_t_sizetype				k;
//	_t_sizetype				nKeySetSize;
//	keySortEntry_t			sThisData;
//	keySortEntry_t			sData;
//	keySortEntry_t			sKey;
//	keysort_citer_t			sKeySortCIter;
//
//	pKeySet = dynamic_cast <CBTreeKeySort_t *> (pKeySetTest);
//
//	for (i = 0; i < this->size (); i += nKeySetSize)
//	{
//		this->get_at (i, sKey);
//
//		nKeySetSize = pKeySort->get (sKey, *pKeySet);
//
//		if (nKeySetSize != this->count (sKey))
//		{
//			break;
//		}
//
//		for (j = 0; j < nKeySetSize; j++)
//		{
//			this->get_at (i + j, sThisData);
//
//			sKeySortCIter = pKeySet->cbegin ();
//
//			for (k = 0; k < pKeySet->size (); k++, sKeySortCIter++)
//			{
//				sData = *sKeySortCIter;
//
//				if ((sThisData.nKey == sData.nKey) && (sThisData.nData == sData.nData))
//				{
//					pKeySet->erase_tb (sKey, k);
//
//					break;
//				}
//			}
//		}
//
//		if (pKeySet->size () != 0)
//		{
//			break;
//		}
//	}
//
//	delete pKeySetTest;
//
//	return (i >= this->size ());
//}
//
//template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
//bool CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator!= (const CBTreeKeySortTest &rKeySort) const
//{
//	return ( ! (*this == rKeySort));
//}
//
template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::test () const
{
	if (!m_bAtomicTesting)
	{
		return;
	}

	typedef ::std::multimap<const uint32_t, keySortMap_t>		mmap_t;
	typedef ::std::pair<const uint32_t, keySortMap_t>			value_t;

	typedef typename mmap_t::const_iterator						citer_mmap_t;

	mmap_t										sMMap;
	uint32_t									nKey;
	uint32_t									nNextKey;
	bool										bBounce;
	_t_sizetype									nTotalCount = 0;
	keySortEntry_t								sEntry;
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

			value_t						sValue ((value_t) *sItMMapLower);

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
					value_t						sValue ((value_t) *sItMMap);

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
					value_t						sValue ((value_t) *sItMMap);

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
		cerr << "empty: " << this->empty () << endl;
		cerr << "reference empty: " << m_pClRefData->empty () << endl;

		cerr << "creating size.html..." << endl;

		this->show_integrity ("size.html");

		cerr << "finished!" << endl;

		exit (-1);
	}
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_reference (typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reference_t *pReference)
{
	m_pClRefData = pReference;
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_atomic_testing (bool bEnable)
{
	m_bAtomicTesting = bEnable;
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
int CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::comp (const uint32_t &rKey0, const uint32_t &rKey1) const
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
template<class _t_iterator, class _t_ref_iterator>
keySortEntry_t CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data_reference (_t_iterator &rIter)
{
	typedef typename ::std::multimap<const uint32_t, keySortMap_t>::value_type	mmap_value_type_t;

	keySortEntry_t		sRetval;
	_t_iterator			sIterBegin;
	_t_ref_iterator		sMMapCIter;

	get_begin (this, sIterBegin);
	
	get_begin (this->m_pClRefData, sMMapCIter);

	_t_sizetype			nOffset = ::std::distance<_t_iterator> (sIterBegin, rIter);

	::std::advance<_t_ref_iterator, _t_sizetype> (sMMapCIter, nOffset);

	sRetval.nKey = ((mmap_value_type_t) *sMMapCIter).first;
	sRetval.nData = ((mmap_value_type_t) *sMMapCIter).second.nData;
	sRetval.nDebug = ((mmap_value_type_t) *sMMapCIter).second.nDebug;

	return (sRetval);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::show_data (std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const
{
	keySortEntry_t		*psData;
	uint32_t			rData;

	try
	{
		psData = this->get_data (nNode, nSubPos);

		rData = psData->nKey;
		rData = (rData >> 16) | (rData << 16);
		rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);
		
		rstrData.clear ();
		rstrData << "<table border=\"0\" cellspacing=\"1\" width=\"220\"><tr><td>";
		rstrData << "key: " << hex << setfill ('0') << setw (8) << rData << "<br>";
		rstrData << "data: " << psData->nData << dec << "<br>";
		rstrData << "debug: " << psData->nDebug << "<br>";
		rstrData << "instance: " << this->get_instancePos (nNode, nSubPos);
		rstrData << "</td>";

		_t_sizetype		nDiff = ::std::distance (this->cbegin (), this->lower_bound (psData->nKey));
		_t_sizetype		nOffset = nDiff + this->get_instancePos (nNode, nSubPos);

		rstrData << "<td align=\"top\">";

		if (nOffset < m_pClRefData->size ())
		{
			::std::multimap<const uint32_t, keySortMap_t>::const_iterator		sItMMap;

			sItMMap = m_pClRefData->cbegin ();

			::std::advance (sItMMap, nOffset);

			rData = (*sItMMap).first;
			rData = (rData >> 16) | (rData << 16);
			rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);

			if (psData->nKey != (*sItMMap).first)
			{
				rstrData << "<font color=\"#FF0000\">";
			}
			else
			{
				rstrData << "<font color=\"#00BB00\">";
			}
			
			rstrData << "key: " << hex << setfill ('0') << setw (8) << rData << "<br>";
			rstrData << "</font>";

			if (psData->nData != (*sItMMap).second.nData)
			{
				rstrData << "<font color=\"#AAAA00\">";
			}
			else
			{
				rstrData << "<font color=\"#00BB00\">";
			}
			
			rstrData << "data: " << (*sItMMap).second.nData << dec << "<br>";
			rstrData << "</font>";

			if (psData->nDebug != (*sItMMap).second.nDebug)
			{
				rstrData << "<font color=\"#AAAA00\">";
			}
			else
			{
				rstrData << "<font color=\"#00BB00\">";
			}
			
			rstrData << "debug: " << (*sItMMap).second.nDebug << "<br>";
			rstrData << "</font>";
			rstrData << "-";
		}
		else
		{
			rstrData << "<font color=\"#FF0000\">";
			rstrData << "reference<br>";
			rstrData << "out of<br>";
			rstrData << "range";
			rstrData << "</font>";
		}

		rstrData << "</td></tr>";
		rstrData << "</table>" << endl;
	}
	catch (exception *pE)
	{
		if (!ofs.is_open ())
		{
			return (false);
		}

		rszMsg.clear ();
		rszMsg << "<br>data: corruption (" << pE->what () << ")";

		rData = psData->nKey;
		rData = (rData >> 16) | (rData << 16);
		rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);
		
		rstrData.clear ();
		rstrData << "key: " << hex << setfill ('0') << setw (8) << rData << dec << "<br>debug: " << psData->nDebug << "<br>instance: ---";
	}

	return (true);
}

#endif // BTREETESTKEYSORT_CPP
