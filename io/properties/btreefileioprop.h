/************************************************************
**
** file:	btreefileioprop.h
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** file data layer properties class.
**
************************************************************/

#ifndef BTREEIOFILEPROP_H
#define BTREEIOFILEPROP_H

#include <stdint.h>
#include <stdlib.h>

#include <type_traits>
#include <string>

#include "common/btreecommon.h"
#include "io/properties/base/btreeioprop.h"
#include "btreeiofile.h"

template<class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_addresstype = uint64_t, class _t_offsettype = uint32_t>
class CBTreeIOpropertiesFile : public CBTreeIOproperties<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
{
#define	BAYERTREE_FILE_IO_PROPERTIES_ADDR_SPACE_SOFT_LIMIT	((_t_sizetype) 1048576)

public:

	typedef CBTreeFileIO<CBTreeIOpropertiesFile<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype> >
															data_layer_type;

	typedef _t_sizetype										size_type;
	typedef _t_nodeiter										node_iter_type;
	typedef _t_subnodeiter									sub_node_iter_type;
	typedef _t_addresstype									address_type;
	typedef _t_offsettype									offset_type;

	typedef ::std::true_type								has_latency;

					CBTreeIOpropertiesFile<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
															(
																const char *pszPathName, 
																address_type nAddrSpaceSoftLimit = BAYERTREE_FILE_IO_PROPERTIES_ADDR_SPACE_SOFT_LIMIT, 
																address_type nBlockSize = 4096
															);

					CBTreeIOpropertiesFile<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
															(CBTreeIOpropertiesFile &rBT);

					~CBTreeIOpropertiesFile<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
															();

	const char		*get_pathname							() const;
	
	address_type	get_address_space_soft_limit			() const;

	address_type	get_blocksize							() const;

protected:

	::std::string	*m_pStrPathNames;
	address_type	m_nAddrSpaceSoftLimit;
	address_type	m_nBlockSize;
};

#endif // BTREEIOFILEPROP_H

#include "btreefileioprop.cpp"
