/************************************************************
**
** file:	avp_path_find_core.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains the core module to find the fastest path
** from one objective to another in the video game
** Aliens versus Predator on the Atari Jaguar.
**
************************************************************/

#include "avp_path_find_core.h"

field_t g_sAirduct1B[]	= {
#include "airduct1B.h"
};

field_t g_sAirduct2B[]	= {
#include "airduct2B.h"
};

field_t g_sAirduct3B[]	= {
#include "airduct3B.h"
};

field_t g_sAirduct3C[]	= {
#include "airduct3C.h"
};

field_t g_sAirduct4B[]	= {
#include "airduct4B.h"
};

field_t g_sAirduct4C[]	= {
#include "airduct4C.h"
};

field_t g_sAirduct5B[]	= {
#include "airduct5B.h"
};

field_t g_sAirduct5C[]	= {
#include "airduct5C.h"
};

field_t g_sSubLevel1[]	= {
#include "sublevel1.h"
};

field_t g_sSubLevel2[]	= {
#include "sublevel2.h"
};

field_t g_sSubLevel3[]	= {
#include "sublevel3.h"
};

field_t g_sSubLevel4[]	= {
#include "sublevel4.h"
};

field_t g_sSubLevel5[]	= {
#include "sublevel5.h"
};

field_t g_sAlienVessel[]	= {
#include "alienvessel.h"
};

field_t g_sPredatorShip[]	= {
#include "predatorship.h"
};

level_t g_sLevelList[] = {
	{38 + 1, 50 + 1, g_sSubLevel1}, 
	{41 + 1, 45 + 1, g_sSubLevel2}, 
	{42 + 1, 48 + 1, g_sSubLevel3}, 
	{51 + 1, 42 + 1, g_sSubLevel4}, 
	{38 + 1, 53 + 1, g_sSubLevel5}, 
	{52 + 1, 38 + 1, g_sAlienVessel}, 
	{46 + 1, 27 + 1, g_sPredatorShip}, 
	{16 + 1, 20 + 1, g_sAirduct1B}, 
	{15 + 1, 18 + 1, g_sAirduct2B}, 
	{17 + 1, 22 + 1, g_sAirduct3B}, 
	{17 + 1, 20 + 1, g_sAirduct3C}, 
	{16 + 1, 22 + 1, g_sAirduct4B}, 
	{18 + 1, 21 + 1, g_sAirduct4C}, 
	{17 + 1, 19 + 1, g_sAirduct5B}, 
	{17 + 1, 21 + 1, g_sAirduct5C}
};

bool	g_bPrintStatistics;

void start_timer (time_statistic_t *psTimeStatistic, uint32_t nID, uint64_t nStartComplexity)
{
	if (g_bPrintStatistics)
	{
		psTimeStatistic->nID = nID;
		psTimeStatistic->nComplexity = nStartComplexity;
		psTimeStatistic->sStartClock = std::chrono::system_clock::now ();
	}
}

void end_timer (time_statistic_t *psTimeStatistic, uint64_t nEndComplexity)
{
	std::chrono::system_clock::time_point	sEndClock;
	uint64_t								nAvgComplexity;
	
	if (g_bPrintStatistics)
	{
		sEndClock = std::chrono::system_clock::now ();

		std::chrono::duration<double> sTimeSpan = std::chrono::duration_cast<std::chrono::duration<double>> (sEndClock - psTimeStatistic->sStartClock);

		double			nTimeDiff = sTimeSpan.count ();
		double			nOpsPerSec;

		if (nEndComplexity != psTimeStatistic->nComplexity)
		{
			if (nEndComplexity > psTimeStatistic->nComplexity)
			{
				nTimeDiff /= (double) (nEndComplexity - psTimeStatistic->nComplexity);
			}
			else
			{
				nTimeDiff /= (double) (psTimeStatistic->nComplexity - nEndComplexity);
			}
		}

		if (nTimeDiff > 0.0)
		{
			nOpsPerSec = 1.0 / nTimeDiff;

			nAvgComplexity = (nEndComplexity + psTimeStatistic->nComplexity) / 2;

//			::std::cerr << psTimeStatistic->nID << "\t" << nAvgComplexity << "\t" << ((uint64_t) nOpsPerSec);

//			::std::cerr << "\t" << psTimeStatistic->nComplexity << "\t" << nEndComplexity << "\t" << psTimeStatistic->nStart.QuadPart << "\t" << sEndTime.QuadPart << "\t" << sFreq.QuadPart;

//			::std::cerr << ::std::endl;
		}
	}
}

void create_agent (uint32_t *pnNextAgentID, set_type_intrinsic_t *psAgentList, uint32_t nOffSpringAgent, multiset_agent_step_t *psAgentStepLists, agent_t *psProperties)
{
	typedef multiset_agent_step_t::const_iterator			citer_t;

	time_statistic_t	sTimeStat;
	
	citer_t			sCIter;
	agent_step_t	sNextStep;

	start_timer (&sTimeStat, 0, psAgentList->size ());
	{
		psAgentList->insert (*pnNextAgentID);
	}
	end_timer (&sTimeStat, psAgentList->size ());

	if ((*pnNextAgentID) != nOffSpringAgent)
	{
		agent_step_t	sAgentKey;

		sAgentKey.nAgent = nOffSpringAgent;

		uint64_t		u;
		uint64_t		nNumInst = psAgentStepLists->count (sAgentKey);

		agent_step_t	*psOffSpringAgentStepList = new agent_step_t[(size_t) nNumInst];

		if (psOffSpringAgentStepList == NULL)
		{
			printf ("ERROR: insufficient memory!\n");

			exit (-1);
		}

		sCIter = psAgentStepLists->lower_bound (sAgentKey);

		for (u = 0; u < nNumInst; u++, sCIter++)
		{
			psOffSpringAgentStepList[u] = *sCIter;
			
			psOffSpringAgentStepList[u].nAgent = *pnNextAgentID;
		}

		start_timer (&sTimeStat, 1, psAgentStepLists->size ());
		{
			for (u = 0; u < nNumInst; u++)
			{
				psAgentStepLists->insert (psOffSpringAgentStepList[u]);
			}

			sNextStep.nAgent = *pnNextAgentID;
			sNextStep.sAgent = *psProperties;

			psAgentStepLists->insert (sNextStep);
		}
		end_timer (&sTimeStat, psAgentStepLists->size ());

		delete [] psOffSpringAgentStepList;
	}

	(*pnNextAgentID)++;
}

field_t *get_field (uint32_t nPosX, uint32_t nPosY, uint32_t nLevel)
{
	uint32_t nWidth = g_sLevelList[nLevel].nWidth;
	uint32_t nLinearSubscription = nPosY * nWidth + nPosX;

	return (&(g_sLevelList[nLevel].psFieldList[nLinearSubscription]));
}

