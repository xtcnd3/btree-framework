/************************************************************
**
** file:	btreeassociative.h
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** abstract associative container interface class.
**
************************************************************/

#ifndef	BTREEASSOCIATIVE_H
#define	BTREEASSOCIATIVE_H

#include <iterator>
#include <list>

#include "base_class_stack/btreebasedefaults.h"

#include "iterators/btreeiter.h"

template<class _t_data, class _t_key, class _t_datalayerproperties>
class CBTreeAssociative;

template<class _t_sizetype, class _t_key>
class CBTreeKeySortPos;

/***********************************************************************/

template<class _t_data, class _t_key, class _t_datalayerproperties, class _t_iterator>
struct btree_associative_container_iterator_self_reference_arbiter
{
	static bool	test_self_reference_of_iterator_to_this
	(
		CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>	*pClKeySort, 
		_t_iterator &sItFirst, 
		_t_iterator &sItLast, 
		bool &bSelfReverse, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator	**ppsItFirst, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator	**ppsItLast
		
	)
	{
		pClKeySort;
		sItFirst;
		sItLast;

		bSelfReverse = false;

		if (ppsItFirst != NULL)
		{
			*ppsItFirst = NULL;
		}

		if (ppsItLast != NULL)
		{
			*ppsItLast = NULL;
		}

		return (false);
	}
};

template<class _t_data, class _t_key, class _t_datalayerproperties>
struct btree_associative_container_iterator_self_reference_arbiter<_t_data, _t_key, _t_datalayerproperties, typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::iterator >
{
	static bool	test_self_reference_of_iterator_to_this
	(
		CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>									*pClKeySort, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::iterator			&sItFirst, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::iterator			&sItLast, 
		bool																						&bSelfReverse, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator		**ppsItFirst, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator		**ppsItLast
	)
	{
		typedef typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator		citer_t;
		
		bool	bSelfReference = (pClKeySort == sItFirst.get_container ()) || (pClKeySort == sItLast.get_container ());

		bSelfReverse = false;

		if ((ppsItFirst != NULL) && bSelfReference)
		{
			(*ppsItFirst) = new citer_t (sItFirst);

			BTREE_ASSERT (((citer_t *) (*ppsItFirst)) != NULL, "btree_associative_container_iterator_self_reference_arbiter<..., const_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItFirst)");
		}

		if ((ppsItLast != NULL) && bSelfReference)
		{
			(*ppsItLast) = new citer_t (sItLast);

			BTREE_ASSERT (((citer_t *) (*ppsItLast)) != NULL, "btree_associative_container_iterator_self_reference_arbiter<..., const_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItLast)");
		}

		return (bSelfReference);
	}
};

template<class _t_data, class _t_key, class _t_datalayerproperties>
struct btree_associative_container_iterator_self_reference_arbiter<_t_data, _t_key, _t_datalayerproperties, typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator >
{
	static bool	test_self_reference_of_iterator_to_this
	(
		CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>									*pClKeySort, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator		&sItFirst, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator		&sItLast, 
		bool																						&bSelfReverse, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator		**ppsItFirst, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator		**ppsItLast
	)
	{
		typedef typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator		citer_t;
		
		bool	bSelfReference = (pClKeySort == sItFirst.get_container ()) || (pClKeySort == sItLast.get_container ());

		bSelfReverse = false;

		if ((ppsItFirst != NULL) && bSelfReference)
		{
			(*ppsItFirst) = new citer_t (sItFirst);

			BTREE_ASSERT (((citer_t *) (*ppsItFirst)) != NULL, "btree_associative_container_iterator_self_reference_arbiter<..., const_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItFirst)");
		}

		if ((ppsItLast != NULL) && bSelfReference)
		{
			(*ppsItLast) = new citer_t (sItLast);

			BTREE_ASSERT (((citer_t *) (*ppsItLast)) != NULL, "btree_associative_container_iterator_self_reference_arbiter<..., const_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItLast)");
		}

		return (bSelfReference);
	}
};

