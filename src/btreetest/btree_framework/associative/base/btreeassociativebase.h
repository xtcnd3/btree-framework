/************************************************************
**
** file:	btreeassociativebase.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** STL base class, containing a common sub-set of methods
** shared between all STL interchangeable container classes.
**
************************************************************/

#ifndef BTREEASSOCAITIVEBASE_H
#define	BTREEASSOCAITIVEBASE_H

#include <utility>

#include "btreeassociative.h"
#include "btreeiter.h"

template <class _t_data, class _t_key = _t_data, class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter> >
class CBTreeAssociativeBase
	:	public CBTreeAssociative<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

	typedef CBTreeAssociativeBase							CBTreeAssociativeBase_t;

	typedef CBTreeAssociative<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
															CBTreeAssociative_t;

	typedef typename CBTreeAssociative_t::position_t		position_t;

	typedef typename CBTreeAssociative_t::CBTreeBase_t		CBTreeBase_t;

	typedef typename CBTreeBase_t::CBTreeBaseIf_t			CBTreeBaseIf_t;

	typedef typename CBTreeBaseIf_t::CBTreeDefaults_t		CBTreeDefaults_t;

	typedef CBTreeIf<_t_data, _t_sizetype>					CBTreeIf_t;

	typedef typename CBTreeIf_t::iterator					iterator;
	typedef	typename CBTreeIf_t::const_iterator				const_iterator;
	typedef typename CBTreeIf_t::reverse_iterator			reverse_iterator;
	typedef	typename CBTreeIf_t::const_reverse_iterator		const_reverse_iterator;

	typedef	typename CBTreeBase_t::node_t					node_t;

	typedef _t_sizetype										size_type;
	typedef _t_nodeiter										nodeiter_t;
	typedef _t_subnodeiter									subnodeiter_t;
	typedef _t_datalayerproperties							datalayerproperties_t;
	typedef _t_datalayer									datalayer_t;

	// construction
							CBTreeAssociativeBase<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

							CBTreeAssociativeBase<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(const CBTreeAssociativeBase_t &rBT, bool bAssign = true);

	// destruction
	virtual					~CBTreeAssociativeBase<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													();

	CBTreeAssociativeBase_t &		operator=				(const CBTreeAssociativeBase_t &rBT);

	iterator				begin					();
	iterator				end						();
	reverse_iterator		rbegin					();
	reverse_iterator		rend					();

	const_iterator			begin					() const;
	const_iterator			end						() const;
	const_reverse_iterator	rbegin					() const;
	const_reverse_iterator	rend					() const;

	_t_sizetype				max_size				() const;

	iterator				lower_bound				(const _t_key &rKey);
	const_iterator			lower_bound				(const _t_key &rKey) const;

	iterator				upper_bound				(const _t_key &rKey);
	const_iterator			upper_bound				(const _t_key &rKey) const;

protected:

	int						comp					(const _t_key &rKey0, const _t_key &rKey1) const;

	static int				comp					(const _t_key &rKey0, const _t_key &rKey1, ::std::true_type);
	static int				comp					(const _t_key &rKey0, const _t_key &rKey1, ::std::false_type);

	void					_swap					(CBTreeAssociativeBase_t &rContainer);

public:

	friend class CBTreeIterator<CBTreeBase_t>;
	friend class CBTreeConstIterator<CBTreeBase_t>;
	friend class CBTreeReverseIterator<iterator>;
	friend class CBTreeConstReverseIterator<const_iterator>;
};

#endif // BTREEASSOCAITIVEBASE_H

#include "btreeassociativebase.cpp"
