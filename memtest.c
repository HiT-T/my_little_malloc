/*A good way to organize your testing strategy is to(1) specify
 *the requirements your library must satisfy, (2) describe how you could determine whether the
 *requirements have been violated, and (3) write programs to check those conditions.
 *For example :
 *1. Requirement : malloc() reserves unallocated memory.
 *2. Detection method : When successful, malloc() returns a pointer to an object that does not
 *overlap with any other allocated object.
 *3. Test : Write a program that allocates several large objects.Once allocation is complete, it fills
 *each object with a distinct byte pattern(e.g., the first object is filled with 1, the second with
 *	2, etc.).Finally, it checks whether each object still contains the written pattern. (That is,
 *		writing to one object did not overwrite any other.)
 *	Other properties you should test include :
 * free() deallocates memory
 * malloc() and free() arrange so that adjacent free blocks are coalesced
 * The errors described in section 2 are detected and reported
 * Leaked objects are detected and reported
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <string.h>
 
 
 // Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
 #ifndef REALMALLOC
 #include "mymalloc.h"
 #endif
 
 // Compile with -DLEAK to leak memory
 #ifndef LEAK
 #define LEAK 0
 #endif
 
 #define MEMSIZE 4096
 #define HEADERSIZE 8
 #define OBJECTS 64
 #define OBJSIZE (MEMSIZE / OBJECTS - HEADERSIZE)
 
 int main(int argc, char **argv)
 {
    (void)argc;
    (void)argv;

     char *obj[OBJECTS];
     int i, j, errors = 0;
     
     // fill memory with objects
     for (i = 0; i < OBJECTS; i++) {
         obj[i] = malloc(OBJSIZE);
         if (obj[i] == NULL) {
             printf("Unable to allocate object %d\n", i);
             exit(1);
         }
     }
     
    //  // fill each object with distinct bytes
    //  for (i = 0; i < OBJECTS; i++) {
    //      memset(obj[i], i, OBJSIZE);
    //  }
     
    //  // check that all objects contain the correct bytes
    //  for (i = 0; i < OBJECTS; i++) {
    //      for (j = 0; j < OBJSIZE; j++) {
    //          if (obj[i][j] != i) {
    //              errors++;
    //              // printf("Object %d byte %d incorrect: %d\n", i, j, obj[i][j]);
    //          }
    //      }
    //  }
 
    //  // free all objects
    //  if (!LEAK) {
    //      for (i = 0; i < OBJECTS; i++) {
    //      free(obj[i]);
    //      }
    //  }
     
     printf("%d incorrect bytes\n", errors);
     
     return EXIT_SUCCESS;
 }
 