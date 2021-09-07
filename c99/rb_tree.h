#ifndef __RED_BLACK_TREE_H__
#define __RED_BLACK_TREE_H__

#define    RB_RED               (1u)
#define    RB_BLACK             (0u)

typedef struct rb_node rb_node;

struct rb_node {
    rb_node *left;
    rb_node *right;
    rb_node *parent;
    _Bool color;
};


/* 获取自定义结构的地址 */
#define RB_TREE_ENTRY(ptr, type, member)                           \
        ((type *)((char *)ptr - (size_t)&((type *)0)->member))

#ifdef __cplusplus
extern "C" {
#endif

void rbt_insert(rb_node **root, rb_node *node);
void rbt_erase(rb_node **root, rb_node *node);

rb_node *rbt_next(rb_node *node);
rb_node *rbt_prev(rb_node *node);
rb_node *rbt_first(rb_node *root);
rb_node *rbt_last(rb_node *root);

size_t rbt_rotate_times();
void rbt_reset_rotate_times();

/* 把 node 放在 parent 之后，放置位置在 pos */
static inline void rb_link_node(rb_node *parent, rb_node *node, rb_node **pos) {
    node->parent = parent;
    node->left = node->right = NULL;
    node->color = RB_RED;
    *pos = node;
}





#ifdef __cplusplus
}
#endif


#endif /* !__RED_BLACK_TREE_H__ */
