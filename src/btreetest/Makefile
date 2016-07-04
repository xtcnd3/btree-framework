GPP=g++
COV=lcov
GEN=genhtml
VALGRIND=/usr/bin/valgrind
NICE=nice

PROGREL = ./btreetest
PROGDBG = ./btreetest_dbg
PROGCOV = ./btreetest_cov

COVERAGE_BASE = coverage.base
COVERAGE_OUT = coverage.out
COVERAGE_COMB = coverage.combined
COVERAGE_REPORT = coverage_report

VALGRIND_REPORT = valgrind_report.txt

NICE_ARGS = -n 19

INCLUDE_PATHS = -I./btree_framework -I./compatibility -I./avp_path_find

CFLAGS_REL = -O3 --std=c++11 -march=native -fpermissive -DBTREE_ITERATOR_REGISTRATION -DBTREEFRAMEWORK_TEST_BENCH_ENABLED $(INCLUDE_PATHS)
CFLAGS_DBG = -g -O0 --std=c++11 -ftemplate-backtrace-limit=0 -fpermissive -DBTREE_ITERATOR_REGISTRATION -DBTREEFRAMEWORK_TEST_BENCH_ENABLED -D_DEBUG $(INCLUDE_PATHS)
CFLAGS_COV = -g -O0 --std=c++11 -fprofile-arcs -ftest-coverage -coverage -fno-inline -fno-inline-small-functions -fno-default-inline -fpermissive -DBTREE_ITERATOR_REGISTRATION -DBTREEFRAMEWORK_TEST_BENCH_ENABLED -D_DEBUG $(INCLUDE_PATHS)

LFLAGS_REL = 
LFLAGS_DBG = 
LFLAGS_COV = -lgcov -coverage

SOURCE =													\
./compatibility/windows.cpp									\
./btree_framework/btreecommon.cpp							\
./btree_framework/btreeioprop.cpp							\
./btree_framework/btreeramioprop.cpp						\
./btree_framework/btreefileioprop.cpp						\
./btree_framework/btreeio.cpp								\
./btree_framework/btreeioblock.cpp							\
./btree_framework/btreeiolinear.cpp							\
./btree_framework/btreeioram.cpp							\
./btree_framework/btreeiofile.cpp							\
./btree_framework/btreeaux.cpp								\
./btree_framework/btreeif.cpp								\
./btree_framework/btreedefaults.cpp							\
./btree_framework/btreebaseif.cpp							\
./btree_framework/btreebasedefaults.cpp						\
./btree_framework/btreeiter.cpp								\
./btree_framework/btreeassociative.cpp						\
./btree_framework/btreearray.cpp							\
./btree_framework/btreekeysort.cpp							\
./btree_framework/associative/base/btreeassociativebase.cpp	\
./btree_framework/associative/btreemap.cpp					\
./btree_framework/associative/btreemultimap.cpp				\
./btree_framework/associative/btreemultiset.cpp				\
./btree_framework/associative/btreeset.cpp					\
./avp_path_find/avp_path_find_core.cpp						\
./btreetestcommon.cpp										\
./btreecommonprimitives.cpp									\
./btreearraytestprimitive.cpp								\
./btreekeysorttestprimitive.cpp								\
./btreemultimapprimitives.cpp								\
./btreemultisetprimitives.cpp								\
./btreetestarray.cpp										\
./btreetestkeysort.cpp										\
./btreetestmap.cpp											\
./btreetestmultimap.cpp										\
./btreetestmultiset.cpp										\
./btreetestset.cpp											\
./btreearraytestwrapper.cpp									\
./btreeassociativetestwrapper.cpp							\
./btreemaptestwrapper.cpp									\
./btreemultimaptestwrapper.cpp								\
./btreesettestwrapper.cpp									\
./btreemultisettestwrapper.cpp								\
./btreekeysorttestwrapper.cpp								\
./btreearraytestbench.cpp									\
./btreearrayitertestbench.cpp								\
./btreekeysorttestbench.cpp									\
./btreekeysortitertestbench.cpp								\
./btreemaptestbench.cpp										\
./btreemultimaptestbench.cpp								\
./btreemultisettestbench.cpp								\
./btreesettestbench.cpp										\
./btreetestarrayperf.cpp									\
./btreearrayperftestbench.cpp								\
./btreetest.cpp

