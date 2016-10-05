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

#include "testbench/application_classes/regression/btreetestarray.h"
#include "testbench/application_classes/regression/btreetestkeysort.h"

#include "testbench/application_classes/regression/btreetestmultimap.h"
#include "testbench/application_classes/regression/btreetestmap.h"
#include "testbench/application_classes/regression/btreetestmultiset.h"
#include "testbench/application_classes/regression/btreetestset.h"

#include "testbench/tests/regression/btreearraytestbench.h"
#include "testbench/tests/regression/btreearraytestbench.cpp"
#include "testbench/tests/regression/btreekeysorttestbench.h"
#include "testbench/tests/regression/btreekeysorttestbench.cpp"

#include "testbench/tests/regression/btreearrayitertestbench.h"
#include "testbench/tests/regression/btreearrayitertestbench.cpp"
#include "testbench/tests/regression/btreekeysortitertestbench.h"
#include "testbench/tests/regression/btreekeysortitertestbench.cpp"

#include "testbench/tests/regression/btreemultimaptestbench.h"
#include "testbench/tests/regression/btreemultimaptestbench.cpp"
#include "testbench/tests/regression/btreemaptestbench.h"
#include "testbench/tests/regression/btreemaptestbench.cpp"
#include "testbench/tests/regression/btreemultisettestbench.h"
#include "testbench/tests/regression/btreemultisettestbench.cpp"
#include "testbench/tests/regression/btreesettestbench.h"
#include "testbench/tests/regression/btreesettestbench.cpp"

#include "testbench/tests/performance/btreearrayperftestbench.h"

#include "testbench/wrapper_classes/btreearraytestwrapper.h"
#include "testbench/wrapper_classes/btreekeysorttestwrapper.h"

#include "testbench/wrapper_classes/btreemaptestwrapper.h"
#include "testbench/wrapper_classes/btreemultimaptestwrapper.h"
#include "testbench/wrapper_classes/btreesettestwrapper.h"
#include "testbench/wrapper_classes/btreemultisettestwrapper.h"

#include "avp_path_find_core.h"

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

typedef enum
{
	APPLICATION_TYPE_NOT_SPECIFIED, 
	APPLICATION_TYPE_AVP
} application_type_e;

typedef enum
{
	SIZE_TYPE_32, 
	SIZE_TYPE_64
} size_type_e;

