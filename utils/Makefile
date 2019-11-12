CFLAGS = -Wall -pedantic -std=c11 -ggdb -I. 
CC = gcc
MAKE = make

OFILES= queue.o hash.o webpage.o tools.o indexio.o locked-queue.o locked-hash.o

all: $(OFILES)
	ar cr ../lib/libutils.a $(OFILES)

webpage.o: webpage.h
tools.o: tools.h
queue.o: queue.h
hash.o: hash.h
locked-queue.o: locked-queue.c
locked-hash.o: locked-hash.c
indexio.o: indexio.h webpage.h tools.h hash.h queue.h

clean:
	rm -rf locked-hash.o locked-queue.o webpage.o tools.o queue.o hash.o indexio.o *~
