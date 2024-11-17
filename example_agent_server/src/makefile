# example_agent_server/src/makefile

CC = gcc
CFLAGS = -g -I../../libcapwap/inc -I../../libeventloop/inc -Wall -Wextra -O2
LIBS = ../../libcapwap/obj/libcapwap.a ../../libeventloop/src/obj/libeloop.a -lssl -lcrypto

TARGETS = agent server

all: $(TARGETS)

agent: agent.o
	$(CC) -o ../obj/agent ../obj/agent.o $(LIBS)

server: server.o
	$(CC) -o ../obj/server ../obj/server.o $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o ../obj/$@

clean:
	rm -f ../obj/*.o ../obj/agent ../obj/server
