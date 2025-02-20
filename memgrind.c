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

 #include <stdio.h>
 #include <stdlib.h>
 #include <sys/time.h>
 #include "mymalloc.h"
 
 #define NUM_TASKS 5
 #define NUM_RUNS 50
 
 void task1() {
     for (int i = 0; i < 120; i++) {
         void *ptr = malloc(1);
         free(ptr);
     }
 }
 
 void task2() {
     void *ptrs[120];
     for (int i = 0; i < 120; i++) {
         ptrs[i] = malloc(1);
     }
     for (int i = 0; i < 120; i++) {
         free(ptrs[i]);
     }
 }
 
 void task3() {
     void *ptrs[120] = {NULL};
     int alloc_count = 0, index = 0;
     while (alloc_count < 120) {
         if (index < 120 && (rand() % 2 || index == 0)) { // Allocate
             ptrs[index] = malloc(1);
             if (ptrs[index]) {
                 alloc_count++;
                 index++;
             }
         } else if (index > 0) { // Free
             index--;
             free(ptrs[index]);
             ptrs[index] = NULL;
         }
     }
     // Free remaining
     for (int i = 0; i < 120; i++) {
         if (ptrs[i]) free(ptrs[i]);
     }
 }
 
 // Task 4: Simulate linked list
 void task4() {
     struct Node {
         int data;
         struct Node *next;
     };
     struct Node *head = NULL;
     for (int i = 0; i < 50; i++) {
         struct Node *node = malloc(sizeof(struct Node));
         node->data = i;
         node->next = head;
         head = node;
     }
     // Free all nodes
     while (head) {
         struct Node *temp = head;
         head = head->next;
         free(temp);
     }
 }
 
 // Task 5: Simulate binary tree
 void task5() {
     struct TreeNode {
         int value;
         struct TreeNode *left, *right;
     };
     struct TreeNode *root = NULL;
     // Build a small tree
     root = malloc(sizeof(struct TreeNode));
     root->left = malloc(sizeof(struct TreeNode));
     root->right = malloc(sizeof(struct TreeNode));
     // Free the tree
     free(root->left);
     free(root->right);
     free(root);
 }
 
 int main() {
     struct timeval start, end;
     void (*tasks[NUM_TASKS])() = {task1, task2, task3, task4, task5};
 
     for (int i = 0; i < NUM_TASKS; i++) {
         gettimeofday(&start, NULL);
         for (int j = 0; j < NUM_RUNS; j++) {
             tasks[i]();
         }
         gettimeofday(&end, NULL);
         double avg_time = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
         avg_time /= NUM_RUNS;
         printf("Task %d average time: %.2f microseconds\n", i+1, avg_time);
     }
 
     return 0;
 }