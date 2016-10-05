/************************************************************
**
** file:	btreetestset.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains set application class code derived
** from the b-tree framework's set class to allow
** for the set class being tested by having
** the set application class act as a wrapper.
**
************************************************************/

#ifndef BTREETESTSET_CPP
#define	BTREETESTSET_CPP

#include "testbench/application_classes/regression/btreetestset.h"

template<class _t_datalayerproperties>
CBTreeTestSet<_t_datalayerproperties>::CBTreeTestSet
	(
		_t_datalayerproperties &rDataLayerProperties, 
		const bayerTreeCacheDescription_t *psCacheDescription, 
		typename _t_datalayerproperties::sub_node_iter_type nNodeSize, 
		typename CBTreeTestSet<_t_datalayerproperties>::reference_t *pClRefData
	)
	:	CBTreeSet<uint32_t, _t_datalayerproperties>
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
CBTreeTestSet<_t_datalayerproperties>::CBTreeTestSet
	(const CBTreeTestSet<_t_datalayerproperties> &rBT, bool bAssign)
	:	CBTreeSet<uint32_t, _t_datalayerproperties>
	(
		dynamic_cast<const CBTreeSet<uint32_t, _t_datalayerproperties> &> (rBT), 
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
CBTreeTestSet<_t_datalayerproperties>::~CBTreeTestSet ()
{
}

template<class _t_datalayerproperties>
CBTreeTestSet<_t_datalayerproperties>&
	CBTreeTestSet<_t_datalayerproperties>::operator=
	(
		const CBTreeTestSet<_t_datalayerproperties> &rBT
	)
{
	if (this != &rBT)
	{
		CBTreeSet_t			&rThisMM = dynamic_cast<CBTreeSet_t &> (*this);
		const CBTreeSet_t	&rBTMM = dynamic_cast<const CBTreeSet_t &> (rBT);

		rThisMM = rBTMM;

		(*m_pClRef) = *(rBT.m_pClRef);

		test ();
	}

	return (*this);
}

template<class _t_datalayerproperties>
template<class _t_iterator>
void CBTreeTestSet<_t_datalayerproperties>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	CBTreeSet_t::insert (sItFirst, sItLast);

	test ();
}

