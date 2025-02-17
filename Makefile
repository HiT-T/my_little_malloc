CFLAGS     = -std=c99 -Wall

build: 
	gcc $(CFLAGS) memgrind.c mymalloc.c -o memgrind && ./memgrind
	gcc $(CFLAGS) memtest.c mymalloc.c -o memtest && ./memtest
clean:
	rm -rf memgrind memtest