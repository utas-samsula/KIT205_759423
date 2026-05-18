#ifndef PROTO1_H
#define PROTO1_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

// Prototype 1: Array of Cars, each car has a BST of owners
// Good for: fixed/known number of cars, fast indexed access
// Search for car: O(n) linear scan of array
// Insert owner: O(log n) BST insert

#define MAX_CARS 100000
#define MAX_ID_LEN 20

typedef struct car_p1 {
    char car_id[MAX_ID_LEN];
    BST owners;   // BST of owner IDs for this car
    int active;   // 1 if slot is used
} CarP1;

typedef struct database_p1 {
    CarP1 *cars;
    int count;
    int capacity;
} DatabaseP1;

DatabaseP1 create_db_p1(int capacity);
void destroy_db_p1(DatabaseP1 *db);

// core operations
void register_car_p1(DatabaseP1 *db, const char *car_id);
void add_owner_p1(DatabaseP1 *db, const char *car_id, int owner_id);
void print_all_cars_p1(DatabaseP1 *db);
void print_owners_of_car_p1(DatabaseP1 *db, const char *car_id);
void print_cars_of_owner_p1(DatabaseP1 *db, int owner_id);

// test function
void test_proto1();

#endif
