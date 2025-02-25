#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <string.h>

#define MEMLENGTH 4096
#define HEADERSIZE 8

// 'static' prevent client code from accessing storage directly.
static union {
	// all chunks of data is made up of bytes, and an array of chars
	//  provides the convenience for accessing each byte in the heap.
	char bytes[MEMLENGTH];

	// ensures begining at an address with 8-byte alignment.
	double not_used;
} heap; 

struct node {
	int payload_size;
	int allocated;
};

struct node *first_header = (struct node *)heap.bytes;

static int not_initialized = 1;

static void leak_detector() {
	int leaked_size = 0;
	int leaked_chunks = 0;

	for (struct node *curr_header = first_header;
		 (char *)curr_header < heap.bytes + MEMLENGTH;
		curr_header = (struct node *)((char *)curr_header + HEADERSIZE + curr_header->payload_size))
   {
	if (curr_header->allocated) {
		leaked_chunks += 1;
		leaked_size += curr_header->payload_size;
	}
   }

   if (leaked_chunks > 0) {
   	fprintf(stderr, "mymalloc: %d bytes leaked in %d objects.\n", leaked_size, leaked_chunks);
   }
}

static void initialize() {
	atexit(leak_detector);
	
	first_header->payload_size = MEMLENGTH - HEADERSIZE;
	first_header->allocated = 0;
	memset((char *)first_header + HEADERSIZE, 0xAA, MEMLENGTH - HEADERSIZE);
	
	not_initialized = 0;
}

void * mymalloc(size_t size, char *file, int line) {
	if (not_initialized) {
		initialize();
	}

	if (size > MEMLENGTH - HEADERSIZE) {
		fprintf(stderr, "malloc: Unable to allocate %zu bytes (%s:%d)\n", size, file, line);
		return NULL;
	}

	if (size == 0) {
		return NULL;
	}

	// calculate the nearest size number which is the multiple of 8 (guarantee alignment).
	int real_size = ((int)size + 7) & ~7;

	for (struct node *curr_header = first_header;
		 (char *)curr_header < heap.bytes + MEMLENGTH;
		 curr_header = (struct node *)((char *)curr_header + HEADERSIZE + curr_header->payload_size))
	{
		if (!curr_header->allocated) {
			int *curr_size = &(curr_header->payload_size);

			if (*curr_size >= real_size) {
				if (*curr_size >= real_size + HEADERSIZE + 8) {
					struct node *new_header = (struct node *)((char *)curr_header + HEADERSIZE + real_size);
					new_header->payload_size = (int)(*curr_size - real_size - HEADERSIZE);
					new_header->allocated = 0;
					*curr_size = real_size;
				} else {
					// doesn't change payload_size cuz don't have space to allocate a new chunk.
				}

				curr_header->allocated = 1;

				return (void *) ((char *)curr_header + HEADERSIZE);
			} 
			else if (*curr_size < real_size) {
				for (struct node *next_header = (struct node *)((char *)curr_header + HEADERSIZE + *curr_size);
				 (char *)next_header < heap.bytes + MEMLENGTH && !next_header->allocated;
				  next_header = (struct node *)((char *)next_header + HEADERSIZE + (next_header->payload_size))) 
				{ *curr_size += HEADERSIZE + (next_header->payload_size); }

				memset((char *)curr_header + HEADERSIZE, 0xAA, *curr_size);
			
				if (*curr_size < real_size) { continue; }
				else if (*curr_size >= real_size + HEADERSIZE + 8) {
					struct node *new_header = (struct node *)((char *)curr_header + HEADERSIZE + real_size);
					new_header->payload_size = (int)(*curr_size - real_size - HEADERSIZE);
					new_header->allocated = 0;
					*curr_size = real_size;
				} else {
					// doesn't change payload_size cuz don't have space to allocate a new chunk.
				}

				curr_header->allocated = 1;

				return (void *) ((char *)curr_header + HEADERSIZE);
			}
		}
	}

	fprintf(stderr, "malloc: Unable to allocate %zu bytes (%s:%d)\n", size, file, line);
	return NULL;
}

void myfree(void *ptr, char *file, int line) {
	if (not_initialized) {
		initialize();
	}
	
	// no operations when free(NULL).
    if (ptr == NULL) {
        return;
    }

    // Check if pointer is within the heap
    if ((char *)ptr < heap.bytes || (char *)ptr >= heap.bytes + MEMLENGTH) {
        fprintf(stderr, "free: Pointer out of heap (%s:%d)\n", file, line);
        exit(2);
    }

    // Get the header by moving back from the payload
    struct node *header = (struct node *)((char *)ptr - HEADERSIZE);
	int *curr_size = &(header->payload_size);

	// check whether header is at the start of a chunk (whether ptr is a the start of a payload).
	for (struct node *curr_header = first_header; ;
		curr_header = (struct node *)((char *)curr_header + HEADERSIZE + curr_header->payload_size))
   	{
		if (curr_header == header) { break; }

		if ((char *)curr_header >= heap.bytes + MEMLENGTH) {
			fprintf(stderr, "free: Invalid pointer (%s:%d)\n", file, line);
			exit(2);
		}
   	}

    if (!header->allocated) {
        fprintf(stderr, "free: Double free detected (%s:%d)\n", file, line);
        exit(2);
    }

	// coalesce all following unallocated chunks.
	for (struct node *next_header = (struct node *)((char *)header + HEADERSIZE + *curr_size);
	 (char *)next_header < heap.bytes + MEMLENGTH && !next_header->allocated;
	  next_header = (struct node *)((char *)next_header + HEADERSIZE + (next_header->payload_size)))
	{ *curr_size += HEADERSIZE + next_header->payload_size; }

	memset((char *)header + HEADERSIZE, 0xAA, *curr_size);
    header->allocated = 0;
}