bool get_door_clearance (uint32_t nItemVector, wall_e eWall)
{
	uint32_t	u;
	uint32_t	nMaxClearance = 0;
	uint32_t	nReqClearance = 0;

	for (u = 0; u < 10; u++)
	{
		if ((nItemVector & (1 << (u + ITEM_CLEARANCE_CARD_1))) != 0x0)
		{
			nMaxClearance = u + 1;
		}
	}

	switch (eWall)
	{
	case WALL_WALL		:	
	case WALL_NO_WALL	:	
	case WALL_DOOR		:	
	case WALL_PORTAL	:	

							break;

	case WALL_DOOR_10	:	nReqClearance++;
	case WALL_DOOR_9	:	nReqClearance++;
	case WALL_DOOR_8	:	nReqClearance++;
	case WALL_DOOR_7	:	nReqClearance++;
	case WALL_DOOR_6	:	nReqClearance++;
	case WALL_DOOR_5	:	nReqClearance++;
	case WALL_DOOR_4	:	nReqClearance++;
	case WALL_DOOR_3	:	nReqClearance++;
	case WALL_DOOR_2	:	nReqClearance++;
	case WALL_DOOR_1	:	nReqClearance++;

							break;

	default				:	printf ("ERROR: get_door_clearance: Unknown wall type specified!\n");

							exit (-1);

							break;
	}

	return (nMaxClearance >= nReqClearance);
}

bool get_adjacent_path_unblocked (uint32_t nItemVector, wall_e eWall)
{
	switch (eWall)
	{
	case WALL_WALL		:	
	case WALL_PORTAL	:	return (false);

							break;

	case WALL_NO_WALL	:	
	case WALL_DOOR		:	return (true);

							break;

	case WALL_DOOR_1	:	
	case WALL_DOOR_2	:	
	case WALL_DOOR_3	:	
	case WALL_DOOR_4	:	
	case WALL_DOOR_5	:	
	case WALL_DOOR_6	:	
	case WALL_DOOR_7	:	
	case WALL_DOOR_8	:	
	case WALL_DOOR_9	:	
	case WALL_DOOR_10	:	return (get_door_clearance (nItemVector, eWall));

							break;

	default				:	printf ("ERROR: get_adjacent_path_unblocked: Unknown wall type specified!\n");

							exit (-1);

							break;
	}
}

uint32_t get_unblocked_paths (agent_step_t sAgentStep, uint32_t nItemVector)
{
	uint32_t		uRslt = 0x0;
	field_t			*psField;
	field_t			*psFieldN;
	field_t			*psFieldS;
	field_t			*psFieldE;
	field_t			*psFieldW;
	uint32_t	nPosX = sAgentStep.sAgent.nPosX;
	uint32_t	nPosY = sAgentStep.sAgent.nPosY;
	uint32_t	nLevel = sAgentStep.sAgent.nLevel;

	psField = get_field (nPosX, nPosY, nLevel);

	VERBOSE_OUTPUT_PRINT ("1000");
	VERBOSE_OUTPUT_PRINT (nPosX);
	VERBOSE_OUTPUT_PRINT (nPosY);
	VERBOSE_OUTPUT_PRINT (nLevel);

	// test north west direction
	if ((nPosX > 0) && (nPosY > 0))
	{
		VERBOSE_OUTPUT_PRINT ("1100");

		if ((psField->aeWalls[WALLDIR_NORTH] == WALL_NO_WALL) && (psField->aeWalls[WALLDIR_WEST] == WALL_NO_WALL))
		{
			VERBOSE_OUTPUT_PRINT ("1200");

			psFieldN = get_field (nPosX, nPosY - 1, nLevel);

			if (psFieldN->aeWalls[WALLDIR_WEST] == WALL_NO_WALL)
			{
				VERBOSE_OUTPUT_PRINT ("1300");

				psFieldW = get_field (nPosX - 1, nPosY, nLevel);

				if (psFieldW->aeWalls[WALLDIR_NORTH] == WALL_NO_WALL)
				{
					VERBOSE_OUTPUT_PRINT ("1400");

					uRslt |= UNBLOCKED_PATH_NW;
				}
			}
		}
	}

	// test west direction
	if (nPosX > 0)
	{
		VERBOSE_OUTPUT_PRINT ("1101");

		if (get_adjacent_path_unblocked (nItemVector, psField->aeWalls[WALLDIR_WEST]))
		{
			VERBOSE_OUTPUT_PRINT ("1201");

			uRslt |= UNBLOCKED_PATH_W;
		}
	}

	// test south west direction
	if ((nPosX > 0) && (nPosY < (g_sLevelList[nLevel].nHeight - 1)))
	{
		VERBOSE_OUTPUT_PRINT ("1102");

		if ((psField->aeWalls[WALLDIR_SOUTH] == WALL_NO_WALL) && (psField->aeWalls[WALLDIR_WEST] == WALL_NO_WALL))
		{
			VERBOSE_OUTPUT_PRINT ("1202");

			psFieldS = get_field (nPosX, nPosY + 1, nLevel);

			if (psFieldS->aeWalls[WALLDIR_WEST] == WALL_NO_WALL)
			{
				VERBOSE_OUTPUT_PRINT ("1302");

				psFieldW = get_field (nPosX - 1, nPosY, nLevel);

				if (psFieldW->aeWalls[WALLDIR_SOUTH] == WALL_NO_WALL)
				{
					VERBOSE_OUTPUT_PRINT ("1402");

					uRslt |= UNBLOCKED_PATH_SW;
				}
			}
		}
	}

	// test south direction
	if (nPosY < (g_sLevelList[nLevel].nHeight - 1))
	{
		VERBOSE_OUTPUT_PRINT ("1103");

		if (get_adjacent_path_unblocked (nItemVector, psField->aeWalls[WALLDIR_SOUTH]))
		{
			VERBOSE_OUTPUT_PRINT ("1203");

			uRslt |= UNBLOCKED_PATH_S;
		}
	}

	// test south east direction
	if ((nPosX < (g_sLevelList[nLevel].nWidth - 1)) && (nPosY < (g_sLevelList[nLevel].nHeight - 1)))
	{
		VERBOSE_OUTPUT_PRINT ("1104");

		if ((psField->aeWalls[WALLDIR_SOUTH] == WALL_NO_WALL) && (psField->aeWalls[WALLDIR_EAST] == WALL_NO_WALL))
		{
			VERBOSE_OUTPUT_PRINT ("1204");

			psFieldS = get_field (nPosX, nPosY + 1, nLevel);

			if (psFieldS->aeWalls[WALLDIR_EAST] == WALL_NO_WALL)
			{
				VERBOSE_OUTPUT_PRINT ("1304");

				psFieldE = get_field (nPosX + 1, nPosY, nLevel);

				if (psFieldE->aeWalls[WALLDIR_SOUTH] == WALL_NO_WALL)
				{
					VERBOSE_OUTPUT_PRINT ("1404");

					uRslt |= UNBLOCKED_PATH_SE;
				}
			}
		}
	}

	// test east direction
	if (nPosX < (g_sLevelList[nLevel].nWidth - 1))
	{
		VERBOSE_OUTPUT_PRINT ("1105");

		if (get_adjacent_path_unblocked (nItemVector, psField->aeWalls[WALLDIR_EAST]))
		{
			VERBOSE_OUTPUT_PRINT ("1205");

			uRslt |= UNBLOCKED_PATH_E;
		}
	}

	// test north east direction
	if ((nPosX < (g_sLevelList[nLevel].nWidth - 1)) && (nPosY > 0))
	{
		VERBOSE_OUTPUT_PRINT ("1106");

		if ((psField->aeWalls[WALLDIR_NORTH] == WALL_NO_WALL) && (psField->aeWalls[WALLDIR_EAST] == WALL_NO_WALL))
		{
			VERBOSE_OUTPUT_PRINT ("1206");

			psFieldN = get_field (nPosX, nPosY - 1, nLevel);

			if (psFieldN->aeWalls[WALLDIR_EAST] == WALL_NO_WALL)
			{
				VERBOSE_OUTPUT_PRINT ("1306");

				psFieldE = get_field (nPosX + 1, nPosY, nLevel);

				if (psFieldE->aeWalls[WALLDIR_NORTH] == WALL_NO_WALL)
				{
					VERBOSE_OUTPUT_PRINT ("1406");

					uRslt |= UNBLOCKED_PATH_NE;
				}
			}
		}
	}

	// test north direction
	if (nPosY > 0)
	{
		VERBOSE_OUTPUT_PRINT ("1107");

		if (get_adjacent_path_unblocked (nItemVector, psField->aeWalls[WALLDIR_NORTH]))
		{
			VERBOSE_OUTPUT_PRINT ("1207");

			uRslt |= UNBLOCKED_PATH_N;
		}
	}

	if (psField->aeWalls[WALLDIR_NORTH] == WALL_PORTAL)
	{
		VERBOSE_OUTPUT_PRINT ("1108");

		uRslt |= UNBLOCKED_PORTAL_N;
	}

	if (psField->aeWalls[WALLDIR_SOUTH] == WALL_PORTAL)
	{
		VERBOSE_OUTPUT_PRINT ("1109");

		uRslt |= UNBLOCKED_PORTAL_S;
	}

	if (psField->aeWalls[WALLDIR_EAST] == WALL_PORTAL)
	{
		VERBOSE_OUTPUT_PRINT ("1110");

		uRslt |= UNBLOCKED_PORTAL_E;
	}

	if (psField->aeWalls[WALLDIR_WEST] == WALL_PORTAL)
	{
		VERBOSE_OUTPUT_PRINT ("1111");

		uRslt |= UNBLOCKED_PORTAL_W;
	}

	return (uRslt);
}

