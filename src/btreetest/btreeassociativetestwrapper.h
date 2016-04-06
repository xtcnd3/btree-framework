/************************************************************
**
** file:	btreeassociativetestwrapper.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains the declaration of the associative container
** test class wrapping abstract accesses to associative container test class
** and a template based reference container instance, while making sure the
** associative container instance acts the same way as the abstract reference.
**
************************************************************/

#ifndef BTREEASSOCIATIVETESTWRAPPER_H
#define BTREEASSOCIATIVETESTWRAPPER_H

#include <stdint.h>

#include <sstream>

#include "btreeassociative.h"

#include "btreetestkeysort.h"

template <class _t_data, class _t_value, class _t_ref_container>
class CBTreeAssociativeTestWrapper
{
public:

	typedef CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>
																CBTreeAssociativeTestWrapper_t;
	
	typedef _t_ref_container									reference_t;

	typedef typename reference_t::iterator						iterator;
	typedef typename reference_t::const_iterator				const_iterator;
	typedef typename reference_t::reverse_iterator				reverse_iterator;
	typedef typename reference_t::const_reverse_iterator		const_reverse_iterator;

	typedef _t_data												data_t;
	
	typedef typename reference_t::value_type					value_type;

	typedef typename reference_t::key_type						key_type;

	typedef typename reference_t::size_type						sizetype_ref_t;

	typedef sizetype_ref_t										size_type;

	typedef uint64_t											sizetype_test_t;

	typedef CBTreeAssociativeIf<_t_value, uint32_t, sizetype_test_t>
																CBTreeAssociativeIf_t;

	typedef typename CBTreeAssociativeIf_t::iterator			test_iterator;

	typedef typename CBTreeAssociativeIf_t::const_iterator		test_const_iterator;

	typedef typename CBTreeAssociativeIf_t::reverse_iterator	test_reverse_iterator;

	typedef typename CBTreeAssociativeIf_t::const_reverse_iterator
																test_const_reverse_iterator;

							CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>
																(const uint32_t nNodeSize, const uint32_t nPageSize);

							CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>
																(const CBTreeAssociativeTestWrapper &rContainer);

	virtual					~CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container>
																();

	sizetype_ref_t			size								() const;

	void					clear								();

	iterator				begin								();
	iterator				end									();
	
	reverse_iterator		rbegin								();
	reverse_iterator		rend								();

	const_iterator			cbegin								() const;
	const_iterator			cend								() const;

	const_reverse_iterator	crbegin								() const;
	const_reverse_iterator	crend								() const;

	void					swap								(CBTreeAssociativeTestWrapper<_t_data, _t_value, _t_ref_container> &rContainer);

//	template<class _t_iterator>
//	void					insert								(_t_iterator sIterFirst, _t_iterator sIterLast);
	iterator				insert								(const value_type &rData);

//	template<class _t_iterator, class _t_test_iterator>
//	void					insert_self_reference				(_t_iterator sIterFirst, _t_iterator sIterLast, _t_test_iterator &rIterTest);

	iterator				erase								(const_iterator sCIterPos);
	sizetype_ref_t			erase								(key_type &rKey);
	iterator				erase								(const_iterator sCIterFirst, const_iterator sCIterLast);

	iterator				find								(key_type &rKey);
	const_iterator			find								(key_type &rKey) const;

	iterator				lower_bound							(key_type &rKey);
	const_iterator			lower_bound							(key_type &rKey) const;

	iterator				upper_bound							(key_type &rKey);
	const_iterator			upper_bound							(key_type &rKey) const;

	sizetype_ref_t			count								(key_type &rKey) const;

	sizetype_test_t			serialize							(const sizetype_test_t nStart, const sizetype_test_t nLen, value_type *pData) const;

	void					unload								();

	void					show_integrity						(const char *pszPrefix);

	CBTreeAssociativeTestWrapper &
							operator=							(const CBTreeAssociativeTestWrapper &rContainer);

	bool					operator==							(const CBTreeAssociativeTestWrapper &rContainer) const;
	bool					operator!=							(const CBTreeAssociativeTestWrapper &rContainer) const;

protected:

	void					test								() const;

	void					disable_atomic_testing				() const;
	void					enable_atomic_testing				() const;

	void					instantiate_reference_container		();

	uint32_t				get_num_containers					() const;

	virtual void			init_containers						(const uint32_t nNodeSize, const uint32_t nPageSize) = 0;

	virtual void			init_containers						(const CBTreeAssociativeTestWrapper &rWrapper) = 0;

	virtual void			transfer_containers					() = 0;

	uint32_t							m_nNodeSize;
	uint32_t							m_nPageSize;

	uint32_t							m_nNumContainers;
	CBTreeAssociativeIf_t				**m_ppContainers;

	reference_t							*m_pReference;
};

#include "btreeassociativetestwrapper.cpp"

#endif // BTREEASSOCIATIVETESTWRAPPER_H
