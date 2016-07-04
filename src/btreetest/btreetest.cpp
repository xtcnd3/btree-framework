/************************************************************
**
** file:	btreetest.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** exercising functional, code coverage and regression tests
** for any part of said framework there is.
**
************************************************************/

#include <stdio.h>

#if defined (_MSC_VER)
 #if defined (_HAS_MFC)
  #include <afxtempl.h>
 #else
  #include <list>
 #endif

#include "conio.h"

#elif defined(__GNUC__) || defined(__GNUG__)
 #include <vector>
#endif

#include <string.h>
#include <time.h>
#include <stdlib.h>

#include <string>

#include <stdint.h>

#include "btreeioram.h"
#include "btreeiofile.h"

#include "btreearray.h"

#include "btreetestarray.h"
#include "btreetestkeysort.h"

#include "btreetestmultimap.h"
#include "btreetestmap.h"
#include "btreetestmultiset.h"
#include "btreetestset.h"

#include "btreearraytestbench.h"
#include "btreearraytestbench.cpp"
#include "btreekeysorttestbench.h"
#include "btreekeysorttestbench.cpp"

#include "btreearrayitertestbench.h"
#include "btreearrayitertestbench.cpp"
#include "btreekeysortitertestbench.h"
#include "btreekeysortitertestbench.cpp"

#include "btreemultimaptestbench.h"
#include "btreemultimaptestbench.cpp"
#include "btreemaptestbench.h"
#include "btreemaptestbench.cpp"
#include "btreemultisettestbench.h"
#include "btreemultisettestbench.cpp"
#include "btreesettestbench.h"
#include "btreesettestbench.cpp"

#include "btreekeysorttestwrapper.h"
#include "btreemaptestwrapper.h"
#include "btreemultimaptestwrapper.h"
#include "btreesettestwrapper.h"
#include "btreemultisettestwrapper.h"

#include "btreearraytestwrapper.h"

#include "avp_path_find_core.h"

#include "btreearrayperftestbench.h"

#if defined (_MSC_VER)
 #include "stdafx.h"
#endif

using namespace std;

/*

btreetest	-type <name> -test <n> [-nodesize <n>]

-type <name>	- specifies btree type to be tested
					valid names are:
					+ array
					+ keysort

-test <n>		- specifies number of test to be executed

-nodesize <n>	- specifies parameter t of btree instance

*/

typedef enum
{
	BTREETYPE_NOT_SPECIFIED, 
	BTREETYPE_KEY_SORT, 
	BTREETYPE_ARRAY, 
	BTREETYPE_INDEX,
	BTREETYPE_REFERENCE, 
	BTREETYPE_EXAMPLES, 
	BTREETYPE_APPLICATION, 
	BTREETYPE_KEY_SORT_ITERATOR, 
	BTREETYPE_ARRAY_ITERATOR, 
	BTREETYPE_STL_MULTIMAP, 
	BTREETYPE_STL_MAP, 
	BTREETYPE_STL_MULTISET, 
	BTREETYPE_STL_SET
} btreeType_e;

