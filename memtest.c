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

    if (argc < 2) {
        perror("Choice not found");
        return EXIT_FAILURE;
    }
    int choice = atoi(argv[1]);

    switch (choice) {
        // test whether mymalloc allocates memory without overlapping, whether
        // free deallocates memory, and whether leak_detector works well.
        case 1: {
            char *obj[OBJECTS];
            int i, j, errors = 0;
            
            // fill memory with objects
            for (i = 0; i < OBJECTS; i++) {
               obj[i] = malloc(OBJSIZE);
               if (obj[i] == NULL) {
                   printf("Unable to allocate object %d\n", i);
                   return EXIT_FAILURE;
               }
            }
            
            // fill each object with distinct bytes
            for (i = 0; i < OBJECTS; i++) {
                memset(obj[i], i, OBJSIZE);
            }
       
            // check that all objects contain the correct bytes
            for (i = 0; i < OBJECTS; i++) {
                for (j = 0; j < OBJSIZE; j++) {
                    if (obj[i][j] != i) {
                        errors++;
                        printf("Object %d byte %d incorrect: %d\n", i, j, obj[i][j]);
                    }
                }
            }
        
            // free all objects
            if (!LEAK) {
                for (i = 0; i < OBJECTS; i++) {
                   free(obj[i]);
                }
            }

            if (errors > 0) {
                printf("%d incorrect bytes\n", errors);
                return EXIT_FAILURE;
            }

            return EXIT_SUCCESS;
        }

        // test whether mymalloc coalesce adjecent free chunks.
        case 2: {
            char *obj[OBJECTS];
            int i;

            for (i = 0; i < OBJECTS; i++) {
                obj[i] = malloc(OBJSIZE);
            }

            // free doesn't coalesce previous free chunks here (only coalesce latter ones).
            for (i = 0; i < OBJECTS; i++) {
                free(obj[i]);
                obj[i] = NULL;
            }

            obj[0] = malloc(OBJSIZE * OBJECTS);
            if (obj[0] == NULL) {
                perror("Mymalloc fail to coalesce adjacent free chunks");
                return EXIT_FAILURE;
            }

            free(obj[0]);

            return EXIT_SUCCESS;
        }

        // test whether myfree coalesce adjacent free chunks.
        case 3: {
            char *obj[OBJECTS];
            int i;

            for (i = 0; i < OBJECTS; i++) {
                obj[i] = malloc(OBJSIZE);
            }

            // free coalesce latter free chunks here.
            for (i = OBJECTS - 1; i >= 0; i--) {
                free(obj[i]);
                obj[i] = NULL;
            }

            obj[0] = malloc(OBJSIZE * OBJECTS);
            if (obj[0] == NULL) {
                perror("Myfree fail to coalesce adjacent free chunks");
                return EXIT_FAILURE;
            }

            free(obj[0]);

            return EXIT_SUCCESS;
        }

        // test whether mymalloc detects requests larger than available memory.
        case 4: {
            void *ptr1 = malloc(MEMSIZE - HEADERSIZE + 1);
            void *ptr2 = malloc(MEMSIZE); 
            void *ptr3 = malloc(0); 

            if (ptr1 == NULL && ptr2 == NULL && ptr3 == NULL) {
                return EXIT_SUCCESS;
            } else {
                perror("mymalloc failed to detect excessive allocation requests");
                return EXIT_FAILURE;
            }
        }


        // test whether mymalloc detects normal unallocatable cases.
        case 5: {
            char *obj[OBJECTS + 1];
            int i;

            for (i = 0; i < OBJECTS + 1; i++) {
                obj[i] = malloc(OBJSIZE);
            }

            for (i = 0; i < OBJECTS; i++) {
                free(obj[i]);
                obj[i] = NULL;
            }

            if (obj[OBJECTS] == NULL) {
                return EXIT_SUCCESS;
            } else {
                perror("Wrongly accept unallocatable size");
                return EXIT_FAILURE;
            }
        }

        // test whether myfree detects the error when the address is outside the heap.
        case 6: {
            free(NULL); // this should work.

            int x;
            free(&x);

            return EXIT_SUCCESS;
        }

        // test whether myfree detects the error when the address isn't at the start of payloads.
        case 7: {
            char *ptr = malloc(OBJSIZE);

            free(ptr + 1);

            return EXIT_SUCCESS;
        }

        // test whether myfree detects the error when double free a pointer.
        case 8: {
            char * ptr = malloc(OBJSIZE);
            free(ptr);
            free(ptr);

            return EXIT_SUCCESS;
        }

        default: {
            perror("Wrong input of choice");
            return EXIT_FAILURE;
        }
    }
    
    perror("You shouldn't reach here");
    return EXIT_FAILURE;
 }
 