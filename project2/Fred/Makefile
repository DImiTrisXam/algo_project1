CPP = g++
CFLAGS = -c -march=native -Ofast -static-libgcc -static-libstdc++ -fpermissive -fPIC -ffast-math -fno-trapping-math -ftree-vectorize -fopt-info-vec -fopt-info-loop


BIN = ../bin
BUILD = ../build
INC = -Iinclude

SRC_DIR = src
SRC_SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
SRC_OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BIN)/%.o, $(SRC_SRC_FILES))

all: $(SRC_OBJ_FILES)

create_directories:
	@mkdir -p ../bin
	@mkdir -p ../build

$(BIN)/%.o: $(SRC_DIR)/%.cpp include/%.hpp
	$(CPP) $(INC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -r -f $(BIN) $(BUILD)

