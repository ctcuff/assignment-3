CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wtype-limits -Wextra -lpthread
OUT_DIR = ./out

all: birthday_party temperature

create_build_folder:
	@ mkdir -p $(OUT_DIR)

birthday_party: create_build_folder
	@ $(CXX) \
		./src/birthday_party.cpp \
		./src/concurrent_linked_list.cpp \
		-o $(OUT_DIR)/birthday_party \
		$(CXXFLAGS) \
		$(FLAGS)

temperature: create_build_folder
	@ mkdir -p out
	@ $(CXX) ./src/temperature.cpp -o $(OUT_DIR)/temperature $(CXXFLAGS) $(FLAGS)

clean:
	@ rm -rf $(OUT_DIR)
