/************************************************************
**
** file:	btreeaux.cpp
** author:	Andreas Steffens
** license:	LGPL v3
**
** description:
**
** This file contains code for the b-tree framework's super and
** interface class.
**
************************************************************/

#include <stdio.h>
#include <string.h>

#include "super_classes/btreeaux.h"

CBTreeSuper::CBTreeSuper ()
	:	m_nNumPools (0)
	,	m_pPoolDesc (NULL)
{
	m_sTimeStamp.nAccCtr = ~0x0;
	
	this->update_time_stamp ();
}

CBTreeSuper::CBTreeSuper (const CBTreeSuper &rContainer)
	:	m_nNumPools (0)
	,	m_pPoolDesc (NULL)
{
	uint32_t	ui32;

	for (ui32 = 0; ui32 < rContainer.m_nNumPools; ui32++)
	{
		this->create_dataPool (&(rContainer.m_pPoolDesc[ui32]));
	}

	m_sTimeStamp.nAccCtr = ~0x0;
	
	this->update_time_stamp ();
}

CBTreeSuper::CBTreeSuper (CBTreeSuper &&rRhsContainer)
	:	m_nNumPools (0)
	,	m_pPoolDesc (NULL)
{
	// to make sure any iterators, that are were deemed evaluated before the move construction
	// on the old (moved away) container, are not trying to access the previous content, we
	// have to make sure that the updated time stamp looks different so that any iterator
	// accessing the new content has to re-evaluate

	uint32_t	nHalfAccCtr = ~0x0;

	nHalfAccCtr >>= 1;
	nHalfAccCtr++;
	
	// we are achieving this by seeking for the greatest distance possible from the original access counter
	m_sTimeStamp.nAccCtr = rRhsContainer.m_sTimeStamp.nAccCtr + nHalfAccCtr;
	
	_swap (rRhsContainer);
}

CBTreeSuper::~CBTreeSuper ()
{
	if (m_pPoolDesc != NULL)
	{
		delete [] m_pPoolDesc;
	}
}

btree_time_stamp_t CBTreeSuper::get_time_stamp () const
{
	return (m_sTimeStamp);
}

CBTreeSuper & CBTreeSuper::operator= (const CBTreeSuper &rContainer)
{
	if (this != &rContainer)
	{
		if (m_pPoolDesc != NULL)
		{
			delete [] m_pPoolDesc;
		}

		m_nNumPools = 0;

		uint32_t	ui32;

		for (ui32 = 0; ui32 < rContainer.m_nNumPools; ui32++)
		{
			this->create_dataPool (&(rContainer.m_pPoolDesc[ui32]));
		}

		this->update_time_stamp ();
	}

	return (*this);
}

CBTreeSuper & CBTreeSuper::operator= (CBTreeSuper &&rRhsContainer)
{
	_swap (rRhsContainer);

	return (*this);
}

void CBTreeSuper::update_time_stamp ()
{
	m_sTimeStamp.sTime = ::std::chrono::high_resolution_clock::now ();
	m_sTimeStamp.nAccCtr++;
}

uint32_t CBTreeSuper::create_dataPool (CBTreeIOperBlockPoolDesc_t *pPoolDesc)
{
	uint32_t	nNewID = m_nNumPools;

	m_nNumPools++;

	CBTreeIOperBlockPoolDesc_t *pTempPoolDesc = new CBTreeIOperBlockPoolDesc_t [m_nNumPools];

	BTREE_ASSERT (pTempPoolDesc != NULL, "CBTreeSuper::create_dataPool: insufficient memory!");

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

void CBTreeSuper::_swap (CBTreeSuper &rContainer)
{
	fast_swap (this->m_sTimeStamp, rContainer.m_sTimeStamp);
	fast_swap (this->m_nNumPools, rContainer.m_nNumPools);
	fast_swap (this->m_pPoolDesc, rContainer.m_pPoolDesc);

	// make sure the time stamps are different, which forces any
	// iterator referring to these containers has to re-evaluate
	// on its next access
		
	this->update_time_stamp ();

	rContainer.update_time_stamp ();
}
