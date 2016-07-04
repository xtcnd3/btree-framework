
// #ifndef	BTREEARRAYPERFTESTBENCH_CPP
// #define	BTREEARRAYPERFTESTBENCH_CPP

#include "btreearrayperftestbench.h"

template<class _t_container, class _t_type>
void TestArrayPerfInsertSingleRun (const char *pszTitle, _t_container *pContainer, _t_type &rType, uint32_t nOperation, uint64_t nProblemSize, CResultList_t *pClResultList)
{
	typedef ::std::chrono::duration<double>			duration_t;

	typedef typename _t_container::const_iterator	citer_t;

	uint64_t		i;
	uint64_t		nCnt = 0;
	double			dblTimeSpan;
	auto			sClkStart = ::std::chrono::high_resolution_clock::now ();
	citer_t			sCIter;

	do
	{
		pContainer->clear ();

		for (i = 0; i < nProblemSize; i++)
		{
			switch (nOperation)
			{
			case	0	:	sCIter = pContainer->cbegin ();

							break;

			case	1	:	sCIter = pContainer->cend ();

							break;
			
			case	2	:	sCIter = pContainer->cbegin ();

							auto nSize = pContainer->size ();

							if (nSize > 0)
							{
								nSize = generate_rand32 () % nSize;

								::std::advance (sCIter, nSize);
							}

							break;
			}

			pContainer->insert (sCIter, rType);

			nCnt++;
		}

		auto		sClkEnd = ::std::chrono::high_resolution_clock::now ();
		duration_t	sTimeSpan = ::std::chrono::duration_cast<duration_t> (sClkEnd - sClkStart);

		dblTimeSpan = sTimeSpan.count ();
	} while (dblTimeSpan < 1.0);

	double									dblIOPs = nCnt / dblTimeSpan;
	::std::pair<double, const char *>		sResult (dblIOPs, pszTitle);

	pClResultList->insert (sResult);
}

