
BUILD_ROOT ?= ../../..
VERBOSE ?= off
BIN_PATH := $(BUILD_ROOT)
BIN := $(BIN_PATH)/dbBenchmarkCpp
SRC := $(BUILD_ROOT)/src

GTEST_OUTPUT_XML_FILE = test.xml

EXCLUDES := $(SRC)/dbBenchmarkCpp.cpp

CC = g++
INCLUDES = -I/usr/include/acl-redis/lib_acl -I/usr/include/acl-redis/lib_acl_cpp -I/usr/include/acl-redis/lib_protocol \
			-I/usr/include/mongocxx/v_noabi -I/usr/include/bsoncxx/v_noabi
CCFLAGS = -Wall -g -std=c++14 -I$(BUILD_ROOT)/src $(INCLUDES)
LDFLAGS = -lboost_system -lboost_filesystem -lboost_date_time -lboost_program_options -lboost_thread  -pthread -lg3logger \
		-lacl -lacl_cpp -lprotocol -lg3logger -lmongocxx -lbson-1.0 -lbsoncxx -lpqxx -lpq -lcassandra \
		-lgtest -lgtest_main -lgcov -pthread -lgmock --coverage

SRC_OBJ_FILES := $(patsubst %.cpp, obj/%.o, $(patsubst $(SRC)/%,%,$(filter-out $(EXCLUDES), $(shell find $(SRC) -type f -name "*.cpp"))))
OBJ_FILES := $(patsubst %.cpp, obj/%.o, $(shell find . -type f -name "*.cpp"))
HEADER_FILES := $(shell find . -type f -name "*.h")

all: check-dirs run

check-dirs:
	@if [ ! -d obj ]; then mkdir -p obj; fi

run: $(BIN)
	@echo ">>> Running unit tests"
	@cd $(BIN_PATH);	\
	$(BIN) --gtest_output="xml:$(BIN_PATH)/$(GTEST_OUTPUT_XML_FILE)"
	@gcovr -r $(BUILD_ROOT) --gcov-exclude='src/Cinar/.*' -x > $(BIN_PATH)/coverage-cnrnrf.xml \
		-e ".*TestHelper.*" -e ".*MongoDB.*" -e ".*Log.*" -e ".*DBFactory.*" -e ".*DBConnectionHandler.*" \
		-e "test/unit-tests/src/.*" -e ".*EventSpy.*" -e ".*NotificationEvent.*" -e ".*Exception.*"

$(BIN): $(SRC_OBJ_FILES) $(OBJ_FILES)
	@echo ">>> Building $@"
	@if [ "$(VERBOSE)" = "on" ]; then echo $(CC) -o $@ $^ $(LDFLAGS); fi
	@$(CC) -o $@ $^ $(LDFLAGS)

obj/%.o: $(SRC)/%.cpp $(SRC)/%.h $(SRC)/%.hpp
	@mkdir -p $(dir $@)
	@echo ">>> Compiling $(patsubst $(SRC)/%,%,$<)"
	@if [ "$(VERBOSE)" = "on" ]; then echo $(CC) $(CCFLAGS) -c -o $@ $<; fi
	@$(CC) $(CCFLAGS) -c -o $@ $<

$(SRC)/%.h:
	@true

$(SRC)/%.hpp:
	@true

obj/%.o: %.cpp %.h %.hpp $(HEADER_FILES)
	@mkdir -p $(dir $@)
	@echo ">>> Compiling $<"
	@if [ "$(VERBOSE)" = "on" ]; then echo $(CC) $(CCFLAGS) -c -o $@ $<; fi
	@$(CC) $(CCFLAGS) -c -o $@ $<

%.h:
	@true

%.hpp:
	@true

clean:
	@echo ">>> Cleaning build artifacts - dir: $(shell pwd)"
	@rm -rf obj
	@rm -f $(BIN)
	@rm -f $(BIN_PATH)/$(GTEST_OUTPUT_XML_FILE)
	@rm -f $(BIN_PATH)/coverage-*.xml
	@find . \( -name "*.o" -o -name "*.gcno" -o -name "*.gcda" \) -exec rm -rf {} \;

covhtml: run
	@echo ">>> Generating html for coverage report"
	@mkdir -p $(BIN_PATH)/Htmlreports
	@gcovr -r $(BUILD_ROOT) --gcov-exclude='src/Cinar/.*' --html --html-details -o $(BIN_PATH)/Htmlreports/coverage-html.html \
		-e ".*TestHelper.*" -e ".*MongoDB.*" -e ".*Log.*" -e ".*DBFactory.*" -e ".*DBConnectionHandler.*" \
		-e "test/unit-tests/src/.*" -e ".*EventSpy.*" -e ".*NotificationEvent.*" -e ".*Exception.*"