void remove_agent (uint32_t nAgent, set_type_intrinsic_t *psAgentList, multiset_agent_step_t *psAgentStepLists)
{
	agent_step_t		sAgentStep;
	time_statistic_t	sTimeStat;

	start_timer (&sTimeStat, 0, psAgentList->size ());	
	{
		psAgentList->erase (nAgent);
	}
	end_timer (&sTimeStat, psAgentList->size ());
	
	sAgentStep.nAgent = nAgent;

	start_timer (&sTimeStat, 1, psAgentStepLists->size ());
	{
		psAgentStepLists->erase (sAgentStep);
	}
	end_timer (&sTimeStat, psAgentStepLists->size ());
}

void remove_agents (uint32_t nAgent, set_type_intrinsic_t *psAgentList, multiset_agent_step_t *psAgentStepLists)
{
	typedef multiset_agent_step_t::const_iterator			citer_steps_t;

#if defined (USE_STL)

	multiset_agent_step_t			sAgentStepListsTmp;
	
	agent_step_t					sAgentStep;

#else

	multiset_agent_step_t			sAgentStepListsTmp (*psAgentStepLists, false);

#endif

	citer_steps_t					sItStepsLower;
	citer_steps_t					sItStepsUpper;

	psAgentList->clear ();
	psAgentList->insert (nAgent);

#if defined (USE_STL)

	sAgentStep.nAgent = nAgent;
	
	sItStepsLower = psAgentStepLists->lower_bound (sAgentStep);
	sItStepsUpper = psAgentStepLists->upper_bound (sAgentStep);
	
#else

	sItStepsLower = psAgentStepLists->lower_bound (nAgent);
	sItStepsUpper = psAgentStepLists->upper_bound (nAgent);

#endif

	sAgentStepListsTmp.insert (sItStepsLower, sItStepsUpper);

	psAgentStepLists->swap (sAgentStepListsTmp);
}

item_e get_item (agent_t sAgent)
{
	uint32_t	nPosX = sAgent.nPosX;
	uint32_t	nPosY = sAgent.nPosY;
	uint32_t	nLevel = sAgent.nLevel;
	field_t			*psField = get_field (nPosX, nPosY, nLevel);

	return (psField->eItem);
}

bool agent_was_here_before (uint32_t nAgent, multiset_agent_step_t *psAgentStepLists)
{
	typedef multiset_agent_step_t::const_iterator		citer_t;

	agent_step_t	sAgentStep;
	agent_t			sAgent;
	citer_t			sCIter;
	citer_t			sCIterLower;
	citer_t			sCIterUpper;
	
#if defined (USE_STL)

	sAgentStep.nAgent = nAgent;
	
	sCIterLower = psAgentStepLists->lower_bound (sAgentStep);
	sCIterUpper = psAgentStepLists->upper_bound (sAgentStep);
	
#else

	sCIterLower = psAgentStepLists->lower_bound (nAgent);
	sCIterUpper = psAgentStepLists->upper_bound (nAgent);

#endif

	sCIterUpper--;

	sAgent = ((agent_step_t)(*sCIterUpper)).sAgent;

	for (sCIter = sCIterLower; sCIter != sCIterUpper; sCIter++)
	{
		sAgentStep = *sCIter;

		if ((sAgent.nPosX == sAgentStep.sAgent.nPosX) && 
			(sAgent.nPosY == sAgentStep.sAgent.nPosY) && 
			(sAgent.nLevel == sAgentStep.sAgent.nLevel))
		{
			return (true);
		}
	}

	return (false);
}

