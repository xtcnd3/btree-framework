/************************************************************
**
** file:	btreeramioprop.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** RAM data layer properties class.
**
************************************************************/

#ifndef BTREEIORAMPROP_H
#define BTREEIORAMPROP_H

#include <stdint.h>

#include "btreecommon.h"
#include "btreeioprop.h"

class CBTreeIOpropertiesRAM : public CBTreeIOproperties
{
public:

					CBTreeIOpropertiesRAM		();

					CBTreeIOpropertiesRAM		(CBTreeIOpropertiesRAM &rBT);

					~CBTreeIOpropertiesRAM		();

protected:

};

#endif // BTREEIORAMPROP_H