VGFLAGS = --tool=memcheck -v --child-silent-after-fork=yes --leak-check=full --leak-resolution=high --show-leak-kinds=all --show-reachable=yes --track-origins=yes --num-callers=30 --read-var-info=yes --keep-stacktraces=alloc-and-free

OBJREL = $(patsubst %.cpp,%.rel.o,$(SOURCE))
OBJDBG = $(patsubst %.cpp,%.dbg.o,$(SOURCE))
OBJCOV = $(patsubst %.cpp,%.cov.o,$(SOURCE))

REGRESSION_ARRAY_TESTS = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
REGRESSION_KEYSORT_TESTS = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33
REGRESSION_ARRAY_ITER_TESTS = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
REGRESSION_KEYSORT_ITER_TESTS = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17
REGRESSION_MULTIMAP_TESTS = 0 1 2 3 4 5 6 7 8 9 10
REGRESSION_MAP_TESTS = 0 1 2 3 4 5 6 7 8 9 10
REGRESSION_MULTISET_TESTS = 0 1 2 3 4 5 6 7 8 9 10
REGRESSION_SET_TESTS = 0 1 2 3 4 5 6 7 8 9 10
PERFORMANCE_APP_AVP_TESTS = 0 1 2

ARRAY_TESTS = $(patsubst %,ARRAY_TEST_%,$(REGRESSION_ARRAY_TESTS))
KEYSORT_TESTS = $(patsubst %,KEYSORT_TEST_%,$(REGRESSION_KEYSORT_TESTS))
MULTIMAP_TESTS = $(patsubst %,MULTIMAP_TEST_%,$(REGRESSION_MULTIMAP_TESTS))
MAP_TESTS = $(patsubst %,MAP_TEST_%,$(REGRESSION_MAP_TESTS))
MULTISET_TESTS = $(patsubst %,MULTISET_TEST_%,$(REGRESSION_MULTISET_TESTS))
SET_TESTS = $(patsubst %,SET_TEST_%,$(REGRESSION_SET_TESTS))

ARRAY_ITER_TESTS = $(patsubst %,ARRAY_ITER_TEST_%,$(REGRESSION_ARRAY_ITER_TESTS))
KEYSORT_ITER_TESTS = $(patsubst %,KEYSORT_ITER_TEST_%,$(REGRESSION_KEYSORT_ITER_TESTS))

APP_AVP_TESTS = $(patsubst %,APP_AVP_TEST_%,$(PERFORMANCE_APP_AVP_TESTS))

ARRAY_COV_TESTS = $(patsubst %,ARRAY_COV_TEST_%,$(REGRESSION_ARRAY_TESTS))
KEYSORT_COV_TESTS = $(patsubst %,KEYSORT_COV_TEST_%,$(REGRESSION_KEYSORT_TESTS))
MULTIMAP_COV_TESTS = $(patsubst %,MULTIMAP_COV_TEST_%,$(REGRESSION_MULTIMAP_TESTS))
MAP_COV_TESTS = $(patsubst %,MAP_COV_TEST_%,$(REGRESSION_MAP_TESTS))
MULTISET_COV_TESTS = $(patsubst %,MULTISET_COV_TEST_%,$(REGRESSION_MULTISET_TESTS))
SET_COV_TESTS = $(patsubst %,SET_COV_TEST_%,$(REGRESSION_SET_TESTS))

ARRAY_ITER_COV_TESTS = $(patsubst %,ARRAY_ITER_COV_TEST_%,$(REGRESSION_ARRAY_ITER_TESTS))
KEYSORT_ITER_COV_TESTS = $(patsubst %,KEYSORT_ITER_COV_TEST_%,$(REGRESSION_KEYSORT_ITER_TESTS))

APP_AVP_COV_TESTS = $(patsubst %,APP_AVP_COV_TEST_%,$(PERFORMANCE_APP_AVP_TESTS))