template<class _t_data, class _t_key, class _t_datalayerproperties>
struct btree_associative_container_iterator_self_reference_arbiter<_t_data, _t_key, _t_datalayerproperties, typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::reverse_iterator >
{
	static bool	test_self_reference_of_iterator_to_this
	(
		CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>										*pClKeySort, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::reverse_iterator		&sItFirst, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::reverse_iterator		&sItLast, 
		bool																							&bSelfReverse, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator			**ppsItFirst, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator			**ppsItLast
	)
	{
		typedef typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator		citer_t;
		
		bool	bSelfReference = (pClKeySort == sItFirst.base ().get_container ()) || (pClKeySort == sItLast.base ().get_container ());
				
		bSelfReverse = true;

		if ((ppsItFirst != NULL) && bSelfReference)
		{
			(*ppsItFirst) = new citer_t (sItLast.base ());

			BTREE_ASSERT (((citer_t *) (*ppsItFirst)) != NULL, "btree_associative_container_iterator_self_reference_arbiter<..., const_reverse_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItFirst)");

			((citer_t) (**ppsItFirst))--;
		}

		if ((ppsItLast != NULL) && bSelfReference)
		{
			(*ppsItLast) = new citer_t (sItFirst.base ());

			BTREE_ASSERT (((citer_t *) (*ppsItLast)) != NULL, "btree_associative_container_iterator_self_reference_arbiter<..., const_reverse_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItLast)");

			((citer_t) (**ppsItLast))--;
		}

		return (bSelfReference);
	}
};

template<class _t_data, class _t_key, class _t_datalayerproperties>
struct btree_associative_container_iterator_self_reference_arbiter<_t_data, _t_key, _t_datalayerproperties, typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_reverse_iterator >
{
	static bool	test_self_reference_of_iterator_to_this
	(
		CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>											*pClKeySort, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_reverse_iterator		&sItFirst, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_reverse_iterator		&sItLast, 
		bool																								&bSelfReverse, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator				**ppsItFirst, 
		typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator				**ppsItLast
	)
	{
		typedef typename CBTreeIf<_t_data, typename _t_datalayerproperties::size_type>::const_iterator		citer_t;
		
		bool	bSelfReference = (pClKeySort == sItFirst.base ().get_container ()) || (pClKeySort == sItLast.base ().get_container ());
				
		bSelfReverse = true;

		if ((ppsItFirst != NULL) && bSelfReference)
		{
			(*ppsItFirst) = new citer_t (sItLast.base ());

			BTREE_ASSERT (((citer_t *) (*ppsItFirst)) != NULL, "btree_associative_container_iterator_self_reference_arbiter<..., const_reverse_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItFirst)");

			((citer_t) (**ppsItFirst))--;
		}

		if ((ppsItLast != NULL) && bSelfReference)
		{
			(*ppsItLast) = new citer_t (sItFirst.base ());

			BTREE_ASSERT (((citer_t *) (*ppsItLast)) != NULL, "btree_associative_container_iterator_self_reference_arbiter<..., const_reverse_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItLast)");

			((citer_t) (**ppsItLast))--;
		}

		return (bSelfReference);
	}
};

/***********************************************************************/

template<class _t_data, class _t_key, class _t_datalayerproperties>
class CBTreeAssociative;

