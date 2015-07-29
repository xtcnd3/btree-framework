/************************************************************
**
** file:	btreefileioprop.h
** author:	Andreas Steffens
** license:	GPL v2
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

#include <string>

#include "btreecommon.h"
#include "btreeioprop.h"

using namespace std;

class CBTreeIOpropertiesFile : public CBTreeIOproperties
{
#define	BAYERTREE_FILE_CACHE_NUMBER_OF_LOG2_LINES		15
#define	BAYERTREE_FILE_CACHE_NUMBER_OF_BYTES_PER_LINE	512ULL

public:

					CBTreeIOpropertiesFile					(
																const char *pszPathName, 
																uint32_t nFileCacheNumOfLog2Lines = BAYERTREE_FILE_CACHE_NUMBER_OF_LOG2_LINES, 
																uint64_t nFileCacheNumOfBytesPerLine = BAYERTREE_FILE_CACHE_NUMBER_OF_BYTES_PER_LINE
															);

					CBTreeIOpropertiesFile					(CBTreeIOpropertiesFile &rBT);

					~CBTreeIOpropertiesFile					();

	void			attach_pathname							(const char *pszPathName);
	const char		*get_pathname							();
	uint32_t		get_num_of_log2_cache_lines				();
	uint64_t		get_num_of_log2_bytes_per_cache_line	();

protected:

	string			**m_ppStrPathNames;
	uint32_t		m_nNumPaths;
	uint32_t		m_nFileCacheNumOfLog2Lines;
	uint64_t		m_nFileCacheNumOfBytesPerLine;
};

#endif // BTREEIOFILEPROP_H

