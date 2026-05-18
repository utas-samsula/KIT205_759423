#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>

// AVL tree node - stores owner_id, tracks height for balancing
typedef struct avlNode {
    int owner_id;
    int height;
    struct avlNode *left;
    struct avlNode *right;
} *AVLNodePtr;

typedef struct avl {
    AVLNodePtr root;
} AVL;

AVL new_avl();
void insert_avl(AVL *self, int owner_id);
int find_avl(AVL *self, int owner_id);
void print_avl_inorder(AVL *self);
void destroy_avl(AVL *self);
int count_avl(AVL *self);

#endif
