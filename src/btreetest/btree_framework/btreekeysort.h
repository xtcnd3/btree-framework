/************************************************************
**
** file:	btreekeysort.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** key sort (ordered associative) data class.
**
************************************************************/

#ifndef	BTREEKEYSORT_H
#define	BTREEKEYSORT_H

#include <iterator>
#include <list>

#include "btree.h"

#include "btreeiter.h"

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
class CBTreeKeySort;

template <class _t_sizetype, class _t_key>
class CBTreeKeySortPos;

/***********************************************************************/

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer, class _t_iterator>
struct btree_keysort_self_reference_of_iterator_to_this_arbiter
{
	static bool	test_self_reference_of_iterator_to_this
	(
		CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *pClKeySort, 
		_t_iterator &sItFirst, 
		_t_iterator &sItLast, 
		bool &bSelfReverse, 
		typename CBTreeBase <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator	**ppsItFirst, 
		typename CBTreeBase <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator	**ppsItLast
		
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
struct btree_keysort_self_reference_of_iterator_to_this_arbiter<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, typename CBTreeBase <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator >
{
	static bool	test_self_reference_of_iterator_to_this
	(
		CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>															*pClKeySort, 
		typename CBTreeBase <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator	&sItFirst, 
		typename CBTreeBase <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator	&sItLast, 
		bool &bSelfReverse, 
		typename CBTreeBase <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator	**ppsItFirst, 
		typename CBTreeBase <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator	**ppsItLast
	)
	{
		typedef typename CBTreeBase <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator	citer_t;
		
		bool	bSelfReference = (pClKeySort == sItFirst.get_container ()) || (pClKeySort == sItLast.get_container ());

		bSelfReverse = false;

		if ((ppsItFirst != NULL) && bSelfReference)
		{
			(*ppsItFirst) = new citer_t (sItFirst);

			BTREE_ASSERT (((citer_t *) (*ppsItFirst)) != NULL, "btree_keysort_self_reference_of_iterator_to_this_arbiter<..., const_iterator>::test_self_reference_of_iterator_to_this (): ERROR: insufficient memory! (ppsItFirst)");
		}

		if ((ppsItLast != NULL) && bSelfReference)
		{
			(*ppsItLast) = new citer_t (sItLast);

			BTREE_ASSERT (((citer_t *) (*ppsItLast)) != NULL, "btree_keysort_self_reference_of_iterator_to_this_arbiter<..., const_iterator>::test_self_reference_of_iterator_to_this (): ERROR: insufficient memory! (ppsItLast)");
		}

		return (bSelfReference);
	}
};

template <class _t_data, class _t_key, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
struct btree_keysort_self_reference_of_iterator_to_this_arbiter<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, typename CBTreeBase <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator >
{
	static bool	test_self_reference_of_iterator_to_this
	(
		CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>																	*pClKeySort, 
		typename CBTreeBase <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator	&sItFirst, 
		typename CBTreeBase <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator	&sItLast, 
		bool &bSelfReverse, 
		typename CBTreeBase <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator			**ppsItFirst, 
		typename CBTreeBase <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator			**ppsItLast
	)
	{
		typedef typename CBTreeBase <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator	citer_t;
		
		bool	bSelfReference = (pClKeySort == sItFirst.base ().get_container ()) || (pClKeySort == sItLast.base ().get_container ());
				
		bSelfReverse = true;

		if ((ppsItFirst != NULL) && bSelfReference)
		{
			(*ppsItFirst) = new citer_t (sItLast.base ());

			BTREE_ASSERT (((citer_t *) (*ppsItFirst)) != NULL, "btree_keysort_self_reference_of_iterator_to_this_arbiter<..., const_reverse_iterator>::test_self_reference_of_iterator_to_this (): ERROR: insufficient memory! (ppsItFirst)");

			((citer_t) (**ppsItFirst))--;
		}

		if ((ppsItLast != NULL) && bSelfReference)
		{
			(*ppsItLast) = new citer_t (sItFirst.base ());

			BTREE_ASSERT (((citer_t *) (*ppsItLast)) != NULL, "btree_keysort_self_reference_of_iterator_to_this_arbiter<..., const_reverse_iterator>::test_self_reference_of_iterator_to_this (): ERROR: insufficient memory! (ppsItLast)");

			((citer_t) (**ppsItLast))--;
		}

		return (bSelfReference);
	}
};

/***********************************************************************/

template <class _t_sizetype, class _t_key>
class CBTreeKeySortPos
{
public:
	_t_key			*pKey;
	_t_sizetype		nInstance;
};

template <class _t_data, class _t_key = _t_data, class _t_sizetype = uint64_t>
class CBTreeKeySortDataIf
	:	public virtual CBTreeIf<_t_sizetype>
{
public:

	// construction
						CBTreeKeySortDataIf<_t_data, _t_key, _t_sizetype>
													()
						{
						};
						
	// destruction
						~CBTreeKeySortDataIf<_t_data, _t_key, _t_sizetype>
													()
						{
						};

	virtual _t_key		*extract_key				(_t_key *pKey, const _t_data &rData) = 0;

	virtual _t_sizetype	insert_tb					(const _t_data &rData) = 0;

	virtual _t_sizetype	erase_tb					(const _t_key &rKey) = 0;
	virtual _t_sizetype	erase_tb					(const _t_key &rKey, const _t_sizetype instance) = 0;

	virtual void		clear						() = 0;

	virtual _t_sizetype	get							(const _t_key &rKey, _t_data *pObj) = 0;
	virtual bool		get							(const _t_key &rKey, _t_sizetype nInstance, _t_data *pObj) = 0;
	
	virtual _t_sizetype	get_init_pos_of_key			(const _t_key &rKey) = 0;
	
	virtual void		get_next_key				(const _t_key &rKey, _t_key &rNextKey, bool &bBounce) = 0;
	virtual void		get_prev_key				(const _t_key &rKey, _t_key &rPrevKey, bool &bBounce) = 0;

	virtual _t_sizetype	count						(const _t_key &rKey) = 0;

	virtual bool		get_at						(const _t_sizetype nPos, _t_data &rData) = 0;
	
	virtual _t_sizetype	serialize					(const _t_sizetype nStart, const _t_sizetype nLen, _t_data *pData) = 0;
};

template <class _t_data, class _t_key = _t_data, class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter> >
class CBTreeKeySort
	:	virtual public CBTreeKeySortDataIf <_t_data, _t_key, _t_sizetype>
	,	public CBTreeBase <CBTreeKeySortPos <_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

#if defined(__GNUC__) || defined(__GNUG__)

	typedef	typename CBTreeBase<CBTreeKeySortPos<_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::node_t					node_t;

	typedef	typename CBTreeBase<CBTreeKeySortPos<_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator			const_iterator;
	typedef	typename CBTreeBase<CBTreeKeySortPos<_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_reverse_iterator	const_reverse_iterator;

#endif

	typedef CBTreeBase<CBTreeKeySortPos<_t_sizetype, _t_key>, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>	CBTreeBase_t;

	typedef CBTreeKeySort											CBTreeKeySort_t;

	typedef CBTreeKeySortPos<_t_sizetype, _t_key>					position_t;
	typedef _t_data													data_t;
	typedef _t_sizetype												sizetype_t;
	typedef _t_nodeiter												nodeiter_t;
	typedef _t_subnodeiter											subnodeiter_t;
	typedef _t_datalayerproperties									datalayerproperties_t;
	typedef _t_datalayer											datalayer_t;

	// construction
						CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

						CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign = true);

