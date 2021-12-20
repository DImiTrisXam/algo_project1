CPP = g++
CFLAGS = -c
BIN = bin
BUILD = build

FREDFLAGS = -c -march=native -Ofast -static-libgcc -static-libstdc++ -fpermissive -fPIC -ffast-math -fno-trapping-math -ftree-vectorize -fopt-info-vec -fopt-info-loop

UTIL_DIR = utilities
UTIL_SRC_FILES = $(wildcard $(UTIL_DIR)/*.cpp)
UTIL_OBJ_FILES = $(patsubst $(UTIL_DIR)/%.cpp, $(BIN)/%.o, $(UTIL_SRC_FILES))

SEARCH_DIR = search
SEARCH_SRC_FILES = $(wildcard $(SEARCH_DIR)/*.cpp)
SEARCH_INC_FILES = $(wildcard $(SEARCH_DIR)/*.hpp)
SEARCH_OBJ_FILES = $(patsubst $(SEARCH_DIR)/%.cpp, $(BIN)/%.o, $(SEARCH_SRC_FILES))

CLUSTER_DIR = cluster
CLUSTER_SRC_FILES = $(wildcard $(CLUSTER_DIR)/*.cpp)
CLUSTER_INC_FILES = $(wildcard $(CLUSTER_DIR)/*.hpp)
CLUSTER_OBJ_FILES = $(patsubst $(CLUSTER_DIR)/%.cpp, $(BIN)/%.o, $(CLUSTER_SRC_FILES))

CLUSTER_DEPENDS = bin/cluster.o bin/clusteringMethods.o bin/lshSearch.o bin/cubeSearch.o

FRED_DIR = Fred
FRED_FILES = $(wildcard $(FRED_DIR)/src/*.cpp)
FRED_OBJ_FILES = $(patsubst $(FRED_DIR)/src/%.cpp, $(BIN)/%.o, $(FRED_FILES))
INC = -IFred/include

all: search cluster

create_directories:
	@mkdir -p bin
	@mkdir -p build

search: create_directories $(SEARCH_OBJ_FILES) $(UTIL_OBJ_FILES) $(FRED_OBJ_FILES)
	$(CPP) $(SEARCH_OBJ_FILES) $(UTIL_OBJ_FILES) $(FRED_OBJ_FILES) -o $(BUILD)/$@

cluster: create_directories $(CLUSTER_DEPENDS) $(UTIL_OBJ_FILES) $(FRED_OBJ_FILES)
	$(CPP) $(CLUSTER_DEPENDS) $(UTIL_OBJ_FILES) $(FRED_OBJ_FILES) -o $(BUILD)/$@

$(BIN)/%.o: $(UTIL_DIR)/%.cpp $(UTIL_DIR)/%.hpp
	$(CPP) $(CFLAGS) $< -o $@

$(BIN)/%.o: $(SEARCH_DIR)/%.cpp $(SEARCH_INC_FILES)
	$(CPP) $(CFLAGS) $< -o $@

$(BIN)/%.o: $(CLUSTER_DIR)/%.cpp $(CLUSTER_INC_FILES)
	$(CPP) $(CFLAGS) $< -o $@

$(BIN)/%.o: $(FRED_DIR)/src/%.cpp $(FRED_DIR)/include/%.hpp
	$(CPP) $(FREDFLAGS) $(INC) $< -o $@


test: create_directories test.cpp $(UTIL_OBJ_FILES)
	$(CPP) test.cpp $(UTIL_OBJ_FILES) -o test

tests: create_directories ./tests/* $(UTIL_OBJ_FILES)
	$(CPP) ./tests/*.cpp $(UTIL_OBJ_FILES) -o runTests -lcunit

.PHONY: clean

clean:
	rm -r -f $(BIN) $(BUILD) output* test runTests