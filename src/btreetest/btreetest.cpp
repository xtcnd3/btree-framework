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

#if defined (WIN32)
 #if defined (_HAS_MFC)
  #include <afxtempl.h>
 #else
  #include <list>
 #endif

#include "conio.h"

#elif defined (LINUX)
 #include <vector>
#endif

#include <string.h>
#include <time.h>

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
//#include "btreekeysorttestbench.cpp"

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

#include "avp_path_find_core.h"

#if defined (WIN32)
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
#if defined (LINUX)
	// this variable is for gdb to test when errno gets modified, since errno as such doesn't exist anymore -> usage: watch *pErrno
	int 					*pErrno = __errno_location ();

#endif

	int						nRetval = 0;
	int						i;
	uint32_t				nTestNum = ~0x0;
	uint32_t				nNodeSize = 2;
	bool					bStoreInRAM = false;
	btreeType_e				eBtreeType = BTREETYPE_NOT_SPECIFIED;
	CBTreeIOpropertiesRAM	sDataPropertiesRAM;

#if defined (WIN32)

	SYSTEM_INFO				systemInfo;

	GetSystemInfo (&systemInfo);

	uint32_t				nPageSize = systemInfo.dwAllocationGranularity;

#elif defined (LINUX)

	uint32_t				nPageSize = (uint64_t) getpagesize ();

#endif

#if defined (_DEBUG)
	CBTreeIOpropertiesFile	sDataPropertiesFile ("./", 1);
#else
	CBTreeIOpropertiesFile	sDataPropertiesFile ("./");