	// destruction
	virtual				~CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													();
	
	_t_key				*extract_key				(_t_key *pKey, const _t_data &rData);

	_t_key				*extract_key				(_t_key *pKey, const _t_data &rData, typename ::std::true_type);
	_t_key				*extract_key				(_t_key *pKey, const _t_data &rData, typename ::std::false_type);

	template <class _t_iterator>
	void				insert						(_t_iterator sItFirst, _t_iterator sItLast);
	const_iterator		insert						(const _t_data &rData);

	const_iterator		erase						(const_iterator sCIterPos);
	_t_sizetype			erase						(const _t_key &rKey);
	const_iterator		erase						(const_iterator sCIterFirst, const_iterator sCIterLast);

	void				swap						(CBTreeKeySort &rKeySort);

	_t_sizetype			insert_tb					(const _t_data &rData);
	
	_t_sizetype			erase_tb					(const _t_key &rKey);
	_t_sizetype			erase_tb					(const _t_key &rKey, const _t_sizetype nInstance);

	void				clear						();

	const_iterator		find						(const _t_key &rKey);

	_t_sizetype			get							(const _t_key &rKey, _t_data *pObj);
	_t_sizetype			get							(const _t_key &rKey, CBTreeKeySortDataIf<_t_data, _t_key, _t_sizetype> &rDestData);
	bool				get							(const _t_key &rKey, _t_sizetype nInstance, _t_data *pObj);
	
