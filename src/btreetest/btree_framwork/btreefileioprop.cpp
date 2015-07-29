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

CBTreeIOpropertiesFile::CBTreeIOpropertiesFile (const char *pszPathName, uint32_t nFileCacheNumOfLog2Lines, uint64_t nFileCacheNumOfBytesPerLine)
	:	CBTreeIOproperties (CBTREE_IO_PROPERTIES_FILE)
	,	m_nFileCacheNumOfLog2Lines (nFileCacheNumOfLog2Lines)
	,	m_nFileCacheNumOfBytesPerLine (nFileCacheNumOfBytesPerLine)
	,	m_nNumPaths (1)
{
	m_ppStrPathNames = new string * [m_nNumPaths];

	BTREE_ASSERT (m_ppStrPathNames != NULL, "CBTreeIOpropertiesFile::CBTreeIOpropertiesFile (const char *, uint32_t , uint64_t): insufficient memory!");

	m_ppStrPathNames[0] = new string (pszPathName);

	BTREE_ASSERT (m_ppStrPathNames[0] != NULL, "CBTreeIOpropertiesFile::CBTreeIOpropertiesFile (const char *, uint32_t , uint64_t): insufficient memory! (string)");
}

CBTreeIOpropertiesFile::CBTreeIOpropertiesFile (CBTreeIOpropertiesFile &rBT)
	:	CBTreeIOproperties (CBTREE_IO_PROPERTIES_FILE)
	,	m_nFileCacheNumOfLog2Lines (rBT.m_nFileCacheNumOfLog2Lines)
	,	m_nFileCacheNumOfBytesPerLine (rBT.m_nFileCacheNumOfBytesPerLine)
	,	m_nNumPaths (rBT.m_nNumPaths)
{
	uint32_t	i;

	m_ppStrPathNames = new string * [m_nNumPaths];

	BTREE_ASSERT (m_ppStrPathNames != NULL, "CBTreeIOpropertiesFile::CBTreeIOpropertiesFile (CBTreeIOpropertiesFile &): insufficient memory!");

	for (i = 0; i < m_nNumPaths; i++)
	{
		m_ppStrPathNames[i] = new string ();

		BTREE_ASSERT (m_ppStrPathNames[i] != NULL, "CBTreeIOpropertiesFile::CBTreeIOpropertiesFile (CBTreeIOpropertiesFile &): insuficient memory! (string)");

		*(m_ppStrPathNames[i]) = *(rBT.m_ppStrPathNames[i]);
	}
}

CBTreeIOpropertiesFile::~CBTreeIOpropertiesFile ()
{
	uint32_t	i;

	for (i = 0; i < m_nNumPaths; i++)
	{
		delete m_ppStrPathNames[i];
	}

	delete [] m_ppStrPathNames;
}

void CBTreeIOpropertiesFile::attach_pathname (const char *pszPathName)
{
	string		**ppStrPathNames = new string * [m_nNumPaths + 1];
	uint32_t	i;

	BTREE_ASSERT (ppStrPathNames != NULL, "CBTreeIOpropertiesFile::attach_pathname (const char *): insufficient memory!");

	for (i = 0; i < m_nNumPaths; i++)
	{
		ppStrPathNames[i] = new string ();

		BTREE_ASSERT (ppStrPathNames[i] != NULL, "CBTreeIOpropertiesFile::attach_pathname (CBTreeIOpropertiesFile &): insuficient memory! (string)");

		*(ppStrPathNames[i]) = *(m_ppStrPathNames[i]);

		delete m_ppStrPathNames[i];
	}

	ppStrPathNames[i] = new string (pszPathName);

	delete [] m_ppStrPathNames;

	m_ppStrPathNames = ppStrPathNames;

	m_nNumPaths++;
}

const char *CBTreeIOpropertiesFile::get_pathname ()
{
	uint32_t	n = 0;

	if (m_nNumPaths > 1)
	{
		n = rand ();
		n = n % m_nNumPaths;
	}

	return (m_ppStrPathNames[n]->c_str ());
}

uint32_t CBTreeIOpropertiesFile::get_num_of_log2_cache_lines ()
{
	return (m_nFileCacheNumOfLog2Lines);
}

uint64_t CBTreeIOpropertiesFile::get_num_of_log2_bytes_per_cache_line ()
{
	return (m_nFileCacheNumOfBytesPerLine);
}