VG_ARRAY_TESTS = $(patsubst %,VG_ARRAY_TEST_%,$(REGRESSION_ARRAY_TESTS))
VG_KEYSORT_TESTS = $(patsubst %,VG_KEYSORT_TEST_%,$(REGRESSION_KEYSORT_TESTS))
VG_MULTIMAP_TESTS = $(patsubst %,VG_MULTIMAP_TEST_%,$(REGRESSION_MULTIMAP_TESTS))
VG_MAP_TESTS = $(patsubst %,VG_MAP_TEST_%,$(REGRESSION_MAP_TESTS))
VG_MULTISET_TESTS = $(patsubst %,VG_MULTISET_TEST_%,$(REGRESSION_MULTISET_TESTS))
VG_SET_TESTS = $(patsubst %,VG_SET_TEST_%,$(REGRESSION_SET_TESTS))

VG_ARRAY_ITER_TESTS = $(patsubst %,VG_ARRAY_ITER_TEST_%,$(REGRESSION_ARRAY_ITER_TESTS))
VG_KEYSORT_ITER_TESTS = $(patsubst %,VG_KEYSORT_ITER_TEST_%,$(REGRESSION_KEYSORT_ITER_TESTS))

VG_APP_AVP_TESTS = $(patsubst %,VG_APP_AVP_TEST_%,$(PERFORMANCE_APP_AVP_TESTS))

APP_COV_TESTS = $(APP_AVP_COV_TESTS)
VG_APPS = $(VG_APP_AVP_TESTS)

all: regression valgrind_report coverage_report performance release

%.rel.o: %.cpp
	$(GPP) $(CFLAGS_REL) -c -o $@ $<

%.dbg.o: %.cpp
	$(GPP) $(CFLAGS_DBG) -c -o $@ $<

%.cov.o: %.cpp
	$(GPP) $(CFLAGS_COV) -c -o $@ $<

release: $(OBJREL)
	$(GPP) $(LFLAGS_REL) $? -o $(PROGREL)

debug: $(OBJDBG)
	$(GPP) $(LFLAGS_DBG) $? -o $(PROGDBG)

coverage: $(OBJCOV)
	$(GPP) $(LFLAGS_COV) $? -o $(PROGCOV)

array_tests: $(ARRAY_TESTS)

keysort_tests: $(KEYSORT_TESTS)

map_tests: $(MAP_TESTS)

multimap_tests: $(MULTIMAP_TESTS)

set_tests: $(SET_TESTS)

multiset_tests: $(MULTISET_TESTS)

array_iter_tests: $(ARRAY_ITER_TESTS)

keysort_iter_tests: $(KEYSORT_ITER_TESTS)

regression: array_tests keysort_tests map_tests multimap_tests set_tests multiset_tests array_iter_tests keysort_iter_tests

performance: $(APP_AVP_TESTS)

ARRAY_TEST_%: debug
	$(PROGDBG) -type array -test $(patsubst ARRAY_TEST_%,%,$@)

KEYSORT_TEST_%: debug
	$(PROGDBG) -type keysort -test $(patsubst KEYSORT_TEST_%,%,$@)
	
MULTIMAP_TEST_%: debug
	$(PROGDBG) -type stl-multimap -test $(patsubst MULTIMAP_TEST_%,%,$@)

MAP_TEST_%: debug
	$(PROGDBG) -type stl-map -test $(patsubst MAP_TEST_%,%,$@)

MULTISET_TEST_%: debug
	$(PROGDBG) -type stl-multiset -test $(patsubst MULTISET_TEST_%,%,$@)

SET_TEST_%: debug
	$(PROGDBG) -type stl-set -test $(patsubst SET_TEST_%,%,$@)

ARRAY_ITER_TEST_%: debug
	$(PROGDBG) -type array-iter -test $(patsubst ARRAY_ITER_TEST_%,%,$@)

KEYSORT_ITER_TEST_%: debug
	$(PROGDBG) -type keysort-iter -test $(patsubst KEYSORT_ITER_TEST_%,%,$@)

APP_AVP_TEST_%: release
	$(PROGREL) -type app -test $(patsubst APP_AVP_TEST_%,%,$@)