void find_counter_portal (uint32_t nID, uint32_t nLevel, uint32_t *pnPosX, uint32_t *pnPosY, uint32_t *pnLevel)
{
	uint32_t	u;
	uint32_t	v;
	uint32_t	w;

	for (u = 0; u < (sizeof (g_sLevelList) / sizeof (*g_sLevelList)); u++)
	{
		for (v = 0; v < (g_sLevelList[u].nWidth * g_sLevelList[u].nHeight); v++)
		{
			for (w = 0; w < WALLDIR_END_MARKER; w++)
			{
				if (g_sLevelList[u].psFieldList[v].anPortalIDs[w] == nID)
				{
					if (u != nLevel)
					{
						*pnLevel = u;
						*pnPosX = v % g_sLevelList[u].nWidth;
						*pnPosY = v / g_sLevelList[u].nWidth;

						return;
					}
				}
			}
		}
	}

	printf ("ERROR: find_counter_portal: Failed to find portal!\n");

	exit (-1);
}

void find_path (objective_t *psObjectiveList, uint32_t nNumObjectives, vector_pos_t *psStepList, multiset_agent_step_t *psAgentStepLists, set_type_intrinsic_t *psAgentList, multiset_agent_t *psOccupation)
{
	typedef set_type_intrinsic_t::const_iterator						citer_agent_t;
	typedef multiset_agent_step_t::const_iterator						citer_steps_t;

	struct
	{
		uint32_t	nDiffPosX;
		uint32_t	nDiffPosY;
	} asDiffCoord[UNBLOCKED_TOTAL_DIRECTIONS] = 
	{
		{~0x0U, ~0x0U}, 
		{~0x0U, 0}, 
		{~0x0U, +1}, 
		{0, +1}, 
		{+1, +1}, 
		{+1, 0}, 
		{+1, ~0x0U}, 
		{0, ~0x0U}, 
		{0, 0}, 
		{0, 0}, 
		{0, 0}, 
		{0, 0}
	}; 

	uint32_t			nNextAgentID = 0;
	uint32_t			u, v;
	agent_t				sAgent;
	uint32_t			nObjective;
	uint64_t			nNumAgents;
	uint64_t			nNumTotalSteps;
	agent_step_t		sAgentStep;
	uint32_t			nItemVector = 0x0;
	uint32_t			uPathVector;
	uint64_t			nInstances;
	item_e				eItem;
	uint32_t			uMask;
	bool				bComplete;
	field_t				*psField;
	wallDir_e			eWallDir;
	citer_agent_t		sItAgent;
	citer_agent_t		sItAgentEnd;
	citer_steps_t		sItStepsUpper;
	uint64_t			ui64;
	uint64_t			nOrgAgentListSize;
	bool				bReEvaluateIter;
	time_statistic_t	sTimeStat;

	for (u = 0; u < (sizeof (g_sLevelList) / sizeof (*g_sLevelList)); u++)
	{
		for (v = 0; v < (g_sLevelList[u].nWidth * g_sLevelList[u].nHeight); v++)
		{
			if (g_sLevelList[u].psFieldList[v].eItem == ITEM_START)
			{
				break;
			}
		}

		if (v < (g_sLevelList[u].nWidth * g_sLevelList[u].nHeight))
		{
			break;
		}
	}

	sAgent.nPosX = v % g_sLevelList[u].nWidth;
	sAgent.nPosY = v / g_sLevelList[u].nWidth;
	sAgent.nLevel = u;
	sAgent.nObjective = 0x0;
	sAgent.nOOOvector = 0x0;

	sAgentStep.sAgent = sAgent;
	sAgentStep.nAgent = nNextAgentID;

	create_agent (&nNextAgentID, psAgentList, nNextAgentID, psAgentStepLists, &sAgent);

	psOccupation->insert (sAgent);

	psAgentStepLists->insert (sAgentStep);

//	psAgentStepLists->test_integrity ();

	for (nObjective = 0; nObjective < nNumObjectives; )
	{
		bComplete = false;

		nOrgAgentListSize = nNumAgents = psAgentList->size ();

		nNumTotalSteps = psAgentStepLists->size ();

		printf ("objective: %d agents: %I64u total step list size: %I64u   \n", nObjective, nNumAgents, nNumTotalSteps);

		sItAgentEnd = psAgentList->cend ();
		sItAgent = psAgentList->cbegin ();

		bReEvaluateIter = false;

		for (ui64 = 0; ui64 < nOrgAgentListSize; ui64++, sItAgent++)
		{
			VERBOSE_OUTPUT_PRINT ("0000");
			VERBOSE_OUTPUT_PRINT (ui64);

			{
				set_type_intrinsic_t::difference_type	nTmp = ::std::distance (psAgentList->cbegin(), sItAgent);

				VERBOSE_OUTPUT_PRINT (nTmp);

#if defined (_DEBUG) && !defined (USE_STL) && !defined (_MSC_VER)

				assert (nTmp == ui64);

#endif
			}

			VERBOSE_OUTPUT_PRINT (nOrgAgentListSize);
			VERBOSE_OUTPUT_PRINT (*sItAgent);

			nNumTotalSteps = psAgentStepLists->size ();

#if defined (_DEBUG)
			printf ("objective: %d agents: %I64u total step list size: %I64u   \n", nObjective, psAgentList->size (), nNumTotalSteps);
#else
			printf ("objective: %d agents: %I64u total step list size: %I64u   \r", nObjective, psAgentList->size (), nNumTotalSteps);
#endif

#if defined (USE_STL)

			if (bReEvaluateIter)
			{
				sItAgent = psAgentList->cbegin ();

				::std::advance(sItAgent, ui64);

				bReEvaluateIter = false;
			}

			sAgentStep.nAgent = *sItAgent;
			
			sItStepsUpper = psAgentStepLists->upper_bound (sAgentStep);

#else

			sItStepsUpper = psAgentStepLists->upper_bound (*sItAgent);

#endif

/*			{
				uint64_t	nTmp = ::std::templatedistance<citer_agent_t>(psAgentList->cbegin(), sItAgent);

				VERBOSE_OUTPUT_PRINT (nTmp);
			}*/

			sItStepsUpper--;

			sAgentStep = *sItStepsUpper;

			VERBOSE_OUTPUT_PRINT (sAgentStep.sAgent.nPosX);
			VERBOSE_OUTPUT_PRINT (sAgentStep.sAgent.nPosY);
			VERBOSE_OUTPUT_PRINT (sAgentStep.sAgent.nLevel);
			VERBOSE_OUTPUT_PRINT (sAgentStep.sAgent.nObjective);
			VERBOSE_OUTPUT_PRINT (sAgentStep.sAgent.nOOOvector);
			VERBOSE_OUTPUT_PRINT (sAgentStep.nAgent);

			eItem = get_item (sAgentStep.sAgent);

			if (eItem != ITEM_NO_ITEM)
			{
				VERBOSE_OUTPUT_PRINT ("0100");

				if (psObjectiveList[sAgentStep.sAgent.nObjective].eObjectiveParameter == OBJECTIVE_PARAMETER_IN_ORDER)
				{
					VERBOSE_OUTPUT_PRINT ("0200");

					if (psObjectiveList[sAgentStep.sAgent.nObjective].eItem == eItem)
					{
						VERBOSE_OUTPUT_PRINT ("0300");

						sAgentStep.sAgent.nObjective++;

						nObjective++;

						// destroy all agents except the current one
						bComplete = true;
					}
				}
				else
				{
					VERBOSE_OUTPUT_PRINT ("0201");

					if (!agent_was_here_before (sAgentStep.nAgent, psAgentStepLists))
					{
						VERBOSE_OUTPUT_PRINT ("0211");

						for (u = sAgentStep.sAgent.nObjective; u < nNumObjectives; u++)
						{
							VERBOSE_OUTPUT_PRINT ("0212");

							if (psObjectiveList[u].eObjectiveParameter == OBJECTIVE_PARAMETER_IN_ORDER)
							{
								VERBOSE_OUTPUT_PRINT ("0213");

								break;
							}

							if (psObjectiveList[u].eItem == eItem)
							{
								VERBOSE_OUTPUT_PRINT ("0221");

								uMask = 1 << (u - sAgentStep.sAgent.nObjective);

								if ((uMask & sAgentStep.sAgent.nOOOvector) == 0x0)
								{
									VERBOSE_OUTPUT_PRINT ("0222");

									sAgentStep.sAgent.nOOOvector |= uMask;

									break;
								}
							}
						}

						VERBOSE_OUTPUT_PRINT ("0201");

						bComplete = true;

						for (u = sAgentStep.sAgent.nObjective; u < nNumObjectives; u++)
						{
							VERBOSE_OUTPUT_PRINT ("0202");

							if (psObjectiveList[u].eObjectiveParameter == OBJECTIVE_PARAMETER_IN_ORDER)
							{
								VERBOSE_OUTPUT_PRINT ("0212");

								break;
							}

							uMask = 1 << (u - sAgentStep.sAgent.nObjective);

							if ((uMask & sAgentStep.sAgent.nOOOvector) == 0x0)
							{
								VERBOSE_OUTPUT_PRINT ("0222");

								bComplete = false;

								break;
							}
						}

						VERBOSE_OUTPUT_PRINT ("0211");

						if (bComplete)
						{
							VERBOSE_OUTPUT_PRINT ("0212");

							// destroy all agents except the current one
							sAgentStep.sAgent.nObjective = u;
							sAgentStep.sAgent.nOOOvector = 0;

							nObjective = u;
						}
					}
				}

				VERBOSE_OUTPUT_PRINT ("0101");

				// if the current in order task or a set of out of order tasks has been completed ...
				if (bComplete)
				{
					VERBOSE_OUTPUT_PRINT ("0201");

					// ... then check instruction

					if ((u >= nNumObjectives) || (psObjectiveList[u].eInstruction == INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION))
					{
						VERBOSE_OUTPUT_PRINT ("0301");

						// ... then destroy all agents except the current one
						remove_agents (sAgentStep.nAgent, psAgentList, psAgentStepLists);

						sItAgent = psAgentList->cend ();
						ui64 = nOrgAgentListSize;

/*
						sItAgentEnd = psAgentList->cend ();

						ui64 -= nOrgAgentListSize - psAgentList->size ();
						
						nOrgAgentListSize = psAgentList->size ();
*/
						bReEvaluateIter = true;

						// copy agent's path to step list
						while (psAgentStepLists->size () > 1)
						{
							VERBOSE_OUTPUT_PRINT ("0311");

							pos_t			sPos;
							citer_steps_t	sCIterStep = psAgentStepLists->cbegin ();

							sPos.nPosX = ((agent_step_t)(*sCIterStep)).sAgent.nPosX;
							sPos.nPosY = ((agent_step_t)(*sCIterStep)).sAgent.nPosY;
							sPos.nLevel = ((agent_step_t)(*sCIterStep)).sAgent.nLevel;

							start_timer (&sTimeStat, 1, psAgentStepLists->size ());
							{
								psAgentStepLists->erase (sCIterStep);
							}
							end_timer (&sTimeStat, psAgentStepLists->size ());

							psStepList->push_back (sPos);
						}
					}

					nItemVector |= (1 << eItem);
				}
			}

			VERBOSE_OUTPUT_PRINT ("0001");

			// get open path vector
			uPathVector = get_unblocked_paths (sAgentStep, nItemVector);

			// filter already occupied positions
			for (u = 0; u < UNBLOCKED_TOTAL_DIRECTIONS; u++)
			{
				VERBOSE_OUTPUT_PRINT ("0101");

				if ((uPathVector & (0x1 << u)) != 0x0)
				{
					VERBOSE_OUTPUT_PRINT ("0201");

					sAgent = sAgentStep.sAgent;

					if (u < UNBLOCKED_NUM_DIRECTIONS)
					{
						VERBOSE_OUTPUT_PRINT ("0301");

						sAgent.nPosX += asDiffCoord[u].nDiffPosX;
						sAgent.nPosY += asDiffCoord[u].nDiffPosY;
					}
					else
					{
						VERBOSE_OUTPUT_PRINT ("0302");

						psField = get_field (sAgent.nPosX, sAgent.nPosY, sAgent.nLevel);

						switch (1 << u)
						{
						case UNBLOCKED_PORTAL_N	:	eWallDir = WALLDIR_NORTH;
							
													break;

						case UNBLOCKED_PORTAL_S	:	eWallDir = WALLDIR_SOUTH;
							
													break;

						case UNBLOCKED_PORTAL_E	:	eWallDir = WALLDIR_EAST;
							
													break;

						case UNBLOCKED_PORTAL_W	:	eWallDir = WALLDIR_WEST;
							
													break;

						default					:	printf ("ERROR: find_path: counter corrupted!\n");

													exit (-1);

													break;
						}

						find_counter_portal (psField->anPortalIDs[eWallDir], sAgent.nLevel, &sAgent.nPosX, &sAgent.nPosY, &sAgent.nLevel);
					}

					VERBOSE_OUTPUT_PRINT ("0200");

					nInstances = psOccupation->count (sAgent);

					if (nInstances != 0)
					{
						VERBOSE_OUTPUT_PRINT ("0201");

						uPathVector &= ~(0x1 << u);
					}
				}
			}

			VERBOSE_OUTPUT_PRINT ("0002");

			if (uPathVector == 0x0)
			{
				VERBOSE_OUTPUT_PRINT ("0102");

				remove_agent (sAgentStep.nAgent, psAgentList, psAgentStepLists);

				sItAgent--;
				sItAgentEnd--;
				ui64--;
				nOrgAgentListSize--;

				bReEvaluateIter = true;
			}

			VERBOSE_OUTPUT_PRINT ("0003");

			for (u = 0; uPathVector != 0x0; uPathVector >>= 1, u++)
			{
				VERBOSE_OUTPUT_PRINT ("0103");

				if ((uPathVector & 0x1) != 0x0)
				{
					VERBOSE_OUTPUT_PRINT ("0203");

					sAgent = sAgentStep.sAgent;

					if (u < UNBLOCKED_NUM_DIRECTIONS)
					{
						VERBOSE_OUTPUT_PRINT ("0303");

						sAgent.nPosX += asDiffCoord[u].nDiffPosX;
						sAgent.nPosY += asDiffCoord[u].nDiffPosY;
					}
					else
					{
						VERBOSE_OUTPUT_PRINT ("0304");

						psField = get_field (sAgent.nPosX, sAgent.nPosY, sAgent.nLevel);

						switch (1 << u)
						{
						case UNBLOCKED_PORTAL_N	:	eWallDir = WALLDIR_NORTH;
							
													break;

						case UNBLOCKED_PORTAL_S	:	eWallDir = WALLDIR_SOUTH;
							
													break;

						case UNBLOCKED_PORTAL_E	:	eWallDir = WALLDIR_EAST;
							
													break;

						case UNBLOCKED_PORTAL_W	:	eWallDir = WALLDIR_WEST;
							
													break;

						default					:	printf ("ERROR: find_path: counter corrupted!\n");

													exit (-1);

													break;
						}

						find_counter_portal (psField->anPortalIDs[eWallDir], sAgent.nLevel, &sAgent.nPosX, &sAgent.nPosY, &sAgent.nLevel);
					}

					VERBOSE_OUTPUT_PRINT ("0204");

					if (uPathVector != 0x1)
					{
						VERBOSE_OUTPUT_PRINT ("0304");

						create_agent (&nNextAgentID, psAgentList, sAgentStep.nAgent, psAgentStepLists, &sAgent);

						bReEvaluateIter = true;
					}
					else
					{
						VERBOSE_OUTPUT_PRINT ("0305");

						sAgentStep.sAgent = sAgent;

						start_timer (&sTimeStat, 1, psAgentStepLists->size ());
						{
							psAgentStepLists->insert (sAgentStep);
						}
						end_timer (&sTimeStat, psAgentStepLists->size ());
					}

					start_timer (&sTimeStat, 2, psOccupation->size ());
					{
						psOccupation->insert (sAgent);
					}
					end_timer (&sTimeStat, psOccupation->size ());
				}
			}
		}
	}
}

