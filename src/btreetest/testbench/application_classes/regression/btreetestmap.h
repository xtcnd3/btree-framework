/************************************************************
**
** file:	btreetestmap.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains map application class code derived
** from the b-tree framework's map class to allow
** for the map class being tested by having
** the map application class act as a wrapper.
**
************************************************************/

#ifndef BTREETESTMAP_H
#define	BTREETESTMAP_H

#include <stdint.h>

#include <map>
#include <utility>

#include "testbench/common/btreetestcommon.h"

#include "base_class_stack/btreebasedefaults.h"
#include "specific_data_classes/btreemap.h"

typedef struct mapMap_s
{
	uint32_t	nData;
	uint32_t	nDebug;

	bool	operator== (const struct mapMap_s &rOpr) const
	{
		return ((nData == rOpr.nData) && (nDebug == rOpr.nDebug));
	};
} mapMap_t;

template<class _t_datalayerproperties>
class CBTreeTestMap
	:	public CBTreeMap<uint32_t, mapMap_t, _t_datalayerproperties>
{
public:

	typedef mapMap_t												map_type;
	typedef uint32_t												key_type;
	typedef typename ::std::pair<key_type, map_type>				value_type;
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

	typedef CBTreeTestMap											CBTreeTestMap_t;
	
	typedef CBTreeMap<key_type, map_type, _t_datalayerproperties>	CBTreeMap_t;

	typedef typename CBTreeMap_t::CBTreeAssociativeIf_t				CBTreeAssociativeIf_t;

	typedef typename CBTreeMap_t::CBTreeIf_t						CBTreeIf_t;

	typedef typename CBTreeMap_t::CBTreeDefaults_t					CBTreeDefaults_t;

	typedef typename CBTreeMap_t::CBTreeBaseIf_t					CBTreeBaseIf_t;

	typedef typename CBTreeMap_t::CBTreeBaseDefaults_t				CBTreeBaseDefaults_t;

	typedef typename CBTreeMap_t::iterator							iterator;
	typedef typename CBTreeMap_t::const_iterator					const_iterator;
	typedef typename CBTreeMap_t::reverse_iterator					reverse_iterator;
	typedef typename CBTreeMap_t::const_reverse_iterator			const_reverse_iterator;

	typedef	typename CBTreeMap_t::key_compare						key_compare;
	typedef typename CBTreeMap_t::value_compare						value_compare;

	typedef ::std::map<key_type, map_type>							reference_t;

							CBTreeTestMap<_t_datalayerproperties>
								(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, sub_node_iter_type nNodeSize, reference_t *pClRefData);

							CBTreeTestMap<_t_datalayerproperties>
								(const CBTreeTestMap<_t_datalayerproperties> &rBT, bool bAssign = true);

							~CBTreeTestMap<_t_datalayerproperties>
								();

	CBTreeTestMap_t &		operator=				(const CBTreeTestMap_t &rBT);

	template<class _t_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);

	iterator				insert					(const value_type &rData);

	iterator				erase					(const_iterator sCIterPos);
	size_type				erase					(const key_type &rKey);
	iterator				erase					(const_iterator sCIterFirst, const_iterator sCIterLast);

	void					swap					(CBTreeTestMap &rTMM);

	void					clear					();

//	key_compare				key_comp				() const;
//	value_compare			value_comp				() const;
	
	bool					operator==				(const CBTreeTestMap &rTMM) const;
	bool					operator!=				(const CBTreeTestMap &rTMM) const;

	void					test					() const;

	void					set_reference			(reference_t *pReference);

	void					set_atomic_testing		(bool bEnable);

protected:

	bool					show_data				(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const node_iter_type nNode, const sub_node_iter_type nSubPos) const;

	reference_t				*m_pClRef;

	bool					m_bAtomicTesting;

public:

	friend class CBTreeIterator<CBTreeIf_t>;
	friend class CBTreeConstIterator<CBTreeIf_t>;
	friend class CBTreeReverseIterator<iterator>;
	friend class CBTreeConstReverseIterator<const_iterator>;
};

#endif // !BTREETESTMAP_H

#include "btreetestmap.cpp"
