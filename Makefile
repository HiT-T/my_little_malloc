CC=gcc
CFLAGS=-Wall -Wextra

all: memgrind memtest

memgrind: memgrind.c mymalloc.c
	$(CC) $(CFLAGS) -o $@ $^

memtest: memtest.c mymalloc.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f memgrind memtest