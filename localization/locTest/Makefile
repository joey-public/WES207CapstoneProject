CXX?=g++
CXXFLAGS=-O1


###
# Setup & config
###

BUILD_DIR := build/

# What binary are we trying to build?
BIN := locTest

# C++ source files
CXXSRCS := $(wildcard src/*.cpp)

# Create list of objects to build
OBJS := $(CXXSRCS:src/%.cpp=$(BUILD_DIR)%.o)

# Any flags for the C++ compiler?
CXXFLAGS += -Iinclude

# Libraries
LDFLAGS += -llapack -lblas

###
# Build rules
###

# Convention, `all` is a phony target that builds everything
# and must be listed first
.PHONY: all
all:	$(BIN)

# Binary depends on its objects (use default build rules)
$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BIN) $(LDFLAGS)

$(BUILD_DIR)%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

###
# Convenience targets
###

.PHONY: clean
clean:
	rm -rf build/*
	rm -f $(BIN)
