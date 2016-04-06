/************************************************************
**
** file:	btreeassociativetestwrapper.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains the definition of the associative container
** test class wrapping abstract accesses to associative container test class
** and a template based reference container instance, while making sure the
** associative container instance acts the same way as the abstract reference.
**
************************************************************/

#include "btreeassociativetestwrapper.h"

#ifndef BTREEASSOCIATIVETESTWRAPPER_CPP
#define BTREEASSOCIATIVETESTWRAPPER_CPP

template <class _t_data, class _t_value, class _t_ref_container>
CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::CBTreeAssociativeTestWrapper (const uint32_t nNodeSize, const uint32_t nPageSize)
	:	m_pReference (NULL)
	,	m_nNodeSize (nNodeSize)
	,	m_nPageSize (nPageSize)
{
	m_pReference = new reference_t;

	BTREE_ASSERT (m_pReference != NULL, "CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::CBTreeAssociativeTestWrapper: ERROR: insufficient memory! (m_pReference)");
}

template <class _t_data, class _t_value, class _t_ref_container>
CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::CBTreeAssociativeTestWrapper (const CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container> &rContainer)
	:	m_pReference (NULL)
	,	m_nNodeSize (rContainer.m_nNodeSize)
	,	m_nPageSize (rContainer.m_nPageSize)
{
	m_pReference = new reference_t (*rContainer.m_pReference);

	BTREE_ASSERT (m_pReference != NULL, "CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::CBTreeAssociativeTestWrapper (const CBTreeAssociativeTestWrapper &): ERROR: insufficient memory! (m_pReference)");
}

template <class _t_data, class _t_value, class _t_ref_container>
CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::~CBTreeAssociativeTestWrapper ()
{
	uint32_t		i;

	if (m_ppContainers != NULL)
	{
		for (i = 0; i < this->get_num_containers (); i++)
		{
			if (m_ppContainers[i] != NULL)
			{
				delete m_ppContainers[i];
			}
		}

		delete [] m_ppContainers;
	}

	if (m_pReference != NULL)
	{
		delete m_pReference;
	}
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::size_type CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::size () const
{
	sizetype_ref_t		nSize = m_pReference->size ();
	uint32_t			i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		if (nSize != m_ppContainers[i]->size ())
		{
			::std::cerr << "Container returned different size than the reference container" << ::std::endl;
			::std::cerr << "reference size:" << nSize << ::std::endl;
			::std::cerr << "container size (" << i << "):" << m_ppContainers[i]->size () << ::std::endl;
			::std::cerr << "writing output to size.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("size.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}
	}

	return (nSize);
}

