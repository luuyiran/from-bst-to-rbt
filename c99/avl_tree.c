#include "avl_tree.h"

#ifndef MIN
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#endif 

#ifndef MAX
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#endif 

static size_t __rotates = 0;

static size_t avl_height(avl_node *node) {
    return NULL == node ? 0 : node->height;
}

static size_t avl_update_height(avl_node *node) {
    return NULL == node ? 0 : (1 + MAX(avl_height(node->left), avl_height(node->right)));
}


/*
         y    Right Rotate (y)        x      
        / \   ----------------->     / \   
       x   T3                      T1   y        
      / \      <--------------         / \
    T1   T2     Left Rotation(x)     T2   T3
*/
static avl_node *avl_right_rotate(avl_node **root, avl_node *y) {
    avl_node *x = y->left;
    avl_node *T2 = x->right;
    avl_node *parent = y->parent;

    x->right = y;
    y->parent = x;

    y->left = T2;
    if (T2) T2->parent = y;

    x->parent = parent;
    if (parent) {
        if (parent->left == y)
            parent->left = x;
        else 
            parent->right = x;
    } else {
        *root = x;
    }

    y->height = avl_update_height(y);
    x->height = avl_update_height(x);

    if (parent)
        parent->height = avl_update_height(parent);

    // printf("%s\n", __func__);
    __rotates++;
    return x;
}

static avl_node *avl_left_rotate(avl_node **root, avl_node *x) {
    avl_node *y = x->right;
    avl_node *T2 = y->left;
    avl_node *parent = x->parent;

    y->left = x;
    x->parent = y;

    x->right = T2;
    if (T2) T2->parent = x;

    y->parent = parent;
    if (parent) {
        if (parent->left == x)
            parent->left = y;
        else 
            parent->right = y;
    } else {
        *root = y;
    }


    /* 注意顺序，先计算较矮的 */
    x->height = avl_update_height(x);
    y->height = avl_update_height(y);
    if (parent)
        parent->height = avl_update_height(parent);

    // printf("%s\n", __func__);
    __rotates++;
    return y;
}

static int avl_balance_factor(avl_node *node) {
    if (NULL == node) return 0;
    return avl_height(node->left) - avl_height(node->right);
}


static void avl_rebalance(avl_node **root, avl_node *node) {
    if (NULL == node) return;
    node->height = avl_update_height(node);
    int factor = avl_balance_factor(node);

    avl_node *parent = node->parent;

    /* If this node becomes unbalanced, then there are 4 cases */

    /************************************************************
    a) Left Left Case 
        T1, T2, T3 and T4 are subtrees.
                z                                      y 
               / \                                   /  \
              y   T4      Right Rotate (z)          x    z
            / \          - - - - - - - - ->       / \   / \ 
          x   T3                                T1  T2 T3  T4
         / \
       T1   T2
    ************************************************************/
    if (factor > 1 && avl_balance_factor(node->left) >=0 ) 
        avl_right_rotate(root, node);
    
    /************************************************************
    b) Right Right Case 
          z                                y
         /  \                            /   \ 
        T1   y     Left Rotate(z)       z      x
            /  \   - - - - - - - ->    / \    / \
           T2   x                     T1  T2 T3  T4
               / \
             T3  T4
    ************************************************************/
    else if (factor < -1 && avl_balance_factor(node->right) <=0 ) 
        avl_left_rotate(root, node);
    
    /************************************************************
    c) Left Right Case 
            z                               z                           x
          / \                            /   \                        /  \ 
         y   T4  Left Rotate (y)        x    T4  Right Rotate(z)    y      z
        / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    / \
      T1   x                          y    T3                    T1  T2 T3  T4
          / \                        / \
        T2   T3                    T1   T2
    ************************************************************/
    else if (factor > 1 && avl_balance_factor(node->left) < 0 ) {
        avl_left_rotate(root, node->left);
        avl_right_rotate(root, node);
    }

    /************************************************************
    d) Right Left Case 
           z                            z                            x
          / \                          / \                          /  \ 
        T1   y   Right Rotate (y)    T1   x      Left Rotate(z)   z      y
            / \  - - - - - - - - ->     /  \   - - - - - - - ->  / \    / \
           x   T4                      T2   y                  T1  T2  T3  T4
          / \                              /  \
        T2   T3                           T3   T4
    ************************************************************/
    else if (factor < -1 && avl_balance_factor(node->right) > 0 ) {
        avl_right_rotate(root, node->right);
        avl_left_rotate(root, node);
    }

    avl_rebalance(root, parent);
}


void avl_insert(avl_node **root, avl_node *node) {
    avl_rebalance(root, node);
}

void avl_erase(avl_node **root, avl_node *node) {
    avl_node *child, *parent;
    if (NULL == node) return;

    /* two children */
    if (node->left && node->right) {
        avl_node *left;
        avl_node *old = node;

        node = node->right;
        while ((left = node->left) != NULL)
            node = node->left;
        child = node->right;
        parent = node->parent;
        /* 分离 node, 用于替换 old */
        if (child) child->parent = parent;
        if (parent == old) {
            parent->right = child;
            parent = node;
        } else {
            parent->left = child;
        }

        /* 右子树的最小节点 node 替换删除位置的节点 old */
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

    avl_rebalance(root, parent);

}

avl_node *avl_next(avl_node *node) {
    avl_node *parent;
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

avl_node *avl_prev(avl_node *node) {
    avl_node *parent;
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

avl_node *avl_first(avl_node *root) {
    if (NULL == root) return NULL;
    while (root->left)
        root = root->left;
    return root;
}

avl_node *avl_last(avl_node *root) {
    if (NULL == root) return NULL;
    while (root->right)
        root = root->right;
    return root;
}

size_t avl_rotate_times() { return __rotates;}
void avl_reset_rotate_times() { __rotates = 0;}

