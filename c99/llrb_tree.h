#ifndef __LEFT_LEANING_RED_BLACK_TREE_H__
#define __LEFT_LEANING_RED_BLACK_TREE_H__

#define    LLRB_RED               (1u)
#define    LLRB_BLACK             (0u)


typedef struct llrb_node llrb_node;

struct llrb_node {
    llrb_node *left;
    llrb_node *right;
    llrb_node *parent;
    _Bool color;
};


/* 获取自定义结构的地址 */
#define LLRB_TREE_ENTRY(ptr, type, member)                           \
        ((type *)((char *)ptr - (size_t)&((type *)0)->member))

#ifdef __cplusplus
extern "C" {
#endif

void llrb_fix_up(llrb_node **root, llrb_node *node);
void llrb_erase(llrb_node **root, llrb_node *node);

size_t llrb_rotate_times();
void llrb_reset_rotate_times();

/* 把 node 放在 parent 之后，放置位置在 pos */
static inline void llrb_link_node(llrb_node *parent, llrb_node *node, llrb_node **pos) {
    node->parent = parent;
    node->left = node->right = NULL;
    node->color = LLRB_RED;
    *pos = node;
}





#ifdef __cplusplus
}
#endif


#endif /* !__LEFT_LEANING_RED_BLACK_TREE_H__ */


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
 *      list  list;
 *      ...
 *      llrb_node node;
 * };
 * 
 * // initialization
 * llrb_node *root = NULL; 
 * 
 * // insertion
 * int my_insert(llrb_node **root, mytype *data) {
 *      llrb_node *parent = NULL;           // insert after parent node
 *      llrb_node **pos = root;             // hold for parent->left or parent->right
 *      while (*pos) {
 *          mytype *this = LLRB_TREE_ENTRY(*pos, mytype, node);
 *          parent = *pos;
 *          if (data->key < this->key)
 *              pos = &((*pos)->left);
 *          else if (data->key > this->key) 
 *              pos = &((*pos)->right);
 *          else 
 *              return -1;                  // or update value;
 *      }
 *      
 *      // add to leaf: Insert a new node at the bottom
 *      llrb_link_node(parent, &data->node, pos);
 *      // rebanlance if necessary
 *      llrb_fix_up(&root, &data->node);
 *      return 0;
 * }
 * 
 * // search
 * mytype *my_search(llrb_node **root, int key) {
 *      llrb_node *node = *root;
 *      while (node) {
 *          mytype *this = LLRB_TREE_ENTRY(node, mytype, node);
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
 *      llrb_erase(&root, &data->node);
 *      my_free(data);
 * }
 * \endcode
 * 
 */
