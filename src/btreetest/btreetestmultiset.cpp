/************************************************************
**
** file:	btreetestmultiset.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains multi set application class code derived
** from the b-tree framework's multi set class to allow
** for the multi set class being tested by having
** the multi set application class act as a wrapper.
**
************************************************************/

#ifndef BTREETESTMULTISET_CPP
#define	BTREETESTMULTISET_CPP

#include "btreetestmultiset.h"

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeTestMultiSet
	(_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize)
	:	CBTreeMultiSet<uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		rDataLayerProperties, 
		psCacheDescription, 
		nNodeSize
	)
	,	m_nDebug (0)
	,	m_pClRef (NULL)
{
	m_pClRef = new ::std::multiset<uint32_t> ();

	BTREE_ASSERT (m_pClRef != NULL, "CBTreeTestMultiSet<>::CBTreeTestMultiSet: ERROR: insufficient memory!");
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeTestMultiSet
	(CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign)
	:	CBTreeMultiSet<uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		dynamic_cast<CBTreeMultiSet<uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &> (rBT), 
		bAssign
	)
	,	m_nDebug (0)
	,	m_pClRef (NULL)
{
	m_pClRef = new ::std::multiset<uint32_t> ();

	BTREE_ASSERT (m_pClRef != NULL, "CBTreeTestMultiSet<>::CBTreeTestMultiSet: ERROR: insufficient memory!");
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeTestMultiSet ()
{
	delete m_pClRef;
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>&
	CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator=
	(
		CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT
	)
{
	if (this != &rBT)
	{
		CBTreeMultiSet_t		&rThisMM = dynamic_cast<CBTreeMultiSet_t &> (*this);
		CBTreeMultiSet_t		&rBTMM = dynamic_cast<CBTreeMultiSet_t &> (rBT);

		rThisMM = rBTMM;

		(*m_pClRef) = *(rBT.m_pClRef);

		test ();
	}

	return (*this);
}

/*
template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::begin ()
{

}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::end ()
{
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator
	CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rbegin ()
{
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator
	CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rend ()
{
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::max_size ()
{
}

*/

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
template <class _t_iterator>
void CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	m_pClRef->template insert<_t_iterator> (sItFirst, sItLast);

	CBTreeMultiSet_t::template insert<_t_iterator> (sItFirst, sItLast);

	test ();
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert
	(
		const value_t &rData
	)
{
	iterator	sIter;
	value_t		sData = rData;

	m_nDebug++;

	m_pClRef->insert (sData);

	sIter = CBTreeMultiSet_t::insert (sData);

	test ();

	return (sIter);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterPos
	)
{
	typedef typename ::std::multiset<uint32_t>::iterator		iter_mm_t;

	_t_sizetype		nPos = sCIterPos.get_pos ();
	iter_mm_t		sItMM;
	iterator		sRslt;

	sItMM = m_pClRef->begin ();

	::std::advance<iter_mm_t, _t_sizetype> (sItMM, nPos);

	m_pClRef->erase (sItMM);

	sRslt = CBTreeMultiSet_t::erase (sCIterPos);

	test ();

	return (sRslt);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (const uint32_t &rKey)
{
	_t_sizetype		nRslt;

	m_pClRef->erase (rKey);

	nRslt = CBTreeMultiSet_t::erase (rKey);

	test ();

	return (nRslt);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterFirst, 
		typename CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterLast
	)
{
	typedef typename ::std::multiset<uint32_t>::iterator		iter_mm_t;

	_t_sizetype		nPos = sCIterFirst.get_pos ();
	iter_mm_t		sItMMfirst;
	iter_mm_t		sItMMlast;

	sItMMlast = sItMMfirst = m_pClRef->begin ();

	::std::advance<iter_mm_t, _t_sizetype> (sItMMfirst, nPos);

	nPos = sCIterLast.get_pos ();

	::std::advance<iter_mm_t, _t_sizetype> (sItMMlast, nPos);

	m_pClRef->erase (sItMMfirst, sItMMlast);

	return (CBTreeMultiSet_t::erase (sCIterFirst, sCIterLast));
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap (CBTreeTestMultiSet_t &rTMM)
{
	if (this != &rTMM)
	{
		CBTreeMultiSet_t	&rThisMM = dynamic_cast<CBTreeMultiSet_t &> (*this);
		CBTreeMultiSet_t	&rMM = dynamic_cast<CBTreeMultiSet_t &> (rTMM);

		rThisMM.swap (rMM);

		m_pClRef->swap (*(rTMM.m_pClRef));
	}
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ()
{
	m_pClRef->clear ();

	CBTreeMultiSet_t::clear ();
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::key_compare
	CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::key_comp () const
{
	key_compare		sRslt;

	return (sRslt);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::value_compare
	CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::value_comp () const
{
	value_compare	sRslt;

	return (sRslt);
}

/*
template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find (const uint32_t &rKey)
{

}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::lower_bound (const uint32_t &rKey)
{

}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::upper_bound (const uint32_t &rKey)
{

}
*/

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator== (CBTreeTestMultiSet_t &rTMM)
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

		if (sThisVal != sTMMval)
		{
			return (false);
		}
	}

	return (true);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator!= (CBTreeTestMultiSet_t &rTMM)
{
	return (! ((*this) == rTMM));
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeTestMultiSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::test ()
{
	typedef ::std::multiset<uint32_t>::const_iterator		citer_mmap_t;

	::std::multiset<uint32_t>	sMSet;
	uint32_t									nKey;
	uint32_t									nNextKey;
	bool										bBounce;
	_t_sizetype									nTotalCount = 0;
	value_t										sEntry;
	value_t										sValue;
	citer_mmap_t								sItMSetLower;
	citer_mmap_t								sItMSetUpper;
	citer_mmap_t								sItMSet;
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
			sItMSetLower = m_pClRef->lower_bound (nKey);

			sCIterLower = this->lower_bound (nKey);

			sValue = *sItMSetLower;

			sEntry = ((data_t) (*sCIterLower));

			if (sEntry != sValue)
			{
				cerr << endl;
				cerr << "data mismatches" << endl;
				cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << sEntry << endl;
				
				cerr << "reference" << endl;

				cerr << "key: " << std::setfill ('0') << std::hex << std::setw (8) << sValue << endl;

				cerr << std::setfill (' ') << std::dec << std::setw (0);
				
				cerr << "creating data.html..." << endl;

				this->show_integrity ("data.html");

				cerr << "finished!" << endl;

				exit (-1);
			}
		}
		else
		{
			sItMSetLower = m_pClRef->lower_bound (nKey);
			sItMSetUpper = m_pClRef->upper_bound (nKey);

			sMSet.insert<citer_mmap_t> (sItMSetLower, sItMSetUpper);

			sCIterLower = this->lower_bound (nKey);
			sCIterUpper = this->upper_bound (nKey);

			for (sCIter = sCIterLower; sCIter != sCIterUpper; sCIter++)
			{
				sEntry = ((data_t) (*sCIter));

				bDeleted = false;

				for (sItMSet = sMSet.cbegin (); sItMSet != sMSet.cend (); sItMSet++)
				{
					sValue = *sItMSet;

					if (sEntry == sValue)
					{
						sMSet.erase (sItMSet);

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

			if (sMSet.size () != 0)
			{
				cerr << endl;
				cerr << "number of instances mismatches" << endl;
				cerr << "the following entries are still present in reference:" << endl;

				for (sItMSet = sMSet.cbegin (); sItMSet != sMSet.cend (); sItMSet++)
				{
					sValue = *sItMSet;

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

#endif // !BTREETESTMULTISET_CPP
