#ifndef PROTO2_H
#define PROTO2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

// Prototype 2: Linked List of Cars, each car has an AVL tree of owners
// Good for: dynamic/unknown number of cars, balanced O(log n) owner lookup
// Search for car: O(n) linked list traversal
// Insert owner: O(log n) AVL insert with guaranteed balance

#define MAX_ID_LEN2 20

typedef struct car_node {
    char car_id[MAX_ID_LEN2];
    AVL owners;   // AVL tree of owner IDs
    struct car_node *next;
} CarNode;

typedef struct database_p2 {
    CarNode *head;
    int count;
} DatabaseP2;

DatabaseP2 create_db_p2();
void destroy_db_p2(DatabaseP2 *db);

// core operations
void register_car_p2(DatabaseP2 *db, const char *car_id);
void add_owner_p2(DatabaseP2 *db, const char *car_id, int owner_id);
void print_all_cars_p2(DatabaseP2 *db);
void print_owners_of_car_p2(DatabaseP2 *db, const char *car_id);
void print_cars_of_owner_p2(DatabaseP2 *db, int owner_id);

// test function
void test_proto2();

#endif
