/************************************************************
**
** file:	btreeiter.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** iterator classes.
**
************************************************************/

#ifndef	BTREEITER_H
#define	BTREEITER_H

#include <iterator>

#include <stdint.h>

#include "btreecommon.h"
#include "btreeaux.h"

template <class _ti_container>
class CBTreeBaseConstIterator : public ::std::iterator < ::std::random_access_iterator_tag, typename _ti_container::data_t, typename _ti_container::sizetype_t>
{
public:

	typedef CBTreeBaseConstIterator<_ti_container>					CBTreeBaseConstIterator_t;

	typedef _ti_container											container_t;

	typedef typename _ti_container::position_t						position_t;
	typedef typename _ti_container::data_t							data_t;
	typedef typename _ti_container::sizetype_t						sizetype_t;
	typedef typename _ti_container::nodeiter_t						nodeiter_t;
	typedef typename _ti_container::subnodeiter_t					subnodeiter_t;

									CBTreeBaseConstIterator<_ti_container> ();
									CBTreeBaseConstIterator<_ti_container> (_ti_container *pContainer, sizetype_t nPos, bool bRegister = true);
									CBTreeBaseConstIterator<_ti_container> (_ti_container *pContainer, sizetype_t nPos, nodeiter_t nNode, subnodeiter_t nSub, btree_time_stamp_t sTimeStamp, bool bRegister = true);
									CBTreeBaseConstIterator<_ti_container> (const CBTreeBaseConstIterator &rIter, bool bRegister = true);

									~CBTreeBaseConstIterator<_ti_container> ();

	void							detach ();
	
	void							swap (CBTreeBaseConstIterator &rRight);
	
	CBTreeBaseConstIterator &		operator= (const CBTreeBaseConstIterator &rIter);

	bool							operator== (const CBTreeBaseConstIterator &rIter);
	bool							operator!= (const CBTreeBaseConstIterator &rIter);
	
	data_t &						operator* ();
	data_t *						operator-> ();

	CBTreeBaseConstIterator&		operator++ ();
	CBTreeBaseConstIterator			operator++ (int);
	CBTreeBaseConstIterator&		operator-- ();
	CBTreeBaseConstIterator			operator-- (int);
	
	CBTreeBaseConstIterator&		operator+= (const int nOperand);
	CBTreeBaseConstIterator&		operator+= (const sizetype_t nOperand);
	CBTreeBaseConstIterator&		operator+= (const CBTreeBaseConstIterator &rIter);

	CBTreeBaseConstIterator&		operator-= (const int nOperand);
	CBTreeBaseConstIterator&		operator-= (const sizetype_t nOperand);
	CBTreeBaseConstIterator&		operator-= (const CBTreeBaseConstIterator &rIter);
	
	const CBTreeBaseConstIterator	operator+ (const int nOperand) const;
	const CBTreeBaseConstIterator	operator+ (const sizetype_t nOperand) const;
	const CBTreeBaseConstIterator	operator+ (const CBTreeBaseConstIterator &rIter) const;

	const CBTreeBaseConstIterator	operator- (const int nOperand) const;
	const CBTreeBaseConstIterator	operator- (const sizetype_t nOperand) const;
	const sizetype_t				operator- (const CBTreeBaseConstIterator &rIter) const;

	bool							operator<  (const CBTreeBaseConstIterator &rIter);
	bool							operator<= (const CBTreeBaseConstIterator &rIter);
	bool							operator>  (const CBTreeBaseConstIterator &rIter);
	bool							operator>= (const CBTreeBaseConstIterator &rIter);

	data_t&							operator[] (const int nPos);
	data_t&							operator[] (const sizetype_t nPos);

	_ti_container					*get_container	();
	bool							is_btree_valid	();

	bool							is_evaluated	();

	void							sync			();

	sizetype_t						get_pos			();
	nodeiter_t						get_node		();
	subnodeiter_t					get_sub_pos		();

	void							set_time_stamp	(_ti_container *pContainer);
	
protected:

	typedef struct btree_iter_last_access_s
	{
		btree_time_stamp_t			sTimeStamp;
		_ti_container				*pContainer;

		bool			operator== (const struct btree_iter_last_access_s &rLA)
		{
			return ((sTimeStamp == rLA.sTimeStamp) && (pContainer == rLA.pContainer));
		}

		bool			operator!= (const struct btree_iter_last_access_s &rLA)
		{
			return ((sTimeStamp != rLA.sTimeStamp) && (pContainer != rLA.pContainer));
		}
	} btree_iter_last_access_t;

	void						increment_by	(const sizetype_t nOperand);
	void						decrement_by	(const sizetype_t nOperand);
	
	bool						out_of_range	();
	
	void						evaluate		();
	
