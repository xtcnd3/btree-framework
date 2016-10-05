
#ifndef	BTREEARRAYPERFTESTBENCH_H
#define	BTREEARRAYPERFTESTBENCH_H

#include <stdint.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

#include <list>
#include <vector>

#include "testbench/common/btreetestcommon.h"
#include "testbench/application_classes/performance/btreetestarrayperf.h"

#include "btreeioram.h"

#include "specific_data_classes/btreemultimap.h"

typedef enum
{
	BTREETEST_ARRAY_PERF_INSERT_SINGLE, 
	BTREETEST_ARRAY_PERF_INSERT_ARRAY_COPY, 
	BTREETEST_ARRAY_PERF_INSERT_ITERATOR, 
	BTREETEST_ARRAY_PERF_ERASE_SINGLE,
	BTREETEST_ARRAY_PERF_ERASE_ITERATOR,
	BTREETEST_ARRAY_PERF_ASSIGN_SET,
	BTREETEST_ARRAY_PERF_ASSIGN_ITERATOR, 
	BTREETEST_ARRAY_PERF_POP_BACK, 
	BTREETEST_ARRAY_PERF_PUSH_BACK, 
} btreetest_array_perf_e;

template<class _t_keytype, class _t_maptype, class _t_datalayerproperties = CBTreeIOpropertiesRAM <> >
class CResultList
	:	public CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>
{
public:

	typedef typename _t_datalayerproperties::size_type					size_type;
	typedef typename _t_datalayerproperties::node_iter_type				node_iter_type;
	typedef typename _t_datalayerproperties::sub_node_iter_type			sub_node_iter_type;
	typedef _t_datalayerproperties										data_layer_properties_type;
	typedef typename _t_datalayerproperties::data_layer_type			data_layer_type;

				CResultList<_t_keytype, _t_maptype, _t_datalayerproperties>
													(_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, sub_node_iter_type nNodeSize)
					:	CBTreeMultiMap<_t_keytype, _t_maptype, _t_datalayerproperties>
					(
						rDataLayerProperties, 
						psCacheDescription, 
						nNodeSize
					)
				{
				};

	virtual		~CResultList<_t_keytype, _t_maptype, _t_datalayerproperties>
													()
				{
				};
};

typedef CBTreeIOpropertiesRAM<uint64_t, uint64_t, uint32_t, uint64_t, uint32_t>	btree_ram_io_properties_66565_t;
typedef CBTreeIOpropertiesRAM<uint64_t, uint64_t, uint32_t, uint32_t, uint32_t>	btree_ram_io_properties_66555_t;
typedef CBTreeIOpropertiesRAM<uint64_t, uint32_t, uint32_t, uint32_t, uint32_t>	btree_ram_io_properties_65555_t;
typedef CBTreeIOpropertiesRAM<uint32_t, uint32_t, uint32_t, uint32_t, uint32_t>	btree_ram_io_properties_55555_t;
typedef CBTreeIOpropertiesRAM<uint32_t, uint32_t, uint16_t, uint32_t, uint32_t>	btree_ram_io_properties_55455_t;

typedef CResultList<double, const char *, btree_ram_io_properties_55555_t>		CResultList_t;

void TestArrayPerf (uint32_t nTestNum);

#endif
