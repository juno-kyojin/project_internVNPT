# Makefile tổng cho project

# Compiler flags
CFLAGS = -Ilib_management/lib_capwap/include -Ilib_management/lib_eloop/inc

# Directories
LIB_CAPWAP_DIR = lib_management/lib_capwap
LIB_ELOOP_DIR = lib_management/lib_eloop
AGENT_DIR = agent
SERVER_DIR = server

# Build tất cả
all: lib_capwap lib_eloop agent server

# Build lib_capwap
lib_capwap:
	$(MAKE) -C $(LIB_CAPWAP_DIR)

# Build lib_eloop
lib_eloop:
	$(MAKE) -C $(LIB_ELOOP_DIR)

# Build agent
agent: lib_capwap lib_eloop
	$(MAKE) -C $(AGENT_DIR)

# Build server
server: lib_capwap lib_eloop
	$(MAKE) -C $(SERVER_DIR)

# Clean tất cả các thư mục
clean:
	$(MAKE) -C $(LIB_CAPWAP_DIR) clean
	$(MAKE) -C $(LIB_ELOOP_DIR) clean
	$(MAKE) -C $(AGENT_DIR) clean
	$(MAKE) -C $(SERVER_DIR) clean
	rm -f $(AGENT_DIR)/agent $(SERVER_DIR)/server

.PHONY: all lib_capwap lib_eloop agent server clean
