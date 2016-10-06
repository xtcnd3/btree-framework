GPP = g++
COV = lcov
GEN = genhtml
VALGRIND = /usr/bin/valgrind
NICE = nice
SH = sh
MAKE = make
CMAKE = cmake
PWD = pwd
CP = cp

PROGREL = ./btreetest
PROGDBG = ./btreetest_dbg
PROGCOV = ./btreetest_cov

COVERAGE_BASE = coverage.base
COVERAGE_OUT = coverage.out
COVERAGE_COMB = coverage.combined
COVERAGE_REPORT = coverage_report

VALGRIND_REPORT = valgrind_report.txt

NICE_ARGS = -n 19

CMAKE_DIR = ./cmake
CMAKE_DIR_DEBUG = $(CMAKE_DIR)/debug
CMAKE_DIR_RELEASE = $(CMAKE_DIR)/release
CMAKE_DIR_COVERAGE = $(CMAKE_DIR)/coverage

CMAKE_LISTS_FILE = ./CMakeLists.txt

MAKEFILE_VIA_CMAKE_DEBUG = $(CMAKE_DIR_DEBUG)/Makefile
MAKEFILE_VIA_CMAKE_RELEASE = $(CMAKE_DIR_RELEASE)/Makefile
MAKEFILE_VIA_CMAKE_COVERAGE = $(CMAKE_DIR_COVERAGE)/Makefile

MAKEFILE_REGRESSION_TESTS = ./Makefile.regression
MAKEFILE_CODE_COVERAGE_TESTS = ./Makefile.code_coverage
MAKEFILE_VALIDATION_TESTS = ./Makefile.validation
MAKEFILE_PERFORMANCE_TESTS = ./Makefile.performance

TEST_DIR = ./tests
TEST_DIR_REGRESSION = $(TEST_DIR)/regression
TEST_DIR_CODE_COVERAGE = $(TEST_DIR)/code_coverage
TEST_DIR_VALIDATION = $(TEST_DIR)/validation
TEST_DIR_VALIDATION_VALGRIND = $(TEST_DIR_VALIDATION)/valgrind

TEST_GENERATE_SCRIPT = ./generate_tests.sh
TEST_RESULT_SCRIPT = ./test_result.sh

VGFLAGS = --tool=memcheck -v --child-silent-after-fork=yes --leak-check=full --leak-resolution=high --show-leak-kinds=all --show-reachable=yes --track-origins=yes --num-callers=30 --read-var-info=yes --keep-stacktraces=alloc-and-free --log-file=valgrind.log


all: regression valgrind_report coverage_report performance release


$(MAKEFILE_VIA_CMAKE_DEBUG): $(CMAKE_LISTS_FILE)
	mkdir -p $(CMAKE_DIR_DEBUG)
	cd $(CMAKE_DIR_DEBUG); \
	$(CMAKE) ../..

debug: $(MAKEFILE_VIA_CMAKE_DEBUG)
	$(MAKE) -C $(CMAKE_DIR_DEBUG) -f ../../$(MAKEFILE_VIA_CMAKE_DEBUG) btreetest
	$(CP) $(CMAKE_DIR_DEBUG)/btreetest $(PROGDBG)


$(MAKEFILE_VIA_CMAKE_RELEASE): $(CMAKE_LISTS_FILE)
	mkdir -p $(CMAKE_DIR_RELEASE)
	cd $(CMAKE_DIR_RELEASE); \
	$(CMAKE) -D LINUX_BUILD_RELEASE=ON -D LINUX_BUILD_DEBUG=OFF ../..


release: $(MAKEFILE_VIA_CMAKE_RELEASE)
	$(MAKE) -C $(CMAKE_DIR_RELEASE) -f ../../$(MAKEFILE_VIA_CMAKE_RELEASE) btreetest
	$(CP) $(CMAKE_DIR_RELEASE)/btreetest $(PROGREL)


$(MAKEFILE_VIA_CMAKE_COVERAGE): $(CMAKE_LISTS_FILE)
	mkdir -p $(CMAKE_DIR_COVERAGE)
	cd $(CMAKE_DIR_COVERAGE); \
	$(CMAKE) -D LINUX_BUILD_COVERAGE=ON -D LINUX_BUILD_DEBUG=OFF ../..

coverage: $(MAKEFILE_VIA_CMAKE_COVERAGE)
	$(MAKE) -C $(CMAKE_DIR_COVERAGE) -f ../../$(MAKEFILE_VIA_CMAKE_COVERAGE) btreetest
	$(CP) $(CMAKE_DIR_COVERAGE)/btreetest $(PROGCOV)


$(MAKEFILE_REGRESSION_TESTS): 
	$(eval $@PWD := $(shell $(PWD)))
	$(SH) $(TEST_GENERATE_SCRIPT) $(MAKEFILE_REGRESSION_TESTS) $($@PWD)/$(PROGDBG) $(TEST_DIR_REGRESSION)

