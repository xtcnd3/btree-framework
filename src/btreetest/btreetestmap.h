/************************************************************
**
** file:	btreetestmap.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains map application class code derived
** from the b-tree framework's map class to allow
** for the map class being tested by having
** the map application class act as a wrapper.
**
************************************************************/

#ifndef BTREETESTMAP_H
#define	BTREETESTMAP_H

#include <stdint.h>

#include <map>
#include <utility>

#include "btree.h"
#include "btreemap.h"

typedef struct
{
	uint32_t	nData;
	uint32_t	nDebug;
} mapMap_t;

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
class CBTreeTestMap
	:	public CBTreeMap<uint32_t, mapMap_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
//	,	public virtual CBTreeKeySortDataIf < ::std::pair<uint32_t, mapMap_t>, uint32_t, _t_sizetype >
{
public:

	typedef CBTreeTestMap												CBTreeTestMap_t;
	
	typedef CBTreeMap<uint32_t, mapMap_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
																		CBTreeMap_t;

	typedef typename CBTreeMap_t::CBTreeKeySort_t::CBTreeBase_t			CBTreeBase_t;

#if defined(__GNUC__) || defined(__GNUG__)

	typedef typename ::CBTreeBaseIterator<CBTreeBase_t>					iterator;
	typedef typename ::CBTreeBaseReverseIterator<iterator>				reverse_iterator;
	typedef typename ::CBTreeBaseConstIterator<CBTreeBase_t>			const_iterator;
	typedef typename ::CBTreeBaseConstReverseIterator<const_iterator>	const_reverse_iterator;

#endif

	typedef typename CBTreeMap_t::position_t							position_t;
	typedef typename ::std::pair<const uint32_t, mapMap_t>				data_t;
	typedef _t_sizetype													sizetype_t;
	typedef _t_nodeiter													nodeiter_t;
	typedef _t_subnodeiter												subnodeiter_t;
	typedef _t_datalayerproperties										datalayerproperties_t;
	typedef _t_datalayer												datalayer_t;

	typedef ::std::pair<uint32_t, mapMap_t>								value_t;

	typedef typename ::std::pair<iterator, bool>						insert_rslt_t;

	typedef	typename CBTreeMap_t::key_compare							key_compare;
	typedef typename CBTreeMap_t::value_compare							value_compare;

							CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
								(_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

							CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
								(CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign = true);

							~CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
								();

	CBTreeTestMap_t &		operator=				(CBTreeTestMap_t &rBT);

//	iterator				begin					();
//	iterator				end						();
//	reverse_iterator		rbegin					();
//	reverse_iterator		rend					();
//	const_iterator			cbegin					();
//	const_iterator			cend					();
//	const_reverse_iterator	crbegin					();
//	const_reverse_iterator	crend					();

//	bool					empty					();
//	_t_sizetype				size					();
//	_t_sizetype				max_size				();

	template <class _t_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);
	insert_rslt_t			insert					(const value_t &rData);

	iterator				erase					(const_iterator sCIterPos);
	_t_sizetype				erase					(const uint32_t &rKey);
	iterator				erase					(const_iterator sCIterFirst, const_iterator sCIterLast);

	void					swap					(CBTreeTestMap &rTMM);

	void					clear					();

	key_compare				key_comp				() const;
	value_compare			value_comp				() const;
	
//	iterator				find					(const uint32_t &rKey);
//	_t_sizetype				count					(const uint32_t &rKey);
//	iterator				lower_bound				(const uint32_t &rKey);
//	iterator				upper_bound				(const uint32_t &rKey);

	bool					operator==				(CBTreeTestMap &rTMM);
	bool					operator!=				(CBTreeTestMap &rTMM);

protected:

	void					test					();

	uint32_t				m_nDebug;

	::std::map<uint32_t, mapMap_t>
							*m_pClRef;

public:

	friend class CBTreeBaseIterator<CBTreeBase_t>;
	friend class CBTreeBaseReverseIterator<iterator>;
	friend class CBTreeBaseConstIterator<CBTreeBase_t>;
	friend class CBTreeBaseConstReverseIterator<const_iterator>;

};

#endif // !BTREETESTMAP_H

#include "btreetestmap.cpp"