uint32_t convert_wall_to_HTML_color (wall_e eWall)
{
	switch (eWall)
	{
	case WALL_NO_WALL	:	return (0xFFFFFF);
	case WALL_WALL		:	return (0x000000);
	case WALL_DOOR		:	return (0xCCCCCC);
	case WALL_DOOR_1	:	return (0x6F0000);
	case WALL_DOOR_2	:	return (0x7F0000);
	case WALL_DOOR_3	:	return (0x8F0000);
	case WALL_DOOR_4	:	return (0x9F0000);
	case WALL_DOOR_5	:	return (0xAF0000);
	case WALL_DOOR_6	:	return (0xBF0000);
	case WALL_DOOR_7	:	return (0xCF0000);
	case WALL_DOOR_8	:	return (0xDF0000);
	case WALL_DOOR_9	:	return (0xEF0000);
	case WALL_DOOR_10	:	return (0xFF0000);
	case WALL_PORTAL	:	return (0x555555);

							break;

	default				:	printf ("ERROR: convert_wall_to_HTML_color: Unknown wall type specified!\n");

							exit (-1);

							break;
	}

	return (~0x0);
}

const char *convert_item_to_text (item_e eItem)
{
	switch (eItem)
	{
	case ITEM_NO_ITEM			:	return ("");
	case ITEM_CLEARANCE_CARD_1	:	return ("#1");
	case ITEM_CLEARANCE_CARD_2	:	return ("#2");
	case ITEM_CLEARANCE_CARD_3	:	return ("#3");
	case ITEM_CLEARANCE_CARD_4	:	return ("#4");
	case ITEM_CLEARANCE_CARD_5	:	return ("#5");
	case ITEM_CLEARANCE_CARD_6	:	return ("#6");
	case ITEM_CLEARANCE_CARD_7	:	return ("#7");
	case ITEM_CLEARANCE_CARD_8	:	return ("#8");
	case ITEM_CLEARANCE_CARD_9	:	return ("#9");
	case ITEM_CLEARANCE_CARD_10	:	return ("#10");
	case ITEM_SHOTGUN			:	return ("SHOT<br>GUN");
	case ITEM_RIFLE				:	return ("RI<br>FLE");
	case ITEM_FLAMETHROWER		:	return ("FLME<br>THRW");
	case ITEM_SMARTGUN			:	return ("SMRT<br>GUN");
	case ITEM_MOTION_TRACKER	:	return ("MTN<br>TRCK");
	case ITEM_AMMO_SHOTGUN		:	return ("AM'O 1");
	case ITEM_AMMO_RIFLE		:	return ("AM'O 2");
	case ITEM_AMMO_FLAMETHROWER	:	return ("GAS");
	case ITEM_AMMO_SMARTGUN		:	return ("AM'O 4");
	case ITEM_MEDIKIT			:	return ("MED<br>KIT");
	case ITEM_FOOD				:	return ("FOOD");
	case ITEM_SELF_DESTRUCT		:	return ("SELF<br>DEST");
	case ITEM_ESCAPE_POD		:	return ("ESC<br>POD");
	case ITEM_START				:	return ("START");
	case ITEM_STEP				:	return ("X");
	case ITEM_STEP2				:	return ("XX");
	case ITEM_STEP3				:	return ("XXX");
	case ITEM_STEP4				:	return ("XXXX");
	case ITEM_STEP5				:	return ("XXXX<br>X");
	case ITEM_STEP6				:	return ("XXXX<br>XX");
	case ITEM_STEP7				:	return ("XXXX<br>XXX");
	case ITEM_STEPM				:	return ("XXXX<br>XXX>");

									break;

	default						:	printf ("ERROR: convert_item_to_text: Unknown item specified!\n");

									exit (-1);

									break;
	}
}

