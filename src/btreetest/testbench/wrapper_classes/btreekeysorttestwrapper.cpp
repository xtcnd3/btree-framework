/************************************************************
**
** file:	btreekeysorttestwrapper.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains the definition of a key sort test class 
** wrapping accesses to a key sort application class instance and
** a reference container instance, while making sure the
** key sort instance acts the same way as the reference.
**
************************************************************/

#ifndef	BTREEKEYSORTTESTWRAPPER_CPP
#define	BTREEKEYSORTTESTWRAPPER_CPP

#include "testbench/wrapper_classes/btreekeysorttestwrapper.h"

template<class _t_data, class _t_value, class _t_sizetype, class _t_ref_container>
CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper (const uint32_t nNodeSize, const uint32_t nPageSize)
	:	CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container> (nNodeSize, nPageSize)
{
	this->m_nNumContainers = CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::m_nNumContainersKS;

	this->instantiate_container_array ();

	this->init_containers (this->m_nNodeSize, this->m_nPageSize);

	this->transfer_containers ();
}

template<class _t_data, class _t_value, class _t_sizetype, class _t_ref_container>
CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper (const CBTreeKeySortTestWrapper &rContainer)
	:	CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container> (rContainer)
{
	this->m_nNumContainers = CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::m_nNumContainersKS;

	this->instantiate_container_array ();

	this->init_containers (rContainer);

	this->transfer_containers ();

	this->test ();
}

template<class _t_data, class _t_value, class _t_sizetype, class _t_ref_container>
CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::~CBTreeKeySortTestWrapper ()
{
}

template<class _t_data, class _t_value, class _t_sizetype, class _t_ref_container>
template<class _t_iterator>
void CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::insert (_t_iterator sIterFirst, _t_iterator sIterLast)
{
	uint32_t		i = 0;

	this->m_pReference->insert (sIterFirst, sIterLast);

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

	BTREE_ASSERT (i == this->get_num_containers (), "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::insert<_t_iterator>: ERROR: Unexpected number of test containers!");

	this->test ();
}

template<class _t_data, class _t_value, class _t_sizetype, class _t_ref_container>
typename CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::iterator CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::insert (const typename CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::value_type &rData)
{
	return (CBTreeAssociativeTestWrapper_t::insert (rData));
}

template<class _t_data, class _t_value, class _t_sizetype, class _t_ref_container>
template<class _t_iterator, class _t_test_iterator>
void CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::insert_self_reference (_t_iterator sIterFirst, _t_iterator sIterLast, _t_test_iterator &rIterTest)
{
	uint32_t			i = 0;
	_t_iterator			sIterBegin;
	size_test_type		nFirst;
	size_test_type		nLast;
	reference_t			sTempContainer;
	_t_test_iterator	sIterTestFirst;
	_t_test_iterator	sIterTestLast;

	get_begin (this->m_pReference, sIterBegin);
	nFirst = (size_test_type) ::std::distance (sIterBegin, sIterFirst);
	nLast = (size_test_type) ::std::distance (sIterBegin, sIterLast);

	sTempContainer.insert (sIterFirst, sIterLast);

	this->m_pReference->insert (sTempContainer.cbegin (), sTempContainer.cend ());

	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM6565_n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM6555_n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5555_n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5554_n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5454_n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5444_n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM4444_n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM6565_2n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM6555_2n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5555_2n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5554_2n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5454_2n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5444_2n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM4444_2n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM6565_4n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM6555_4n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5555_4n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5554_4n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5454_4n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM5444_4n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerRAM4444_4n->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile6565min->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile6555min->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5555min->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5554min->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5454min->insert (sIterTestFirst, sIterTestLast); i++;

	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile6565default->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile6555default->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5555default->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5554default->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5454default->insert (sIterTestFirst, sIterTestLast); i++;

	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile6565large->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile6555large->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5555large->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5554large->insert (sIterTestFirst, sIterTestLast); i++;
	get_begin (this->m_ppContainers[i], sIterTestFirst); sIterTestLast = sIterTestFirst; ::std::advance (sIterTestFirst, nFirst); ::std::advance (sIterTestLast, nLast); m_pContainerFile5454large->insert (sIterTestFirst, sIterTestLast); i++;

	BTREE_ASSERT (i == this->get_num_containers (), "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::insert<_t_iterator>: ERROR: Unexpected number of test containers!");

	this->test ();
}