	void						evaluate_from	(const nodeiter_t nFromNode, const sizetype_t nFromPos);

	void						evaluate_by_seek ();
	
	sizetype_t					get_pos			() const;

	bool						is_time_stamp_up_to_data	();

	void						invalidate_time_stamp	();

	void						update_time_stamp	();
	
	float						get_average_node_hops (const sizetype_t nHopDistance) const;
	
	void						assign			(const CBTreeBaseConstIterator &rIter);

	void						register_iterator	(_ti_container *pContainer);
	void						unregister_iterator (_ti_container *pContainer);
	
	_ti_container				*m_pContainer;
	sizetype_t					m_nPos;
	sizetype_t					m_nAssociatedPos;
	nodeiter_t					m_nNode;
	subnodeiter_t				m_nSub;
	data_t						m_sData;
	data_t						m_sSubScriptResult;
	btree_iter_last_access_t	m_sTimeStamp;

public:

	template <class _fti_container>
	friend typename ::CBTreeBaseConstIterator<_fti_container> operator+
	(
		typename _fti_container::sizetype_t nLhs, 
		const typename ::CBTreeBaseConstIterator<_fti_container> &nRhs
	);

	template <class _fti_container>
	friend typename ::CBTreeBaseConstIterator<_fti_container> operator+
	(
		int nLhs, 
		const typename ::CBTreeBaseConstIterator<_fti_container> &nRhs
	);
};

template <class _ti_container>
typename ::CBTreeBaseConstIterator<_ti_container> operator+
(
	typename _ti_container::sizetype_t nLhs, 
	const typename ::CBTreeBaseConstIterator<_ti_container> &nRhs
);

template <class _ti_container>
typename ::CBTreeBaseConstIterator<_ti_container> operator+
(
	int nLhs, 
	const typename ::CBTreeBaseConstIterator<_ti_container> &nRhs
);

template <class _ti_container>
class CBTreeBaseIteratorSubScriptWrapper;

template <class _t_iterator>
class CBTreeBaseReverseIterator;

template <class _ti_container>
class CBTreeBaseIterator : public CBTreeBaseConstIterator<_ti_container>
{
public:

	typedef _ti_container											container_t;

	typedef typename _ti_container::position_t						position_t;
	typedef typename _ti_container::data_t							data_t;
	typedef typename _ti_container::sizetype_t						sizetype_t;
	typedef typename _ti_container::nodeiter_t						nodeiter_t;
	typedef typename _ti_container::subnodeiter_t					subnodeiter_t;

									CBTreeBaseIterator<_ti_container> ();
									CBTreeBaseIterator<_ti_container> (_ti_container *pContainer, sizetype_t nPos, bool bRegister = true);
									CBTreeBaseIterator<_ti_container> (const CBTreeBaseIterator &rIter, bool bRegister = true);
						explicit	CBTreeBaseIterator<_ti_container> (const CBTreeBaseReverseIterator<CBTreeBaseIterator<_ti_container> > &rRIter, bool bRegister = true);
						
									CBTreeBaseIterator<_ti_container> (const CBTreeBaseConstIterator<_ti_container> &rIter, bool bRegister = true);

									~CBTreeBaseIterator<_ti_container> ();

//									operator const CBTreeBaseConstIterator<_ti_container> & ();

	const data_t &					operator* () const;

//	data_t &						operator* ();

	CBTreeBaseIteratorSubScriptWrapper<_ti_container>
									operator* ();
	
	CBTreeBaseIterator&				operator= (const CBTreeBaseIterator &rIter);
//	CBTreeBaseIterator&				operator= (const data_t &rData);

	CBTreeBaseIteratorSubScriptWrapper<_ti_container>
									operator[] (const int nPos);

	CBTreeBaseIteratorSubScriptWrapper<_ti_container>
									operator[] (const sizetype_t nPos);

protected:

	void							set			(const data_t &rData);

public:

	friend typename ::CBTreeBaseIteratorSubScriptWrapper<_ti_container>;

	template <class _fti_container>
	friend typename ::CBTreeBaseIterator<_fti_container> operator+
	(
		typename _fti_container::_t_sizetype nLhs, 
		const typename ::CBTreeBaseIterator<_fti_container> &nRhs
	);

	template <class _fti_container>
	friend typename ::CBTreeBaseIterator<_fti_container> operator+
	(
		int nLhs, 
		const typename ::CBTreeBaseIterator<_fti_container> &nRhs
	);
};

template <class _ti_container>
typename ::CBTreeBaseIterator<_ti_container> operator+
(
	typename _ti_container::_t_sizetype nLhs, 
	const typename ::CBTreeBaseIterator<_ti_container> &nRhs
);

