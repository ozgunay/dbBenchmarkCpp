
# Build system defines
BUILD_ROOT := $(shell pwd)
MAKE := BUILD_ROOT=$(BUILD_ROOT) make --silent -j $(shell grep -c ^processor /proc/cpuinfo)
MAKE_SINGLE := BUILD_ROOT=$(BUILD_ROOT) make --silent -j 1
PACKAGE_VERSION := $(MAJOR).$(PHASE_NUMBER).$(SPRINT_NUMBER).$(BUILD_NUMBER)
DOC_PATH  := doc

# Build target folders
DIRS ?= src

all: banner check-dirs .cppcheck.done $(DIRS)

banner:
	@echo
	@echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
	@echo ">>> Starting build"
	@echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
	@echo

check-dirs:
	@if [ ! -d logs ]; then mkdir -p logs; fi

.cppcheck.done: $(shell find . -name "*.cpp" -o -name "*.h*")
	@echo ">>> Running cppcheck"
	@cppcheck --enable=all --suppress=missingIncludeSystem --xml --xml-version=2 src 2> cppcheck-dbBenchmarkCpp.xml
	@touch $@

.PHONY: $(DIRS) test
$(DIRS) test:
	@echo ">>> Building $@ folder"
	@$(MAKE) -C $@

clean:
	@echo ">>> Cleaning build artifacts - dir: $(shell pwd)"
	@rm -rf .cppcheck.done
	@rm -rf cppcheck-*.xml
	@rm -rf $(DOC_PATH)
	@for dir in $(DIRS); do $(MAKE) -C $$dir $@; done
	@$(MAKE) -C test $@

dist: clean all test
	@echo ">>> Building $@ package - Version: $(PACKAGE_VERSION)"
	@PACKAGE_VERSION=$(PACKAGE_VERSION) $(MAKE) -C $@

covhtml:
	@$(MAKE) -C test/unit-tests/src $@

debug:
	@$(MAKE) -C src $@

doc:
	@echo ">> Creating Doxygen documentation"
	@mkdir -p $(DOC_PATH)
	@doxygen Doxyfile
