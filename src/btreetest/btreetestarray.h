/************************************************************
**
** file:	btreetestarray.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains array application class code derived
** from the b-tree framework's array data class to allow
** for the array data class being tested by having
** the array application class act as a wrapper.
**
************************************************************/

#ifndef	BTREETESTARRAY_H
#define	BTREETESTARRAY_H

#include <stdint.h>

#include <list>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <btreearray.h>

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
class CBTreeArrayTestSubscriptAccessWrapper;

typedef struct arrayEntry_s
{
	uint32_t		nData;
	uint32_t		nDebug;

	bool			operator!= (const struct arrayEntry_s sOperand) const
	{
		return ((nData != sOperand.nData) || (nDebug != sOperand.nDebug));
	}
} arrayEntry_t;

template <class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter, uint64_t, uint32_t> >
class CBTreeArrayTest
	:	public CBTreeArray <arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

	typedef CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
																			CBTreeArray_t;

	typedef typename CBTreeArray_t::CBTreeBase_t							CBTreeBase_t;

	typedef typename CBTreeArray_t::iterator								iterator;
	typedef typename CBTreeArray_t::const_iterator							const_iterator;
	
						CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

						CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT);

						~CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													();

	template <class _t_iterator>
	void				assign						(_t_iterator sItFirst, _t_iterator sItLast);
	void				assign						(_t_sizetype nNewSize, const arrayEntry_t& rVal);

	void				push_back					(const arrayEntry_t& rData);

	void				pop_back					();

	template <class _t_iterator>
	iterator			insert						(const_iterator sCIterPos, _t_iterator sItFirst, _t_iterator sItLast);

	iterator			insert						(const_iterator sCIterPos, const arrayEntry_t& rData);
	iterator			insert						(const_iterator sCIterPos, const _t_sizetype nLen, const arrayEntry_t& rData);

	iterator			erase						(const_iterator sCIterPos);
	iterator			erase						(const_iterator sCIterFirst, const_iterator sCIterLast);

	void				swap						(CBTreeArrayTest &rArray);

	void				clear						();

	bool				operator==					(const CBTreeArrayTest &rArray) const;
	bool				operator!=					(const CBTreeArrayTest &rArray) const;

	void				test						() const;

	bool				show_data					(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos) const;

	CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
						&operator=					(const CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT);

protected:

	bool				set_at						(const _t_sizetype nPos, const arrayEntry_t &rData);
	bool				get_at						(const _t_sizetype nPos, arrayEntry_t &rData) const;

	std::list<arrayEntry_t>							*m_pClRef;
};

#endif // BTREETESTARRAY_H

#include "btreetestarray.cpp"