#endif

	bayerTreeCacheDescription_t		asBTreeMultiCacheDesc[] = 
	{
		{1}, 
		{nPageSize - 1}, 
		{nPageSize}, 
		{nPageSize + 1}, 
		{1}, 
		{nPageSize - 1}, 
		{nPageSize}, 
		{nPageSize + 1}, 
		{1}, 
		{nPageSize - 1}, 
		{nPageSize}, 
		{nPageSize + 1}
	};

	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize0 ("./", 1);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize1 ("./", 1);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize2 ("./", 1);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize3 ("./", 1);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize4 ("./");
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize5 ("./");
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize6 ("./");
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize7 ("./");
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize8 ("./", 16777216);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize9 ("./", 16777216);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize10 ("./", 16777216);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize11 ("./", 16777216);

	CBTreeIOpropertiesFile	*apDataPropertiesFileMultiCachesize[] = 
	{
		&sDataPropertiesFileMultiCachesize0, 
		&sDataPropertiesFileMultiCachesize1, 
		&sDataPropertiesFileMultiCachesize2, 
		&sDataPropertiesFileMultiCachesize3, 
		&sDataPropertiesFileMultiCachesize4, 
		&sDataPropertiesFileMultiCachesize5, 
		&sDataPropertiesFileMultiCachesize6, 
		&sDataPropertiesFileMultiCachesize7, 
		&sDataPropertiesFileMultiCachesize8, 
		&sDataPropertiesFileMultiCachesize9, 
		&sDataPropertiesFileMultiCachesize10, 
		&sDataPropertiesFileMultiCachesize11
	};

	CBTreeIOpropertiesRAM	*apDataPropertiesRAMmultiCachesize[] = 
	{
		&sDataPropertiesRAM, 
		&sDataPropertiesRAM, 
		&sDataPropertiesRAM, 
		&sDataPropertiesRAM, 
		&sDataPropertiesRAM, 
		&sDataPropertiesRAM, 
		&sDataPropertiesRAM, 
		&sDataPropertiesRAM, 
		&sDataPropertiesRAM, 
		&sDataPropertiesRAM, 
		&sDataPropertiesRAM, 
		&sDataPropertiesRAM
	};

	uint32_t								nNumMultiCacheSizes = sizeof (asBTreeMultiCacheDesc) / sizeof (*asBTreeMultiCacheDesc);

	CBTreeArrayIf<arrayEntry_t>				*apClArraysPrim[6];
	CBTreeAssociativeIf<keySortEntry_t, uint32_t>	*apClKeySortsPrim[6];

	uint32_t								nNumMultiTemplateParams = 0;

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
		else if (strcmp (argv[i], "-RAM") == 0)
		{
			bStoreInRAM = true;
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

	if (bStoreInRAM)
	{
		CBTreeArrayTest<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint64_t, uint32_t, uint32_t, uint32_t> >	*pClArray6555;
		CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint32_t> >	*pClArray5555;
		CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint16_t> >	*pClArray5554;
		CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint32_t, uint16_t> >	*pClArray5454;
		CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint16_t, uint16_t> >	*pClArray5444;
		CBTreeArrayTest<uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint16_t, uint16_t, uint16_t, uint16_t> >	*pClArray4444;
	
		pClArray6555 = new CBTreeArrayTest<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint64_t, uint32_t, uint32_t, uint32_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize);
		pClArray5555 = new CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint32_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize);
		pClArray5554 = new CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint16_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize);
		pClArray5454 = new CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint32_t, uint16_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize);
		pClArray5444 = new CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint16_t, uint16_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize);
		pClArray4444 = new CBTreeArrayTest<uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint16_t, uint16_t, uint16_t, uint16_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize);

		if (
			(pClArray6555 == NULL) || 
			(pClArray5555 == NULL) || 
			(pClArray5554 == NULL) || 
			(pClArray5454 == NULL) || 
			(pClArray5444 == NULL) || 
			(pClArray4444 == NULL)
		)
		{
			cerr << "ERROR: insufficient memory!" << endl;

			return (-1);
		}
	
		apClArraysPrim[0] = dynamic_cast <CBTreeArrayIf<arrayEntry_t> *> (pClArray6555);
		apClArraysPrim[1] = dynamic_cast <CBTreeArrayIf<arrayEntry_t> *> (pClArray5555);
		apClArraysPrim[2] = dynamic_cast <CBTreeArrayIf<arrayEntry_t> *> (pClArray5554);
		apClArraysPrim[3] = dynamic_cast <CBTreeArrayIf<arrayEntry_t> *> (pClArray5454);
		apClArraysPrim[4] = dynamic_cast <CBTreeArrayIf<arrayEntry_t> *> (pClArray5444);
		apClArraysPrim[5] = dynamic_cast <CBTreeArrayIf<arrayEntry_t> *> (pClArray4444);

		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint64_t, uint32_t, uint32_t, uint32_t> >	*pClKeySort6555;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint32_t> >	*pClKeySort5555;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint16_t> >	*pClKeySort5554;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint32_t, uint16_t> >	*pClKeySort5454;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint16_t, uint16_t> >	*pClKeySort5444;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint16_t, uint16_t, uint16_t, uint16_t> >	*pClKeySort4444;
	
		pClKeySort6555 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint64_t, uint32_t, uint32_t, uint32_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize, NULL);
		pClKeySort5555 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint32_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize, NULL);
		pClKeySort5554 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint16_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize, NULL);
		pClKeySort5454 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint32_t, uint16_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize, NULL);
		pClKeySort5444 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint16_t, uint16_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize, NULL);
		pClKeySort4444 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint16_t, uint16_t, uint16_t, uint16_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize, NULL);

		if (
			(pClKeySort6555 == NULL) || 
			(pClKeySort5555 == NULL) || 
			(pClKeySort5554 == NULL) || 
			(pClKeySort5454 == NULL) || 
			(pClKeySort5444 == NULL) || 
			(pClKeySort4444 == NULL)
		)
		{
			cerr << "ERROR: insufficient memory!" << endl;

			return (-1);
		}
	
		apClKeySortsPrim[0] = dynamic_cast <CBTreeAssociativeIf<keySortEntry_t, uint32_t> *> (pClKeySort6555);
		apClKeySortsPrim[1] = dynamic_cast <CBTreeAssociativeIf<keySortEntry_t, uint32_t> *> (pClKeySort5555);
		apClKeySortsPrim[2] = dynamic_cast <CBTreeAssociativeIf<keySortEntry_t, uint32_t> *> (pClKeySort5554);
		apClKeySortsPrim[3] = dynamic_cast <CBTreeAssociativeIf<keySortEntry_t, uint32_t> *> (pClKeySort5454);
		apClKeySortsPrim[4] = dynamic_cast <CBTreeAssociativeIf<keySortEntry_t, uint32_t> *> (pClKeySort5444);
		apClKeySortsPrim[5] = dynamic_cast <CBTreeAssociativeIf<keySortEntry_t, uint32_t> *> (pClKeySort4444);

		nNumMultiTemplateParams = sizeof (apClArraysPrim) / sizeof (*apClArraysPrim);
	}
	else
	{
		CBTreeArrayTest<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint64_t, uint32_t, uint32_t, uint32_t> >	*pClArray6555;
		CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint32_t> >	*pClArray5555;
		CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint16_t> >	*pClArray5554;
		CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint32_t, uint16_t> >	*pClArray5454;
		CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint16_t, uint16_t> >	*pClArray5444;
		CBTreeArrayTest<uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint16_t, uint16_t, uint16_t, uint16_t> >	*pClArray4444;
	
		pClArray6555 = new CBTreeArrayTest<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint64_t, uint32_t, uint32_t, uint32_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize);
		pClArray5555 = new CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint32_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize);
		pClArray5554 = new CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint16_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize);
		pClArray5454 = new CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint32_t, uint16_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize);
		pClArray5444 = new CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint16_t, uint16_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize);
		pClArray4444 = new CBTreeArrayTest<uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint16_t, uint16_t, uint16_t, uint16_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize);

		if (
			(pClArray6555 == NULL) || 
			(pClArray5555 == NULL) || 
			(pClArray5554 == NULL) || 
			(pClArray5454 == NULL) || 
			(pClArray5444 == NULL) || 
			(pClArray4444 == NULL)
		)
		{
			cerr << "ERROR: insufficient memory!" << endl;

			return (-1);
		}
	
		apClArraysPrim[0] = dynamic_cast <CBTreeArrayIf<arrayEntry_t> *> (pClArray6555);
		apClArraysPrim[1] = dynamic_cast <CBTreeArrayIf<arrayEntry_t> *> (pClArray5555);
		apClArraysPrim[2] = dynamic_cast <CBTreeArrayIf<arrayEntry_t> *> (pClArray5554);
		apClArraysPrim[3] = dynamic_cast <CBTreeArrayIf<arrayEntry_t> *> (pClArray5454);
		apClArraysPrim[4] = dynamic_cast <CBTreeArrayIf<arrayEntry_t> *> (pClArray5444);
		apClArraysPrim[5] = dynamic_cast <CBTreeArrayIf<arrayEntry_t> *> (pClArray4444);

		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint64_t, uint32_t, uint32_t, uint32_t> >	*pClKeySort6555;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint32_t> >	*pClKeySort5555;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint16_t> >	*pClKeySort5554;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint32_t, uint16_t> >	*pClKeySort5454;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint16_t, uint16_t> >	*pClKeySort5444;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint16_t, uint16_t, uint16_t, uint16_t> >	*pClKeySort4444;
	
		pClKeySort6555 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint64_t, uint32_t, uint32_t, uint32_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize, NULL);
		pClKeySort5555 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint32_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize, NULL);
		pClKeySort5554 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint16_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize, NULL);
		pClKeySort5454 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint32_t, uint16_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize, NULL);
		pClKeySort5444 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint16_t, uint16_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize, NULL);
		pClKeySort4444 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint16_t, uint16_t, uint16_t, uint16_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize, NULL);

		if (
			(pClKeySort6555 == NULL) || 
			(pClKeySort5555 == NULL) || 
			(pClKeySort5554 == NULL) || 
			(pClKeySort5454 == NULL) || 
			(pClKeySort5444 == NULL) || 
			(pClKeySort4444 == NULL)
		)
		{
			cerr << "ERROR: insufficient memory!" << endl;

			return (-1);
		}
	
		apClKeySortsPrim[0] = dynamic_cast <CBTreeAssociativeIf<keySortEntry_t, uint32_t> *> (pClKeySort6555);
		apClKeySortsPrim[1] = dynamic_cast <CBTreeAssociativeIf<keySortEntry_t, uint32_t> *> (pClKeySort5555);
		apClKeySortsPrim[2] = dynamic_cast <CBTreeAssociativeIf<keySortEntry_t, uint32_t> *> (pClKeySort5554);
		apClKeySortsPrim[3] = dynamic_cast <CBTreeAssociativeIf<keySortEntry_t, uint32_t> *> (pClKeySort5454);
		apClKeySortsPrim[4] = dynamic_cast <CBTreeAssociativeIf<keySortEntry_t, uint32_t> *> (pClKeySort5444);
		apClKeySortsPrim[5] = dynamic_cast <CBTreeAssociativeIf<keySortEntry_t, uint32_t> *> (pClKeySort4444);

		nNumMultiTemplateParams = sizeof (apClArraysPrim) / sizeof (*apClArraysPrim);
		nNumMultiTemplateParams -= 2;
	}

	try
	{
		switch (eBtreeType)
		{
		case BTREETYPE_KEY_SORT		:	TestBTreeKeySort (nTestNum, nNodeSize, nPageSize, pKeySortWrapper, pKeySortPairWrapper);

										break;

		case BTREETYPE_ARRAY		:	if (bStoreInRAM)
										{
											TestBTreeArray<CBTreeArrayTest <>, CBTreeArrayIf<arrayEntry_t>, CBTreeIOpropertiesRAM, CBTreeRAMIO <> >
												(nTestNum, nNodeSize, sDataPropertiesRAM, sCacheDesc, nNumMultiCacheSizes, asBTreeMultiCacheDesc, nNumMultiCacheSizes, apDataPropertiesRAMmultiCachesize, nNumMultiTemplateParams, apClArraysPrim, argc, argv);
										}
										else
										{
											switch (nTestNum)
											{
													case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_ITER		:
													case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_CITER		:
													case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_RITER		:
													case BTREETEST_ARRAY_STL_IF_INSERT_VIA_ITERATOR_LARGE_CRITER	:
													{
														CBTreeIOpropertiesFile	sDataPropertiesFileLarge ("./", 16777216);
														sCacheDesc.nMinNumberOfBytesPerSuperBlock *= 32;

														TestBTreeArray<CBTreeArrayTest<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO<uint64_t, uint32_t> >, CBTreeArrayIf<arrayEntry_t>, CBTreeIOpropertiesFile, CBTreeFileIO <> >
															(nTestNum, nNodeSize, sDataPropertiesFileLarge, sCacheDesc, nNumMultiCacheSizes, asBTreeMultiCacheDesc, nNumMultiCacheSizes, apDataPropertiesFileMultiCachesize, nNumMultiTemplateParams, apClArraysPrim, argc, argv);

														break;
													}

													default:
													{
														TestBTreeArray<CBTreeArrayTest<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO<uint64_t, uint32_t> >, CBTreeArrayIf<arrayEntry_t>, CBTreeIOpropertiesFile, CBTreeFileIO <> >
															(nTestNum, nNodeSize, sDataPropertiesFile, sCacheDesc, nNumMultiCacheSizes, asBTreeMultiCacheDesc, nNumMultiCacheSizes, apDataPropertiesFileMultiCachesize, nNumMultiTemplateParams, apClArraysPrim, argc, argv);

														break;
													}
											}
										}

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
										case	2	:	nRetval = avp_path_find_core (nTestNum, bStoreInRAM, nNodeSize);

														break;

										default		:	cerr << "ERROR: unknown application test specified! -test " << nTestNum << endl;

														break;
										}

										break;

		case BTREETYPE_KEY_SORT_ITERATOR	:
										if (bStoreInRAM)
										{
											TestBTreeKeySortIter<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <> >
												(nTestNum, nNodeSize, sDataPropertiesRAM, sCacheDesc, argc, argv);
										}
										else
										{
											TestBTreeKeySortIter<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <> >
												(nTestNum, nNodeSize, sDataPropertiesFile, sCacheDesc, argc, argv);
										}	

										break;

		case BTREETYPE_ARRAY_ITERATOR:	if (bStoreInRAM)
										{
											TestBTreeArrayIter<CBTreeArrayTest <>, CBTreeArrayIf<arrayEntry_t>, arrayEntry_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <> >
												(nTestNum, nNodeSize, sDataPropertiesRAM, sCacheDesc, argc, argv);
										}
										else
										{
											TestBTreeArrayIter<CBTreeArrayTest<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO<uint64_t, uint32_t> >, CBTreeArrayIf<arrayEntry_t>, arrayEntry_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <> >
												(nTestNum, nNodeSize, sDataPropertiesFile, sCacheDesc, argc, argv);
										}	

										break;

		case BTREETYPE_STL_MULTIMAP:	TestBTreeSTLmultiMap (nTestNum, pMultiMapWrapper);

										break;

		case BTREETYPE_STL_MAP		:	TestBTreeSTLmap (nTestNum, pMapWrapper);

										break;

		case BTREETYPE_STL_MULTISET	:	if (bStoreInRAM)
										{
											TestBTreeSTLmultiSet<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <> >
												(nTestNum, nNodeSize, sDataPropertiesRAM, sCacheDesc, argc, argv);
										}
										else
										{
											TestBTreeSTLmultiSet<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <> >
												(nTestNum, nNodeSize, sDataPropertiesFile, sCacheDesc, argc, argv);
										}	

										break;

		case BTREETYPE_STL_SET		:	if (bStoreInRAM)
										{
											TestBTreeSTLset<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <> >
												(nTestNum, nNodeSize, sDataPropertiesRAM, sCacheDesc, argc, argv);
										}
										else
										{
											TestBTreeSTLset<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <> >
												(nTestNum, nNodeSize, sDataPropertiesFile, sCacheDesc, argc, argv);
										}	

										break;

		default						:	cerr << "ERROR: btree type not specified or not supported!" << endl;

										return (-1);

										break;
		}
	}
	catch (std::runtime_error *pe)
	{
		cerr << endl << "ERROR caught in test bench main: ";
		cerr << pe->what () << endl;
		
		nRetval = -1;
	}
	
	if (bStoreInRAM)
	{
		CBTreeArrayTest<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint64_t, uint32_t, uint32_t, uint32_t> >	*pClArray6555;
		CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint32_t> >	*pClArray5555;
		CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint16_t> >	*pClArray5554;
		CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint32_t, uint16_t> >	*pClArray5454;
		CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint16_t, uint16_t> >	*pClArray5444;
		CBTreeArrayTest<uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint16_t, uint16_t, uint16_t, uint16_t> >	*pClArray4444;
	
		pClArray6555 = dynamic_cast <CBTreeArrayTest<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint64_t, uint32_t, uint32_t, uint32_t> > *> (apClArraysPrim[0]);
		pClArray5555 = dynamic_cast <CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint32_t> > *> (apClArraysPrim[1]);
		pClArray5554 = dynamic_cast <CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint16_t> > *> (apClArraysPrim[2]);
		pClArray5454 = dynamic_cast <CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint32_t, uint16_t> > *> (apClArraysPrim[3]);
		pClArray5444 = dynamic_cast <CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint16_t, uint16_t> > *> (apClArraysPrim[4]);
		pClArray4444 = dynamic_cast <CBTreeArrayTest<uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint16_t, uint16_t, uint16_t, uint16_t> > *> (apClArraysPrim[5]);

		delete pClArray6555;
		delete pClArray5555;
		delete pClArray5554;
		delete pClArray5454;
		delete pClArray5444;
		delete pClArray4444;

		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint64_t, uint32_t, uint32_t, uint32_t> >	*pClKeySort6555;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint32_t> >	*pClKeySort5555;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint16_t> >	*pClKeySort5554;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint32_t, uint16_t> >	*pClKeySort5454;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint16_t, uint16_t> >	*pClKeySort5444;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint16_t, uint16_t, uint16_t, uint16_t> >	*pClKeySort4444;
	
		pClKeySort6555 = dynamic_cast <CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint64_t, uint32_t, uint32_t, uint32_t> > *> (apClKeySortsPrim[0]);
		pClKeySort5555 = dynamic_cast <CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint32_t> > *> (apClKeySortsPrim[1]);
		pClKeySort5554 = dynamic_cast <CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint16_t> > *> (apClKeySortsPrim[2]);
		pClKeySort5454 = dynamic_cast <CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint32_t, uint16_t> > *> (apClKeySortsPrim[3]);
		pClKeySort5444 = dynamic_cast <CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint16_t, uint16_t> > *> (apClKeySortsPrim[4]);
		pClKeySort4444 = dynamic_cast <CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint16_t, uint16_t, uint16_t, uint16_t> > *> (apClKeySortsPrim[5]);

		delete pClKeySort6555;
		delete pClKeySort5555;
		delete pClKeySort5554;
		delete pClKeySort5454;
		delete pClKeySort5444;
		delete pClKeySort4444;
	}
	else
	{
		CBTreeArrayTest<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint64_t, uint32_t, uint32_t, uint32_t> >	*pClArray6555;
		CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint32_t> >	*pClArray5555;
		CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint16_t> >	*pClArray5554;
		CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint32_t, uint16_t> >	*pClArray5454;
		CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint16_t, uint16_t> >	*pClArray5444;
		CBTreeArrayTest<uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint16_t, uint16_t, uint16_t, uint16_t> >	*pClArray4444;
	
		pClArray6555 = dynamic_cast <CBTreeArrayTest<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint64_t, uint32_t, uint32_t, uint32_t> > *> (apClArraysPrim[0]);
		pClArray5555 = dynamic_cast <CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint32_t> > *> (apClArraysPrim[1]);
		pClArray5554 = dynamic_cast <CBTreeArrayTest<uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint16_t> > *> (apClArraysPrim[2]);
		pClArray5454 = dynamic_cast <CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint32_t, uint16_t> > *> (apClArraysPrim[3]);
		pClArray5444 = dynamic_cast <CBTreeArrayTest<uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint16_t, uint16_t> > *> (apClArraysPrim[4]);
		pClArray4444 = dynamic_cast <CBTreeArrayTest<uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint16_t, uint16_t, uint16_t, uint16_t> > *> (apClArraysPrim[5]);

		delete pClArray6555;
		delete pClArray5555;
		delete pClArray5554;
		delete pClArray5454;
		delete pClArray5444;
		delete pClArray4444;

		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint64_t, uint32_t, uint32_t, uint32_t> >	*pClKeySort6555;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint32_t> >	*pClKeySort5555;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint16_t> >	*pClKeySort5554;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint32_t, uint16_t> >	*pClKeySort5454;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint16_t, uint16_t> >	*pClKeySort5444;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint16_t, uint16_t, uint16_t, uint16_t> >	*pClKeySort4444;
	
		pClKeySort6555 = dynamic_cast <CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint64_t, uint32_t, uint32_t, uint32_t> > *> (apClKeySortsPrim[0]);
		pClKeySort5555 = dynamic_cast <CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint32_t> > *> (apClKeySortsPrim[1]);
		pClKeySort5554 = dynamic_cast <CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint16_t> > *> (apClKeySortsPrim[2]);
		pClKeySort5454 = dynamic_cast <CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint32_t, uint16_t> > *> (apClKeySortsPrim[3]);
		pClKeySort5444 = dynamic_cast <CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint16_t, uint16_t> > *> (apClKeySortsPrim[4]);
		pClKeySort4444 = dynamic_cast <CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint16_t, uint16_t, uint16_t, uint16_t> > *> (apClKeySortsPrim[5]);

		delete pClKeySort6555;
		delete pClKeySort5555;
		delete pClKeySort5554;
		delete pClKeySort5454;
		delete pClKeySort5444;
		delete pClKeySort4444;
	}

	delete pMultiMapWrapper;
	delete pMapWrapper;
	delete pKeySortPairWrapper;
	delete pKeySortWrapper;

#if defined (WIN32)

	if (GetLastError () != ERROR_SUCCESS)
	{
		::std::cerr << "main: test bench terminated with GetLastError () not ERROR_SUCCESS!" << ::std::endl;
		::std::cerr << "error code: " << GetLastError () << ::std::endl << ::std::flush;

		nRetval = -1;
	}

#elif defined (LINUX)

	if (errno != 0)
	{
		::std::cerr << "main: test bench terminated with errno not zero!" << ::std::endl;
		::std::cerr << "error code: " << errno << ::std::endl << ::std::flush;

		nRetval = -1;
	}

#endif

	return (nRetval);
}
