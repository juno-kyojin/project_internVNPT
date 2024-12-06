CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lm

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/libcapwap.a

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

AC_LISTENER = $(BIN_DIR)/ac_listener
WTP_SENDER = $(BIN_DIR)/wtp_sender

AC_LISTENER_SRC = tests/ac_listener.c
WTP_SENDER_SRC = tests/wtp_sender.c

all: $(TARGET) $(AC_LISTENER) $(WTP_SENDER)

$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	ar rcs $@ $^

# Compile test executables and link with libcapwap.a
$(AC_LISTENER): $(AC_LISTENER_SRC) $(TARGET)
	$(CC) $(CFLAGS) $< -o $@ -L$(BIN_DIR) -lcapwap $(LDFLAGS)

$(WTP_SENDER): $(WTP_SENDER_SRC) $(TARGET)
	$(CC) $(CFLAGS) $< -o $@ -L$(BIN_DIR) -lcapwap $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
