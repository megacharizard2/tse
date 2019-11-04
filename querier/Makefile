L = ../lib
B = ../utils

CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I$L -I$B
PROG = querier
OBJS = querier.o
LIBS =
LLIBS = $L/libutils.a -lcurl

.PHONY: all clean

all: $(PROG) 

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

querier.o: $B/queue.h $B/hash.h $B/webpage.h

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM
