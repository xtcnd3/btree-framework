/************************************************************
**
** file:	btreeioram.h
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** RAM data layer class.
**
************************************************************/

#ifndef	BTREERAMIO_H
#define	BTREERAMIO_H

#include "io/data_layer/type/btreeiolinear.h"

template<class _t_datalayerproperties>
class CBTreeRAMIO : public CBTreeLinearIO<_t_datalayerproperties>
{
public:

	typedef typename _t_datalayerproperties::size_type				size_type;
	typedef typename _t_datalayerproperties::node_iter_type			node_iter_type;
	typedef typename _t_datalayerproperties::sub_node_iter_type		sub_node_iter_type;
	typedef typename _t_datalayerproperties::address_type			address_type;
	typedef typename _t_datalayerproperties::offset_type			offset_type;

	// construction
						CBTreeRAMIO<_t_datalayerproperties>
													(
														_t_datalayerproperties &rDataLayerProperties, 
														sub_node_iter_type nNodeSize,
														uint32_t nNumDataPools, 
														CBTreeIOperBlockPoolDesc_t *psDataPools
													);

						~CBTreeRAMIO<_t_datalayerproperties>
													();

	// monitoring
	void				get_performance_counters	(uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR]);

	// data access primitives
	template<class _t_dl_data>
	inline _t_dl_data *	get_pooledData				(uint32_t nPool, node_iter_type nNode);

	template<class _t_dl_data>
	inline _t_dl_data *	get_pooledData				(uint32_t nPool, node_iter_type nNode, sub_node_iter_type nEntry);

	// mid level data access
	template<class _t_dl_data>
	void				insert_dataIntoPool			(uint32_t nPool, node_iter_type nNode, sub_node_iter_type nNodeLen, sub_node_iter_type nOffset, sub_node_iter_type nDataLen);

	// maintanence
	void				set_size					(node_iter_type nMaxNodes);
	void				unload						();

	// cache management
	void				unload_from_cache			(node_iter_type nNode);

	// cache information
	bool				is_dataCached				(uint32_t nPool, node_iter_type nNode);

	void				showdump					(std::ofstream &ofs, node_iter_type nTreeSize, char *pAlloc);

protected:

	_t_datalayerproperties							*m_pClDataLayerProperties;
};

#endif // BTREERAMIO_H

#include "btreeioram.cpp"
