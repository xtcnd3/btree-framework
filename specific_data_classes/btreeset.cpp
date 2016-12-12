/************************************************************
**
** file:	btreeset.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** set data class, which has an interface interchangeable
** with ::std::set.
**
************************************************************/

#ifndef BTREESET_CPP
#define	BTREESET_CPP

#include "specific_data_classes/btreeset.h"

template<class _t_keytype, class _t_datalayerproperties>
CBTreeSet<_t_keytype, _t_datalayerproperties>::CBTreeSet
	(const _t_datalayerproperties &rDataLayerProperties, const typename _t_datalayerproperties::sub_node_iter_type nNodeSize)
	:	CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties>
		(
			rDataLayerProperties, 
			nNodeSize
		)
{
}

template<class _t_keytype, class _t_datalayerproperties>
CBTreeSet<_t_keytype, _t_datalayerproperties>::CBTreeSet
	(const CBTreeSet_t &rContainer, const bool bAssign)
	:	CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties>
	(
		dynamic_cast<const CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties> &> (rContainer), 
		bAssign
	)
{
}

template<class _t_keytype, class _t_datalayerproperties>
CBTreeSet<_t_keytype, _t_datalayerproperties>::CBTreeSet
	(CBTreeSet_t &&rRhsContainer)
	:	CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties>
	(
		dynamic_cast<CBTreeAssociativeBase<_t_keytype, _t_keytype, _t_datalayerproperties> &&> (rRhsContainer)
	)
{
}

template<class _t_keytype, class _t_datalayerproperties>
CBTreeSet<_t_keytype, _t_datalayerproperties>::~CBTreeSet ()
{
}

template<class _t_keytype, class _t_datalayerproperties>
template<class _t_iterator>
void CBTreeSet<_t_keytype, _t_datalayerproperties>::insert (_t_iterator sItFirst, _t_iterator sItLast)
{
	CBTreeAssociativeBase_t::template _insert_unique <_t_iterator> (sItFirst, sItLast);
}

template<class _t_keytype, class _t_datalayerproperties>
typename CBTreeSet<_t_keytype, _t_datalayerproperties>::iterator
	CBTreeSet<_t_keytype, _t_datalayerproperties>::insert (const typename CBTreeSet_t::value_type &rData)
{
	return (CBTreeAssociativeBase_t::_insert_unique (rData));
}

template<class _t_keytype, class _t_datalayerproperties>
typename CBTreeSet<_t_keytype, _t_datalayerproperties>::iterator
	CBTreeSet<_t_keytype, _t_datalayerproperties>::insert (typename CBTreeSet_t::const_iterator sCIterHint, const typename CBTreeSet_t::value_type &rData)
{
	return (CBTreeAssociativeBase_t::_insert_unique (sCIterHint, rData));
}

template<class _t_keytype, class _t_datalayerproperties>
typename CBTreeSet<_t_keytype, _t_datalayerproperties>::iterator
	CBTreeSet<_t_keytype, _t_datalayerproperties>::emplace (typename CBTreeSet_t::value_type && rrData)
{
	return (CBTreeAssociativeBase_t::_emplace_unique (::std::forward<value_type> (rrData)));
}

template<class _t_keytype, class _t_datalayerproperties>
typename CBTreeSet<_t_keytype, _t_datalayerproperties>::iterator
	CBTreeSet<_t_keytype, _t_datalayerproperties>::emplace_hint (const_iterator sCIterHint, typename CBTreeSet_t::value_type && rrData)
{
	return (CBTreeAssociativeBase_t::_emplace_hint_unique (sCIterHint, ::std::forward<value_type> (rrData)));
}

template<class _t_keytype, class _t_datalayerproperties>
template<class ..._t_va_args>
typename CBTreeSet<_t_keytype, _t_datalayerproperties>::iterator
	CBTreeSet<_t_keytype, _t_datalayerproperties>::emplace (_t_va_args && ... rrArgs)
{
	return (CBTreeAssociativeBase_t::_emplace_unique (::std::forward<_t_va_args> (rrArgs) ...));
}

