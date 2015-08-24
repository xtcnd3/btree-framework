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

#include "btreeio.h"
#include "btreeaux.h"
#include "btreeramioprop.h"

class CBTreeIOpropertiesRAM;

template <class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_addresstype = uint64_t, class _t_offsettype = uint32_t>
class CBTreeRAMIO : public CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
{
public:

	// construction
						CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
												(
													CBTreeIOpropertiesRAM &rDataLayerProperties, 
													uint32_t nBlockSize, 
													_t_subnodeiter nNodeSize,
													uint32_t nNumDataPools, 
													CBTreeIOperBlockPoolDesc_t *psDataPools
												);

						~CBTreeRAMIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
												();

	// monitoring
	void				get_performance_counters				(uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR]);

	// data access primitives
	void				get_pooledData				(uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, _t_subnodeiter nEntry, void *pData);
	void				set_pooledData				(uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, _t_subnodeiter nEntry, void *pData);

	void				get_pooledData				(uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, void *pData);
	void				set_pooledData				(uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, void *pData);

	// mid level data access
	void				insert_dataIntoPool			(uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nNodeLen, _t_subnodeiter nOffset, _t_subnodeiter nDataLen, void *pData);

	// maintanence
	void				set_size					(_t_nodeiter nMaxNodes);
	void				unload						();

	// cache management
	void				unload_from_cache			(_t_nodeiter nNode);

	// cache information
	bool				is_dataCached				(uint32_t nPool, _t_nodeiter nNode);
	
	uint32_t			get_node_buffer_cache_size	();

	void				showdump					(std::ofstream &ofs, _t_nodeiter nTreeSize, char *pAlloc);

protected:

	// address generation
	_t_addresstype		get_blockAddr				(_t_nodeiter nNode);
	_t_offsettype		get_poolOffset				();
	_t_addresstype		get_nodeAddr				(_t_nodeiter nNode);
	
	void											*m_pMemory;

	CBTreeIOpropertiesRAM							*m_pClDataLayerProperties;
};

#endif // BTREERAMIO_H

#include "btreeioram.cpp"

