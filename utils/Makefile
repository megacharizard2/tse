CFLAGS = -Wall -pedantic -std=c11 -ggdb -I.
CC = gcc
MAKE = make

OFILES= queue.o hash.o webpage.o tools.o indexio.o locked-queue.o

all: $(OFILES)
	ar cr ../lib/libutils.a $(OFILES)

webpage.o: webpage.h
tools.o: tools.h
queue.o: queue.h
hash.o: hash.h
locked-queue.o: locked-queue.c
indexio.o: indexio.h webpage.h tools.h hash.h queue.h

locked-queue.o: locked-queue.c locked-queue.h queue.h
			gcc $(CFLAGS) -c locked-queue.c

lqtest1.o: lqtest1.c locked-queue.h
	gcc $(CFLAGS) -c lqtest1.c

lqtest2.o: lqtest2.c locked-queue.h
	gcc $(CFLAGS) -c lqtest2.c  

lqtest1: lqtest1.o locked-queue.o
	gcc $(CFLAGS) lqtest1.o locked-queue.o queue.o -o lqtest1

lqtest2: lqtest2.o locked-queue.o
	gcc $(CFLAGS) lqtest2.o locked-queue.o queue.o -o lqtest2 

clean:
	rm -rf webpage.o tools.o queue.o hash.o indexio.o locked-queue.o lqtest1.o  *~

