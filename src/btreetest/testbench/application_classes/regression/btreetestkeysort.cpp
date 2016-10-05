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

#include "testbench/application_classes/regression/btreetestkeysort.h"

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::CBTreeKeySortTest
	(
		_t_datalayerproperties &rDataLayerProperties, 
		const bayerTreeCacheDescription_t *psCacheDescription, 
		typename _t_datalayerproperties::sub_node_iter_type nNodeSize, 
		typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::reference_t *pClRefData
	)
	:	CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>
	(
		rDataLayerProperties, 
		psCacheDescription, 
		nNodeSize
	)
	,	m_pClRefData (pClRefData)
	,	m_bAtomicTesting (true)
{
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::CBTreeKeySortTest
	(
		CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties> &rBT, 
		bool bAssign
	)
	:	CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties> (rBT, false)
	,	m_pClRefData (NULL)
	,	m_bAtomicTesting (false)
{
	if (bAssign)
	{
		this->_assign (rBT);
	}
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::~CBTreeKeySortTest ()
{
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
template<class _t_iterator>
void CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	this->insert_via_iterator (sItFirst, sItLast);

	test ();
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::insert
	(
		const typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::value_type &rData
	)
{
	iterator					sIter;

	sIter = CBTreeKeySort_t::insert (rData);

	test ();

	return (sIter);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::erase
	(
		typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::const_iterator sCIterPos
	)
{
	typedef typename reference_t::iterator		itmmap_t;

	key_type		sKey = (*sCIterPos).first;
	itmmap_t		sItMMapLower = m_pClRefData->lower_bound (sKey);
	itmmap_t		sItMMapUpper = m_pClRefData->upper_bound (sKey);
	itmmap_t		sItMMap;
	iterator		sIter;
	
	sIter = CBTreeKeySort_t::erase (sCIterPos);

	test ();

	return (sIter);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename _t_datalayerproperties::size_type CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::erase (const typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::key_type &rKey)
{
	size_type		nRetval;

	nRetval = CBTreeKeySort_t::erase (rKey);

	test ();

	return (nRetval);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::erase
	(
		typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::const_iterator sCIterFirst, 
		typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::const_iterator sCIterLast
	)
{
	typedef typename reference_t::iterator			itmmap_t;

	const_iterator		sCIterRetval;
	const_iterator		sCIter;
	itmmap_t			sItMMapLower;
	itmmap_t			sItMMapUpper;
	itmmap_t			sItMMap;
	iterator			sIter;

	sIter = CBTreeKeySort_t::erase (sCIterFirst, sCIterLast);

	test ();

	return (sIter);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::swap
	(
		CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties> &rKeySort
	)
{
	CBTreeKeySort_t	&rBtrKeySort = dynamic_cast <CBTreeKeySort_t &> (rKeySort);

	m_pClRefData->swap (*(rKeySort.m_pClRefData));

	CBTreeKeySort_t::swap (rBtrKeySort);

	test ();
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::find (const typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::key_type &rKey)
{
	return (CBTreeKeySort_t::find (rKey));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::lower_bound (const typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::key_type &rKey)
{
	return (CBTreeKeySort_t::lower_bound (rKey));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::const_iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::lower_bound (const typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::key_type &rKey) const
{
	return (CBTreeKeySort_t::lower_bound (rKey));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::upper_bound (const typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::key_type &rKey)
{
	return (CBTreeKeySort_t::upper_bound (rKey));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::const_iterator
	CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::upper_bound (const typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::key_type &rKey) const
{
	return (CBTreeKeySort_t::upper_bound (rKey));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::clear ()
{
	CBTreeKeySort_t::clear ();

	test ();
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename _t_datalayerproperties::size_type CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::count (const typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::key_type &rKey) const
{
	return (CBTreeKeySort_t::count (rKey));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::value_type CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::get_data_reference
	(
		typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::const_iterator &rCIter
	)
{
	typedef typename reference_t::const_iterator		mmap_citer_t;

	return (this->template get_data_reference<const_iterator, mmap_citer_t> (rCIter));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::value_type CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::get_data_reference
	(
		typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::const_reverse_iterator &rCRIter
	)
{
	typedef typename reference_t::const_reverse_iterator		mmap_criter_t;

	return (this->template get_data_reference<const_reverse_iterator, mmap_criter_t> (rCRIter));
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties> &CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::operator=
	(const CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties> &rBT)
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

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::test () const
{
	if (!m_bAtomicTesting)
	{
		return;
	}

	typedef typename reference_t::const_iterator			citer_mmap_t;

	reference_t									sMMap;
	key_type									nKey;
	key_type									nNextKey;
	bool										bBounce;
	size_type									nTotalCount = 0;
	value_type									sEntry;
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

	sCIterBegin = CBTreeKeySort_t::cbegin ();
	sCIterEnd = CBTreeKeySort_t::cend ();

	sCIter = sCIterBegin;

	if (this->size () > 0)
	{
		this->extract_key (&nKey, ((value_type) (*sCIter)));
	}

	while (sCIter != sCIterEnd)
	{
		if (m_pClRefData->count (nKey) != this->count (nKey))
		{
			::std::cerr << ::std::endl;
			::std::cerr << "number of instances mismatches" << ::std::endl;
			::std::cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << nKey << ::std::endl;
			::std::cerr << std::setfill (' ') << std::dec << std::setw (0);

			::std::cerr << "count: " << this->count (nKey) << ::std::endl;
			::std::cerr << "reference: " << m_pClRefData->count (nKey) << ::std::endl;
			
			::std::cerr << "creating count.html..." << ::std::endl;

			this->show_integrity ("count.html");

			::std::cerr << "finished!" << ::std::endl;

			exit (-1);
		}

		if (this->count (nKey) == 1)
		{
			sItMMapLower = m_pClRefData->lower_bound (nKey);

			sCIterLower = CBTreeKeySort_t::lower_bound (nKey);

			value_type						sValue ((value_type) *sItMMapLower);

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
			sItMMapLower = m_pClRefData->lower_bound (nKey);
			sItMMapUpper = m_pClRefData->upper_bound (nKey);

			sMMap.insert<citer_mmap_t> (sItMMapLower, sItMMapUpper);

			sCIterLower = CBTreeKeySort_t::lower_bound (nKey);
			sCIterUpper = CBTreeKeySort_t::upper_bound (nKey);

			for (sCIter = sCIterLower; sCIter != sCIterUpper; sCIter++)
			{
				sEntry = ((value_type) (*sCIter));

				bDeleted = false;

				for (sItMMap = sMMap.cbegin (); sItMMap != sMMap.cend (); sItMMap++)
				{
					value_type				sValue ((value_type) *sItMMap);

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
					value_type						sValue ((value_type) *sItMMap);

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

		CBTreeKeySort_t::get_next_key (nKey, nNextKey, bBounce);

		if (bBounce)
		{
			break;
		}

		nKey = nNextKey;
	}

	if ((m_pClRefData == NULL) && (!this->empty ()))
	{
		::std::cerr << ::std::endl;
		::std::cerr << "reference not set while data container not empty" << ::std::endl;
		::std::cerr << "size: " << this->size () << ::std::endl;

		exit (-1);
	}
	
	if ((m_pClRefData != NULL) && (m_pClRefData->size () != this->size ()))
	{
		::std::cerr << ::std::endl;
		::std::cerr << "size mismatches" << ::std::endl;
		::std::cerr << "size: " << this->size () << ::std::endl;
		::std::cerr << "reference size: " << m_pClRefData->size () << ::std::endl;

		::std::cerr << "creating size.html..." << ::std::endl;

		this->show_integrity ("size.html");

		::std::cerr << "finished!" << ::std::endl;

		exit (-1);
	}
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::set_reference (typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::reference_t *pReference)
{
	m_pClRefData = pReference;
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
void CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::set_atomic_testing (bool bEnable)
{
	m_bAtomicTesting = bEnable;
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
int CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::comp (const typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::key_type &rKey0, const typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::key_type &rKey1) const
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

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::key_type* CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::extract_key (typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::key_type *pKey, const typename _t_datalayerproperties::node_iter_type nNode, const typename _t_datalayerproperties::sub_node_iter_type nEntry) const
{
	pKey = CBTreeKeySort_t::extract_key (pKey, nNode, nEntry);

	return (pKey);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::key_type* CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::extract_key (typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::key_type *pKey, const typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::value_type &rData) const
{
	*pKey = rData.first;

	return (pKey);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
template<class _t_iterator, class _t_ref_iterator>
typename CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::value_type CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::get_data_reference (_t_iterator &rIter)
{
	_t_iterator			sIterBegin;
	_t_ref_iterator		sMMapCIter;

	get_begin (this, sIterBegin);
	
	get_begin (this->m_pClRefData, sMMapCIter);

	size_type			nOffset = ::std::distance (sIterBegin, rIter);

	::std::advance (sMMapCIter, nOffset);

	return ((value_type) *sMMapCIter);
}

template<class _t_data, class _t_key, class _t_datalayerproperties>
bool CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>::show_data (std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const typename _t_datalayerproperties::node_iter_type nNode, const typename _t_datalayerproperties::sub_node_iter_type nSubPos) const
{
	typedef typename reference_t::const_iterator		mmap_citer_t;

	value_type				*psData;
	key_type				rData;

	try
	{
		psData = this->get_data (nNode, nSubPos);

		rData = psData->first;
		rData = (rData >> 16) | (rData << 16);
		rData = ((rData >> 8) & 0xFF00FF) | ((rData << 8) & 0xFF00FF00);
		
		rstrData.clear ();
		rstrData << "<table border=\"0\" cellspacing=\"1\" width=\"220\"><tr><td>";
		rstrData << "key: " << ::std::hex << ::std::setfill ('0') << ::std::setw (8) << rData << "<br>";
		rstrData << "data: " << psData->second.nData << ::std::dec << "<br>";
		rstrData << "debug: " << psData->second.nDebug << "<br>";
		rstrData << "instance: " << this->get_instancePos (nNode, nSubPos);
		rstrData << "</td>";

		size_type		nDiff = this->lower_bound (psData->first) - this->cbegin ();
		size_type		nOffset = nDiff + this->get_instancePos (nNode, nSubPos);

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
			
			rstrData << "key: " << ::std::hex << ::std::setfill ('0') << ::std::setw (8) << rData << "<br>";
			rstrData << "</font>";

			if (psData->second.nData != (*sItMMap).second.nData)
			{
				rstrData << "<font color=\"#AAAA00\">";
			}
			else
			{
				rstrData << "<font color=\"#00BB00\">";
			}
			
			rstrData << "data: " << (*sItMMap).second.nData << ::std::dec << "<br>";
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
		rstrData << "</table>" << ::std::endl;
	}
	catch (::std::exception *pE)
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
		rstrData << "key: " << ::std::hex << ::std::setfill ('0') << ::std::setw (8) << rData << ::std::dec << "<br>debug: " << psData->second.nDebug << "<br>instance: ---";
	}

	return (true);
}

/******************************************************************/

template<class _t_datalayerproperties>
CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::CBTreeKeySortTest
	(
		_t_datalayerproperties &rDataLayerProperties, 
		const bayerTreeCacheDescription_t *psCacheDescription, 
		typename _t_datalayerproperties::sub_node_iter_type nNodeSize, 
		typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::reference_t *pClRefData
	)
	:	CBTreeKeySort<keySortEntry_t, uint32_t, _t_datalayerproperties>
	(
		rDataLayerProperties, 
		psCacheDescription, 
		nNodeSize
	)
	,	m_pClRefData (pClRefData)
	,	m_bAtomicTesting (true)
{
}

template<class _t_datalayerproperties>
CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::CBTreeKeySortTest
	(
		CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties> &rBT, 
		bool bAssign
	)
	:	CBTreeKeySort<keySortEntry_t, uint32_t, _t_datalayerproperties>
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

template<class _t_datalayerproperties>
CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::~CBTreeKeySortTest ()
{
}

template<class _t_datalayerproperties>
template<class _t_iterator>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	this->insert_via_iterator (sItFirst, sItLast);

	test ();
}

template<class _t_datalayerproperties>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::insert
	(
		const typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::value_type &rData
	)
{
	iterator								sIter;

	sIter = CBTreeKeySort_t::insert (rData);

	test ();

	return (sIter);
}

template<class _t_datalayerproperties>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::erase
	(
		typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::const_iterator sCIterPos
	)
{
	iterator	sIter = CBTreeKeySort_t::erase (sCIterPos);

	test ();

	return (sIter);
}

template<class _t_datalayerproperties>
typename _t_datalayerproperties::size_type CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::erase (const typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::key_type &rKey)
{
	size_type		nRetval;

	nRetval = CBTreeKeySort_t::erase (rKey);

	test ();

	return (nRetval);
}

template<class _t_datalayerproperties>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::erase
	(
		typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::const_iterator sCIterFirst, 
		typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::const_iterator sCIterLast
	)
{
	iterator			sIter;

	sIter = CBTreeKeySort_t::erase (sCIterFirst, sCIterLast);

	return (sIter);
}

template<class _t_datalayerproperties>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::swap
	(
		CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties> &rKeySort
	)
{
	CBTreeKeySort_t	&rBtrKeySort = dynamic_cast <CBTreeKeySort_t &> (rKeySort);

	CBTreeKeySort_t::swap (rBtrKeySort);

	test ();
}

template<class _t_datalayerproperties>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::find (const typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::key_type &rKey)
{
	return (CBTreeKeySort_t::find (rKey));
}

template<class _t_datalayerproperties>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::lower_bound (const typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::key_type &rKey)
{
	return (CBTreeKeySort_t::lower_bound (rKey));
}

template<class _t_datalayerproperties>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::const_iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::lower_bound (const typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::key_type &rKey) const
{
	return (CBTreeKeySort_t::lower_bound (rKey));
}

template<class _t_datalayerproperties>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::upper_bound (const typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::key_type &rKey)
{
	return (CBTreeKeySort_t::upper_bound (rKey));
}

template<class _t_datalayerproperties>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::const_iterator
	CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::upper_bound (const typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::key_type &rKey) const
{
	return (CBTreeKeySort_t::upper_bound (rKey));
}

template<class _t_datalayerproperties>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::clear ()
{
	CBTreeKeySort_t::clear ();
}

template<class _t_datalayerproperties>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::value_type CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::get_data_reference
	(
		typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::const_iterator &rCIter
	)
{
	typedef typename reference_t::const_iterator		mmap_citer_t;

	return (this->get_data_reference (rCIter));
}

template<class _t_datalayerproperties>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::value_type CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::get_data_reference
	(
		typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::const_reverse_iterator &rCRIter
	)
{
	typedef typename reference_t::const_reverse_iterator		mmap_criter_t;

	return (this->get_data_reference (rCRIter));
}

template<class _t_datalayerproperties>
CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties> &CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::operator=
	(const CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties> &rBT)
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

//template<class _t_datalayerproperties>
//bool CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::operator== (const CBTreeKeySortTest &rKeySort) const
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
//	size_type				i;
//	size_type				j;
//	size_type				k;
//	size_type				nKeySetSize;
//	value_type				sThisData;
//	value_type				sData;
//	value_type				sKey;
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
//template<class _t_datalayerproperties>
//bool CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::operator!= (const CBTreeKeySortTest &rKeySort) const
//{
//	return ( ! (*this == rKeySort));
//}
//
template<class _t_datalayerproperties>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::test () const
{
	if (!m_bAtomicTesting)
	{
		return;
	}

	typedef typename reference_t::value_type		value_ref_t;
	typedef typename reference_t::const_iterator	citer_ref_t;

	reference_t										sMMap;
	key_type										nKey;
	key_type										nNextKey;
	bool											bBounce;
	size_type										nTotalCount = 0;
	value_type										sEntry;
	citer_ref_t										sItMMapLower;
	citer_ref_t										sItMMapUpper;
	citer_ref_t										sItMMap;
	const_iterator									sCIterBegin;
	const_iterator									sCIterEnd;
	const_iterator									sCIterLower;
	const_iterator									sCIterUpper;
	const_iterator									sCIter;
	bool											bDeleted;
	
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
		nKey = ((value_type) (*sCIter)).nKey;
	}

	while (sCIter != sCIterEnd)
	{
		if (m_pClRefData->count (nKey) != this->count (nKey))
		{
			::std::cerr << ::std::endl;
			::std::cerr << "number of instances mismatches" << ::std::endl;
			::std::cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << nKey << ::std::endl;
			::std::cerr << std::setfill (' ') << std::dec << std::setw (0);

			::std::cerr << "count: " << this->count (nKey) << ::std::endl;
			::std::cerr << "reference: " << m_pClRefData->count (nKey) << ::std::endl;
			
			::std::cerr << "creating count.html..." << ::std::endl;

			this->show_integrity ("count.html");

			::std::cerr << "finished!" << ::std::endl;

			exit (-1);
		}

		if (this->count (nKey) == 1)
		{
			sItMMapLower = m_pClRefData->lower_bound (nKey);

			sCIterLower = CBTreeKeySort_t::lower_bound (nKey);

			value_ref_t					sValue ((value_ref_t) *sItMMapLower);

			sEntry = ((value_type) (*sCIterLower));

			if ((sEntry.nData != sValue.second.nData) || (sEntry.nDebug != sValue.second.nDebug))
			{
				::std::cerr << ::std::endl;
				::std::cerr << "data mismatches" << ::std::endl;
				::std::cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << sEntry.nKey << ::std::endl;
				::std::cerr << "data: " << sEntry.nData << ::std::endl;

				::std::cerr << std::setfill (' ') << std::dec << std::setw (0);

				::std::cerr << "debug: " << sEntry.nDebug << ::std::endl;
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
			sItMMapLower = m_pClRefData->lower_bound (nKey);
			sItMMapUpper = m_pClRefData->upper_bound (nKey);

			sMMap.insert (sItMMapLower, sItMMapUpper);

			sCIterLower = CBTreeKeySort_t::lower_bound (nKey);
			sCIterUpper = CBTreeKeySort_t::upper_bound (nKey);

			for (sCIter = sCIterLower; sCIter != sCIterUpper; sCIter++)
			{
				sEntry = ((value_type) (*sCIter));

				bDeleted = false;

				for (sItMMap = sMMap.cbegin (); sItMMap != sMMap.cend (); sItMMap++)
				{
					value_ref_t					sValue ((value_ref_t) *sItMMap);

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
					::std::cerr << ::std::endl;
					::std::cerr << "number of instances mismatches" << ::std::endl;
					::std::cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << sEntry.nKey << ::std::endl;
					::std::cerr << "data: " << sEntry.nData << ::std::endl;

					::std::cerr << std::setfill (' ') << std::dec << std::setw (0);

					::std::cerr << "debug: " << sEntry.nDebug << ::std::endl;
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
					value_ref_t					sValue ((value_ref_t) *sItMMap);

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

		CBTreeKeySort_t::get_next_key (nKey, nNextKey, bBounce);

		if (bBounce)
		{
			break;
		}

		nKey = nNextKey;
	}
	
	if (m_pClRefData->size () != this->size ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "size mismatches" << ::std::endl;
		::std::cerr << "size: " << this->size () << ::std::endl;
		::std::cerr << "reference size: " << m_pClRefData->size () << ::std::endl;
		::std::cerr << "empty: " << this->empty () << ::std::endl;
		::std::cerr << "reference empty: " << m_pClRefData->empty () << ::std::endl;

		::std::cerr << "creating size.html..." << ::std::endl;

		this->show_integrity ("size.html");

		::std::cerr << "finished!" << ::std::endl;

		exit (-1);
	}
}

template<class _t_datalayerproperties>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::set_reference (typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::reference_t *pReference)
{
	m_pClRefData = pReference;
}

template<class _t_datalayerproperties>
void CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::set_atomic_testing (bool bEnable)
{
	m_bAtomicTesting = bEnable;
}

template<class _t_datalayerproperties>
int CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::comp (const typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::key_type &rKey0, const typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::key_type &rKey1) const
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

template<class _t_datalayerproperties>
template<class _t_iterator, class _t_ref_iterator>
typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::value_type CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::get_data_reference (_t_iterator &rIter)
{
	typedef typename reference_t::value_type	value_ref_t;

	value_type			sRetval;
	_t_iterator			sIterBegin;
	_t_ref_iterator		sMMapCIter;

	get_begin (this, sIterBegin);
	
	get_begin (this->m_pClRefData, sMMapCIter);

	size_type			nOffset = ::std::distance (sIterBegin, rIter);

	::std::advance (sMMapCIter, nOffset);

	sRetval.nKey = ((value_ref_t) *sMMapCIter).first;
	sRetval.nData = ((value_ref_t) *sMMapCIter).second.nData;
	sRetval.nDebug = ((value_ref_t) *sMMapCIter).second.nDebug;

	return (sRetval);
}

template<class _t_datalayerproperties>
bool CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::show_data (std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::node_iter_type nNode, const typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>::sub_node_iter_type nSubPos) const
{
	value_type		*psData;
	key_type		rData;

	try
	{
		psData = this->get_data (nNode, nSubPos);

		rData = psData->nKey;
		rData = (rData >> 16) | (rData << 16);
		rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);
		
		rstrData.clear ();
		rstrData << "<table border=\"0\" cellspacing=\"1\" width=\"220\"><tr><td>";
		rstrData << "key: " << ::std::hex << ::std::setfill ('0') << ::std::setw (8) << rData << "<br>";
		rstrData << "data: " << psData->nData << ::std::dec << "<br>";
		rstrData << "debug: " << psData->nDebug << "<br>";
		rstrData << "instance: " << this->get_instancePos (nNode, nSubPos);
		rstrData << "</td>";

		size_type		nDiff = ::std::distance (this->cbegin (), this->lower_bound (psData->nKey));
		size_type		nOffset = nDiff + this->get_instancePos (nNode, nSubPos);

		rstrData << "<td align=\"top\">";

		if (nOffset < m_pClRefData->size ())
		{
			reference_t::const_iterator		sItMMap;

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
			
			rstrData << "key: " << ::std::hex << ::std::setfill ('0') << ::std::setw (8) << rData << "<br>";
			rstrData << "</font>";

			if (psData->nData != (*sItMMap).second.nData)
			{
				rstrData << "<font color=\"#AAAA00\">";
			}
			else
			{
				rstrData << "<font color=\"#00BB00\">";
			}
			
			rstrData << "data: " << (*sItMMap).second.nData << ::std::dec << "<br>";
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
		rstrData << "</table>" << ::std::endl;
	}
	catch (::std::exception *pE)
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
		rstrData << "key: " << ::std::hex << ::std::setfill ('0') << ::std::setw (8) << rData << ::std::dec << "<br>debug: " << psData->nDebug << "<br>instance: ---";
	}

	return (true);
}

#endif // BTREETESTKEYSORT_CPP

