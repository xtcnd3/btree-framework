/************************************************************
**
** file:	btreeassociative.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** key sort interface class.
**
************************************************************/

#ifndef	BTREEASSOCIATIVE_H
#define	BTREEASSOCIATIVE_H

#include <iterator>
#include <list>

#include "btreebasedefaults.h"

#include "btreeiter.h"

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
class CBTreeAssociative;

template <class _t_sizetype, class _t_key>
class CBTreeKeySortPos;

/***********************************************************************/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer, class _t_iterator>
struct btree_keysort_self_reference_of_iterator_to_this_arbiter
{
	static bool	test_self_reference_of_iterator_to_this
	(
		CBTreeAssociative<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>	*pClKeySort, 
		_t_iterator &sItFirst, 
		_t_iterator &sItLast, 
		bool &bSelfReverse, 
		typename CBTreeIf<_t_data, _t_sizetype>::const_iterator			**ppsItFirst, 
		typename CBTreeIf<_t_data, _t_sizetype>::const_iterator			**ppsItLast
		
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

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
struct btree_keysort_self_reference_of_iterator_to_this_arbiter<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, typename CBTreeIf<_t_data, _t_sizetype>::iterator >
{
	static bool	test_self_reference_of_iterator_to_this
	(
		CBTreeAssociative<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		*pClKeySort, 
		typename CBTreeIf<_t_data, _t_sizetype>::iterator					&sItFirst, 
		typename CBTreeIf<_t_data, _t_sizetype>::iterator					&sItLast, 
		bool &bSelfReverse, 
		typename CBTreeIf<_t_data, _t_sizetype>::const_iterator				**ppsItFirst, 
		typename CBTreeIf<_t_data, _t_sizetype>::const_iterator				**ppsItLast
	)
	{
		typedef typename CBTreeIf<_t_data, _t_sizetype>::const_iterator		citer_t;
		
		bool	bSelfReference = (pClKeySort == sItFirst.get_container ()) || (pClKeySort == sItLast.get_container ());

		bSelfReverse = false;

		if ((ppsItFirst != NULL) && bSelfReference)
		{
			(*ppsItFirst) = new citer_t (sItFirst);

			BTREE_ASSERT (((citer_t *) (*ppsItFirst)) != NULL, "btree_keysort_self_reference_of_iterator_to_this_arbiter<..., const_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItFirst)");
		}

		if ((ppsItLast != NULL) && bSelfReference)
		{
			(*ppsItLast) = new citer_t (sItLast);

			BTREE_ASSERT (((citer_t *) (*ppsItLast)) != NULL, "btree_keysort_self_reference_of_iterator_to_this_arbiter<..., const_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItLast)");
		}

		return (bSelfReference);
	}
};

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
struct btree_keysort_self_reference_of_iterator_to_this_arbiter<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, typename CBTreeIf<_t_data, _t_sizetype>::const_iterator >
{
	static bool	test_self_reference_of_iterator_to_this
	(
		CBTreeAssociative<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		*pClKeySort, 
		typename CBTreeIf<_t_data, _t_sizetype>::const_iterator				&sItFirst, 
		typename CBTreeIf<_t_data, _t_sizetype>::const_iterator				&sItLast, 
		bool &bSelfReverse, 
		typename CBTreeIf<_t_data, _t_sizetype>::const_iterator				**ppsItFirst, 
		typename CBTreeIf<_t_data, _t_sizetype>::const_iterator				**ppsItLast
	)
	{
		typedef typename CBTreeIf<_t_data, _t_sizetype>::const_iterator		citer_t;
		
		bool	bSelfReference = (pClKeySort == sItFirst.get_container ()) || (pClKeySort == sItLast.get_container ());

		bSelfReverse = false;

		if ((ppsItFirst != NULL) && bSelfReference)
		{
			(*ppsItFirst) = new citer_t (sItFirst);

			BTREE_ASSERT (((citer_t *) (*ppsItFirst)) != NULL, "btree_keysort_self_reference_of_iterator_to_this_arbiter<..., const_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItFirst)");
		}

		if ((ppsItLast != NULL) && bSelfReference)
		{
			(*ppsItLast) = new citer_t (sItLast);

			BTREE_ASSERT (((citer_t *) (*ppsItLast)) != NULL, "btree_keysort_self_reference_of_iterator_to_this_arbiter<..., const_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItLast)");
		}

		return (bSelfReference);
	}
};

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
struct btree_keysort_self_reference_of_iterator_to_this_arbiter<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, typename CBTreeIf<_t_data, _t_sizetype>::reverse_iterator >
{
	static bool	test_self_reference_of_iterator_to_this
	(
		CBTreeAssociative<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		*pClKeySort, 
		typename CBTreeIf<_t_data, _t_sizetype>::reverse_iterator			&sItFirst, 
		typename CBTreeIf<_t_data, _t_sizetype>::reverse_iterator			&sItLast, 
		bool &bSelfReverse, 
		typename CBTreeIf<_t_data, _t_sizetype>::const_iterator				**ppsItFirst, 
		typename CBTreeIf<_t_data, _t_sizetype>::const_iterator				**ppsItLast
	)
	{
		typedef typename CBTreeIf<_t_data, _t_sizetype>::const_iterator		citer_t;
		
		bool	bSelfReference = (pClKeySort == sItFirst.base ().get_container ()) || (pClKeySort == sItLast.base ().get_container ());
				
		bSelfReverse = true;

		if ((ppsItFirst != NULL) && bSelfReference)
		{
			(*ppsItFirst) = new citer_t (sItLast.base ());

			BTREE_ASSERT (((citer_t *) (*ppsItFirst)) != NULL, "btree_keysort_self_reference_of_iterator_to_this_arbiter<..., const_reverse_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItFirst)");

			((citer_t) (**ppsItFirst))--;
		}

		if ((ppsItLast != NULL) && bSelfReference)
		{
			(*ppsItLast) = new citer_t (sItFirst.base ());

			BTREE_ASSERT (((citer_t *) (*ppsItLast)) != NULL, "btree_keysort_self_reference_of_iterator_to_this_arbiter<..., const_reverse_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItLast)");

			((citer_t) (**ppsItLast))--;
		}

		return (bSelfReference);
	}
};

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
struct btree_keysort_self_reference_of_iterator_to_this_arbiter<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, typename CBTreeIf<_t_data, _t_sizetype>::const_reverse_iterator >
{
	static bool	test_self_reference_of_iterator_to_this
	(
		CBTreeAssociative<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		*pClKeySort, 
		typename CBTreeIf<_t_data, _t_sizetype>::const_reverse_iterator		&sItFirst, 
		typename CBTreeIf<_t_data, _t_sizetype>::const_reverse_iterator		&sItLast, 
		bool &bSelfReverse, 
		typename CBTreeIf<_t_data, _t_sizetype>::const_iterator				**ppsItFirst, 
		typename CBTreeIf<_t_data, _t_sizetype>::const_iterator				**ppsItLast
	)
	{
		typedef typename CBTreeIf<_t_data, _t_sizetype>::const_iterator		citer_t;
		
		bool	bSelfReference = (pClKeySort == sItFirst.base ().get_container ()) || (pClKeySort == sItLast.base ().get_container ());
				
		bSelfReverse = true;

		if ((ppsItFirst != NULL) && bSelfReference)
		{
			(*ppsItFirst) = new citer_t (sItLast.base ());

			BTREE_ASSERT (((citer_t *) (*ppsItFirst)) != NULL, "btree_keysort_self_reference_of_iterator_to_this_arbiter<..., const_reverse_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItFirst)");

			((citer_t) (**ppsItFirst))--;
		}

		if ((ppsItLast != NULL) && bSelfReference)
		{
			(*ppsItLast) = new citer_t (sItFirst.base ());

			BTREE_ASSERT (((citer_t *) (*ppsItLast)) != NULL, "btree_keysort_self_reference_of_iterator_to_this_arbiter<..., const_reverse_iterator>::test_self_reference_of_iterator_to_this (): insufficient memory! (ppsItLast)");

			((citer_t) (**ppsItLast))--;
		}

		return (bSelfReference);
	}
};

/***********************************************************************/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
class CBTreeAssociative;

template <class _t_data, class _t_key = _t_data, class _t_sizetype = uint64_t>
class CBTreeAssociativeIf
	:	public virtual CBTreeIf<_t_data, _t_sizetype>
{
public:

	typedef CBTreeIf<_t_data, _t_sizetype>						CBTreeIf_t;

	typedef typename CBTreeIf_t::iterator						iterator;
	typedef typename CBTreeIf_t::const_iterator					const_iterator;
	typedef typename CBTreeIf_t::reverse_iterator				reverse_iterator;
	typedef typename CBTreeIf_t::const_reverse_iterator			const_reverse_iterator;

	typedef _t_data												value_type;

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
			_t_key		sKey0;
			_t_key		sKey1;
			_t_key		*pKey0 = pThis->extract_key (&sKey0, rVal0);
			_t_key		*pKey1 = pThis->extract_key (&sKey1, rVal1);

			return (pThis->comp (*pKey0, *pKey1) < 0);
		}

		CBTreeAssociativeIf		*pThis;
	};