$(COVERAGE_BASE): coverage
	$(COV) -i -c -b . -d . -o $(COVERAGE_BASE)

ARRAY_COV_TEST_%: $(COVERAGE_BASE)
	$(PROGCOV) -type array -test $(patsubst ARRAY_COV_TEST_%,%,$@)

KEYSORT_COV_TEST_%: $(COVERAGE_BASE)
	$(PROGCOV) -type keysort -test $(patsubst KEYSORT_COV_TEST_%,%,$@)
	
MULTIMAP_COV_TEST_%: $(COVERAGE_BASE)
	$(PROGCOV) -type stl-multimap -test $(patsubst MULTIMAP_COV_TEST_%,%,$@)

MAP_COV_TEST_%: $(COVERAGE_BASE)
	$(PROGCOV) -type stl-map -test $(patsubst MAP_COV_TEST_%,%,$@)

MULTISET_COV_TEST_%: $(COVERAGE_BASE)
	$(PROGCOV) -type stl-multiset -test $(patsubst MULTISET_COV_TEST_%,%,$@)

SET_COV_TEST_%: $(COVERAGE_BASE)
	$(PROGCOV) -type stl-set -test $(patsubst SET_COV_TEST_%,%,$@)

ARRAY_ITER_COV_TEST_%: $(COVERAGE_BASE)
	$(PROGCOV) -type array-iter -test $(patsubst ARRAY_ITER_COV_TEST_%,%,$@)
	
KEYSORT_ITER_COV_TEST_%: $(COVERAGE_BASE)
	$(PROGCOV) -type keysort-iter -test $(patsubst KEYSORT_ITER_COV_TEST_%,%,$@)
	
APP_AVP_COV_TEST_%: $(COVERAGE_BASE)
	$(PROGCOV) -type app -test $(patsubst APP_AVP_COV_TEST_%,%,$@)

$(COVERAGE_OUT): $(APP_COV_TESTS) $(ARRAY_COV_TESTS) $(KEYSORT_COV_TESTS) $(MULTIMAP_COV_TESTS) $(MAP_COV_TESTS) $(MULTISET_COV_TESTS) $(SET_COV_TESTS) $(ARRAY_ITER_COV_TESTS) $(KEYSORT_ITER_COV_TESTS)
	$(COV) -c -b . -d . -o $(COVERAGE_OUT)

$(COVERAGE_COMB): $(COVERAGE_OUT)
	$(COV) -a $(COVERAGE_BASE) -a $(COVERAGE_OUT) -o $(COVERAGE_COMB)

coverage_report: $(COVERAGE_COMB)
	rm -rfv $(COVERAGE_REPORT)
	mkdir $(COVERAGE_REPORT)
	$(GEN) $(COVERAGE_COMB) -o $(COVERAGE_REPORT)

VG_ARRAY_TEST_%: debug
	$(eval $@PARAMS := -type array -test $(patsubst VG_ARRAY_TEST_%,%,$@))
	$(eval $@LOG_FILE := vg_array$(patsubst VG_ARRAY_TEST_%,%,$@).vglog)
	$(eval $@OUT_FILE := vg_array$(patsubst VG_ARRAY_TEST_%,%,$@).vgout)
	$(NICE) $(NICE_ARGS) $(VALGRIND) $(VGFLAGS) --log-file=$($@LOG_FILE) $(PROGDBG) $($@PARAMS)
	echo $(PROGDBG) $($@PARAMS) > $($@OUT_FILE)
	cat $($@LOG_FILE) >> $($@OUT_FILE)
	cat $($@OUT_FILE) >> $(VALGRIND_REPORT)
	rm -fv $($@LOG_FILE) $($@OUT_FILE)

