/************************************************************
**
** file:	btreeaux.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's super and
** interface class.
**
************************************************************/

#ifndef	BTREEAUX_H
#define	BTREEAUX_H

#include <stdint.h>

#include <ctime>

#include "btreecommon.h"
#include "btreeio.h"

class CBTreeSuper
{
public:

								CBTreeSuper					();
								~CBTreeSuper				();

	btree_time_stamp_t			get_time_stamp				();

	void						swap						(CBTreeSuper &rBT);

protected:

	void						update_time_stamp			();

	uint32_t					create_dataPool				(CBTreeIOperBlockPoolDesc_t *pPoolDesc);

	uint32_t					m_nNumPools;
	CBTreeIOperBlockPoolDesc_t	*m_pPoolDesc;

	btree_time_stamp_t			m_sTimeStamp;
};

template <class _t_sizetype = uint64_t>
class CBTreeIf
{
public:

	// construction
						CBTreeIf<_t_sizetype>
													()
						{
						};
						
	// destruction
						~CBTreeIf<_t_sizetype>
													()
						{
						};

	virtual bool		empty						() = 0;
	virtual _t_sizetype	size						() = 0;
};

#endif // BTREEAUX_H