template <class _ti_container>
typename ::CBTreeBaseIterator<_ti_container> operator+
(
	int nLhs, 
	const typename ::CBTreeBaseIterator<_ti_container> &nRhs
);

template <class _ti_container>
class CBTreeBaseIteratorSubScriptWrapper
{
public:

	typedef _ti_container											container_t;

	typedef typename _ti_container::position_t						position_t;
	typedef typename _ti_container::data_t							data_t;
	typedef typename _ti_container::sizetype_t						sizetype_t;
	typedef typename _ti_container::nodeiter_t						nodeiter_t;
	typedef typename _ti_container::subnodeiter_t					subnodeiter_t;

						CBTreeBaseIteratorSubScriptWrapper<_ti_container>
						(
							const CBTreeBaseIterator<_ti_container>	&rInstance, 
							sizetype_t								nPos
						);

						~CBTreeBaseIteratorSubScriptWrapper<_ti_container> ();

	CBTreeBaseIteratorSubScriptWrapper<_ti_container> &
						operator=					(const data_t &rData);

						operator const data_t &	();

protected:

	CBTreeBaseIterator<_ti_container>	m_nOffset;
	sizetype_t							m_nPos;
	data_t								m_sData;
};

template <class _ti_container>
typename ::CBTreeBaseIterator<_ti_container> operator+
(
	const int nLhs, 
	const typename ::CBTreeBaseIterator<_ti_container> &nRhs
);

template <class _ti_container>
typename ::CBTreeBaseIterator<_ti_container> operator+
(
	const typename _ti_container::_t_sizetype nLhs, 
	const typename ::CBTreeBaseIterator<_ti_container> &nRhs
);

template <class _t_iterator>
class CBTreeBaseConstReverseIterator	:	public ::std::reverse_iterator<_t_iterator>
{
public:

	typedef typename _t_iterator::container_t		container_t;

	typedef typename container_t::data_t			data_t;
	typedef typename container_t::sizetype_t		sizetype_t;
	
												CBTreeBaseConstReverseIterator<_t_iterator>	();
									explicit	CBTreeBaseConstReverseIterator<_t_iterator>	(const _t_iterator &rIter);
												CBTreeBaseConstReverseIterator<_t_iterator>	(const CBTreeBaseConstReverseIterator<_t_iterator> &rIter);

												~CBTreeBaseConstReverseIterator<_t_iterator>	();

	void										swap (CBTreeBaseConstReverseIterator &rRight);
												
	CBTreeBaseConstReverseIterator<_t_iterator>&	operator= (const reverse_iterator<_t_iterator> &rIter);

	bool										operator== (const CBTreeBaseConstReverseIterator &rIter);
	bool										operator!= (const CBTreeBaseConstReverseIterator &rIter);

	data_t&										operator* ();
	data_t*										operator-> ();

	data_t&										operator[] (const int nPos);
	data_t&										operator[] (const sizetype_t nPos);

protected:

	data_t										m_sSubScriptResult;
};

template <class _t_iterator>
class CBTreeBaseReverseIterator	:	public CBTreeBaseConstReverseIterator<_t_iterator>
{
public:

	typedef typename _t_iterator::container_t					container_t;

	typedef typename _t_iterator::container_t::data_t			data_t;
	typedef typename _t_iterator::container_t::sizetype_t		sizetype_t;
	typedef typename _t_iterator::container_t::nodeiter_t		nodeiter_t;
	typedef typename _t_iterator::container_t::subnodeiter_t	subnodeiter_t;

												CBTreeBaseReverseIterator<_t_iterator>	();
									explicit	CBTreeBaseReverseIterator<_t_iterator>	(const _t_iterator &rIter);
												CBTreeBaseReverseIterator<_t_iterator>	(const CBTreeBaseReverseIterator<_t_iterator> &rIter);

												~CBTreeBaseReverseIterator<_t_iterator>	();

	const data_t &								operator*	() const;

//	data_t &									operator*	();

	CBTreeBaseIteratorSubScriptWrapper<container_t>
												operator*	();
											
	CBTreeBaseReverseIterator&					operator=	(const reverse_iterator<_t_iterator> &rIter);
	CBTreeBaseReverseIterator&					operator=	(const data_t &rData);

	CBTreeBaseIteratorSubScriptWrapper<container_t>
												operator[]	(const int nPos);

	CBTreeBaseIteratorSubScriptWrapper<container_t>
												operator[]	(const sizetype_t nPos);

protected:

	void										set			(const data_t &rData);

public:

	friend typename ::CBTreeBaseIterator<container_t>;
	friend typename ::CBTreeBaseIteratorSubScriptWrapper<container_t>;

};

#endif	// BTREEITER_H

#include "btreeiter.cpp"
