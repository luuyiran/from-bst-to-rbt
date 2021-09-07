#include "bst.h"

void bst_erase(bst_node **root, bst_node *node) {
    bst_node *child, *parent;
    if (NULL == *root || NULL == node) return;

    /* two children */
    if (node->left && node->right) {
        bst_node *left;
        bst_node *old = node;

        node = node->right;
        while ((left = node->left) != NULL)
            node = node->left;
        child = node->right;
        parent = node->parent;
        if (child) child->parent = parent;
        if (parent == old) {
            parent->right = child;
            //parent = node;
        } else {
            parent->left = child;
        }

        /* 右子树的最小节点替换删除位置的节点 */
        node->parent = old->parent;
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
}

bst_node *bst_next(bst_node *node) {
    bst_node *parent;
    if (node == NULL) return NULL;

    if (node->right) {
        node = node->right; 
        while (node->left)
            node=node->left;
        return node;
    }

    /**
     *    r
     *     \
     *      x
     *      \
     *      node
     *      /
     *     T
     */
    while ((parent = node->parent) && node == parent->right)
        node = parent;

    return parent;
}

bst_node *bst_prev(bst_node *node) {
    bst_node *parent;
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

bst_node *bst_first(bst_node *root) {
    if (NULL == root) return NULL;
    while (root->left)
        root = root->left;
    return root;
}

bst_node *bst_last(bst_node *root) {
    if (NULL == root) return NULL;
    while (root->right)
        root = root->right;
    return root;
}