template <class _t_data, class _t_value, class _t_ref_container>
void CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::clear ()
{
	uint32_t		i;

	m_pReference->clear ();

	for (i = 0; i < this->get_num_containers (); i++)
	{
		m_ppContainers[i]->clear ();
	}

	this->test ();
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::begin ()
{
	iterator		sIter = this->m_pReference->begin ();

	return (sIter);
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::end ()
{
	iterator		sIter = this->m_pReference->end ();

	return (sIter);
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::reverse_iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::rbegin ()
{
	reverse_iterator		sRIter = this->m_pReference->rbegin ();

	return (sRIter);
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::reverse_iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::rend ()
{
	reverse_iterator		sRIter = this->m_pReference->rend ();

	return (sRIter);
}

	

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::const_iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::cbegin () const
{
	const_iterator		sCIter = this->m_pReference->cbegin ();

	return (sCIter);
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::const_iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::cend () const
{
	const_iterator		sCIter = this->m_pReference->cend ();

	return (sCIter);
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::const_reverse_iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::crbegin () const
{
	const_reverse_iterator	sCRIter = this->m_pReference->crbegin ();

	return (sCRIter);
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::const_reverse_iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::crend () const
{
	const_reverse_iterator	sCRIter = this->m_pReference->crend ();

	return (sCRIter);
}

template <class _t_data, class _t_value, class _t_ref_container>
void CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::swap (CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container> &rContainer)
{
	m_pReference->swap ((reference_t &) (*(rContainer.m_pReference)));

	uint32_t	i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		m_ppContainers[i]->swap ((CBTreeAssociativeIf_t &) (*(rContainer.m_ppContainers[i])));
	}

	test ();
}

/*
template <class _t_data, class _t_value, class _t_ref_container>
template<class _t_iterator>
void CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::insert (_t_iterator sIterFirst, _t_iterator sIterLast)
{
	uint32_t		i = 0;

	m_pReference->insert (sIterFirst, sIterLast);

	m_pContainerRAM6565_n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM6555_n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM5555_n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM5554_n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM5454_n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM5444_n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM4444_n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM6565_2n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM6555_2n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM5555_2n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM5554_2n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM5454_2n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM5444_2n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM4444_2n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM6565_4n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM6555_4n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM5555_4n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM5554_4n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM5454_4n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM5444_4n->insert (sIterFirst, sIterLast); i++;
	m_pContainerRAM4444_4n->insert (sIterFirst, sIterLast); i++;
	m_pContainerFile6565min->insert (sIterFirst, sIterLast); i++;
	m_pContainerFile6555min->insert (sIterFirst, sIterLast); i++;
	m_pContainerFile5555min->insert (sIterFirst, sIterLast); i++;
	m_pContainerFile5554min->insert (sIterFirst, sIterLast); i++;
	m_pContainerFile5454min->insert (sIterFirst, sIterLast); i++;

	m_pContainerFile6565default->insert (sIterFirst, sIterLast); i++;
	m_pContainerFile6555default->insert (sIterFirst, sIterLast); i++;
	m_pContainerFile5555default->insert (sIterFirst, sIterLast); i++;
	m_pContainerFile5554default->insert (sIterFirst, sIterLast); i++;
	m_pContainerFile5454default->insert (sIterFirst, sIterLast); i++;

	m_pContainerFile6565large->insert (sIterFirst, sIterLast); i++;
	m_pContainerFile6555large->insert (sIterFirst, sIterLast); i++;
	m_pContainerFile5555large->insert (sIterFirst, sIterLast); i++;
	m_pContainerFile5554large->insert (sIterFirst, sIterLast); i++;
	m_pContainerFile5454large->insert (sIterFirst, sIterLast); i++;

	BTREE_ASSERT (i == this->get_num_containers (), "CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::insert<_t_iterator>: ERROR: Unexpected number of test containers!");

	this->test ();
}
*/

template <class _t_data, class _t_value, class _t_ref_container>
typename CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::insert (const typename CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::value_type &rData)
{
	iterator			sRefIter;
	sizetype_ref_t		nRefDiff;
	sizetype_ref_t		nRefKeyDist;
	uint32_t			i;
	test_iterator		sTestIter;
	sizetype_test_t		nTestDiff;
	sizetype_test_t		nTestKeyDist;
	data_t				sData;

	sRefIter = m_pReference->insert (rData);

	nRefDiff = ::std::distance (m_pReference->begin (), sRefIter);

	sRefIter = m_pReference->lower_bound (get_entry_key (rData));

	nRefKeyDist = ::std::distance (m_pReference->begin (), sRefIter);

	for (i = 0; i < this->get_num_containers (); i++)
	{
		entry_conversion (sData, rData);

		sTestIter = m_ppContainers[i]->insert (sData);

		nTestDiff = ::std::distance (m_ppContainers[i]->begin (), sTestIter);

		sTestIter = m_ppContainers[i]->lower_bound (get_entry_key (sData));

		nTestKeyDist = ::std::distance (m_ppContainers[i]->begin (), sTestIter);

		if ((nTestDiff != nRefDiff) || (nTestKeyDist != nRefKeyDist))
		{
			::std::cerr << "Container returned different iterator offset than reference container" << ::std::endl;
			::std::cerr << "reference offset:" << nRefDiff << ::std::endl;
			::std::cerr << "container offset (" << i << "):" << nTestDiff << ::std::endl;
			::std::cerr << "reference distance:" << nRefKeyDist << ::std::endl;
			::std::cerr << "container distance (" << i << "):" << nTestKeyDist << ::std::endl;
			::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("mismatch.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}
	}

	this->test ();

	return (sRefIter);
}

/*
template <class _t_data, class _t_value, class _t_ref_container>
template<class _t_iterator, class _t_test_iterator>
void CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::insert_self_reference (_t_iterator sIterFirst, _t_iterator sIterLast, _t_test_iterator &rIterTest)
{
	uint32_t			i = 0;
	_t_iterator			sIterBegin;
	sizetype_test_t		nFirst;
	sizetype_test_t		nLast;
	reference_t			sTempContainer;
	_t_test_iterator	sIterTestFirst;
	_t_test_iterator	sIterTestLast;

	get_begin (m_pReference, sIterBegin);
	nFirst = (sizetype_test_t) ::std::distance (sIterBegin, sIterFirst);
	nLast = (sizetype_test_t) ::std::distance (sIterBegin, sIterLast);

	sTempContainer.insert (sIterFirst, sIterLast);

	m_pReference->insert (sTempContainer.cbegin (), sTempContainer.cend ());

	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM6565_n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM6555_n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5555_n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5554_n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5454_n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5444_n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM4444_n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM6565_2n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM6555_2n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5555_2n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5554_2n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5454_2n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5444_2n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM4444_2n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM6565_4n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM6555_4n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5555_4n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5554_4n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5454_4n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5444_4n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM4444_4n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile6565min->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile6555min->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5555min->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5554min->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5454min->insert (sIterTestFirst, sIterTestLast); i++;

	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile6565default->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile6555default->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5555default->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5554default->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5454default->insert (sIterTestFirst, sIterTestLast); i++;

	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile6565large->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile6555large->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5555large->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5554large->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5454large->insert (sIterTestFirst, sIterTestLast); i++;

	BTREE_ASSERT (i == this->get_num_containers (), "CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::insert<_t_iterator>: ERROR: Unexpected number of test containers!");

	this->test ();
}
*/

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::erase (typename _t_ref_container::const_iterator sCIterPos)
{
	sizetype_ref_t		nOffset = ::std::distance (m_pReference->cbegin (), sCIterPos);
	uint32_t			i;
	iterator			sRefIter;
	sizetype_ref_t		nRefDiff;
	sizetype_test_t		nTestDiff;

	sRefIter = m_pReference->erase (sCIterPos);

	nRefDiff = ::std::distance (m_pReference->begin (), sRefIter);

	for (i = 0; i < this->get_num_containers (); i++)
	{
		test_const_iterator		sCIter = m_ppContainers[i]->cbegin ();
		test_iterator			sIter;

		::std::advance (sCIter, (sizetype_test_t) nOffset);

		sIter = m_ppContainers[i]->erase (sCIter);

		nTestDiff = ::std::distance (m_ppContainers[i]->begin (), sIter);

		if (nTestDiff != nRefDiff)
		{
			::std::cerr << "Container returned different iterator offset than reference container" << ::std::endl;
			::std::cerr << "reference offset:" << nRefDiff << ::std::endl;
			::std::cerr << "container offset (" << i << "):" << nTestDiff << ::std::endl;
			::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("mismatch.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}
	}

	this->test ();

	return (sRefIter);
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::size_type CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::erase (typename _t_ref_container::key_type &rKey)
{
	sizetype_ref_t		nRefRslt;
	sizetype_test_t		nTestRslt;
	uint32_t			i;
	
	nRefRslt = m_pReference->erase (rKey);

	for (i = 0; i < this->get_num_containers (); i++)
	{
		nTestRslt = m_ppContainers[i]->erase (rKey);

		if (nTestRslt != nRefRslt)
		{
			::std::cerr << "Container returned different number of entries removed than reference container" << ::std::endl;
			::std::cerr << "reference offset:" << nRefRslt << ::std::endl;
			::std::cerr << "container offset (" << i << "):" << nTestRslt << ::std::endl;
			::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("mismatch.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}
	}

	this->test ();

	return (nRefRslt);
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::erase (typename _t_ref_container::const_iterator sCIterFirst, typename _t_ref_container::const_iterator sCIterLast)
{
	sizetype_ref_t		nFirst = ::std::distance (m_pReference->cbegin (), sCIterFirst);
	sizetype_ref_t		nLast = ::std::distance (m_pReference->cbegin (), sCIterLast);
	uint32_t			i;
	iterator			sRefIter;
	sizetype_test_t		nRefDiff;
	sizetype_test_t		nTestDiff;

	sRefIter = m_pReference->erase (sCIterFirst, sCIterLast);

	nRefDiff = ::std::distance (m_pReference->begin (), sRefIter);

	for (i = 0; i < this->get_num_containers (); i++)
	{
		test_const_iterator		sCIterTestFirst = m_ppContainers[i]->cbegin ();
		test_const_iterator		sCIterTestLast = m_ppContainers[i]->cbegin ();
		test_iterator			sIter;

		::std::advance (sCIterTestFirst, (sizetype_test_t) nFirst);
		::std::advance (sCIterTestLast, (sizetype_test_t) nLast);

		sIter = m_ppContainers[i]->erase (sCIterTestFirst, sCIterTestLast);

		nTestDiff = ::std::distance (m_ppContainers[i]->begin (), sIter);

		if (nTestDiff != nRefDiff)
		{
			::std::cerr << "Container returned different iterator offset than reference container" << ::std::endl;
			::std::cerr << "reference offset:" << nRefDiff << ::std::endl;
			::std::cerr << "container offset (" << i << "):" << nTestDiff << ::std::endl;
			::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("mismatch.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}
	}

	this->test ();

	return (sRefIter);
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::find (typename _t_ref_container::key_type &rKey)
{
	iterator		sIter = this->m_pReference->find (rKey);
	sizetype_ref_t	nRefOffset = ::std::distance (m_pReference->begin (), sIter);
	uint32_t		i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		test_iterator	sTestIter = m_ppContainers[i]->find (rKey);
		sizetype_test_t	nTestOffset = ::std::distance (m_ppContainers[i]->begin (), sTestIter);

		if (((sIter == m_pReference->end ()) != (sTestIter == m_ppContainers[i]->end ())) || 
			((sIter != m_pReference->end ()) && (get_entry_key (*sIter) != get_entry_key ((const _t_value &) *sTestIter))))
		{
			::std::cerr << "Container returned iterator pointing at an entry displaying a different key than reference container" << ::std::endl;

			if (sIter == m_pReference->end ())
			{
				::std::cerr << "reference key value: ? at " << nRefOffset << ::std::endl;
			}
			else
			{
				::std::cerr << "reference key value: " << get_entry_key (*sIter) << " at " << nRefOffset << ::std::endl;
			}

			if (sTestIter == m_ppContainers[i]->end ())
			{
				::std::cerr << "container key value (" << i << "): ? at " << nTestOffset << ::std::endl;
			}
			else
			{
				::std::cerr << "container key value (" << i << "): " << get_entry_key ((const _t_value &) *sTestIter) << " at " << nTestOffset << ::std::endl;
			}

			::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("mismatch.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}
	}

	return (sIter);
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::const_iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::find (typename _t_ref_container::key_type &rKey) const
{
	const_iterator		sCIter = this->m_pReference->find (rKey);
	sizetype_ref_t		nRefOffset = ::std::distance (m_pReference->cbegin (), sCIter);
	uint32_t			i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		test_const_iterator		sTestCIter = m_ppContainers[i]->find (rKey);
		sizetype_test_t			nTestOffset = ::std::distance (m_ppContainers[i]->cbegin (), sTestCIter);

		if (((sCIter == m_pReference->end ()) != (sTestCIter == m_ppContainers[i]->end ())) || 
			((sCIter != m_pReference->end ()) && (get_entry_key (*sCIter) != get_entry_key ((const _t_data &) *sTestCIter))))
		{
			::std::cerr << "Container returned iterator pointing at an entry displaying a different key than reference container" << ::std::endl;

			if (sCIter == m_pReference->cend ())
			{
				::std::cerr << "reference key value: ? at " << nRefOffset << ::std::endl;
			}
			else
			{
				::std::cerr << "reference key value: " << get_entry_key (*sCIter) << " at " << nRefOffset << ::std::endl;
			}

			if (sTestCIter == m_ppContainers[i]->cend ())
			{
				::std::cerr << "container key value (" << i << "): ? at " << nTestOffset << ::std::endl;
			}
			else
			{
				::std::cerr << "container key value (" << i << "): " << get_entry_key ((const _t_data &) *sTestCIter) << " at " << nTestOffset << ::std::endl;
			}

			::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("mismatch.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}
	}

	return (sCIter);
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::lower_bound (typename _t_ref_container::key_type &rKey)
{
	iterator		sIter = m_pReference->lower_bound (rKey);
	sizetype_ref_t	nRefOffset = ::std::distance (m_pReference->begin (), sIter);
	uint32_t		i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		test_iterator	sTestIter = m_ppContainers[i]->lower_bound (rKey);
		sizetype_test_t	nTestOffset = ::std::distance (m_ppContainers[i]->begin (), sTestIter);

		if (sIter == this->m_pReference->end ())
		{
			if (sTestIter != m_ppContainers[i]->end ())
			{
				::std::cerr << "Container didn't return end (iterator) while the reference container did" << ::std::endl;
				::std::cerr << "reference offset:" << nRefOffset << ::std::endl;
				::std::cerr << "container key value (" << i << "):" << nTestOffset << ::std::endl;
				::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

				m_ppContainers[i]->show_integrity ("mismatch.html");

				::std::cerr << "completed" << ::std::endl << ::std::flush;

				exit (-1);
			}

			continue;
		}

		if (get_entry_key (*sIter) != get_entry_key ((const _t_value &) *sTestIter))
		{
			::std::cerr << "Container returned iterator pointing at an entry displaying a different key than reference container" << ::std::endl;
			::std::cerr << "reference key value:" << get_entry_key (*sIter) << " at " << nRefOffset << ::std::endl;
			::std::cerr << "container key value (" << i << "):" << get_entry_key ((const _t_value &) *sTestIter) << " at " << nTestOffset << ::std::endl;
			::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("mismatch.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}

		if (nRefOffset != nTestOffset)
		{
			::std::cerr << "Container returned different offset than reference container" << ::std::endl;
			::std::cerr << "reference key value:" << get_entry_key (*sIter) << " at " << nRefOffset << ::std::endl;
			::std::cerr << "container key value (" << i << "):" << get_entry_key ((const _t_value &) *sTestIter) << " at " << nTestOffset << ::std::endl;
			::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("mismatch.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}
	}

	return (sIter);
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::const_iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::lower_bound (typename _t_ref_container::key_type &rKey) const
{
	const_iterator		sCIter = this->m_pReference->lower_bound (rKey);
	sizetype_ref_t		nRefOffset = ::std::distance (m_pReference->cbegin (), sCIter);
	uint32_t			i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		test_const_iterator		sTestCIter = m_ppContainers[i]->lower_bound (rKey);
		sizetype_test_t			nTestOffset = ::std::distance (m_ppContainers[i]->cbegin (), sTestCIter);

		if ((*sCIter).first != (*sTestCIter).nKey)
		{
			::std::cerr << "Container returned iterator pointing at an entry displaying a different key than reference container" << ::std::endl;
			::std::cerr << "reference key value:" << (*sCIter).first << " at " << nRefOffset << ::std::endl;
			::std::cerr << "container key value (" << i << "):" << (*sTestCIter).nKey << " at " << nTestOffset << ::std::endl;
			::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("mismatch.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}

		if (nRefOffset != nTestOffset)
		{
			::std::cerr << "Container returned different offset than reference container" << ::std::endl;
			::std::cerr << "reference key value:" << (*sCIter).first << " at " << nRefOffset << ::std::endl;
			::std::cerr << "container key value (" << i << "):" << (*sTestCIter).nKey << " at " << nTestOffset << ::std::endl;
			::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("mismatch.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}
	}

	return (sCIter);
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::upper_bound (typename _t_ref_container::key_type &rKey)
{
	iterator		sIter = this->m_pReference->upper_bound (rKey);
	sizetype_ref_t	nRefOffset = ::std::distance (m_pReference->begin (), sIter);
	uint32_t		i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		test_iterator	sTestIter = m_ppContainers[i]->upper_bound (rKey);
		sizetype_test_t	nTestOffset = ::std::distance (m_ppContainers[i]->begin (), sTestIter);

		if (((sIter == m_pReference->end ()) != (sTestIter == m_ppContainers[i]->end ())) || 
			((sIter != m_pReference->end ()) && (get_entry_key (*sIter) != get_entry_key ((const _t_value &) *sTestIter))))
		{
			::std::cerr << "Container returned iterator pointing at an entry displaying a different key than reference container" << ::std::endl;

			if (sIter == m_pReference->end ())
			{
				::std::cerr << "reference key value: ? at " << nRefOffset << ::std::endl;
			}
			else
			{
				::std::cerr << "reference key value: " << get_entry_key (*sIter) << " at " << nRefOffset << ::std::endl;
			}

			if (sTestIter == m_ppContainers[i]->end ())
			{
				::std::cerr << "container key value (" << i << "): ? at " << nTestOffset << ::std::endl;
			}
			else
			{
				::std::cerr << "container key value (" << i << "): " << get_entry_key ((const _t_value &) *sTestIter) << " at " << nTestOffset << ::std::endl;
			}

			::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("mismatch.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}

		if (nRefOffset != nTestOffset)
		{
			::std::cerr << "Container returned different offset than reference container" << ::std::endl;
			
			if (sIter == m_pReference->end ())
			{
				::std::cerr << "reference key value: ? at " << nRefOffset << ::std::endl;
			}
			else
			{
				::std::cerr << "reference key value: " << get_entry_key (*sIter) << " at " << nRefOffset << ::std::endl;
			}

			if (sTestIter == m_ppContainers[i]->end ())
			{
				::std::cerr << "container key value (" << i << "): ? at " << nTestOffset << ::std::endl;
			}
			else
			{
				::std::cerr << "container key value (" << i << "): " << get_entry_key ((const _t_value &) *sTestIter) << " at " << nTestOffset << ::std::endl;
			}
			
			::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("mismatch.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}
	}

	return (sIter);
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::const_iterator CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::upper_bound (typename _t_ref_container::key_type &rKey) const
{
	const_iterator		sCIter = this->m_pReference->upper_bound (rKey);
	sizetype_ref_t		nRefOffset = ::std::distance (m_pReference->cbegin (), sCIter);
	uint32_t			i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		test_const_iterator		sTestCIter = m_ppContainers[i]->upper_bound (rKey);
		sizetype_test_t			nTestOffset = ::std::distance (m_ppContainers[i]->cbegin (), sTestCIter);

		if ((*sCIter).first != (*sTestCIter).nKey)
		{
			::std::cerr << "Container returned iterator pointing at an entry displaying a different key than reference container" << ::std::endl;
			::std::cerr << "reference key value:" << (*sCIter).first << " at " << nRefOffset << ::std::endl;
			::std::cerr << "container key value (" << i << "):" << (*sTestCIter).nKey << " at " << nTestOffset << ::std::endl;
			::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("mismatch.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}

		if (nRefOffset != nTestOffset)
		{
			::std::cerr << "Container returned different offset than reference container" << ::std::endl;
			::std::cerr << "reference key value:" << (*sCIter).first << " at " << nRefOffset << ::std::endl;
			::std::cerr << "container key value (" << i << "):" << (*sTestCIter).nKey << " at " << nTestOffset << ::std::endl;
			::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("mismatch.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}
	}

	return (sCIter);
}

template <class _t_data, class _t_value, class _t_ref_container>
typename _t_ref_container::size_type CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::count (typename _t_ref_container::key_type &rKey) const
{
	sizetype_ref_t		nRefRslt = m_pReference->count (rKey);
	uint32_t			i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		sizetype_test_t			nTestRslt = m_ppContainers[i]->count (rKey);

		if (nRefRslt != nTestRslt)
		{
			::std::cerr << "Container returned different offset than reference container" << ::std::endl;
			::std::cerr << "input key value: " << rKey << :: std::endl;
			::std::cerr << "reference count result:" << nRefRslt << ::std::endl;
			::std::cerr << "container key value (" << i << "):" << nTestRslt << ::std::endl;
			::std::cerr << "writing output to mismatch.html" << ::std::endl << ::std::flush;

			m_ppContainers[i]->show_integrity ("mismatch.html");

			::std::cerr << "completed" << ::std::endl << ::std::flush;

			exit (-1);
		}
	}

	return (nRefRslt);
}

template <class _t_data, class _t_value, class _t_ref_container>
uint64_t CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::serialize (const sizetype_test_t nStart, const sizetype_test_t nLen, typename CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::value_type *pData) const
{
	sizetype_ref_t			i;
	sizetype_ref_t			j;
	const_iterator			sCIter;
	sizetype_test_t			nRslt;
	sizetype_test_t			nRetval;
	data_t					*pContainerData = new data_t [(size_t) nLen];

	if (pContainerData == NULL)
	{
		::std::cerr << "CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::serialize: insufficient memory!" << ::std::endl;

		exit (-1);
	}

	for (i = 0; i < this->get_num_containers (); i++)
	{
		nRetval = m_ppContainers[i]->serialize (nStart, nLen, pContainerData);
		
		for (j = 0; j < nRetval; j++)
		{
			entry_conversion (pData[j], pContainerData[j]);
		}

		if (i == 0)
		{
			nRslt = nRetval;
		}
		else
		{
			if (nRslt != nRetval)
			{
				::std::cerr << "ERROR: CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::serialize: unexpected size!" << ::std::endl;
				::std::cerr << "container return value (" << i << "): " << nRetval << ::std::endl;
				::std::cerr << "container return value (" << (i - 1) << "): " << nRslt << ::std::endl;
				::std::cerr << "outputting serialize.html" << ::std::endl << ::std::flush;

				m_ppContainers[i]->show_integrity ("serialize.html");

				::std::cerr << "completed" << ::std::endl << ::std::flush;

				exit (-1);
			}
		}

		sCIter = m_pReference->cbegin ();

		::std::advance (sCIter, nStart);

		for (j = 0; j < nRslt; j++, sCIter++)
		{
			value_type				sEntry (*sCIter);
			
			if (memcmp ((void *) &(pData[j]), (void *) &sEntry, sizeof (sEntry)) != 0)
			{
				::std::cerr << "ERROR: CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::serialize: data mismatch!" << ::std::endl;
				::std::cerr << "container: " << i << ::std::endl;
				::std::cerr << "position: " << j << ::std::endl;
				::std::cerr << "key: " << get_entry_key (pData[j]) << " " << get_entry_key (sEntry) << ::std::endl;
				::std::cerr << "data: " << get_entry_data (pData[j]) << " " << get_entry_data (sEntry) << ::std::endl;
				::std::cerr << "debug: " << get_entry_debug (pData[j]) << " " << get_entry_debug (sEntry) << ::std::endl;
				::std::cerr << "outputting serialize.html" << ::std::endl << ::std::flush;

				m_ppContainers[i]->show_integrity ("serialize.html");

				::std::cerr << "completed" << ::std::endl << ::std::flush;

				exit (-1);
			}
		}
	}

	delete [] pContainerData;

	return (nRslt);
}

template <class _t_data, class _t_value, class _t_ref_container>
void CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::unload ()
{
	uint32_t	i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		m_ppContainers[i]->unload ();
	}
}

template <class _t_data, class _t_value, class _t_ref_container>
void CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::show_integrity (const char *pszPrefix)
{
	uint32_t				i;
	
	for (i = 0; i < this->get_num_containers (); i++)
	{
		::std::stringstream		sStr;

		sStr << pszPrefix << ::std::setw (4) << ::std::setfill ('0') << i << ".html";

		m_ppContainers[i]->show_integrity (sStr.str ().c_str ());
	}
}

template <class _t_data, class _t_value, class _t_ref_container>
CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>& CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::operator= (const CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container> &rContainer)
{
	uint32_t		i;

	*(this->m_pReference) = *(rContainer.m_pReference);

	disable_atomic_testing ();
	{
		for (i = 0; i < this->get_num_containers (); i++)
		{
			*(this->m_ppContainers[i]) = *(rContainer.m_ppContainers[i]);
		}
	}
	enable_atomic_testing ();

	this->test ();

	return (*this);
}

template <class _t_data, class _t_value, class _t_ref_container>
bool CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::operator== (const CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container> &rContainer) const
{
	if (this == &rContainer)
	{
		return (true);
	}

	this->test ();

	if (this->size () != rContainer.size ())
	{
		return (false);
	}

	const_iterator		sCIter, sExtCIter;

	sCIter = m_pReference->cbegin ();
	sExtCIter = rContainer.m_pReference->cbegin ();

	while (sCIter != m_pReference->cend ())
	{
		typename reference_t::value_type		sEntry (*sCIter);
		typename reference_t::value_type		sExtEntry (*sExtCIter);

		if ((get_entry_key (sEntry) != get_entry_key (sExtEntry)) ||
			(get_entry_data (sEntry) != get_entry_data (sExtEntry)))
		{
			return (false);
		}

		::std::advance (sCIter, 1);
		::std::advance (sExtCIter, 1);
	}

	uint32_t		i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		test_const_iterator		sTestCIter;
		test_const_iterator		sExtTestCIter;

		sTestCIter = m_ppContainers[i]->cbegin ();
		sExtTestCIter = rContainer.m_ppContainers[i]->cbegin ();

		while (sTestCIter != m_ppContainers[i]->cend ())
		{
			_t_data				sTestEntry (*sTestCIter);
			_t_data				sExtTestEntry (*sExtTestCIter);

			if ((get_entry_key (sTestEntry) != get_entry_key (sExtTestEntry)) ||
				(get_entry_data (sTestEntry) != get_entry_data (sExtTestEntry)))
			{
				if (get_entry_key (sTestEntry) != get_entry_key (sExtTestEntry))
				{
					return (false);
				}
				else
				{
					test_const_iterator		sTestBeginCIter = m_ppContainers[i]->lower_bound (get_entry_key (sTestEntry));
					test_const_iterator		sTestEndCIter = m_ppContainers[i]->upper_bound (get_entry_key (sTestEntry));
					test_const_iterator		sExtTestBeginCIter = rContainer.m_ppContainers[i]->lower_bound (get_entry_key (sTestEntry));
					test_const_iterator		sExtTestEndCIter = rContainer.m_ppContainers[i]->upper_bound (get_entry_key (sTestEntry));

					sizetype_test_t			nKeySetSize = ::std::distance (sTestBeginCIter, sTestEndCIter);
					sizetype_test_t			nExtKeySetSize = ::std::distance (sExtTestBeginCIter, sExtTestEndCIter);

					if (nKeySetSize != nExtKeySetSize)
					{
						return (false);
					}
					else
					{
						::std::list<data_t>									sList;
						typename ::std::list<data_t>::const_iterator		sListCIter;

						sList.insert (sList.cbegin (), sExtTestBeginCIter, sExtTestEndCIter);

						while (sTestBeginCIter != sTestEndCIter)
						{
							sListCIter = sList.cbegin ();

							while (sListCIter != sList.cend ())
							{
								if (get_entry_data ((_t_data &) *sListCIter) == get_entry_data ((_t_data &) *sTestBeginCIter))
								{
									sList.erase (sListCIter);

									break;
								}

								::std::advance (sListCIter, 1);
							}

							::std::advance (sTestBeginCIter, 1);
						}

						if (sList.size () > 0)
						{
							return (false);
						}
					}

					sExtTestCIter = sExtTestEndCIter;
					sTestCIter = sTestEndCIter;
				}
			}
			else
			{
				::std::advance (sExtTestCIter, 1);
				::std::advance (sTestCIter, 1);
			}
		}
	}

	return (true);
}

template <class _t_data, class _t_value, class _t_ref_container>
bool CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::operator!= (const CBTreeAssociativeTestWrapper &rContainer) const
{
	return (!(*(this) == rContainer));
}

template <class _t_data, class _t_value, class _t_ref_container>
void CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::test () const
{
	uint32_t			i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		m_ppContainers[i]->test ();
	}
}

template <class _t_data, class _t_value, class _t_ref_container>
void CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::disable_atomic_testing () const
{
	uint32_t			i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		m_ppContainers[i]->set_atomic_testing (false);
	}
}

template <class _t_data, class _t_value, class _t_ref_container>
void CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::enable_atomic_testing () const
{
	uint32_t			i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		m_ppContainers[i]->set_atomic_testing (true);
	}
}

template <class _t_data, class _t_value, class _t_ref_container>
void CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::instantiate_reference_container ()
{
	m_ppContainers = new CBTreeAssociativeIf<_t_value, uint32_t, sizetype_test_t> * [this->get_num_containers ()];

	BTREE_ASSERT (NULL != m_ppContainers, "CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::instantiate_reference_container: ERROR: insufficient memory! (m_ppContainers)");
}

template <class _t_data, class _t_value, class _t_ref_container>
uint32_t CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>::get_num_containers () const
{
	return (m_nNumContainers);
}

#endif // BTREEASSOCIATIVETESTWRAPPER_CPP
