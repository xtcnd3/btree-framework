/************************************************************
**
** file:	btreetestmultimap.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains multi map application class code derived
** from the b-tree framework's multi map class to allow
** for the multi map class being tested by having
** the multi map application class act as a wrapper.
**
************************************************************/

#ifndef BTREETESTMULTIMAP_H
#define	BTREETESTMULTIMAP_H

#include <stdint.h>

#include <map>
#include <utility>
#include <type_traits>

#include "btreetestcommon.h"

#include "btreebasedefaults.h"
#include "./associative/btreemultimap.h"

typedef struct multiMapMap_s
{
	uint32_t	nData;
	uint32_t	nDebug;

	bool	operator== (const struct multiMapMap_s &rOpr) const
	{
		return ((nData == rOpr.nData) && (nDebug == rOpr.nDebug));
	};
} multiMapMap_t;

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
class CBTreeTestMultiMap
	:	public CBTreeMultiMap<uint32_t, multiMapMap_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

	typedef CBTreeTestMultiMap											CBTreeTestMultiMap_t;
	
	typedef CBTreeMultiMap<uint32_t, multiMapMap_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
																		CBTreeMultiMap_t;

	typedef typename CBTreeMultiMap_t::CBTreeAssociativeBase_t			CBTreeAssociativeBase_t;

	typedef typename CBTreeAssociativeBase_t::CBTreeAssociative_t		CBTreeAssociative_t;

	typedef typename CBTreeMultiMap_t::CBTreeAssociativeIf_t			CBTreeAssociativeIf_t;

	typedef typename CBTreeAssociative_t::CBTreeBase_t					CBTreeBase_t;

	typedef typename CBTreeBase_t::CBTreeBaseIf_t						CBTreeBaseIf_t;

	typedef typename CBTreeBaseIf_t::CBTreeDefaults_t					CBTreeDefaults_t;

	typedef typename CBTreeMultiMap_t::iterator							iterator;
	typedef typename CBTreeMultiMap_t::const_iterator					const_iterator;
	typedef typename CBTreeMultiMap_t::reverse_iterator					reverse_iterator;
	typedef typename CBTreeMultiMap_t::const_reverse_iterator			const_reverse_iterator;

//	typedef typename CBTreeMultiMap_t::position_t						position_t;
	typedef typename ::std::pair<const uint32_t, multiMapMap_t>			data_t;
	typedef _t_sizetype													size_type;
	typedef _t_nodeiter													nodeiter_t;
	typedef _t_subnodeiter												subnodeiter_t;
	typedef _t_datalayerproperties										datalayerproperties_t;
	typedef _t_datalayer												datalayer_t;

	typedef ::std::pair<uint32_t, multiMapMap_t>						value_t;

	typedef	typename CBTreeMultiMap_t::key_compare						key_compare;
	typedef typename CBTreeMultiMap_t::value_compare					value_compare;

	typedef ::std::multimap<const uint32_t, multiMapMap_t>					reference_t;

							CBTreeTestMultiMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
								(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize, reference_t *pClRefData);

							CBTreeTestMultiMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
								(const CBTreeTestMultiMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign = true);

							~CBTreeTestMultiMap<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
								();

	CBTreeTestMultiMap_t &	operator=				(const CBTreeTestMultiMap_t &rBT);

	template <class _t_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);
	template <class _t_iterator, class _t_ref_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);
	iterator				insert					(const value_t &rData);

	iterator				erase					(const_iterator sCIterPos);
	_t_sizetype				erase					(const uint32_t &rKey);
	iterator				erase					(const_iterator sCIterFirst, const_iterator sCIterLast);

	void					swap					(CBTreeTestMultiMap &rTMM);

	void					clear					();

	key_compare				key_comp				() const;
	value_compare			value_comp				() const;
	
	bool					operator==				(const CBTreeTestMultiMap &rTMM) const;
	bool					operator!=				(const CBTreeTestMultiMap &rTMM) const;

	void					test					() const;

	void					set_reference			(reference_t *pReference);

	void					set_atomic_testing		(bool bEnable);

protected:

	reference_t				*m_pClRef;

	bool					m_bAtomicTesting;

public:

	friend class CBTreeIterator<CBTreeDefaults_t >;
	friend class CBTreeConstIterator<CBTreeDefaults_t>;
	friend class CBTreeReverseIterator<iterator>;
	friend class CBTreeConstReverseIterator<const_iterator>;
};

#endif // !BTREETESTMULTIMAP_H

#include "btreetestmultimap.cpp"
