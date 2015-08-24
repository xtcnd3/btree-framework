/************************************************************
**
** file:	btreearray.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's array data class.
**
************************************************************/

#ifndef	BTREEARRAY_H
#define	BTREEARRAY_H

#if defined (LINUX)
#define	_FILE_OFFSET_BITS	64
#endif

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <fstream>
#include <iterator>

#include "btree.h"

#include "btreecommon.h"
#include "btreeaux.h"
#include "btreeiter.h"

template <class _t_data, class _t_sizetype>
class CBTreeArrayConstAccessWrapper;

template <class _t_data, class _t_sizetype>
class CBTreeArrayAccessWrapper;

template <class _t_sizetype>
class CBTreeArrayPos
{
public:
	_t_sizetype			nIndex;
};

template <class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
class CBTreeArray;

/***********************************************************************/

template <class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer, class _t_iterator>
struct btree_array_self_reference_of_iterator_to_this_arbiter
{
	static bool	test_self_reference_of_iterator_to_this
	(
		CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *pClArray, 
		_t_iterator &sItFirst, 
		_t_iterator &sItLast, 
		_t_sizetype &nDist, 
		_t_sizetype &nAppendix, 
		bool &bSelfReverse
	)
	{
		pClArray;
		sItFirst;
		sItLast;

		nDist = 0;
		nAppendix = 0;

		bSelfReverse = false;

		return (false);
	}
};

template <class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
struct btree_array_self_reference_of_iterator_to_this_arbiter<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator>
{
    static bool	test_self_reference_of_iterator_to_this
    (
    	CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *pClArray, 
		typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator &sItFirst, 
    	typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator &sItLast, 
    	_t_sizetype &nDist, 
    	_t_sizetype &nAppendix, 
		bool &bSelfReverse
    )
	{
		nDist = sItLast - sItFirst;
		nAppendix = sItLast.get_container ()->end () - sItLast;

		bSelfReverse = false;

		return ((pClArray == sItFirst.get_container ()) || (pClArray == sItLast.get_container ()));
	}
};

template <class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
struct btree_array_self_reference_of_iterator_to_this_arbiter<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator>
{
    static bool	test_self_reference_of_iterator_to_this
    (
    	CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *pClArray, 
		typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator &sItFirst, 
    	typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator &sItLast, 
    	_t_sizetype &nDist, 
    	_t_sizetype &nAppendix, 
		bool &bSelfReverse
    )
	{
		nDist = sItLast - sItFirst;
		nAppendix = sItLast.get_container ()->cend () - sItLast;

		bSelfReverse = false;

		return ((pClArray == sItFirst.get_container ()) || (pClArray == sItLast.get_container ()));
	}
};

template <class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
struct btree_array_self_reference_of_iterator_to_this_arbiter<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator>
{
    static bool	test_self_reference_of_iterator_to_this
    (
    	CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *pClArray, 
		typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator &sItFirst, 
    	typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator &sItLast, 
    	_t_sizetype &nDist, 
    	_t_sizetype &nAppendix, 
		bool &bSelfReverse
    )
	{
		bool		bSelfReverence = ((pClArray == sItFirst.base ().get_container ()) || (pClArray == sItLast.base ().get_container ()));

		nDist = sItLast - sItFirst;
		nAppendix = sItLast.base ().get_container ()->rend () - sItLast;

		bSelfReverse = bSelfReverence;

		return (bSelfReverence);
	}
};

template <class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
struct btree_array_self_reference_of_iterator_to_this_arbiter<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator>
{
    static bool	test_self_reference_of_iterator_to_this
    (
    	CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *pClArray, 
		typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator &sItFirst, 
    	typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator &sItLast, 
    	_t_sizetype &nDist, 
    	_t_sizetype &nAppendix, 
		bool &bSelfReverse
    )
	{
		bool		bSelfReverence = ((pClArray == sItFirst.base ().get_container ()) || (pClArray == sItLast.base ().get_container ()));

		nDist = sItLast - sItFirst;
		nAppendix = sItLast.base ().get_container ()->crend () - sItLast;

		bSelfReverse = bSelfReverence;

		return (bSelfReverence);
	}
};

/***********************************************************************/

template <class _t_data, class _t_sizetype = uint64_t>
class CBTreeArrayDataIf
	:	public virtual CBTreeIf<_t_sizetype>
{
	public:

	// construction
						CBTreeArrayDataIf<_t_data, _t_sizetype>
													()
						{
						};
						
	// destruction
						~CBTreeArrayDataIf<_t_data, _t_sizetype>
													()
						{
						};

	virtual CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
						at							(const _t_sizetype nPos) = 0;

	virtual CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
						front						() = 0;

	virtual CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
						back						() = 0;

	virtual void		assign						(_t_sizetype nNewSize, const _t_data& rVal) = 0;

	virtual	void		push_back					(const _t_data &rData) = 0;
	virtual	void		pop_back					() = 0;
	
	virtual _t_sizetype	insert_at					(const _t_sizetype nPos, const _t_data &rData) = 0;

	virtual _t_sizetype	replace_at					(const _t_sizetype nStart, const _t_sizetype nLen, _t_data *pSrcData) = 0;

	virtual _t_sizetype	remove_at					(const _t_sizetype nPos) = 0;

	virtual void		clear						() = 0;

	virtual _t_sizetype	serialize					(const _t_sizetype nStart, const _t_sizetype nLen, _t_data *pData) = 0;

	virtual CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
						operator[]					(const _t_sizetype nPos) = 0;

protected:

	virtual bool		set_at						(const _t_sizetype nPos, const _t_data &rData) = 0;
	virtual bool		get_at						(const _t_sizetype nPos, _t_data &rData) = 0;

public:

	friend class CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype>;
	friend class CBTreeArrayAccessWrapper<_t_data, _t_sizetype>;
};

template <class _t_data, class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter> >
class CBTreeArray
	:	public virtual CBTreeArrayDataIf <_t_data, _t_sizetype>
	,	public CBTreeBaseIteratorWriteAccess <CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

#if defined(__GNUC__) || defined(__GNUG__)

	typedef	typename CBTreeBase<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::node_t	node_t;
	
	typedef	typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator	iterator;
	typedef	typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator	const_iterator;
	typedef	typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator	reverse_iterator;
	typedef	typename CBTreeBaseIteratorWriteAccess<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator	const_reverse_iterator;
	
