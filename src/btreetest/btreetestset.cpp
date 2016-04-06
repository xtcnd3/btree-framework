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

#include "btreetestset.h"

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeTestSet
	(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize)
	:	CBTreeSet<uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		rDataLayerProperties, 
		psCacheDescription, 
		nNodeSize
	)
	,	m_pClRef (NULL)
{
	m_pClRef = new ::std::set<uint32_t> ();

	BTREE_ASSERT (m_pClRef != NULL, "CBTreeTestSet<>::CBTreeTestSet (_t_datalayerproperties &, bayerTreeCacheDescription_t *, _t_subnodeiter): insufficient memory!");
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeTestSet
	(const CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign)
	:	CBTreeSet<uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		dynamic_cast<const CBTreeSet<uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &> (rBT), 
		bAssign
	)
	,	m_pClRef (NULL)
{
	m_pClRef = new ::std::set<uint32_t> ();

	BTREE_ASSERT (m_pClRef != NULL, "CBTreeTestSet<>::CBTreeTestSet (CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &, bool): insufficient memory!");
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeTestSet ()
{
	delete m_pClRef;
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>&
	CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator=
	(
		const CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT
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

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
template <class _t_iterator>
void CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	m_pClRef->template insert<_t_iterator> (sItFirst, sItLast);

	CBTreeSet_t::template insert<_t_iterator> (sItFirst, sItLast);

	test ();
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert
	(
		const value_t &rData
	)
{
	::std::pair<::std::set<uint32_t>::iterator, bool>		sRefRslt;
	iterator												sIter;
	value_t													sData = rData;
	bool													bSuccess;

	sRefRslt = m_pClRef->insert (sData);

	sIter = CBTreeSet_t::insert (sData);

	bSuccess = sIter < this->end ();

	if (bSuccess != sRefRslt.second)
	{
		cerr << endl;
		cerr << "CBTreeTestSet<>::insert: ERROR: reference claims that key ";
		cerr << std::setfill ('0') << std::hex << std::setw (8);
		{
			cerr << sData;
		}
		cerr << std::setfill (' ') << std::dec << std::setw (0);
		cerr << " was ";

		if (sRefRslt.second)
		{
			cerr << "not ";
		}

		cerr << "present, whereas the instance claims that said key was ";

		if (bSuccess)
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

	return (sIter);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterPos
	)
{
	typedef typename ::std::set<uint32_t>::iterator		iter_set_t;

	_t_sizetype		nPos = sCIterPos.get_pos ();
	iter_set_t		sItMM;
	iterator		sRslt;

	sItMM = m_pClRef->begin ();

	::std::advance<iter_set_t, _t_sizetype> (sItMM, nPos);

	m_pClRef->erase (sItMM);

	sRslt = CBTreeSet_t::erase (sCIterPos);

	test ();

	return (sRslt);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (const uint32_t &rKey)
{
	_t_sizetype		nRslt;

	m_pClRef->erase (rKey);

	nRslt = CBTreeSet_t::erase (rKey);

	test ();

	return (nRslt);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterFirst, 
		typename CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterLast
	)
{
	typedef typename ::std::set<uint32_t>::iterator		iter_set_t;

	_t_sizetype		nPos = sCIterFirst.get_pos ();
	iter_set_t		sItSetFirst;
	iter_set_t		sItSetLast;

	sItSetLast = sItSetFirst = m_pClRef->begin ();

	::std::advance<iter_set_t, _t_sizetype> (sItSetFirst, nPos);

	nPos = sCIterLast.get_pos ();

	::std::advance<iter_set_t, _t_sizetype> (sItSetLast, nPos);

	m_pClRef->erase (sItSetFirst, sItSetLast);

	return (CBTreeSet_t::erase (sCIterFirst, sCIterLast));
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap (CBTreeTestSet_t &rTSet)
{
	if (this != &rTSet)
	{
		CBTreeSet_t	&rThisMM = dynamic_cast<CBTreeSet_t &> (*this);
		CBTreeSet_t	&rMM = dynamic_cast<CBTreeSet_t &> (rTSet);

		rThisMM.swap (rMM);

		m_pClRef->swap (*(rTSet.m_pClRef));
	}
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ()
{
	m_pClRef->clear ();

	CBTreeSet_t::clear ();

	test ();
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::key_compare
	CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::key_comp () const
{
	key_compare		sRslt;

	sRslt.pThis = (CBTreeAssociativeIf_t *) (this);

	return (sRslt);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::value_compare
	CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::value_comp () const
{
	value_compare	sRslt;

	sRslt.pThis = (CBTreeAssociativeIf_t *) (this);

	return (sRslt);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator== (const CBTreeTestSet_t &rTSet) const
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
	value_t				sThisVal;
	value_t				sTSetVal;

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

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator!= (const CBTreeTestSet_t &rTSet) const
{
	return (! ((*this) == rTSet));
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::test () const
{
	typedef ::std::set<uint32_t>::const_iterator		citer_set_t;

	::std::set<uint32_t>						sSet;
	uint32_t									nKey;
	uint32_t									nNextKey;
	bool										bBounce;
	_t_sizetype									nTotalCount = 0;
	value_t										sEntry;
	value_t										sValue;
	citer_set_t									sItSetLower;
	citer_set_t									sItSetUpper;
	citer_set_t									sItSet;
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
			sItSetLower = m_pClRef->lower_bound (nKey);

			sCIterLower = this->lower_bound (nKey);

			sValue = *sItSetLower;

			sEntry = ((data_t) (*sCIterLower));

			if (sEntry != sValue)
			{
				cerr << endl;
				cerr << "data mismatches" << endl;
				cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << sEntry << endl;
				
				cerr << std::setfill (' ') << std::dec << std::setw (0);

				cerr << "reference" << endl;

				cerr << "data: " << std::setfill ('0') << std::hex << std::setw (8) << sValue << endl;

				cerr << std::setfill (' ') << std::dec << std::setw (0);

				cerr << "creating data.html..." << endl;

				this->show_integrity ("data.html");

				cerr << "finished!" << endl;

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
				sEntry = ((data_t) (*sCIter));

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
					cerr << endl;
					cerr << "number of instances mismatches" << endl;
					cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << sEntry << endl;
					
					cerr << std::setfill (' ') << std::dec << std::setw (0);

					cerr << "Instance not found in reference!" << endl;

					cerr << "creating error.html..." << endl;

					this->show_integrity ("error.html");

					cerr << "finished!" << endl;

					exit (-1);
				}
			}

			if (sSet.size () != 0)
			{
				cerr << endl;
				cerr << "number of instances mismatches" << endl;
				cerr << "the following entries are still present in reference:" << endl;

				for (sItSet = sSet.cbegin (); sItSet != sSet.cend (); sItSet++)
				{
					sValue = *sItSet;

					cerr << "key: ";

					cerr << std::setfill ('0') << std::hex << std::setw (8);
					{
						cerr << sValue << " ";
					}
					cerr << std::setfill (' ') << std::dec << std::setw (0);
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

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::show_data (std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const
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
		rstrData << "key: " << hex << setfill ('0') << setw (8) << rData << "<br>";

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

		_t_sizetype		nDiff = this->lower_bound (*psData) - this->begin ();
		_t_sizetype		nOffset = nDiff + this->get_instancePos (nNode, nSubPos);

		rstrData << "<td align=\"top\">";

		if (nOffset < m_pClRef->size ())
		{
			::std::set<uint32_t>::const_iterator		sItSet;

			sItSet = m_pClRef->cbegin ();

			::std::advance< ::std::set<uint32_t>::const_iterator, _t_sizetype> (sItSet, nDiff);

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
			
			rstrData << "key: " << hex << setfill ('0') << setw (8) << rData << "<br>";
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

		rData = *psData;
		rData = (rData >> 16) | (rData << 16);
		rData = ((rData >> 8) & 0xFF00FF)| ((rData << 8) & 0xFF00FF00);
		
		rstrData.clear ();
		rstrData << "key: " << hex << setfill ('0') << setw (8) << rData << "<br>instance: ---";
	}

	return (true);
}

#endif // !BTREETESTSET_CPP
