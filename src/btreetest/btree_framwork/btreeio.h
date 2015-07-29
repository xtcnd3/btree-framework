/************************************************************
**
** file:	btreeio.h
** author:	Andreas Steffens
** license:	GPL v2
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

#if defined (WIN32)

 #if defined (_HAS_MFC)
  #include <afxtempl.h>
 #endif

 #if !defined (_HAS_MFC)
  #include <windows.h>
 #endif

 #include <io.h>

#elif defined (LINUX)

 #include <string.h>
 #include <stdlib.h>
 #include <sys/time.h>
 #include <sys/vfs.h>

 #include <windows.h>

#endif

#include "btreeioprop.h"

#define	BTREEDATA_ASSERT(c,m)							if (!(c)) throw (new runtime_error (m))

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

using namespace std;

typedef struct
{
	uint32_t		nTotalSize;
	uint32_t		nEntrySize;
	uint32_t		nCacheVectorSize;
} CBTreeIOperBlockPoolDesc_t;

template <class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_addresstype = uint64_t, class _t_offsettype = uint32_t>
class CBTreeIO
{
public:

	// construction
						CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
												(
													uint32_t nBlockSize, 
													_t_subnodeiter nNodeSize,
													uint32_t nNumDataPools, 
													CBTreeIOperBlockPoolDesc_t *psDataPools
												);

						~CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
												();

	// properties
	_t_nodeiter			get_maxNodes				();
	
	_t_offsettype		get_dataAlignment			();
	_t_offsettype		get_alignedOffset			(_t_offsettype nOffset);

	// monitoring
	void				get_performance_counters				(uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR]);

	// cache information
	virtual uint32_t	get_perBlockPoolRawEntrySize	(uint32_t nPool);

	// data access primitives
//	virtual void		get_pooledData				(uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, _t_subnodeiter nEntry, void *pData) = 0;
//	virtual void		set_pooledData				(uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, _t_subnodeiter nEntry, void *pData) = 0;

//	virtual void		get_pooledData				(uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, void *pData) = 0;
//	virtual void		set_pooledData				(uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nLen, void *pData) = 0;

	// mid level data access
//	virtual void		insert_dataIntoPool			(uint32_t nPool, _t_nodeiter nNode, _t_subnodeiter nNodeLen, _t_subnodeiter nOffset, _t_subnodeiter nDataLen, void *pData) = 0;

	// maintanence
	virtual void		set_size					(_t_nodeiter nMaxNodes) = 0;
	virtual void		unload						() = 0;

	// cache management
	virtual void		unload_from_cache			(_t_nodeiter nNode) = 0;

	// cache settings
	void				set_cacheFreeze				(bool bEnabled);

	// cache information
	virtual bool		is_dataCached				(uint32_t nPool, _t_nodeiter nNode) = 0;
	
	virtual uint32_t	get_node_buffer_cache_size	();

	virtual void		showdump					(std::ofstream &ofs, _t_nodeiter nTreeSize, char *pAlloc) = 0;

protected:

	// address generation
	_t_addresstype		get_blockAddr				(_t_nodeiter nNode);
	_t_offsettype		get_poolOffset				();
	_t_addresstype		get_nodeAddr				(_t_nodeiter nNode);
	
	virtual uint32_t	get_perBlockPoolTotalBlockSize	(uint32_t nPool);
	_t_offsettype		get_perBlockPoolOffset		(uint32_t nPool);
	_t_addresstype		get_perBlockPoolAddr		(uint32_t nPool, _t_nodeiter nNode);

	_t_offsettype		get_alignedDataOffsets		();

	// low level access control
	void			set_writeThrough			(bool bWriteThrough);

	// initialisation
	void			setup					(uint32_t nBlockSize);

	// debugging
	void				print_addresses				();

	uint32_t										m_nBlockSize;				// optimal block size
	uint32_t										m_nNodeSize;				// node descriptor size
	uint32_t										m_nNodesPerBlock;			// number of nodes that fit into one block
	uint32_t										m_nNodesPerBlockVectorSize;	// number of nodes that fit into one block (vector size)
	
	uint32_t										m_nNumDataPools;
	CBTreeIOperBlockPoolDesc_t						*m_psDataPools;

	uint32_t										m_nAlignedNodeSize;			// size of one node in a block plus alignment
	
	_t_offsettype									*m_pnPerBlockPoolOffset;

	_t_nodeiter										m_nMaxNodes;

	// cache
	bool											m_bCacheFreeze;
	bool											m_bWriteThrough;

#if defined (USE_PERFORMANCE_COUNTERS)
	uint64_t										m_anHitCtrs[PERFCTR_TERMINATOR];
	uint64_t										m_anMissCtrs[PERFCTR_TERMINATOR];
#endif
};

#endif // BTREEIO_H

#include "btreeio.cpp"

