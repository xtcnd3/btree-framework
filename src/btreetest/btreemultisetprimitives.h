/************************************************************
**
** file:	btreemultisetprimitives.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains primitive code for the b-tree framework's
** test bench to support multi set application class testing.
**
************************************************************/

#ifndef BTREEMULTISETPRIMITIVES_H
#define	BTREEMULTISETPRIMITIVES_H

#include <stdint.h>

#include "btreetestcommon.h"
#include "./associative/btreemultiset.h"

typedef enum
{
	BTREETEST_MULTISET_PRIMITIVE_DESCEND_KEY, 
	BTREETEST_MULTISET_PRIMITIVE_ASCEND_KEY, 
	BTREETEST_MULTISET_PRIMITIVE_RANDOM_KEY, 
	BTREETEST_MULTISET_PRIMITIVE_CONST_KEY
} btreetest_multiset_primitive_seek_e;

template<class _t_container>
void multiSetPrim_add (_t_container *pClMS, uint32_t nEntries, uint32_t &nFromWhereOrSeed, btreetest_multiset_primitive_seek_e eWhere);

template<class _t_container>
void multiSetPrim_add (_t_container *pClMS, uint32_t nEntries, uint32_t &nFromWhereOrSeed, btreetest_multiset_primitive_seek_e eWhere)
{
	typedef typename _t_container::value_type		value_type;

	value_type							sSet = 0;

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

#endif // !BTREEMULTIMAPPRIMITIVES_H

//#include "btreemultisetprimitives.cpp"
