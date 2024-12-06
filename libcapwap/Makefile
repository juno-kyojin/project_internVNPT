CC = gcc
CFLAGS = -Wall -Iinclude
AR = ar
ARFLAGS = rcs

LIBNAME = libcapwap.a
TEST_NAME = test_discovery_request
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
OBJ_DIR = obj
BIN_DIR = bin

LIB_SOURCES = capwap.c capwap_control.c capwap_header.c capwap_message_element.c capwap_preamble.c
LIB_SRCS = $(patsubst %,$(SRC_DIR)/%,$(LIB_SOURCES))
LIB_OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(LIB_SRCS))

TEST_SRC = $(TEST_DIR)/$(TEST_NAME).c
TEST_OBJ = $(TEST_DIR)/$(TEST_NAME).o

.PHONY: all clean
all: $(LIBNAME) $(BIN_DIR)/$(TEST_NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBNAME): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/$(TEST_NAME): $(TEST_OBJ) $(LIBNAME) | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -L. -lcapwap -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(LIBNAME) $(TEST_DIR)/*.o $(BIN_DIR)/$(TEST_NAME)
