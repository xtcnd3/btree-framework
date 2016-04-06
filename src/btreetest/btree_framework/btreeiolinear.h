/************************************************************
**
** file:	btreeioram.h
** author:	Andreas Steffens
** license:	GPL v2
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

#if defined (WIN32)

 #if defined (_HAS_MFC)
  #include <afxtempl.h>
 #endif

 #if !defined (_HAS_MFC)
  #include <windows.h>
 #endif

 #include <io.h>

#elif defined (LINUX)

 #include <string.h>
 #include <stdlib.h>
 #include <sys/time.h>
 #include <sys/vfs.h>

 #include <windows.h>

#endif

#include "btreeio.h"
#include "btreeaux.h"

template <class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_addresstype = uint64_t, class _t_offsettype = uint32_t>
class CBTreeLinearIO : public CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
{
public:

	// construction
							CBTreeLinearIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
												(
													_t_addresstype nBlockSize, 
													uint32_t nNumDataPools, 
													CBTreeIOperBlockPoolDesc_t *psDataPools
												);

							~CBTreeLinearIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>
												();

protected:

	// address generation
	inline const uint8_t	*get_node_base			(uint32_t nPool, _t_nodeiter nNode);
	
	uint8_t											**m_ppsPools;
};

#endif // BTREEIOLINEAR_H

#include "btreeiolinear.cpp"
