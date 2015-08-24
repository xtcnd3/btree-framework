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
#define	BAYERTREE_FILE_IO_PROPERTIES_ADDR_SPACE_SOFT_LIMIT	1048576ULL

public:

					CBTreeIOpropertiesFile					(
																const char *pszPathName, 
																uint64_t nAddrSpaceSoftLimit = BAYERTREE_FILE_IO_PROPERTIES_ADDR_SPACE_SOFT_LIMIT
															);

					CBTreeIOpropertiesFile					(CBTreeIOpropertiesFile &rBT);

					~CBTreeIOpropertiesFile					();

	const char		*get_pathname							();
	
	uint64_t		get_address_space_soft_limit			();

protected:

	string			*m_pStrPathNames;
	uint64_t		m_nAddrSpaceSoftLimit;
};

#endif // BTREEIOFILEPROP_H

