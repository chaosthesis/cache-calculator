CC = gcc
CFLAGS = -std=c99 -g -Wall -O3

OBJECTS = measure_cache.o

all : measure_cache
	./measure_cache

measure_cache : $(OBJECTS)
	$(CC) $(CFLAGS) -o measure_cache $(OBJECTS)

measure_cache.o : measure_cache.c
	$(CC) $(CFLAGS) -c measure_cache.c

clean   :
	rm -f *.o measure_cache