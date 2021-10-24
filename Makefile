CPP = g++
OBJ = lsh.o cube.o cluster.o
CFLAGS = -c
BIN = bin
BUILD = build

all: lsh cube cluster

create_directories:
	@mkdir -p bin
	@mkdir -p build

lsh: create_directories bin/lsh.o bin/metrics.o
	$(CPP) bin/lsh.o bin/metrics.o -o $(BUILD)/$@

cube: create_directories bin/cube.o bin/metrics.o
	$(CPP) bin/cube.o bin/metrics.o -o $(BUILD)/$@

cluster: create_directories bin/cluster.o bin/metrics.o
	$(CPP) bin/cluster.o bin/metrics.o -o $(BUILD)/$@

bin/lsh.o: lsh/lsh.cpp
	$(CPP) $(CFLAGS) $< -o $@

bin/cube.o: cube/cube.cpp
	$(CPP) $(CFLAGS) $< -o $@

bin/cluster.o: cluster/cluster.cpp
	$(CPP) $(CFLAGS) $< -o $@

bin/metrics.o: utilities/metrics.cpp
	$(CPP) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -r -f $(BIN) $(BUILD)