template<class _t_data, class _t_value, class _t_sizetype, class _t_ref_container>
void CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::init_containers (const uint32_t nNodeSize, const uint32_t nPageSize)
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

	m_pContainerRAM6565_n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (sRAMprop6565, &sCacheDescPageSize, nNodeSize, this->m_pReference);
	m_pContainerRAM6555_n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (sRAMprop6555, &sCacheDescPageSize, nNodeSize, this->m_pReference);
	m_pContainerRAM5555_n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (sRAMprop5555, &sCacheDescPageSize, nNodeSize, this->m_pReference);
	m_pContainerRAM5554_n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (sRAMprop5554, &sCacheDescPageSize, nNodeSize, this->m_pReference);
	m_pContainerRAM5454_n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (sRAMprop5454, &sCacheDescPageSize, nNodeSize, this->m_pReference);
	m_pContainerRAM5444_n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> > (sRAMprop5444, &sCacheDescPageSize, nNodeSize, this->m_pReference);
	m_pContainerRAM4444_n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> > (sRAMprop4444, &sCacheDescPageSize, nNodeSize, this->m_pReference);
	m_pContainerRAM6565_2n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (sRAMprop6565, &sCacheDescPageSize, nNodeSize * 2, this->m_pReference);
	m_pContainerRAM6555_2n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (sRAMprop6555, &sCacheDescPageSize, nNodeSize * 2, this->m_pReference);
	m_pContainerRAM5555_2n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (sRAMprop5555, &sCacheDescPageSize, nNodeSize * 2, this->m_pReference);
	m_pContainerRAM5554_2n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (sRAMprop5554, &sCacheDescPageSize, nNodeSize * 2, this->m_pReference);
	m_pContainerRAM5454_2n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (sRAMprop5454, &sCacheDescPageSize, nNodeSize * 2, this->m_pReference);
	m_pContainerRAM5444_2n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> > (sRAMprop5444, &sCacheDescPageSize, nNodeSize * 2, this->m_pReference);
	m_pContainerRAM4444_2n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> > (sRAMprop4444, &sCacheDescPageSize, nNodeSize * 2, this->m_pReference);
	m_pContainerRAM6565_4n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (sRAMprop6565, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerRAM6555_4n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (sRAMprop6555, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerRAM5555_4n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (sRAMprop5555, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerRAM5554_4n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (sRAMprop5554, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerRAM5454_4n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (sRAMprop5454, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerRAM5444_4n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> > (sRAMprop5444, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerRAM4444_4n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> > (sRAMprop4444, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerFile6565min = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (sFilePropertiesMin6565, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerFile6555min = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesMin6555, &sCacheDescPageSize, nNodeSize * 8, this->m_pReference);
	m_pContainerFile5555min = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesMin5555, &sCacheDescMin, nNodeSize * 7, this->m_pReference);
	m_pContainerFile5554min = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (sFilePropertiesMin5554, &sCacheDescNearestBigger, nNodeSize * 6, this->m_pReference);
	m_pContainerFile5454min = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (sFilePropertiesMin5454, &sCacheDescLarge, nNodeSize * 5, this->m_pReference);

	m_pContainerFile6565default = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (sFilePropertiesDefault6565, &sCacheDescNearestBigger, nNodeSize * 3, this->m_pReference);
	m_pContainerFile6555default = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesDefault6555, &sCacheDescNearestBigger, nNodeSize * 2, this->m_pReference);
	m_pContainerFile5555default = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesDefault5555, &sCacheDescLarge, nNodeSize, this->m_pReference);
	m_pContainerFile5554default = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (sFilePropertiesDefault5554, &sCacheDescPageSize, nNodeSize * 8, this->m_pReference);
	m_pContainerFile5454default = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (sFilePropertiesDefault5454, &sCacheDescMin, nNodeSize * 7, this->m_pReference);

	m_pContainerFile6565large = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (sFilePropertiesLarge6565, &sCacheDescPageSize, nNodeSize * 5, this->m_pReference);
	m_pContainerFile6555large = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesLarge6555, &sCacheDescPageSize, nNodeSize * 4, this->m_pReference);
	m_pContainerFile5555large = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (sFilePropertiesDefault5555, &sCacheDescMin, nNodeSize * 3, this->m_pReference);
	m_pContainerFile5554large = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (sFilePropertiesDefault5554, &sCacheDescNearestBigger, nNodeSize * 2, this->m_pReference);
	m_pContainerFile5454large = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (sFilePropertiesDefault5454, &sCacheDescLarge, nNodeSize, this->m_pReference);

	BTREE_ASSERT (NULL != m_pContainerRAM6565_n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM6565_n)");
	BTREE_ASSERT (NULL != m_pContainerRAM6555_n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM6555_n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5555_n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5555_n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5554_n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5554_n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5454_n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5454_n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5444_n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5444_n)");
	BTREE_ASSERT (NULL != m_pContainerRAM4444_n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM4444_n)");
	BTREE_ASSERT (NULL != m_pContainerRAM6565_2n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM6565_2n)");
	BTREE_ASSERT (NULL != m_pContainerRAM6555_2n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM6555_2n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5555_2n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5555_2n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5554_2n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5554_2n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5454_2n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5454_2n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5444_2n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5444_2n)");
	BTREE_ASSERT (NULL != m_pContainerRAM4444_2n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM4444_2n)");
	BTREE_ASSERT (NULL != m_pContainerRAM6565_4n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM6565_4n)");
	BTREE_ASSERT (NULL != m_pContainerRAM6555_4n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM6555_4n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5555_4n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5555_4n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5554_4n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5554_4n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5454_4n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5454_4n)");
	BTREE_ASSERT (NULL != m_pContainerRAM5444_4n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5444_4n)");
	BTREE_ASSERT (NULL != m_pContainerRAM4444_4n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM4444_4n)");
	BTREE_ASSERT (NULL != m_pContainerFile6565min, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile6565min)");
	BTREE_ASSERT (NULL != m_pContainerFile6555min, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile6555min)");
	BTREE_ASSERT (NULL != m_pContainerFile5555min, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5555min)");
	BTREE_ASSERT (NULL != m_pContainerFile5554min, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5554min)");
	BTREE_ASSERT (NULL != m_pContainerFile5454min, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5454min)");

	BTREE_ASSERT (NULL != m_pContainerFile6565default, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile6565default)");
	BTREE_ASSERT (NULL != m_pContainerFile6555default, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile6555default)");
	BTREE_ASSERT (NULL != m_pContainerFile5555default, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5555default)");
	BTREE_ASSERT (NULL != m_pContainerFile5554default, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5554default)");
	BTREE_ASSERT (NULL != m_pContainerFile5454default, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5454default)");

	BTREE_ASSERT (NULL != m_pContainerFile6565large, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile6565large)");
	BTREE_ASSERT (NULL != m_pContainerFile6555large, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile6555large)");
	BTREE_ASSERT (NULL != m_pContainerFile5555large, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5555large)");
	BTREE_ASSERT (NULL != m_pContainerFile5554large, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5554large)");
	BTREE_ASSERT (NULL != m_pContainerFile5454large, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5454large)");
}

template<class _t_data, class _t_value, class _t_sizetype, class _t_ref_container>
void CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::init_containers (const CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container> &rWrapper)
{
	uint32_t	i = 0;

	const CBTreeKeySortTestWrapper_t		&rThisWrapper = dynamic_cast<const CBTreeKeySortTestWrapper_t &> (rWrapper);

	m_pContainerRAM6565_n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (*rThisWrapper.m_pContainerRAM6565_n); i++;
	m_pContainerRAM6555_n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (*rThisWrapper.m_pContainerRAM6555_n); i++;
	m_pContainerRAM5555_n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (*rThisWrapper.m_pContainerRAM5555_n); i++;
	m_pContainerRAM5554_n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (*rThisWrapper.m_pContainerRAM5554_n); i++;
	m_pContainerRAM5454_n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (*rThisWrapper.m_pContainerRAM5454_n); i++;
	m_pContainerRAM5444_n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> > (*rThisWrapper.m_pContainerRAM5444_n); i++;
	m_pContainerRAM4444_n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> > (*rThisWrapper.m_pContainerRAM4444_n); i++;
	m_pContainerRAM6565_2n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (*rThisWrapper.m_pContainerRAM6565_2n); i++;
	m_pContainerRAM6555_2n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (*rThisWrapper.m_pContainerRAM6555_2n); i++;
	m_pContainerRAM5555_2n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (*rThisWrapper.m_pContainerRAM5555_2n); i++;
	m_pContainerRAM5554_2n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (*rThisWrapper.m_pContainerRAM5554_2n); i++;
	m_pContainerRAM5454_2n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (*rThisWrapper.m_pContainerRAM5454_2n); i++;
	m_pContainerRAM5444_2n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> > (*rThisWrapper.m_pContainerRAM5444_2n); i++;
	m_pContainerRAM4444_2n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> > (*rThisWrapper.m_pContainerRAM4444_2n); i++;
	m_pContainerRAM6565_4n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (*rThisWrapper.m_pContainerRAM6565_4n); i++;
	m_pContainerRAM6555_4n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (*rThisWrapper.m_pContainerRAM6555_4n); i++;
	m_pContainerRAM5555_4n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (*rThisWrapper.m_pContainerRAM5555_4n); i++;
	m_pContainerRAM5554_4n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (*rThisWrapper.m_pContainerRAM5554_4n); i++;
	m_pContainerRAM5454_4n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (*rThisWrapper.m_pContainerRAM5454_4n); i++;
	m_pContainerRAM5444_4n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> > (*rThisWrapper.m_pContainerRAM5444_4n); i++;
	m_pContainerRAM4444_4n = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> > (*rThisWrapper.m_pContainerRAM4444_4n); i++;
	m_pContainerFile6565min = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (*rThisWrapper.m_pContainerFile6565min); i++;
	m_pContainerFile6555min = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (*rThisWrapper.m_pContainerFile6555min); i++;
	m_pContainerFile5555min = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (*rThisWrapper.m_pContainerFile5555min); i++;
	m_pContainerFile5554min = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (*rThisWrapper.m_pContainerFile5554min); i++;
	m_pContainerFile5454min = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (*rThisWrapper.m_pContainerFile5454min); i++;

	m_pContainerFile6565default = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (*rThisWrapper.m_pContainerFile6565default); i++;
	m_pContainerFile6555default = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (*rThisWrapper.m_pContainerFile6555default); i++;
	m_pContainerFile5555default = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (*rThisWrapper.m_pContainerFile5555default); i++;
	m_pContainerFile5554default = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (*rThisWrapper.m_pContainerFile5554default); i++;
	m_pContainerFile5454default = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (*rThisWrapper.m_pContainerFile5454default); i++;

	m_pContainerFile6565large = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> > (*rThisWrapper.m_pContainerFile6565large); i++;
	m_pContainerFile6555large = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> > (*rThisWrapper.m_pContainerFile6555large); i++;
	m_pContainerFile5555large = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> > (*rThisWrapper.m_pContainerFile5555large); i++;
	m_pContainerFile5554large = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> > (*rThisWrapper.m_pContainerFile5554large); i++;
	m_pContainerFile5454large = new CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> > (*rThisWrapper.m_pContainerFile5454large); i++;

	BTREE_ASSERT (i == this->get_num_containers (), "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::init_container (const CBTreeAssociativeTestWrapper &): ERROR: Unexpected number of test containers instantiated!");

	i = 0;

	BTREE_ASSERT (NULL != m_pContainerRAM6565_n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM6565_n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM6555_n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM6555_n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5555_n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5555_n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5554_n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5554_n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5454_n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5454_n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5444_n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5444_n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM4444_n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM4444_n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM6565_2n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM6565_2n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM6555_2n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM6555_2n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5555_2n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5555_2n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5554_2n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5554_2n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5454_2n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5454_2n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5444_2n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5444_2n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM4444_2n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM4444_2n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM6565_4n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM6565_4n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM6555_4n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM6555_4n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5555_4n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5555_4n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5554_4n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5554_4n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5454_4n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5454_4n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM5444_4n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM5444_4n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerRAM4444_4n, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerRAM4444_4n)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile6565min, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile6565min)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile6555min, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile6555min)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5555min, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5555min)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5554min, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5554min)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5454min, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5454min)"); i++;

	BTREE_ASSERT (NULL != m_pContainerFile6565default, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile6565default)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile6555default, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile6555default)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5555default, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5555default)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5554default, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5554default)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5454default, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5454default)"); i++;

	BTREE_ASSERT (NULL != m_pContainerFile6565large, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile6565large)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile6555large, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile6555large)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5555large, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5555large)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5554large, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5554large)"); i++;
	BTREE_ASSERT (NULL != m_pContainerFile5454large, "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeKeySortTestWrapper: ERROR: insufficient memory! (m_pContainerFile5454large)"); i++;

	BTREE_ASSERT (i == this->get_num_containers (), "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::init_container (const CBTreeAssociativeTestWrapper &): ERROR: Unexpected number of test containers checked to be instantiated successfully!");

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

	BTREE_ASSERT (i == this->get_num_containers (), "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::init_container (const CBTreeAssociativeTestWrapper &): ERROR: Unexpected number of test containers after setting reference container!");
}

