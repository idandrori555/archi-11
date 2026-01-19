# Compiler
WCC := i686-w64-mingw32-g++

# Flags
FLAGS := -Wall -static

# Build dir
BUILD_DIR := build

all:
	@echo "Available tasks:"
	@echo "  task1 - Compile task1"
	@echo "  task2 - Compile task2"
	@echo "  clean - Clean build dir"

# Ensure build dir exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Task 1
task1: $(BUILD_DIR)
	@echo "Building task1..."
	$(WCC) $(FLAGS) ./ex10-cs.cpp -o $(BUILD_DIR)/task1.exe

# Task 2
task2: $(BUILD_DIR)
	@echo "Building task2..."
	$(WCC) $(FLAGS) ./ex10-philosopher.cpp -o $(BUILD_DIR)/philosopher.exe
	$(WCC) $(FLAGS) ./ex10-runner.cpp -o $(BUILD_DIR)/runner.exe

clean:
	rm -rf $(BUILD_DIR)

.PHONY: task1 task2 clean all

