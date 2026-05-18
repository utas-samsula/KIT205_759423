#include "proto2.h"

DatabaseP2 create_db_p2() {
    DatabaseP2 db;
    db.head = NULL;
    db.count = 0;
    return db;
}

void destroy_db_p2(DatabaseP2 *db) {
    CarNode *current = db->head;
    while (current != NULL) {
        CarNode *to_free = current;
        current = current->next;
        destroy_avl(&to_free->owners);
        free(to_free);
    }
    db->head = NULL;
    db->count = 0;
}

// finds a car node by id, returns NULL if not found
static CarNode *find_car_p2(DatabaseP2 *db, const char *car_id) {
    CarNode *current = db->head;
    while (current != NULL) {
        if (strcmp(current->car_id, car_id) == 0) return current;
        current = current->next;
    }
    return NULL;
}

void register_car_p2(DatabaseP2 *db, const char *car_id) {
    if (find_car_p2(db, car_id) != NULL) return; // already exists

    CarNode *new_car = malloc(sizeof(CarNode));
    strncpy(new_car->car_id, car_id, MAX_ID_LEN2 - 1);
    new_car->car_id[MAX_ID_LEN2 - 1] = '\0';
    new_car->owners = new_avl();
    new_car->next = db->head;
    db->head = new_car;
    db->count++;
}

void add_owner_p2(DatabaseP2 *db, const char *car_id, int owner_id) {
    CarNode *car = find_car_p2(db, car_id);
    if (car == NULL) {
        register_car_p2(db, car_id);
        car = find_car_p2(db, car_id);
    }
    if (car != NULL) {
        insert_avl(&car->owners, owner_id);
    }
}

// collect all car ids and sort them for ordered output
void print_all_cars_p2(DatabaseP2 *db) {
    printf("--- All Cars (Prototype 2: Linked List + AVL) ---\n");
    char **ids = malloc(db->count * sizeof(char *));
    int n = 0;
    CarNode *current = db->head;
    while (current != NULL) {
        ids[n++] = current->car_id;
        current = current->next;
    }
    // sort by car id
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

void print_owners_of_car_p2(DatabaseP2 *db, const char *car_id) {
    CarNode *car = find_car_p2(db, car_id);
    if (car == NULL) {
        printf("Car %s not found\n", car_id);
        return;
    }
    printf("--- Owners of %s (sorted, via AVL inorder) ---\n", car_id);
    print_avl_inorder(&car->owners);
}

// many-to-many: find all cars associated with a given owner
void print_cars_of_owner_p2(DatabaseP2 *db, int owner_id) {
    printf("--- Cars owned by Owner %d (Prototype 2) ---\n", owner_id);
    char **ids = malloc(db->count * sizeof(char *));
    int n = 0;
    CarNode *current = db->head;
    while (current != NULL) {
        if (find_avl(&current->owners, owner_id)) {
            ids[n++] = current->car_id;
        }
        current = current->next;
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
    if (n == 0) printf("  (no cars found)\n");
    free(ids);
}

void test_proto2() {
    printf("\n====== Testing Prototype 2: Linked List + AVL ======\n\n");

    DatabaseP2 db = create_db_p2();

    // test register car
    printf("Test 1: register cars\n");
    register_car_p2(&db, "CAR003");
    register_car_p2(&db, "CAR001");
    register_car_p2(&db, "CAR002");
    register_car_p2(&db, "CAR001"); // duplicate
    printf("Expected: 3 cars\n");
    printf("Got: %d cars\n", db.count);
    printf("Pass: %s\n\n", db.count == 3 ? "YES" : "NO");

    // test add owners
    printf("Test 2: add owners to cars\n");
    add_owner_p2(&db, "CAR001", 200);
    add_owner_p2(&db, "CAR001", 100);
    add_owner_p2(&db, "CAR001", 300);
    add_owner_p2(&db, "CAR001", 100); // duplicate
    add_owner_p2(&db, "CAR002", 100);
    add_owner_p2(&db, "CAR002", 400);
    add_owner_p2(&db, "CAR003", 200);

    CarNode *car = find_car_p2(&db, "CAR001");
    int cnt = count_avl(&car->owners);
    printf("Expected: 3 owners for CAR001\n");
    printf("Got: %d owners\n", cnt);
    printf("Pass: %s\n\n", cnt == 3 ? "YES" : "NO");

    // test AVL balance - insert worst case order for BST (ascending)
    printf("Test 3: AVL stays balanced with ascending inserts\n");
    DatabaseP2 db2 = create_db_p2();
    add_owner_p2(&db2, "TEST", 1);
    add_owner_p2(&db2, "TEST", 2);
    add_owner_p2(&db2, "TEST", 3);
    add_owner_p2(&db2, "TEST", 4);
    add_owner_p2(&db2, "TEST", 5);
    CarNode *test_car = find_car_p2(&db2, "TEST");
    // AVL height should be 2, not 4 (which BST would give)
    int avl_h = test_car->owners.root->height;
    printf("AVL height for 5 sequential inserts: %d (BST worst case would be 4)\n", avl_h);
    printf("Pass: %s\n\n", avl_h <= 2 ? "YES" : "NO");
    destroy_db_p2(&db2);

    // test print all cars ordered
    printf("Test 4: print all cars (should be sorted CAR001, CAR002, CAR003)\n");
    print_all_cars_p2(&db);

    // test print owners (inorder = sorted)
    printf("\nTest 5: print owners of CAR001 (should be 100, 200, 300)\n");
    print_owners_of_car_p2(&db, "CAR001");

    // test many-to-many
    printf("\nTest 6: cars owned by owner 100 (should be CAR001 and CAR002)\n");
    print_cars_of_owner_p2(&db, 100);

    // test owner with no cars
    printf("\nTest 7: cars owned by owner 999 (should be none)\n");
    print_cars_of_owner_p2(&db, 999);

    destroy_db_p2(&db);
    printf("Prototype 2 tests complete.\n");
}
