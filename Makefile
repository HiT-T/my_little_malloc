CC = gcc
CFLAGS = -Wall -Wextra -g

OBJ_FILES = mymalloc.o
TEST_OBJ_FILES = memtest.o
GRIND_OBJ_FILES = memgrind.o

all: memgrind memtest

memgrind: $(GRIND_OBJ_FILES) $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

memtest: $(TEST_OBJ_FILES) $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c mymalloc.c -o mymalloc.o

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) -c memgrind.c -o memgrind.o

memtest.o: memtest.c mymalloc.h
	$(CC) $(CFLAGS) -c memtest.c -o memtest.o

clean:
	rm -f memgrind memtest *.o