void TestArrayPerfInsertSingleSelectOperation (uint32_t nOperation, uint64_t nProblemSize)
{
	btree_ram_io_properties_66565_t	sRAMproperties66565;
	btree_ram_io_properties_66555_t	sRAMproperties66555;
	btree_ram_io_properties_65555_t	sRAMproperties65555;
	btree_ram_io_properties_55555_t	sRAMproperties55555;
	btree_ram_io_properties_55455_t	sRAMproperties55455;
	bayerTreeCacheDescription_t		sCacheDesc = {4096};
	CResultList_t					sResultList (sRAMproperties55555, &sCacheDesc, 16);
	::std::list<uint32_t>			sList32;
	::std::vector<uint32_t>			sVector32;
	CBTreeArray<uint32_t, btree_ram_io_properties_66565_t>
									sBtrArray32_66565_16 (sRAMproperties66565, &sCacheDesc, 16);
	CBTreeArray<uint32_t, btree_ram_io_properties_66565_t>
									sBtrArray32_66565_256 (sRAMproperties66565, &sCacheDesc, 256);
	CBTreeArray<uint32_t, btree_ram_io_properties_66565_t>
									sBtrArray32_66565_1024 (sRAMproperties66565, &sCacheDesc, 1024);
	
	CBTreeArray<uint32_t, btree_ram_io_properties_66555_t>
									sBtrArray32_66555_16 (sRAMproperties66555, &sCacheDesc, 16);
	CBTreeArray<uint32_t, btree_ram_io_properties_66555_t>
									sBtrArray32_66555_256 (sRAMproperties66555, &sCacheDesc, 256);
	CBTreeArray<uint32_t, btree_ram_io_properties_66555_t>
									sBtrArray32_66555_1024 (sRAMproperties66555, &sCacheDesc, 1024);
	
	CBTreeArray<uint32_t, btree_ram_io_properties_65555_t>
									sBtrArray32_65555_16 (sRAMproperties65555, &sCacheDesc, 16);
	CBTreeArray<uint32_t, btree_ram_io_properties_65555_t>
									sBtrArray32_65555_256 (sRAMproperties65555, &sCacheDesc, 256);
	CBTreeArray<uint32_t, btree_ram_io_properties_65555_t>
									sBtrArray32_65555_1024 (sRAMproperties65555, &sCacheDesc, 1024);
	
	CBTreeArray<uint32_t, btree_ram_io_properties_55555_t>
									sBtrArray32_55555_16 (sRAMproperties55555, &sCacheDesc, 16);
	CBTreeArray<uint32_t, btree_ram_io_properties_55555_t>
									sBtrArray32_55555_256 (sRAMproperties55555, &sCacheDesc, 256);
	CBTreeArray<uint32_t, btree_ram_io_properties_55555_t>
									sBtrArray32_55555_1024 (sRAMproperties55555, &sCacheDesc, 1024);
	
	CBTreeArray<uint32_t, btree_ram_io_properties_55455_t>
									sBtrArray32_55455_16 (sRAMproperties55455, &sCacheDesc, 16);
	CBTreeArray<uint32_t, btree_ram_io_properties_55455_t>
									sBtrArray32_55455_256 (sRAMproperties55455, &sCacheDesc, 256);
	CBTreeArray<uint32_t, btree_ram_io_properties_55455_t>
									sBtrArray32_55455_1024 (sRAMproperties55455, &sCacheDesc, 1024);
	
	uint32_t						sType32 = 0;

	// type: uint32_t, uint64_t, uint64_t[2], etc...
	// insertion: front, back, random
	// problem size: 100, 10000, 1000000, ...
	// node size: 16, 256, 1024, ...
	// size type: 32, 64
	// node size type: 32, 64
	// sub-node size type: 16, 32
	
	sResultList.clear ();

	TestArrayPerfInsertSingleRun ("::std::list<uint32_t>", &sList32, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("::std::vector<uint32_t>", &sVector32, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("CBTreeArray<uint32_t, 64, 64, 32, 64, 32> (16)", &sBtrArray32_66565_16, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("CBTreeArray<uint32_t, 64, 64, 32, 64, 32> (256)", &sBtrArray32_66565_256, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("CBTreeArray<uint32_t, 64, 64, 32, 64, 32> (1024)", &sBtrArray32_66565_1024, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("CBTreeArray<uint32_t, 64, 64, 32, 32, 32> (16)", &sBtrArray32_66555_16, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("CBTreeArray<uint32_t, 64, 64, 32, 32, 32> (256)", &sBtrArray32_66555_256, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("CBTreeArray<uint32_t, 64, 64, 32, 32, 32> (1024)", &sBtrArray32_66555_1024, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("CBTreeArray<uint32_t, 64, 32, 32, 32, 32> (16)", &sBtrArray32_65555_16, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("CBTreeArray<uint32_t, 64, 32, 32, 32, 32> (256)", &sBtrArray32_65555_256, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("CBTreeArray<uint32_t, 64, 32, 32, 32, 32> (1024)", &sBtrArray32_65555_1024, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("CBTreeArray<uint32_t, 32, 32, 32, 32, 32> (16)", &sBtrArray32_55555_16, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("CBTreeArray<uint32_t, 32, 32, 32, 32, 32> (256)", &sBtrArray32_55555_256, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("CBTreeArray<uint32_t, 32, 32, 32, 32, 32> (1024)", &sBtrArray32_55555_1024, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("CBTreeArray<uint32_t, 32, 32, 16, 32, 32> (16)", &sBtrArray32_55455_16, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("CBTreeArray<uint32_t, 32, 32, 16, 32, 32> (256)", &sBtrArray32_55455_256, sType32, nOperation, nProblemSize, &sResultList);
	TestArrayPerfInsertSingleRun ("CBTreeArray<uint32_t, 32, 32, 16, 32, 32> (1024)", &sBtrArray32_55455_1024, sType32, nOperation, nProblemSize, &sResultList);

	auto	sCRIter = sResultList.crbegin ();
	auto	sCRIterEnd = sResultList.crend ();

	switch (nOperation)
	{
	case	0	:	::std::cout << "front ";	break;
	case	1	:	::std::cout << "back ";		break;
	case	2	:	::std::cout << "random ";	break;
	}

	::std::cout << "problem size: " << nProblemSize << ::std::endl;

	for (; sCRIter != sCRIterEnd; sCRIter++)
	{
		::std::cout << (*sCRIter).first << " OP/s " << (*sCRIter).second << ::std::endl;
	}
}

void TestArrayPerfInsertSingle ()
{
	TestArrayPerfInsertSingleSelectOperation (0, 100000);
	TestArrayPerfInsertSingleSelectOperation (1, 100000);
	TestArrayPerfInsertSingleSelectOperation (2, 100000);
}

void TestArrayPerf (uint32_t nTestNum)
{
	::std::cout << "array performance test bench" << ::std::endl;

	switch (nTestNum)
	{
	case BTREETEST_ARRAY_PERF_INSERT_SINGLE		:

		TestArrayPerfInsertSingle ();

		break;

	case BTREETEST_ARRAY_PERF_INSERT_ARRAY_COPY	:

		break;

	case BTREETEST_ARRAY_PERF_INSERT_ITERATOR	:

		break;

	case BTREETEST_ARRAY_PERF_ERASE_SINGLE		:

		break;

	case BTREETEST_ARRAY_PERF_ERASE_ITERATOR	:

		break;

	case BTREETEST_ARRAY_PERF_ASSIGN_SET		:

		break;

	case BTREETEST_ARRAY_PERF_ASSIGN_ITERATOR	:

		break;

	case BTREETEST_ARRAY_PERF_POP_BACK			:

		break;

	case BTREETEST_ARRAY_PERF_PUSH_BACK			:

		break;

	default										:

		::std::cerr << "ERROR: TestArrayPerf: Unknown array performance test selected!" << ::std::endl;

		exit (-1);

		break;
	}
}

// #endif