int main (int argc, char **argv)
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
	application_type_e		eApplicationType = APPLICATION_TYPE_NOT_SPECIFIED;
	bool					bPerformanceTest = false;
	size_type_e				eSizeType =	SIZE_TYPE_64;

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
		else if (strcmp (argv[i], "-app") == 0)
		{
			i++;

			if (strcmp (argv[i], "avp") == 0)
			{
				eApplicationType = APPLICATION_TYPE_AVP;
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
		else if (strcmp (argv[i], "-sizetype") == 0)
		{
			uint32_t	nSizeType;

			i++;

			sscanf (argv[i], "%u", &nSizeType);

			switch (nSizeType)
			{
			case	32	:	eSizeType =	SIZE_TYPE_32;

							break;

			case	64	:	eSizeType =	SIZE_TYPE_64;

							break;

			default		:	::std::cerr << "ERROR: Unknown size type specified!" << ::std::endl;

							return (-1);

							break;
			}
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

	CBTreeKeySortTestWrapper<keySortEntry_t, keySortEntry_t, uint32_t, keysort_reference_t>	*pKeySortWrapper32 = NULL;
	CBTreeKeySortTestWrapper<keySortEntry_t, keySortEntry_t, uint64_t, keysort_reference_t>	*pKeySortWrapper64 = NULL;

	CBTreeKeySortTestWrapper<keySortPair_t, keySortPair_t, uint32_t, keysort_reference_t>	*pKeySortPairWrapper32 = NULL;
	CBTreeKeySortTestWrapper<keySortPair_t, keySortPair_t, uint64_t, keysort_reference_t>	*pKeySortPairWrapper64 = NULL;

	CBTreeMapTestWrapper<mapPair_t, ::std::pair<uint32_t, mapMap_t>, uint32_t, map_reference_t>	*pMapWrapper32 = NULL;
	CBTreeMapTestWrapper<mapPair_t, ::std::pair<uint32_t, mapMap_t>, uint64_t, map_reference_t>	*pMapWrapper64 = NULL;

	CBTreeMultiMapTestWrapper<multiMapPair_t, ::std::pair<uint32_t, multiMapMap_t>, uint32_t, multimap_reference_t>	*pMultiMapWrapper32 = NULL;
	CBTreeMultiMapTestWrapper<multiMapPair_t, ::std::pair<uint32_t, multiMapMap_t>, uint64_t, multimap_reference_t>	*pMultiMapWrapper64 = NULL;

	CBTreeSetTestWrapper<uint32_t, uint32_t, uint32_t, set_reference_t>				*pSetWrapper32 = NULL;
	CBTreeSetTestWrapper<uint32_t, uint32_t, uint64_t, set_reference_t>				*pSetWrapper64 = NULL;

	CBTreeMultiSetTestWrapper<uint32_t, uint32_t, uint32_t, multiset_reference_t>	*pMultiSetWrapper32 = NULL;
	CBTreeMultiSetTestWrapper<uint32_t, uint32_t, uint64_t, multiset_reference_t>	*pMultiSetWrapper64 = NULL;

	CBTreeArrayTestWrapper<arrayEntry_t, uint32_t, ::std::list<arrayEntry_t> >		*pArrayWrapper32 = NULL;
	CBTreeArrayTestWrapper<arrayEntry_t, uint64_t, ::std::list<arrayEntry_t> >		*pArrayWrapper64 = NULL;

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
			case BTREETYPE_KEY_SORT		:	
			{
				switch (eSizeType)
				{
				case SIZE_TYPE_32	:	TestBTreeKeySort (nTestNum, nNodeSize, nPageSize, pKeySortWrapper32, pKeySortPairWrapper32);

										break;

				case SIZE_TYPE_64	:	TestBTreeKeySort (nTestNum, nNodeSize, nPageSize, pKeySortWrapper64, pKeySortPairWrapper64);

										break;

				default				:	::std::cerr << "ERROR: size type not specified or not supported!" << ::std::endl;

										return (-1);

										break;
				}
				
				break;
			}

			case BTREETYPE_ARRAY		:	
			{
				switch (eSizeType)
				{
				case SIZE_TYPE_32	:	TestBTreeArray (nTestNum, nNodeSize, nPageSize, pArrayWrapper32);

										break;

				case SIZE_TYPE_64	:	TestBTreeArray (nTestNum, nNodeSize, nPageSize, pArrayWrapper64);

										break;

				default				:	::std::cerr << "ERROR: size type not specified or not supported!" << ::std::endl;

										return (-1);

										break;
				}
				
				break;
			}

			case BTREETYPE_INDEX		:	

											break;

			case BTREETYPE_REFERENCE	:	

											break;

			case BTREETYPE_EXAMPLES		:	

											break;

			case BTREETYPE_APPLICATION	:	
				switch (eApplicationType)
				{
					case APPLICATION_TYPE_AVP	:
					{
						switch (nTestNum)
						{
						case	0	:	
						case	1	:	
						case	2	:	nRetval = avp_path_find_core (nTestNum, nNodeSize);

										break;

						default		:	cerr << "ERROR: unknown AVP application test specified! -test " << nTestNum << endl;

										break;
						}

						break;
					}

					default						:
					{
						::std::cerr << "ERROR: application type not specified or not supported!" << ::std::endl;

						return (-1);

						break;
					}
				}

				break;

			case BTREETYPE_KEY_SORT_ITERATOR	:
			{
				switch (eSizeType)
				{
				case SIZE_TYPE_32	:	TestBTreeKeySortIter<uint32_t> (nTestNum, nNodeSize, nPageSize);

										break;

				case SIZE_TYPE_64	:	TestBTreeKeySortIter<uint64_t> (nTestNum, nNodeSize, nPageSize);

										break;

				default				:	::std::cerr << "ERROR: size type not specified or not supported!" << ::std::endl;

										return (-1);

										break;
				}
				
				break;
			}

			case BTREETYPE_ARRAY_ITERATOR:	
			{
				switch (eSizeType)
				{
				case SIZE_TYPE_32	:	TestBTreeArrayIter<uint32_t> (nTestNum, nNodeSize, nPageSize);

										break;

				case SIZE_TYPE_64	:	TestBTreeArrayIter<uint64_t> (nTestNum, nNodeSize, nPageSize);

										break;

				default				:	::std::cerr << "ERROR: size type not specified or not supported!" << ::std::endl;

										return (-1);

										break;
				}
				
				break;
			}

			case BTREETYPE_STL_MULTIMAP:	
			{
				switch (eSizeType)
				{
				case SIZE_TYPE_32	:	TestBTreeSTLmultiMap (nTestNum, nNodeSize, nPageSize, pMultiMapWrapper32);

										break;

				case SIZE_TYPE_64	:	TestBTreeSTLmultiMap (nTestNum, nNodeSize, nPageSize, pMultiMapWrapper64);

										break;

				default				:	::std::cerr << "ERROR: size type not specified or not supported!" << ::std::endl;

										return (-1);

										break;
				}
				
				break;
			}

			case BTREETYPE_STL_MAP		:	
			{
				switch (eSizeType)
				{
				case SIZE_TYPE_32	:	TestBTreeSTLmap (nTestNum, nNodeSize, nPageSize, pMapWrapper32);

										break;

				case SIZE_TYPE_64	:	TestBTreeSTLmap (nTestNum, nNodeSize, nPageSize, pMapWrapper64);

										break;

				default				:	::std::cerr << "ERROR: size type not specified or not supported!" << ::std::endl;

										return (-1);

										break;
				}
				
				break;
			}

			case BTREETYPE_STL_MULTISET:
			{
				switch (eSizeType)
				{
				case SIZE_TYPE_32	:	TestBTreeSTLmultiSet (nTestNum, nNodeSize, nPageSize, pMultiSetWrapper32);

										break;

				case SIZE_TYPE_64	:	TestBTreeSTLmultiSet (nTestNum, nNodeSize, nPageSize, pMultiSetWrapper64);

										break;

				default				:	::std::cerr << "ERROR: size type not specified or not supported!" << ::std::endl;

										return (-1);

										break;
				}
				
				break;
			}

			case BTREETYPE_STL_SET:
			{
				switch (eSizeType)
				{
				case SIZE_TYPE_32	:	TestBTreeSTLset (nTestNum, nNodeSize, nPageSize, pSetWrapper32);

										break;

				case SIZE_TYPE_64	:	TestBTreeSTLset (nTestNum, nNodeSize, nPageSize, pSetWrapper64);

										break;

				default				:	::std::cerr << "ERROR: size type not specified or not supported!" << ::std::endl;

										return (-1);

										break;
				}

				break;
			}

			default						:	::std::cerr << "ERROR: btree type not specified or not supported!" << ::std::endl;

											return (-1);

											break;
			}
		}
	}
	catch (std::runtime_error *pe)
	{
		::std::cerr << ::std::endl << "ERROR caught in test bench main: ";
		::std::cerr << pe->what () << ::std::endl;
		
		nRetval = -1;
	}

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