VG_KEYSORT_TEST_%: debug
	$(eval $@PARAMS := -type keysort -test $(patsubst VG_KEYSORT_TEST_%,%,$@))
	$(eval $@LOG_FILE := vg_keysort$(patsubst VG_KEYSORT_TEST_%,%,$@).vglog)
	$(eval $@OUT_FILE := vg_keysort$(patsubst VG_KEYSORT_TEST_%,%,$@).vgout)
	$(NICE) $(NICE_ARGS) $(VALGRIND) $(VGFLAGS) --log-file=$($@LOG_FILE) $(PROGDBG) $($@PARAMS)
	echo $(PROGDBG) $($@PARAMS) > $($@OUT_FILE)
	cat $($@LOG_FILE) >> $($@OUT_FILE)
	cat $($@OUT_FILE) >> $(VALGRIND_REPORT)
	rm -fv $($@LOG_FILE) $($@OUT_FILE)
	
VG_MULTIMAP_TEST_%: debug
	$(eval $@PARAMS := -type stl-multimap -test $(patsubst VG_MULTIMAP_TEST_%,%,$@))
	$(eval $@LOG_FILE := vg_multimap$(patsubst VG_MULTIMAP_TEST_%,%,$@).vglog)
	$(eval $@OUT_FILE := vg_multimap$(patsubst VG_MULTIMAP_TEST_%,%,$@).vgout)
	$(NICE) $(NICE_ARGS) $(VALGRIND) $(VGFLAGS) --log-file=$($@LOG_FILE) $(PROGDBG) $($@PARAMS)
	echo $(PROGDBG) $($@PARAMS) > $($@OUT_FILE)
	cat $($@LOG_FILE) >> $($@OUT_FILE)
	cat $($@OUT_FILE) >> $(VALGRIND_REPORT)
	rm -fv $($@LOG_FILE) $($@OUT_FILE)

VG_MAP_TEST_%: debug
	$(eval $@PARAMS := -type stl-map -test $(patsubst VG_MAP_TEST_%,%,$@))
	$(eval $@LOG_FILE := vg_map$(patsubst VG_MAP_TEST_%,%,$@).vglog)
	$(eval $@OUT_FILE := vg_map$(patsubst VG_MAP_TEST_%,%,$@).vgout)
	$(NICE) $(NICE_ARGS) $(VALGRIND) $(VGFLAGS) --log-file=$($@LOG_FILE) $(PROGDBG) $($@PARAMS)
	echo $(PROGDBG) $($@PARAMS) > $($@OUT_FILE)
	cat $($@LOG_FILE) >> $($@OUT_FILE)
	cat $($@OUT_FILE) >> $(VALGRIND_REPORT)
	rm -fv $($@LOG_FILE) $($@OUT_FILE)

VG_MULTISET_TEST_%: debug
	$(eval $@PARAMS := -type stl-multiset -test $(patsubst VG_MULTISET_TEST_%,%,$@))
	$(eval $@LOG_FILE := vg_multiset$(patsubst VG_MULTISET_TEST_%,%,$@).vglog)
	$(eval $@OUT_FILE := vg_multiset$(patsubst VG_MULTISET_TEST_%,%,$@).vgout)
	$(NICE) $(NICE_ARGS) $(VALGRIND) $(VGFLAGS) --log-file=$($@LOG_FILE) $(PROGDBG) $($@PARAMS)
	echo $(PROGDBG) $($@PARAMS) > $($@OUT_FILE)
	cat $($@LOG_FILE) >> $($@OUT_FILE)
	cat $($@OUT_FILE) >> $(VALGRIND_REPORT)
	rm -fv $($@LOG_FILE) $($@OUT_FILE)

VG_SET_TEST_%: debug
	$(eval $@PARAMS := -type stl-set -test $(patsubst VG_SET_TEST_%,%,$@))
	$(eval $@LOG_FILE := vg_set$(patsubst VG_SET_TEST_%,%,$@).vglog)
	$(eval $@OUT_FILE := vg_set$(patsubst VG_SET_TEST_%,%,$@).vgout)
	$(NICE) $(NICE_ARGS) $(VALGRIND) $(VGFLAGS) --log-file=$($@LOG_FILE) $(PROGDBG) $($@PARAMS)
	echo $(PROGDBG) $($@PARAMS) > $($@OUT_FILE)
	cat $($@LOG_FILE) >> $($@OUT_FILE)
	cat $($@OUT_FILE) >> $(VALGRIND_REPORT)
	rm -fv $($@LOG_FILE) $($@OUT_FILE)

