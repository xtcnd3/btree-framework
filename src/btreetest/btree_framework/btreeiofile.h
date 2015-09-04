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

#if defined (WIN32)

 #include <io.h>
 #include <windows.h>

#elif defined (LINUX)

 #include <unistd.h>
 #include <sys/mman.h>
 #include <string.h>
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <sys/ioctl.h>
 #include <linux/fs.h>
 #include <errno.h>

#endif

#include "btreeioblock.h"
#include "btreeaux.h"
#include "btreefileioprop.h"

template <class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_addresstype = uint64_t, class _t_offsettype = uint32_t>
class CBTreeFileIO : public CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
{
public:
	// construction
						CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
												(
													CBTreeIOpropertiesFile &rDataLayerProperties, 
													_t_addresstype nBlockSize, 
													_t_subnodeiter nNodeSize,
													uint32_t nNumDataPools, 
													CBTreeIOperBlockPoolDesc_t *psDataPools
												);

						~CBTreeFileIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
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

	// resources
	void				set_size					(_t_nodeiter nMaxNodes);
	void				unload						();

	// cache management
	void				unload_from_cache			(_t_nodeiter nNode);

	// cache information
	bool				is_dataCached				(uint32_t nPool, _t_nodeiter nNode);

	// resource management
	void				terminate_access			();

	void				showdump					(std::ofstream &ofs, _t_nodeiter nTreeSize, char *pAlloc);

protected:

	void				init_mapping				();
	void				exit_mapping				();

	void				map_descriptor				(uint32_t nDescriptor);
	void				sync_descriptor				(uint32_t nDescriptor);
	void				unmap_descriptor			(uint32_t nDescriptor);

	void				unmap_all_descriptors		(bool bExceptRoot);

	string											m_strTempFile;

#if defined (WIN32)
	HANDLE											m_hFile;
	HANDLE											m_hFileMapping;
#elif defined (LINUX)
	int												m_nFileDesc;
#endif

	CBTreeIOpropertiesFile							*m_pClDataLayerProperties;

	bool											m_bNoMapHandling;

	_t_addresstype									m_nTotalAddressSpace;
};

#endif // BTREEFILEIO_H

#include "btreeiofile.cpp"
