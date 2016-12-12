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

	typedef CBTreeRAMIO												CBTreeRAMIO_t;

	typedef typename _t_datalayerproperties::size_type				size_type;
	typedef typename _t_datalayerproperties::node_iter_type			node_iter_type;
	typedef typename _t_datalayerproperties::sub_node_iter_type		sub_node_iter_type;
	typedef typename _t_datalayerproperties::address_type			address_type;
	typedef typename _t_datalayerproperties::offset_type			offset_type;

	// construction
						CBTreeRAMIO<_t_datalayerproperties>
													(
														const _t_datalayerproperties &rDataLayerProperties, 
														const sub_node_iter_type nNodeSize,
														const uint32_t nNumDataPools, 
														const CBTreeIOperBlockPoolDesc_t *psDataPools
													);

						~CBTreeRAMIO<_t_datalayerproperties>
													();

	// monitoring
	void				get_performance_counters	(uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR]);

	// data access primitives
	template<class _t_dl_data>
	inline _t_dl_data *	get_pooledData				(const uint32_t nPool, const node_iter_type nNode) const;

	template<class _t_dl_data>
	inline _t_dl_data *	get_pooledData				(const uint32_t nPool, const node_iter_type nNode, const sub_node_iter_type nEntry) const;

	// mid level data access
	template<class _t_dl_data>
	void				insert_dataIntoPool			(const uint32_t nPool, const node_iter_type nNode, const sub_node_iter_type nNodeLen, const sub_node_iter_type nOffset, const sub_node_iter_type nDataLen);

	// maintanence
	void				set_size					(const node_iter_type nMaxNodes);
	void				unload						();

	// cache management
	void				unload_from_cache			(const node_iter_type nNode);

	// cache information
	bool				is_dataCached				(const uint32_t nPool, const node_iter_type nNode) const;

	void				showdump					(std::ofstream &ofs, const node_iter_type nTreeSize) const;

protected:

	_t_datalayerproperties							*m_pClDataLayerProperties;
};

#endif // BTREERAMIO_H

#include "btreeioram.cpp"
