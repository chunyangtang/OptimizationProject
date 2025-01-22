# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17

# Source files
SRCS = $(wildcard *.cpp)

# Object files
OBJS = $(patsubst %.cpp,build/%.o,$(SRCS))

# Executable
EXEC = main

# Build directory
BUILD_DIR = build

# Default target
all: $(BUILD_DIR) $(EXEC)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link object files to create executable
$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC)

# Compile source files to object files
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Collect data during execution
collect_data: CXXFLAGS += -DENABLE_DATA_COLLECTION
collect_data: all

# Clean build files and executable
clean:
	rm -rf $(BUILD_DIR) $(EXEC)

# Clean data files
clean_data:
	rm -rf results_*/ *.png

# Phony targets
.PHONY: all collect_data clean clean_data