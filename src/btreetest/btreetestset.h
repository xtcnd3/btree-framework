/************************************************************
**
** file:	btreetestset.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains set application class code derived
** from the b-tree framework's set class to allow
** for the set class being tested by having
** the set application class act as a wrapper.
**
************************************************************/

#ifndef BTREETESTSET_H
#define	BTREETESTSET_H

#include <stdint.h>

#include <set>
#include <utility>

#include "btreetestcommon.h"

#include "btreebasedefaults.h"
#include "./associative/btreeset.h"

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
class CBTreeTestSet
	:	public CBTreeSet<uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

	typedef CBTreeTestSet												CBTreeTestSet_t;
	
	typedef CBTreeSet<uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
																		CBTreeSet_t;

	typedef typename CBTreeSet_t::CBTreeDefaults_t					CBTreeDefaults_t;

	typedef typename CBTreeSet_t::CBTreeAssociativeIf_t					CBTreeAssociativeIf_t;

	typedef typename CBTreeSet_t::iterator								iterator;
	typedef typename CBTreeSet_t::const_iterator						const_iterator;
	typedef typename CBTreeSet_t::reverse_iterator						reverse_iterator;
	typedef typename CBTreeSet_t::const_reverse_iterator				const_reverse_iterator;

//	typedef typename CBTreeSet_t::position_t							position_t;
	typedef const uint32_t												data_t;
	typedef _t_sizetype													size_type;
	typedef _t_nodeiter													nodeiter_t;
	typedef _t_subnodeiter												subnodeiter_t;
	typedef _t_datalayerproperties										datalayerproperties_t;
	typedef _t_datalayer												datalayer_t;

	typedef uint32_t													value_type;

	typedef uint32_t													value_t;

	typedef	typename CBTreeSet_t::key_compare							key_compare;
	typedef typename CBTreeSet_t::value_compare							value_compare;

							CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
								(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

							CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
								(const CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign = true);

							~CBTreeTestSet<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
								();

	CBTreeTestSet_t &		operator=				(const CBTreeTestSet_t &rBT);

	template <class _t_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);

	template <class _t_iterator, class _t_ref_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);

	iterator				insert					(const value_t &rData);

	iterator				erase					(const_iterator sCIterPos);
	_t_sizetype				erase					(const uint32_t &rKey);
	iterator				erase					(const_iterator sCIterFirst, const_iterator sCIterLast);

	void					swap					(CBTreeTestSet &rTMM);

	void					clear					();

	key_compare				key_comp				() const;
	value_compare			value_comp				() const;
	
	bool					operator==				(const CBTreeTestSet &rTMM) const;
	bool					operator!=				(const CBTreeTestSet &rTMM) const;

protected:

	void					test					() const;

	bool					show_data				(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const;

	::std::set<uint32_t>	*m_pClRef;

public:

	friend class CBTreeIterator<CBTreeDefaults_t >;
	friend class CBTreeConstIterator<CBTreeDefaults_t>;
	friend class CBTreeReverseIterator<iterator>;
	friend class CBTreeConstReverseIterator<const_iterator>;
};

#endif // !BTREETESTSET_H

#include "btreetestset.cpp"
