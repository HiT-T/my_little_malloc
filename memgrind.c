 #include <stdio.h>
 #include <stdlib.h>
 #include "mymalloc.h"
 #include <sys/time.h>
 
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
 
    double total_time = 0;

    for (int i = 0; i < NUM_RUNS; i++) {
        gettimeofday(&start, NULL);
        for (int j = 0; j < NUM_TASKS; j++) {
            tasks[j]();
        }
        gettimeofday(&end, NULL);
        total_time += (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
    }

    double avg_time = total_time / NUM_RUNS;
    printf("Workload average time: %.2f microseconds\n", avg_time);
 
    return 0;
 }