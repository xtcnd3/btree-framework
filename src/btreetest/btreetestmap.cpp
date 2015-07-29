/************************************************************
**
** file:	btreetestmap.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains map application class code derived
** from the b-tree framework's map class to allow
** for the map class being tested by having
** the map application class act as a wrapper.
**
************************************************************/

#ifndef BTREETESTMAP_CPP
#define	BTREETESTMAP_CPP

#include "btreetestmap.h"

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeTestMap
	(_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize)
	:	CBTreeMap<uint32_t, mapMap_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		rDataLayerProperties, 
		psCacheDescription, 
		nNodeSize
	)
	,	m_nDebug (0)
	,	m_pClRef (NULL)
{
	m_pClRef = new ::std::map<uint32_t, mapMap_t> ();

	BTREE_ASSERT (m_pClRef != NULL, "CBTreeTestMap<>::CBTreeTestMap: ERROR: insufficient memory!");
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeTestMap
	(CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign)
	:	CBTreeMap<uint32_t, mapMap_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		dynamic_cast<CBTreeMap<uint32_t, mapMap_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &> (rBT), 
		bAssign
	)
	,	m_nDebug (0)
	,	m_pClRef (NULL)
{
	m_pClRef = new ::std::map<uint32_t, mapMap_t> ();

	BTREE_ASSERT (m_pClRef != NULL, "CBTreeTestMap<>::CBTreeTestMap: ERROR: insufficient memory!");
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeTestMap ()
{
	delete m_pClRef;
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>&
	CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator=
	(
		CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT
	)
{
	if (this != &rBT)
	{
		CBTreeMap_t		&rThisMM = dynamic_cast<CBTreeMap_t &> (*this);
		CBTreeMap_t		&rBTMM = dynamic_cast<CBTreeMap_t &> (rBT);

		rThisMM = rBTMM;

		(*m_pClRef) = *(rBT.m_pClRef);

		test ();
	}

	return (*this);
}

/*
template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::begin ()
{

}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::end ()
{
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator
	CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rbegin ()
{
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator
	CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rend ()
{
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::max_size ()
{
}

*/

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
template <class _t_iterator>
void CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	m_pClRef->template insert<_t_iterator> (sItFirst, sItLast);

	CBTreeMap_t::template insert<_t_iterator> (sItFirst, sItLast);

	test ();
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename ::std::pair<typename CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator, bool>
	CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert
	(
		const value_t &rData
	)
{
	::std::pair<::std::map<uint32_t, mapMap_t>::iterator, bool>		sRefRslt;
	insert_rslt_t													sRslt;
	value_t															sData = rData;

	sData.second.nDebug = m_nDebug;

	m_nDebug++;

	sRefRslt = m_pClRef->insert (sData);

	sRslt = CBTreeMap_t::insert (sData);

	if (sRslt.second != sRefRslt.second)
	{
		cerr << endl;
		cerr << "CBTreeTestMap<>::insert: ERROR: reference claims that key ";
		cerr << std::setfill ('0') << std::hex << std::setw (8);
		{
			cerr << sData.first;
		}
		cerr << std::setfill (' ') << std::dec << std::setw (0);
		cerr << " was ";

		if (sRefRslt.second)
		{
			cerr << "not ";
		}

		cerr << "present, whereas the instance claims that said key was ";

		if (sRslt.second)
		{
			cerr << "not ";
		}

		cerr << "present!" << endl;
		cerr << "creating count.html... ";

		this->show_integrity ("count.html");

		cerr << "finished" << endl;

		exit (-1);
	}

	test ();

	return (sRslt);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterPos
	)
{
	typedef typename ::std::multimap<uint32_t, mapMap_t>::iterator		iter_mm_t;

	_t_sizetype		nPos = sCIterPos.get_pos ();
	iter_mm_t		sItMM;
	iterator		sRslt;

	sItMM = m_pClRef->begin ();

	::std::advance<iter_mm_t, _t_sizetype> (sItMM, nPos);

	m_pClRef->erase (sItMM);

	sRslt = CBTreeMap_t::erase (sCIterPos);

	test ();

	return (sRslt);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (const uint32_t &rKey)
{
	_t_sizetype		nRslt;

	m_pClRef->erase (rKey);

	nRslt = CBTreeMap_t::erase (rKey);

	test ();

	return (nRslt);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterFirst, 
		typename CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterLast
	)
{
	typedef typename ::std::multimap<uint32_t, mapMap_t>::iterator		iter_mm_t;

	_t_sizetype		nPos = sCIterFirst.get_pos ();
	iter_mm_t		sItMMfirst;
	iter_mm_t		sItMMlast;

	sItMMlast = sItMMfirst = m_pClRef->begin ();

	::std::advance<iter_mm_t, _t_sizetype> (sItMMfirst, nPos);

	nPos = sCIterLast.get_pos ();

	::std::advance<iter_mm_t, _t_sizetype> (sItMMlast, nPos);

	m_pClRef->erase (sItMMfirst, sItMMlast);

	return (CBTreeMap_t::erase (sCIterFirst, sCIterLast));
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap (CBTreeTestMap_t &rTMM)
{
	if (this != &rTMM)
	{
		CBTreeMap_t	&rThisMM = dynamic_cast<CBTreeMap_t &> (*this);
		CBTreeMap_t	&rMM = dynamic_cast<CBTreeMap_t &> (rTMM);

		rThisMM.swap (rMM);

		m_pClRef->swap (*(rTMM.m_pClRef));
	}
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ()
{
	m_pClRef->clear ();

	CBTreeMap_t::clear ();
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::key_compare
	CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::key_comp () const
{
	key_compare		sRslt;

	return (sRslt);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::value_compare
	CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::value_comp () const
{
	value_compare	sRslt;

	return (sRslt);
}

/*
template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find (const uint32_t &rKey)
{

}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (const uint32_t &rKey)
{

}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (const uint32_t &rKey)
{

}
*/

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator== (CBTreeTestMap_t &rTMM)
{
	if (this == &rTMM)
	{
		return (true);
	}

	if (this->size () != rTMM.size ())
	{
		return (false);
	}

	const_iterator		sCIterThis;
	const_iterator		sCIterThisBegin;
	const_iterator		sCIterThisEnd;
	const_iterator		sCIterRTMM;
	const_iterator		sCIterRTMMbegin;
	value_t				sThisVal;
	value_t				sTMMval;

	sCIterThisBegin = this->cbegin ();
	sCIterThisEnd = this->cend ();

	sCIterRTMMbegin = rTMM.cbegin ();

	for (sCIterThis = sCIterThisBegin, sCIterRTMM = sCIterRTMMbegin; sCIterThis != sCIterThisEnd; sCIterThis++, sCIterRTMM++)
	{
		sThisVal = *sCIterThis;
		sTMMval = *sCIterRTMM;

		if ((sThisVal.first != sTMMval.first) ||
			(sThisVal.second.nData != sThisVal.second.nData) ||
			(sThisVal.second.nDebug != sTMMval.second.nDebug))
		{
			return (false);
		}
	}

	return (true);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator!= (CBTreeTestMap_t &rTMM)
{
	return (! ((*this) == rTMM));
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::test ()
{
	typedef ::std::multimap<uint32_t, mapMap_t>::const_iterator		citer_mmap_t;

	::std::multimap<uint32_t, mapMap_t>			sMMap;
	uint32_t									nKey;
	uint32_t									nNextKey;
	bool										bBounce;
	_t_sizetype									nTotalCount = 0;
	value_t										sEntry;
	value_t										sValue;
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

	sCIterBegin = this->cbegin ();
	sCIterEnd = this->cend ();

	sCIter = sCIterBegin;

	if (this->size () > 0)
	{
		this->extract_key (&nKey, ((data_t) (*sCIter)));
	}

	while (sCIter != sCIterEnd)
	{
		if (m_pClRef->count (nKey) != this->count (nKey))
		{
			cerr << endl;
			cerr << "number of instances mismatches" << endl;
			cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << nKey << endl;
			cerr << std::setfill (' ') << std::dec << std::setw (0);

			cerr << "count: " << this->count (nKey) << endl;
			cerr << "reference: " << m_pClRef->count (nKey) << endl;
			
			cerr << "creating count.html..." << endl;

			this->show_integrity ("count.html");

			cerr << "finished!" << endl;

			exit (-1);
		}

		if (this->count (nKey) == 1)
		{
			sItMMapLower = m_pClRef->lower_bound (nKey);

			sCIterLower = this->lower_bound (nKey);

			sValue = *sItMMapLower;

			sEntry = ((data_t) (*sCIterLower));

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
			sItMMapLower = m_pClRef->lower_bound (nKey);
			sItMMapUpper = m_pClRef->upper_bound (nKey);

			sMMap.insert<citer_mmap_t> (sItMMapLower, sItMMapUpper);

			sCIterLower = this->lower_bound (nKey);
			sCIterUpper = this->upper_bound (nKey);

			for (sCIter = sCIterLower; sCIter != sCIterUpper; sCIter++)
			{
				sEntry = ((data_t) (*sCIter));

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

		this->get_next_key (nKey, nNextKey, bBounce);

		if (bBounce)
		{
			break;
		}

		nKey = nNextKey;
	}
	
	if (m_pClRef->size () != this->size ())
	{
		cerr << endl;
		cerr << "size mismatches" << endl;
		cerr << "size: " << this->size () << endl;
		cerr << "reference size: " << m_pClRef->size () << endl;

		cerr << "creating size.html..." << endl;

		this->show_integrity ("size.html");

		cerr << "finished!" << endl;

		exit (-1);
	}
}

#endif // !BTREETESTMAP_CPP
