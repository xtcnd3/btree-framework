/************************************************************
**
** file:	btreeiofile.h
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** file data layer class.
**
************************************************************/

#ifndef	BTREEFILEIO_H
#define	BTREEFILEIO_H

#if defined (_MSC_VER)

 #include <io.h>

 #include <windows.h>

#elif defined(__GNUC__) || defined(__GNUG__)

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

#include <chrono>
#include <random>
#include <exception>

#include "io/data_layer/type/btreeioblock.h"
#include "super_classes/btreeaux.h"

template<class _t_datalayerproperties>
class CBTreeFileIO : public CBTreeBlockIO<_t_datalayerproperties>
{
public:
	
	typedef	CBTreeFileIO											CBTreeFileIO_t;

	typedef typename _t_datalayerproperties::size_type				size_type;
	typedef typename _t_datalayerproperties::node_iter_type			node_iter_type;
	typedef typename _t_datalayerproperties::sub_node_iter_type		sub_node_iter_type;
	typedef typename _t_datalayerproperties::address_type			address_type;
	typedef typename _t_datalayerproperties::offset_type			offset_type;

	// construction
						CBTreeFileIO<_t_datalayerproperties>
												(
													const _t_datalayerproperties &rDataLayerProperties, 
													const sub_node_iter_type nNodeSize,
													const uint32_t nNumDataPools, 
													const CBTreeIOperBlockPoolDesc_t *psDataPools
												);

						~CBTreeFileIO<_t_datalayerproperties>
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

	// resources
	void				set_size					(const node_iter_type nMaxNodes);
	void				unload						();

	// cache management
	void				unload_from_cache			(const node_iter_type nNode);

	// cache information
	bool				is_dataCached				(const uint32_t nPool, const node_iter_type nNode) const;

	// resource management
	void				terminate_access			();

	void				showdump					(std::ofstream &ofs, const node_iter_type nTreeSize) const;

protected:

	void				init_mapping				();
	void				exit_mapping				();

	void				map_descriptor				(const uint32_t nDescriptor);
	void				sync_descriptor				(const uint32_t nDescriptor);
	void				unmap_descriptor			(const uint32_t nDescriptor);

	void				unmap_all_descriptors		(const bool bExceptRoot);

	::std::string									m_strTempFile;

#if defined (_MSC_VER)

	HANDLE											m_hFile;
	HANDLE											m_hFileMapping;

#elif defined(__GNUC__) || defined(__GNUG__)

	int												m_nFileDesc;

#endif

	_t_datalayerproperties							*m_pClDataLayerProperties;

	bool											m_bNoMapHandling;

	address_type									m_nTotalAddressSpace;
};

#endif // BTREEFILEIO_H

#include "btreeiofile.cpp"
