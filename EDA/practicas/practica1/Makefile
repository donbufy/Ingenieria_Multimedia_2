CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

SRC_DIR  = src
TEST_DIR = tests

# --- Default: build catalog from src/ ---
all: catalog

catalog: $(SRC_DIR)/Resource.cc $(SRC_DIR)/Catalog.cc $(SRC_DIR)/main.cc
	$(CXX) $(CXXFLAGS) -I $(SRC_DIR) $^ -o $@

# --- Clean ---
clean:
	rm -f catalog
	rm -rf tests/output/bin tests/output/obj tests/output/classes

.PHONY: all clean
