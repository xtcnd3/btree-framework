/************************************************************
**
** file:	avp_path_find_core.h
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

#ifndef	AVP_PATH_FIND_CORE_H
#define	AVP_PATH_FIND_CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <stdint.h>

#include <iterator>

#include <windows.h>

#if defined (USE_STL)

#include <set>
#include <vector>
#include <stdexcept>
#include <iostream>

#else

#include "btreeioram.h"

#include "btreearray.h"
#include "btreekeysort.h"

#endif

#define	UNBLOCKED_PATH_NW			0x001
#define	UNBLOCKED_PATH_W			0x002
#define	UNBLOCKED_PATH_SW			0x004
#define	UNBLOCKED_PATH_S			0x008
#define	UNBLOCKED_PATH_SE			0x010
#define	UNBLOCKED_PATH_E			0x020
#define	UNBLOCKED_PATH_NE			0x040
#define	UNBLOCKED_PATH_N			0x080
#define	UNBLOCKED_PORTAL_N			0x100
#define	UNBLOCKED_PORTAL_S			0x200
#define	UNBLOCKED_PORTAL_E			0x400
#define	UNBLOCKED_PORTAL_W			0x800

#define	UNBLOCKED_NUM_DIRECTIONS	8
#define	UNBLOCKED_TOTAL_DIRECTIONS	12

#if defined (VERBOSE_OUTPUT)

#define	VERBOSE_OUTPUT_PRINT(x)		::std::cout << (x) << ::std::endl

#else

#define	VERBOSE_OUTPUT_PRINT(x)

#endif

typedef enum
{
	ITEM_NO_ITEM, 
	ITEM_CLEARANCE_CARD_1, 
	ITEM_CLEARANCE_CARD_2, 
	ITEM_CLEARANCE_CARD_3, 
	ITEM_CLEARANCE_CARD_4, 
	ITEM_CLEARANCE_CARD_5, 
	ITEM_CLEARANCE_CARD_6, 
	ITEM_CLEARANCE_CARD_7, 
	ITEM_CLEARANCE_CARD_8, 
	ITEM_CLEARANCE_CARD_9, 
	ITEM_CLEARANCE_CARD_10, 
	ITEM_SHOTGUN, 
	ITEM_RIFLE, 
	ITEM_FLAMETHROWER, 
	ITEM_SMARTGUN, 
	ITEM_MOTION_TRACKER, 
	ITEM_AMMO_SHOTGUN, 
	ITEM_AMMO_RIFLE, 
	ITEM_AMMO_FLAMETHROWER, 
	ITEM_AMMO_SMARTGUN, 
	ITEM_MEDIKIT, 
	ITEM_FOOD, 
	ITEM_SELF_DESTRUCT, 
	ITEM_ESCAPE_POD, 
	ITEM_START, 
	ITEM_STEP, 
	ITEM_STEP2, 
	ITEM_STEP3, 
	ITEM_STEP4,
	ITEM_STEP5, 
	ITEM_STEP6, 
	ITEM_STEP7,  
	ITEM_STEPM
} item_e;

typedef enum
{
	WALL_NO_WALL, 
	WALL_WALL, 
	WALL_DOOR, 
	WALL_DOOR_1, 
	WALL_DOOR_2, 
	WALL_DOOR_3, 
	WALL_DOOR_4, 
	WALL_DOOR_5, 
	WALL_DOOR_6, 
	WALL_DOOR_7, 
	WALL_DOOR_8, 
	WALL_DOOR_9, 
	WALL_DOOR_10, 
	WALL_PORTAL
} wall_e;

typedef enum
{
	WALLDIR_WEST, 
	WALLDIR_NORTH, 
	WALLDIR_EAST, 
	WALLDIR_SOUTH, 
	WALLDIR_END_MARKER
} wallDir_e;

typedef enum
{
	OBJECTIVE_PARAMETER_IN_ORDER, 
	OBJECTIVE_PARAMETER_OUT_OF_ORDER, 
} objective_parameter_e;

typedef enum
{
	INSTRUCTION_DO_NOTHING, 
	INSTRUCTION_DESTROY_AGENTS_ON_COMPLETION
} instruction_e;

typedef struct
{
	wall_e			aeWalls[WALLDIR_END_MARKER];
	uint32_t	anPortalIDs[WALLDIR_END_MARKER];
	item_e			eItem;
} field_t;

typedef struct
{
	uint32_t	nWidth;
	uint32_t	nHeight;

	field_t			*psFieldList;
} level_t;

typedef struct
{
	uint32_t		nPosX;
	uint32_t		nPosY;
	uint32_t		nLevel;

	uint32_t		nObjective;
	uint32_t		nOOOvector;		// out of order vector
} agent_t;

typedef struct
{
	uint32_t		nPosX;
	uint32_t		nPosY;
	uint32_t		nLevel;
} pos_t;

typedef struct
{
	item_e					eItem;
	objective_parameter_e	eObjectiveParameter;
	instruction_e			eInstruction;
} objective_t;

typedef struct
{
	uint32_t		nAgent;
	agent_t				sAgent;

	operator const uint32_t & ()
	{
		return (nAgent);
	}

} agent_step_t;

typedef struct
{
	uint32_t			nID;
	LARGE_INTEGER		nStart;
	uint64_t			nComplexity;
} time_statistic_t;

#if defined (USE_STL)

struct agent_compare {
	bool operator() (const uint32_t& x, const uint32_t& y) const {return x < y;}
	typedef uint32_t first_argument_type;
	typedef uint32_t second_argument_type;
	typedef bool result_type;
};

struct agent_step_compare {
	bool operator() (const agent_step_t& x, const agent_step_t& y) const {return x.nAgent < y.nAgent;}
	typedef agent_step_t first_argument_type;
	typedef agent_step_t second_argument_type;
	typedef bool result_type;
};

struct occupation_compare {
	bool operator() (const agent_t& x, const agent_t& y) const
	{
		if (x.nPosX == y.nPosX)
		{
			if (x.nPosY == y.nPosY)
			{
				if (x.nLevel == y.nLevel)
				{
					if (x.nObjective == y.nObjective)
					{
						return x.nOOOvector < y.nOOOvector;
					}
					else
					{
						return x.nObjective < y.nObjective;
					}
				}
				else
				{
					return x.nLevel < y.nLevel;
				}
			}
			else
			{
				return x.nPosY < y.nPosY;
			}
		}
		else
		{
			return x.nPosX < y.nPosX;
		}
	}
	
	typedef agent_t first_argument_type;
	typedef agent_t second_argument_type;
	typedef bool result_type;
};

typedef ::std::set<uint32_t>								set_type_intrinsic_t;
typedef ::std::multiset<agent_step_t, agent_step_compare>	multiset_agent_step_t;
typedef ::std::vector<pos_t>								vector_pos_t;
typedef ::std::multiset<agent_t, occupation_compare>		multiset_agent_t;

#else

template <class _t_data, class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter> >
class CBayerTree_intrinsic : public CBTreeKeySort<_t_data, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:
	CBayerTree_intrinsic<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
		(_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nodeSize);

protected:
	int comp (void *pkey0, void *pkey1) const
	{
		_t_data		*ptkey0 = (_t_data *) pkey0;
		_t_data		*ptkey1 = (_t_data *) pkey1;

		if (*ptkey0 == *ptkey1)
		{
			return (0);
		}
		else if (*ptkey0 < *ptkey1)
		{
			return (-1);
		}
		else
		{
			return (1);
		}
	}
};

template <class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBayerTree_intrinsic<_t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBayerTree_intrinsic (_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nodeSize)
	:	CBTreeKeySort <_t_data, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
		(rDataLayerProperties, psCacheDescription, nodeSize)
{
}

template <class _t_sizetype = uint64_t, class _t_nodeiter = uint64_t, class _t_subnodeiter = uint32_t, class _t_datalayerproperties = CBTreeIOpropertiesRAM, class _t_datalayer = CBTreeRAMIO <_t_nodeiter, _t_subnodeiter> >
class CBTreeKeySort_AgentStep : public CBTreeKeySort<agent_step_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
{
public:
	CBTreeKeySort_AgentStep<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
		(_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nodeSize);

	CBTreeKeySort_AgentStep<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
		(CBTreeKeySort_AgentStep<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign = true);

protected:
};

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySort_AgentStep<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySort_AgentStep (_t_datalayerproperties &rDataLayerProperties, bayerTreeCacheDescription_t *psCacheDescription, uint32_t nodeSize)
	:	CBTreeKeySort <agent_step_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
		(rDataLayerProperties, psCacheDescription, nodeSize)
{
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeKeySort_AgentStep<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeKeySort_AgentStep
	(CBTreeKeySort_AgentStep<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT, bool bAssign)
	:	CBTreeKeySort <agent_step_t, uint32_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
		(rBT, bAssign)
{
}

typedef CBayerTree_intrinsic<uint32_t>			set_type_intrinsic_t;
typedef CBTreeKeySort_AgentStep<>				multiset_agent_step_t;
typedef CBTreeArray<pos_t>						vector_pos_t;
typedef CBTreeKeySort<agent_t>					multiset_agent_t;

#endif

void create_agent (uint32_t	*pnNextAgentID, set_type_intrinsic_t *psAgentList, uint32_t nOffSpringAgent, multiset_agent_step_t *psAgentStepLists, agent_t *psProperties);
field_t *get_field (uint32_t nPosX, uint32_t nPosY, uint32_t nLevel);
bool get_door_clearance (uint32_t nItemVector, wall_e eWall);
bool get_adjacent_path_unblocked (uint32_t nItemVector, wall_e eWall);
uint32_t get_unblocked_paths (agent_step_t sAgentStep, uint32_t nItemVector);
void remove_agent (uint32_t nAgent, set_type_intrinsic_t *psAgentList, multiset_agent_step_t *psAgentStepLists);
item_e get_item (agent_t sAgent);
bool agent_was_here_before (uint32_t nAgent, multiset_agent_step_t *psAgentStepLists);
void find_counter_portal (uint32_t nID, uint32_t nLevel, uint32_t *pnPosX, uint32_t *pnPosY, uint32_t *pnLevel);
void find_path (objective_t *psObjectiveList, uint32_t nNumObjectives, vector_pos_t *psStepList, multiset_agent_step_t *psAgentStepLists, set_type_intrinsic_t *psAgentList, multiset_agent_t *psOccupation);
uint32_t convert_wall_to_HTML_color (wall_e eWall);
const char *convert_item_to_text (item_e eItem);
uint32_t convert_item_to_HTML_color (item_e eItem);
void output_level (FILE *pf, level_t *psLevel, const char *pLevelName, uint32_t nLevel);
void output_path (const char *pFileName, vector_pos_t *psStepList);
int avp_path_find_core (int nTestNum, bool bStoreInRAM, const uint32_t nNodeSize, bool bPrintStatistics = false);

#endif // AVP_PATH_FIND_CORE_H
