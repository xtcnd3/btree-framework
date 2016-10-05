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

#include "testbench/primitives/btreemultimapprimitives.h"

template<class _t_container>
void multiMapPrim_add (_t_container *pClMM, typename _t_container::size_type nEntries, uint32_t &nFromWhereOrSeed, btreetest_multimap_primitive_seek_e eWhere)
{
	typedef typename _t_container::value_type		data_t;

	typename ::std::remove_const<typename data_t::first_type>::type		nKey;
	typename data_t::second_type										sMap = {0, 0};

	uint32_t		ui32;

	if (eWhere == BTREETEST_MULTIMAP_PRIMITIVE_RANDOM_KEY)
	{
		srand (nFromWhereOrSeed);
	}

	for (ui32 = 0; ui32 < nEntries; ui32++)
	{
		::std::cout << "insert: " << ui32 << " / " << nEntries << "\r" << ::std::flush;

		sMap.nDebug = g_nDebug;

		g_nDebug++;

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
				::std::cerr << "ERROR: multiMapPrim_add: eWhere corrupted or not set!" << ::std::endl;

				exit (-1);

				break;
			}
		}

		sMap.nData = generate_rand32 ();

		data_t			sEntry (nKey, sMap);

		pClMM->insert (sEntry);
	}

	::std::cout << "insert: " << ui32 << " / " << nEntries << ::std::endl;
}

#endif // BTREEMULTIMAPPRIMITIVES_CPP
