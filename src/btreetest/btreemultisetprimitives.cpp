/************************************************************
**
** file:	btreemultisetprimitives.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains primitive code for the b-tree framework's
** test bench to support multi set application class testing.
**
************************************************************/

#ifndef BTREEMULTISETPRIMITIVES_CPP
#define BTREEMULTISETPRIMITIVES_CPP

#include "btreemultisetprimitives.h"

template<class _t_obj, class _t_value>
void multiSetPrim_add (_t_obj *pClMS, uint32_t nEntries, uint32_t &nFromWhereOrSeed, btreetest_multiset_primitive_seek_e eWhere)
{
	_t_value							sSet = 0;

	uint32_t		ui32;

	if (eWhere == BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY)
	{
		srand (nFromWhereOrSeed);
	}

	for (ui32 = 0; ui32 < nEntries; ui32++)
	{
		cout << "insert: " << ui32 << " / " << nEntries << "\r" << flush;

		switch (eWhere)
		{
		case BTREETEST_MULTISET_PRIMITIVE_DESCEND_KEY		:
			{
				sSet = nFromWhereOrSeed;

				nFromWhereOrSeed--;
				
				break;
			}

		case BTREETEST_MULTISET_PRIMITIVE_ASCEND_KEY		:
			{
				sSet = nFromWhereOrSeed;

				nFromWhereOrSeed++;
				
				break;
			}

		case BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY	:
			{
				sSet = generate_rand32 ();

				break;
			}

		case BTREETEST_MULTISET_PRIMITIVE_CONST_KEY	:
			{
				sSet = nFromWhereOrSeed;

				break;
			}

		default									:
			{
				cerr << "ERROR: multiSetPrim_add: eWhere corrupted or not set!" << endl;

				exit (-1);

				break;
			}
		}

		pClMS->insert (sSet);
	}

	cout << "insert: " << ui32 << " / " << nEntries << endl;
}

#endif // BTREEMULTISETPRIMITIVES_CPP
