/************************************************************
**
** file:	btreefileioprop.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** file data layer properties class.
**
************************************************************/

#ifndef BTREEIOFILEPROP_CPP
#define BTREEIOFILEPROP_CPP

#include "io/properties/btreefileioprop.h"

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeIOpropertiesFile<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeIOpropertiesFile (const char *pszPathName, _t_addresstype nAddrSpaceSoftLimit, _t_addresstype nBlockSize)
	:	CBTreeIOproperties<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype> ()
	,	m_nAddrSpaceSoftLimit (nAddrSpaceSoftLimit)
	, m_nBlockSize (nBlockSize)
{
	m_pStrPathNames = new ::std::string (pszPathName);

	BTREE_ASSERT (m_pStrPathNames != NULL, "CBTreeIOpropertiesFile::CBTreeIOpropertiesFile (const char *, uint64_t): insufficient memory!");
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeIOpropertiesFile<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeIOpropertiesFile (CBTreeIOpropertiesFile &rBT)
	:	CBTreeIOproperties<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype> ()
	,	m_nAddrSpaceSoftLimit (rBT.m_nAddrSpaceSoftLimit)
	,	m_nBlockSize (rBT.m_nBlockSize)
{
	m_pStrPathNames = new ::std::string (*(rBT.m_pStrPathNames));

	BTREE_ASSERT (m_pStrPathNames != NULL, "CBTreeIOpropertiesFile::CBTreeIOpropertiesFile (CBTreeIOpropertiesFile &): insufficient memory!");
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeIOpropertiesFile<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::~CBTreeIOpropertiesFile ()
{
	delete m_pStrPathNames;
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
const char *CBTreeIOpropertiesFile<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_pathname () const
{
	return (m_pStrPathNames->c_str ());
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_addresstype CBTreeIOpropertiesFile<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_address_space_soft_limit () const
{
	return (m_nAddrSpaceSoftLimit);
}

template<class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_addresstype CBTreeIOpropertiesFile<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_blocksize () const
{
	return (m_nBlockSize);
}

#endif
