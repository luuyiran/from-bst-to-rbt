/**
 * @file avl_tree.h
 * @author luyiran @ 872289455@qq.com
 * @brief avl tree
 * @version 0.1
 * @date 2021-09-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__
#include <stdio.h>

typedef struct avl_node avl_node;

struct avl_node {
    avl_node *left;
    avl_node *right;
    avl_node *parent;
    size_t    height;
};


/* 获取自定义结构体的地址 */
#define AVL_ENTRY(ptr, type, member)                           \
        ((type *)((char *)ptr - (size_t)&((type *)0)->member))


#ifdef __cplusplus
extern "C" {
#endif


void avl_insert(avl_node **root, avl_node *node);
void avl_erase(avl_node **root, avl_node *node);

avl_node *avl_next(avl_node *node);
avl_node *avl_prev(avl_node *node);
avl_node *avl_first(avl_node *root);
avl_node *avl_last(avl_node *root);

size_t avl_rotate_times();
void avl_reset_rotate_times();

/* 把 node 放在 parent 之后，放置位置在 pos，pos保存父节点 parent 的左子树或右子树的指针 */
static inline void avl_link_node(avl_node *parent, avl_node *node, avl_node **pos) {
    node->parent = parent;
    node->left = node->right = NULL;

    /* pos保存的是 &parent->left;或者 &parent->right; */
    /* 因此下面一行代码相当于 parent->left = node;或者 parent->right = node; */
    *pos = node;        
}

#ifdef __cplusplus
}
#endif


#endif /* !__AVL_TREE_H__ */



/**
 * Example:
 * 
 * key:     char *key;
 * value:   struct mytype *data;
 * 
 * @code
 * 
 * 
 * typedef struct mytype mytype;
 * struct mytype {
 *      char *key;
 *      char *id;
 *      int   value;
 *      ...
 *      avl_node node;
 * };
 * 
 * // initialization
 * avl_node *root = NULL; 
 * 
 * // insertion
 * int my_insert(avl_node **root, mytype *data) {
 *      int res;
 *      avl_node *parent = NULL;  // insert after parent node
 *      avl_node **pos = root;           // insert position
 *      while (*pos) {
 *          mytype *this = AVL_ENTRY(*pos, mytype, node);
 *          res = strcmp(data->key, this->key);
 * 
 *          parent = *pos;
 *          if (res < 0)
 *              pos = &((*pos)->left);
 *          else if (res > 0) 
 *              pos = &((*pos)->right);
 *          else 
 *              return false or update this;
 *      }
 *      
 *      avl_link_node(parent, &data->node, pos);
 *      avl_insert(root, &data->node);
 * }
 * 
 * // search
 * mytype *my_search(avl_node **root, char *key) {
 *      int res;
 *      avl_node *node = *root;
 *      while (node) {
 *          mytype *this = AVL_ENTRY(node, mytype, node);
 *          res = strcmp(key, this->key);
 * 
 *          if (res < 0)
 *              node = node->left;
 *          else if (res > 0) 
 *              node = node->right;
 *          else 
 *              return this;
 *      }
 *      return NULL;
 * }
 * 
 * // delete node
 * mytype *data = my_search(&root, "996");
 * if (data) {
 *      avl_erase(&root, &data->node);
 *      my_free(data);
 * }
 * \endcode
 * 
 * https://lwn.net/Articles/184495/
 * https://kernel.googlesource.com/pub/scm/linux/kernel/git/ak/linux-misc/+/v3.1/Documentation/rbtree.txt
 * 
 */


