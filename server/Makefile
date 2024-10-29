CC = gcc
CFLAGS = -Wall -Iinc -I/home/tuitachi/hust082022/lib/libeventloop/inc -Imanagement_system/log/inc
SRCDIR = src
OBJDIR = obj
BINDIR = bin
ELOOPDIR = /home/tuitachi/hust082022/lib/libeventloop
LOGDIR = management_system/log

# Các tệp nguồn và thư viện của ELOOP
ELOOP_SOURCES = $(ELOOPDIR)/src/eloop.c $(ELOOPDIR)/src/common.c $(ELOOPDIR)/src/os_unix.c $(ELOOPDIR)/src/trace.c
ELOOP_OBJECTS = $(ELOOP_SOURCES:$(ELOOPDIR)/src/%.c=$(OBJDIR)/eloop/%.o)

# Các tệp nguồn của Server
SOURCES = $(SRCDIR)/server_main.c $(SRCDIR)/server_socket.c $(SRCDIR)/database.c $(SRCDIR)/msg_handle.c $(LOGDIR)/src/log.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/src/%.o)
LOG_OBJECT = $(OBJDIR)/log/log.o

# Tên file thực thi
EXECUTABLE = $(BINDIR)/server

# Tạo tệp thực thi
$(EXECUTABLE): $(OBJECTS) $(ELOOP_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) $(ELOOP_OBJECTS) -o $@

# Biên dịch các tệp nguồn của Server
$(OBJDIR)/src/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Biên dịch các tệp nguồn của LOG
$(OBJDIR)/log/%.o: $(LOGDIR)/src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Biên dịch các tệp nguồn của ELOOP
$(OBJDIR)/eloop/%.o: $(ELOOPDIR)/src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean để xóa các tệp đã biên dịch
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: clean
