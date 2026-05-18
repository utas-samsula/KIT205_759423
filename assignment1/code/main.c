/*
 * KIT205 Assignment 1 - Car Ownership Database
 * Student: Samsul Alam (759423)
 *
 * Two prototypes for a car ownership database (many-to-many relationship):
 *   Prototype 1: Array of Cars + BST of Owners per car
 *   Prototype 2: Linked List of Cars + AVL Tree of Owners per car
 *
 * Many-to-many: a car can have multiple owners (ownership history),
 *               an owner can own multiple cars.
 *
 * Resources used:
 *   - KIT205 tutorial materials (linked list, BST, AVL implementations)
 *   - AVL rotation logic adapted from UTAS lecture notes
 *   - Claude AI used for initial scaffolding and debugging assistance
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "proto1.h"
#include "proto2.h"

// ---- performance evaluation ----

static long time_insert_p1(int n_owners) {
    DatabaseP1 db = create_db_p1(5);
    register_car_p1(&db, "TESTCAR");
    clock_t start = clock();
    for (int j = 1; j <= n_owners; j++) {
        add_owner_p1(&db, "TESTCAR", j); // ascending = BST worst case
    }
    long msec = (clock() - start) * 1000 / CLOCKS_PER_SEC;
    destroy_db_p1(&db);
    return msec;
}

static long time_insert_p2(int n_owners) {
    DatabaseP2 db = create_db_p2();
    register_car_p2(&db, "TESTCAR");
    clock_t start = clock();
    for (int j = 1; j <= n_owners; j++) {
        add_owner_p2(&db, "TESTCAR", j); // ascending = AVL keeps balanced
    }
    long msec = (clock() - start) * 1000 / CLOCKS_PER_SEC;
    destroy_db_p2(&db);
    return msec;
}

static long time_lookup_p1(int n_owners) {
    DatabaseP1 db = create_db_p1(5);
    for (int j = 1; j <= n_owners; j++) add_owner_p1(&db, "TESTCAR", j);
    int target = n_owners / 2;
    int idx = 0;
    for (int k = 0; k < db.capacity; k++) {
        if (db.cars[k].active) { idx = k; break; }
    }
    clock_t start = clock();
    for (int rep = 0; rep < 10000; rep++) {
        find_bst(&db.cars[idx].owners, target);
    }
    long msec = (clock() - start) * 1000 / CLOCKS_PER_SEC;
    destroy_db_p1(&db);
    return msec;
}

static long time_lookup_p2(int n_owners) {
    DatabaseP2 db = create_db_p2();
    for (int j = 1; j <= n_owners; j++) add_owner_p2(&db, "TESTCAR", j);
    int target = n_owners / 2;
    clock_t start = clock();
    for (int rep = 0; rep < 10000; rep++) {
        find_avl(&db.head->owners, target);
    }
    long msec = (clock() - start) * 1000 / CLOCKS_PER_SEC;
    destroy_db_p2(&db);
    return msec;
}

void run_evaluation() {
    printf("\n====== Performance Evaluation ======\n");
    printf("Domain: Car Ownership Database\n");
    printf("Operation tested: insert owner (ascending IDs = BST worst case)\n\n");

    printf("--- Insert Performance ---\n");
    printf("%-15s %-22s %-22s\n", "n owners", "Proto1 Array+BST(ms)", "Proto2 List+AVL(ms)");
    printf("--------------------------------------------------------------\n");

    int sizes[] = {1000, 5000, 10000, 50000, 100000};
    int n = 5;
    for (int i = 0; i < n; i++) {
        long t1 = time_insert_p1(sizes[i]);
        long t2 = time_insert_p2(sizes[i]);
        printf("%-15d %-22ld %-22ld\n", sizes[i], t1, t2);
    }

    printf("\n--- Lookup Performance (100,000 repeated lookups, middle value) ---\n");
    printf("%-15s %-22s %-22s\n", "n owners", "Proto1 BST (ms)", "Proto2 AVL (ms)");
    printf("--------------------------------------------------------------\n");

    int lsizes[] = {100, 1000, 10000, 100000};
    int ln = 4;
    for (int i = 0; i < ln; i++) {
        long t1 = time_lookup_p1(lsizes[i]);
        long t2 = time_lookup_p2(lsizes[i]);
        printf("%-15d %-22ld %-22ld\n", lsizes[i], t1, t2);
    }

    printf("\nKey finding: BST degrades with sorted input (O(n) height).\n");
    printf("AVL tree maintains O(log n) height via rotations.\n");
    printf("Both have O(n) car lookup (array scan vs list traversal).\n");
}

// ---- demo database ----

void run_demo() {
    printf("\n====== Demo: Car Ownership Database ======\n\n");

    // prototype 1 demo
    DatabaseP1 db1 = create_db_p1(1000);
    add_owner_p1(&db1, "TOYOTA-001", 1001);
    add_owner_p1(&db1, "TOYOTA-001", 1003);
    add_owner_p1(&db1, "TOYOTA-001", 1002);
    add_owner_p1(&db1, "BMW-M3-002", 1002);
    add_owner_p1(&db1, "BMW-M3-002", 1005);
    add_owner_p1(&db1, "HONDA-003", 1001);
    add_owner_p1(&db1, "HONDA-003", 1004);
    add_owner_p1(&db1, "FORD-004",  1003);

    printf("Prototype 1 Demo:\n");
    print_all_cars_p1(&db1);
    printf("\n");
    print_owners_of_car_p1(&db1, "TOYOTA-001");
    printf("\n");
    print_cars_of_owner_p1(&db1, 1001);
    printf("\n");
    print_cars_of_owner_p1(&db1, 1002);
    destroy_db_p1(&db1);

    printf("\n");

    // prototype 2 demo
    DatabaseP2 db2 = create_db_p2();
    add_owner_p2(&db2, "TOYOTA-001", 1001);
    add_owner_p2(&db2, "TOYOTA-001", 1003);
    add_owner_p2(&db2, "TOYOTA-001", 1002);
    add_owner_p2(&db2, "BMW-M3-002", 1002);
    add_owner_p2(&db2, "BMW-M3-002", 1005);
    add_owner_p2(&db2, "HONDA-003", 1001);
    add_owner_p2(&db2, "HONDA-003", 1004);
    add_owner_p2(&db2, "FORD-004",  1003);

    printf("Prototype 2 Demo:\n");
    print_all_cars_p2(&db2);
    printf("\n");
    print_owners_of_car_p2(&db2, "TOYOTA-001");
    printf("\n");
    print_cars_of_owner_p2(&db2, 1001);
    destroy_db_p2(&db2);
}

int main() {
    printf("KIT205 Assignment 1 - Car Ownership Database\n");
    printf("Student: Samsul Alam (759423)\n");
    printf("Prototype 1: Array + BST\n");
    printf("Prototype 2: Linked List + AVL Tree\n");
    printf("============================================\n");

    // run all unit tests first
    test_proto1();
    test_proto2();

    // demo with real car data
    run_demo();

    // performance evaluation
    run_evaluation();

    printf("\n============================================\n");
    printf("All tests and evaluation complete.\n");

    return 0;
}
