/************************************************************
**
** file:	btreeioram.h
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's
** linear IO data layer class.
**
************************************************************/

#ifndef	BTREEIOLINEAR_H
#define	BTREEIOLINEAR_H

#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <stdint.h>
#include <memory.h>
#include <stddef.h>

#if defined (_MSC_VER)

 #include <io.h>

#elif defined(__GNUC__) || defined(__GNUG__)

 #include <string.h>
 #include <stdlib.h>
 #include <sys/time.h>
 #include <sys/vfs.h>

#endif

#include "io/data_layer/base/btreeio.h"
#include "super_classes/btreeaux.h"

template<class _t_datalayerproperties>
class CBTreeLinearIO : public CBTreeIO<_t_datalayerproperties>
{
public:

	typedef typename _t_datalayerproperties::size_type				size_type;
	typedef typename _t_datalayerproperties::node_iter_type			node_iter_type;
	typedef typename _t_datalayerproperties::sub_node_iter_type		sub_node_iter_type;
	typedef typename _t_datalayerproperties::address_type			address_type;
	typedef typename _t_datalayerproperties::offset_type			offset_type;

	// construction
							CBTreeLinearIO<_t_datalayerproperties>
												(
													uint32_t nNumDataPools, 
													CBTreeIOperBlockPoolDesc_t *psDataPools
												);

							~CBTreeLinearIO<_t_datalayerproperties>
												();

protected:

	// address generation
	inline const uint8_t	*get_node_base			(uint32_t nPool, node_iter_type nNode);
	
	uint8_t											**m_ppsPools;
};

#endif // BTREEIOLINEAR_H

#include "btreeiolinear.cpp"
