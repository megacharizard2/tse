CFLAGS = -Wall -pedantic -std=c11 -ggdb -I.
CC = gcc
MAKE = make

OFILES= queue.o hash.o webpage.o tools.o indexio.o locked-queue.o lqtest1 lqtest2 locked-hash.o lhashtest1 lhashtest2 lhashtest3

all: $(OFILES)
	ar cr ../lib/libutils.a $(OFILES)

webpage.o: webpage.h
tools.o: tools.h
queue.o: queue.h
hash.o: hash.h
locked-queue.o: locked-queue.h
locked-hash.o: locked-hash.h
indexio.o: indexio.h webpage.h tools.h hash.h queue.h

locked-queue.o: locked-queue.c locked-queue.h queue.h
			gcc $(CFLAGS) -c locked-queue.c

locked-hash.o: locked-hash.c locked-hash.h hash.h
			gcc $(CFLAGS) -c locked-hash.c

lqtest1.o: lqtest1.c locked-queue.h
	gcc $(CFLAGS) -c lqtest1.c

lqtest2.o: lqtest2.c locked-queue.h
	gcc $(CFLAGS) -c lqtest2.c  

lqtest1: lqtest1.o locked-queue.o
	gcc $(CFLAGS) lqtest1.o locked-queue.o queue.o -o lqtest1

lqtest2: lqtest2.o locked-queue.o
	gcc $(CFLAGS) lqtest2.o locked-queue.o queue.o -o lqtest2 

lhashtest1.o: lhashtest1.c locked-hash.h
	gcc $(CFLAGS) -c lhashtest1.c

lhashtest1: lhashtest1.o locked-hash.o
	gcc $(CFLAGS) lhashtest1.o locked-hash.o hash.o queue.o -o lhashtest1

lhashtest2.o: lhashtest2.c locked-hash.h
	gcc $(CFLAGS) -c lhashtest2.c

lhashtest2: lhashtest2.o locked-hash.o
	gcc $(CFLAGS) lhashtest2.o locked-hash.o hash.o queue.o -o lhashtest2

lhashtest3.o: lhashtest3.c locked-hash.h
	gcc $(CFLAGS) -c lhashtest3.c

lhashtest3: lhashtest3.o locked-hash.o
	gcc $(CFLAGS) lhashtest3.o locked-hash.o hash.o queue.o -o lhashtest3

clean:
	rm -rf webpage.o tools.o queue.o hash.o indexio.o locked-queue.o lqtest1.o lhashtest1.o lhashtest2.o lqtest1 lqtest2 lqtest2.o lhashtest3.o lhashtest3 lhashtest1 lhashtest2 *~