#endif

	typedef CBTreeBaseIteratorWriteAccess <CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>	CBTreeBase_t;

	typedef CBTreeArray												CBTreeArray_t;

	typedef CBTreeArrayPos<_t_sizetype>								position_t;
	typedef _t_data													data_t;
	typedef _t_sizetype												sizetype_t;
	typedef _t_nodeiter												nodeiter_t;
	typedef _t_subnodeiter											subnodeiter_t;
	typedef _t_datalayerproperties									datalayerproperties_t;
	typedef _t_datalayer											datalayer_t;

	// construction
								CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
														(_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

								CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
														(CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign = true);
						
	// destruction
	virtual						~CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
														();

	CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
								at						(const _t_sizetype nPos);

	CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
								front					();

	CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
								back					();

	template <class _t_iterator>
	void						assign					(_t_iterator sItFirst, _t_iterator sItLast);
	void						assign					(_t_sizetype nNewSize, const _t_data& rVal);

	void						push_back				(const _t_data &rData);

	void						pop_back				();

	iterator					insert					(const_iterator sCIterPos, const _t_data& rData);
	iterator					insert					(const_iterator sCIterPos, const _t_sizetype nLen, const _t_data& rData);

	template <class _t_iterator>
	iterator					insert					(const_iterator sCIterPos, _t_iterator sItFirst, _t_iterator sItLast);

	iterator					erase					(const_iterator sCIterPos);
	iterator					erase					(const_iterator sCIterFirst, const_iterator sCIterLast);

	void						swap					(CBTreeArray &rArray);

	_t_sizetype					insert_at				(const _t_sizetype nPos, const _t_data &rData);

	_t_sizetype					replace_at				(const _t_sizetype nStart, const _t_sizetype nLen, _t_data *pSrcData);

	_t_sizetype					remove_at				(const _t_sizetype nPos);

	void						clear					();

	_t_sizetype					serialize				(const _t_sizetype nStart, const _t_sizetype nLen, _t_data *pData);

	CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &
								operator=				(CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT);

	CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
								operator[]				(const _t_sizetype nPos);

protected:

	bool						set_at					(const _t_sizetype nPos, const _t_data &rData);
	bool						get_at					(const _t_sizetype nPos, _t_data &rData);

	// node operations
	virtual CBTreeArrayPos<_t_sizetype>	determine_position	(CBTreeArrayPos<_t_sizetype> sPos, _t_nodeiter nNode, _t_subnodeiter &nSubPos, _t_subnodeiter &nSubData, bool &bFound);

	void						rebuild_node			(const _t_nodeiter nNode, const int32_t triMod = 0, _t_subnodeiter nSubStart = 0);

	virtual CBTreeArrayPos<_t_sizetype> generate_prev_position	(const _t_nodeiter nNode, const _t_subnodeiter nSub, CBTreeArrayPos<_t_sizetype> sPos);
	virtual CBTreeArrayPos<_t_sizetype> generate_next_position	(const _t_nodeiter nNode, const _t_subnodeiter nSub, CBTreeArrayPos<_t_sizetype> sPos);

	// manuvering
	virtual _t_subnodeiter		find_next_sub_pos		(const _t_nodeiter nNode, CBTreeArrayPos<_t_sizetype> &sPos);

	virtual bool				show_data				(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos);
	virtual bool				show_node				(std::ofstream &ofs, const _t_nodeiter nNode, const _t_subnodeiter nSubPos);

	void						assign					(CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT);

	template <class _t_iterator>
	iterator					insert_via_iterator_tag (const_iterator sCIterPos, _t_iterator sItFirst, _t_iterator sItLast, ::std::forward_iterator_tag);

	template <class _t_iterator>
	iterator					insert_via_iterator_tag (const_iterator sCIterPos, _t_iterator sItFirst, _t_iterator sItLast, ::std::random_access_iterator_tag);

	void						insert_via_reference	(const_iterator &rCIterPos, const _t_data& rData, bool bReEvaluate = true);

	void						erase_via_reference		(const_iterator &rCIterPos, bool bReEvaluate = true);

	template<class _t_iterator>
	bool						test_self_reference_of_iterator_to_this (_t_iterator &sItFirst, _t_iterator &sItLast, _t_sizetype &nDist, _t_sizetype &nAppendix, bool &bSelfReverse)
	{
		return (btree_array_self_reference_of_iterator_to_this_arbiter<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, _t_iterator>::test_self_reference_of_iterator_to_this (this, sItFirst, sItLast, nDist, nAppendix, bSelfReverse));
	}

	CBTreeArrayAccessWrapper<_t_data, _t_sizetype>		*m_pClAccessWrapper;

public:

	friend class CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype>;
	friend class CBTreeArrayAccessWrapper<_t_data, _t_sizetype>;
};

template <class _t_data, class _t_sizetype>
class CBTreeArrayConstAccessWrapper
{
public:
	
						CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype>
						(
							CBTreeArrayDataIf<_t_data, _t_sizetype>	&rInstance
						);

						~CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype> ();

	void				set							(const _t_sizetype nPos);

	_t_data &			operator* ();
	_t_data &			operator-> ();

						operator const _t_data&		();

protected:

	CBTreeArrayDataIf<_t_data, _t_sizetype>	&m_rInstance;
	_t_sizetype										m_nPos;
	_t_data											m_sData;
};

template <class _t_data, class _t_sizetype>
class CBTreeArrayAccessWrapper
	:	public CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype>
{
public:

						CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
						(
							CBTreeArrayDataIf<_t_data, _t_sizetype>	&rInstance
						);

						~CBTreeArrayAccessWrapper<_t_data, _t_sizetype> ();

	CBTreeArrayAccessWrapper<_t_data, _t_sizetype> &
						operator=					(const _t_data &rData);
						
	CBTreeArrayAccessWrapper<_t_data, _t_sizetype> &
						operator=					(const CBTreeArrayAccessWrapper<_t_data, _t_sizetype> &rData);

};

#endif // BTREEARRAY_H

#include "btreearray.cpp"
