CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wtype-limits -Wextra -lpthread
SRC = $(wildcard ./src/*.cpp)
OUT_DIR = ./out

.PHONY: all
all: $(SRC)

.PHONY: $(wildcard src/*.cpp)
$(wildcard src/*.cpp):
	@ mkdir -p $(OUT_DIR)
	$(CXX) $@ $(CXXFLAGS) -o $(OUT_DIR)/$(notdir $(basename $@))

clean:
	@ rm -rf $(OUT_DIR)