int main(int argc, char **argv)
{
#if defined(__GNUC__) || defined(__GNUG__)
	// this variable is for gdb to test when errno gets modified, since errno as such doesn't exist anymore -> usage: watch *pErrno
	int 					*pErrno = __errno_location ();

#endif

	int						nRetval = 0;
	int						i;
	uint32_t				nTestNum = ~0x0;
	uint32_t				nNodeSize = 2;
	btreeType_e				eBtreeType = BTREETYPE_NOT_SPECIFIED;
	bool					bPerformanceTest = false;

#if defined (_MSC_VER)

	SYSTEM_INFO				systemInfo;

	GetSystemInfo (&systemInfo);

	uint32_t				nPageSize = systemInfo.dwAllocationGranularity;

#elif defined(__GNUC__) || defined(__GNUG__)

	uint32_t				nPageSize = (uint64_t) getpagesize ();

#endif

	bayerTreeCacheDescription_t				sCacheDesc;

#if defined (_DEBUG)

	sCacheDesc.nMinNumberOfBytesPerSuperBlock = nPageSize;

#else

	sCacheDesc.nMinNumberOfBytesPerSuperBlock = nPageSize * 16;

#endif

	srand (0);

	for (i = 1; i < argc; i++)
	{
		if (strcmp (argv[i], "-type") == 0)
		{
			i++;

			if (strcmp (argv[i], "array") == 0)
			{
				eBtreeType = BTREETYPE_ARRAY;
			}
			else if (strcmp (argv[i], "keysort") == 0)
			{
				eBtreeType = BTREETYPE_KEY_SORT;
			}
			else if (strcmp (argv[i], "index") == 0)
			{
				eBtreeType = BTREETYPE_INDEX;
			}
			else if (strcmp (argv[i], "reference") == 0)
			{
				eBtreeType = BTREETYPE_REFERENCE;
			}
			else if (strcmp (argv[i], "app") == 0)
			{
				eBtreeType = BTREETYPE_APPLICATION;
			}
			else if (strcmp (argv[i], "keysort-iter") == 0)
			{
				eBtreeType = BTREETYPE_KEY_SORT_ITERATOR;
			}
			else if (strcmp (argv[i], "array-iter") == 0)
			{
				eBtreeType = BTREETYPE_ARRAY_ITERATOR;
			}
			else if (strcmp (argv[i], "stl-multimap") == 0)
			{
				eBtreeType = BTREETYPE_STL_MULTIMAP;
			}
			else if (strcmp (argv[i], "stl-map") == 0)
			{
				eBtreeType = BTREETYPE_STL_MAP;
			}
			else if (strcmp (argv[i], "stl-multiset") == 0)
			{
				eBtreeType = BTREETYPE_STL_MULTISET;
			}
			else if (strcmp (argv[i], "stl-set") == 0)
			{
				eBtreeType = BTREETYPE_STL_SET;
			}
			else
			{
				cerr << "ERROR: Unknown btree type specified!" << endl;

				return (-1);
			}
		}
		else if (strcmp (argv[i], "-test") == 0)
		{
			i++;

			sscanf (argv[i], "%u", &nTestNum);
		}
		else if (strcmp (argv[i], "-nodesize") == 0)
		{
			i++;

			sscanf (argv[i], "%u", &nNodeSize);
		}
		else if (strcmp (argv[i], "-performance") == 0)
		{
			bPerformanceTest = true;
		}
		else
		{
			break;
		}
	}

	CBTreeKeySortTestWrapper<keySortEntry_t, keySortEntry_t, keysort_reference_t>	*pKeySortWrapper = new CBTreeKeySortTestWrapper<keySortEntry_t, keySortEntry_t, keysort_reference_t> (nNodeSize, nPageSize);

	if (pKeySortWrapper == NULL)
	{
		::std::cerr << "insufficient memory!" << ::std::endl;

		return (-1);
	}

	CBTreeKeySortTestWrapper<keySortPair_t, keySortPair_t, keysort_reference_t>	*pKeySortPairWrapper = new CBTreeKeySortTestWrapper<keySortPair_t, keySortPair_t, keysort_reference_t> (nNodeSize, nPageSize);

	if (pKeySortPairWrapper == NULL)
	{
		::std::cerr << "insufficient memory!" << ::std::endl;

		return (-1);
	}

	CBTreeMapTestWrapper<mapPair_t, ::std::pair<uint32_t, mapMap_t>, map_reference_t>	*pMapWrapper = new CBTreeMapTestWrapper<mapPair_t, ::std::pair<uint32_t, mapMap_t>, map_reference_t> (nNodeSize, nPageSize);

	if (pMapWrapper == NULL)
	{
		::std::cerr << "insufficient memory!" << ::std::endl;

		return (-1);
	}

	CBTreeMultiMapTestWrapper<multiMapPair_t, ::std::pair<uint32_t, multiMapMap_t>, multimap_reference_t>	*pMultiMapWrapper = new CBTreeMultiMapTestWrapper<multiMapPair_t, ::std::pair<uint32_t, multiMapMap_t>, multimap_reference_t> (nNodeSize, nPageSize);

	if (pMultiMapWrapper == NULL)
	{
		::std::cerr << "insufficient memory!" << ::std::endl;

		return (-1);
	}

	CBTreeSetTestWrapper<uint32_t, uint32_t, set_reference_t>	*pSetWrapper = new CBTreeSetTestWrapper<uint32_t, uint32_t, set_reference_t> (nNodeSize, nPageSize);

	if (pSetWrapper == NULL)
	{
		::std::cerr << "insufficient memory!" << ::std::endl;

		return (-1);
	}

	CBTreeMultiSetTestWrapper<uint32_t, uint32_t, multiset_reference_t>	*pMultiSetWrapper = new CBTreeMultiSetTestWrapper<uint32_t, uint32_t, multiset_reference_t> (nNodeSize, nPageSize);

	if (pMultiSetWrapper == NULL)
	{
		::std::cerr << "insufficient memory!" << ::std::endl;

		return (-1);
	}

	CBTreeArrayTestWrapper<arrayEntry_t, ::std::list<arrayEntry_t> >	*pArrayWrapper = new CBTreeArrayTestWrapper<arrayEntry_t, ::std::list<arrayEntry_t> > (nNodeSize, nPageSize);

	if (pArrayWrapper == NULL)
	{
		::std::cerr << "insufficient memory!" << ::std::endl;

		return (-1);
	}

	try
	{
		if (bPerformanceTest)
		{
			switch (eBtreeType)
			{
			case BTREETYPE_KEY_SORT		:	

											break;

			case BTREETYPE_ARRAY		:	TestArrayPerf (nTestNum);

											break;

			case BTREETYPE_INDEX		:	

											break;

			case BTREETYPE_REFERENCE	:	

											break;

			case BTREETYPE_EXAMPLES		:	

											break;

			case BTREETYPE_APPLICATION	:	

											break;

			case BTREETYPE_KEY_SORT_ITERATOR	:
											

											break;

			case BTREETYPE_ARRAY_ITERATOR:	

											break;

			case BTREETYPE_STL_MULTIMAP	:	

											break;

			case BTREETYPE_STL_MAP		:	

											break;

			case BTREETYPE_STL_MULTISET	:	

											break;

			case BTREETYPE_STL_SET		:	

											break;

			default						:	cerr << "ERROR: btree type not specified or not supported!" << endl;

											return (-1);

											break;
			}
		}
		else
		{
			switch (eBtreeType)
			{
			case BTREETYPE_KEY_SORT		:	TestBTreeKeySort (nTestNum, nNodeSize, nPageSize, pKeySortWrapper, pKeySortPairWrapper);

											break;

			case BTREETYPE_ARRAY		:	switch (nTestNum)
											{
											case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_ITER:
											case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_CITER:
											case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_RITER:
											case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_CRITER:
												{
													pArrayWrapper->skip_containers_with_limited_address_space (true);

													break;
												}
											}

											TestBTreeArray (nTestNum, nNodeSize, pArrayWrapper);

											break;

			case BTREETYPE_INDEX		:	

											break;

			case BTREETYPE_REFERENCE	:	

											break;

			case BTREETYPE_EXAMPLES		:	

											break;

			case BTREETYPE_APPLICATION	:	switch (nTestNum)
											{
											case	0	:	
											case	1	:	
											case	2	:	nRetval = avp_path_find_core (nTestNum, nNodeSize);

															break;

											default		:	cerr << "ERROR: unknown application test specified! -test " << nTestNum << endl;

															break;
											}

											break;

			case BTREETYPE_KEY_SORT_ITERATOR	:
											
											TestBTreeKeySortIter<uint64_t> (nTestNum, nNodeSize, nPageSize);

											break;

			case BTREETYPE_ARRAY_ITERATOR:	TestBTreeArrayIter<uint64_t> (nTestNum, nNodeSize, nPageSize);

											break;

			case BTREETYPE_STL_MULTIMAP:	TestBTreeSTLmultiMap (nTestNum, pMultiMapWrapper);

											break;

			case BTREETYPE_STL_MAP		:	TestBTreeSTLmap (nTestNum, pMapWrapper);

											break;

			case BTREETYPE_STL_MULTISET	:	TestBTreeSTLmultiSet (nTestNum, pMultiSetWrapper);

											break;

			case BTREETYPE_STL_SET		:	TestBTreeSTLset (nTestNum, pSetWrapper);

											break;

			default						:	cerr << "ERROR: btree type not specified or not supported!" << endl;

											return (-1);

											break;
			}
		}
	}
	catch (std::runtime_error *pe)
	{
		cerr << endl << "ERROR caught in test bench main: ";
		cerr << pe->what () << endl;
		
		nRetval = -1;
	}
	
	delete pArrayWrapper;
	delete pMultiSetWrapper;
	delete pSetWrapper;
	delete pMultiMapWrapper;
	delete pMapWrapper;
	delete pKeySortPairWrapper;
	delete pKeySortWrapper;

#if defined (_MSC_VER)

	if (GetLastError () != ERROR_SUCCESS)
	{
		::std::cerr << "main: test bench terminated with GetLastError () not ERROR_SUCCESS!" << ::std::endl;
		::std::cerr << "error code: " << GetLastError () << ::std::endl << ::std::flush;

		nRetval = -1;
	}

#elif defined(__GNUC__) || defined(__GNUG__)

	if (errno != 0)
	{
		::std::cerr << "main: test bench terminated with errno not zero!" << ::std::endl;
		::std::cerr << "error code: " << errno << ::std::endl << ::std::flush;

		nRetval = -1;
	}

#endif

	return (nRetval);
}
