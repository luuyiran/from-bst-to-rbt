#ifndef __TEST_HELPER_H
#define __TEST_HELPER_H

#include "bst.h"
#include "avl_tree.h"
#include "rb_tree.h"
#include "llrb_tree.h"

#define container_of(ptr, type, member) ({			\
    const __typeof( ((type *)0)->member ) *__mptr = (ptr);	\
    (type *)( (char *)__mptr - offsetof(type,member) );})

typedef struct bst_t bst_t;
struct bst_t {
    size_t key;
    bst_node node;
};

typedef struct avl_t avl_t;
struct avl_t {
    size_t key;
    avl_node node;
};

typedef struct rbt_t rbt_t;
struct rbt_t {
    size_t key;
    rb_node node;
};

typedef struct llrb_t llrb_t;
struct llrb_t {
    size_t key;
    llrb_node node;
};

#ifdef __cplusplus
extern "C" {
#endif


/* 树状形式打印二叉树 */
void print_bst(bst_node *root);
void print_avl(avl_node *root);
void print_rbt(rb_node *root);
void print_llrb(llrb_node *root);

size_t *get_rand_array1(size_t size);
size_t *get_rand_array2(size_t size);
void drop_random_array(size_t *nums);


/* 针对自定义结构体 mytype 编写插入和搜索函数 */
int insert_bst(bst_node **root, bst_t *data);
int insert_avl(avl_node **root, avl_t *data);
int insert_rbt(rb_node  **root, rbt_t *data);
int insert_llrb(llrb_node  **root, llrb_t *data);
bst_t *search_bst(bst_node **root, size_t key);
avl_t *search_avl(avl_node **root, size_t key);
rbt_t *search_rbt(rb_node  **root, size_t key);
llrb_t *search_llrb(llrb_node  **root, size_t key);


#ifdef __cplusplus
}
#endif


#endif /* !__TEST_HELPER_H */