uint32_t convert_item_to_HTML_color (item_e eItem)
{
	switch (eItem)
	{
	case ITEM_NO_ITEM			:	return (0xFFFFFF);
	case ITEM_CLEARANCE_CARD_1	:	return (0x10FFFF);
	case ITEM_CLEARANCE_CARD_2	:	return (0x20FFFF);
	case ITEM_CLEARANCE_CARD_3	:	return (0x30FFFF);
	case ITEM_CLEARANCE_CARD_4	:	return (0x40FFFF);
	case ITEM_CLEARANCE_CARD_5	:	return (0x50FFFF);
	case ITEM_CLEARANCE_CARD_6	:	return (0x60FFFF);
	case ITEM_CLEARANCE_CARD_7	:	return (0x70FFFF);
	case ITEM_CLEARANCE_CARD_8	:	return (0x80FFFF);
	case ITEM_CLEARANCE_CARD_9	:	return (0x90FFFF);
	case ITEM_CLEARANCE_CARD_10	:	return (0xA0FFFF);
	case ITEM_SHOTGUN			:	return (0xFFFF20);
	case ITEM_RIFLE				:	return (0xFFFF40);
	case ITEM_FLAMETHROWER		:	return (0xFFFF60);
	case ITEM_SMARTGUN			:	return (0xFFFF80);
	case ITEM_MOTION_TRACKER	:	return (0xFFFFA0);
	case ITEM_AMMO_SHOTGUN		:	return (0xC00000);
	case ITEM_AMMO_RIFLE		:	return (0xC0C000);
	case ITEM_AMMO_FLAMETHROWER	:	return (0x707070);
	case ITEM_AMMO_SMARTGUN		:	return (0xB0B030);
	case ITEM_MEDIKIT			:	return (0x00C0C0);
	case ITEM_FOOD				:	return (0x008080);
	case ITEM_SELF_DESTRUCT		:	return (0xB0FFFF);
	case ITEM_ESCAPE_POD		:	return (0xC0FFFF);
	case ITEM_START				:	
	case ITEM_STEP				:	
	case ITEM_STEP2				:	
	case ITEM_STEP3				:	
	case ITEM_STEP4				:	
	case ITEM_STEP5				:	
	case ITEM_STEP6				:	
	case ITEM_STEP7				:	
	case ITEM_STEPM				:	return (0xFFC0FF);

									break;

	default						:	printf ("ERROR: convert_item_to_HTML_color: Unknown item specified!\n");

									exit (-1);

									break;
	}
}