template<class _t_datalayerproperties>
typename CBTreeTestSet<_t_datalayerproperties>::iterator
	CBTreeTestSet<_t_datalayerproperties>::insert
	(
		const typename CBTreeTestSet<_t_datalayerproperties>::value_type &rData
	)
{
	iterator		sIter;
	value_type		sData = rData;
	
	sIter = CBTreeSet_t::insert (sData);

	if (m_pClRef != NULL)
	{
		if (CBTreeSet_t::count (sData) != m_pClRef->count (sData))
		{
			::std::cerr << ::std::endl;
			::std::cerr << "CBTreeTestSet<>::insert: ERROR: multi key size mismatch!" << ::std::endl;
			::std::cerr << "key: ";
			::std::cerr << std::setfill ('0') << std::hex << std::setw (8);
			{
				::std::cerr << sData;
			}
			::std::cerr << std::setfill (' ') << std::dec << std::setw (0);
			::std::cerr << "reference: " << m_pClRef->count (sData) << ::std::endl;
			::std::cerr << "container: " << CBTreeSet_t::count (sData) << ::std::endl;
			
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
typename CBTreeTestSet<_t_datalayerproperties>::iterator
	CBTreeTestSet<_t_datalayerproperties>::erase
	(
		typename CBTreeTestSet<_t_datalayerproperties>::const_iterator sCIterPos
	)
{
	iterator		sRslt;

	sRslt = CBTreeSet_t::erase (sCIterPos);

	test ();

	return (sRslt);
}

template<class _t_datalayerproperties>
typename _t_datalayerproperties::size_type CBTreeTestSet<_t_datalayerproperties>::erase (const uint32_t &rKey)
{
	size_type		nRslt;

	nRslt = CBTreeSet_t::erase (rKey);

	test ();

	return (nRslt);
}

template<class _t_datalayerproperties>
typename CBTreeTestSet<_t_datalayerproperties>::iterator
	CBTreeTestSet<_t_datalayerproperties>::erase
	(
		typename CBTreeTestSet<_t_datalayerproperties>::const_iterator sCIterFirst, 
		typename CBTreeTestSet<_t_datalayerproperties>::const_iterator sCIterLast
	)
{
	iterator	nRslt = CBTreeSet_t::erase (sCIterFirst, sCIterLast);

	test ();

	return (nRslt);
}

template<class _t_datalayerproperties>
void CBTreeTestSet<_t_datalayerproperties>::swap (CBTreeTestSet_t &rTSet)
{
	if (this != &rTSet)
	{
		CBTreeSet_t	&rThisMM = dynamic_cast<CBTreeSet_t &> (*this);
		CBTreeSet_t	&rMM = dynamic_cast<CBTreeSet_t &> (rTSet);

		rThisMM.swap (rMM);
	}
}

template<class _t_datalayerproperties>
void CBTreeTestSet<_t_datalayerproperties>::clear ()
{
	CBTreeSet_t::clear ();

	test ();
}

//template<class _t_datalayerproperties>
//typename CBTreeTestSet<_t_datalayerproperties>::key_compare
//	CBTreeTestSet<_t_datalayerproperties>::key_comp () const
//{
//	key_compare		sRslt;
//
//	sRslt.pThis = (CBTreeAssociativeIf_t *) (this);
//
//	return (sRslt);
//}
//
//template<class _t_datalayerproperties>
//typename CBTreeTestSet<_t_datalayerproperties>::value_compare
//	CBTreeTestSet<_t_datalayerproperties>::value_comp () const
//{
//	value_compare	sRslt;
//
//	sRslt.pThis = (CBTreeAssociativeIf_t *) (this);
//
//	return (sRslt);
//}

template<class _t_datalayerproperties>
bool CBTreeTestSet<_t_datalayerproperties>::operator== (const CBTreeTestSet_t &rTSet) const
{
	if (this == &rTSet)
	{
		return (true);
	}

	if (this->size () != rTSet.size ())
	{
		return (false);
	}

	const_iterator		sCIterThis;
	const_iterator		sCIterThisBegin;
	const_iterator		sCIterThisEnd;
	const_iterator		sCIterRTSet;
	const_iterator		sCIterRTSetbegin;
	value_type			sThisVal;
	value_type			sTSetVal;

	sCIterThisBegin = this->cbegin ();
	sCIterThisEnd = this->cend ();

	sCIterRTSetbegin = rTSet.cbegin ();

	for (sCIterThis = sCIterThisBegin, sCIterRTSet = sCIterRTSetbegin; sCIterThis != sCIterThisEnd; sCIterThis++, sCIterRTSet++)
	{
		sThisVal = *sCIterThis;
		sTSetVal = *sCIterRTSet;

		if (sThisVal != sTSetVal)
		{
			return (false);
		}
	}

	return (true);
}

template<class _t_datalayerproperties>
bool CBTreeTestSet<_t_datalayerproperties>::operator!= (const CBTreeTestSet_t &rTSet) const
{
	return (! ((*this) == rTSet));
}

template<class _t_datalayerproperties>
void CBTreeTestSet<_t_datalayerproperties>::test () const
{
	if (!m_bAtomicTesting)
	{
		return;
	}

	typedef typename reference_t::const_iterator	citer_set_t;

	reference_t										sSet;
	key_type										nKey;
	key_type										nNextKey;
	bool											bBounce;
	size_type										nTotalCount = 0;
	value_type										sEntry;
	value_type										sValue;
	citer_set_t										sItSetLower;
	citer_set_t										sItSetUpper;
	citer_set_t										sItSet;
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
			sItSetLower = m_pClRef->lower_bound (nKey);

			sCIterLower = this->lower_bound (nKey);

			sValue = *sItSetLower;

			sEntry = ((value_type) (*sCIterLower));

			if (sEntry != sValue)
			{
				::std::cerr << ::std::endl;
				::std::cerr << "data mismatches" << ::std::endl;
				::std::cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << sEntry << ::std::endl;
				
				::std::cerr << std::setfill (' ') << std::dec << std::setw (0);

				::std::cerr << "reference" << ::std::endl;

				::std::cerr << "data: " << std::setfill ('0') << std::hex << std::setw (8) << sValue << ::std::endl;

				::std::cerr << std::setfill (' ') << std::dec << std::setw (0);

				::std::cerr << "creating data.html..." << ::std::endl;

				this->show_integrity ("data.html");

				::std::cerr << "finished!" << ::std::endl;

				exit (-1);
			}
		}
		else
		{
			sItSetLower = m_pClRef->lower_bound (nKey);
			sItSetUpper = m_pClRef->upper_bound (nKey);

			sSet.insert<citer_set_t> (sItSetLower, sItSetUpper);

			sCIterLower = this->lower_bound (nKey);
			sCIterUpper = this->upper_bound (nKey);

			for (sCIter = sCIterLower; sCIter != sCIterUpper; sCIter++)
			{
				sEntry = ((value_type) (*sCIter));

				bDeleted = false;

				for (sItSet = sSet.cbegin (); sItSet != sSet.cend (); sItSet++)
				{
					sValue = *sItSet;

					if (sEntry == sValue)
					{
						sSet.erase (sItSet);

						bDeleted = true;

						break;
					}
				}

				if (!bDeleted)
				{
					::std::cerr << ::std::endl;
					::std::cerr << "number of instances mismatches" << ::std::endl;
					::std::cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << sEntry << ::std::endl;
					
					::std::cerr << std::setfill (' ') << std::dec << std::setw (0);

					::std::cerr << "Instance not found in reference!" << ::std::endl;

					::std::cerr << "creating error.html..." << ::std::endl;

					this->show_integrity ("error.html");

					::std::cerr << "finished!" << ::std::endl;

					exit (-1);
				}
			}

			if (sSet.size () != 0)
			{
				::std::cerr << ::std::endl;
				::std::cerr << "number of instances mismatches" << ::std::endl;
				::std::cerr << "the following entries are still present in reference:" << ::std::endl;

				for (sItSet = sSet.cbegin (); sItSet != sSet.cend (); sItSet++)
				{
					sValue = *sItSet;

					::std::cerr << "key: ";

					::std::cerr << std::setfill ('0') << std::hex << std::setw (8);
					{
						::std::cerr << sValue << " ";
					}
					::std::cerr << std::setfill (' ') << std::dec << std::setw (0);
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
void CBTreeTestSet<_t_datalayerproperties>::set_reference (typename CBTreeTestSet<_t_datalayerproperties>::reference_t *pReference)
{
	m_pClRef = pReference;
}

template<class _t_datalayerproperties>
void CBTreeTestSet<_t_datalayerproperties>::set_atomic_testing (bool bEnable)
{
	m_bAtomicTesting = bEnable;
}

template<class _t_datalayerproperties>
bool CBTreeTestSet<_t_datalayerproperties>::show_data (std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const typename _t_datalayerproperties::node_iter_type nNode, const typename _t_datalayerproperties::sub_node_iter_type nSubPos) const
{
	uint32_t				*psData;
	uint32_t				rData;

	try
	{
		psData = this->get_data (nNode, nSubPos);

		rData = *psData;
		rData = (rData >> 16) | (rData << 16);
		rData = ((rData >> 8) & 0xFF00FF) | ((rData << 8) & 0xFF00FF00);
		
		rstrData.clear ();
		rstrData << "<table border=\"0\" cellspacing=\"1\" width=\"220\"><tr><td>";
		rstrData << "key: " << ::std::hex << ::std::setfill ('0') << ::std::setw (8) << rData << "<br>";

		if (this->get_instancePos (nNode, nSubPos) > 0)
		{
			rstrData << "<font color=\"#FF0000\">";
		}
		else
		{
			rstrData << "<font color=\"#000000\">";
		}

		rstrData << "instance: " << this->get_instancePos (nNode, nSubPos);
		rstrData << "</font>";
		rstrData << "</td>";

		size_type		nDiff = this->lower_bound (*psData) - this->begin ();
		size_type		nOffset = nDiff + this->get_instancePos (nNode, nSubPos);

		rstrData << "<td align=\"top\">";

		if (nOffset < m_pClRef->size ())
		{
			reference_t::const_iterator		sItSet;

			sItSet = m_pClRef->cbegin ();

			::std::advance (sItSet, nDiff);

			rData = *sItSet;
			rData = (rData >> 16) | (rData << 16);
			rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);

			if (*psData != *sItSet)
			{
				rstrData << "<font color=\"#FF0000\">";
			}
			else
			{
				rstrData << "<font color=\"#00BB00\">";
			}
			
			rstrData << "key: " << ::std::hex << ::std::setfill ('0') << ::std::setw (8) << rData << "<br>";
			rstrData << "</font>";

			if (this->get_instancePos (nNode, nSubPos) > 0)
			{
				rstrData << "<font color=\"#FF0000\">";
			}
			else
			{
				rstrData << "<font color=\"#00BB00\">";
			}

			rstrData << "instance: 0";
			rstrData << "</font>";
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

		rData = *psData;
		rData = (rData >> 16) | (rData << 16);
		rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);
		
		rstrData.clear ();
		rstrData << "key: " << ::std::hex << ::std::setfill ('0') << ::std::setw (8) << rData << "<br>instance: ---";
	}

	return (true);
}

#endif // !BTREETESTSET_CPP

