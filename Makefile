# Makefile in the root directory of the project

.PHONY: all clean server agent

# Default target: Build both server and agent
all: server agent

# Build server
server:
	@echo "Building Server..."
	$(MAKE) -C server

# Build agent
agent:
	@echo "Building Agent..."
	$(MAKE) -C agent

# Clean both server and agent
clean:
	@echo "Cleaning Server..."
	$(MAKE) -C server clean
	@echo "Cleaning Agent..."
	$(MAKE) -C agent clean
