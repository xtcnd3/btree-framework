/************************************************************
**
** file:	btreefileioprop.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** file data layer properties class.
**
************************************************************/

#include "btreefileioprop.h"

CBTreeIOpropertiesFile::CBTreeIOpropertiesFile (const char *pszPathName, uint64_t nAddrSpaceSoftLimit)
	:	CBTreeIOproperties (CBTREE_IO_PROPERTIES_FILE)
	,	m_nAddrSpaceSoftLimit (nAddrSpaceSoftLimit)
{
	m_pStrPathNames = new string (pszPathName);

	BTREE_ASSERT (m_pStrPathNames != NULL, "CBTreeIOpropertiesFile::CBTreeIOpropertiesFile (const char *, uint32_t , uint64_t): insufficient memory!");
}

CBTreeIOpropertiesFile::CBTreeIOpropertiesFile (CBTreeIOpropertiesFile &rBT)
	:	CBTreeIOproperties (CBTREE_IO_PROPERTIES_FILE)
	,	m_nAddrSpaceSoftLimit (rBT.m_nAddrSpaceSoftLimit)
{
	m_pStrPathNames = new string (*(rBT.m_pStrPathNames));

	BTREE_ASSERT (m_pStrPathNames != NULL, "CBTreeIOpropertiesFile::CBTreeIOpropertiesFile (CBTreeIOpropertiesFile &): insufficient memory!");
}

CBTreeIOpropertiesFile::~CBTreeIOpropertiesFile ()
{
	delete m_pStrPathNames;
}

const char *CBTreeIOpropertiesFile::get_pathname ()
{
	return (m_pStrPathNames->c_str ());
}

uint64_t CBTreeIOpropertiesFile::get_address_space_soft_limit ()
{
	return (m_nAddrSpaceSoftLimit);
}
