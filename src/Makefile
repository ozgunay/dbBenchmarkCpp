BUILD_ROOT ?= ..
VERBOSE ?= off
BIN_PATH = $(BUILD_ROOT)
BIN = $(BIN_PATH)/dbBenchmarkCpp

CC = g++
INCLUDES = -I/usr/include/acl-redis/lib_acl		\
		-I/usr/include/acl-redis/lib_acl_cpp	\
		-I/usr/include/acl-redis/lib_protocol	\
		-I/usr/include/mongocxx/v_noabi		    \
		-I/usr/include/bsoncxx/v_noabi
CCFLAGS = -Wall -g -std=c++14 -I$(BUILD_ROOT)/src $(INCLUDES)
LDFLAGS = -pthread -lg3logger -lboost_system -lboost_filesystem		\
		-lboost_date_time -lboost_program_options -lboost_thread	\
		-lacl -lacl_cpp -lprotocol  -lmongocxx -lbson-1.0 -lbsoncxx	\
		-lpqxx -lpq -lcassandra

all: check-dirs $(BIN)

check-dirs:
	@if [ ! -d obj ]; then mkdir -p obj; fi

$(BIN): $(patsubst %.cpp, obj/%.o, $(shell find . -type f -name "*.cpp"))
	@echo ">>> Building $@"
	@if [ "$(VERBOSE)" = "on" ]; then echo $(CC) -o $@ $^ $(LDFLAGS); fi
	@$(CC) -o $@ $^ $(LDFLAGS)

obj/%.o: %.cpp %.h %.hpp
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
	@find . \( -name "*.o" -o -name "*.gcno" -o -name "*.gcda" \) -exec rm -rf {} \;
	@rm -rf $(BIN) obj

debug: $(BIN)
	@cd $(BIN_PATH);	\
	gdbserver :9091 $(BIN)
