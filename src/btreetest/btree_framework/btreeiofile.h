/************************************************************
**
** file:	btreeiofile.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** file data layer class.
**
************************************************************/

#ifndef	BTREEFILEIO_H
#define	BTREEFILEIO_H

#include "btreeio.h"
#include "btreeaux.h"
#include "btreefileioprop.h"

#include "CFileMem.h"

template <class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_addresstype = uint64_t, class _t_offsettype = uint32_t>
class CBTreeFileIO : public CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
{
public:
	// construction
						CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
												(
													CBTreeIOpropertiesFile &rDataLayerProperties, 
													uint32_t nBlockSize, 
													_t_subnodeiter nNodeSize,
													uint32_t nNumDataPools, 
													CBTreeIOperBlockPoolDesc_t *psDataPools
												);

						~CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
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
	
	uint32_t			get_perBlockPoolCacheSize	(uint32_t nPool);
	uint32_t			get_perBlockPoolCacheMask	(uint32_t nPool);
	uint8_t				*get_perBlockPoolCacheBaseAddress	(uint32_t nPool);
	_t_nodeiter			get_perBlockPoolCacheNode	(uint32_t nPool, uint32_t nLine);
	bool				get_perBlockPoolCacheValid	(uint32_t nPool, uint32_t nLine);
	_t_offsettype		get_perBlockPoolOffset		(uint32_t nPool);
	_t_addresstype		get_perBlockPoolAddr		(uint32_t nPool, _t_nodeiter nNode);
	uint32_t			get_perBlockPoolCacheLength	(uint32_t nPool, _t_nodeiter nNode);

	void				set_perBlockPoolCacheNode	(uint32_t nPool, uint32_t nLine, _t_nodeiter nNode);
	void				set_perBlockPoolCacheLength	(uint32_t nPool, _t_nodeiter nNode, uint32_t nLen);
	void				set_perBlockPoolCacheToInvalid	(uint32_t nPool, uint32_t nLine);

	CFileMem										*m_pFile;
		
	uint32_t										*m_pnPerBlockPoolCacheMaskes;
	uint8_t											**m_ppPoolCaches;
	_t_nodeiter										**m_ppnPerBlockPoolCacheNode;
	bool											**m_ppbPerBlockPoolCacheValid;
	uint32_t										**m_ppnPerBlockPoolCacheLen;

	string											m_strTempFile;

	CBTreeIOpropertiesFile							*m_pClDataLayerProperties;
};

#endif // BTREEFILEIO_H

#include "btreeiofile.cpp"
