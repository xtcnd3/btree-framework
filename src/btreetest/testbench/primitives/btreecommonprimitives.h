/************************************************************
**
** file:	btreecommonprimitives.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains primitive code for the b-tree framework's
** test bench to support any application class testing.
**
************************************************************/

#ifndef BTREECOMMONPRIMITIVES_H
#define BTREECOMMONPRIMITIVES_H

#include <stdint.h>

#include <iostream>

#include "testbench/primitives/btreeprimitivedefinitions.h"
#include "testbench/common/btreetestcommon.h"

template<class _t_container>
void associative_container_add_primitive (_t_container *pContainer, typename _t_container::size_type nEntries, uint32_t &nFromWhereOrSeed, btreetest_key_generation_e eGenerator)
{
	typedef typename _t_container::value_type		data_t;
	typedef typename _t_container::size_type		size_type;

	size_type		i;
	data_t			sData;

	container_data_reset (sData);

	if (eGenerator == BTREETEST_KEY_GENERATION_RANDOM)
	{
		srand (nFromWhereOrSeed);
	}

	for (i = 0; i < nEntries; i++)
	{
		::std::cout << "insert: " << i << " / " << nEntries << "\r" << ::std::flush;

		container_data_set (sData, g_nDebug, nFromWhereOrSeed, eGenerator);

		g_nDebug++;

		pContainer->insert (sData);
	}

	::std::cout << "insert: " << i << " / " << nEntries << ::std::endl;
}

template<class _t_container>
void associative_container_remove_primitive (_t_container *pContainer, typename _t_container::size_type nEntries, uint32_t nInstance, uint32_t &nFromWhereOrSeed, btreetest_key_generation_e eGenerator)
{
	typedef typename _t_container::const_iterator		citer_t;
	typedef typename _t_container::value_type			data_t;
	typedef typename _t_container::size_type			size_type;
	typedef typename _t_container::key_type				key_type;

	size_type		i;
	data_t			sData;
	uint32_t		nDebug = 0;
	key_type		nKey;

	container_data_reset (sData);
	
	if (eGenerator == BTREETEST_KEY_GENERATION_RANDOM)
	{
		srand (nFromWhereOrSeed);
	}

	for (i = 0; i < nEntries; i++)
	{
		::std::cout << "remove: " << i << " / " << nEntries << "\r" << ::std::flush;

		container_data_set (sData, nDebug, nFromWhereOrSeed, eGenerator);		

		if (nInstance == ~0x0)
		{
			nKey = get_entry_key (sData);

			pContainer->erase (nKey);
		}
		else
		{
			citer_t		sCIter;

			nKey = get_entry_key (sData);

			sCIter = pContainer->lower_bound (nKey);

			::std::advance (sCIter, (size_type) nInstance);

			if (nInstance < pContainer->count (nKey))
			{
				pContainer->erase (sCIter);
			}
			else
			{
				::std::cerr << "ERROR: associative_container_remove_primitive: instance exceeded number of available keys!" << ::std::endl;

				exit (-1);
			}
		}
	}

	::std::cout << "remove: " << i << " / " << nEntries << ::std::endl;
}

template<class _t_container>
bool associative_container_compare_primitive (_t_container *pContainer0, _t_container *pContainer1)
{
	if (*pContainer0 != *pContainer1)
	{
		::std::cerr << ::std::endl << "ERROR: copied instance mismatches reference!" << ::std::endl;
		::std::cerr << "outputting reference to comp_container0_????.html" << ::std::endl << ::std::flush;

		pContainer0->show_integrity ("comp_container0");

		::std::cerr << "outputting copied instance to comp_container1_????.html" << ::std::endl << ::std::flush;

		pContainer1->show_integrity ("comp_container1");

		exit (-1);

		return (false);
	}

	return (true);
}

#endif // BTREECOMMONPRIMITIVES_H
