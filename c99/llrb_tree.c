

#include <stdio.h>

#include "llrb_tree.h"


static inline _Bool llrb_is_red(llrb_node *node) {
    return NULL == node ? 0 : (node->color == LLRB_RED);
}

static inline void llrb_set_red(llrb_node *node) {
    if (node) node->color = LLRB_RED;
}


static size_t __rotates = 0;

/**
 *       parent                       parent
 *        |                            |
 *        y    Right Rotate (y)        x      
 *       / \   ----------------->     / \   
 *      x   T3                      T1   y        
 *     / \      <--------------         / \
 *   T1   T2     Left Rotation(x)     T2   T3
 */
static llrb_node *llrbtree_right_rotate(llrb_node **root, llrb_node *y) {
    if (!y) return NULL;
    llrb_node *x = y->left;
    llrb_node *T2= x->right;
    llrb_node *parent = y->parent;

    x->right = y;
    y->parent = x;

    y->left = T2;
    if (T2) T2->parent = y;

    x->parent = parent;
    if (parent) {
        if (y == parent->right)
            parent->right = x;
        else
            parent->left = x;
    } else
        *root = x;
    
    x->color = y->color;
    llrb_set_red(x->right);

    __rotates++;
    // printf("%s\n", __func__);

    return x;
}

static llrb_node *llrbtree_left_rotate(llrb_node **root, llrb_node *x) {
    if (!x) return NULL;
    llrb_node *y = x->right;
    llrb_node *T2= y->left;
    llrb_node *parent = x->parent;

    y->left = x;
    x->parent = y;

    x->right = T2;
    if (T2) T2->parent = x;

    y->parent = parent;
    if (parent) {
        if (x == parent->left) 
            parent->left = y;
        else 
            parent->right = y;
    } else
        *root = y;
    
    y->color = x->color;
    llrb_set_red(y->left);

    __rotates++;
    // printf("%s\n", __func__);
    return y;
}

static void color_flip(llrb_node *node) {
    if (node) {
        node->color = !(node->color);
        if (node->left)  node->left->color = !(node->left->color);
        if (node->right) node->right->color = !(node->right->color);
        // printf("%s\n", __func__);
    }
}

void llrb_fix_up(llrb_node **root, llrb_node *node) {
    if (NULL == node) return;
    if (llrb_is_red(node->right))
        node = llrbtree_left_rotate(root, node);
    if (node->left && llrb_is_red(node->left) && llrb_is_red(node->left->left))
        node = llrbtree_right_rotate(root, node);

    if (llrb_is_red(node->left) && llrb_is_red(node->right))
        color_flip(node);

    // bottom-up
    llrb_fix_up(root, node->parent);
}

void llrb_erase(llrb_node **root, llrb_node *node) {
    llrb_node *child, *parent;
    if (NULL == node) return;

    /* two children */
    if (node->left && node->right) {
        llrb_node *left;
        llrb_node *old = node;

        node = node->right;
        while ((left = node->left) != NULL)
            node = node->left;
        child = node->right;
        parent = node->parent;
        /* 分离 node, 用于替换 old */
        if (child) child->parent = parent;

        if (parent) {
            if (parent == old) {
                parent->right = child;
                parent = node;
            } else
                parent->left = child;
        } else 
            *root = child;

        /* 右子树的最小节点 node 替换删除位置的节点 old */
        node->parent = old->parent;
        node->color = old->color;
        node->right = old->right;
        node->left = old->left;

        if (old->parent) {
            if (old->parent->left == old)
                old->parent->left = node;
            else
                old->parent->right = node;
        } else
            *root = node; /* old 是根节点 */

        old->left->parent = node;
        if (old->right)
            old->right->parent = node;
    } else { 
        /* no child, or only one */
        child = node->left ? node->left : node->right;
        parent = node->parent;
        if (child)
            child->parent = parent;
        if (parent) {
            if (parent->left == node)
                parent->left = child;
            else
                parent->right = child;
        }
        else
            *root = child;
    }

    llrb_fix_up(root, parent);
}




size_t llrb_rotate_times() { return __rotates; }
void llrb_reset_rotate_times() { __rotates = 0; }

