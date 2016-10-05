/************************************************************
**
** file:	btreearraytestwrapper.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** 
**
************************************************************/

#ifndef	BTREEARRAYTESTWRAPPER_CPP
#define	BTREEARRAYTESTWRAPPER_CPP

#include "btreearraytestwrapper.h"

template<class _t_data, class _t_sizetype, class _t_ref_container>
CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::CBTreeArrayTestWrapper (const uint32_t nNodeSize, const uint32_t nPageSize)
	:	m_nNodeSize (nNodeSize)
	,	m_nPageSize (nPageSize)
	,	m_bSkipLimitedContainers (false)
	,	m_pClAccessWrapper (NULL)
	,	m_psReturnData (NULL)
{
	m_pReference = new reference_t;

	BTREE_ASSERT (m_pReference != NULL, "CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::CBTreeArrayTestWrapper (const uint32_t, const uint32_t): ERROR: insufficient memory! (m_pReference)");

	m_pClAccessWrapper = new CBTreeArrayTestAccessWrapper_t (*this);

	BTREE_ASSERT (m_pClAccessWrapper != NULL, "CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::CBTreeArrayTestWrapper (const uint32_t, const uint32_t): ERROR: insufficient memory! (m_pClAccessWrapper)");

	m_psReturnData = new value_type ();

	BTREE_ASSERT (m_pClAccessWrapper != NULL, "CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::CBTreeArrayTestWrapper (const uint32_t, const uint32_t): ERROR: insufficient memory! (m_psReturnData)");

	this->m_nNumContainers = CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::m_nNumArrays;

	this->instantiate_container_array ();

	this->init_containers (this->m_nNodeSize, this->m_nPageSize);

	this->transfer_containers ();
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::CBTreeArrayTestWrapper (const CBTreeArrayTestWrapper_t &rContainer)
	:	m_nNodeSize (rContainer.m_nNodeSize)
	,	m_nPageSize (rContainer.m_nPageSize)
	,	m_bSkipLimitedContainers (rContainer.m_bSkipLimitedContainers)
	,	m_pClAccessWrapper (NULL)
	,	m_psReturnData (NULL)
{
	m_pReference = new reference_t (*(rContainer.m_pReference));

	BTREE_ASSERT (m_pReference != NULL, "CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::CBTreeArrayTestWrapper (const CBTreeArrayTestWrapper_t &): ERROR: insufficient memory! (m_pReference)");

	m_pClAccessWrapper = new CBTreeArrayTestAccessWrapper_t (*this);

	BTREE_ASSERT (m_pClAccessWrapper != NULL, "CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::CBTreeArrayTestWrapper (const CBTreeArrayTestWrapper_t &): ERROR: insufficient memory! (m_pClAccessWrapper)");

	m_psReturnData = new value_type ();

	BTREE_ASSERT (m_pClAccessWrapper != NULL, "CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::CBTreeArrayTestWrapper (const CBTreeArrayTestWrapper_t &): ERROR: insufficient memory! (m_psReturnData)");

	this->m_nNumContainers = CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::m_nNumArrays;

	this->instantiate_container_array ();

	this->init_containers (rContainer);

	this->transfer_containers ();

	this->test ();
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::~CBTreeArrayTestWrapper ()
{
	uint32_t	ui32;

	if (this->m_ppContainers != NULL)
	{
		for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
		{
			if (this->m_ppContainers[ui32] != NULL)
			{
				delete this->m_ppContainers[ui32];
			}
		}

		delete [] this->m_ppContainers;
	}

	if (this->m_pReference != NULL)
	{
		delete this->m_pReference;
	}

	if (m_pClAccessWrapper != NULL)
	{
		delete m_pClAccessWrapper;
	}

	if (m_psReturnData != NULL)
	{
		delete m_psReturnData;
	}
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
bool CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::empty () const
{
	uint32_t	ui32;
	bool		bRetval = this->m_pReference->empty ();

	for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
	{
		if (skip_container (ui32))
		{
			continue;
		}

		if (bRetval != this->m_ppContainers[ui32]->empty ())
		{
			::std::cerr << "CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::empty () const: ERROR: unexpected result! container: " << ui32 << ::std::endl;

			exit (-1);
		}
	}

	return (bRetval);
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size_type CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size () const
{
	size_type		nRslt = this->m_pReference->size ();
	uint32_t		ui32;

	for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
	{
		if (skip_container (ui32))
		{
			continue;
		}

		if (nRslt != this->m_ppContainers[ui32]->size ())
		{
			::std::cerr << "CBTreeArrayTestWrapper<>::size (): ERROR: container size mismatches reference size! (" << ui32 << ")" << ::std::endl;

			exit (-1);
		}
	}

	return (nRslt);
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::const_iterator CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::cbegin () const
{
	return (this->m_pReference->cbegin ());
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::const_iterator CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::cend () const
{
	return (this->m_pReference->cend ());
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::const_reverse_iterator CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::crbegin () const
{
	return (this->m_pReference->crbegin ());
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::const_reverse_iterator CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::crend () const
{
	return (this->m_pReference->crend ());
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::iterator CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::begin ()
{
	return (this->m_pReference->begin ());
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::iterator CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::end ()
{
	return (this->m_pReference->end ());
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::reverse_iterator CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::rbegin ()
{
	return (this->m_pReference->rbegin ());
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::reverse_iterator CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::rend ()
{
	return (this->m_pReference->rend ());
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::const_iterator CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::begin () const
{
	return (this->m_pReference->begin ());
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::const_iterator CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::end () const
{
	return (this->m_pReference->end ());
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::const_reverse_iterator CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::rbegin () const
{
	return (this->m_pReference->rbegin ());
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::const_reverse_iterator CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::rend () const
{
	return (this->m_pReference->rend ());
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
template<class _t_iterator>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::assign (_t_iterator &rItFirst, _t_iterator &rItLast)
{
	uint32_t		ui32 = 0;

	this->m_pReference->assign (rItFirst, rItLast);

	if (!skip_container (ui32)) m_pContainerRAM6565_n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM6555_n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM5555_n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM5554_n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM5454_n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM5444_n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM4444_n->assign (rItFirst, rItLast); ui32++;
	
	if (!skip_container (ui32)) m_pContainerRAM6565_2n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM6555_2n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM5555_2n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM5554_2n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM5454_2n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM5444_2n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM4444_2n->assign (rItFirst, rItLast); ui32++;
	
	if (!skip_container (ui32)) m_pContainerRAM6565_4n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM6555_4n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM5555_4n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM5554_4n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM5454_4n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM5444_4n->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerRAM4444_4n->assign (rItFirst, rItLast); ui32++;

	if (!skip_container (ui32)) m_pContainerFile6565min->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerFile6555min->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerFile5555min->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerFile5554min->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerFile5454min->assign (rItFirst, rItLast); ui32++;
	
	if (!skip_container (ui32)) m_pContainerFile6565default->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerFile6555default->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerFile5555default->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerFile5554default->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerFile5454default->assign (rItFirst, rItLast); ui32++;
	
	if (!skip_container (ui32)) m_pContainerFile6565large->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerFile6555large->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerFile5555large->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerFile5554large->assign (rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) m_pContainerFile5454large->assign (rItFirst, rItLast); ui32++;
	
	test ();
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
template<class _t_iterator>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::assign_via_self_reference (_t_iterator &rItFirst, _t_iterator &rItLast)
{
	_t_iterator				sItBegin;
	size_test_type			nFirst;
	size_test_type			nLast;
	reference_t				sTmpContainer (rItFirst, rItLast);
	const_iterator			sItTmpBegin = sTmpContainer.cbegin ();
	const_iterator			sItTmpEnd = sTmpContainer.cend ();

	get_begin (this->m_pReference, sItBegin);

	nFirst = (size_test_type) ::std::distance (sItBegin, rItFirst);
	nLast = (size_test_type) ::std::distance (sItBegin, rItLast);

	this->m_pReference->assign (sItTmpBegin, sItTmpEnd);

	if (is_const_iterator (this->m_pReference, sItBegin))
	{
		if (is_reverse_iterator (this->m_pReference, sItBegin))
		{
			test_const_reverse_iterator	rCRIter;

			assign_via_self_reference_all (rCRIter, nFirst, nLast);
		}
		else
		{
			test_const_iterator			rCIter;

			assign_via_self_reference_all (rCIter, nFirst, nLast);
		}
	}
	else
	{
		if (is_reverse_iterator (this->m_pReference, sItBegin))
		{
			test_reverse_iterator			rRIter;

			assign_via_self_reference_all (rRIter, nFirst, nLast);
		}
		else
		{
			test_iterator					rIter;

			assign_via_self_reference_all (rIter, nFirst, nLast);
		}
	}

	test ();
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::assign (typename _t_ref_container::size_type nNewSize, const typename _t_ref_container::value_type& rVal)
{
	uint32_t	ui32;

	this->m_pReference->assign (nNewSize, rVal);

	for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
	{
		if (skip_container (ui32))
		{
			continue;
		}

		this->m_ppContainers[ui32]->assign ((_t_sizetype) nNewSize, rVal);
	}

	test ();
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::push_back (const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::value_type &rData)
{
	uint32_t	ui32;

	this->m_pReference->push_back (rData);

	for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
	{
		if (skip_container (ui32))
		{
			continue;
		}

		this->m_ppContainers[ui32]->push_back (rData);
	}

	test ();
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::pop_back ()
{
	uint32_t	ui32;

	this->m_pReference->pop_back ();

	for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
	{
		if (skip_container (ui32))
		{
			continue;
		}

		this->m_ppContainers[ui32]->pop_back ();
	}

	test ();
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::iterator
	CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::insert
	(
		typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::const_iterator sCIterPos, const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::value_type& rData
	)
{
	uint32_t		ui32;
	size_type		nDist;
	const_iterator	sCIterRefBegin;
	iterator		sIterRefRslt;

	sCIterRefBegin = this->m_pReference->cbegin ();

	nDist = ::std::distance (sCIterRefBegin, sCIterPos);

	this->m_pReference->insert (sCIterPos, rData);

	for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
	{
		if (skip_container (ui32))
		{
			continue;
		}

		test_const_iterator		sCIterTest;
		test_iterator			sIterRslt;
		test_iterator			sIterBegin;

		sCIterTest = this->m_ppContainers[ui32]->cbegin ();

		::std::advance (sCIterTest, nDist);

		sIterRslt = this->m_ppContainers[ui32]->insert (sCIterTest, rData);

		sIterBegin = this->m_ppContainers[ui32]->begin ();

		if (nDist != ::std::distance (sIterBegin, sIterRslt))
		{
			::std::cerr << "CBTreeArrayTestWrapper<>::insert (const_iterator, const _t_data &): ERROR: Unexpected result!" << ::std::endl;

			exit (-1);
		}
	}

	sIterRefRslt = this->m_pReference->begin ();

	::std::advance (sIterRefRslt, nDist);

	test ();

	return (sIterRefRslt);
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::iterator
	CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::insert
	(
		typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::const_iterator sCIterPos, 
		const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size_type nLen, 
		const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::value_type& rData
	)
{
	uint32_t		ui32;
	size_type		nDist;
	const_iterator	sCIterRefBegin;
	iterator		sIterRefRslt;

	sCIterRefBegin = this->m_pReference->cbegin ();

	nDist = ::std::distance (sCIterRefBegin, sCIterPos);

	this->m_pReference->insert (sCIterPos, nLen, rData);

	for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
	{
		if (skip_container (ui32))
		{
			continue;
		}

		test_const_iterator		sCIterTest;
		test_iterator			sIterRslt;
		test_iterator			sIterBegin;

		sCIterTest = this->m_ppContainers[ui32]->cbegin ();

		::std::advance (sCIterTest, (int) nDist);

		sIterRslt = this->m_ppContainers[ui32]->insert (sCIterTest, (size_test_type) nLen, rData);

		sIterBegin = this->m_ppContainers[ui32]->begin ();

		if (nDist != ::std::distance (sIterBegin, sIterRslt))
		{
			::std::cerr << "CBTreeArrayTestWrapper<>::insert (const_iterator, const _t_sizetype, const _t_data &): ERROR: Unexpected result!" << ::std::endl;

			exit (-1);
		}
	}

	sIterRefRslt = this->m_pReference->begin ();

	::std::advance (sIterRefRslt, nDist);

	test ();

	return (sIterRefRslt);
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
template<class _t_iterator>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::iterator
	CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::insert
	(
		typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::const_iterator sCIterPos, 
		_t_iterator &rItFirst, 
		_t_iterator &rItLast
	)
{
	uint32_t					ui32 = 0;
	size_type					nDist;
	const_iterator				sCIterRefBegin;
	iterator					sIterRefRslt;

	sCIterRefBegin = this->m_pReference->cbegin ();

	nDist = ::std::distance (sCIterRefBegin, sCIterPos);

	this->m_pReference->insert (sCIterPos, rItFirst, rItLast);

	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM6565_n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM6555_n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM5555_n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM5554_n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM5454_n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM5444_n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM4444_n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM6565_2n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM6555_2n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM5555_2n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM5554_2n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM5454_2n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM5444_2n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM4444_2n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM6565_4n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM6555_4n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM5555_4n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM5554_4n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM5454_4n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM5444_4n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerRAM4444_4n, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerFile6565min, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerFile6555min, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerFile5555min, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerFile5554min, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerFile5454min, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerFile6565default, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerFile6555default, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerFile5555default, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerFile5554default, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerFile5454default, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerFile6565large, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerFile6555large, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerFile5555large, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerFile5554large, nDist, rItFirst, rItLast); ui32++;
	if (!skip_container (ui32)) this->test_insert_all_containers (m_pContainerFile5454large, nDist, rItFirst, rItLast); ui32++;
	
	sIterRefRslt = this->m_pReference->begin ();

	::std::advance (sIterRefRslt, nDist);

	test ();

	return (sIterRefRslt);
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
template<class _t_iterator>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::insert_via_self_reference
	(
		typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::const_iterator sCIterPos, 
		_t_iterator &rItFirst, 
		_t_iterator &rItLast
	)
{
	const_iterator						sCItBegin;
	_t_iterator							sItBegin;
	size_test_type						nFirst;
	size_test_type						nLast;
	size_test_type						nPos;
	reference_t							sTmpContainer (rItFirst, rItLast);
	const_iterator						sItTmpBegin = sTmpContainer.cbegin ();
	const_iterator						sItTmpEnd = sTmpContainer.cend ();

	get_begin (this->m_pReference, sItBegin);
	get_begin (this->m_pReference, sCItBegin);

	nFirst = (size_test_type) ::std::distance (sItBegin, rItFirst);
	nLast = (size_test_type) ::std::distance (sItBegin, rItLast);
	nPos = (size_test_type) ::std::distance (sCItBegin, sCIterPos);

	this->m_pReference->insert (sCIterPos, sItTmpBegin, sItTmpEnd);

	if (is_const_iterator (this->m_pReference, sItBegin))
	{
		if (is_reverse_iterator (this->m_pReference, sItBegin))
		{
			test_const_reverse_iterator	rCRIter;

			insert_via_self_reference_all (rCRIter, nPos, nFirst, nLast);
		}
		else
		{
			test_const_iterator			rCIter;

			insert_via_self_reference_all (rCIter, nPos, nFirst, nLast);
		}
	}
	else
	{
		if (is_reverse_iterator (this->m_pReference, sItBegin))
		{
			test_reverse_iterator			rRIter;

			insert_via_self_reference_all (rRIter, nPos, nFirst, nLast);
		}
		else
		{
			test_iterator					rIter;

			insert_via_self_reference_all (rIter, nPos, nFirst, nLast);
		}
	}

	test ();
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::iterator
	CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::erase
	(
		typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::const_iterator sCIterPos
	)
{
	uint32_t		ui32;
	size_type		nDist;
	const_iterator	sCIterRefBegin;
	iterator		sIterRefRslt;

	sCIterRefBegin = this->m_pReference->cbegin ();

	nDist = ::std::distance (sCIterRefBegin, sCIterPos);

	this->m_pReference->erase (sCIterPos);

	for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
	{
		if (skip_container (ui32))
		{
			continue;
		}

		test_const_iterator		sCIterTest;
		test_iterator			sIterRslt;
		test_iterator			sIterBegin;

		sCIterTest = this->m_ppContainers[ui32]->cbegin ();

		::std::advance (sCIterTest, (size_test_type) nDist);

		sIterRslt = this->m_ppContainers[ui32]->erase (sCIterTest);

		sIterBegin = this->m_ppContainers[ui32]->begin ();

		if (nDist != ::std::distance (sIterBegin, sIterRslt))
		{
			::std::cerr << "CBTreeArrayTestWrapper<>::erase (const_iterator): ERROR: Unexpected result!" << ::std::endl;

			exit (-1);
		}
	}

	sIterRefRslt = this->m_pReference->begin ();

	::std::advance (sIterRefRslt, nDist);

	test ();

	return (sIterRefRslt);
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::iterator
	CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::erase
	(
		typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::const_iterator sCIterFirst, 
		typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::const_iterator sCIterLast
	)
{
	uint32_t		ui32;
	size_type		nDistFirst;
	size_type		nDistLast;
	const_iterator	sCIterRefBegin;
	iterator		sIterRefRslt;

	sCIterRefBegin = this->m_pReference->cbegin ();

	nDistFirst = ::std::distance (sCIterRefBegin, sCIterFirst);

	nDistLast = ::std::distance (sCIterRefBegin, sCIterLast);

	this->m_pReference->erase (sCIterFirst, sCIterLast);

	for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
	{
		if (skip_container (ui32))
		{
			continue;
		}

		test_const_iterator			sCIterTestFirst;
		test_const_iterator			sCIterTestLast;
		test_iterator				sIterRslt;
		test_iterator				sIterBegin;

		sCIterTestFirst = this->m_ppContainers[ui32]->cbegin ();

		::std::advance (sCIterTestFirst, (CBTreeArrayTestWrapper_t::size_test_type) nDistFirst);

		sCIterTestLast = this->m_ppContainers[ui32]->cbegin ();

		::std::advance (sCIterTestLast, (size_test_type) nDistLast);

		sIterRslt = this->m_ppContainers[ui32]->erase (sCIterTestFirst, sCIterTestLast);

		sIterBegin = this->m_ppContainers[ui32]->begin ();

		if (nDistFirst != ::std::distance (sIterBegin, sIterRslt))
		{
			::std::cerr << "CBTreeArrayTestWrapper<>::erase (const_iterator, const_iterator): ERROR: Unexpected result!" << ::std::endl;

			exit (-1);
		}
	}

	sIterRefRslt = this->m_pReference->begin ();

	::std::advance (sIterRefRslt, nDistFirst);

	test ();

	return (sIterRefRslt);
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::replace (const_iterator sCIterPos, const _t_data& rData)
{
	uint32_t		ui32;
	iterator		sIter = this->m_pReference->begin ();
	const_iterator	sCIterBegin = this->m_pReference->cbegin ();
	size_type		nPos;

	nPos = ::std::distance (sCIterBegin, sCIterPos);

	::std::advance (sIter, nPos);

	*sIter = rData;

	for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
	{
		if (skip_container (ui32))
		{
			continue;
		}

		test_iterator		sIterTest = this->m_ppContainers[ui32]->begin ();

		::std::advance (sIterTest, (size_test_type) nPos);

		*sIterTest = rData;
	}

	test ();
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::swap (CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container> &rArrayWrapper)
{
	uint32_t	ui32;

	this->m_pReference->swap (*(rArrayWrapper.m_pReference));

	for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
	{
		this->m_ppContainers[ui32]->swap (*(rArrayWrapper.m_ppContainers[ui32]));
	}

	test ();
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::clear ()
{
	uint32_t	ui32;

	this->m_pReference->clear ();

	for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
	{
		this->m_ppContainers[ui32]->clear ();
	}

	test ();
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size_type CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::serialize (const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size_type nStart, const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size_type nLen, typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::value_type *pData) const
{
	typedef typename _t_ref_container::size_type			size_type;
	
	uint32_t		ui32;
	const_iterator	sCIter;
	size_type		nPos;
	size_test_type	nSize;
	size_test_type	nSizeFirst;
	size_type		nRslt;

	for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
	{
		if (skip_container (ui32))
		{
			continue;
		}

		nSize = this->m_ppContainers[ui32]->serialize ((size_test_type) nStart, (size_test_type) nLen, pData);

		if (ui32 == 0)
		{
			nSizeFirst = nSize;
		}
		else if (nSize != nSizeFirst)
		{
			::std::cerr << "CBTreeArrayTestWrapper<>::serialize (const _t_sizetype, const _t_sizetype, _t_data *) const: ERROR: unexpected result!" << ::std::endl;

			exit (-1);
		}

		sCIter = this->m_pReference->cbegin ();

		::std::advance (sCIter, nStart);

		for (nPos = 0; nPos < nSize; nPos++, ::std::advance (sCIter, 1))
		{
			if (pData[nPos] != *sCIter)
			{
				::std::cerr << "CBTreeArrayTestWrapper<>::serialize (const _t_sizetype, const _t_sizetype, _t_data *) const: ERROR: data mismatch!" << ::std::endl;

				exit (-1);
			}
		}
	}

	nRslt = (size_type) nSizeFirst;

	return (nRslt);
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::unload ()
{
	uint32_t	ui32;

	for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
	{
		this->m_ppContainers[ui32]->unload ();
	}

	test ();
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::show_integrity (const char *pszPrefix)
{
	uint32_t				i;
	
	for (i = 0; i < this->get_num_containers (); i++)
	{
		::std::stringstream		sStr;

		sStr << pszPrefix << ::std::setw (4) << ::std::setfill ('0') << i << ".html";

		this->m_ppContainers[i]->show_integrity (sStr.str ().c_str ());
	}
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container> & CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::operator= (const CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container> &rContainer)
{
	uint32_t		ui32;

	*(this->m_pReference) = *(rContainer.m_pReference);

	disable_atomic_testing ();
	{
		for (ui32 = 0; ui32 < this->get_num_containers (); ui32++)
		{
			*(this->m_ppContainers[ui32]) = *(rContainer.m_ppContainers[ui32]);
		}
	}
	enable_atomic_testing ();

	test ();

	return (*this);
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container> & CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::operator= (CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container> &&rRhBT)
{

}

template<class _t_data, class _t_sizetype, class _t_ref_container>
typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::CBTreeArrayTestAccessWrapper_t CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::operator[] (const typename reference_t::size_type nPos)
{
	BTREE_ASSERT_EXCEPT (nPos < this->size (), ::std::out_of_range, "CBTreeArrayTestWrapper<>::operator[] (const size_type): nPos exceeds range!");

	m_pClAccessWrapper->set ((size_test_type) nPos);

	return (*m_pClAccessWrapper);
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
const _t_data& CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::operator[] (const typename reference_t::size_type nPos) const
{
	BTREE_ASSERT_EXCEPT (nPos < this->size (), ::std::out_of_range, "CBTreeArrayTestWrapper<>::operator[] (const size_type) const: nPos exceeds range!");

	const_iterator		sCIter;

	sCIter = this->cbegin ();

	::std::advance (sCIter, nPos);

	*m_psReturnData = *sCIter;

	return (*m_psReturnData);
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
bool CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::operator== (const CBTreeArrayTestWrapper_t &rContainer) const
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

		if (sEntry != sExtEntry)
		{
			return (false);
		}

		::std::advance (sCIter, 1);
		::std::advance (sExtCIter, 1);
	}

	uint32_t		i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		if (skip_container (i))
		{
			continue;
		}

		test_const_iterator		sTestCIter;
		test_const_iterator		sExtTestCIter;

		sTestCIter = m_ppContainers[i]->cbegin ();
		sExtTestCIter = rContainer.m_ppContainers[i]->cbegin ();

		while (sTestCIter != m_ppContainers[i]->cend ())
		{
			_t_data				sTestEntry (*sTestCIter);
			_t_data				sExtTestEntry (*sExtTestCIter);

			if (sTestEntry != sExtTestEntry)
			{
				return (false);
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

template<class _t_data, class _t_sizetype, class _t_ref_container>
bool CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::operator!= (const CBTreeArrayTestWrapper_t &rContainer) const
{
	return (!(*(this) == rContainer));
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::skip_containers_with_limited_address_space (const bool bEnable)
{
	m_bSkipLimitedContainers = bEnable;
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::test () const
{
	uint32_t			i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		if (skip_container (i))
		{
			continue;
		}

		m_ppContainers[i]->test ();
	}
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::disable_atomic_testing () const
{
	uint32_t			i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		m_ppContainers[i]->set_atomic_testing (false);
	}
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::enable_atomic_testing () const
{
	uint32_t			i;

	for (i = 0; i < this->get_num_containers (); i++)
	{
		m_ppContainers[i]->set_atomic_testing (true);
	}
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
bool CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::is_limited_container (uint32_t nContainer) const
{
	switch (nContainer)
	{
	case	14	:	
	case	22	:	return (false);

					break;
	}

	return (true);
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
bool CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::skip_container (uint32_t nContainer) const
{
	return (is_limited_container (nContainer) && m_bSkipLimitedContainers);
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::instantiate_container_array ()
{
	m_ppContainers = new CBTreeArrayIf_t * [this->get_num_containers ()];

	BTREE_ASSERT (NULL != m_ppContainers, "CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::instantiate_container_array (): ERROR: insufficient memory! (m_ppContainers)");
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
uint32_t CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::get_num_containers () const
{
	return (this->m_nNumArrays);
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::init_containers (const uint32_t nNodeSize, const uint32_t nPageSize)
{
	CBTreeIOpropertiesRAM<size_test_type, uint64_t, uint32_t, uint64_t, uint32_t>		sRAMprop6565;
	CBTreeIOpropertiesRAM<size_test_type, uint64_t, uint32_t, uint32_t, uint32_t>		sRAMprop6555;
	CBTreeIOpropertiesRAM<size_test_type, uint32_t, uint32_t, uint32_t, uint32_t>		sRAMprop5555;
	CBTreeIOpropertiesRAM<size_test_type, uint32_t, uint32_t, uint32_t, uint16_t>		sRAMprop5554;
	CBTreeIOpropertiesRAM<size_test_type, uint32_t, uint16_t, uint32_t, uint16_t>		sRAMprop5454;
	CBTreeIOpropertiesRAM<size_test_type, uint32_t, uint16_t, uint16_t, uint16_t>		sRAMprop5444;
	CBTreeIOpropertiesRAM<size_test_type, uint16_t, uint16_t, uint16_t, uint16_t>		sRAMprop4444;
	CBTreeIOpropertiesFile<size_test_type, uint64_t, uint32_t, uint64_t, uint32_t>		sFilePropertiesMin6565 ("./", 1);
	CBTreeIOpropertiesFile<size_test_type, uint64_t, uint32_t, uint32_t, uint32_t>		sFilePropertiesMin6555 ("./", 1);
	CBTreeIOpropertiesFile<size_test_type, uint32_t, uint32_t, uint32_t, uint32_t>		sFilePropertiesMin5555 ("./", 1);
	CBTreeIOpropertiesFile<size_test_type, uint32_t, uint32_t, uint32_t, uint16_t>		sFilePropertiesMin5554 ("./", 1);
	CBTreeIOpropertiesFile<size_test_type, uint32_t, uint16_t, uint32_t, uint16_t>		sFilePropertiesMin5454 ("./", 1);
	CBTreeIOpropertiesFile<size_test_type, uint64_t, uint32_t, uint64_t, uint32_t>		sFilePropertiesDefault6565 ("./");
	CBTreeIOpropertiesFile<size_test_type, uint64_t, uint32_t, uint32_t, uint32_t>		sFilePropertiesDefault6555 ("./");
	CBTreeIOpropertiesFile<size_test_type, uint32_t, uint32_t, uint32_t, uint32_t>		sFilePropertiesDefault5555 ("./");
	CBTreeIOpropertiesFile<size_test_type, uint32_t, uint32_t, uint32_t, uint16_t>		sFilePropertiesDefault5554 ("./");
	CBTreeIOpropertiesFile<size_test_type, uint32_t, uint16_t, uint32_t, uint16_t>		sFilePropertiesDefault5454 ("./");
	CBTreeIOpropertiesFile<size_test_type, uint64_t, uint32_t, uint64_t, uint32_t>		sFilePropertiesLarge6565 ("./", 16777216);
	CBTreeIOpropertiesFile<size_test_type, uint64_t, uint32_t, uint32_t, uint32_t>		sFilePropertiesLarge6555 ("./", 16777216);
	bayerTreeCacheDescription_t															sCacheDescPageSize = {nPageSize};
	bayerTreeCacheDescription_t															sCacheDescMin = {1};
	bayerTreeCacheDescription_t															sCacheDescNearestBigger = {nPageSize * 2 / 3};
	bayerTreeCacheDescription_t															sCacheDescLarge = {nPageSize * 16};
	uint32_t																			i = 0;

	m_pContainerRAM6565_n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (sRAMprop6565, &sCacheDescPageSize, nNodeSize, this->m_pReference);
	m_pContainerRAM6555_n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (sRAMprop6555, &sCacheDescPageSize, nNodeSize, this->m_pReference);
	m_pContainerRAM5555_n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (sRAMprop5555, &sCacheDescPageSize, nNodeSize, this->m_pReference);
	m_pContainerRAM5554_n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (sRAMprop5554, &sCacheDescPageSize, nNodeSize, this->m_pReference);
	m_pContainerRAM5454_n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (sRAMprop5454, &sCacheDescPageSize, nNodeSize, this->m_pReference);
	m_pContainerRAM5444_n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> > (sRAMprop5444, &sCacheDescPageSize, nNodeSize, this->m_pReference);
	m_pContainerRAM4444_n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> > (sRAMprop4444, &sCacheDescPageSize, nNodeSize, this->m_pReference);
	m_pContainerRAM6565_2n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (sRAMprop6565, &sCacheDescPageSize, nNodeSize * 2, this->m_pReference);
	m_pContainerRAM6555_2n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (sRAMprop6555, &sCacheDescPageSize, nNodeSize * 2, this->m_pReference);
	m_pContainerRAM5555_2n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (sRAMprop5555, &sCacheDescPageSize, nNodeSize * 2, this->m_pReference);
	m_pContainerRAM5554_2n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (sRAMprop5554, &sCacheDescPageSize, nNodeSize * 2, this->m_pReference);
	m_pContainerRAM5454_2n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (sRAMprop5454, &sCacheDescPageSize, nNodeSize * 2, this->m_pReference);
	m_pContainerRAM5444_2n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> > (sRAMprop5444, &sCacheDescPageSize, nNodeSize * 2, this->m_pReference);
	m_pContainerRAM4444_2n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> > (sRAMprop4444, &sCacheDescPageSize, nNodeSize * 2, this->m_pReference);
	m_pContainerRAM6565_4n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (sRAMprop6565, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerRAM6555_4n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (sRAMprop6555, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerRAM5555_4n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (sRAMprop5555, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerRAM5554_4n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (sRAMprop5554, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerRAM5454_4n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (sRAMprop5454, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerRAM5444_4n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> > (sRAMprop5444, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerRAM4444_4n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> > (sRAMprop4444, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerFile6565min = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (sFilePropertiesMin6565, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerFile6555min = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesMin6555, &sCacheDescPageSize, nNodeSize * 8, this->m_pReference);
	m_pContainerFile5555min = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesMin5555, &sCacheDescMin, nNodeSize * 7, this->m_pReference);
	m_pContainerFile5554min = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (sFilePropertiesMin5554, &sCacheDescNearestBigger, nNodeSize * 6, this->m_pReference);
	m_pContainerFile5454min = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (sFilePropertiesMin5454, &sCacheDescLarge, nNodeSize * 5, this->m_pReference);

	m_pContainerFile6565default = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (sFilePropertiesDefault6565, &sCacheDescNearestBigger, nNodeSize * 3, this->m_pReference);
	m_pContainerFile6555default = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesDefault6555, &sCacheDescNearestBigger, nNodeSize * 2, this->m_pReference);
	m_pContainerFile5555default = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesDefault5555, &sCacheDescLarge, nNodeSize, this->m_pReference);
	m_pContainerFile5554default = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (sFilePropertiesDefault5554, &sCacheDescPageSize, nNodeSize * 8, this->m_pReference);
	m_pContainerFile5454default = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (sFilePropertiesDefault5454, &sCacheDescMin, nNodeSize * 7, this->m_pReference);

	m_pContainerFile6565large = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (sFilePropertiesLarge6565, &sCacheDescPageSize, nNodeSize * 5, this->m_pReference);
	m_pContainerFile6555large = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesLarge6555, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerFile5555large = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesDefault5555, &sCacheDescMin, nNodeSize * 3, this->m_pReference);
	m_pContainerFile5554large = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (sFilePropertiesDefault5554, &sCacheDescNearestBigger, nNodeSize * 2, this->m_pReference);
	m_pContainerFile5454large = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (sFilePropertiesDefault5454, &sCacheDescLarge, nNodeSize, this->m_pReference);

	BTREE_ASSERT (NULL != m_pContainerRAM6565_n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM6565_n)");
	BTREE_ASSERT (NULL != m_pContainerRAM6555_n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM6555_n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5555_n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5555_n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5554_n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5554_n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5454_n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5454_n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5444_n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5444_n)");
	BTREE_ASSERT (NULL != m_pContainerRAM4444_n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM4444_n)");
	BTREE_ASSERT (NULL != m_pContainerRAM6565_2n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM6565_2n)");
	BTREE_ASSERT (NULL != m_pContainerRAM6555_2n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM6555_2n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5555_2n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5555_2n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5554_2n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5554_2n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5454_2n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5454_2n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5444_2n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5444_2n)");
	BTREE_ASSERT (NULL != m_pContainerRAM4444_2n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM4444_2n)");
	BTREE_ASSERT (NULL != m_pContainerRAM6565_4n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM6565_4n)");
	BTREE_ASSERT (NULL != m_pContainerRAM6555_4n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM6555_4n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5555_4n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5555_4n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5554_4n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5554_4n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5454_4n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5454_4n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5444_4n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5444_4n)");
	BTREE_ASSERT (NULL != m_pContainerRAM4444_4n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM4444_4n)");
	BTREE_ASSERT (NULL != m_pContainerFile6565min, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile6565min)");
	BTREE_ASSERT (NULL != m_pContainerFile6555min, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile6555min)");
	BTREE_ASSERT (NULL != m_pContainerFile5555min, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5555min)");
	BTREE_ASSERT (NULL != m_pContainerFile5554min, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5554min)");
	BTREE_ASSERT (NULL != m_pContainerFile5454min, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5454min)");

	BTREE_ASSERT (NULL != m_pContainerFile6565default, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile6565default)");
	BTREE_ASSERT (NULL != m_pContainerFile6555default, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile6555default)");
	BTREE_ASSERT (NULL != m_pContainerFile5555default, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5555default)");
	BTREE_ASSERT (NULL != m_pContainerFile5554default, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5554default)");
	BTREE_ASSERT (NULL != m_pContainerFile5454default, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5454default)");

	BTREE_ASSERT (NULL != m_pContainerFile6565large, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile6565large)");
	BTREE_ASSERT (NULL != m_pContainerFile6555large, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile6555large)");
	BTREE_ASSERT (NULL != m_pContainerFile5555large, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5555large)");
	BTREE_ASSERT (NULL != m_pContainerFile5554large, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5554large)");
	BTREE_ASSERT (NULL != m_pContainerFile5454large, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5454large)");
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::init_containers (const CBTreeArrayTestWrapper_t &rWrapper)
{
	uint32_t	i = 0;

	m_pContainerRAM6565_n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (*rWrapper.m_pContainerRAM6565_n); i++;
	m_pContainerRAM6555_n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (*rWrapper.m_pContainerRAM6555_n); i++;
	m_pContainerRAM5555_n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (*rWrapper.m_pContainerRAM5555_n); i++;
	m_pContainerRAM5554_n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (*rWrapper.m_pContainerRAM5554_n); i++;
	m_pContainerRAM5454_n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (*rWrapper.m_pContainerRAM5454_n); i++;
	m_pContainerRAM5444_n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> > (*rWrapper.m_pContainerRAM5444_n); i++;
	m_pContainerRAM4444_n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> > (*rWrapper.m_pContainerRAM4444_n); i++;
	m_pContainerRAM6565_2n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (*rWrapper.m_pContainerRAM6565_2n); i++;
	m_pContainerRAM6555_2n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (*rWrapper.m_pContainerRAM6555_2n); i++;
	m_pContainerRAM5555_2n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (*rWrapper.m_pContainerRAM5555_2n); i++;
	m_pContainerRAM5554_2n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (*rWrapper.m_pContainerRAM5554_2n); i++;
	m_pContainerRAM5454_2n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (*rWrapper.m_pContainerRAM5454_2n); i++;
	m_pContainerRAM5444_2n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> > (*rWrapper.m_pContainerRAM5444_2n); i++;
	m_pContainerRAM4444_2n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> > (*rWrapper.m_pContainerRAM4444_2n); i++;
	m_pContainerRAM6565_4n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (*rWrapper.m_pContainerRAM6565_4n); i++;
	m_pContainerRAM6555_4n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (*rWrapper.m_pContainerRAM6555_4n); i++;
	m_pContainerRAM5555_4n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (*rWrapper.m_pContainerRAM5555_4n); i++;
	m_pContainerRAM5554_4n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (*rWrapper.m_pContainerRAM5554_4n); i++;
	m_pContainerRAM5454_4n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (*rWrapper.m_pContainerRAM5454_4n); i++;
	m_pContainerRAM5444_4n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> > (*rWrapper.m_pContainerRAM5444_4n); i++;
	m_pContainerRAM4444_4n = new CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> > (*rWrapper.m_pContainerRAM4444_4n); i++;
	m_pContainerFile6565min = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (*rWrapper.m_pContainerFile6565min); i++;
	m_pContainerFile6555min = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (*rWrapper.m_pContainerFile6555min); i++;
	m_pContainerFile5555min = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (*rWrapper.m_pContainerFile5555min); i++;
	m_pContainerFile5554min = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (*rWrapper.m_pContainerFile5554min); i++;
	m_pContainerFile5454min = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (*rWrapper.m_pContainerFile5454min); i++;

	m_pContainerFile6565default = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (*rWrapper.m_pContainerFile6565default); i++;
	m_pContainerFile6555default = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (*rWrapper.m_pContainerFile6555default); i++;
	m_pContainerFile5555default = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (*rWrapper.m_pContainerFile5555default); i++;
	m_pContainerFile5554default = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (*rWrapper.m_pContainerFile5554default); i++;
	m_pContainerFile5454default = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (*rWrapper.m_pContainerFile5454default); i++;

	m_pContainerFile6565large = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (*rWrapper.m_pContainerFile6565large); i++;
	m_pContainerFile6555large = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (*rWrapper.m_pContainerFile6555large); i++;
	m_pContainerFile5555large = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (*rWrapper.m_pContainerFile5555large); i++;
	m_pContainerFile5554large = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (*rWrapper.m_pContainerFile5554large); i++;
	m_pContainerFile5454large = new CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (*rWrapper.m_pContainerFile5454large); i++;

	BTREE_ASSERT (i == this->get_num_containers (), "CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::init_containers (const CBTreeArrayTestWrapper &): ERROR: Unexpected number of test containers instantiated!");

	i = 0;

	BTREE_ASSERT (NULL != m_pContainerRAM6565_n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM6565_n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM6555_n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM6555_n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5555_n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5555_n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5554_n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5554_n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5454_n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5454_n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5444_n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5444_n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM4444_n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM4444_n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM6565_2n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM6565_2n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM6555_2n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM6555_2n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5555_2n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5555_2n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5554_2n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5554_2n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5454_2n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5454_2n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5444_2n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5444_2n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM4444_2n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM4444_2n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM6565_4n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM6565_4n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM6555_4n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM6555_4n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5555_4n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5555_4n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5554_4n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5554_4n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5454_4n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5454_4n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5444_4n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM5444_4n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM4444_4n, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerRAM4444_4n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile6565min, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile6565min)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile6555min, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile6555min)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5555min, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5555min)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5554min, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5554min)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5454min, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5454min)"); i++;

	BTREE_ASSERT (NULL != m_pContainerFile6565default, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile6565default)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile6555default, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile6555default)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5555default, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5555default)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5554default, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5554default)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5454default, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5454default)"); i++;

	BTREE_ASSERT (NULL != m_pContainerFile6565large, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile6565large)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile6555large, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile6555large)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5555large, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5555large)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5554large, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5554large)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5454large, "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers: ERROR: insufficient memory! (m_pContainerFile5454large)"); i++;

	BTREE_ASSERT (i == this->get_num_containers (), "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers (const CBTreeAssociativeTestWrapper &): ERROR: Unexpected number of test containers checked to be instantiated successfully!");

	i = 0;

	m_pContainerRAM6565_n->set_reference (this->m_pReference); i++;
	m_pContainerRAM6555_n->set_reference (this->m_pReference); i++;
	m_pContainerRAM5555_n->set_reference (this->m_pReference); i++;
	m_pContainerRAM5554_n->set_reference (this->m_pReference); i++;
	m_pContainerRAM5454_n->set_reference (this->m_pReference); i++;
	m_pContainerRAM5444_n->set_reference (this->m_pReference); i++;
	m_pContainerRAM4444_n->set_reference (this->m_pReference); i++;
	m_pContainerRAM6565_2n->set_reference (this->m_pReference); i++;
	m_pContainerRAM6555_2n->set_reference (this->m_pReference); i++;
	m_pContainerRAM5555_2n->set_reference (this->m_pReference); i++;
	m_pContainerRAM5554_2n->set_reference (this->m_pReference); i++;
	m_pContainerRAM5454_2n->set_reference (this->m_pReference); i++;
	m_pContainerRAM5444_2n->set_reference (this->m_pReference); i++;
	m_pContainerRAM4444_2n->set_reference (this->m_pReference); i++;
	m_pContainerRAM6565_4n->set_reference (this->m_pReference); i++;
	m_pContainerRAM6555_4n->set_reference (this->m_pReference); i++;
	m_pContainerRAM5555_4n->set_reference (this->m_pReference); i++;
	m_pContainerRAM5554_4n->set_reference (this->m_pReference); i++;
	m_pContainerRAM5454_4n->set_reference (this->m_pReference); i++;
	m_pContainerRAM5444_4n->set_reference (this->m_pReference); i++;
	m_pContainerRAM4444_4n->set_reference (this->m_pReference); i++;
	m_pContainerFile6565min->set_reference (this->m_pReference); i++;
	m_pContainerFile6555min->set_reference (this->m_pReference); i++;
	m_pContainerFile5555min->set_reference (this->m_pReference); i++;
	m_pContainerFile5554min->set_reference (this->m_pReference); i++;
	m_pContainerFile5454min->set_reference (this->m_pReference); i++;

	m_pContainerFile6565default->set_reference (this->m_pReference); i++;
	m_pContainerFile6555default->set_reference (this->m_pReference); i++;
	m_pContainerFile5555default->set_reference (this->m_pReference); i++;
	m_pContainerFile5554default->set_reference (this->m_pReference); i++;
	m_pContainerFile5454default->set_reference (this->m_pReference); i++;

	m_pContainerFile6565large->set_reference (this->m_pReference); i++;
	m_pContainerFile6555large->set_reference (this->m_pReference); i++;
	m_pContainerFile5555large->set_reference (this->m_pReference); i++;
	m_pContainerFile5554large->set_reference (this->m_pReference); i++;
	m_pContainerFile5454large->set_reference (this->m_pReference); i++;

	BTREE_ASSERT (i == this->get_num_containers (), "CBTreeArrayTestWrapper<_t_data, _t_value, _t_ref_container>::init_containers (const CBTreeAssociativeTestWrapper &): ERROR: Unexpected number of test containers after setting reference container!");
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::transfer_containers ()
{
	uint32_t		i = 0;

	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM6565_n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM6555_n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM5555_n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM5554_n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM5454_n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM5444_n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM4444_n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM6565_2n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM6555_2n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM5555_2n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM5554_2n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM5454_2n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM5444_2n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM4444_2n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM6565_4n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM6555_4n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM5555_4n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM5554_4n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM5454_4n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM5444_4n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerRAM4444_4n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerFile6565min); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerFile6555min); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerFile5555min); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerFile5554min); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerFile5454min); i++;

	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerFile6565default); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerFile6555default); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerFile5555default); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerFile5554default); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerFile5454default); i++;

	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerFile6565large); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerFile6555large); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerFile5555large); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerFile5554large); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeArrayIf_t *> (m_pContainerFile5454large); i++;

	BTREE_ASSERT (i == this->get_num_containers (), "CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::transfer_containers: ERROR: Unexpected number of test containers!");
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
template<class _t_container, class _t_iterator>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::test_insert_all_containers (_t_container *pContainer, const size_type nDist, _t_iterator &sItFirst, _t_iterator &sItLast)
{
	test_const_iterator		sCIterTest;
	test_iterator			sIterRslt;
	test_iterator			sIterBegin;

	sCIterTest = pContainer->cbegin ();

	::std::advance (sCIterTest, (int) nDist);

	sIterRslt = pContainer->insert (sCIterTest, sItFirst, sItLast);

	sIterBegin = pContainer->begin ();

	if (nDist != ::std::distance (sIterBegin, sIterRslt))
	{
		::std::cerr << "CBTreeArrayTestWrapper<>::test_insert_all_containers (): ERROR: Unexpected result!" << ::std::endl;

		exit (-1);
	}
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
template<class _t_iterator>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::assign_via_self_reference_all (_t_iterator &rIter, const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size_test_type nFirst, const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size_test_type nLast)
{
	uint32_t			ui32 = 0;

	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM6565_n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM6555_n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM5555_n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM5554_n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM5454_n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM5444_n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM4444_n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM6565_2n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM6555_2n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM5555_2n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM5554_2n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM5454_2n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM5444_2n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM4444_2n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM6565_4n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM6555_4n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM5555_4n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM5554_4n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM5454_4n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM5444_4n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerRAM4444_4n, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerFile6565min, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerFile6555min, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerFile5555min, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerFile5554min, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerFile5454min, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerFile6565default, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerFile6555default, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerFile5555default, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerFile5554default, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerFile5454default, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerFile6565large, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerFile6555large, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerFile5555large, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerFile5554large, rIter, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) assign_via_self_reference_one (m_pContainerFile5454large, rIter, nFirst, nLast); ui32++;
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
template<class _t_container, class _t_iterator>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::assign_via_self_reference_one (_t_container *pContainer, _t_iterator &rIter, const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size_test_type nFirst, const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size_test_type nLast)
{
	_t_iterator		sItFirst;
	_t_iterator		sItLast;

	get_begin (pContainer, sItFirst);
	get_begin (pContainer, sItLast);

	::std::advance (sItFirst, nFirst);
	::std::advance (sItLast, nLast);

	pContainer->assign (sItFirst, sItLast);
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
template<class _t_iterator>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::insert_via_self_reference_all (_t_iterator &rIter, const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size_test_type nPos, const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size_test_type nFirst, const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size_test_type nLast)
{
	uint32_t			ui32 = 0;

	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM6565_n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM6555_n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM5555_n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM5554_n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM5454_n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM5444_n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM4444_n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM6565_2n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM6555_2n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM5555_2n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM5554_2n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM5454_2n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM5444_2n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM4444_2n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM6565_4n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM6555_4n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM5555_4n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM5554_4n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM5454_4n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM5444_4n, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerRAM4444_4n, rIter, nPos, nFirst, nLast); ui32++;	
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerFile6565min, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerFile6555min, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerFile5555min, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerFile5554min, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerFile5454min, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerFile6565default, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerFile6555default, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerFile5555default, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerFile5554default, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerFile5454default, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerFile6565large, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerFile6555large, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerFile5555large, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerFile5554large, rIter, nPos, nFirst, nLast); ui32++;
	if (!skip_container (ui32)) insert_via_self_reference_one (m_pContainerFile5454large, rIter, nPos, nFirst, nLast); ui32++;
}

template<class _t_data, class _t_sizetype, class _t_ref_container>
template<class _t_container, class _t_iterator>
void CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::insert_via_self_reference_one (_t_container *pContainer, _t_iterator &rIter, const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size_test_type nPos, const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size_test_type nFirst, const typename CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::size_test_type nLast)
{
	typename _t_container::const_iterator	sCItPos;
	_t_iterator								sItFirst;
	_t_iterator								sItLast;

	get_begin (pContainer, sItFirst);
	get_begin (pContainer, sItLast);
	get_begin (pContainer, sCItPos);

	::std::advance (sItFirst, nFirst);
	::std::advance (sItLast, nLast);
	::std::advance (sCItPos, nPos);

	pContainer->insert (sCItPos, sItFirst, sItLast);
}
	
template<class _t_data, class _t_sizetype, class _t_ref_container>
const uint32_t CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>::m_nNumArrays = 36;

/*******************************************************/

template<class _t_container, class _t_ref_container>
CBTreeArrayConstTestAccessWrapper<_t_container, _t_ref_container>::CBTreeArrayConstTestAccessWrapper
	(
		CBTreeArrayTestWrapper<typename _t_container::value_type, typename _t_container::size_type, _t_ref_container>	&rInstance
	)
	:	m_rInstance (rInstance)
	,	m_nPos (0)
{
}

template<class _t_container, class _t_ref_container>
CBTreeArrayConstTestAccessWrapper<_t_container, _t_ref_container>::~CBTreeArrayConstTestAccessWrapper ()
{
}

template<class _t_container, class _t_ref_container>
void CBTreeArrayConstTestAccessWrapper<_t_container, _t_ref_container>::set (const typename _t_container::size_type nPos)
{
	this->m_nPos = nPos;
}

template<class _t_container, class _t_ref_container>
typename _t_container::value_type & CBTreeArrayConstTestAccessWrapper<_t_container, _t_ref_container>::operator* ()
{
	bool		bRslt;

	bRslt = m_rInstance.get_at (this->m_nPos, this->m_sData);

	BTREE_ASSERT (bRslt, "CBTreeArrayAccessWrapper<>::operator* (): get_at was not successful!");

	return (this->m_sData);
}

template<class _t_container, class _t_ref_container>
typename _t_container::value_type & CBTreeArrayConstTestAccessWrapper<_t_container, _t_ref_container>::operator-> ()
{
	return (*this);
}

template<class _t_container, class _t_ref_container>
CBTreeArrayConstTestAccessWrapper<_t_container, _t_ref_container>::operator const typename _t_container::value_type & ()
{
	const_iterator		sCIter;
	uint32_t			ui32;

	get_begin (&m_rInstance, sCIter);

	::std::advance (sCIter, this->m_nPos);

	this->m_sData = *sCIter;

	for (ui32 = 0; ui32 < m_rInstance.get_num_containers (); ui32++)
	{
		BTREE_ASSERT (this->m_sData == (value_test_type)((*(m_rInstance.m_ppContainers[ui32]))[this->m_nPos]), "CBTreeArrayConstTestAccessWrapper<>::operator const value_type &: ERROR: data mismatch");
	}

	return (this->m_sData);
}

/************************************************************/

template<class _t_container, class _t_ref_container>
CBTreeArrayTestAccessWrapper<_t_container, _t_ref_container>::CBTreeArrayTestAccessWrapper
	(
		CBTreeArrayTestWrapper<typename _t_container::value_type, typename _t_container::size_type, _t_ref_container>	&rInstance
	)
	:	CBTreeArrayConstTestAccessWrapper<_t_container, _t_ref_container> (rInstance)
{
}

template<class _t_container, class _t_ref_container>
CBTreeArrayTestAccessWrapper<_t_container, _t_ref_container>::~CBTreeArrayTestAccessWrapper ()
{
}

template<class _t_container, class _t_ref_container>
CBTreeArrayTestAccessWrapper<_t_container, _t_ref_container> &CBTreeArrayTestAccessWrapper<_t_container, _t_ref_container>::operator=
	(const typename _t_container::value_type &rData)
{
	iterator		sIter;
	uint32_t		ui32;

	get_begin (&this->m_rInstance, sIter);

	::std::advance (sIter, this->m_nPos);

	*sIter = rData;

	for (ui32 = 0; ui32 < this->m_rInstance.get_num_containers (); ui32++)
	{
		(*(this->m_rInstance.m_ppContainers[ui32]))[this->m_nPos] = rData;
	}

	this->m_rInstance.test ();

	return (*this);
}

template<class _t_container, class _t_ref_container>
CBTreeArrayTestAccessWrapper<_t_container, _t_ref_container> &CBTreeArrayTestAccessWrapper<_t_container, _t_ref_container>::operator=
	(const CBTreeArrayTestAccessWrapper<_t_container, _t_ref_container> &rData)
{
	if (this != &rData)
	{
		const_iterator		sCIter;

		get_begin (&(rData.m_rInstance), sCIter);

		::std::advance (sCIter, rData.m_nPos);

		*this = (value_test_type) *sCIter;
	}

	return (*this);
}

#endif // BTREEARRAYTESTWRAPPER_CPP
