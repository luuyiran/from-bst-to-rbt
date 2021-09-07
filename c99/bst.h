/**
 * @file bst.h
 * @author luyiran @ 872289455@qq.com
 * @brief balanced binary search trees
 * @version 0.1
 * @date 2021-08-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __BALANCED_BINARY_SEARCH_TREE_H__
#define __BALANCED_BINARY_SEARCH_TREE_H__
#include <stdio.h>

typedef struct bst_node bst_node;

struct bst_node {
    bst_node *left;
    bst_node *right;
    bst_node *parent;
};


/* 获取自定义结构体的地址 */
#define BST_ENTRY(ptr, type, member)                           \
        ((type *)((char *)ptr - (size_t)&((type *)0)->member))

#ifdef __cplusplus
extern "C" {
#endif

void bst_erase(bst_node **root, bst_node *node);

bst_node *bst_next(bst_node *node);
bst_node *bst_prev(bst_node *node);
bst_node *bst_first(bst_node *root);
bst_node *bst_last(bst_node *root);

/* 把 node 放在 parent 之后，放置位置在 pos，pos保存父节点 parent 的左子树或右子树的指针 */
static inline void bst_link_node(bst_node *parent, bst_node *node, bst_node **pos) {
    node->parent = parent;
    node->left = node->right = NULL;

    /* pos保存的是 &parent->left;或者 &parent->right; */
    /* 因此下面一行代码相当于 parent->left = node;或者 parent->right = node; */
    *pos = node;        
}

#ifdef __cplusplus
}
#endif

#endif /* !__BALANCED_BINARY_SEARCH_TREE_H__ */


/**
 * Example:
 * 
 * key:     int key;
 * value:   struct mytype
 * 
 * @code
 * typedef struct mytype mytype;
 * struct mytype {
 *      int key;
 *      char *id;
 *      ...
 *      bst_node node;
 * };
 * 
 * // initialization
 * bst_node *root = NULL; 
 * 
 * // insertion
 * int my_insert(bst_node **root, mytype *data) {
 *      bst_node *parent = NULL;  // insert after parent node
 *      bst_node **pos = root;           // insert position
 *      while (*pos) {
 *          mytype *this = BST_ENTRY(*pos, mytype, node);
 *          parent = *pos;
 *          if (data->key < this->key)
 *              pos = &((*pos)->left);
 *          else if (data->key > this->key) 
 *              pos = &((*pos)->right);
 *          else 
 *              return false or update value;
 *      }
 *      
 *      bst_link_node(parent, &data->node, pos);
 * }
 * 
 * // search
 * mytype *my_search(bst_node **root, int key) {
 *      bst_node *node = *root;
 *      while (node) {
 *          mytype *this = BST_ENTRY(node, mytype, node);
 *          if (key < this->key)
 *              node = node->left;
 *          else if (key > this->key) 
 *              node = node->right;
 *          else 
 *              return this;
 *      }
 *      return NULL;
 * }
 * 
 * // delete node
 * mytype *data = my_search(&root, 996);
 * if (data) {
 *      bst_erase(&root, &data->node);
 *      my_free(data);
 * }
 * \endcode
 * 
 */

