/************************************************************
**
** file:	btreetestmultimap.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains multi map application class code derived
** from the b-tree framework's multi map class to allow
** for the multi map class being tested by having
** the multi map application class act as a wrapper.
**
************************************************************/

#ifndef BTREETESTMULTIMAP_H
#define	BTREETESTMULTIMAP_H

#include <stdint.h>

#include <map>
#include <utility>
#include <type_traits>

#include "testbench/common/btreetestcommon.h"

#include "base_class_stack/btreebasedefaults.h"
#include "specific_data_classes/btreemultimap.h"

typedef struct multiMapMap_s
{
	uint32_t	nData;
	uint32_t	nDebug;

	bool	operator== (const struct multiMapMap_s &rOpr) const
	{
		return ((nData == rOpr.nData) && (nDebug == rOpr.nDebug));
	};
} multiMapMap_t;

template<class _t_datalayerproperties>
class CBTreeTestMultiMap
	:	public CBTreeMultiMap<uint32_t, multiMapMap_t, _t_datalayerproperties>
{
public:

	typedef multiMapMap_t												map_type;
	typedef uint32_t													key_type;
	typedef typename ::std::pair<key_type, map_type>					value_type;
	typedef typename _t_datalayerproperties::size_type					size_type;
	typedef typename _t_datalayerproperties::node_iter_type				node_iter_type;
	typedef typename _t_datalayerproperties::sub_node_iter_type			sub_node_iter_type;
	typedef _t_datalayerproperties										data_layer_properties_type;
	typedef typename _t_datalayerproperties::data_layer_type			data_layer_type;

	typedef value_type&													reference;
	typedef const value_type&											const_reference;
	typedef value_type*													pointer;
	typedef const value_type*											const_pointer;
	typedef	typename ::std::make_signed<size_type>::type				difference_type;

	typedef CBTreeTestMultiMap											CBTreeTestMultiMap_t;
	
	typedef CBTreeMultiMap<key_type, map_type, _t_datalayerproperties>	CBTreeMultiMap_t;

	typedef typename CBTreeMultiMap_t::CBTreeAssociativeBase_t			CBTreeAssociativeBase_t;

	typedef typename CBTreeAssociativeBase_t::CBTreeAssociative_t		CBTreeAssociative_t;

	typedef typename CBTreeMultiMap_t::CBTreeAssociativeIf_t			CBTreeAssociativeIf_t;

	typedef typename CBTreeMultiMap_t::CBTreeIf_t						CBTreeIf_t;

	typedef typename CBTreeMultiMap_t::CBTreeDefaults_t					CBTreeDefaults_t;

	typedef typename CBTreeMultiMap_t::CBTreeBaseIf_t					CBTreeBaseIf_t;

	typedef typename CBTreeMultiMap_t::CBTreeBaseDefaults_t				CBTreeBaseDefaults_t;

	typedef typename CBTreeMultiMap_t::iterator							iterator;
	typedef typename CBTreeMultiMap_t::const_iterator					const_iterator;
	typedef typename CBTreeMultiMap_t::reverse_iterator					reverse_iterator;
	typedef typename CBTreeMultiMap_t::const_reverse_iterator			const_reverse_iterator;

	typedef	typename CBTreeMultiMap_t::key_compare						key_compare;
	typedef typename CBTreeMultiMap_t::value_compare					value_compare;

	typedef ::std::multimap<key_type, map_type>							reference_t;

							CBTreeTestMultiMap<_t_datalayerproperties>
								(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, sub_node_iter_type nNodeSize, reference_t *pClRefData);

							CBTreeTestMultiMap<_t_datalayerproperties>
								(const CBTreeTestMultiMap<_t_datalayerproperties> &rBT, bool bAssign = true);

							~CBTreeTestMultiMap<_t_datalayerproperties>
								();

	CBTreeTestMultiMap_t &	operator=				(const CBTreeTestMultiMap_t &rBT);

	template<class _t_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);
	template<class _t_iterator, class _t_ref_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);
	iterator				insert					(const value_type &rData);

	iterator				erase					(const_iterator sCIterPos);
	size_type				erase					(const key_type &rKey);
	iterator				erase					(const_iterator sCIterFirst, const_iterator sCIterLast);

	void					swap					(CBTreeTestMultiMap &rTMM);

	void					clear					();

//	key_compare				key_comp				() const;
//	value_compare			value_comp				() const;
	
	bool					operator==				(const CBTreeTestMultiMap &rTMM) const;
	bool					operator!=				(const CBTreeTestMultiMap &rTMM) const;

	void					test					() const;

	void					set_reference			(reference_t *pReference);

	void					set_atomic_testing		(bool bEnable);

protected:

	reference_t				*m_pClRef;

	bool					m_bAtomicTesting;

public:

	friend class CBTreeIterator<CBTreeIf_t>;
	friend class CBTreeConstIterator<CBTreeIf_t>;
	friend class CBTreeReverseIterator<iterator>;
	friend class CBTreeConstReverseIterator<const_iterator>;
};

#endif // !BTREETESTMULTIMAP_H

#include "btreetestmultimap.cpp"
