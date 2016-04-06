/************************************************************
**
** file:	btreedefaults.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains declarations for the b-tree frameworks base
** class on the iterator interface level.
**
************************************************************/

#ifndef	BTREEDEFAULTS_H
#define	BTREEDEFAULTS_H

#include "btreeif.h"

template <class _t_data, class _t_sizetype = uint64_t>
class CBTreeDefaults
	:	public virtual CBTreeIf<_t_data, _t_sizetype>
{
public:

	typedef CBTreeDefaults<_t_data, _t_sizetype>		CBTreeDefaults_t;

	typedef CBTreeIf<_t_data, _t_sizetype>				CBTreeIf_t;

	typedef typename CBTreeIf_t::iterator				iterator;
	typedef typename CBTreeIf_t::const_iterator			const_iterator;
	typedef typename CBTreeIf_t::reverse_iterator		reverse_iterator;
	typedef typename CBTreeIf_t::const_reverse_iterator	const_reverse_iterator;

//	typedef _ti_pos										position_t;
	typedef _t_data										data_t;
	typedef _t_sizetype									size_type;

	// construction
									CBTreeDefaults<_t_data, _t_sizetype>
															(const bayerTreeCacheDescription_t *psCacheDescription);

									CBTreeDefaults<_t_data, _t_sizetype>
															(const CBTreeDefaults<_t_data, _t_sizetype> &rBT);

	// destruction
	virtual							~CBTreeDefaults<_t_data, _t_sizetype> ();

	// monitoring
	void							get_performance_counters(uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR]);

	// read only iterators
	const_iterator					cbegin					() const;
	const_iterator					cend					() const;
	const_reverse_iterator			crbegin					() const;
	const_reverse_iterator			crend					() const;

	iterator						begin					();
	iterator						end						();
	reverse_iterator				rbegin					();
	reverse_iterator				rend					();

	const_iterator					begin					() const;
	const_iterator					end						() const;
	const_reverse_iterator			rbegin					() const;
	const_reverse_iterator			rend					() const;

	CBTreeDefaults &				operator=				(const CBTreeDefaults &rBT);

protected:
 
	void							register_iterator		(const_iterator *pIter);
	void							unregister_iterator		(const_iterator *pIter);

	void							_swap					(CBTreeDefaults &rBT);

	// properties
	bayerTreeCacheDescription_t								m_sCacheDescription;

#if defined (USE_PERFORMANCE_COUNTERS)

	uint64_t												m_nHitCtr;
	uint64_t												m_nMissCtr;

#endif

#if defined (BTREE_ITERATOR_REGISTRATION)

	typename ::std::set<const_iterator *>					*m_psIterRegister;

#endif

public:

	friend class CBTreeIterator<CBTreeIf<_t_data, _t_sizetype> >;
	friend class CBTreeConstIterator<CBTreeIf<_t_data, _t_sizetype> >;
	friend class CBTreeReverseIterator<iterator>;
	friend class CBTreeConstReverseIterator<const_iterator>;
};

#endif // BTREEDEFAULTS_H

#include "btreedefaults.cpp"
