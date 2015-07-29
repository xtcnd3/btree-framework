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

#include <btreeiofile.h>

#include <btreekeysort.h>

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

template<class _t_key, class _t_map>
class CBTreePairTest	:	public ::std::pair < _t_key, _t_map >
{
public:

				CBTreePairTest<_t_key, _t_map> ()
					:	::std::pair<_t_key, _t_map> ()
				{

				}

				CBTreePairTest<_t_key, _t_map> (const _t_key &rFirst, const _t_map &rSecond)
					:	::std::pair<_t_key, _t_map> (rFirst, rSecond)
				{

				}
				
				~CBTreePairTest<_t_key, _t_map> ()
				{

				}
				/*
	operator		const _t_key ()
	{
		return (this->first);
	}*/
};

/************************************************************************************/

template <class _t_iterator, class _t_data>
struct btree_key_sort_test_insert_reference_via_iterator_arbiter
{
	static void btree_key_sort_test_insert_reference_via_iterator (_t_iterator &sItFirst, _t_iterator &sItLast, typename ::std::multimap<uint32_t, keySortMap_t> *pClRefData)
	{
		pClRefData->insert<_t_iterator> (sItFirst, sItLast);
	}
};

template <class _t_iterator>
struct btree_key_sort_test_insert_reference_via_iterator_arbiter<_t_iterator, keySortEntry_t>
{
	static void btree_key_sort_test_insert_reference_via_iterator (_t_iterator &sItFirst, _t_iterator &sItLast, typename ::std::multimap<uint32_t, keySortMap_t> *pClRefData)
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
	:	public virtual CBTreeKeySortDataIf <_t_data, _t_key, _t_sizetype>
	,	public CBTreeKeySort <_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

#if defined(__GNUC__) || defined(__GNUG__)

	typedef typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator			const_iterator;
	typedef typename CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator	const_reverse_iterator;
	
#endif

						CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize, uint32_t nKeyOffset = 0, uint32_t nKeySize = sizeof (_t_data));

						CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT);

						~CBTreeKeySortTest<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													();

	virtual _t_key		*extract_key				(_t_key *pKey, const _t_data &rData);

	template <class _t_iterator>
	void				insert						(_t_iterator sItFirst, _t_iterator sItLast);
	const_iterator		insert						(const _t_data &rData);

	const_iterator		erase						(const_iterator sCIterPos);
	_t_sizetype			erase						(const _t_key &rKey);
	const_iterator		erase						(const_iterator sCIterFirst, const_iterator sCIterLast);

	void				swap						(CBTreeKeySortTest &rKeySort);

	const_iterator		find						(const _t_key &rKey);

	const_iterator		lower_bound					(const _t_key &rKey);
	const_iterator		upper_bound					(const _t_key &rKey);

	_t_sizetype			insert_tb					(const _t_data &rData);

	_t_sizetype			erase_tb					(const _t_key &rKey);
	_t_sizetype			erase_tb					(const _t_key &rKey, const _t_sizetype instance);

	void				clear						();

	_t_sizetype			get							(const _t_key &rKey, _t_data *pObj);
	bool				get							(const _t_key &rKey, _t_sizetype nInstance, _t_data *pObj);
	_t_sizetype			count						(const _t_key &rKey);
	_t_sizetype			get_init_pos_of_key			(const _t_key &rKey);
	bool				get_at						(const _t_sizetype nPos, _t_data &rData);

	CBTreeKeySortTest	&operator=					(CBTreeKeySortTest &rBT);

	bool				operator==					(CBTreeKeySortTest &rKeySort);
	bool				operator!=					(CBTreeKeySortTest &rKeySort);

protected:

	int					comp						(const _t_key &rKey0, const _t_key &rKey1);

	_t_key				*extract_key				(_t_key *pKey, const _t_nodeiter nNode, const _t_subnodeiter nEntry);

	template <class _t_iterator>
	void				insert_via_iterator			(_t_iterator sItFirst, _t_iterator sItLast)
	{
		btree_key_sort_test_insert_reference_via_iterator_arbiter<_t_iterator, _t_data >::btree_key_sort_test_insert_reference_via_iterator (sItFirst, sItLast, m_pClRefData);

		CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::template insert<_t_iterator> (sItFirst, sItLast);
	}

	void				test						();

	bool				showdata					(std::ofstream &ofs, const _t_nodeiter nNode, const _t_subnodeiter nSubPos);

	::std::multimap<_t_key, keySortMap_t>
						*m_pClRefData;

	uint32_t			m_nKeyOffset;
	uint32_t			m_nKeySize;
};

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
class CBTreeKeySortTest <keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	: public virtual CBTreeKeySortDataIf <keySortEntry_t, uint32_t, _t_sizetype>
	, public CBTreeKeySort < keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer >
{
public:

#if defined(__GNUC__) || defined(__GNUG__)

	typedef typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator			const_iterator;
	typedef typename CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator	const_reverse_iterator;

#endif

						CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize, uint32_t nKeyOffset = 0, uint32_t nKeySize = sizeof (keySortEntry_t));

						CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(CBTreeKeySortTest<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT);

						~CBTreeKeySortTest <keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer >
													();

	template <class _t_iterator>
	void				insert						(_t_iterator sItFirst, _t_iterator sItLast);
	const_iterator		insert						(const keySortEntry_t &rData);

	const_iterator		erase						(const_iterator sCIterPos);
	_t_sizetype			erase						(const uint32_t &rKey);
	const_iterator		erase						(const_iterator sCIterFirst, const_iterator sCIterLast);

	void				swap						(CBTreeKeySortTest &rKeySort);

	const_iterator		find						(const uint32_t &rKey);
	
	const_iterator		lower_bound					(const uint32_t &rKey);
	const_iterator		upper_bound					(const uint32_t &rKey);
	
	_t_sizetype			insert_tb					(const keySortEntry_t &rData);

	_t_sizetype			erase_tb					(const uint32_t &rKey);
	_t_sizetype			erase_tb					(const uint32_t &rKey, const _t_sizetype instance);

	void				clear						();

	CBTreeKeySortTest	&operator=					(CBTreeKeySortTest &rBT);

	bool				operator==					(CBTreeKeySortTest &rKeySort);
	bool				operator!=					(CBTreeKeySortTest &rKeySort);

protected:

	int					comp						(const uint32_t &rKey0, const uint32_t &rKey1);

	template <class _t_iterator>
	void				insert_via_iterator			(_t_iterator sItFirst, _t_iterator sItLast)
	{
		btree_key_sort_test_insert_reference_via_iterator_arbiter<_t_iterator, keySortEntry_t >::btree_key_sort_test_insert_reference_via_iterator (sItFirst, sItLast, m_pClRefData);

		CBTreeKeySort<keySortEntry_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::template insert<_t_iterator> (sItFirst, sItLast);
	}

	void				test						();

	bool				showdata					(std::ofstream &ofs, const _t_nodeiter nNode, const _t_subnodeiter nSubPos);

	::std::multimap<uint32_t, keySortMap_t>
						*m_pClRefData;

	uint32_t			m_nKeyOffset;
	uint32_t			m_nKeySize;
};

#endif // BTREETESTKEYSORT_H

#include "btreetestkeysort.cpp"
