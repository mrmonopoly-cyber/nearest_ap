# Compiler settings
CC = gcc
CXX = g++

# Directories
BDIR := build
SDIR := src
PROTOBUF_MESSAGE_PATH := $(SDIR)/nearest_ap/scheduler/messages

# Compiler flags (optional, add as needed)
CC_FLAGS := -Wall -Wextra 
CXX_FLAGS := $(CC_FLAGS) -std=c++17

# Find all .cpp files in the src directory
DEPS_CXX = $(shell find $(SDIR) -follow -type f -name "*.cpp")
DEPS_C += $(shell find $(SDIR) -follow -type f -name "*.c")

# Generate object file paths by replacing .cpp with .o and using build directory
OBJ_CXX = $(DEPS_CXX:$(SDIR)/%.cpp=$(BDIR)/%.o)
OBJ_C += $(DEPS_C:$(SDIR)/%.c=$(BDIR)/%.o)

OBJ := $(OBJ_C) $(OBJ_CXX)

CC_INCLUDE := -I$(PROTOBUF_MESSAGE_PATH)/nanopb_src
CXX_INCLUDE := $(CC_INCLUDE)

# Info for debugging
$(info CXX sources: $(DEPS_CXX))
$(info C sources: $(DEPS_C))

# Default target
all: clean build

setup:
	make -C $(PROTOBUF_MESSAGE_PATH) setup_protobuf
	make -C $(PROTOBUF_MESSAGE_PATH) build_protobuf_messages 

include $(PROTOBUF_MESSAGE_PATH)/Makefile

# Build target
build: $(OBJ)
	@mkdir -p $(BDIR)  # Create the build directory if it doesn't exist
	$(CXX) $(CXX_INCLUDE) $(CXX_FLAGS) $(OBJ) -o main

# Rule to compile each .cpp file into an object (.o)
$(BDIR)/%.o: $(SDIR)/%.cpp
	$(info cpp compiling $<)
	@mkdir -p $(dir $@)  # Create the target directory for the object file
	$(CXX) -c $(CXX_INCLUDE) $(CXX_FLAGS) -o $@ $<

# Rule to compile each .c file into an object (.o)
$(BDIR)/%.o: $(SDIR)/%.c
	$(info c compiling $<)
	@mkdir -p $(dir $@)  # Create the target directory for the object file
	$(CC) -c $(CC_INCLUDE) $(CC_FLAGS) -o $@ $<

clean:
	rm -f main
	rm -rf build

clean_setup:
	make -C $(PROTOBUF_MESSAGE_PATH) clean_setup_protobuf

clean_all_linux:
	make clean
	make clean_setup

clean_all: clean_all_linux