void output_level (FILE *pf, level_t *psLevel, const char *pLevelName, uint32_t nLevel)
{
	uint32_t	x, y;
	field_t			*psField;

	fprintf (pf, "<font size=28> %s </font><br>\n", pLevelName);

	fprintf (pf, "<table width=%d cellpadding=0>\n", 150 * psLevel->nWidth);

	for (y = 0; y < psLevel->nHeight; y++)
	{
		fprintf (pf, "<tr>\n");

		for (x = 0; x < psLevel->nWidth; x++)
		{
			psField = get_field (x, y, nLevel);

			fprintf (pf, "<td width=100>\n");

			fprintf (pf, "<table>\n");

			{
				fprintf (pf, "<tr>\n");

				fprintf (pf, "<td></td><td width=100 bgcolor=%06X align=center>00</td><td></td>\n", convert_wall_to_HTML_color (psField->aeWalls[WALLDIR_NORTH]));

				fprintf (pf, "</tr>\n");


				fprintf (pf, "<tr>\n");

				fprintf (pf, "<td height=100 bgcolor=%06X>00</td><td align=center bgcolor=%06X>\n", convert_wall_to_HTML_color (psField->aeWalls[WALLDIR_WEST]), convert_item_to_HTML_color (psField->eItem));


				fprintf (pf, "<font size=5><b>%s</b></font>", convert_item_to_text (psField->eItem));

				
				fprintf (pf, "</td><td bgcolor=%06X>00</td>\n", convert_wall_to_HTML_color (psField->aeWalls[WALLDIR_EAST]));

				fprintf (pf, "</tr>\n");


				fprintf (pf, "<tr>\n");

				fprintf (pf, "<td></td><td bgcolor=%06X align=center>00</td><td></td>\n", convert_wall_to_HTML_color (psField->aeWalls[WALLDIR_SOUTH]));

				fprintf (pf, "</tr>\n");
			}

			fprintf (pf, "</table>\n");

			fprintf (pf, "</td>\n");
		}

		fprintf (pf, "</tr>\n");
	}

	fprintf (pf, "</table>\n");
}

