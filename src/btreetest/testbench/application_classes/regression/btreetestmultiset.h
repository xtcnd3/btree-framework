/************************************************************
**
** file:	btreetestmultiset.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains multi set application class code derived
** from the b-tree framework's multi set class to allow
** for the multi set class being tested by having
** the multi set application class act as a wrapper.
**
************************************************************/

#ifndef BTREETESTMULTISET_H
#define	BTREETESTMULTISET_H

#include <stdint.h>

#include <set>
#include <utility>

#include "testbench/common/btreetestcommon.h"

#include "base_class_stack/btreebasedefaults.h"
#include "specific_data_classes/btreemultiset.h"

template<class _t_datalayerproperties>
class CBTreeTestMultiSet
	:	public CBTreeMultiSet<uint32_t, _t_datalayerproperties>
{
public:

	typedef uint32_t													value_type;
	typedef uint32_t													key_type;
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

	typedef CBTreeTestMultiSet											CBTreeTestMultiSet_t;
	
	typedef CBTreeMultiSet<key_type, _t_datalayerproperties>			CBTreeMultiSet_t;

	typedef typename CBTreeMultiSet_t::CBTreeAssociativeIf_t			CBTreeAssociativeIf_t;

	typedef typename CBTreeMultiSet_t::CBTreeIf_t						CBTreeIf_t;

	typedef typename CBTreeMultiSet_t::CBTreeDefaults_t					CBTreeDefaults_t;

	typedef typename CBTreeMultiSet_t::CBTreeBaseIf_t					CBTreeBaseIf_t;

	typedef typename CBTreeMultiSet_t::CBTreeBaseDefaults_t				CBTreeBaseDefaults_t;

	typedef typename CBTreeMultiSet_t::iterator							iterator;
	typedef typename CBTreeMultiSet_t::const_iterator					const_iterator;
	typedef typename CBTreeMultiSet_t::reverse_iterator					reverse_iterator;
	typedef typename CBTreeMultiSet_t::const_reverse_iterator			const_reverse_iterator;

	typedef	typename CBTreeMultiSet_t::key_compare						key_compare;
	typedef typename CBTreeMultiSet_t::value_compare					value_compare;

	struct data_s
	{
		key_type			nKey;
	};

	typedef ::std::multiset<key_type>									reference_t;

							CBTreeTestMultiSet<_t_datalayerproperties>
								(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, sub_node_iter_type nNodeSize, reference_t *pClRefData);

							CBTreeTestMultiSet<_t_datalayerproperties>
								(const CBTreeTestMultiSet<_t_datalayerproperties> &rBT, bool bAssign = true);

							~CBTreeTestMultiSet<_t_datalayerproperties>
								();

	CBTreeTestMultiSet_t &	operator=				(const CBTreeTestMultiSet_t &rBT);

	template<class _t_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);
	
	template<class _t_iterator, class _t_ref_iterator>
	void					insert					(_t_iterator sItFirst, _t_iterator sItLast);
	
	iterator				insert					(const value_type &rData);

	iterator				erase					(const_iterator sCIterPos);
	size_type				erase					(const key_type &rKey);
	iterator				erase					(const_iterator sCIterFirst, const_iterator sCIterLast);

	void					swap					(CBTreeTestMultiSet &rTMM);

	void					clear					();

//	key_compare				key_comp				() const;
//	value_compare			value_comp				() const;
	
	bool					operator==				(const CBTreeTestMultiSet &rTMM) const;
	bool					operator!=				(const CBTreeTestMultiSet &rTMM) const;

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

#endif // !BTREETESTMULTISET_H

#include "btreetestmultiset.cpp"
