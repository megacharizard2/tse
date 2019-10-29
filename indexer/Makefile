L = ../lib
B = ../utils

CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I$L -I$B
PROG = indexer
OBJS = indexer.o
PROG2= indexload
OBJS2= indexload.o
LIBS = 
LLIBS = $L/libutils.a -lcurl

.PHONY: all clean

all: $(PROG) $(PROG2)

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(PROG2): $(OBJS2) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indexer.o: $B/queue.h $B/hash.h $B/webpage.h 
indexload.o: $B/queue.h $B/hash.h $B/webpage.h

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM
