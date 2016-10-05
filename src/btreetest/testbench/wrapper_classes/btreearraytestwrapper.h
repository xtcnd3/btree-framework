/************************************************************
**
** file:	btreearraytestwrapper.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** 
**
************************************************************/

#ifndef	BTREEARRAYTESTWRAPPER_H
#define	BTREEARRAYTESTWRAPPER_H

#include <stdint.h>

#include "btreeiofile.h"
#include "btreeioram.h"

#include "testbench/application_classes/regression/btreetestarray.h"

template<class _t_container, class _t_ref_container>
class CBTreeArrayConstTestAccessWrapper;

template<class _t_container, class _t_ref_container>
class CBTreeArrayTestAccessWrapper;

template<class _t_data, class _t_sizetype, class _t_ref_container>
class CBTreeArrayTestWrapper
{
public:

	typedef CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>	CBTreeArrayTestWrapper_t;

	typedef _t_ref_container									reference_t;

	typedef typename reference_t::iterator						iterator;
	typedef typename reference_t::const_iterator				const_iterator;
	typedef typename reference_t::reverse_iterator				reverse_iterator;
	typedef typename reference_t::const_reverse_iterator		const_reverse_iterator;

	typedef typename reference_t::value_type					value_type;
	typedef typename reference_t::size_type						size_type;

	typedef _t_data												value_test_type;
	typedef _t_sizetype											size_test_type;

	typedef	CBTreeArrayIf<value_test_type, size_test_type>		CBTreeArrayIf_t;

	typedef typename CBTreeArrayIf_t::difference_type			difference_type;

	typedef typename CBTreeArrayIf_t::iterator					test_iterator;
	typedef typename CBTreeArrayIf_t::const_iterator			test_const_iterator;
	typedef typename CBTreeArrayIf_t::reverse_iterator			test_reverse_iterator;
	typedef typename CBTreeArrayIf_t::const_reverse_iterator	test_const_reverse_iterator;

	typedef CBTreeArrayTestAccessWrapper<CBTreeArrayIf_t, _t_ref_container>
																CBTreeArrayTestAccessWrapper_t;

	// construction
								CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>
																(const uint32_t nNodeSize, const uint32_t nPageSize);

								CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>
																(const CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container> &rContainer);

	// destruction
								~CBTreeArrayTestWrapper<_t_data, _t_sizetype, _t_ref_container>
																();

	bool						empty							() const;
	size_type					size							() const;

	const_iterator				cbegin							() const;
	const_iterator				cend							() const;
	const_reverse_iterator		crbegin							() const;
	const_reverse_iterator		crend							() const;
	
	iterator					begin							();
	iterator					end								();
	reverse_iterator			rbegin							();
	reverse_iterator			rend							();
	
	const_iterator				begin							() const;
	const_iterator				end								() const;
	const_reverse_iterator		rbegin							() const;
	const_reverse_iterator		rend							() const;

	template<class _t_iterator>
	void						assign							(_t_iterator &rItFirst, _t_iterator &rItLast);

	template<class _t_iterator>
	void						assign_via_self_reference		(_t_iterator &rItFirst, _t_iterator &rItLast);

	void						assign							(size_type nNewSize, const value_type& rVal);

	void						push_back						(const value_type &rData);

	void						pop_back						();

	iterator					insert							(const_iterator sCIterPos, const value_type& rData);
	iterator					insert							(const_iterator sCIterPos, const size_type nLen, const value_type& rData);

	template<class _t_iterator>
	iterator					insert							(const_iterator sCIterPos, _t_iterator &rItFirst, _t_iterator &rItLast);

	template<class _t_iterator>
	void						insert_via_self_reference		(const_iterator sCIterPos, _t_iterator &rItFirst, _t_iterator &rItLast);

	iterator					erase							(const_iterator sCIterPos);
	iterator					erase							(const_iterator sCIterFirst, const_iterator sCIterLast);

	void						replace							(const_iterator sCIterPos, const _t_data& rData);

	void						swap							(CBTreeArrayTestWrapper_t &rArrayWrapper);

	void						clear							();

	size_type					serialize						(const size_type nStart, const size_type nLen, value_type *pData) const;

	void						unload							();

	void						show_integrity					(const char *pszPrefix);

	CBTreeArrayTestWrapper_t&	operator=						(const CBTreeArrayTestWrapper_t &rContainer);

	CBTreeArrayTestWrapper_t&	operator=						(CBTreeArrayTestWrapper_t &&rRhBT);

	CBTreeArrayTestAccessWrapper_t
								operator[]						(const size_type nPos);

	const value_test_type&		operator[]						(const size_type nPos) const;
	
	bool						operator==						(const CBTreeArrayTestWrapper_t &rContainer) const;
	bool						operator!=						(const CBTreeArrayTestWrapper_t &rContainer) const;

	void						skip_containers_with_limited_address_space
																(const bool bEnable);

protected:

	void						test							() const;

	void						disable_atomic_testing			() const;
	void						enable_atomic_testing			() const;

	bool						is_limited_container			(uint32_t nContainer) const;

	bool						skip_container					(uint32_t nContainer) const;

	void						instantiate_container_array		();

	uint32_t					get_num_containers				() const;

