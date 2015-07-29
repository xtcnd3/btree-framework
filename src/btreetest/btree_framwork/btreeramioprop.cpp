/************************************************************
**
** file:	btreeramioprop.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** RAM data layer properties class.
**
************************************************************/

#include "btreeramioprop.h"

CBTreeIOpropertiesRAM::CBTreeIOpropertiesRAM ()
	:	CBTreeIOproperties (CBTREE_IO_PROPERTIES_RAM)
{
}

CBTreeIOpropertiesRAM::CBTreeIOpropertiesRAM (CBTreeIOpropertiesRAM &rBT)
	:	CBTreeIOproperties (CBTREE_IO_PROPERTIES_RAM)
{
	rBT;
}

CBTreeIOpropertiesRAM::~CBTreeIOpropertiesRAM ()
{
}


