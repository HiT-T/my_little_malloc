CC=gcc
CFLAGS=-Wall -Wextra

all: memgrind memtest

memgrind: memgrind.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ $^

memtest: memtest.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f memgrind memtest