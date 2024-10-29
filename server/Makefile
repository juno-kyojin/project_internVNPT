CC = gcc
CFLAGS = -Wall -Iinc -I/home/tuitachi/hust082022/lib/libeventloop/inc -Imanagement_system/log/inc
SRCDIR = src
OBJDIR = obj
BINDIR = bin
LOGDIR = management_system/log

# List of source files
SOURCES = $(SRCDIR)/server_main.c $(SRCDIR)/server_socket.c $(SRCDIR)/database.c management_system/log/src/log.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/src/%.o)

# ELOOP sources and objects
ELOOP_SOURCES = /home/tuitachi/hust082022/lib/libeventloop/src/eloop.c /home/tuitachi/hust082022/lib/libeventloop/src/common.c /home/tuitachi/hust082022/lib/libeventloop/src/os_unix.c /home/tuitachi/hust082022/lib/libeventloop/src/trace.c
ELOOP_OBJECTS = $(ELOOP_SOURCES:/home/tuitachi/hust082022/lib/libeventloop/src/%.c=$(OBJDIR)/eloop/%.o)

# Executable name
EXECUTABLE = $(BINDIR)/server

# Create the executable
$(EXECUTABLE): $(OBJECTS) $(ELOOP_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) $(ELOOP_OBJECTS) -o $@

# Compile source files
$(OBJDIR)/src/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile ELOOP files
$(OBJDIR)/eloop/%.o: /home/tuitachi/hust082022/lib/libeventloop/src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: clean
