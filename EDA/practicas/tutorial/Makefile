CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

SRC_DIR  = src

# --- Default: build counter from src/ ---
all: counter

counter: $(SRC_DIR)/Counter.cc $(SRC_DIR)/main.cc
	$(CXX) $(CXXFLAGS) -I $(SRC_DIR) $^ -o $@

# --- Clean ---
clean:
	rm -f counter
	rm -rf tests/output/bin

.PHONY: all clean
