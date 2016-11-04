/************************************************************
**
** file:	btreekeysort.h
** author:	Andreas Steffens
** license:	LGPL v3
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

#include "abstract_data_classes/btreeassociative.h"

template<class _t_data, class _t_key = _t_data, class _t_datalayerproperties = CBTreeIOpropertiesRAM <> >
class CBTreeKeySort
	:	virtual public CBTreeAssociativeIf<_t_data, _t_key, typename _t_datalayerproperties::size_type>
	,	public CBTreeAssociative <_t_data, _t_key, _t_datalayerproperties>
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

	typedef CBTreeKeySort											CBTreeKeySort_t;

	typedef CBTreeAssociative<value_type, key_type, _t_datalayerproperties>
																	CBTreeAssociative_t;

	typedef typename CBTreeAssociative_t::CBTreeBaseDefaults_t		CBTreeBaseDefaults_t;

	typedef CBTreeAssociativeIf<_t_data, _t_key, size_type>			CBTreeAssociativeIf_t;

	typedef typename CBTreeAssociative_t::CBTreeBaseIf_t			CBTreeBaseIf_t;

	typedef	typename CBTreeBaseDefaults_t::node_t					node_t;

	typedef	typename CBTreeAssociative_t::iterator					iterator;
	typedef	typename CBTreeAssociative_t::const_iterator			const_iterator;
	typedef	typename CBTreeAssociative_t::reverse_iterator			reverse_iterator;
	typedef	typename CBTreeAssociative_t::const_reverse_iterator	const_reverse_iterator;

	typedef	typename CBTreeAssociative_t::iterator_state_t			iterator_state_t;

	// construction
						CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>
													(_t_datalayerproperties &rDataLayerProperties, sub_node_iter_type nNodeSize);

						CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>
													(CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties> &rBT, bool bAssign = true);

	// destruction
	virtual				~CBTreeKeySort<_t_data, _t_key, _t_datalayerproperties>
													();
	
	void				swap						(CBTreeAssociativeIf_t &rContainerIf);
	void				swap						(CBTreeKeySort_t &rContainer);
	
	void				clear						();

	CBTreeKeySort_t &	operator=					(const CBTreeKeySort_t &rBT);

protected:

	void				set_iter_data				(void *pState, const value_type &rData);

	void				_swap						(CBTreeKeySort_t &rContainer);
};

#endif // BTREEKEYSORT_H

#include "btreekeysort.cpp"
