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

#include "CFileMem.h"

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
	int						nRetval = 0;
	int						i;
	uint32_t				nTestNum = ~0x0;
	uint32_t				nNodeSize = 2;
	bool					bStoreInRAM = false;
	btreeType_e				eBtreeType = BTREETYPE_NOT_SPECIFIED;
	CBTreeIOpropertiesRAM	sDataPropertiesRAM;

#if defined (_DEBUG)
	CBTreeIOpropertiesFile	sDataPropertiesFile ("./", 1, 512);
#else
	CBTreeIOpropertiesFile	sDataPropertiesFile ("./", 17, 4096);
#endif

	bayerTreeCacheDescription_t		asBTreeMultiCacheDesc[] = 
	{
		{512, 1, 1, 1}, 
		{512, 1, 1, 1}, 
		{512, 7, 1, 1}, 
		{512, 1, 1, 4}, 
		{512, 1, 5, 1}, 
		{2048, 1, 1, 1}, 
		{8192, 5, 2, 8}, 
		{4096, 3, 3, 5}, 
		{512, 7, 4, 2}, 
		{1024, 1, 7, 4}, 
		{16384, 4, 5, 9}, 
		{2048, 6, 1, 2}
	};

	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize0 ("./", 5, 512);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize1 ("./", 1, 16384);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize2 ("./", 1, 512);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize3 ("./", 1, 512);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize4 ("./", 1, 512);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize5 ("./", 1, 512);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize6 ("./", 5, 2048);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize7 ("./", 2, 16384);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize8 ("./", 3, 8192);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize9 ("./", 4, 512);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize10 ("./", 8, 1024);
	CBTreeIOpropertiesFile	sDataPropertiesFileMultiCachesize11 ("./", 7, 512);

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

	CBTreeArrayDataIf<arrayEntry_t>				*apClArraysPrim[6];
	CBTreeKeySortDataIf<keySortEntry_t, uint32_t>	*apClKeySortsPrim[6];

	uint32_t								nNumMultiTemplateParams = sizeof (apClArraysPrim) / sizeof (*apClArraysPrim);

	bayerTreeCacheDescription_t				sCacheDesc;

#if defined (_DEBUG)

	sCacheDesc.nMinNumberOfBytesPerSuperBlock = 512;
	sCacheDesc.nNodeDescriptorNumOfLog2Lines = 1;
	sCacheDesc.nDataCacheNumOfLog2Lines = 1;
	sCacheDesc.nSubNodeCacheNumOfLog2Lines = 1;
	sCacheDesc.nSerVectorCacheNumOfLog2Lines = 1;