	typedef	struct key_compare_s<_t_key>				key_compare;
	typedef struct value_compare_s<_t_data>				value_compare;
	
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

	virtual iterator		insert						(const _t_data &rData) = 0;

	virtual iterator		erase						(const_iterator sCIterPos) = 0;
	virtual _t_sizetype		erase						(const _t_key &rKey) = 0;
	virtual iterator		erase						(const_iterator sCIterFirst, const_iterator sCIterLast) = 0;

	virtual void			swap						(CBTreeAssociativeIf &rContainer) = 0;

	virtual iterator		find						(const _t_key &rKey) = 0;
	virtual const_iterator	find						(const _t_key &rKey) const = 0;

	virtual iterator		lower_bound					(const _t_key &rKey) = 0;
	virtual const_iterator	lower_bound					(const _t_key &rKey) const = 0;

	virtual iterator		upper_bound					(const _t_key &rKey) = 0;
	virtual const_iterator	upper_bound					(const _t_key &rKey) const = 0;
	
	virtual void			get_next_key				(const _t_key &rKey, _t_key &rNextKey, bool &bBounce) const = 0;
	virtual void			get_prev_key				(const _t_key &rKey, _t_key &rPrevKey, bool &bBounce) const = 0;

	virtual _t_sizetype		count						(const _t_key &rKey) const = 0;

