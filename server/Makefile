# Makefile for server

# Compiler and flags
CC = gcc
CFLAGS = -I../lib_management/lib_capwap/include -I../lib_management/lib_eloop/inc

# Directories
SRC_DIR = src
BUILD_DIR = build
LIB_CAPWAP_DIR = ../lib_management/lib_capwap/build
LIB_ELOOP_DIR = ../lib_management/lib_eloop/build
INC_DIR = inc

# Output binary
TARGET = server

# Source files
SRCS = $(SRC_DIR)/server.c
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Libraries
LIBS = -L$(LIB_CAPWAP_DIR) -lcapwap -L$(LIB_ELOOP_DIR) -leloop

# Build rules
all: $(TARGET)

# Compile server
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
