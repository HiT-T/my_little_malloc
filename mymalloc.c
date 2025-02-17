#include <stdio.h>
#include <string.h>

#define MEMLENGTH 4096

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
	size_t leaked_size = 0;
	int leaked_chunks = 0;

	for (struct node *curr_header = first_header;
		 (char *)curr_header < heap.bytes + MEMLENGTH;
		curr_header = (struct node *)((char *)curr_header + 8 + curr_header->payload_size))
   {
	if (curr_header->allocated) {
		leaked_chunks += 1;
		leaked_size += curr_header->payload_size;
	}
   }

   fprintf(stderr, "mymalloc: %zu bytes leaked in %d objects.\n", leaked_size, leaked_chunks);
}

static void initialize() {
	atexit(leak_detector);
	
	first_header->payload_size = MEMLENGTH - 8;
	first_header->allocated = 0;
	memset((char *)first_header + 8, 0xAA, MEMLENGTH - 8);
	
	not_initialized = 0;
}

void * mymalloc(size_t size, char *file, int line) {
	if (not_initialized) {
		initialize();
	}

	if (size == 0) {
		return NULL;
	}

	size_t real_size = (size + 7) & ~7;

	for (struct node *curr_header = first_header;
		 (char *)curr_header < heap.bytes + MEMLENGTH;
		 curr_header = (struct node *)((char *)curr_header + 8 + curr_header->payload_size))
	{
		int *curr_size = &(curr_header->payload_size);

		if (*curr_size >= real_size) {
			if (*curr_size >= real_size + 16) {
				struct node *new_header = (struct node *)((char *)curr_header + 8 + real_size);
				new_header->payload_size = *curr_size - real_size - 8;
				new_header->allocated = 0;
				*curr_size = real_size;
			} else {
				// doesn't change payload_size cuz don't have space to allocate a new chunk.
			}

			curr_header->allocated = 1;

			return (void *) ((char *)curr_header + 8);
		} 
		else if (*curr_size < real_size) {
			for (struct node *next_header = (struct node *)((char *)curr_header + 8 + *curr_size);
			 (char *)next_header < heap.bytes + MEMLENGTH && next_header->allocated == 0;
			  next_header = (struct node *)((char *)next_header + 8 + (next_header->payload_size))) 
			{ *curr_size += 8 + (next_header->payload_size); }

			memset((char *)curr_header + 8, 0xAA, *curr_size);
			
			if (*curr_size < real_size) { continue; }
			else if (*curr_size >= real_size + 16) {
				struct node *new_header = (struct node *)((char *)curr_header + 8 + real_size);
				new_header->payload_size = *curr_size - real_size - 8;
				new_header->allocated = 0;
				*curr_size = real_size;
			} else {
				// doesn't change payload_size cuz don't have space to allocate a new chunk.
			}

			curr_header->allocated = 1;

			return (void *) ((char *)curr_header + 8);
		}
	}

	fprintf(stderr, "malloc: Unable to allocate %zu bytes (%s:%d)\n", size, file, line);
	return NULL;
}

void   myfree(void *ptr, char *file, int line) {
	if (not_initialized) {
		initialize();
	}
}