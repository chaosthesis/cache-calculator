CC = gcc
CFLAGS = -std=c99 -g -Wall -O3

all : compute_ram
	./compute_ram

compute_ram : compute_ram.o
	$(CC) $(CFLAGS) -o compute_ram compute_ram.o

compute_ram.o :
	$(CC) $(CFLAGS) -c compute_ram.c

clean:
	rm *.o compute_ram
