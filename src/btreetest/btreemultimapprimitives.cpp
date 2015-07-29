/************************************************************
**
** file:	btreemultimapprimitives.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains primitive code for the b-tree framework's
** test bench to support multi map application class testing.
**
************************************************************/

#ifndef BTREEMULTIMAPPRIMITIVES_CPP
#define BTREEMULTIMAPPRIMITIVES_CPP

#include "btreemultimapprimitives.h"

template<class _t_obj, class _t_value>
void multiMapPrim_add (_t_obj *pClMM, uint32_t nEntries, uint32_t &nFromWhereOrSeed, btreetest_multimap_primitive_seek_e eWhere)
{
	uint32_t							nKey;
	typename _t_value::second_type		sMap = {0, 0};

	uint32_t		ui32;

	if (eWhere == BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY)
	{
		srand (nFromWhereOrSeed);
	}

	for (ui32 = 0; ui32 < nEntries; ui32++)
	{
		cout << "insert: " << ui32 << " / " << nEntries << "\r" << flush;

		sMap.nDebug = 0;

		switch (eWhere)
		{
		case BTREETEST_MULTIMAP_PRIMITIVE_DESCEND_KEY		:
			{
				nKey = nFromWhereOrSeed;

				nFromWhereOrSeed--;
				
				break;
			}

		case BTREETEST_MULTIMAP_PRIMITIVE_ASCEND_KEY		:
			{
				nKey = nFromWhereOrSeed;

				nFromWhereOrSeed++;
				
				break;
			}

		case BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY	:
			{
				nKey = generate_rand32 ();

				break;
			}

		case BTREETEST_MULTIMAP_PRIMITIVE_CONST_KEY	:
			{
				nKey = nFromWhereOrSeed;

				break;
			}

		default									:
			{
				cerr << "ERROR: multiMapPrim_add: eWhere corrupted or not set!" << endl;

				exit (-1);

				break;
			}
		}

		sMap.nData = generate_rand32 ();

		_t_value		sEntry (nKey, sMap);

		pClMM->insert (sEntry);
	}

	cout << "insert: " << ui32 << " / " << nEntries << endl;
}

#endif // BTREEMULTIMAPPRIMITIVES_CPP