	_t_sizetype			get_init_pos_of_key			(const _t_key &rKey);
	
	void				get_next_key				(const _t_key &rKey, _t_key &rNextKey, bool &bBounce);
	void				get_prev_key				(const _t_key &rKey, _t_key &rPrevKey, bool &bBounce);
	
	_t_sizetype			count						(const _t_key &rKey);

	const_iterator		lower_bound					(const _t_key &rKey);
	const_iterator		upper_bound					(const _t_key &rKey);
	
	bool				get_at						(const _t_sizetype nPos, _t_data &rData);
	
	_t_sizetype			serialize					(const _t_sizetype nStart, const _t_sizetype nLen, _t_data *pData);

	CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &
						operator=					(CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT);

protected:

	// node operations
	virtual CBTreeKeySortPos<_t_sizetype, _t_key>	determine_position	(CBTreeKeySortPos<_t_sizetype, _t_key> sPos, _t_nodeiter nNode, _t_subnodeiter &nSubPos, _t_subnodeiter &nSubData, bool &bFound);

	void				rebuild_node				(const _t_nodeiter nNode, const int32_t triMod = 0, _t_subnodeiter nSubStart = 0);

	virtual CBTreeKeySortPos<_t_sizetype, _t_key> generate_prev_position	(const _t_nodeiter nNode, const _t_subnodeiter nSub, CBTreeKeySortPos<_t_sizetype, _t_key> sPos);
	virtual CBTreeKeySortPos<_t_sizetype, _t_key> generate_next_position	(const _t_nodeiter nNode, const _t_subnodeiter nSub, CBTreeKeySortPos<_t_sizetype, _t_key> sPos);

	// testing
	virtual int			comp						(const _t_key &rKey0, const _t_key &rKey1);

	// data retrieval
	virtual _t_key		*extract_key				(_t_key *pKey, const _t_nodeiter nNode, const _t_subnodeiter nEntry);

	_t_sizetype			get_instancePos				(const _t_nodeiter nNode, const _t_subnodeiter nSub);

	// manuvering
	virtual _t_subnodeiter	get_firstSubPos			(const _t_nodeiter nNode, const _t_key &rKey, bool bReverse = false);
	virtual _t_subnodeiter	find_next_sub_pos		(const _t_nodeiter nNode, CBTreeKeySortPos<_t_sizetype, _t_key> &sPos);

	virtual bool		find_oneKey					(const _t_key &rKey, _t_nodeiter &nNode, _t_subnodeiter &nSub, _t_sizetype *pnPos = NULL);
	virtual _t_sizetype	find_firstKey				(const _t_nodeiter nFromNode, const _t_subnodeiter nFromSub, _t_nodeiter &nNode, _t_subnodeiter &nSub);

	void				erase_via_reference			(const_iterator &rCIterPos, bool bReEvaluate = true);

	template<class _t_iterator>
	bool				test_self_reference_of_iterator_to_this (_t_iterator &sItFirst, _t_iterator &sItLast, bool &bSelfReverse, const_iterator **ppsItFirst, const_iterator **ppsItLast)
	{
		return (btree_keysort_self_reference_of_iterator_to_this_arbiter<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer, _t_iterator>::test_self_reference_of_iterator_to_this (this, sItFirst, sItLast, bSelfReverse, ppsItFirst, ppsItLast));
	}

	// variable size buffer service
	void				vbufferAllocate				(_t_key **pp);
	void				vbufferDeallocate			(_t_key **pp);

	void				allocateShortLiveKey		();
	void				freeShortLiveKey			();

	virtual bool		show_data					(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos);
	virtual bool		show_node					(std::ofstream &ofs, const _t_nodeiter nNode, const _t_subnodeiter nSubPos);

	void				assign						(CBTreeKeySort<_t_data, _t_key, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT);

	// protected variables

	// variable size buffers
	_t_key											*m_pShortLiveKey;
	_t_key											*m_pRemoveKey;
	_t_key											*m_pTempRemoveKey;
	_t_key											*m_pInstancesNewKey;
	_t_key											*m_pTempFindFirstKeyKey;
	_t_key											*m_pTempFindFirstKeyNewKey;
	_t_key											*m_pAddToNodeKey;
	_t_key											*m_pGetNewKey;

#if defined (_DEBUG)
	bool											m_bShortLiveKeyInUse;
#endif
};

#endif // BTREEKEYSORT_H

#include "btreekeysort.cpp"
