/************************************************************
**
** file:	btreetestkeysort.h
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

#ifndef	BTREETESTKEYSORT_H
#define	BTREETESTKEYSORT_H

#include <stdint.h>

#include <map>
#include <list>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "testbench/common/btreetestcommon.h"

#include "btreekeysort.h"

template<class _t_key>
class multiMapMemCmp
{
public:

	bool	operator () (const _t_key &rLhs, const _t_key &rRhs)
	{
		return (memcmp ((const void *) &rLhs, (const void *) &rRhs, sizeof (rLhs)) < 0);
	};
};

typedef struct keySortEntry_s
{
	uint32_t		nKey;
	uint32_t		nData;
	uint32_t		nDebug;

	operator		const uint32_t ()
	{
		return (nKey);
	}

	bool	operator== (const struct keySortEntry_s &rRhs) const
	{
		return ((this->nKey == rRhs.nKey) && (this->nData == rRhs.nData));
	}

	bool	operator!= (const struct keySortEntry_s &rRhs) const
	{
		return ((this->nKey != rRhs.nKey) || (this->nData != rRhs.nData));
	}
} keySortEntry_t;

typedef struct keySortMap_s
{
	uint32_t		nData;
	uint32_t		nDebug;

	bool	operator== (const struct keySortMap_s &rRhs) const
	{
		return (this->nData == rRhs.nData);
	}

	bool	operator!= (const struct keySortMap_s &rRhs) const
	{
		return (this->nData != rRhs.nData);
	}
} keySortMap_t;

/************************************************************************************/

template<class _t_iterator, class _t_data>
struct btree_key_sort_test_insert_reference_via_iterator_arbiter
{
	static void btree_key_sort_test_insert_reference_via_iterator (_t_iterator &sItFirst, _t_iterator &sItLast, typename ::std::multimap<const uint32_t, keySortMap_t> *pClRefData)
	{
		pClRefData->insert<_t_iterator> (sItFirst, sItLast);
	}
};

template<class _t_iterator>
struct btree_key_sort_test_insert_reference_via_iterator_arbiter<_t_iterator, keySortEntry_t>
{
	static void btree_key_sort_test_insert_reference_via_iterator (_t_iterator &sItFirst, _t_iterator &sItLast, typename ::std::multimap<const uint32_t, keySortMap_t> *pClRefData)
	{
		::std::pair<uint32_t, keySortMap_t>		sValue;
		_t_iterator								sIt;

		for (sIt = sItFirst; sIt != sItLast; sIt++)
		{
			sValue.first = ((keySortEntry_t) (*sIt)).nKey;
			sValue.second.nData = ((keySortEntry_t) (*sIt)).nData;
			sValue.second.nDebug = ((keySortEntry_t) (*sIt)).nDebug;

			pClRefData->insert (sValue);
		}
	}
};

/************************************************************************************/

template<class _t_data, class _t_key, class _t_datalayerproperties = CBTreeIOpropertiesRAM <> >
class CBTreeKeySortTest
	:	public CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>
{
public:

	typedef _t_data													value_type;
	typedef	keySortMap_t											map_type;
	typedef _t_key													key_type;
	typedef typename _t_datalayerproperties::size_type				size_type;
	typedef typename _t_datalayerproperties::node_iter_type			node_iter_type;
	typedef typename _t_datalayerproperties::sub_node_iter_type		sub_node_iter_type;
	typedef _t_datalayerproperties									data_layer_properties_type;
	typedef typename _t_datalayerproperties::data_layer_type		data_layer_type;
	
	typedef value_type&												reference;
	typedef const value_type&										const_reference;
	typedef value_type*												pointer;
	typedef const value_type*										const_pointer;
	typedef	typename ::std::make_signed<size_type>::type			difference_type;

	typedef CBTreeKeySortTest										CBTreeKeySortTest_t;

	typedef CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>	CBTreeKeySort_t;

	typedef typename CBTreeKeySort_t::iterator						iterator;
	typedef typename CBTreeKeySort_t::const_iterator				const_iterator;
	typedef typename CBTreeKeySort_t::reverse_iterator				reverse_iterator;
	typedef typename CBTreeKeySort_t::const_reverse_iterator		const_reverse_iterator;

	typedef ::std::multimap<key_type, keySortMap_t>					reference_t;

						CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>
													(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, sub_node_iter_type nNodeSize, reference_t *pClRefData);

						CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>
													(CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties> &rBT, bool bAssign = true);

	virtual				~CBTreeKeySortTest<_t_data, _t_key, _t_datalayerproperties>
													();

	template<class _t_iterator>
	void				insert						(_t_iterator sItFirst, _t_iterator sItLast);
	iterator			insert						(const value_type &rData);

	iterator			erase						(const_iterator sCIterPos);
	size_type			erase						(const key_type &rKey);
	iterator			erase						(const_iterator sCIterFirst, const_iterator sCIterLast);

	void				swap						(CBTreeKeySortTest &rKeySort);

	iterator			find						(const key_type &rKey);

	iterator			lower_bound					(const key_type &rKey);
	const_iterator		lower_bound					(const key_type &rKey) const;

	iterator			upper_bound					(const key_type &rKey);
	const_iterator		upper_bound					(const key_type &rKey) const;

	void				clear						();

	size_type			count						(const key_type &rKey) const;
	
	value_type			get_data_reference			(const_iterator &rCIter);
	value_type			get_data_reference			(const_reverse_iterator &rCRIter);
	
	CBTreeKeySortTest	&operator=					(const CBTreeKeySortTest &rBT);

	void				test						() const;

	void				set_reference				(reference_t *pReference);

	void				set_atomic_testing			(bool bEnable);

protected:

	int					comp						(const key_type &rKey0, const key_type &rKey1) const;

	_t_key				*extract_key				(key_type *pKey, const node_iter_type nNode, const sub_node_iter_type nEntry) const;

	virtual _t_key		*extract_key				(key_type *pKey, const value_type &rData) const;

	template<class _t_iterator>
	void				insert_via_iterator			(_t_iterator sItFirst, _t_iterator sItLast)
	{
		CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>::insert (sItFirst, sItLast);
	}

	template<class _t_iterator, class _t_ref_iterator>
	_t_data				get_data_reference			(_t_iterator &rIter);

	bool				show_data					(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const node_iter_type nNode, const sub_node_iter_type nSubPos) const;

	reference_t			*m_pClRefData;

	bool				m_bAtomicTesting;
};

