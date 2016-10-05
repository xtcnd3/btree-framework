/************************************************************
**
** file:	btreekeysorttestwrapper.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains the declaration of a key sort test class 
** wrapping accesses to a key sort application class instance and
** a reference container instance, while making sure the
** key sort instance acts the same way as the reference.
**
************************************************************/

#ifndef BTREEKEYSORTTESTWRAPPER_H
#define BTREEKEYSORTTESTWRAPPER_H

#include <sstream>
#include <vector>

#include <stdint.h>

#include "btreeiofile.h"
#include "btreeioram.h"

#include "btreearray.h"
#include "testbench/application_classes/regression/btreetestkeysort.h"

#include "testbench/wrapper_classes/base/btreeassociativetestwrapper.h"

#include "testbench/primitives/btreekeysorttestprimitive.h"

template<class _t_data, class _t_value, class _t_sizetype, class _t_ref_container>
class CBTreeKeySortTestWrapper	:	public CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>
{
public:

	typedef CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>
																CBTreeKeySortTestWrapper_t;

	typedef CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>
																CBTreeAssociativeTestWrapper_t;

	typedef _t_ref_container									reference_t;

	typedef typename reference_t::iterator						iterator;
	typedef typename reference_t::const_iterator				const_iterator;
	typedef typename reference_t::reverse_iterator				reverse_iterator;
	typedef typename reference_t::const_reverse_iterator		const_reverse_iterator;

	typedef typename reference_t::value_type					value_type;
	typedef typename reference_t::key_type						key_type;
	typedef typename reference_t::size_type						size_type;

	typedef _t_data												value_test_type;
	typedef uint32_t											key_test_type;
	typedef _t_sizetype											size_test_type;

	typedef CBTreeAssociativeIf<_t_value, uint32_t, size_test_type>
																CBTreeAssociativeIf_t;

	typedef typename CBTreeAssociativeIf_t::iterator			test_iterator;

	typedef typename CBTreeAssociativeIf_t::const_iterator		test_const_iterator;

	typedef typename CBTreeAssociativeIf_t::reverse_iterator	test_reverse_iterator;

	typedef typename CBTreeAssociativeIf_t::const_reverse_iterator
																test_const_reverse_iterator;

							CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>
																(const uint32_t nNodeSize, const uint32_t nPageSize);

							CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>
																(const CBTreeKeySortTestWrapper &rContainer);

							~CBTreeKeySortTestWrapper<_t_data, _t_value, _t_sizetype, _t_ref_container>
																();

	template<class _t_iterator>
	void					insert								(_t_iterator sIterFirst, _t_iterator sIterLast);
	iterator				insert								(const value_type &rData);
	
	template<class _t_iterator, class _t_test_iterator>
	void					insert_self_reference				(_t_iterator sIterFirst, _t_iterator sIterLast, _t_test_iterator &rIterTest);

protected:

	void					init_containers						(const uint32_t nNodeSize, const uint32_t nPageSize);

	void					init_containers						(const CBTreeAssociativeTestWrapper_t &rWrapper);

	void					transfer_containers					();

	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> >	*m_pContainerFile6565min;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> >	*m_pContainerFile6555min;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> >	*m_pContainerFile5555min;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> >	*m_pContainerFile5554min;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> >	*m_pContainerFile5454min;

	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> >	*m_pContainerFile6565default;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> >	*m_pContainerFile6555default;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> >	*m_pContainerFile5555default;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> >	*m_pContainerFile5554default;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> >	*m_pContainerFile5454default;

	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> >	*m_pContainerFile6565large;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> >	*m_pContainerFile6555large;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> >	*m_pContainerFile5555large;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> >	*m_pContainerFile5554large;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> >	*m_pContainerFile5454large;

	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerRAM6565_n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM6555_n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM5555_n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerRAM5554_n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerRAM5454_n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM5444_n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM4444_n;

	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerRAM6565_2n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM6555_2n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM5555_2n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerRAM5554_2n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerRAM5454_2n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM5444_2n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM4444_2n;

	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerRAM6565_4n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM6555_4n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM5555_4n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerRAM5554_4n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerRAM5454_4n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM5444_4n;
	CBTreeKeySortTest<value_test_type, uint32_t, CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM4444_4n;

	static const uint32_t											m_nNumContainersKS;
};

#include "btreekeysorttestwrapper.cpp"

#endif // BTREEKEYSORTTESTWRAPPER_H
