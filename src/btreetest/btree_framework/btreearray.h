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
#include <algorithm>

#include "btreebasedefaults.h"

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
struct btree_array_self_reference_of_iterator_to_this_arbiter<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, typename CBTreeBaseDefaults<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator>
{
    static bool	test_self_reference_of_iterator_to_this
    (
    	CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *pClArray, 
		typename CBTreeBaseDefaults<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator &sItFirst, 
    	typename CBTreeBaseDefaults<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator &sItLast, 
    	_t_sizetype &nDist, 
    	_t_sizetype &nAppendix, 
		bool &bSelfReverse
    )
	{
		typedef CBTreeBaseDefaults<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		container_t;

		container_t		&rIter = dynamic_cast<container_t &> (*(sItLast.get_container ()));

		nDist = sItLast - sItFirst;
		nAppendix = rIter.end () - sItLast;

		bSelfReverse = false;

		return ((pClArray == sItFirst.get_container ()) || (pClArray == sItLast.get_container ()));
	}
};

template <class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
struct btree_array_self_reference_of_iterator_to_this_arbiter<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, typename CBTreeBaseDefaults<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator>
{
    static bool	test_self_reference_of_iterator_to_this
    (
    	CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *pClArray, 
		typename CBTreeBaseDefaults<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator &sItFirst, 
    	typename CBTreeBaseDefaults<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator &sItLast, 
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
struct btree_array_self_reference_of_iterator_to_this_arbiter<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, typename CBTreeBaseDefaults<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator>
{
    static bool	test_self_reference_of_iterator_to_this
    (
    	CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *pClArray, 
		typename CBTreeBaseDefaults<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator &sItFirst, 
    	typename CBTreeBaseDefaults<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reverse_iterator &sItLast, 
    	_t_sizetype &nDist, 
    	_t_sizetype &nAppendix, 
		bool &bSelfReverse
    )
	{
		typedef CBTreeBaseDefaults<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		container_t;

		bool		bSelfReverence = ((pClArray == sItFirst.base ().get_container ()) || (pClArray == sItLast.base ().get_container ()));

		container_t		&rIter = dynamic_cast<container_t &> (*(sItLast.base ().get_container ()));

		nDist = sItLast - sItFirst;
		nAppendix = rIter.rend () - sItLast;

		bSelfReverse = bSelfReverence;

		return (bSelfReverence);
	}
};

template <class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
struct btree_array_self_reference_of_iterator_to_this_arbiter<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, typename CBTreeBaseDefaults<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator>
{
    static bool	test_self_reference_of_iterator_to_this
    (
    	CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *pClArray, 
		typename CBTreeBaseDefaults<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator &sItFirst, 
    	typename CBTreeBaseDefaults<CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator &sItLast, 
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
class CBTreeArrayIf
	:	public virtual CBTreeIf<_t_data, _t_sizetype>
{
public:

	typedef CBTreeArrayPos <_t_sizetype>						position_t;

	typedef CBTreeIf<_t_data, _t_sizetype>						CBTreeIf_t;

	typedef typename CBTreeIf_t::iterator						iterator;
	typedef typename CBTreeIf_t::const_iterator					const_iterator;
	typedef typename CBTreeIf_t::reverse_iterator				reverse_iterator;
	typedef typename CBTreeIf_t::const_reverse_iterator			const_reverse_iterator;

	typedef _t_data												data_t;
	typedef _t_sizetype											size_type;
	
	// construction
						CBTreeArrayIf<_t_data, _t_sizetype>
													()
						{
						};
						
	// destruction
	virtual				~CBTreeArrayIf<_t_data, _t_sizetype>
													()
						{
						};

	virtual CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
						at							(const _t_sizetype nPos) = 0;

	virtual const _t_data &
						at							(const _t_sizetype nPos) const = 0;

	virtual CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
						front						() = 0;

	virtual const _t_data &
						front						() const = 0;

	virtual CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
						back						() = 0;

	virtual const _t_data &
						back						() const = 0;

	virtual void		assign						(_t_sizetype nNewSize, const _t_data& rVal) = 0;

	virtual	void		push_back					(const _t_data &rData) = 0;
	virtual	void		pop_back					() = 0;

	virtual iterator	insert						(const_iterator sCIterPos, const _t_data& rData) = 0;
	virtual iterator	insert						(const_iterator sCIterPos, const _t_sizetype nLen, const _t_data& rData) = 0;
	
	virtual iterator	erase						(const_iterator sCIterPos) = 0;
	virtual iterator	erase						(const_iterator sCIterFirst, const_iterator sCIterLast) = 0;

	virtual void		swap						(CBTreeArrayIf &rArrayIf) = 0;

	virtual void		clear						() = 0;

	virtual _t_sizetype	serialize					(const _t_sizetype nStart, const _t_sizetype nLen, _t_data *pData) const = 0;

	virtual CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
						operator[]					(const _t_sizetype nPos) = 0;

	virtual const _t_data &
						operator[]					(const _t_sizetype nPos) const = 0;

protected:

	virtual bool		set_at						(const _t_sizetype nPos, const _t_data &rData) = 0;
	virtual bool		get_at						(const _t_sizetype nPos, _t_data &rData) const = 0;

public:

	friend class CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype>;
	friend class CBTreeArrayAccessWrapper<_t_data, _t_sizetype>;

	friend class CBTreeIterator<CBTreeIf<_t_data, _t_sizetype> >;
	friend class CBTreeConstIterator<CBTreeIf<_t_data, _t_sizetype> >;
};

template <class _t_data, class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter> >
class CBTreeArray
	:	public virtual CBTreeArrayIf <_t_data, _t_sizetype>
	,	public CBTreeBaseDefaults <CBTreeArrayPos <_t_sizetype>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

	typedef CBTreeArrayPos<_t_sizetype>								position_t;

	typedef CBTreeArray												CBTreeArray_t;

	typedef CBTreeArrayIf<_t_data, _t_sizetype>			CBTreeArrayIf_t;

	typedef CBTreeBaseDefaults<position_t, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
														CBTreeBase_t;

	typedef typename CBTreeBase_t::CBTreeBaseIf_t		CBTreeBaseIf_t;

	typedef typename CBTreeBaseIf_t::CBTreeDefaults_t	CBTreeDefaults_t;

	typedef CBTreeIf<_t_data, _t_sizetype>	CBTreeIf_t;

	typedef typename CBTreeIf_t::iterator				iterator;
	typedef typename CBTreeIf_t::const_iterator			const_iterator;
	typedef typename CBTreeIf_t::reverse_iterator		reverse_iterator;
	typedef typename CBTreeIf_t::const_reverse_iterator	const_reverse_iterator;

	typedef	typename CBTreeBase_t::node_t				node_t;

	typedef _t_data										data_t;
	typedef _t_sizetype									size_type;
	typedef _t_nodeiter									nodeiter_t;
	typedef _t_subnodeiter								subnodeiter_t;
	typedef _t_datalayerproperties						datalayerproperties_t;
	typedef _t_datalayer								datalayer_t;

	typedef	typename CBTreeBaseIf_t::iterator_state_t	iterator_state_t;

	// construction
								CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
														(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

								CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
														(const CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign = true);
						
								CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
														(CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &&rRhBT);

	// destruction
	virtual						~CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
														();

	CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
								at						(const _t_sizetype nPos);

	const _t_data &				at						(const _t_sizetype nPos) const;

	CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
								front					();

	const _t_data &				front					() const;

	CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
								back					();

	const _t_data &				back					() const;

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

	void						swap					(CBTreeArrayIf_t &rArrayIf);
	void						swap					(CBTreeArray &rArray);

	void						clear					();

	_t_sizetype					serialize				(const _t_sizetype nStart, const _t_sizetype nLen, _t_data *pData) const;

	CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &
								operator=				(const CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT);

	CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &
								operator=				(CBTreeArray<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &&rRhBT);

	CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
								operator[]				(const _t_sizetype nPos);

	const _t_data&				operator[]				(const _t_sizetype nPos) const;

protected:

	virtual void				_assign					(const CBTreeIf_t &rContainer);

	bool						set_at					(const _t_sizetype nPos, const _t_data &rData);
	bool						get_at					(const _t_sizetype nPos, _t_data &rData) const;

	// node operations
	virtual CBTreeArrayPos<_t_sizetype>	determine_position	(CBTreeArrayPos<_t_sizetype> sPos, _t_nodeiter nNode, _t_subnodeiter &nSubPos, _t_subnodeiter &nSubData, bool &bFound) const;

	void						rebuild_node			(const _t_nodeiter nNode, const int32_t triMod = 0, _t_subnodeiter nSubStart = 0);

	virtual CBTreeArrayPos<_t_sizetype> generate_prev_position	(const _t_nodeiter nNode, const _t_subnodeiter nSub, CBTreeArrayPos<_t_sizetype> sPos);
	virtual CBTreeArrayPos<_t_sizetype> generate_next_position	(const _t_nodeiter nNode, const _t_subnodeiter nSub, CBTreeArrayPos<_t_sizetype> sPos);

	// manuvering
	virtual _t_subnodeiter		find_next_sub_pos		(const _t_nodeiter nNode, CBTreeArrayPos<_t_sizetype> &sPos) const;

	virtual bool				show_data				(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const;
	virtual bool				show_node				(std::ofstream &ofs, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const;

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

	void						_swap					(CBTreeArray &rArray);

	CBTreeArrayAccessWrapper<_t_data, _t_sizetype>		*m_pClAccessWrapper;

	_t_data												*m_psReturnData;

public:

	friend class CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype>;
	friend class CBTreeArrayAccessWrapper<_t_data, _t_sizetype>;

	friend class CBTreeIterator<CBTreeIf<_t_data, _t_sizetype> >;
	friend class CBTreeConstIterator<CBTreeIf<_t_data, _t_sizetype> >;
};

template <class _t_data, class _t_sizetype>
class CBTreeArrayConstAccessWrapper
{
public:
	
						CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype>
						(
							CBTreeArrayIf<_t_data, _t_sizetype>	&rInstance
						);

						~CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype> ();

	void				set							(const _t_sizetype nPos);

	_t_data &			operator* ();
	_t_data &			operator-> ();

						operator const _t_data&		();

protected:

	CBTreeArrayIf<_t_data, _t_sizetype>		&m_rInstance;
	_t_sizetype									m_nPos;
	_t_data										m_sData;
};

template <class _t_data, class _t_sizetype>
class CBTreeArrayAccessWrapper
	:	public CBTreeArrayConstAccessWrapper<_t_data, _t_sizetype>
{
public:

						CBTreeArrayAccessWrapper<_t_data, _t_sizetype>
						(
							CBTreeArrayIf<_t_data, _t_sizetype>	&rInstance
						);

						~CBTreeArrayAccessWrapper<_t_data, _t_sizetype> ();

	CBTreeArrayAccessWrapper<_t_data, _t_sizetype> &
						operator=					(const _t_data &rData);
						
	CBTreeArrayAccessWrapper<_t_data, _t_sizetype> &
						operator=					(const CBTreeArrayAccessWrapper<_t_data, _t_sizetype> &rData);

};

#endif // BTREEARRAY_H

#include "btreearray.cpp"
