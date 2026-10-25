CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

SRC_DIR  = src
TEST_DIR = tests

# --- Default: build player from src/ ---
all: player

player: $(SRC_DIR)/Track.cc $(SRC_DIR)/Playlist.cc $(SRC_DIR)/main.cc
	$(CXX) $(CXXFLAGS) -I $(SRC_DIR) $^ -o $@

# --- Clean ---
clean:
	rm -f player
	rm -rf tests/output/bin tests/output/obj tests/output/classes

.PHONY: all clean
