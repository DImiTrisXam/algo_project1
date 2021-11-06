CPP = g++
OBJ = lsh.o cube.o cluster.o
CFLAGS = -c
BIN = bin
BUILD = build
UTIL = bin/metrics.o bin/utilities.o bin/hash.o bin/PriorityQueue.o

all: lsh cube cluster

create_directories:
	@mkdir -p bin
	@mkdir -p build

lsh: create_directories bin/lsh.o bin/lshSearch.o $(UTIL)
	$(CPP) bin/lsh.o bin/lshSearch.o $(UTIL) -o $(BUILD)/$@

cube: create_directories bin/cube.o $(UTIL)
	$(CPP) bin/cube.o $(UTIL) -o $(BUILD)/$@

cluster: create_directories bin/cluster.o $(UTIL)
	$(CPP) bin/cluster.o $(UTIL) -o $(BUILD)/$@

bin/lsh.o: lsh/lsh.cpp
	$(CPP) $(CFLAGS) $< -o $@

bin/lshSearch.o: lsh/lshSearch.cpp lsh/lshSearch.hpp 
	$(CPP) $(CFLAGS) $< -o $@

bin/cube.o: cube/cube.cpp
	$(CPP) $(CFLAGS) $< -o $@

bin/cluster.o: cluster/cluster.cpp
	$(CPP) $(CFLAGS) $< -o $@

bin/metrics.o: utilities/metrics.cpp utilities/metrics.hpp 
	$(CPP) $(CFLAGS) $< -o $@

bin/utilities.o: utilities/utilities.cpp utilities/utilities.hpp 
	$(CPP) $(CFLAGS) $< -o $@

bin/hash.o: utilities/hash.cpp utilities/hash.hpp 
	$(CPP) $(CFLAGS) $< -o $@

bin/PriorityQueue.o: utilities/PriorityQueue.cpp utilities/PriorityQueue.hpp 
	$(CPP) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -r -f $(BIN) $(BUILD) output*
