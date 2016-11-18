/************************************************************
**
** file:	btreeio.h
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** abstract data layer class.
**
************************************************************/

#ifndef	BTREEIO_H
#define	BTREEIO_H

#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <stdint.h>
#include <memory.h>
#include <stddef.h>

#if defined (_MSC_VER)

 #include <io.h>

#elif defined(__GNUC__) || defined(__GNUG__)

 #include <string.h>
 #include <stdlib.h>
 #include <sys/time.h>
 #include <sys/vfs.h>

#endif

#include "io/properties/base/btreeioprop.h"

#define	BTREEDATA_ASSERT(c,m)							if (!(c)) throw (new ::std::runtime_error (m))

#define RESERVATION_VECTOR_LOG2_BYTES_PER_SUPERBLOCK	6

typedef enum
{
	PERFCTR_SUPER_BLOCK_ADDR,
	PERFCTR_BLOCK_ADDR,
	PERFCTR_RESERVATION_VECTOR_ADDR,
	PERFCTR_NODE_ADDR,
	PERFCTR_SUBNODE_ADDR,
	PERFCTR_SERIAL_VECTOR_ADDR,
	PERFCTR_DATA_ADDR,
	PERFCTR_FILE_CACHE,
	PERFCTR_NODEDATA_DATA,
	PERFCTR_SUBNODE_DATA,
	PERFCTR_DATA,
	PERFCTR_TERMINATOR
} bayertree_perfctrs_e;

typedef struct
{
	uint32_t		nTotalSize;
	uint32_t		nEntrySize;
} CBTreeIOperBlockPoolDesc_t;

template<class _t_datalayerproperties>
class CBTreeIO
{
public:

	typedef typename _t_datalayerproperties::size_type				size_type;
	typedef typename _t_datalayerproperties::node_iter_type			node_iter_type;
	typedef typename _t_datalayerproperties::sub_node_iter_type		sub_node_iter_type;
	typedef typename _t_datalayerproperties::address_type			address_type;
	typedef typename _t_datalayerproperties::offset_type			offset_type;

	// construction
						CBTreeIO<_t_datalayerproperties>
													(
														const uint32_t nNumDataPools, 
														const CBTreeIOperBlockPoolDesc_t *psDataPools
													);

						~CBTreeIO<_t_datalayerproperties>
													();

	// properties
	node_iter_type		get_maxNodes				();
	
	offset_type			get_dataAlignment			();
	offset_type			get_alignedOffset			(offset_type nOffset);

	// monitoring
	void				get_performance_counters	(uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR]);

	// cache information
	uint32_t			get_pool_entry_size			(uint32_t nPool);

	// resources
	virtual void		set_size					(node_iter_type nMaxNodes) = 0;
	virtual void		unload						() = 0;

	// resource management
	virtual void		set_root_node				(node_iter_type nRootNode);
	virtual void		terminate_access			();

	// cache management
	virtual void		unload_from_cache			(node_iter_type nNode) = 0;

	// cache settings
	void				set_cacheFreeze				(bool bEnabled);

	// cache information
	virtual bool		is_dataCached				(uint32_t nPool, node_iter_type nNode) = 0;

	virtual void		showdump					(std::ofstream &ofs, node_iter_type nTreeSize, char *pAlloc) = 0;

protected:

	uint32_t			get_pool_total_size			(uint32_t nPool);

	uint32_t										m_nNumDataPools;
	CBTreeIOperBlockPoolDesc_t						*m_psDataPools;

	node_iter_type									m_nMaxNodes;

	// cache
	bool											m_bCacheFreeze;
	
#if defined (USE_PERFORMANCE_COUNTERS)

	uint64_t										m_anHitCtrs[PERFCTR_TERMINATOR];
	uint64_t										m_anMissCtrs[PERFCTR_TERMINATOR];

#endif
};

#endif // BTREEIO_H

#include "btreeio.cpp"