$(MAKEFILE_CODE_COVERAGE_TESTS): 
	$(eval $@PWD := $(shell $(PWD)))
	$(SH) $(TEST_GENERATE_SCRIPT) $(MAKEFILE_CODE_COVERAGE_TESTS) $($@PWD)/$(PROGCOV) $(TEST_DIR_CODE_COVERAGE)

$(MAKEFILE_VALIDATION_TESTS):
	$(eval $@PWD := $(shell $(PWD)))
	$(SH) $(TEST_GENERATE_SCRIPT) $(MAKEFILE_VALIDATION_TESTS) "$(NICE) $(NICE_ARGS) $(VALGRIND) $(VGFLAGS) $($@PWD)/$(PROGDBG)" ./tests/validation


regression: $(MAKEFILE_REGRESSION_TESTS) debug
	$(MAKE) -f $(MAKEFILE_REGRESSION_TESTS) tests
	$(SH) $(TEST_RESULT_SCRIPT) $(TEST_DIR_REGRESSION)


$(COVERAGE_BASE): coverage
	$(COV) -i -c -b . -d . -o $(COVERAGE_BASE)

$(COVERAGE_OUT): $(MAKEFILE_CODE_COVERAGE_TESTS) $(COVERAGE_BASE)
	$(MAKE) -f $(MAKEFILE_CODE_COVERAGE_TESTS) tests
	$(COV) -c -b . -d . -o $(COVERAGE_OUT)

$(COVERAGE_COMB): $(COVERAGE_OUT)
	$(COV) -a $(COVERAGE_BASE) -a $(COVERAGE_OUT) -o $(COVERAGE_COMB)

coverage_report: $(COVERAGE_COMB)
	rm -rfv $(COVERAGE_REPORT)
	mkdir $(COVERAGE_REPORT)
	$(GEN) $(COVERAGE_COMB) -o $(COVERAGE_REPORT)


valgrind_report: $(MAKEFILE_VALIDATION_TESTS) debug
	$(MAKE) -f $(MAKEFILE_VALIDATION_TESTS) $(TEST_DIR_VALIDATION_VALGRIND)


clean: 
	rm -fv $(PROGREL) $(PROGDBG) $(PROGCOV) $(COVERAGE_BASE) $(COVERAGE_OUT) $(COVERAGE_COMB) ./*/*.gcov ./*/*.gcda ./*/*.gcno ./*/*.gch ./*.gcov ./*.gcda ./*.gcno ./*.gch *.vglog *.vgout $(VALGRIND_REPORT) $(MAKEFILE_REGRESSION_TESTS) $(MAKEFILE_CODE_COVERAGE_TESTS) $(MAKEFILE_VALIDATION_TESTS)
	rm -rfv $(CMAKE_DIR)

clean_regression_tests: $(MAKEFILE_REGRESSION_TESTS)
	$(MAKE) -f $(MAKEFILE_REGRESSION_TESTS) clean

clean_failed_regression_tests: $(MAKEFILE_REGRESSION_TESTS)
	$(MAKE) -f $(MAKEFILE_REGRESSION_TESTS) clean_failed

clean_coverage_tests: $(MAKEFILE_CODE_COVERAGE_TESTS)
	$(MAKE) -f $(MAKEFILE_CODE_COVERAGE_TESTS) clean

clean_failed_coverage_tests: $(MAKEFILE_CODE_COVERAGE_TESTS)
	$(MAKE) -f $(MAKEFILE_CODE_COVERAGE_TESTS) clean_failed

clean_validation_tests: $(MAKEFILE_VALIDATION_TESTS)
	$(MAKE) -f $(MAKEFILE_VALIDATION_TESTS) clean

clean_failed_validation_tests: $(MAKEFILE_VALIDATION_TESTS)
	$(MAKE) -f $(MAKEFILE_VALIDATION_TESTS) clean_failed

clean_tests: clean_regression_tests clean_coverage_tests clean_validation_tests

clean_failed_tests: clean_failed_regression_tests clean_failed_coverage_tests clean_failed_validation_tests

clean_coverage: $(MAKEFILE_CODE_COVERAGE_TESTS)
	rm -rfv $(COVERAGE_REPORT) $(COVERAGE_BASE) $(COVERAGE_OUT) $(COVERAGE_COMB) $(MAKEFILE_CODE_COVERAGE_TESTS) ./*/*.gcov ./*/*.gcda ./*/*.gch ./*.gcov ./*.gcda ./*.gch

clean_all: clean_tests clean_coverage clean


.PHONY: clean clean_regression_tests clean_coverage_tests clean_validation_tests clean_failed_regression_tests clean_failed_coverage_tests clean_failed_validation_tests clean_tests clean_failed_tests clean_coverage $(MAKEFILE_REGRESSION_TESTS) $(MAKEFILE_CODE_COVERAGE_TESTS) $(MAKEFILE_VALIDATION_TESTS)