template<class _t_keytype, class _t_datalayerproperties>
template<class ..._t_va_args>
typename CBTreeSet<_t_keytype, _t_datalayerproperties>::iterator
	CBTreeSet<_t_keytype, _t_datalayerproperties>::emplace_hint (const_iterator sCIterHint, _t_va_args && ... rrArgs)
{
	return (CBTreeAssociativeBase_t::_emplace_hint_unique (sCIterHint, ::std::forward<_t_va_args> (rrArgs) ...));
}

template<class _t_keytype, class _t_datalayerproperties>
typename CBTreeSet<_t_keytype, _t_datalayerproperties>::equal_range_type
	CBTreeSet<_t_keytype, _t_datalayerproperties>::equal_range (const key_type &rKey)
{
	return (CBTreeAssociativeBase_t::template _equal_range_unique <iterator> (rKey));
}

template<class _t_keytype, class _t_datalayerproperties>
typename CBTreeSet<_t_keytype, _t_datalayerproperties>::equal_range_const_type
	CBTreeSet<_t_keytype, _t_datalayerproperties>::equal_range (const key_type &rKey) const
{
	return (CBTreeAssociativeBase_t::template _equal_range_unique <const_iterator> (rKey));
}

template<class _t_keytype, class _t_datalayerproperties>
void CBTreeSet<_t_keytype, _t_datalayerproperties>::swap (typename CBTreeSet_t::CBTreeAssociativeIf_t &rContainerIf)
{
	CBTreeSet_t		&rContainer = dynamic_cast<CBTreeSet_t &> (rContainerIf);

	this->swap (rContainer);
}

template<class _t_keytype, class _t_datalayerproperties>
void CBTreeSet<_t_keytype, _t_datalayerproperties>::swap (CBTreeSet_t &rContainer)
{
	if (this != &rContainer)
	{
		this->_swap (rContainer);
	}
}

template<class _t_keytype, class _t_datalayerproperties>
CBTreeSet<_t_keytype, _t_datalayerproperties> &CBTreeSet<_t_keytype, _t_datalayerproperties>::operator=
	(const CBTreeSet<_t_keytype, _t_datalayerproperties> &rContainer)
{
	// if this is not the same instance as the referenced b-tree (rContainer) ...
	if (this != &rContainer)
	{
		CBTreeAssociativeBase_t			&rAssociativeBaseThis = dynamic_cast<CBTreeAssociativeBase_t &> (*this);
		const CBTreeAssociativeBase_t	&rAssociativeBaseContainer = dynamic_cast<const CBTreeAssociativeBase_t &> (rContainer);
		
		rAssociativeBaseThis = rAssociativeBaseContainer;
	}

	return (*this);
}

template<class _t_keytype, class _t_datalayerproperties>
CBTreeSet<_t_keytype, _t_datalayerproperties> &CBTreeSet<_t_keytype, _t_datalayerproperties>::operator=
	(CBTreeSet<_t_keytype, _t_datalayerproperties> &&rRhsContainer)
{
	CBTreeAssociativeBase_t		&rAssociativeBaseThis = dynamic_cast<CBTreeAssociativeBase_t &> (*this);
	CBTreeAssociativeBase_t		&rAssociativeBaseContainer = dynamic_cast<CBTreeAssociativeBase_t &> (rRhsContainer);
		
	rAssociativeBaseThis = ::std::move (rAssociativeBaseContainer);

	return (*this);
}

template<class _t_keytype, class _t_datalayerproperties>
void CBTreeSet<_t_keytype, _t_datalayerproperties>::_swap (CBTreeSet_t &rContainer)
{
	CBTreeAssociativeBase_t		&rSTLbaseContainerContainer = dynamic_cast <CBTreeAssociativeBase_t &> (rContainer);
	
	CBTreeAssociativeBase_t::_swap (rSTLbaseContainerContainer);
}

#endif // BTREESET_CPP