	virtual key_compare		key_comp					() const = 0;
	virtual value_compare	value_comp					() const = 0;
	
	virtual _t_sizetype		serialize					(const _t_sizetype nStart, const _t_sizetype nLen, _t_data *pData) const = 0;

	virtual bool			operator==					(const CBTreeAssociativeIf &rRhs) const = 0;
	virtual bool			operator!=					(const CBTreeAssociativeIf &rRhs) const = 0;
protected:

	virtual bool			get_at						(const _t_sizetype nPos, _t_data &rData) const = 0;

	virtual bool			get							(const _t_key &rKey, _t_sizetype nInstance, _t_data *pObj) const = 0;
	virtual _t_sizetype		get_init_pos_of_key			(const _t_key &rKey) const = 0;
	
	virtual int				comp						(const _t_key &rKey0, const _t_key &rKey1) const = 0;

	virtual _t_key			*extract_key				(_t_key *pKey, const _t_data &rData) const = 0;
};

/***********************************************************************/

template <class _t_sizetype, class _t_key>
class CBTreeKeySortPos
{
public:
	_t_key			*pKey;
	_t_sizetype		nInstance;
};

template <class _t_data, class _t_key = _t_data, class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter> >
class CBTreeAssociative
	:	virtual public CBTreeAssociativeIf<_t_data, _t_key, _t_sizetype>
	,	public CBTreeBaseDefaults <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

	typedef CBTreeKeySortPos<_t_sizetype, _t_key>					position_t;

	typedef CBTreeAssociative										CBTreeAssociative_t;

	typedef CBTreeAssociativeIf<_t_data, _t_key, _t_sizetype>		CBTreeAssociativeIf_t;

	typedef CBTreeBaseDefaults <position_t, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
																	CBTreeBase_t;

	typedef typename CBTreeBase_t::CBTreeBaseIf_t					CBTreeBaseIf_t;

	typedef typename CBTreeBaseIf_t::CBTreeDefaults_t				CBTreeDefaults_t;

	typedef CBTreeIf<_t_data, _t_sizetype>							CBTreeIf_t;

	typedef	typename CBTreeBase_t::node_t							node_t;

	typedef	typename CBTreeBase_t::iterator							iterator;
	typedef	typename CBTreeBase_t::const_iterator					const_iterator;
	typedef	typename CBTreeBase_t::reverse_iterator					reverse_iterator;
	typedef	typename CBTreeBase_t::const_reverse_iterator			const_reverse_iterator;

	typedef _t_data													data_t;
	typedef _t_sizetype												size_type;
	typedef _t_nodeiter												nodeiter_t;
	typedef _t_subnodeiter											subnodeiter_t;
	typedef _t_datalayerproperties									datalayerproperties_t;
	typedef _t_datalayer											datalayer_t;

	typedef	typename CBTreeBaseIf<position_t, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter>::iterator_state_t
																	iterator_state_t;

	typedef	typename CBTreeAssociativeIf_t::key_compare			key_compare;
	typedef typename CBTreeAssociativeIf_t::value_compare		value_compare;

	// construction
						CBTreeAssociative<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

						CBTreeAssociative<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(const CBTreeAssociative<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign = true);

	// destruction
	virtual				~CBTreeAssociative<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													();
	
	iterator			insert						(const _t_data &rData);

	template <class _t_iterator>
	void				insert						(_t_iterator sItFirst, _t_iterator sItLast);
	
	iterator			erase						(const_iterator sCIterPos);
	_t_sizetype			erase						(const _t_key &rKey);
	iterator			erase						(const_iterator sCIterFirst, const_iterator sCIterLast);
	
	iterator			find						(const _t_key &rKey);
	const_iterator		find						(const _t_key &rKey) const;

	iterator			lower_bound					(const _t_key &rKey);
	const_iterator		lower_bound					(const _t_key &rKey) const;

	iterator			upper_bound					(const _t_key &rKey);
	const_iterator		upper_bound					(const _t_key &rKey) const;

	void				get_next_key				(const _t_key &rKey, _t_key &rNextKey, bool &bBounce) const;
	void				get_prev_key				(const _t_key &rKey, _t_key &rPrevKey, bool &bBounce) const;
	
	_t_sizetype			count						(const _t_key &rKey) const;

	key_compare			key_comp					() const;
	value_compare		value_comp					() const;
	
	_t_sizetype			serialize					(const _t_sizetype nStart, const _t_sizetype nLen, _t_data *pData) const;

	CBTreeAssociative &	operator=					(const CBTreeAssociative &rBT);

	bool				operator==					(const CBTreeAssociativeIf_t &rRhs) const;
	bool				operator!=					(const CBTreeAssociativeIf_t &rRhs) const;

