#ifndef	BTREEIOBLOCK_H
#define	BTREEIOBLOCK_H

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

#include "btreeio.h"

template <class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_addresstype = uint64_t, class _t_offsettype = uint32_t>
class CBTreeBlockIO : public CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
{
	typedef struct btree_block_io_descriptor_s
	{
		uint8_t		*pBlockData;
		uint32_t	nAccessCtr;
		bool		bMarkedForUnmap;
	} btree_block_io_descriptor_t;

public:

	// construction
							CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
														(
															_t_addresstype nBlockSize, 
															_t_subnodeiter nNodeSize,
															uint32_t nNumDataPools, 
															CBTreeIOperBlockPoolDesc_t *psDataPools
														);

							~CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
														();

	// properties
	_t_nodeiter				get_maxNodes				();
	
	_t_offsettype			get_dataAlignment			();
	_t_offsettype			get_alignedOffset			(_t_offsettype nOffset);

	// resource management
	void					set_root_node				(_t_nodeiter nRootNode);

protected:

	// address generation
	inline _t_addresstype	get_blockAddr				(_t_nodeiter nNode);
	virtual _t_offsettype	get_poolOffset				();
	inline _t_addresstype	get_node_offset				(_t_nodeiter nNode);
	inline _t_addresstype	get_nodeAddr				(_t_nodeiter nNode);
	
	inline _t_offsettype	get_per_node_pool_offset	(uint32_t nPool);
	inline _t_addresstype	get_pool_address			(uint32_t nPool, _t_nodeiter nNode);

	_t_offsettype			generate_pool_offsets		();

	void					realloc_descriptor_vector	(_t_nodeiter nMaxNodes);

	uint32_t				convert_node_to_descriptor	(_t_nodeiter nNode, bool bRoundUp = false);

	// initialisation
	void					setup						(_t_addresstype nBlockSize);

	void					increment_access_counter	(uint32_t nDescriptor);

	_t_addresstype										m_nBlockSize;				// optimal block size
	_t_subnodeiter										m_nNodeSize;				// node size parameter a.k.a. t
	uint32_t											m_nNodesPerBlock;			// number of nodes that fit into one block
	uint32_t											m_nNodesPerBlockVectorSize;	// number of nodes that fit into one block (vector size)

	_t_offsettype										m_nAlignedNodeSize;			// size of one node in a block plus alignment
	
	_t_offsettype										*m_pnPerNodePoolOffset;

	btree_block_io_descriptor_t							*m_psDescriptorVector;
	uint32_t											m_nDescriptorVectorSize;

	_t_nodeiter											m_nPrevRootNode;

	uint32_t											m_nMaxAccessCtr;			// number of accesses before degrading statistics
};

#endif // BTREEIOBLOCK_H

#include "btreeioblock.cpp"
