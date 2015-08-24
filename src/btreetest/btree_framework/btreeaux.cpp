/************************************************************
**
** file:	btreeaux.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's super and
** interface class.
**
************************************************************/

#include <stdio.h>
#include <string.h>

#include "btreeaux.h"

CBTreeSuper::CBTreeSuper ()
	:	m_nNumPools (0)
	,	m_pPoolDesc (NULL)
{
	m_sTimeStamp.nAccCtr = ~0x0;
	
	this->update_time_stamp ();
}

CBTreeSuper::~CBTreeSuper ()
{
	if (m_pPoolDesc != NULL)
	{
		delete [] m_pPoolDesc;
	}
}

btree_time_stamp_t CBTreeSuper::get_time_stamp ()
{
	return (m_sTimeStamp);
}

void CBTreeSuper::swap (CBTreeSuper &rBT)
{
	this->update_time_stamp ();

	rBT.update_time_stamp ();
}

void CBTreeSuper::update_time_stamp ()
{
	m_sTimeStamp.sTime = ::std::clock ();
	m_sTimeStamp.nAccCtr++;
}

uint32_t CBTreeSuper::create_dataPool (CBTreeIOperBlockPoolDesc_t *pPoolDesc)
{
	uint32_t	nNewID = m_nNumPools;

	m_nNumPools++;

	CBTreeIOperBlockPoolDesc_t *pTempPoolDesc = new CBTreeIOperBlockPoolDesc_t [m_nNumPools];

	BTREE_ASSERT (pTempPoolDesc != NULL, "CBTreeSuper::CBTreeSuper: insufficient memory!");

	if (m_pPoolDesc != NULL)
	{
		uint32_t		ui32;

		for (ui32 = 0; ui32 < nNewID; ui32++)
		{
			pTempPoolDesc[ui32] = m_pPoolDesc[ui32];
		}

		delete [] m_pPoolDesc;
	}

	m_pPoolDesc = pTempPoolDesc;

	m_pPoolDesc[nNewID] = *pPoolDesc;

	return (nNewID);
}