template<class _t_data, class _t_value, class _t_sizetype, class _t_ref_container>
void CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::transfer_containers ()
{
	uint32_t		i = 0;

	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM6565_n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM6555_n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM5555_n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM5554_n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM5454_n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM5444_n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM4444_n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM6565_2n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM6555_2n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM5555_2n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM5554_2n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM5454_2n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM5444_2n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM4444_2n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM6565_4n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM6555_4n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM5555_4n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM5554_4n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM5454_4n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM5444_4n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerRAM4444_4n); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerFile6565min); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerFile6555min); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerFile5555min); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerFile5554min); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerFile5454min); i++;

	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerFile6565default); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerFile6555default); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerFile5555default); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerFile5554default); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerFile5454default); i++;

	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerFile6565large); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerFile6555large); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerFile5555large); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerFile5554large); i++;
	this->m_ppContainers[i] = dynamic_cast<CBTreeAssociativeIf_t *> (m_pContainerFile5454large); i++;

	BTREE_ASSERT (i == this->get_num_containers (), "CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::CBTreeMapTestWrapper: ERROR: Unexpected number of test containers!");
}
	
template<class _t_data, class _t_value, class _t_sizetype, class _t_ref_container>
const uint32_t CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>::m_nNumContainersKS = 36;

#endif // BTREEKEYSORTTESTWRAPPER_CPP