VG_ARRAY_ITER_TEST_%: debug
	$(eval $@PARAMS := -type array-iter -test $(patsubst VG_ARRAY_ITER_TEST_%,%,$@))
	$(eval $@LOG_FILE := vg_array$(patsubst VG_ARRAY_ITER_TEST_%,%,$@).vglog)
	$(eval $@OUT_FILE := vg_array$(patsubst VG_ARRAY_ITER_TEST_%,%,$@).vgout)
	$(NICE) $(NICE_ARGS) $(VALGRIND) $(VGFLAGS) --log-file=$($@LOG_FILE) $(PROGDBG) $($@PARAMS)
	echo $(PROGDBG) $($@PARAMS) > $($@OUT_FILE)
	cat $($@LOG_FILE) >> $($@OUT_FILE)
	cat $($@OUT_FILE) >> $(VALGRIND_REPORT)
	rm -fv $($@LOG_FILE) $($@OUT_FILE)
	
VG_KEYSORT_ITER_TEST_%: debug
	$(eval $@PARAMS := -type keysort-iter -test $(patsubst VG_KEYSORT_ITER_TEST_%,%,$@))
	$(eval $@LOG_FILE := vg_array$(patsubst VG_KEYSORT_ITER_TEST_%,%,$@).vglog)
	$(eval $@OUT_FILE := vg_array$(patsubst VG_KEYSORT_ITER_TEST_%,%,$@).vgout)
	$(NICE) $(NICE_ARGS) $(VALGRIND) $(VGFLAGS) --log-file=$($@LOG_FILE) $(PROGDBG) $($@PARAMS)
	echo $(PROGDBG) $($@PARAMS) > $($@OUT_FILE)
	cat $($@LOG_FILE) >> $($@OUT_FILE)
	cat $($@OUT_FILE) >> $(VALGRIND_REPORT)
	rm -fv $($@LOG_FILE) $($@OUT_FILE)
	
VG_APP_AVP_TEST_%: debug
	$(eval $@PARAMS := -type app -test $(patsubst VG_APP_AVP_TEST_%,%,$@))
	$(eval $@LOG_FILE := vg_app_avp$(patsubst VG_APP_AVP_COV_TEST_%,%,$@).vglog)
	$(eval $@OUT_FILE := vg_app_avp$(patsubst VG_APP_AVP_COV_TEST_%,%,$@).vgout)
	$(NICE) $(NICE_ARGS) $(VALGRIND) $(VGFLAGS) --log-file=$($@LOG_FILE) $(PROGDBG) $($@PARAMS)
	echo $(PROGDBG) $($@PARAMS) > $($@OUT_FILE)
	cat $($@LOG_FILE) >> $($@OUT_FILE)
	cat $($@OUT_FILE) >> $(VALGRIND_REPORT)
	rm -fv $($@LOG_FILE) $($@OUT_FILE)

valgrind_report: $(VG_APPS) $(VG_ARRAY_TESTS) $(VG_KEYSORT_TESTS) $(VG_MULTIMAP_TESTS) $(VG_MAP_TESTS) $(VG_MULTISET_TESTS) $(VG_SET_TESTS) $(VG_ARRAY_ITER_TESTS) $(VG_KEYSORT_ITER_TESTS)

clean:
	rm -fv $(PROGREL) $(PROGDBG) $(PROGCOV) $(OBJREL) $(OBJDBG) $(OBJCOV) $(COVERAGE_BASE) $(COVERAGE_OUT) $(COVERAGE_COMB) ./*/*.gcov ./*/*.gcda ./*/*.gcno ./*/*.gch ./*.gcov ./*.gcda ./*.gcno ./*.gch *.vglog *.vgout $(VALGRIND_REPORT)

clean_coverage:
	rm -rfv $(COVERAGE_REPORT) $(COVERAGE_BASE) $(COVERAGE_OUT) $(COVERAGE_COMB) ./*/*.gcov ./*/*.gcda ./*/*.gch ./*.gcov ./*.gcda ./*.gch

