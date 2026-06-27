CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
RELEASEFLAGS = -O2
DEBUGFLAGS = -g -O0

BUILD_DIR := build
FILE ?= main.cpp

OUT := $(basename $(FILE))
TARGET := $(BUILD_DIR)/$(OUT)
TARGET_DEBUG := $(BUILD_DIR)/$(OUT)_dbg

.PHONY: build build_debug run debug clean

build:
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) $(FILE) -o $(TARGET)

run: build
	@./$(TARGET)

build_debug:
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) $(FILE) -o $(TARGET_DEBUG)

debug: build_debug
	gdb -tui ./$(TARGET_DEBUG)

clean:
	rm -rf $(BUILD_DIR)

