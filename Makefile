CPP = g++
OBJ = lsh.o cube.o cluster.o
OUT = lsh cube cluster
CFLAGS = -c

all: lsh cube

lsh: lsh.o
	$(CPP) $^ -o $@

cube: cube.o
	$(CPP) $^ -o $@

cluster: cluster.o
	$(CPP) $^ -o $@

lsh.o: lsh.cpp
	$(CPP) $(CFLAGS) $<

cube.o: cube.cpp
	$(CPP) $(CFLAGS) $<

cluster.o: cluster.cpp
	$(CPP) $(CFLAGS) $<

.PHONY: clean

clean:
	rm -f $(OBJ) $(OUT)
