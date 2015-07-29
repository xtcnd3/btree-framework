/************************************************************
**
** file:	btreeioprop.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** abstract data layer properties class.
**
************************************************************/

#include "btreeioprop.h"

CBTreeIOproperties::CBTreeIOproperties (cbtree_io_properties_type_e ePropertyType)
	:	m_ePropertyType (ePropertyType)
{
}

CBTreeIOproperties::~CBTreeIOproperties ()
{
}

CBTreeIOproperties::cbtree_io_properties_type_e CBTreeIOproperties::get_type ()
{
	return (m_ePropertyType);
}