void output_path (const char *pFileName, vector_pos_t *psStepList)
{
	FILE			*pf = fopen (pFileName, "wb");

	if (pf == NULL)
	{
		printf ("ERROR: output_path: Failed to open file! file name = %s\n", pFileName);

		return;
	}

	pos_t							sPos;
	uint64_t						ui64 = 0;
	field_t							*psField;
	uint32_t						u;
	vector_pos_t::const_iterator	sCIter;

	const char			*ppLevelName[] = {
											"sublevel 1", 
											"sublevel 2", 
											"sublevel 3", 
											"sublevel 4", 
											"sublevel 5", 
											"alien vessel", 
											"predator ship", 
											"airduct 1B", 
											"airduct 2B", 
											"airduct 3B", 
											"airduct 3C", 
											"airduct 4B", 
											"airduct 4C", 
											"airduct 5B", 
											"airduct 5C", 
										};

	sCIter = psStepList->cbegin ();

	for (ui64 = 0; ui64 < psStepList->size (); ui64++, sCIter++)
	{
		try
		{
			sPos = *sCIter;
		}
		catch (::std::out_of_range *e)
		{
			e;

			break;
		}
		
		psField = get_field (sPos.nPosX, sPos.nPosY, sPos.nLevel);

		switch (psField->eItem)
		{
		case	ITEM_NO_ITEM	:	psField->eItem = ITEM_STEP;

						break;

		case	ITEM_STEP	:	psField->eItem = ITEM_STEP2;

						break;

		case	ITEM_STEP2	:	psField->eItem = ITEM_STEP3;

						break;

		case	ITEM_STEP3	:	psField->eItem = ITEM_STEP4;

						break;

		case	ITEM_STEP4	:	psField->eItem = ITEM_STEP5;

						break;

		case	ITEM_STEP5	:	psField->eItem = ITEM_STEP6;

						break;

		case	ITEM_STEP6	:	psField->eItem = ITEM_STEP7;

						break;

		case	ITEM_STEP7	:	psField->eItem = ITEM_STEPM;

						break;

		default			:	

						break;
		}
	}

	fprintf (pf, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN http://www.w3.org/TR/html4/loose.dtd\"><table width=8000 cellspacing=0>\n");

	fprintf (pf, "<html>\n");

	for (u = 0; u < (sizeof (g_sLevelList) / sizeof (*g_sLevelList)); u++)
	{
		output_level (pf, &(g_sLevelList[u]), ppLevelName[u], u);
	}

	fprintf (pf, "</html>\n");

	fclose (pf);
}

int avp_path_find_core (int nTestNum, const uint32_t nNodeSize, bool bPrintStatistics)
{
	objective_t		asObjectiveList0[] =	{
											{ITEM_SHOTGUN,				OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_CLEARANCE_CARD_1,	OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_RIFLE,				OBJECTIVE_PARAMETER_OUT_OF_ORDER,	INSTRUCTION_DO_NOTHING}, 
											{ITEM_CLEARANCE_CARD_2,	OBJECTIVE_PARAMETER_OUT_OF_ORDER,		INSTRUCTION_DO_NOTHING}, 
											{ITEM_CLEARANCE_CARD_3,	OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_CLEARANCE_CARD_4,	OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_CLEARANCE_CARD_5,	OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_CLEARANCE_CARD_6,	OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_CLEARANCE_CARD_7,	OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_FLAMETHROWER,	OBJECTIVE_PARAMETER_OUT_OF_ORDER,		INSTRUCTION_DO_NOTHING}, 
											{ITEM_MOTION_TRACKER,	OBJECTIVE_PARAMETER_OUT_OF_ORDER,		INSTRUCTION_DO_NOTHING}, 
											{ITEM_CLEARANCE_CARD_8,	OBJECTIVE_PARAMETER_OUT_OF_ORDER,		INSTRUCTION_DO_NOTHING}, 
											{ITEM_CLEARANCE_CARD_9,	OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_SMARTGUN,		OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_CLEARANCE_CARD_10,	OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_SELF_DESTRUCT,		OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_ESCAPE_POD,			OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
										};


	objective_t		asObjectiveList1[] =	{
											{ITEM_SHOTGUN,				OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_RIFLE,				OBJECTIVE_PARAMETER_OUT_OF_ORDER,	INSTRUCTION_DO_NOTHING}, 
											{ITEM_CLEARANCE_CARD_6,		OBJECTIVE_PARAMETER_OUT_OF_ORDER,	INSTRUCTION_DO_NOTHING}, 
											{ITEM_MOTION_TRACKER,		OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_CLEARANCE_CARD_10,	OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_SELF_DESTRUCT,		OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_ESCAPE_POD,			OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
										};


	objective_t		asObjectiveList2[] =	{
											{ITEM_SHOTGUN,				OBJECTIVE_PARAMETER_IN_ORDER,		INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
											{ITEM_AMMO_RIFLE,			OBJECTIVE_PARAMETER_OUT_OF_ORDER,	INSTRUCTION_DO_NOTHING}, 
											{ITEM_AMMO_SHOTGUN,			OBJECTIVE_PARAMETER_OUT_OF_ORDER,	INSTRUCTION_DO_NOTHING}, 
											{ITEM_AMMO_SHOTGUN,			OBJECTIVE_PARAMETER_OUT_OF_ORDER,	INSTRUCTION_DO_NOTHING}, 
											{ITEM_RIFLE,				OBJECTIVE_PARAMETER_OUT_OF_ORDER,	INSTRUCTION_DO_NOTHING}, 
											{ITEM_CLEARANCE_CARD_6,		OBJECTIVE_PARAMETER_OUT_OF_ORDER,	INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION}, 
										};

	objective_t		*psObjectiveList = NULL;
	uint32_t		nNumObjectives = 0;

	switch (nTestNum)
	{
	case	0	:	psObjectiveList = asObjectiveList0;
				nNumObjectives = sizeof (asObjectiveList0) / sizeof (*asObjectiveList0);

				break;

	case	1	:	psObjectiveList = asObjectiveList1;
				nNumObjectives = sizeof (asObjectiveList1) / sizeof (*asObjectiveList1);

				break;

	case	2	:	psObjectiveList = asObjectiveList2;
				nNumObjectives = sizeof (asObjectiveList2) / sizeof (*asObjectiveList2);

				break;

	default		:	printf ("ERROR: AVP path find: unknown test %u\n", nTestNum);

				return (-1);

				break;
	}

#if defined (USE_STL)
#else

	CBTreeIOpropertiesRAM <>			clBTreeRAMIOproperties;
	bayerTreeCacheDescription_t			sCacheDesc;
	
#endif

	multiset_agent_t					*psOccupation;
	set_type_intrinsic_t				*psAgentList;
	multiset_agent_step_t				*psAgentStepLists;
	vector_pos_t						*psStepList;

	field_t								*psField;

#if defined (USE_STL)
#else

	sCacheDesc.nMinNumberOfBytesPerSuperBlock = 4096;

#endif

	g_bPrintStatistics = bPrintStatistics;
	
#if defined (USE_STL)

	psOccupation = new multiset_agent_t;
	
#else

	psOccupation = new multiset_agent_t (clBTreeRAMIOproperties, &sCacheDesc, nNodeSize);

#endif

	if (psOccupation == NULL)
	{
		printf ("ERROR: insufficient memory!\n");

		return (-1);
	}

#if defined (USE_STL)

	psAgentList = new set_type_intrinsic_t;
	
#else

	psAgentList = new set_type_intrinsic_t (clBTreeRAMIOproperties, &sCacheDesc, nNodeSize);

#endif

	if (psAgentList == NULL)
	{
		printf ("ERROR: insufficient memory!\n");

		return (-1);
	}

#if defined (USE_STL)

	psAgentStepLists = new multiset_agent_step_t;
	
#else

	psAgentStepLists = new multiset_agent_step_t (clBTreeRAMIOproperties, &sCacheDesc, nNodeSize);

#endif

	if (psAgentStepLists == NULL)
	{
		printf ("ERROR: insufficient memory!\n");

		return (-1);
	}

#if defined (USE_STL)

	psStepList = new vector_pos_t;
	
#else

	psStepList = new vector_pos_t (clBTreeRAMIOproperties, &sCacheDesc, nNodeSize);

#endif

	if (psStepList == NULL)
	{
		printf ("ERROR: insufficient memory!\n");

		return (-1);
	}

	psField = get_field (34, 7, 2);

	psField->eItem = ITEM_START;

	psField = get_field (18, 11, 1);

	psField->eItem = ITEM_SELF_DESTRUCT;

	psField = get_field (15, 52, 4);

	psField->eItem = ITEM_ESCAPE_POD;

	find_path (psObjectiveList, nNumObjectives, psStepList, psAgentStepLists, psAgentList, psOccupation);

	output_path ("test.htm", psStepList);

	delete psStepList;

	delete psAgentStepLists;

	delete psAgentList;

	delete psOccupation;

	return (0);
}

