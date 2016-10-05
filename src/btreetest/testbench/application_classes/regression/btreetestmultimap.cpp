/************************************************************
**
** file:	btreetestmultimap.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains multi map application class code derived
** from the b-tree framework's multi map class to allow
** for the multi map class being tested by having
** the multi map application class act as a wrapper.
**
************************************************************/

#ifndef BTREETESTMULTIMAP_CPP
#define	BTREETESTMULTIMAP_CPP

#include "testbench/application_classes/regression/btreetestmultimap.h"

template<class _t_datalayerproperties>
CBTreeTestMultiMap<_t_datalayerproperties>::CBTreeTestMultiMap
	(
		_t_datalayerproperties &rDataLayerProperties, 
		const bayerTreeCacheDescription_t *psCacheDescription, 
		typename _t_datalayerproperties::sub_node_iter_type nNodeSize, 
		typename CBTreeTestMultiMap<_t_datalayerproperties>::reference_t *pClRefData
	)
	:	CBTreeMultiMap<uint32_t, multiMapMap_t, _t_datalayerproperties>
	(
		rDataLayerProperties, 
		psCacheDescription, 
		nNodeSize
	)
	,	m_pClRef (pClRefData)
	,	m_bAtomicTesting (true)
{
}

template<class _t_datalayerproperties>
CBTreeTestMultiMap<_t_datalayerproperties>::CBTreeTestMultiMap
	(const CBTreeTestMultiMap<_t_datalayerproperties> &rBT, bool bAssign)
	:	CBTreeMultiMap<uint32_t, multiMapMap_t, _t_datalayerproperties>
	(
		dynamic_cast<const CBTreeMultiMap<uint32_t, multiMapMap_t, _t_datalayerproperties> &> (rBT), 
		false
	)
	,	m_pClRef (NULL)
	,	m_bAtomicTesting (false)
{
	if (bAssign)
	{
		this->_assign (rBT);
	}

	this->set_atomic_testing (true);
}

template<class _t_datalayerproperties>
CBTreeTestMultiMap<_t_datalayerproperties>::~CBTreeTestMultiMap ()
{
}

template<class _t_datalayerproperties>
CBTreeTestMultiMap<_t_datalayerproperties>&
	CBTreeTestMultiMap<_t_datalayerproperties>::operator=
	(
		const CBTreeTestMultiMap<_t_datalayerproperties> &rBT
	)
{
	if (this != &rBT)
	{
		CBTreeMultiMap_t		&rThisMM = dynamic_cast<CBTreeMultiMap_t &> (*this);
		const CBTreeMultiMap_t	&rBTMM = dynamic_cast<const CBTreeMultiMap_t &> (rBT);

		rThisMM = rBTMM;

		(*m_pClRef) = *(rBT.m_pClRef);

		test ();
	}

	return (*this);
}

template<class _t_datalayerproperties>
template<class _t_iterator>
void CBTreeTestMultiMap<_t_datalayerproperties>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	CBTreeMultiMap_t::insert (sItFirst, sItLast);

	test ();
}

template<class _t_datalayerproperties>
template<class _t_iterator, class _t_ref_iterator>
void CBTreeTestMultiMap<_t_datalayerproperties>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	_t_ref_iterator		sCItFirst (sItFirst);
	_t_ref_iterator		sCItLast (sItLast);

	CBTreeMultiMap_t::insert (sItFirst, sItLast);

	test ();
}

template<class _t_datalayerproperties>
typename CBTreeTestMultiMap<_t_datalayerproperties>::iterator
	CBTreeTestMultiMap<_t_datalayerproperties>::insert
	(
		const typename CBTreeTestMultiMap<_t_datalayerproperties>::value_type &rData
	)
{
	iterator		sIter;
	value_type		sData = rData;

	sIter = CBTreeMultiMap_t::insert (sData);

	if (m_pClRef != NULL)
	{
		if (CBTreeMultiMap_t::count (sData.first) != m_pClRef->count (sData.first))
		{
			::std::cerr << ::std::endl;
			::std::cerr << "CBTreeTestMultiMap<>::insert: ERROR: multi key size mismatch!" << ::std::endl;
			::std::cerr << "key: ";
			::std::cerr << std::setfill ('0') << std::hex << std::setw (8);
			{
				::std::cerr << sData.first;
			}
			::std::cerr << std::setfill (' ') << std::dec << std::setw (0);
			::std::cerr << "reference: " << m_pClRef->count (sData.first) << ::std::endl;
			::std::cerr << "container: " << CBTreeMultiMap_t::count (sData.first) << ::std::endl;
			
			::std::cerr << "creating count.html... ";

			this->show_integrity ("count.html");

			::std::cerr << "finished" << ::std::endl;

			exit (-1);
		}
	}

	test ();

	return (sIter);
}

