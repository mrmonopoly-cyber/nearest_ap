# Compiler settings
CC = gcc
CXX = g++

# Directories
BDIR := build
SDIR := src
PROTOBUF_MESSAGE_PATH := $(SDIR)/nearest_ap/tasks/messages

# Compiler flags (optional, add as needed)
CXX_FLAGS := -Wall -Wextra -std=c++17

# Find all .cpp files in the src directory
DEPS_CXX = $(shell find $(SDIR) -follow -type f -name "*.cpp")

# Generate object file paths by replacing .cpp with .o and using build directory
OBJ_CXX = $(DEPS_CXX:$(SDIR)/%.cpp=$(BDIR)/%.o)

include $(PROTOBUF_MESSAGE_PATH)/Makefile

# Info for debugging
$(info CXX sources: $(DEPS_CXX))

# Default target
all: build

setup:
	cd $(PROTOBUF_MESSAGE_PATH) && make build_protobuf_messages 


# Build target
build: $(OBJ_CXX)
	@mkdir -p $(BDIR)  # Create the build directory if it doesn't exist
	$(CXX) $(OBJ_CXX) -o main

# Rule to compile each .cpp file into an object (.o)
$(BDIR)/%.o: $(SDIR)/%.cpp
	$(info cpp compiling $<)
	@mkdir -p $(dir $@)  # Create the target directory for the object file
	$(CXX) -c $(CXX_FLAGS) -o $@ $<

clean:
	rm -rf build
	cd $(PROTOBUF_MESSAGE_PATH) && make clean_protobuf_messages 