template<class _t_data, class _t_key = _t_data, class _t_sizetype = uint64_t>
class CBTreeAssociativeIf
	:	public virtual CBTreeIf<_t_data, _t_sizetype>
{
public:

	typedef _t_data												value_type;
	typedef _t_key												key_type;
	typedef _t_sizetype											size_type;
	
	typedef value_type&											reference;
	typedef const value_type&									const_reference;
	typedef value_type*											pointer;
	typedef const value_type*									const_pointer;
	typedef	typename ::std::make_signed<size_type>::type		difference_type;

	typedef CBTreeIf<value_type, size_type>						CBTreeIf_t;

	typedef typename CBTreeIf_t::iterator						iterator;
	typedef typename CBTreeIf_t::const_iterator					const_iterator;
	typedef typename CBTreeIf_t::reverse_iterator				reverse_iterator;
	typedef typename CBTreeIf_t::const_reverse_iterator			const_reverse_iterator;

	template<class _ti_keytype>
	struct key_compare_s
	{
		bool operator()(const _ti_keytype& rKey0, const _ti_keytype& rKey1) const
		{	
			return (pThis->comp (rKey0, rKey1) < 0);
		}

		CBTreeAssociativeIf		*pThis;
	};

	template<class _t_valuetype>
	struct value_compare_s
	{
		bool operator()(const _t_valuetype& rVal0, const _t_valuetype& rVal1) const
		{
			key_type		sKey0;
			key_type		sKey1;
			key_type		*pKey0 = pThis->extract_key (&sKey0, rVal0);
			key_type		*pKey1 = pThis->extract_key (&sKey1, rVal1);

			return (pThis->comp (*pKey0, *pKey1) < 0);
		}

		CBTreeAssociativeIf		*pThis;
	};

	typedef	struct key_compare_s<key_type>				key_compare;
	typedef struct value_compare_s<value_type>			value_compare;
	
	// construction
							CBTreeAssociativeIf<_t_data, _t_key, _t_sizetype>
														()
							{
							};
						
	// destruction
	virtual					~CBTreeAssociativeIf<_t_data, _t_key, _t_sizetype>
														()
							{
							};

	virtual iterator		insert						(const value_type &rData) = 0;

	virtual iterator		insert						(const_iterator sCIterHint, const value_type &rData) = 0;

	virtual void			insert						(iterator sItFirst, iterator sItLast) = 0;
	virtual void			insert						(const_iterator sItFirst, const_iterator sItLast) = 0;
	virtual void			insert						(reverse_iterator sItFirst, reverse_iterator sItLast) = 0;
	virtual void			insert						(const_reverse_iterator sItFirst, const_reverse_iterator sItLast) = 0;

	virtual	iterator		emplace						(value_type &&rData) = 0;
	virtual	iterator		emplace_hint				(const_iterator sCIterHint, value_type &&rData) = 0;

	virtual iterator		erase						(const_iterator sCIterPos) = 0;
	virtual size_type		erase						(const key_type &rKey) = 0;
	virtual iterator		erase						(const_iterator sCIterFirst, const_iterator sCIterLast) = 0;

	virtual void			swap						(CBTreeAssociativeIf &rContainer) = 0;

	virtual iterator		find						(const key_type &rKey) = 0;
	virtual const_iterator	find						(const key_type &rKey) const = 0;

	virtual iterator		lower_bound					(const key_type &rKey) = 0;
	virtual const_iterator	lower_bound					(const key_type &rKey) const = 0;

	virtual iterator		upper_bound					(const key_type &rKey) = 0;
	virtual const_iterator	upper_bound					(const key_type &rKey) const = 0;
	
	virtual void			get_next_key				(const key_type &rKey, key_type &rNextKey, bool &bBounce) const = 0;
	virtual void			get_prev_key				(const key_type &rKey, key_type &rPrevKey, bool &bBounce) const = 0;

	virtual size_type		count						(const key_type &rKey) const = 0;

	virtual key_compare		key_comp					() const = 0;
	virtual value_compare	value_comp					() const = 0;
	
	virtual size_type		serialize					(const size_type nStart, const size_type nLen, value_type *pData) const = 0;

	virtual bool			operator==					(const CBTreeAssociativeIf &rRhs) const = 0;
	virtual bool			operator!=					(const CBTreeAssociativeIf &rRhs) const = 0;

protected:

	virtual bool			get_at						(const size_type nPos, value_type &rData) const = 0;

	virtual size_type		get_init_pos_of_key			(const key_type &rKey) const = 0;
	
	virtual int				comp						(const key_type &rKey0, const key_type &rKey1) const = 0;

	virtual key_type		*extract_key				(key_type *pKey, const value_type &rData) const = 0;
};

/***********************************************************************/

template<class _t_sizetype, class _t_key>
class CBTreeKeySortPos
{
public:
	_t_key			*pKey;
	_t_sizetype		nInstance;
};

