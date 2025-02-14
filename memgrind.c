/*1. malloc() and immediately free() a 1 - byte object, 120 times.
 *2. Use malloc() to get 120 1 - byte objects, storing the pointers in an array, then use free() to
 *deallocate the chunks.
 *3. Create an array of 120 pointers.Repeatedly make a random choice between(a) allocating a
 *1 - byte object and adding the pointer to the array and (b)deallocating a previously allocated
 *object(if any).Once 120 allocations have been performed, deallocate all objects.
 * The remaining two are for you to design, but should attempt to simulate a program with non-trivial
 *dynamic memory use. (For example, manipulating a linked list or binary tree.) To avoid running
 *out of memory, each task should free all heap objects it created.
 */

#include "mymalloc.h"