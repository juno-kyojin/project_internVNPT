# Compiler và các flags
CC = gcc
CFLAGS = -Iinclude
BUILD_DIR = build

# Tên thư viện
TARGET = $(BUILD_DIR)/libcapwap.a

# Danh sách các đối tượng object
OBJS = src/capwap.o

# Rule để build tất cả
all: $(BUILD_DIR) $(TARGET)

# Tạo thư viện tĩnh (.a)
$(TARGET): $(OBJS)
	ar rcs $@ $(OBJS)

# Compile source code thành các file .o
src/capwap.o: src/capwap.c
	$(CC) $(CFLAGS) -c $< -o $@

# Tạo thư mục build nếu chưa tồn tại
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean: Xóa thư mục build và các file .o
clean:
	rm -rf $(BUILD_DIR) src/*.o