template<class _t_datalayerproperties>
typename CBTreeTestMultiMap<_t_datalayerproperties>::iterator
	CBTreeTestMultiMap<_t_datalayerproperties>::erase
	(
		typename CBTreeTestMultiMap<_t_datalayerproperties>::const_iterator sCIterPos
	)
{
	iterator		sRslt;

	sRslt = CBTreeMultiMap_t::erase (sCIterPos);

	test ();

	return (sRslt);
}

template<class _t_datalayerproperties>
typename _t_datalayerproperties::size_type CBTreeTestMultiMap<_t_datalayerproperties>::erase (const uint32_t &rKey)
{
	size_type		nRslt;

	nRslt = CBTreeMultiMap_t::erase (rKey);

	test ();

	return (nRslt);
}

template<class _t_datalayerproperties>
typename CBTreeTestMultiMap<_t_datalayerproperties>::iterator
	CBTreeTestMultiMap<_t_datalayerproperties>::erase
	(
		typename CBTreeTestMultiMap<_t_datalayerproperties>::const_iterator sCIterFirst, 
		typename CBTreeTestMultiMap<_t_datalayerproperties>::const_iterator sCIterLast
	)
{
	iterator	sRslt = CBTreeMultiMap_t::erase (sCIterFirst, sCIterLast);

	test ();
		
	return (sRslt);
}

template<class _t_datalayerproperties>
void CBTreeTestMultiMap<_t_datalayerproperties>::swap (CBTreeTestMultiMap_t &rTMM)
{
	if (this != &rTMM)
	{
		CBTreeMultiMap_t	&rThisMM = dynamic_cast<CBTreeMultiMap_t &> (*this);
		CBTreeMultiMap_t	&rMM = dynamic_cast<CBTreeMultiMap_t &> (rTMM);

		rThisMM.swap (rMM);
	}

	test ();
}

template<class _t_datalayerproperties>
void CBTreeTestMultiMap<_t_datalayerproperties>::clear ()
{
	CBTreeMultiMap_t::clear ();

	test ();
}

//template<class _t_datalayerproperties>
//typename CBTreeTestMultiMap<_t_datalayerproperties>::key_compare
//	CBTreeTestMultiMap<_t_datalayerproperties>::key_comp () const
//{
//	key_compare		sRslt;
//
//	sRslt.pThis = (CBTreeAssociativeIf_t *) (this);
//
//	return (sRslt);
//}
//
//template<class _t_datalayerproperties>
//typename CBTreeTestMultiMap<_t_datalayerproperties>::value_compare
//	CBTreeTestMultiMap<_t_datalayerproperties>::value_comp () const
//{
//	value_compare	sRslt;
//
//	sRslt.pThis = (CBTreeAssociativeIf_t *) (this);
//
//	return (sRslt);
//}

template<class _t_datalayerproperties>
bool CBTreeTestMultiMap<_t_datalayerproperties>::operator== (const CBTreeTestMultiMap_t &rTMM) const
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
	value_type			sThisVal;
	value_type			sTMMval;

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

template<class _t_datalayerproperties>
bool CBTreeTestMultiMap<_t_datalayerproperties>::operator!= (const CBTreeTestMultiMap_t &rTMM) const
{
	return (! ((*this) == rTMM));
}

