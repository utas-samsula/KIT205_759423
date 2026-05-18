#include "bst.h"

BST new_bst() {
    BST t;
    t.root = NULL;
    return t;
}

static BSTNodePtr insert_node(BSTNodePtr self, int id) {
    if (self == NULL) {
        self = malloc(sizeof *self);
        self->owner_id = id;
        self->left = self->right = NULL;
    } else if (id < self->owner_id) {
        self->left = insert_node(self->left, id);
    } else if (id > self->owner_id) {
        self->right = insert_node(self->right, id);
    }
    // duplicates ignored
    return self;
}

void insert_bst(BST *self, int owner_id) {
    self->root = insert_node(self->root, owner_id);
}

static int find_node(BSTNodePtr self, int id) {
    if (self == NULL) return 0;
    if (id == self->owner_id) return 1;
    if (id < self->owner_id) return find_node(self->left, id);
    return find_node(self->right, id);
}

int find_bst(BST *self, int owner_id) {
    return find_node(self->root, owner_id);
}

static void print_inorder(BSTNodePtr self) {
    if (self == NULL) return;
    print_inorder(self->left);
    printf("  Owner %d\n", self->owner_id);
    print_inorder(self->right);
}

void print_bst_inorder(BST *self) {
    print_inorder(self->root);
}

static int count_nodes(BSTNodePtr self) {
    if (self == NULL) return 0;
    return 1 + count_nodes(self->left) + count_nodes(self->right);
}

int count_bst(BST *self) {
    return count_nodes(self->root);
}

static void destroy_nodes(BSTNodePtr self) {
    if (self == NULL) return;
    destroy_nodes(self->left);
    destroy_nodes(self->right);
    free(self);
}

void destroy_bst(BST *self) {
    destroy_nodes(self->root);
    self->root = NULL;
}
