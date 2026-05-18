#include "proto1.h"

DatabaseP1 create_db_p1(int capacity) {
    DatabaseP1 db;
    db.cars = malloc(capacity * sizeof(CarP1));
    db.count = 0;
    db.capacity = capacity;
    for (int i = 0; i < capacity; i++) {
        db.cars[i].active = 0;
        db.cars[i].owners = new_bst();
    }
    return db;
}

void destroy_db_p1(DatabaseP1 *db) {
    for (int i = 0; i < db->capacity; i++) {
        if (db->cars[i].active) {
            destroy_bst(&db->cars[i].owners);
        }
    }
    free(db->cars);
    db->count = 0;
}

// finds index of a car by id, returns -1 if not found
static int find_car_index_p1(DatabaseP1 *db, const char *car_id) {
    for (int i = 0; i < db->capacity; i++) {
        if (db->cars[i].active && strcmp(db->cars[i].car_id, car_id) == 0) {
            return i;
        }
    }
    return -1;
}

void register_car_p1(DatabaseP1 *db, const char *car_id) {
    if (find_car_index_p1(db, car_id) != -1) return; // already exists
    if (db->count >= db->capacity) {
        printf("p1: array full, cannot add %s\n", car_id);
        return;
    }
    // find first empty slot
    for (int i = 0; i < db->capacity; i++) {
        if (!db->cars[i].active) {
            strncpy(db->cars[i].car_id, car_id, MAX_ID_LEN - 1);
            db->cars[i].car_id[MAX_ID_LEN - 1] = '\0';
            db->cars[i].owners = new_bst();
            db->cars[i].active = 1;
            db->count++;
            return;
        }
    }
}

void add_owner_p1(DatabaseP1 *db, const char *car_id, int owner_id) {
    int idx = find_car_index_p1(db, car_id);
    if (idx == -1) {
        register_car_p1(db, car_id);
        idx = find_car_index_p1(db, car_id);
    }
    if (idx != -1) {
        insert_bst(&db->cars[idx].owners, owner_id);
    }
}

// prints all cars in order (array doesnt store in sorted order so we do a simple sort print)
void print_all_cars_p1(DatabaseP1 *db) {
    printf("--- All Cars (Prototype 1: Array + BST) ---\n");
    // collect active car ids and sort them for ordered output
    char **ids = malloc(db->count * sizeof(char *));
    int n = 0;
    for (int i = 0; i < db->capacity; i++) {
        if (db->cars[i].active) {
            ids[n++] = db->cars[i].car_id;
        }
    }
    // simple selection sort by car id
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            if (strcmp(ids[j], ids[min]) < 0) min = j;
        }
        char *tmp = ids[i]; ids[i] = ids[min]; ids[min] = tmp;
    }
    for (int i = 0; i < n; i++) {
        printf("  Car: %s\n", ids[i]);
    }
    free(ids);
    printf("Total: %d cars\n", db->count);
}

void print_owners_of_car_p1(DatabaseP1 *db, const char *car_id) {
    int idx = find_car_index_p1(db, car_id);
    if (idx == -1) {
        printf("Car %s not found\n", car_id);
        return;
    }
    printf("--- Owners of %s (sorted, via BST inorder) ---\n", car_id);
    print_bst_inorder(&db->cars[idx].owners);
}

// many-to-many: find all cars associated with a given owner
void print_cars_of_owner_p1(DatabaseP1 *db, int owner_id) {
    printf("--- Cars owned by Owner %d (Prototype 1) ---\n", owner_id);
    int found = 0;
    // collect matching car ids
    char **ids = malloc(db->count * sizeof(char *));
    int n = 0;
    for (int i = 0; i < db->capacity; i++) {
        if (db->cars[i].active && find_bst(&db->cars[i].owners, owner_id)) {
            ids[n++] = db->cars[i].car_id;
            found = 1;
        }
    }
    // sort for ordered output
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            if (strcmp(ids[j], ids[min]) < 0) min = j;
        }
        char *tmp = ids[i]; ids[i] = ids[min]; ids[min] = tmp;
    }
    for (int i = 0; i < n; i++) {
        printf("  Car: %s\n", ids[i]);
    }
    if (!found) printf("  (no cars found)\n");
    free(ids);
}

void test_proto1() {
    printf("\n====== Testing Prototype 1: Array + BST ======\n\n");

    DatabaseP1 db = create_db_p1(100);

    // test register car
    printf("Test 1: register cars\n");
    register_car_p1(&db, "CAR003");
    register_car_p1(&db, "CAR001");
    register_car_p1(&db, "CAR002");
    register_car_p1(&db, "CAR001"); // duplicate - should be ignored
    printf("Expected: 3 cars\n");
    printf("Got: %d cars\n", db.count);
    printf("Pass: %s\n\n", db.count == 3 ? "YES" : "NO");

    // test add owners
    printf("Test 2: add owners to cars\n");
    add_owner_p1(&db, "CAR001", 200);
    add_owner_p1(&db, "CAR001", 100);
    add_owner_p1(&db, "CAR001", 300);
    add_owner_p1(&db, "CAR001", 100); // duplicate owner - should be ignored
    add_owner_p1(&db, "CAR002", 100);
    add_owner_p1(&db, "CAR002", 400);
    add_owner_p1(&db, "CAR003", 200);

    int idx = 0;
    // find car001 index
    for (int i = 0; i < db.capacity; i++) {
        if (db.cars[i].active && strcmp(db.cars[i].car_id, "CAR001") == 0) {
            idx = i; break;
        }
    }
    int cnt = count_bst(&db.cars[idx].owners);
    printf("Expected: 3 owners for CAR001\n");
    printf("Got: %d owners\n", cnt);
    printf("Pass: %s\n\n", cnt == 3 ? "YES" : "NO");

    // test print all cars ordered
    printf("Test 3: print all cars (should be sorted CAR001, CAR002, CAR003)\n");
    print_all_cars_p1(&db);

    // test print owners of car (inorder = sorted)
    printf("\nTest 4: print owners of CAR001 (should be 100, 200, 300)\n");
    print_owners_of_car_p1(&db, "CAR001");

    // test many-to-many: owner 100 has CAR001 and CAR002
    printf("\nTest 5: cars owned by owner 100 (should be CAR001 and CAR002)\n");
    print_cars_of_owner_p1(&db, 100);

    // test owner with no cars
    printf("\nTest 6: cars owned by owner 999 (should be none)\n");
    print_cars_of_owner_p1(&db, 999);

    // test add owner to new car (auto-register)
    printf("\nTest 7: add owner to non-existent car (auto-register)\n");
    add_owner_p1(&db, "CAR004", 500);
    printf("Expected: 4 cars\n");
    printf("Got: %d cars\n", db.count);
    printf("Pass: %s\n\n", db.count == 4 ? "YES" : "NO");

    destroy_db_p1(&db);
    printf("Prototype 1 tests complete.\n");
}
