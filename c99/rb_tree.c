#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "rb_tree.h"


#define rbt_is_red(node)     (RB_RED == (node)->color)
#define rbt_is_black(node)   (RB_BLACK == (node)->color)
#define rbt_set_red(node)    ((node)->color = RB_RED)
#define rbt_set_black(node)  ((node)->color = RB_BLACK)


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
static void rbtree_right_rotate(rb_node **root, rb_node *y) {
    rb_node *x = y->left;
    rb_node *T2= x->right;
    rb_node *parent = y->parent;

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
    
    __rotates++;
    // printf("%s\n", __func__);
}

static void rbtree_left_rotate(rb_node **root, rb_node *x) {
    rb_node *y = x->right;
    rb_node *T2= y->left;
    rb_node *parent = x->parent;

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
    
    __rotates++;
    // printf("%s\n", __func__);
}

void rbt_insert(rb_node **root, rb_node *node) {
    rb_node *parent, *gparent;
    // node->color = RB_RED;
    while ((parent = node->parent) && rbt_is_red(parent)) {
        gparent = parent->parent;

        if (parent == gparent->left) {
            rb_node *uncle = gparent->right;
            /* case 1 */
            if (uncle && rbt_is_red(uncle)) {
                rbt_set_black(uncle);
                rbt_set_black(parent);
                rbt_set_red(gparent);
                node = gparent;
                continue;
            }
            /* case 2 */
            if (parent->right == node) {
                struct rb_node *tmp;
                rbtree_left_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }
            /* case 3 */
            rbt_set_black(parent);
            rbt_set_red(gparent);
            rbtree_right_rotate(root, gparent);
        } else {
            rb_node *uncle = gparent->left;
            if (uncle && rbt_is_red(uncle)) {
                rbt_set_black(uncle);
                rbt_set_black(parent);
                rbt_set_red(gparent);
                node = gparent;
                continue;
            }

            if (parent->left == node) {
                rb_node *tmp;
                rbtree_right_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            rbt_set_black(parent);
            rbt_set_red(gparent);
            rbtree_left_rotate(root, gparent);
        }
    }

    rbt_set_black(*root);
}

static void rbt_erase_fixup(rb_node **root, rb_node *node, rb_node *parent) {
    rb_node *other;
    while ((!node || rbt_is_black(node)) && node != *root) {
        if (parent->left == node) {
            other = parent->right;
            if (rbt_is_red(other)) {
                rbt_set_black(other);
                rbt_set_red(parent);
                rbtree_left_rotate(root, parent);
                other = parent->right;
            } 
            if ((!other->left || rbt_is_black(other->left)) &&
                (!other->right || rbt_is_black(other->right))) {
                rbt_set_red(other);
                node = parent;
                parent = node->parent;
            } else {
                if (!other->right || rbt_is_black(other->right)) {
                    rb_node *o_left;
                    if ((o_left = other->left))
                        rbt_set_black(o_left);
                    rbt_set_red(other);
                    rbtree_right_rotate(root, other);
                    other = parent->right;
                }
                other->color = parent->color;
                rbt_set_black(parent);
                if (other->right)
                    rbt_set_black(other->right);
                rbtree_left_rotate(root, parent);
                node = *root;
                break;
            }
        } else {
            other = parent->left;
            if (rbt_is_red(other)) {
                rbt_set_black(other);
                rbt_set_red(parent);
                rbtree_right_rotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rbt_is_black(other->left)) &&
                (!other->right || rbt_is_black(other->right))) {
                rbt_set_red(other);
                node = parent;
                parent = node->parent;
            } else {
                if (!other->left || rbt_is_black(other->left)) {
                    rb_node *o_right;
                    if ((o_right = other->right))
                        rbt_set_black(o_right);
                    rbt_set_red(other);
                    rbtree_left_rotate(root, other);
                    other = parent->left;
                }
                other->color = parent->color;
                rbt_set_black(parent);
                if (other->left)
                    rbt_set_black(other->left);
                rbtree_right_rotate(root, parent);
                node = *root;
                break;
            }
        }
    }
    if (node)
        rbt_set_black(node);
}

void rbt_erase(rb_node **root, rb_node *node) {
    rb_node *child, *parent;
    int color;

    if (!node->left)
        child = node->right;
    else if (!node->right)
        child = node->left;
    else {
        rb_node *old = node, *left;

        node = node->right;
        while ((left = node->left) != NULL)
            node = node->left;
        child = node->right;
        parent = node->parent;
        color = node->color;

        if (child) child->parent = parent;

        if (parent) {
            if (parent == old) {
                parent->right = child;
                parent = node;
            } else
                parent->left = child;
        } else 
            *root = child;

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
            *root = node;

        old->left->parent = node;
        if (old->right)
            old->right->parent = node;
        goto FIXUP;
    }

    parent = node->parent;
    color = node->color;

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

FIXUP:
    if (color == RB_BLACK) 
        rbt_erase_fixup(root, child, parent);
}

rb_node *rbt_next(rb_node *node) {
    rb_node *parent;
    if (node == NULL) return NULL;

    if (node->right) {
        node = node->right; 
        while (node->left)
            node=node->left;
        return node;
    }
    while ((parent = node->parent) && node == parent->right)
        node = parent;

    return parent;
}

rb_node *rbt_prev(rb_node *node) {
    rb_node *parent;
    if (node == NULL) return NULL;

    if (node->left) {
        node = node->left; 
        while (node->right)
            node=node->right;
        return node;
    }

    while ((parent = node->parent) && node == parent->left)
        node = parent;

    return parent;
}

rb_node *rbt_first(rb_node *root) {
    if (NULL == root) return NULL;
    while (root->left)
        root = root->left;
    return root;
}

rb_node *rbt_last(rb_node *root) {
    if (NULL == root) return NULL;
    while (root->right)
        root = root->right;
    return root;
}

size_t rbt_rotate_times() { return __rotates; }
void rbt_reset_rotate_times() { __rotates = 0; }