template<class _t_datalayerproperties>
class CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>
	:	public CBTreeKeySort<keySortEntry_t, uint32_t, _t_datalayerproperties>
{
public:

	typedef keySortEntry_t											value_type;
	typedef	keySortMap_t											map_type;
	typedef uint32_t												key_type;
	typedef typename _t_datalayerproperties::size_type				size_type;
	typedef typename _t_datalayerproperties::node_iter_type			node_iter_type;
	typedef typename _t_datalayerproperties::sub_node_iter_type		sub_node_iter_type;
	typedef _t_datalayerproperties									data_layer_properties_type;
	typedef typename _t_datalayerproperties::data_layer_type		data_layer_type;
	
	typedef CBTreeKeySortTest										CBTreeKeySortTest_t;

	typedef CBTreeKeySort<keySortEntry_t, uint32_t, _t_datalayerproperties>
																	CBTreeKeySort_t;

	typedef typename CBTreeKeySort_t::iterator						iterator;
	typedef typename CBTreeKeySort_t::const_iterator				const_iterator;
	typedef typename CBTreeKeySort_t::reverse_iterator				reverse_iterator;
	typedef typename CBTreeKeySort_t::const_reverse_iterator		const_reverse_iterator;

	typedef ::std::multimap<key_type, map_type>						reference_t;

						CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>
													(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, sub_node_iter_type nNodeSize, reference_t *pClRefData);

						CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties>
													(CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_datalayerproperties> &rBT, bool bAssign = true);

	virtual				~CBTreeKeySortTest <keySortEntry_t, uint32_t, _t_datalayerproperties>
													();

	template<class _t_iterator>
	void				insert						(_t_iterator sItFirst, _t_iterator sItLast);
	iterator			insert						(const value_type &rData);

	iterator			erase						(const_iterator sCIterPos);
	size_type			erase						(const key_type &rKey);
	iterator			erase						(const_iterator sCIterFirst, const_iterator sCIterLast);

	void				swap						(CBTreeKeySortTest &rKeySort);

	iterator			find						(const key_type &rKey);
	
	iterator			lower_bound					(const key_type &rKey);
	const_iterator		lower_bound					(const key_type &rKey) const;

	iterator			upper_bound					(const key_type &rKey);
	const_iterator		upper_bound					(const key_type &rKey) const;
	
	value_type			get_data_reference			(const_iterator &rCIter);
	value_type			get_data_reference			(const_reverse_iterator &rCRIter);
	
	void				clear						();

	CBTreeKeySortTest	&operator=					(const CBTreeKeySortTest &rBT);

//	bool				operator==					(const CBTreeKeySortTest &rKeySort) const;
//	bool				operator!=					(const CBTreeKeySortTest &rKeySort) const;

	void				test						() const;

	void				set_reference				(reference_t *pReference);

	void				set_atomic_testing			(bool bEnable);

protected:

	int					comp						(const key_type &rKey0, const key_type &rKey1) const;

	template<class _t_iterator>
	void				insert_via_iterator			(_t_iterator sItFirst, _t_iterator sItLast)
	{
		CBTreeKeySort<value_type, key_type, _t_datalayerproperties>::insert (sItFirst, sItLast);
	}

	template<class _t_iterator, class _t_ref_iterator>
	value_type			get_data_reference			(_t_iterator &rIter);

	bool				show_data					(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const node_iter_type nNode, const sub_node_iter_type nSubPos) const;

	reference_t			*m_pClRefData;

	bool				m_bAtomicTesting;
};

#endif // BTREETESTKEYSORT_H

#include "btreetestkeysort.cpp"
