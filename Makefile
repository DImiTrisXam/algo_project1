CPP = g++
CFLAGS = -c
BIN = bin
BUILD = build
UTIL_DIR = utilities
UTIL_SRC_FILES = $(wildcard $(UTIL_DIR)/*.cpp)
UTIL_OBJ_FILES = $(patsubst $(UTIL_DIR)/%.cpp, $(BIN)/%.o, $(UTIL_SRC_FILES))

all: lsh cube cluster

create_directories:
	@mkdir -p bin
	@mkdir -p build

lsh: create_directories bin/lsh.o $(UTIL_OBJ_FILES)
	$(CPP) bin/lsh.o $(UTIL_OBJ_FILES) -o $(BUILD)/$@

cube: create_directories bin/cube.o $(UTIL_OBJ_FILES)
	$(CPP) bin/cube.o $(UTIL_OBJ_FILES) -o $(BUILD)/$@

cluster: create_directories bin/cluster.o $(UTIL_OBJ_FILES)
	$(CPP) bin/cluster.o $(UTIL_OBJ_FILES) -o $(BUILD)/$@

bin/lsh.o: lsh/lsh.cpp
	$(CPP) $(CFLAGS) $< -o $@

bin/cube.o: cube/cube.cpp
	$(CPP) $(CFLAGS) $< -o $@

bin/cluster.o: cluster/cluster.cpp
	$(CPP) $(CFLAGS) $< -o $@

$(BIN)/%.o: $(UTIL_DIR)/%.cpp $(UTIL_DIR)/%.hpp
	$(CPP) $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -r -f $(BIN) $(BUILD) output*