template<class _t_datalayerproperties>
void CBTreeTestMultiMap<_t_datalayerproperties>::test () const
{
	if (!m_bAtomicTesting)
	{
		return;
	}

	typedef typename reference_t::const_iterator		citer_mmap_t;

	reference_t									sMMap;
	key_type									nKey;
	key_type									nNextKey;
	bool										bBounce;
	size_type									nTotalCount = 0;
	value_type									sEntry;
	value_type									sValue;
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
		::std::cerr << ::std::endl;
		::std::cerr << "integrity test failed" << ::std::endl;

		::std::cerr << "creating integrity.html..." << ::std::endl;

		this->show_integrity ("integrity.html");

		::std::cerr << "finished!" << ::std::endl;

		exit (-1);
	}

	sCIterBegin = this->cbegin ();
	sCIterEnd = this->cend ();

	sCIter = sCIterBegin;

	if (this->size () > 0)
	{
		this->extract_key (&nKey, ((value_type) (*sCIter)));
	}

	while (sCIter != sCIterEnd)
	{
		if (m_pClRef->count (nKey) != this->count (nKey))
		{
			::std::cerr << ::std::endl;
			::std::cerr << "number of instances mismatches" << ::std::endl;
			::std::cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << nKey << ::std::endl;
			::std::cerr << std::setfill (' ') << std::dec << std::setw (0);

			::std::cerr << "count: " << this->count (nKey) << ::std::endl;
			::std::cerr << "reference: " << m_pClRef->count (nKey) << ::std::endl;
			
			::std::cerr << "creating count.html..." << ::std::endl;

			this->show_integrity ("count.html");

			::std::cerr << "finished!" << ::std::endl;

			exit (-1);
		}

		if (this->count (nKey) == 1)
		{
			sItMMapLower = m_pClRef->lower_bound (nKey);

			sCIterLower = this->lower_bound (nKey);

			sValue = *sItMMapLower;

			sEntry = ((value_type) (*sCIterLower));

			if ((sEntry.second.nData != sValue.second.nData) || (sEntry.second.nDebug != sValue.second.nDebug))
			{
				::std::cerr << ::std::endl;
				::std::cerr << "data mismatches" << ::std::endl;
				::std::cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << sEntry.first << ::std::endl;
				::std::cerr << "data: " << sEntry.second.nData << ::std::endl;

				::std::cerr << std::setfill (' ') << std::dec << std::setw (0);

				::std::cerr << "debug: " << sEntry.second.nDebug << ::std::endl;
				::std::cerr << "reference" << ::std::endl;

				::std::cerr << "data: " << std::setfill ('0') << std::hex << std::setw (8) << sValue.second.nData << ::std::endl;

				::std::cerr << std::setfill (' ') << std::dec << std::setw (0);

				::std::cerr << "debug: " << sValue.second.nDebug << ::std::endl;
				
				::std::cerr << "creating data.html..." << ::std::endl;

				this->show_integrity ("data.html");

				::std::cerr << "finished!" << ::std::endl;

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
				sEntry = ((value_type) (*sCIter));

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
					::std::cerr << ::std::endl;
					::std::cerr << "number of instances mismatches" << ::std::endl;
					::std::cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << sEntry.first << ::std::endl;
					::std::cerr << "data: " << sEntry.second.nData << ::std::endl;

					::std::cerr << std::setfill (' ') << std::dec << std::setw (0);

					::std::cerr << "debug: " << sEntry.second.nDebug << ::std::endl;
					::std::cerr << "Instance not found in reference!" << ::std::endl;

					::std::cerr << "creating error.html..." << ::std::endl;

					this->show_integrity ("error.html");

					::std::cerr << "finished!" << ::std::endl;

					exit (-1);
				}
			}

			if (sMMap.size () != 0)
			{
				::std::cerr << ::std::endl;
				::std::cerr << "number of instances mismatches" << ::std::endl;
				::std::cerr << "the following entries are still present in reference:" << ::std::endl;

				for (sItMMap = sMMap.cbegin (); sItMMap != sMMap.cend (); sItMMap++)
				{
					sValue = *sItMMap;

					::std::cerr << "key: ";

					::std::cerr << std::setfill ('0') << std::hex << std::setw (8);
					{
						::std::cerr << sValue.first << " ";
						::std::cerr << "data: " << ::std::flush;
						::std::cerr << sValue.second.nData << " ";
					}
					::std::cerr << std::setfill (' ') << std::dec << std::setw (0);

					::std::cerr << "debug: " << sValue.second.nDebug << ::std::endl;
				}

				::std::cerr << "creating error.html..." << ::std::endl;

				this->show_integrity ("error.html");

				::std::cerr << "finished!" << ::std::endl;

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
	
	if ((m_pClRef == NULL) && (!this->empty ()))
	{
		::std::cerr << ::std::endl;
		::std::cerr << "reference not set while data container not empty" << ::std::endl;
		::std::cerr << "size: " << this->size () << ::std::endl;

		exit (-1);
	}
	
	if ((m_pClRef != NULL) && (m_pClRef->size () != this->size ()))
	{
		::std::cerr << ::std::endl;
		::std::cerr << "size mismatches" << ::std::endl;
		::std::cerr << "size: " << this->size () << ::std::endl;
		::std::cerr << "reference size: " << m_pClRef->size () << ::std::endl;

		::std::cerr << "creating size.html..." << ::std::endl;

		this->show_integrity ("size.html");

		::std::cerr << "finished!" << ::std::endl;

		exit (-1);
	}
}

template<class _t_datalayerproperties>
void CBTreeTestMultiMap<_t_datalayerproperties>::set_reference (typename CBTreeTestMultiMap<_t_datalayerproperties>::reference_t *pReference)
{
	m_pClRef = pReference;
}

template<class _t_datalayerproperties>
void CBTreeTestMultiMap<_t_datalayerproperties>::set_atomic_testing (bool bEnable)
{
	m_bAtomicTesting = bEnable;
}

#endif // !BTREETESTMULTIMAP_CPP

