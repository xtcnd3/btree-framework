/************************************************************
**
** file:	btreedefaults.h
** author:	Andreas Steffens
** license:	LGPL v3
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

template<class _t_data, class _t_sizetype = uint64_t>
class CBTreeDefaults
	:	public virtual CBTreeIf<_t_data, _t_sizetype>
{
public:

	typedef _t_data											value_type;
	typedef _t_sizetype										size_type;

	typedef value_type&										reference;
	typedef const value_type&								const_reference;
	typedef value_type*										pointer;
	typedef const value_type*								const_pointer;
	typedef	typename ::std::make_signed<size_type>::type	difference_type;

	typedef CBTreeDefaults<value_type, size_type>			CBTreeDefaults_t;

	typedef CBTreeIf<value_type, size_type>					CBTreeIf_t;

	typedef typename CBTreeIf_t::iterator					iterator;
	typedef typename CBTreeIf_t::const_iterator				const_iterator;
	typedef typename CBTreeIf_t::reverse_iterator			reverse_iterator;
	typedef typename CBTreeIf_t::const_reverse_iterator		const_reverse_iterator;

	// construction
									CBTreeDefaults<_t_data, _t_sizetype>
															();

	explicit						CBTreeDefaults<_t_data, _t_sizetype>
															(const CBTreeDefaults<_t_data, _t_sizetype> &rContainer);

									CBTreeDefaults<_t_data, _t_sizetype>
															(CBTreeDefaults<_t_data, _t_sizetype> &&rRhsContainer);

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

	CBTreeDefaults &				operator=				(CBTreeDefaults &&rRhsContainer);

protected:
 
	void							register_iterator		(iterator *pIter);
	void							register_iterator		(const_iterator *pCIter);
//	void							register_iterator		(reverse_iterator *pRIter);
//	void							register_iterator		(const_reverse_iterator *pCRIter);

	void							unregister_iterator		(iterator *pIter);
	void							unregister_iterator		(const_iterator *pCIter);
//	void							unregister_iterator		(reverse_iterator *pRIter);
//	void							unregister_iterator		(const_reverse_iterator *pCRIter);

	void							_swap					(CBTreeDefaults &rContainer);

	void							_local_swap				(CBTreeDefaults &rContainer);

#if defined (USE_PERFORMANCE_COUNTERS)

	uint64_t												m_nHitCtr;
	uint64_t												m_nMissCtr;

#endif

#if defined (BTREE_ITERATOR_REGISTRATION)

	typename ::std::set<iterator *>							*m_psIterRegister;
	typename ::std::set<const_iterator *>					*m_psCIterRegister;
//	typename ::std::set<reverse_iterator *>					*m_psRIterRegister;
//	typename ::std::set<const_reverse_iterator *>			*m_psCRIterRegister;

#endif

public:

	friend class CBTreeIterator<CBTreeIf_t>;
	friend class CBTreeConstIterator<CBTreeIf_t>;
	friend class CBTreeReverseIterator<iterator>;
	friend class CBTreeConstReverseIterator<const_iterator>;
};

#endif // BTREEDEFAULTS_H

#include "btreedefaults.cpp"
