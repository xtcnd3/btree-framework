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

#include "btreetestcommon.h"

#include "btreekeysort.h"

template <class _t_key>
class multiMapMemCmp
{
public:

	bool	operator () (const _t_key &rLhs, const _t_key &rRhs)
	{
		return (memcmp ((const void *) &rLhs, (const void *) &rRhs, sizeof (rLhs)) < 0);
	};
};

typedef struct
{
	uint32_t		nKey;
	uint32_t		nData;
	uint32_t		nDebug;

	operator		const uint32_t ()
	{
		return (nKey);
	}
} keySortEntry_t;

typedef struct
{
	uint32_t		nData;
	uint32_t		nDebug;
} keySortMap_t;

/************************************************************************************/

template <class _t_iterator, class _t_data>
struct btree_key_sort_test_insert_reference_via_iterator_arbiter
{
	static void btree_key_sort_test_insert_reference_via_iterator (_t_iterator &sItFirst, _t_iterator &sItLast, typename ::std::multimap<const uint32_t, keySortMap_t> *pClRefData)
	{
		pClRefData->insert<_t_iterator> (sItFirst, sItLast);
	}
};

template <class _t_iterator>
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

template <class _t_data, class _t_key, class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter, uint64_t, uint32_t> >
class CBTreeKeySortTest
	:	public CBTreeKeySort <_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

	typedef CBTreeKeySortTest									CBTreeKeySortTest_t;

	typedef CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
																CBTreeKeySort_t;

	typedef typename CBTreeKeySort_t::iterator					iterator;
	typedef typename CBTreeKeySort_t::const_iterator			const_iterator;
	typedef typename CBTreeKeySort_t::reverse_iterator			reverse_iterator;
	typedef typename CBTreeKeySort_t::const_reverse_iterator	const_reverse_iterator;

	typedef ::std::multimap<const _t_key, keySortMap_t>			reference_t;

						CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize, reference_t *pClRefData);

						CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign = true);

	virtual				~CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													();

	template <class _t_iterator>
	void				insert						(_t_iterator sItFirst, _t_iterator sItLast);
	iterator			insert						(const _t_data &rData);

	iterator			erase						(const_iterator sCIterPos);
	_t_sizetype			erase						(const _t_key &rKey);
	iterator			erase						(const_iterator sCIterFirst, const_iterator sCIterLast);

	void				swap						(CBTreeKeySortTest &rKeySort);

	iterator			find						(const _t_key &rKey);

	iterator			lower_bound					(const _t_key &rKey);
	const_iterator		lower_bound					(const _t_key &rKey) const;

	iterator			upper_bound					(const _t_key &rKey);
	const_iterator		upper_bound					(const _t_key &rKey) const;

	void				clear						();

	_t_sizetype			count						(const _t_key &rKey) const;
	
	_t_data				get_data_reference			(const_iterator &rCIter);
	_t_data				get_data_reference			(const_reverse_iterator &rCRIter);
	
	CBTreeKeySortTest	&operator=					(const CBTreeKeySortTest &rBT);

	void				test						() const;

	void				set_reference				(reference_t *pReference);

	void				set_atomic_testing			(bool bEnable);

protected:

	int					comp						(const _t_key &rKey0, const _t_key &rKey1) const;

	_t_key				*extract_key				(_t_key *pKey, const _t_nodeiter nNode, const _t_subnodeiter nEntry) const;

	virtual _t_key		*extract_key				(_t_key *pKey, const _t_data &rData) const;

	template <class _t_iterator>
	void				insert_via_iterator			(_t_iterator sItFirst, _t_iterator sItLast)
	{
		CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::template insert<_t_iterator> (sItFirst, sItLast);
	}

	template<class _t_iterator, class _t_ref_iterator>
	_t_data				get_data_reference			(_t_iterator &rIter);

	bool				show_data					(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const;

	reference_t			*m_pClRefData;

	bool				m_bAtomicTesting;
};

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
class CBTreeKeySortTest <keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	:	public CBTreeKeySort < keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer >
{
public:

	typedef CBTreeKeySortTest									CBTreeKeySortTest_t;

	typedef CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
																CBTreeKeySort_t;

	typedef typename CBTreeKeySort_t::iterator					iterator;
	typedef typename CBTreeKeySort_t::const_iterator			const_iterator;
	typedef typename CBTreeKeySort_t::reverse_iterator			reverse_iterator;
	typedef typename CBTreeKeySort_t::const_reverse_iterator	const_reverse_iterator;

	typedef ::std::multimap<const uint32_t, keySortMap_t>		reference_t;

						CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize, typename ::std::multimap<const uint32_t, keySortMap_t> *pClRefData);

						CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign = true);

	virtual				~CBTreeKeySortTest <keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer >
													();

	template <class _t_iterator>
	void				insert						(_t_iterator sItFirst, _t_iterator sItLast);
	iterator			insert						(const keySortEntry_t &rData);

	iterator			erase						(const_iterator sCIterPos);
	_t_sizetype			erase						(const uint32_t &rKey);
	iterator			erase						(const_iterator sCIterFirst, const_iterator sCIterLast);

	void				swap						(CBTreeKeySortTest &rKeySort);

	iterator			find						(const uint32_t &rKey);
	
	iterator			lower_bound					(const uint32_t &rKey);
	const_iterator		lower_bound					(const uint32_t &rKey) const;

	iterator			upper_bound					(const uint32_t &rKey);
	const_iterator		upper_bound					(const uint32_t &rKey) const;
	
	keySortEntry_t		get_data_reference			(const_iterator &rCIter);
	keySortEntry_t		get_data_reference			(const_reverse_iterator &rCRIter);
	
	void				clear						();

	CBTreeKeySortTest	&operator=					(const CBTreeKeySortTest &rBT);

//	bool				operator==					(const CBTreeKeySortTest &rKeySort) const;
//	bool				operator!=					(const CBTreeKeySortTest &rKeySort) const;

	void				test						() const;

	void				set_reference				(reference_t *pReference);

	void				set_atomic_testing			(bool bEnable);

protected:

	int					comp						(const uint32_t &rKey0, const uint32_t &rKey1) const;

	template <class _t_iterator>
	void				insert_via_iterator			(_t_iterator sItFirst, _t_iterator sItLast)
	{
		CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::template insert<_t_iterator> (sItFirst, sItLast);
	}

	template<class _t_iterator, class _t_ref_iterator>
	keySortEntry_t		get_data_reference			(_t_iterator &rIter);

	bool				show_data					(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const;

	reference_t			*m_pClRefData;

	bool				m_bAtomicTesting;
};

#endif // BTREETESTKEYSORT_H

#include "btreetestkeysort.cpp"
