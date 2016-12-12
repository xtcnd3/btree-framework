/************************************************************
**
** file:	btreeaux.h
** author:	Andreas Steffens
** license:	LGPL v3
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

#include <chrono>

#include "common/btreecommon.h"
#include "io/data_layer/base/btreeio.h"

class CBTreeSuper
{
public:

								CBTreeSuper					();
	explicit					CBTreeSuper					(const CBTreeSuper &rContainer);
								CBTreeSuper					(CBTreeSuper &&rRhsContainer);

								~CBTreeSuper				();

	btree_time_stamp_t			get_time_stamp				() const;

	CBTreeSuper &				operator=					(const CBTreeSuper &rContainer);
	CBTreeSuper &				operator=					(CBTreeSuper &&rRhsContainer);

protected:

	void						update_time_stamp			();

	uint32_t					create_dataPool				(CBTreeIOperBlockPoolDesc_t *pPoolDesc);

	void						_swap						(CBTreeSuper &rContainer);

	void						_local_swap					(CBTreeSuper &rContainer);

	uint32_t					m_nNumPools;
	CBTreeIOperBlockPoolDesc_t	*m_pPoolDesc;

	btree_time_stamp_t			m_sTimeStamp;
};

#endif // BTREEAUX_H
