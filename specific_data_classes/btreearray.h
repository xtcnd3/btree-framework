/************************************************************
**
** file:	btreearray.h
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's array data class.
**
************************************************************/

#ifndef	BTREEARRAY_H
#define	BTREEARRAY_H

#if defined(__GNUC__) || defined(__GNUG__)

#define	_FILE_OFFSET_BITS	64

#endif

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <fstream>
#include <iterator>
#include <algorithm>
#include <functional>

#include "base_class_stack/btreebasedefaults.h"

#include "common/btreecommon.h"
#include "super_classes/btreeaux.h"
#include "iterators/btreeiter.h"

template<class _t_data, class _t_sizetype>
class CBTreeArrayConstAccessWrapper;

template<class _t_data, class _t_sizetype>
class CBTreeArrayAccessWrapper;

template<class _t_sizetype>
class CBTreeArrayPos
{
public:
	_t_sizetype			nIndex;
};

template<class _t_data, class _t_datalayerproperties>
class CBTreeArray;

/***********************************************************************/

template<class _t_data, class _t_datalayerproperties, class _t_iterator>
struct btree_array_self_reference_of_iterator_to_this_arbiter
{
	static bool	test_self_reference_of_iterator_to_this
	(
		CBTreeIf<_t_data, typename _t_datalayerproperties::size_type> *pClArray,
		_t_iterator &sItFirst,
		_t_iterator &sItLast,
		typename _t_datalayerproperties::size_type &nDist,
		typename _t_datalayerproperties::size_type &nAppendix,
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

template<class _t_data, class _t_datalayerproperties>
struct btree_array_self_reference_of_iterator_to_this_arbiter<_t_data, _t_datalayerproperties, typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::iterator>
{
    static bool	test_self_reference_of_iterator_to_this
    (
    	CBTreeIf<_t_data, typename _t_datalayerproperties::size_type> *pClArray,
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::iterator &sItFirst,
    	typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::iterator &sItLast,
    	typename _t_datalayerproperties::size_type &nDist,
    	typename _t_datalayerproperties::size_type &nAppendix,
		bool &bSelfReverse
    )
	{
		typedef CBTreeBaseDefaults<CBTreeArrayPos <typename _t_datalayerproperties::size_type>, _t_data, _t_datalayerproperties>		container_t;

		container_t		&rIter = dynamic_cast<container_t &> (*(sItLast.get_container ()));

		nDist = sItLast - sItFirst;
		nAppendix = rIter.end () - sItLast;

		bSelfReverse = false;

		return ((pClArray == sItFirst.get_container ()) || (pClArray == sItLast.get_container ()));
	}
};

template<class _t_data, class _t_datalayerproperties>
struct btree_array_self_reference_of_iterator_to_this_arbiter<_t_data, _t_datalayerproperties, typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator>
{
    static bool	test_self_reference_of_iterator_to_this
    (
    	CBTreeIf<_t_data, typename _t_datalayerproperties::size_type> *pClArray,
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator &sItFirst,
    	typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator &sItLast,
    	typename _t_datalayerproperties::size_type &nDist,
    	typename _t_datalayerproperties::size_type &nAppendix,
		bool &bSelfReverse
    )
	{
		nDist = sItLast - sItFirst;
		nAppendix = sItLast.get_container ()->cend () - sItLast;

		bSelfReverse = false;

		return ((pClArray == sItFirst.get_container ()) || (pClArray == sItLast.get_container ()));
	}
};

template<class _t_data, class _t_datalayerproperties>
struct btree_array_self_reference_of_iterator_to_this_arbiter<_t_data, _t_datalayerproperties, typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::reverse_iterator>
{
    static bool	test_self_reference_of_iterator_to_this
    (
    	CBTreeIf<_t_data, typename _t_datalayerproperties::size_type> *pClArray,
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::reverse_iterator &sItFirst,
    	typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::reverse_iterator &sItLast,
    	typename _t_datalayerproperties::size_type &nDist,
    	typename _t_datalayerproperties::size_type &nAppendix,
		bool &bSelfReverse
    )
	{
		typedef CBTreeBaseDefaults<CBTreeArrayPos <typename _t_datalayerproperties::size_type>, _t_data, _t_datalayerproperties>		container_t;

		bool		bSelfReference = ((pClArray == sItFirst.base ().get_container ()) || (pClArray == sItLast.base ().get_container ()));

		container_t		&rIter = dynamic_cast<container_t &> (*(sItLast.base ().get_container ()));

		nDist = sItLast - sItFirst;
		nAppendix = rIter.rend () - sItLast;

		bSelfReverse = bSelfReference;

		return (bSelfReference);
	}
};

template<class _t_data, class _t_datalayerproperties>
struct btree_array_self_reference_of_iterator_to_this_arbiter<_t_data, _t_datalayerproperties, typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_reverse_iterator>
{
    static bool	test_self_reference_of_iterator_to_this
    (
    	CBTreeIf<_t_data, typename _t_datalayerproperties::size_type> *pClArray,
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_reverse_iterator &sItFirst,
    	typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_reverse_iterator &sItLast,
    	typename _t_datalayerproperties::size_type &nDist,
    	typename _t_datalayerproperties::size_type &nAppendix,
		bool &bSelfReverse
    )
	{
		bool		bSelfReference = ((pClArray == sItFirst.base ().get_container ()) || (pClArray == sItLast.base ().get_container ()));

		nDist = sItLast - sItFirst;
		nAppendix = sItLast.base ().get_container ()->crend () - sItLast;

		bSelfReverse = bSelfReference;

		return (bSelfReference);
	}
};

/***********************************************************************/

template<class _t_data, class _t_sizetype = uint64_t>
class CBTreeArrayIf
	:	public virtual CBTreeIf<_t_data, _t_sizetype>
{
public:

	typedef _t_data												value_type;
	typedef _t_sizetype											size_type;

	typedef value_type&											reference;
	typedef const value_type&									const_reference;
	typedef value_type*											pointer;
	typedef const value_type*									const_pointer;
	typedef	typename ::std::make_signed<size_type>::type		difference_type;

	typedef CBTreeArrayPos <size_type>							position_t;

	typedef CBTreeIf<value_type, size_type>						CBTreeIf_t;

	typedef typename CBTreeIf_t::iterator						iterator;
	typedef typename CBTreeIf_t::const_iterator					const_iterator;
	typedef typename CBTreeIf_t::reverse_iterator				reverse_iterator;
	typedef typename CBTreeIf_t::const_reverse_iterator			const_reverse_iterator;

	// construction
						CBTreeArrayIf<value_type, size_type>
													()
						{
						};

	// destruction
	virtual				~CBTreeArrayIf<value_type, size_type>
													()
						{
						};

	virtual void		assign						(const size_type nNewSize, const value_type& rVal) = 0;

	virtual	void		push_back					(const value_type &rData) = 0;
	virtual	void		pop_back					() = 0;

	virtual iterator	insert						(const_iterator sCIterPos, const value_type& rData) = 0;
	virtual iterator	insert						(const_iterator sCIterPos, const _t_sizetype nLen, const value_type& rData) = 0;

	virtual iterator	emplace						(const_iterator sCIterPos, value_type &&rData) = 0;
	virtual iterator	emplace_back				(value_type &&rData) = 0;

	virtual iterator	erase						(const_iterator sCIterPos) = 0;
	virtual iterator	erase						(const_iterator sCIterFirst, const_iterator sCIterLast) = 0;

	virtual void		swap						(CBTreeArrayIf &rArrayIf) = 0;

	virtual void		clear						() = 0;

	virtual _t_sizetype	serialize					(const _t_sizetype nStart, const _t_sizetype nLen, value_type *pData) const = 0;

	virtual CBTreeArrayAccessWrapper<value_type, _t_sizetype>
						operator[]					(const _t_sizetype nPos) = 0;

	virtual const value_type &
						operator[]					(const _t_sizetype nPos) const = 0;

	virtual bool		operator==					(const CBTreeArrayIf &rRhs) const = 0;
	virtual bool		operator!=					(const CBTreeArrayIf &rRhs) const = 0;

protected:

	virtual bool		set_at						(const _t_sizetype nPos, const value_type &rData) = 0;
	virtual bool		get_at						(const _t_sizetype nPos, value_type &rData) const = 0;

public:

	friend class CBTreeArrayConstAccessWrapper<value_type, size_type>;
	friend class CBTreeArrayAccessWrapper<value_type, size_type>;

	friend class CBTreeIterator<CBTreeIf_t>;
	friend class CBTreeConstIterator<CBTreeIf_t>;
};

template<class _t_data, class _t_datalayerproperties = CBTreeIOpropertiesRAM <> >
class CBTreeArray
	:	public virtual CBTreeArrayIf <_t_data, typename _t_datalayerproperties::size_type>
	,	public CBTreeBaseDefaults <CBTreeArrayPos <typename _t_datalayerproperties::size_type>, _t_data, _t_datalayerproperties>
{
public:

	typedef _t_data													value_type;
	typedef typename _t_datalayerproperties::size_type				size_type;
	typedef typename _t_datalayerproperties::node_iter_type			node_iter_type;
	typedef typename _t_datalayerproperties::sub_node_iter_type		sub_node_iter_type;
	typedef _t_datalayerproperties									data_layer_properties_type;
	typedef typename _t_datalayerproperties::data_layer_type		data_layer_type;

	typedef value_type&												reference;
	typedef const value_type&										const_reference;
	typedef value_type*												pointer;
	typedef const value_type*										const_pointer;
	typedef	typename ::std::make_signed<size_type>::type			difference_type;

	typedef CBTreeArrayPos<size_type>								position_t;

	typedef CBTreeArray												CBTreeArray_t;

	typedef CBTreeArrayIf<value_type, size_type>					CBTreeArrayIf_t;

	typedef CBTreeBaseDefaults<position_t, value_type, _t_datalayerproperties>
																	CBTreeBaseDefaults_t;

	typedef CBTreeIf<value_type, size_type>							CBTreeIf_t;

	typedef typename CBTreeIf_t::iterator							iterator;
	typedef typename CBTreeIf_t::const_iterator						const_iterator;
	typedef typename CBTreeIf_t::reverse_iterator					reverse_iterator;
	typedef typename CBTreeIf_t::const_reverse_iterator				const_reverse_iterator;

	typedef	typename CBTreeBaseDefaults_t::node_t					node_t;

	typedef	typename CBTreeBaseDefaults_t::iterator_state_t			iterator_state_t;

	// construction
								CBTreeArray<_t_data, _t_datalayerproperties>
														(const _t_datalayerproperties &rDataLayerProperties, const sub_node_iter_type nNodeSize);

								CBTreeArray<_t_data, _t_datalayerproperties>
														(const CBTreeArray<_t_data, _t_datalayerproperties> &rContainer, const bool bAssign = true);

								CBTreeArray<_t_data, _t_datalayerproperties>
														(CBTreeArray<_t_data, _t_datalayerproperties> &&rRhsContainer);

	// destruction
	virtual						~CBTreeArray<_t_data, _t_datalayerproperties>
														();

	template<class _t_iterator>
	void						assign					(_t_iterator sItFirst, _t_iterator sItLast);
	void						assign					(const size_type nNewSize, const value_type& rVal);

	void						push_back				(const value_type &rData);

	void						pop_back				();

	iterator					insert					(const_iterator sCIterPos, const value_type& rData);
	iterator					insert					(const_iterator sCIterPos, const size_type nLen, const value_type& rData);

	template<class _t_iterator>
	iterator					insert					(const_iterator sCIterPos, _t_iterator sItFirst, _t_iterator sItLast);

	iterator					emplace					(const_iterator sCIterPos, value_type &&rData);
	iterator					emplace_back			(value_type &&rData);

	template<class ..._t_va_args>
	iterator					emplace					(const_iterator sCIterPos, _t_va_args && ... rrArgs);
	
	template<class ..._t_va_args>
	iterator					emplace_back			(_t_va_args && ... rrArgs);

	iterator					erase					(const_iterator sCIterPos);
	iterator					erase					(const_iterator sCIterFirst, const_iterator sCIterLast);

	void						swap					(CBTreeArrayIf_t &rArrayIf);
	void						swap					(CBTreeArray &rArray);

	void						clear					();

	size_type					serialize				(const size_type nStart, const size_type nLen, value_type *pData) const;

	CBTreeArray<_t_data, _t_datalayerproperties> &
								operator=				(const CBTreeArray<_t_data, _t_datalayerproperties> &rContainer);

	CBTreeArray<_t_data, _t_datalayerproperties> &
								operator=				(CBTreeArray<_t_data, _t_datalayerproperties> &&rRhsContainer);

	CBTreeArrayAccessWrapper<value_type, size_type>
								operator[]				(const size_type nPos);

	const value_type&			operator[]				(const size_type nPos) const;

	bool						operator==				(const CBTreeArrayIf_t &rRhs) const;
	bool						operator!=				(const CBTreeArrayIf_t &rRhs) const;

protected:

	virtual void				_assign					(const CBTreeIf_t &rContainer);

	bool						set_at					(const size_type nPos, const value_type &rData);
	bool						get_at					(const size_type nPos, value_type &rData) const;

	// node operations
	virtual position_t			determine_position		(position_t sPos, node_iter_type nNode, sub_node_iter_type &nSubPos, sub_node_iter_type &nSubData, bool &bFound) const;

	void						rebuild_node			(const node_iter_type nNode, const int32_t triMod = 0, sub_node_iter_type nSubStart = 0);

	virtual position_t			generate_prev_position	(const node_iter_type nNode, const sub_node_iter_type nSub, position_t sPos) const;
	virtual position_t			generate_next_position	(const node_iter_type nNode, const sub_node_iter_type nSub, position_t sPos) const;

	// manuvering
	virtual sub_node_iter_type		find_next_sub_pos	(const node_iter_type nNode, position_t &sPos) const;

	virtual bool				show_data				(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const node_iter_type nNode, const sub_node_iter_type nSubPos) const;
	virtual bool				show_node				(std::ofstream &ofs, const node_iter_type nNode, const sub_node_iter_type nSubPos) const;

	template<class _t_iterator>
	iterator					insert_via_iterator_tag (const_iterator sCIterPos, _t_iterator sItFirst, _t_iterator sItLast, ::std::forward_iterator_tag);

	template<class _t_iterator>
	iterator					insert_via_iterator_tag (const_iterator sCIterPos, _t_iterator sItFirst, _t_iterator sItLast, ::std::random_access_iterator_tag);

	void						insert_via_reference	(const_iterator &rCIterPos, const value_type& rData, bool bCopyNotMove, bool bReEvaluate = true);

	void						erase_via_reference		(const_iterator &rCIterPos, bool bReEvaluate = true);

	template<class _t_iterator>
	bool						test_self_reference_of_iterator_to_this (_t_iterator &sItFirst, _t_iterator &sItLast, size_type &nDist, size_type &nAppendix, bool &bSelfReverse)
	{
		return (btree_array_self_reference_of_iterator_to_this_arbiter<value_type, _t_datalayerproperties, _t_iterator>::test_self_reference_of_iterator_to_this (this, sItFirst, sItLast, nDist, nAppendix, bSelfReverse));
	}

	void						_swap					(CBTreeArray &rArray);

	void						_local_swap				(CBTreeArray &rArray);

	bool						compare_value_type		(const value_type & rLhs, const value_type & rRhs, ::std::true_type) const;
	bool						compare_value_type		(const value_type & rLhs, const value_type & rRhs, ::std::false_type) const;

	CBTreeArrayAccessWrapper<value_type, size_type>		*m_pClAccessWrapper;

	value_type											*m_psReturnData;

public:

	friend class CBTreeArrayConstAccessWrapper<value_type, size_type>;
	friend class CBTreeArrayAccessWrapper<value_type, size_type>;

	friend class CBTreeIterator<CBTreeIf_t>;
	friend class CBTreeConstIterator<CBTreeIf_t>;
};

template<class _t_data, class _t_sizetype>
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

	CBTreeArrayIf<_t_data, _t_sizetype>			&m_rInstance;
	_t_sizetype									m_nPos;
	_t_data										m_sData;
};

template<class _t_data, class _t_sizetype>
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

