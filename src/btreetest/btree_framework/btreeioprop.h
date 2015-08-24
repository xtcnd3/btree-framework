/************************************************************
**
** file:	btreeioprop.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** abstract data layer properties class.
**
************************************************************/

#ifndef BTREEIOPROP_H
#define BTREEIOPROP_H

#include <stdint.h>

#include "btreecommon.h"

class CBTreeIOproperties
{
public:

	typedef enum
	{
		CBTREE_IO_PROPERTIES_RAM, 
		CBTREE_IO_PROPERTIES_FILE
	} cbtree_io_properties_type_e;

									CBTreeIOproperties		(cbtree_io_properties_type_e ePropertyType);

									~CBTreeIOproperties		();

	cbtree_io_properties_type_e		get_type	();

protected:

	cbtree_io_properties_type_e		m_ePropertyType;
};

#endif // BTREEIOPROP_H

