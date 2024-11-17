# project_provip/Makefile

.PHONY: all clean libeventloop libcapwap example_agent_server

all: libeventloop libcapwap example_agent_server

# libeventloop:
# 	$(MAKE) -C libeventloop/src

libcapwap: libeventloop
	$(MAKE) -C libcapwap/src

example_agent_server: libcapwap libeventloop
	$(MAKE) -C example_agent_server/src

clean:
	$(MAKE) -C libcapwap/src clean
	# $(MAKE) -C libeventloop/src clean
	$(MAKE) -C example_agent_server/src clean
