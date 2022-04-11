CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wtype-limits -Wextra -lpthread
OUT_DIR = ./out

birthday_party:
	@ mkdir -p out
	@ $(CXX) \
		./src/birthday_party.cpp \
		./src/concurrent_linked_list.cpp \
		$(CXXFLAGS) \
		-o $(OUT_DIR)/birthday_party

clean:
	@ rm -rf $(OUT_DIR)