	void						init_containers					(const uint32_t nNodeSize, const uint32_t nPageSize);

	void						init_containers					(const CBTreeArrayTestWrapper_t &rWrapper);

	void						transfer_containers				();

	template<class _t_container, class _t_iterator>
	void						test_insert_all_containers		(_t_container *pContainer, const size_type nDist, _t_iterator &sItFirst, _t_iterator &sItLast);

	template<class _t_iterator>
	void						assign_via_self_reference_all	(_t_iterator &rIter, const size_test_type nFirst, const size_test_type nLast);

	template<class _t_container, class _t_iterator>
	void						assign_via_self_reference_one	(_t_container *pContainer, _t_iterator &rIter, const size_test_type nFirst, const size_test_type nLast);

	template<class _t_iterator>
	void						insert_via_self_reference_all	(_t_iterator &rIter, const size_test_type nPos, const size_test_type nFirst, const size_test_type nLast);

	template<class _t_container, class _t_iterator>
	void						insert_via_self_reference_one	(_t_container *pContainer, _t_iterator &rIter, const size_test_type nPos, const size_test_type nFirst, const size_test_type nLast);

	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerRAM6565_n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM6555_n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM5555_n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerRAM5554_n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerRAM5454_n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM5444_n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM4444_n;

	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerRAM6565_2n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM6555_2n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM5555_2n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerRAM5554_2n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerRAM5454_2n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM5444_2n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM4444_2n;

	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerRAM6565_4n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM6555_4n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerRAM5555_4n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerRAM5554_4n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerRAM5454_4n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint32_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM5444_4n;
	CBTreeTestArray<CBTreeIOpropertiesRAM <size_test_type, uint16_t, uint16_t, uint16_t, uint16_t> >		*m_pContainerRAM4444_4n;

	CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerFile6565min;
	CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile6555min;
	CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile5555min;
	CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerFile5554min;
	CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerFile5454min;

	CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerFile6565default;
	CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile6555default;
	CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile5555default;
	CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerFile5554default;
	CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerFile5454default;

	CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint64_t, uint32_t> >		*m_pContainerFile6565large;
	CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint64_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile6555large;
	CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint32_t> >		*m_pContainerFile5555large;
	CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint32_t, uint32_t, uint16_t> >		*m_pContainerFile5554large;
	CBTreeTestArray<CBTreeIOpropertiesFile <size_test_type, uint32_t, uint16_t, uint32_t, uint16_t> >		*m_pContainerFile5454large;

	reference_t							*m_pReference;

	CBTreeArrayIf_t						**m_ppContainers;

	uint32_t							m_nNodeSize;
	uint32_t							m_nPageSize;

	uint32_t							m_nNumContainers;

	bool								m_bSkipLimitedContainers;

	CBTreeArrayTestAccessWrapper_t		*m_pClAccessWrapper;

	value_type							*m_psReturnData;

	static const uint32_t				m_nNumArrays;

public:

	friend class CBTreeArrayConstTestAccessWrapper<CBTreeArrayIf_t, _t_ref_container>;
	friend class CBTreeArrayTestAccessWrapper<CBTreeArrayIf_t, _t_ref_container>;
};

template<class _t_container, class _t_ref_container>
class CBTreeArrayConstTestAccessWrapper
{
public:

	typedef typename _t_ref_container::const_iterator	const_iterator;

	typedef typename _t_container::size_type			size_test_type;
	typedef typename _t_container::value_type			value_test_type;

						CBTreeArrayConstTestAccessWrapper<_t_container, _t_ref_container>
						(
							CBTreeArrayTestWrapper<value_test_type, size_test_type, _t_ref_container>	&rInstance
						);

						~CBTreeArrayConstTestAccessWrapper<_t_container, _t_ref_container> ();

	void				set								(const size_test_type nPos);

	value_test_type &	operator*						();
	value_test_type &	operator->						();

						operator const value_test_type&	();

protected:

	CBTreeArrayTestWrapper<value_test_type, size_test_type, _t_ref_container>
																&m_rInstance;
	size_test_type												m_nPos;
	value_test_type												m_sData;
};

template<class _t_container, class _t_ref_container>
class CBTreeArrayTestAccessWrapper
	:	public CBTreeArrayConstTestAccessWrapper<_t_container, _t_ref_container>
{
public:

	typedef typename _t_ref_container::iterator			iterator;
	typedef typename _t_ref_container::const_iterator	const_iterator;

	typedef typename _t_container::size_type			size_test_type;
	typedef typename _t_container::value_type			value_test_type;

	
						CBTreeArrayTestAccessWrapper<_t_container, _t_ref_container>
						(
							CBTreeArrayTestWrapper<value_test_type, size_test_type, _t_ref_container>	&rInstance
						);

						~CBTreeArrayTestAccessWrapper<_t_container, _t_ref_container> ();

	CBTreeArrayTestAccessWrapper<_t_container, _t_ref_container> &
						operator=					(const value_test_type &rData);

	CBTreeArrayTestAccessWrapper<_t_container, _t_ref_container> &
						operator=					(const CBTreeArrayTestAccessWrapper<_t_container, _t_ref_container> &rData);

};

#include "btreearraytestwrapper.cpp"

#endif // BTREEARRAYTESTWRAPPER_H
