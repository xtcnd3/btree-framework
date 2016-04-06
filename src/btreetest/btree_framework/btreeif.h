/************************************************************
**
** file:	btreeif.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree frameworks base class
** on the iterator interface level.
**
************************************************************/

#ifndef	BTREEBASEITERIF_H
#define	BTREEBASEITERIF_H

#include <iostream>
#include <fstream>
#include <set>

#include <string.h>

#if defined (_OPENMP)
 #include <omp.h>
#endif

#include <string>

#if defined (WIN32)

 #if defined (_HAS_MFC)
  #include <afxtempl.h>
 #endif

 #if !defined (_HAS_MFC)
  #include <windows.h>
 #endif

 #include <io.h>

#elif defined (LINUX)

 #include <stdlib.h>

#endif

#include <memory.h>

#include "btreeaux.h"
#include "btreeiter.h"

typedef struct
{
	uint32_t	nMinNumberOfBytesPerSuperBlock;
} bayerTreeCacheDescription_t;

template <class _t_data, class _t_sizetype = uint64_t>
class CBTreeIf
	:	public CBTreeSuper
{
public:

	typedef CBTreeIf<_t_data, _t_sizetype>					CBTreeIf_t;

	typedef CBTreeIterator<CBTreeIf_t>						iterator;
	typedef CBTreeConstIterator<CBTreeIf_t>					const_iterator;
	typedef CBTreeReverseIterator<iterator>					reverse_iterator;
	typedef CBTreeConstReverseIterator<const_iterator>		const_reverse_iterator;

	typedef _t_data											data_t;
	typedef _t_sizetype										size_type;
//	typedef _ti_pos											position_t;

									CBTreeIf				();

	explicit						CBTreeIf				(const CBTreeIf<_t_data, _t_sizetype> &rContainer);

	virtual							~CBTreeIf				();

	virtual bool					empty					() const = 0;
	virtual _t_sizetype				size					() const = 0;

	virtual void					clear					() = 0;

	// read only iterators
	virtual const_iterator			cbegin					() const = 0;
	virtual const_iterator			cend					() const = 0;
	virtual const_reverse_iterator	crbegin					() const = 0;
	virtual const_reverse_iterator	crend					() const = 0;

	virtual iterator				begin					() = 0;
	virtual iterator				end						() = 0;
	virtual reverse_iterator		rbegin					() = 0;
	virtual reverse_iterator		rend					() = 0;

	virtual const_iterator			begin					() const = 0;
	virtual const_iterator			end						() const = 0;
	virtual const_reverse_iterator	rbegin					() const = 0;
	virtual const_reverse_iterator	rend					() const = 0;

	// storage managment
	virtual void					unload					() = 0;
	
	virtual void					show_integrity			(const char *pFilename) const = 0;

#if defined (BTREEFRAMEWORK_TEST_BENCH_ENABLED)

	virtual void					test					() const = 0;

	virtual void					set_atomic_testing		(bool bEnable) = 0;

#endif

	CBTreeIf_t &					operator=				(const CBTreeIf_t &rContainer);

protected:

	virtual void					_assign					(const CBTreeIf_t &rContainer) = 0;

	virtual void					register_iterator		(const_iterator *pIter) = 0;
	virtual void					unregister_iterator		(const_iterator *pIter) = 0;

	virtual uint32_t				get_iter_state_size		() const = 0;
	virtual void					reset_iter_state		(void *pState) const = 0;
	virtual void					evaluate_iter			(void *pState, _t_sizetype nOffsetPos) const = 0;
	virtual void					evaluate_iter_by_seek	(void *pState, _t_sizetype nNewPos) const = 0;
	virtual bool					is_iter_pos_evaluated	(void *pState, _t_sizetype nPos) const = 0; // part of is_evaluated ()

	virtual _t_data					*get_iter_data			(void *pState) const = 0;
	virtual void					set_iter_data			(void *pState, const _t_data &rData) = 0;

public:

	friend class CBTreeIterator<CBTreeIf<_t_data, _t_sizetype> >;
	friend class CBTreeConstIterator<CBTreeIf<_t_data, _t_sizetype> >;
	friend class CBTreeReverseIterator<iterator>;
	friend class CBTreeConstReverseIterator<const_iterator>;

};

#endif // BTREEBASEITERIF_H

#include "btreeif.cpp"
