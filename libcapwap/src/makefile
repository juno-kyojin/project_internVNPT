# libcapwap/src/makefile

CC = gcc
CFLAGS = -g -I../../libcapwap/inc -I../../libeventloop/inc -Wall -Wextra -O2
AR = ar
ARFLAGS = rcs

SRC = capwap.c
OBJ = capwap.o
LIB = ../obj/libcapwap.a

all: $(LIB)

$(LIB): ../obj/$(OBJ)
	$(AR) $(ARFLAGS) $@ $<

../obj/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o ../obj/$@

clean:
	rm -f ../obj/*.o ../obj/libcapwap.a
