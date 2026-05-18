#include "avl.h"

AVL new_avl() {
    AVL t;
    t.root = NULL;
    return t;
}

static int height(AVLNodePtr n) {
    if (n == NULL) return -1;
    return n->height;
}

static int max_int(int a, int b) {
    return a > b ? a : b;
}

static void update_height(AVLNodePtr n) {
    if (n != NULL)
        n->height = 1 + max_int(height(n->left), height(n->right));
}

static int balance_factor(AVLNodePtr n) {
    if (n == NULL) return 0;
    return height(n->left) - height(n->right);
}

// right rotation at y
static AVLNodePtr rotate_right(AVLNodePtr y) {
    AVLNodePtr x = y->left;
    AVLNodePtr t2 = x->right;
    x->right = y;
    y->left = t2;
    update_height(y);
    update_height(x);
    return x;
}

// left rotation at x
static AVLNodePtr rotate_left(AVLNodePtr x) {
    AVLNodePtr y = x->right;
    AVLNodePtr t2 = y->left;
    y->left = x;
    x->right = t2;
    update_height(x);
    update_height(y);
    return y;
}

static AVLNodePtr rebalance(AVLNodePtr n) {
    update_height(n);
    int bf = balance_factor(n);

    // left heavy
    if (bf > 1) {
        if (balance_factor(n->left) < 0)
            n->left = rotate_left(n->left); // left-right case
        return rotate_right(n);
    }
    // right heavy
    if (bf < -1) {
        if (balance_factor(n->right) > 0)
            n->right = rotate_right(n->right); // right-left case
        return rotate_left(n);
    }
    return n;
}

static AVLNodePtr insert_node(AVLNodePtr self, int id) {
    if (self == NULL) {
        self = malloc(sizeof *self);
        self->owner_id = id;
        self->height = 0;
        self->left = self->right = NULL;
        return self;
    }
    if (id < self->owner_id)
        self->left = insert_node(self->left, id);
    else if (id > self->owner_id)
        self->right = insert_node(self->right, id);
    // duplicates ignored
    return rebalance(self);
}

void insert_avl(AVL *self, int owner_id) {
    self->root = insert_node(self->root, owner_id);
}

static int find_node(AVLNodePtr self, int id) {
    if (self == NULL) return 0;
    if (id == self->owner_id) return 1;
    if (id < self->owner_id) return find_node(self->left, id);
    return find_node(self->right, id);
}

int find_avl(AVL *self, int owner_id) {
    return find_node(self->root, owner_id);
}

static void print_inorder(AVLNodePtr self) {
    if (self == NULL) return;
    print_inorder(self->left);
    printf("  Owner %d\n", self->owner_id);
    print_inorder(self->right);
}

void print_avl_inorder(AVL *self) {
    print_inorder(self->root);
}

static int count_nodes(AVLNodePtr self) {
    if (self == NULL) return 0;
    return 1 + count_nodes(self->left) + count_nodes(self->right);
}

int count_avl(AVL *self) {
    return count_nodes(self->root);
}

static void destroy_nodes(AVLNodePtr self) {
    if (self == NULL) return;
    destroy_nodes(self->left);
    destroy_nodes(self->right);
    free(self);
}

void destroy_avl(AVL *self) {
    destroy_nodes(self->root);
    self->root = NULL;
}
