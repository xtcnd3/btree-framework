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

#include "btreetestcommon.h"

#include "btreebasedefaults.h"
#include "./associative/btreemap.h"

typedef struct mapMap_s
{
	uint32_t	nData;
	uint32_t	nDebug;

	bool	operator== (const struct mapMap_s &rOpr) const
	{
		return ((nData == rOpr.nData) && (nDebug == rOpr.nDebug));
	};
} mapMap_t;

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
class CBTreeTestMap
	:	public CBTreeMap<uint32_t, mapMap_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

	typedef CBTreeTestMap									CBTreeTestMap_t;
	
	typedef CBTreeMap<uint32_t, mapMap_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
															CBTreeMap_t;

	typedef typename CBTreeMap_t::CBTreeAssociativeIf_t		CBTreeAssociativeIf_t;

	typedef typename CBTreeMap_t::CBTreeDefaults_t		CBTreeDefaults_t;

	typedef typename CBTreeMap_t::iterator					iterator;
	typedef typename CBTreeMap_t::const_iterator			const_iterator;
	typedef typename CBTreeMap_t::reverse_iterator			reverse_iterator;
	typedef typename CBTreeMap_t::const_reverse_iterator	const_reverse_iterator;

//	typedef typename CBTreeMap_t::position_t				position_t;
	typedef typename ::std::pair<const uint32_t, mapMap_t>	data_t;
	typedef _t_sizetype										size_type;
	typedef _t_nodeiter										nodeiter_t;
	typedef _t_subnodeiter									subnodeiter_t;
	typedef _t_datalayerproperties							datalayerproperties_t;
	typedef _t_datalayer									datalayer_t;

	typedef ::std::pair<uint32_t, mapMap_t>					value_t;

	typedef	typename CBTreeMap_t::key_compare				key_compare;
	typedef typename CBTreeMap_t::value_compare				value_compare;

	typedef ::std::map<const uint32_t, mapMap_t>			reference_t;

							CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
								(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize, reference_t *pClRefData);

							CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
								(const CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign = true);

							~CBTreeTestMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
								();

	CBTreeTestMap_t &		operator=				(const CBTreeTestMap_t &rBT);

	template <class _t_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);
/*
	template <class _t_iterator, class _t_ref_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);
*/
	iterator				insert					(const value_t &rData);

	iterator				erase					(const_iterator sCIterPos);
	_t_sizetype				erase					(const uint32_t &rKey);
	iterator				erase					(const_iterator sCIterFirst, const_iterator sCIterLast);

	void					swap					(CBTreeTestMap &rTMM);

	void					clear					();

	key_compare				key_comp				() const;
	value_compare			value_comp				() const;
	
	bool					operator==				(const CBTreeTestMap &rTMM) const;
	bool					operator!=				(const CBTreeTestMap &rTMM) const;

	void					test					() const;

	void					set_reference			(reference_t *pReference);

	void					set_atomic_testing		(bool bEnable);

protected:

	bool					show_data				(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const;

	reference_t				*m_pClRef;

	bool					m_bAtomicTesting;

public:

	friend class CBTreeIterator<CBTreeDefaults_t>;
	friend class CBTreeConstIterator<CBTreeDefaults_t>;
	friend class CBTreeReverseIterator<iterator>;
	friend class CBTreeConstReverseIterator<const_iterator>;
};

#endif // !BTREETESTMAP_H

#include "btreetestmap.cpp"
