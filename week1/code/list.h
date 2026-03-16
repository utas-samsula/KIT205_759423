#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

// From lecture: self-referential node struct
typedef struct listNode {
    int data;
    struct listNode *next;
} *ListNodePtr;

// From lecture: wrapper struct with head pointer
typedef struct list {
    ListNodePtr head;
} List;

// Function prototypes
List new_list();
void insert_at_front(List *self, int data);
void insert_in_order(List *self, int data);
void delete_list(List *self, int data);
void print_list(List *self);
void destroy_list(List *self);

#endif