template<class _t_data, class _t_key = _t_data, class _t_datalayerproperties = CBTreeIOpropertiesRAM <> >
class CBTreeAssociative
	:	virtual public CBTreeAssociativeIf<_t_data, _t_key, typename _t_datalayerproperties::size_type>
	,	public CBTreeBaseDefaults <CBTreeKeySortPos <typename _t_datalayerproperties::size_type, _t_key>, _t_data, _t_datalayerproperties>
{
public:

	typedef _t_data													value_type;
	typedef _t_key													key_type;
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

	typedef CBTreeKeySortPos<size_type, key_type>					position_t;

	typedef CBTreeAssociative										CBTreeAssociative_t;

	typedef CBTreeAssociativeIf<value_type, key_type, size_type>	CBTreeAssociativeIf_t;

	typedef CBTreeBaseDefaults<position_t, value_type, data_layer_properties_type>
																	CBTreeBaseDefaults_t;

	typedef typename CBTreeBaseDefaults_t::CBTreeBaseIf_t			CBTreeBaseIf_t;

	typedef CBTreeIf<_t_data, size_type>							CBTreeIf_t;

	typedef	typename CBTreeBaseDefaults_t::node_t					node_t;

	typedef	typename CBTreeBaseDefaults_t::iterator					iterator;
	typedef	typename CBTreeBaseDefaults_t::const_iterator			const_iterator;
	typedef	typename CBTreeBaseDefaults_t::reverse_iterator			reverse_iterator;
	typedef	typename CBTreeBaseDefaults_t::const_reverse_iterator	const_reverse_iterator;

	typedef	typename CBTreeBaseDefaults_t::iterator_state_t			iterator_state_t;

	typedef	typename CBTreeAssociativeIf_t::key_compare				key_compare;
	typedef typename CBTreeAssociativeIf_t::value_compare			value_compare;

	// construction
						CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>
													(_t_datalayerproperties &rDataLayerProperties, sub_node_iter_type nNodeSize);

						CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>
													(const CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties> &rBT, bool bAssign = true);

	// destruction
	virtual				~CBTreeAssociative<_t_data, _t_key, _t_datalayerproperties>
													();
	
	iterator			insert						(const value_type &rData);

	iterator			insert						(const_iterator sCIterHint, const value_type &rData);

	void				insert						(iterator sItFirst, iterator sItLast);
	void				insert						(const_iterator sItFirst, const_iterator sItLast);
	void				insert						(reverse_iterator sItFirst, reverse_iterator sItLast);
	void				insert						(const_reverse_iterator sItFirst, const_reverse_iterator sItLast);

	template<class _t_iterator>
	void				insert						(_t_iterator sItFirst, _t_iterator sItLast);

	iterator			emplace						(value_type &&rData);
	iterator			emplace_hint				(const_iterator sCIterHint, value_type &&rData);

	template<class ..._t_va_args>
	iterator			emplace						(_t_va_args && ... rrArgs);
	
	template<class ..._t_va_args>
	iterator			emplace_hint				(const_iterator sCIterHint, _t_va_args && ... rrArgs);

	iterator			erase						(const_iterator sCIterPos);
	size_type			erase						(const key_type &rKey);
	iterator			erase						(const_iterator sCIterFirst, const_iterator sCIterLast);
	
	iterator			find						(const key_type &rKey);
	const_iterator		find						(const key_type &rKey) const;

	iterator			lower_bound					(const key_type &rKey);
	const_iterator		lower_bound					(const key_type &rKey) const;

	iterator			upper_bound					(const key_type &rKey);
	const_iterator		upper_bound					(const key_type &rKey) const;

	void				get_next_key				(const key_type &rKey, key_type &rNextKey, bool &bBounce) const;
	void				get_prev_key				(const key_type &rKey, key_type &rPrevKey, bool &bBounce) const;
	
	size_type			count						(const key_type &rKey) const;

	key_compare			key_comp					() const;
	value_compare		value_comp					() const;
	
	size_type			serialize					(const size_type nStart, const size_type nLen, value_type *pData) const;

	CBTreeAssociative &	operator=					(const CBTreeAssociative &rBT);

	bool				operator==					(const CBTreeAssociativeIf_t &rRhs) const;
	bool				operator!=					(const CBTreeAssociativeIf_t &rRhs) const;

protected:

	virtual void		_assign						(const CBTreeIf_t &rContainer);

	bool				get_at						(const size_type nPos, value_type &rData) const;
	
//	bool				get							(const key_type &rKey, size_type nInstance, value_type *pObj) const;
	size_type			get_init_pos_of_key			(const key_type &rKey) const;
	
	// node operations
	virtual position_t	determine_position			(position_t sPos, node_iter_type nNode, sub_node_iter_type &nSubPos, sub_node_iter_type &nSubData, bool &bFound) const;

	void				rebuild_node				(const node_iter_type nNode, const int32_t triMod = 0, sub_node_iter_type nSubStart = 0);

	virtual position_t	generate_prev_position		(const node_iter_type nNode, const sub_node_iter_type nSub, position_t sPos);
	virtual position_t	generate_next_position		(const node_iter_type nNode, const sub_node_iter_type nSub, position_t sPos);

	// testing
	virtual int			comp						(const key_type &rKey0, const key_type &rKey1) const;

	// data retrieval
	virtual key_type	*extract_key				(key_type *pKey, const node_iter_type nNode, const sub_node_iter_type nEntry) const;

	virtual key_type	*extract_key				(key_type *pKey, const value_type &rData) const;

	inline key_type		*extract_dataset_as_key		(key_type *pKey, const value_type &rData, typename ::std::true_type) const;
	inline key_type		*extract_dataset_as_key		(key_type *pKey, const value_type &rData, typename ::std::false_type) const;

	inline key_type		*extract_key_directly		(key_type *pKey, const value_type &rData, typename ::std::true_type) const;
	inline key_type		*extract_key_directly		(key_type *pKey, const value_type &rData, typename ::std::false_type) const;

	inline key_type		*extract_key				(key_type *pKey, const value_type &rData, typename ::std::true_type) const;
	inline key_type		*extract_key				(key_type *pKey, const value_type &rData, typename ::std::false_type) const;

	inline bool			compare_entry				(const value_type &rLhs, const value_type &rRhs, typename ::std::true_type) const;
	inline bool			compare_entry				(const value_type &rLhs, const value_type &rRhs, typename ::std::false_type) const;

	size_type			get_instancePos				(const node_iter_type nNode, const sub_node_iter_type nSub) const;

	// manuvering
	virtual sub_node_iter_type	get_firstSubPos		(const node_iter_type nNode, const key_type &rKey, bool bReverse = false) const;
	virtual sub_node_iter_type	find_next_sub_pos	(const node_iter_type nNode, position_t &sPos) const;

	virtual bool		find_oneKey					(const key_type &rKey, node_iter_type &nNode, sub_node_iter_type &nSub, size_type *pnPos = NULL) const;
	virtual size_type	find_firstKey				(const node_iter_type nFromNode, const sub_node_iter_type nFromSub, node_iter_type &nNode, sub_node_iter_type &nSub) const;

	void				erase_via_reference			(const_iterator &rCIterPos, bool bReEvaluate = true);

	inline bool			_find						(const key_type &rKey, node_iter_type &rnNode, sub_node_iter_type &rnSub, size_type &rnPos) const;
	
	inline void			_lower_bound				(const key_type &rKey, node_iter_type &rnNode, sub_node_iter_type &rnSub, size_type &rnPos) const;
	inline bool			_upper_bound				(const key_type &rKey, node_iter_type &rnNode, sub_node_iter_type &rnSub, size_type &rnPos) const;

	// variable size buffer service
	void				vbufferAllocate				(key_type **pp);
	void				vbufferDeallocate			(key_type **pp);

	void				allocateShortLiveKey		() const;
	void				freeShortLiveKey			() const;

	virtual bool		show_data					(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const node_iter_type nNode, const sub_node_iter_type nSubPos) const;
	virtual bool		show_node					(std::ofstream &ofs, const node_iter_type nNode, const sub_node_iter_type nSubPos) const;

	void				_swap						(CBTreeAssociative &rContainer);

	template<class _t_iterator>
	bool				test_self_reference_of_iterator_to_this (_t_iterator &sItFirst, _t_iterator &sItLast, bool &bSelfReverse, const_iterator **ppsItFirst, const_iterator **ppsItLast)
	{
		return (btree_associative_container_iterator_self_reference_arbiter<value_type, key_type, data_layer_properties_type, _t_iterator>::test_self_reference_of_iterator_to_this (this, sItFirst, sItLast, bSelfReverse, ppsItFirst, ppsItLast));
	}

	// protected variables

	// variable size buffers
	key_type										**m_ppShortLiveKey;
	key_type										**m_ppTempFindFirstKeyKey;

	key_type										*m_pRemoveKey;
	key_type										*m_pTempRemoveKey;
	key_type										*m_pInstancesNewKey;
	key_type										*m_pTempFindFirstKeyNewKey;
	key_type										*m_pAddToNodeKey;
	key_type										*m_pGetNewKey;

#if defined (_DEBUG)

	bool											*m_pbShortLiveKeyInUse;

#endif
};

#endif // BTREEASSOCIATIVE_H

#include "btreeassociative.cpp"
