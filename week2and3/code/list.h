#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct listNode {
    int data;
    struct listNode *next;
} *ListNodePtr;

typedef struct list {
    ListNodePtr head;
} List;

List new_list();
void print_list(List *self);
void insert_at_front(List *self, int data);
void insert_in_order(List *self, int data);
void delete_list(List *self, int data);
void destroy_list(List *self);
void list_test();
void list_adhoc_test();
List reverse(List *self);
List merge(List *list1, List *list2);

#endif