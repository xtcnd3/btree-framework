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

	bool			operator!= (const struct arrayEntry_s sOperand)
	{
		return ((nData != sOperand.nData) || (nDebug != sOperand.nDebug));
	}
} arrayEntry_t;

template <class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter, uint64_t, uint32_t> >
class CBTreeArrayTest
	:	public virtual CBTreeArrayDataIf <arrayEntry_t, _t_sizetype>
	,	public CBTreeArray <arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:

#if defined(__GNUC__) || defined(__GNUG__)

	typedef typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator			iterator;
	typedef typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator	const_iterator;
	
#endif

						CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
													(_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize);

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

	_t_sizetype			insert_at					(const _t_sizetype nPos, const arrayEntry_t &rData);

	_t_sizetype			replace_at					(const _t_sizetype nStart, const _t_sizetype nLen, arrayEntry_t *pSrcData);

	_t_sizetype			remove_at					(const _t_sizetype nPos);

	void				clear						();

	bool				set_at						(const _t_sizetype nPos, const arrayEntry_t &rData);
	bool				get_at						(const _t_sizetype nPos, arrayEntry_t &rData);

	bool				operator==					(CBTreeArrayTest &rArray);
	bool				operator!=					(CBTreeArrayTest &rArray);

	void				test						();

	bool				show_data					(std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos);
//	bool				show_node					(std::ofstream &ofs, const _t_nodeiter nNode, const _t_subnodeiter nSubPos);

	CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
						&operator=					(CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT);

protected:

	std::list<arrayEntry_t>							*m_pClRef;
};

#endif // BTREETESTARRAY_H

#include "btreetestarray.cpp"
