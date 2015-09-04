/************************************************************
**
** file:	btreeioram.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** RAM data layer class.
**
************************************************************/

#ifndef	BTREERAMIO_H
#define	BTREERAMIO_H

#include "btreeiolinear.h"
#include "btreeramioprop.h"

class CBTreeIOpropertiesRAM;

template <class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_addresstype = uint64_t, class _t_offsettype = uint32_t>
class CBTreeRAMIO : public CBTreeLinearIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
{
public:

	// construction
						CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
												(
													CBTreeIOpropertiesRAM &rDataLayerProperties, 
													_t_addresstype nBlockSize, 
													_t_subnodeiter nNodeSize,
													uint32_t nNumDataPools, 
													CBTreeIOperBlockPoolDesc_t *psDataPools
												);

						~CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
												();

	// monitoring
	void				get_performance_counters				(uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR]);

	// data access primitives
	template<class _t_dl_data>
	inline _t_dl_data *	get_pooledData				(uint32_t nPool, _t_nodeiter nNode);

	template<class _t_dl_data>
	inline _t_dl_data *	get_pooledData				(uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nEntry);

	// mid level data access
	template<class _t_dl_data>
	void				insert_dataIntoPool			(uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nNodeLen, _t_subnodeiter nOffset, _t_subnodeiter nDataLen, const _t_dl_data *pData);

	// maintanence
	void				set_size					(_t_nodeiter nMaxNodes);
	void				unload						();

	// cache management
	void				unload_from_cache			(_t_nodeiter nNode);

	// cache information
	bool				is_dataCached				(uint32_t nPool, _t_nodeiter nNode);

	void				showdump					(std::ofstream &ofs, _t_nodeiter nTreeSize, char *pAlloc);

protected:

	CBTreeIOpropertiesRAM							*m_pClDataLayerProperties;
};

#endif // BTREERAMIO_H

#include "btreeioram.cpp"
