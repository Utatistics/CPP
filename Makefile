CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
RELEASEFLAGS = -O2
DEBUGFLAGS = -g -O0

BUILD_DIR := build

# All sources
SRC := main.cpp $(wildcard test/*.cpp)

# Single test mode
TEST ?=
TEST_SRC := main.cpp test/test$(TEST).cpp

.PHONY: all build run debug clean

# Build everything
all:
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) \
	$(SRC) \
	-o $(BUILD_DIR)/main


# Build either all or a specific test
build:
	@mkdir -p $(BUILD_DIR)
ifeq ($(TEST),)
	@$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) \
	$(SRC) \
	-o $(BUILD_DIR)/main
else
	@$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) \
	$(TEST_SRC) \
	-o $(BUILD_DIR)/$(shell echo $(TEST) | tr A-Z a-z)
endif


run: build
ifeq ($(TEST),)
	@./$(BUILD_DIR)/main
else
	@./$(BUILD_DIR)/$(shell echo $(TEST) | tr A-Z a-z)
endif


debug:
	@mkdir -p $(BUILD_DIR)
ifeq ($(TEST),)
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) \
	$(SRC) \
	-o $(BUILD_DIR)/main_dbg
	gdb -tui ./$(BUILD_DIR)/main_dbg
else
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) \
	$(TEST_SRC) \
	-o $(BUILD_DIR)/$(shell echo $(TEST) | tr A-Z a-z)_dbg
	gdb -tui ./$(BUILD_DIR)/$(shell echo $(TEST) | tr A-Z a-z)_dbg
endif


clean:
	rm -rf $(BUILD_DIR)