# Compiler và các flags
CC = gcc
CFLAGS = -I../lib_management/lib_capwap/include -I../lib_management/lib_eloop/inc
LDFLAGS = -L../lib_management/lib_capwap/build -lcapwap -L../lib_management/lib_eloop/build -leloop

# Tên file output
TARGET = agent

# Danh sách các file đối tượng
OBJS = src/agent.o

# Rule để build
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compile các file nguồn thành object files
src/agent.o: src/agent.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(TARGET) $(OBJS)