protected:

	virtual void		_assign						(const CBTreeIf_t &rContainer);

	bool				get_at						(const _t_sizetype nPos, _t_data &rData) const;
	
	bool				get							(const _t_key &rKey, _t_sizetype nInstance, _t_data *pObj) const;
	_t_sizetype			get_init_pos_of_key			(const _t_key &rKey) const;
	
	// node operations
	virtual position_t	determine_position			(position_t sPos, _t_nodeiter nNode, _t_subnodeiter &nSubPos, _t_subnodeiter &nSubData, bool &bFound) const;

	void				rebuild_node				(const _t_nodeiter nNode, const int32_t triMod = 0, _t_subnodeiter nSubStart = 0);

	virtual position_t	generate_prev_position		(const _t_nodeiter nNode, const _t_subnodeiter nSub, position_t sPos);
	virtual position_t	generate_next_position		(const _t_nodeiter nNode, const _t_subnodeiter nSub, position_t sPos);

	// testing
	virtual int			comp						(const _t_key &rKey0, const _t_key &rKey1) const;

	// data retrieval
	virtual _t_key		*extract_key				(_t_key *pKey, const _t_nodeiter nNode, const _t_subnodeiter nEntry) const;

	virtual _t_key		*extract_key				(_t_key *pKey, const _t_data &rData) const;

	inline _t_key		*extract_dataset_as_key		(_t_key *pKey, const _t_data &rData, typename ::std::true_type) const;
	inline _t_key		*extract_dataset_as_key		(_t_key *pKey, const _t_data &rData, typename ::std::false_type) const;

	inline _t_key		*extract_key_directly		(_t_key *pKey, const _t_data &rData, typename ::std::true_type) const;
	inline _t_key		*extract_key_directly		(_t_key *pKey, const _t_data &rData, typename ::std::false_type) const;

	inline _t_key		*extract_key				(_t_key *pKey, const _t_data &rData, typename ::std::true_type) const;
	inline _t_key		*extract_key				(_t_key *pKey, const _t_data &rData, typename ::std::false_type) const;

	_t_sizetype			get_instancePos				(const _t_nodeiter nNode, const _t_subnodeiter nSub) const;

	// manuvering
	virtual _t_subnodeiter	get_firstSubPos			(const _t_nodeiter nNode, const _t_key &rKey, bool bReverse = false) const;
	virtual _t_subnodeiter	find_next_sub_pos		(const _t_nodeiter nNode, position_t &sPos) const;

	virtual bool		find_oneKey					(const _t_key &rKey, _t_nodeiter &nNode, _t_subnodeiter &nSub, _t_sizetype *pnPos = NULL) const;
	virtual _t_sizetype	find_firstKey				(const _t_nodeiter nFromNode, const _t_subnodeiter nFromSub, _t_nodeiter &nNode, _t_subnodeiter &nSub) const;

	void				erase_via_reference			(const_iterator &rCIterPos, bool bReEvaluate = true);

	inline bool			_find						(const _t_key &rKey, _t_nodeiter &rnNode, _t_subnodeiter &rnSub, _t_sizetype &rnPos) const;
	
	inline void			_lower_bound				(const _t_key &rKey, _t_nodeiter &rnNode, _t_subnodeiter &rnSub, _t_sizetype &rnPos) const;
	inline bool			_upper_bound				(const _t_key &rKey, _t_nodeiter &rnNode, _t_subnodeiter &rnSub, _t_sizetype &rnPos) const;

	// variable size buffer service
	void				vbufferAllocate				(_t_key **pp);
	void				vbufferDeallocate			(_t_key **pp);

	void				allocateShortLiveKey		() const;
	void				freeShortLiveKey			() const;

	virtual bool		show_data					(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const;
	virtual bool		show_node					(std::ofstream &ofs, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const;

	void				_swap						(CBTreeAssociative &rContainer);

	template<class _t_iterator>
	bool				test_self_reference_of_iterator_to_this (_t_iterator &sItFirst, _t_iterator &sItLast, bool &bSelfReverse, const_iterator **ppsItFirst, const_iterator **ppsItLast)
	{
		return (btree_keysort_self_reference_of_iterator_to_this_arbiter<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, _t_iterator>::test_self_reference_of_iterator_to_this (this, sItFirst, sItLast, bSelfReverse, ppsItFirst, ppsItLast));
	}

	// protected variables

	// variable size buffers
	_t_key											**m_ppShortLiveKey;
	_t_key											**m_ppTempFindFirstKeyKey;

	_t_key											*m_pRemoveKey;
	_t_key											*m_pTempRemoveKey;
	_t_key											*m_pInstancesNewKey;
	_t_key											*m_pTempFindFirstKeyNewKey;
	_t_key											*m_pAddToNodeKey;
	_t_key											*m_pGetNewKey;

#if defined (_DEBUG)
	bool											*m_pbShortLiveKeyInUse;
#endif
};

#endif // BTREEASSOCIATIVE_H

#include "btreeassociative.cpp"
