#include <cstddef>

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
	int size;
	int allocated;
};

struct node* first_header = (struct node*)heap.bytes;

static int not_initialized = 2;

static void leak_detection() {

	// print out the number of leaked (allocated but unfree) chunks in heap.
}

static void initialize() {
	atexit(leak_detection);

	
}

void * mymalloc(size_t size, char* file, int line) {
	if (not_initialized) {
		initialize();
	}

	// error detection

	for (struct node* current_header = first_header; current_header < heap.bytes + MEMLENGTH; current_header += current_header->size) {
		if (current_header >= size) {
			// ���С
			return (current_header + 8);
		} 
		else if (current_header) {
			// С�۴�
		}
		else {
			printf("Unable to allocate %d bytes (%s %d)", size, file, line);
			exit(2);
		}
	}

	// (size + 7) & 7 instead of size + (8 - size % 8) to accelerate the code.

}

void   myfree(void* ptr, char* file, int line) {
	if (not_initialized) {
		initialize();
	}
}