#else

	sCacheDesc.nMinNumberOfBytesPerSuperBlock = 4096;
	sCacheDesc.nNodeDescriptorNumOfLog2Lines = 10;
	sCacheDesc.nDataCacheNumOfLog2Lines = 12;
	sCacheDesc.nSubNodeCacheNumOfLog2Lines = 12;
	sCacheDesc.nSerVectorCacheNumOfLog2Lines = 12;

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
	
		apClArraysPrim[0] = dynamic_cast <CBTreeArrayDataIf<arrayEntry_t> *> (pClArray6555);
		apClArraysPrim[1] = dynamic_cast <CBTreeArrayDataIf<arrayEntry_t> *> (pClArray5555);
		apClArraysPrim[2] = dynamic_cast <CBTreeArrayDataIf<arrayEntry_t> *> (pClArray5554);
		apClArraysPrim[3] = dynamic_cast <CBTreeArrayDataIf<arrayEntry_t> *> (pClArray5454);
		apClArraysPrim[4] = dynamic_cast <CBTreeArrayDataIf<arrayEntry_t> *> (pClArray5444);
		apClArraysPrim[5] = dynamic_cast <CBTreeArrayDataIf<arrayEntry_t> *> (pClArray4444);

		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint64_t, uint32_t, uint32_t, uint32_t> >	*pClKeySort6555;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint32_t> >	*pClKeySort5555;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint16_t> >	*pClKeySort5554;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint32_t, uint16_t> >	*pClKeySort5454;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint16_t, uint16_t> >	*pClKeySort5444;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint16_t, uint16_t, uint16_t, uint16_t> >	*pClKeySort4444;
	
		pClKeySort6555 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint64_t, uint32_t, uint32_t, uint32_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));
		pClKeySort5555 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint32_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));
		pClKeySort5554 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint32_t, uint32_t, uint16_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));
		pClKeySort5454 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint32_t, uint16_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));
		pClKeySort5444 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint32_t, uint16_t, uint16_t, uint16_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));
		pClKeySort4444 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <uint16_t, uint16_t, uint16_t, uint16_t> > (sDataPropertiesRAM, &sCacheDesc, nNodeSize, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));

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
	
		apClKeySortsPrim[0] = dynamic_cast <CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *> (pClKeySort6555);
		apClKeySortsPrim[1] = dynamic_cast <CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *> (pClKeySort5555);
		apClKeySortsPrim[2] = dynamic_cast <CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *> (pClKeySort5554);
		apClKeySortsPrim[3] = dynamic_cast <CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *> (pClKeySort5454);
		apClKeySortsPrim[4] = dynamic_cast <CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *> (pClKeySort5444);
		apClKeySortsPrim[5] = dynamic_cast <CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *> (pClKeySort4444);
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
	
		apClArraysPrim[0] = dynamic_cast <CBTreeArrayDataIf<arrayEntry_t> *> (pClArray6555);
		apClArraysPrim[1] = dynamic_cast <CBTreeArrayDataIf<arrayEntry_t> *> (pClArray5555);
		apClArraysPrim[2] = dynamic_cast <CBTreeArrayDataIf<arrayEntry_t> *> (pClArray5554);
		apClArraysPrim[3] = dynamic_cast <CBTreeArrayDataIf<arrayEntry_t> *> (pClArray5454);
		apClArraysPrim[4] = dynamic_cast <CBTreeArrayDataIf<arrayEntry_t> *> (pClArray5444);
		apClArraysPrim[5] = dynamic_cast <CBTreeArrayDataIf<arrayEntry_t> *> (pClArray4444);

		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint64_t, uint32_t, uint32_t, uint32_t> >	*pClKeySort6555;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint32_t> >	*pClKeySort5555;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint16_t> >	*pClKeySort5554;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint32_t, uint16_t> >	*pClKeySort5454;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint16_t, uint16_t> >	*pClKeySort5444;
		CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint16_t, uint16_t, uint16_t, uint16_t> >	*pClKeySort4444;
	
		pClKeySort6555 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint64_t, uint32_t, uint32_t, uint32_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));
		pClKeySort5555 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint32_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));
		pClKeySort5554 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint32_t, uint32_t, uint16_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));
		pClKeySort5454 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint32_t, uint16_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));
		pClKeySort5444 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint32_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint32_t, uint16_t, uint16_t, uint16_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));
		pClKeySort4444 = new CBTreeKeySortTest<keySortEntry_t, uint32_t, uint64_t, uint16_t, uint16_t, CBTreeIOpropertiesFile, CBTreeFileIO <uint16_t, uint16_t, uint16_t, uint16_t> > (sDataPropertiesFile, &sCacheDesc, nNodeSize, OFFSETOF (keySortEntry_t, nKey), SIZEOF (keySortEntry_t, nKey));

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
	
		apClKeySortsPrim[0] = dynamic_cast <CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *> (pClKeySort6555);
		apClKeySortsPrim[1] = dynamic_cast <CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *> (pClKeySort5555);
		apClKeySortsPrim[2] = dynamic_cast <CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *> (pClKeySort5554);
		apClKeySortsPrim[3] = dynamic_cast <CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *> (pClKeySort5454);
		apClKeySortsPrim[4] = dynamic_cast <CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *> (pClKeySort5444);
		apClKeySortsPrim[5] = dynamic_cast <CBTreeKeySortDataIf<keySortEntry_t, uint32_t> *> (pClKeySort4444);
	}

	try
	{
		switch (eBtreeType)
		{
		case BTREETYPE_KEY_SORT		:	if (bStoreInRAM)
										{
											TestBTreeKeySort<CBTreeKeySortTest <keySortEntry_t, uint32_t>, CBTreeKeySortDataIf<keySortEntry_t, uint32_t>, CBTreeIOpropertiesRAM, CBTreeRAMIO <> >
												(nTestNum, nNodeSize, sDataPropertiesRAM, sCacheDesc, nNumMultiCacheSizes, asBTreeMultiCacheDesc, nNumMultiCacheSizes, apDataPropertiesRAMmultiCachesize, nNumMultiTemplateParams, apClKeySortsPrim, argc, argv);
										}
										else
										{
											TestBTreeKeySort<CBTreeKeySortTest <keySortEntry_t, uint32_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO<uint64_t, uint32_t> >, CBTreeKeySortDataIf<keySortEntry_t, uint32_t>, CBTreeIOpropertiesFile, CBTreeFileIO <> >
												(nTestNum, nNodeSize, sDataPropertiesFile, sCacheDesc, nNumMultiCacheSizes, asBTreeMultiCacheDesc, nNumMultiCacheSizes, apDataPropertiesFileMultiCachesize, nNumMultiTemplateParams, apClKeySortsPrim, argc, argv);
										}

										break;

		case BTREETYPE_ARRAY		:	if (bStoreInRAM)
										{
											TestBTreeArray<CBTreeArrayTest <>, CBTreeArrayDataIf<arrayEntry_t>, CBTreeIOpropertiesRAM, CBTreeRAMIO <> >
												(nTestNum, nNodeSize, sDataPropertiesRAM, sCacheDesc, nNumMultiCacheSizes, asBTreeMultiCacheDesc, nNumMultiCacheSizes, apDataPropertiesRAMmultiCachesize, nNumMultiTemplateParams, apClArraysPrim, argc, argv);
										}
										else
										{
											TestBTreeArray<CBTreeArrayTest<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO<uint64_t, uint32_t> >, CBTreeArrayDataIf<arrayEntry_t>, CBTreeIOpropertiesFile, CBTreeFileIO <> >
												(nTestNum, nNodeSize, sDataPropertiesFile, sCacheDesc, nNumMultiCacheSizes, asBTreeMultiCacheDesc, nNumMultiCacheSizes, apDataPropertiesFileMultiCachesize, nNumMultiTemplateParams, apClArraysPrim, argc, argv);
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
											TestBTreeKeySortIter<CBTreeKeySortTest <keySortEntry_t, uint32_t>, CBTreeKeySortDataIf<keySortEntry_t, uint32_t>, keySortEntry_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <> >
												(nTestNum, nNodeSize, sDataPropertiesRAM, sCacheDesc, argc, argv);
										}
										else
										{
											TestBTreeKeySortIter<CBTreeKeySortTest <keySortEntry_t, uint32_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO<uint64_t, uint32_t> >, CBTreeKeySortDataIf<keySortEntry_t, uint32_t>, keySortEntry_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <> >
												(nTestNum, nNodeSize, sDataPropertiesFile, sCacheDesc, argc, argv);
										}	

										break;

		case BTREETYPE_ARRAY_ITERATOR:	if (bStoreInRAM)
										{
											TestBTreeArrayIter<CBTreeArrayTest <>, CBTreeArrayDataIf<arrayEntry_t>, arrayEntry_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <> >
												(nTestNum, nNodeSize, sDataPropertiesRAM, sCacheDesc, argc, argv);
										}
										else
										{
											TestBTreeArrayIter<CBTreeArrayTest<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO<uint64_t, uint32_t> >, CBTreeArrayDataIf<arrayEntry_t>, arrayEntry_t, uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <> >
												(nTestNum, nNodeSize, sDataPropertiesFile, sCacheDesc, argc, argv);
										}	

										break;

		case BTREETYPE_STL_MULTIMAP	:	if (bStoreInRAM)
										{
											TestBTreeSTLmultiMap<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <> >
												(nTestNum, nNodeSize, sDataPropertiesRAM, sCacheDesc, argc, argv);
										}
										else
										{
											TestBTreeSTLmultiMap<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <> >
												(nTestNum, nNodeSize, sDataPropertiesFile, sCacheDesc, argc, argv);
										}	

										break;

		case BTREETYPE_STL_MAP		:	if (bStoreInRAM)
										{
											TestBTreeSTLmap<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesRAM, CBTreeRAMIO <> >
												(nTestNum, nNodeSize, sDataPropertiesRAM, sCacheDesc, argc, argv);
										}
										else
										{
											TestBTreeSTLmap<uint64_t, uint64_t, uint32_t, CBTreeIOpropertiesFile, CBTreeFileIO <> >
												(nTestNum, nNodeSize, sDataPropertiesFile, sCacheDesc, argc, argv);
										}	

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

	return (nRetval);
}
