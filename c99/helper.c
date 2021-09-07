#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "helper.h"


/**
 * printing tree in ascii
 * 
 * http://web.archive.org/web/20090617110918/http://www.openasthra.com/c-tidbits/printing-binary-trees-in-ascii/
 * 
 */
typedef struct asciinode_struct asciinode;

struct asciinode_struct {
    asciinode *left, *right;

    /* length of the edge from this node to its children */
    int edge_length;

    int height;

    int lablen;

    /* -1=I am left, 0=I am root, 1=right */
    int parent_dir;

    /* max supported unit32 in dec, 10 digits max */
    char label[16];

    /* 是否是红色节点，辅助打印红黑树 */
    char is_red;    
};

#define MAX_HEIGHT 1000
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
#define INFINITY (1 << 20)

/* adjust gap between left and right nodes */
int gap = 3;

/* used for printing next node in the same level, */
/* this is the x coordinate of the next char printed */
int print_next;

#ifndef MIN
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#endif 

#ifndef MAX
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#endif 

/* Free all the nodes of the given tree */
static void free_ascii_tree(asciinode *node) {
    if (node == NULL) return;
    free_ascii_tree(node->left);
    free_ascii_tree(node->right);
    free(node);
}

/* The following function fills in the lprofile array for the given tree. */
/* It assumes that the center of the label of the root of this tree */
/* is located at a position (x,y).  It assumes that the edge_length */
/* fields have been computed for this tree. */
static void compute_lprofile(asciinode *node, int x, int y) {
    int i, isleft;
    if (node == NULL) return;
    isleft = (node->parent_dir == -1);
    lprofile[y] = MIN(lprofile[y], x - ((node->lablen - isleft) / 2));
    if (node->left != NULL) {
        for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++) {
            lprofile[y + i] = MIN(lprofile[y + i], x - i);
        }
    }
    compute_lprofile(node->left, x - node->edge_length - 1,
                     y + node->edge_length + 1);
    compute_lprofile(node->right, x + node->edge_length + 1,
                     y + node->edge_length + 1);
}

static void compute_rprofile(asciinode *node, int x, int y) {
    int i, notleft;
    if (node == NULL) return;
    notleft = (node->parent_dir != -1);
    rprofile[y] = MAX(rprofile[y], x + ((node->lablen - notleft) / 2));
    if (node->right != NULL) {
        for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++) {
            rprofile[y + i] = MAX(rprofile[y + i], x + i);
        }
    }
    compute_rprofile(node->left, x - node->edge_length - 1,
                     y + node->edge_length + 1);
    compute_rprofile(node->right, x + node->edge_length + 1,
                     y + node->edge_length + 1);
}

/* This function fills in the edge_length and */
/* height fields of the specified tree */
static void compute_edge_lengths(asciinode *node) {
    int h, hmin, i, delta;
    if (node == NULL) return;
    compute_edge_lengths(node->left);
    compute_edge_lengths(node->right);

    /* first fill in the edge_length of node */
    if (node->right == NULL && node->left == NULL) {
        node->edge_length = 0;
    } else {
        if (node->left != NULL) {
            for (i = 0; i < node->left->height && i < MAX_HEIGHT; i++) {
                rprofile[i] = -INFINITY;
            }
            compute_rprofile(node->left, 0, 0);
            hmin = node->left->height;
        } else {
            hmin = 0;
        }
        if (node->right != NULL) {
            for (i = 0; i < node->right->height && i < MAX_HEIGHT; i++) {
                lprofile[i] = INFINITY;
            }
            compute_lprofile(node->right, 0, 0);
            hmin = MIN(node->right->height, hmin);
        } else {
            hmin = 0;
        }
        delta = 4;
        for (i = 0; i < hmin; i++) {
            delta = MAX(delta, gap + 1 + rprofile[i] - lprofile[i]);
        }

        /* If the node has two children of height 1, then we allow the */
        /* two leaves to be within 1, instead of 2 */
        if (((node->left != NULL && node->left->height == 1) ||
             (node->right != NULL && node->right->height == 1)) &&
            delta > 4) {
            delta--;
        }

        node->edge_length = ((delta + 1) / 2) - 1;
    }

    /* now fill in the height of node */
    h = 1;
    if (node->left != NULL) {
        h = MAX(node->left->height + node->edge_length + 1, h);
    }
    if (node->right != NULL) {
        h = MAX(node->right->height + node->edge_length + 1, h);
    }
    node->height = h;
}

/* This function prints the given level of the given tree, assuming */
/* that the node has the given x cordinate. */
static void print_level(asciinode *node, int x, int level) {
    int i, isleft;
    if (node == NULL) return;
    isleft = (node->parent_dir == -1);
    if (level == 0) {
        for (i = 0; i < (x - print_next - ((node->lablen - isleft) / 2)); i++) {
            printf(" ");
        }
        print_next += i;
        if (node->is_red)
            printf("\033[31m%s\033[0m", node->label);
        else 
            printf("%s", node->label);
        print_next += node->lablen;
    } else if (node->edge_length >= level) {
        if (node->left != NULL) {
            for (i = 0; i < (x - print_next - (level)); i++) {
                printf(" ");
            }
            print_next += i;
            printf("/");
            print_next++;
        }
        if (node->right != NULL) {
            for (i = 0; i < (x - print_next + (level)); i++) {
                printf(" ");
            }
            print_next += i;
            printf("\\");
            print_next++;
        }
    } else {
        print_level(node->left, x - node->edge_length - 1,
                    level - node->edge_length - 1);
        print_level(node->right, x + node->edge_length + 1,
                    level - node->edge_length - 1);
    }
}

/* prints ascii tree for given Tree structure */
static void print_ascii_tree(asciinode *proot) {
    int xmin, i;
    if (NULL == proot) return;
    compute_edge_lengths(proot);
    for (i = 0; i < proot->height && i < MAX_HEIGHT; i++) {
        lprofile[i] = INFINITY;
    }
    compute_lprofile(proot, 0, 0);
    xmin = 0;
    for (i = 0; i < proot->height && i < MAX_HEIGHT; i++) {
        xmin = MIN(xmin, lprofile[i]);
    }
    for (i = 0; i < proot->height; i++) {
        print_next = 0;
        print_level(proot, -xmin, i);
        printf("\n");
    }
    if (proot->height >= MAX_HEIGHT) {
        printf("(This tree is taller than %d, and may be drawn incorrectly.)\n",
               MAX_HEIGHT);
    }
    
}



/**
 * BST 辅助打印
*/
static asciinode *bst_build_recursive(bst_node *root) {
    asciinode *node;

    if (root == NULL) return NULL;

    node = calloc(1, sizeof(asciinode));
    node->left = bst_build_recursive(root->left);
    node->right = bst_build_recursive(root->right);

    if (node->left != NULL) 
        node->left->parent_dir = -1;

    if (node->right != NULL) 
        node->right->parent_dir = 1;

    bst_t *this = BST_ENTRY(root, bst_t, node);

    sprintf(node->label, "%ld", this->key);
    node->lablen = strlen(node->label);

    return node;
}

/* Copy the tree into the ascii node structre */
static asciinode *bst_build_ascii_tree(bst_node *root) {
    asciinode *node;
    if (root == NULL) return NULL;
    node = bst_build_recursive(root);
    if (node) node->parent_dir = 0;
    return node;
}

void print_bst(bst_node *root) {
    if (NULL == root) return;
    asciinode *node = bst_build_ascii_tree(root);
    print_ascii_tree(node);
    free_ascii_tree(node);
}



/**
 * AVL 辅助打印
*/
static asciinode *avl_build_recursive(avl_node *root) {
    asciinode *node;

    if (root == NULL) return NULL;

    node = calloc(1, sizeof(asciinode));
    node->left = avl_build_recursive(root->left);
    node->right = avl_build_recursive(root->right);

    if (node->left != NULL) 
        node->left->parent_dir = -1;

    if (node->right != NULL) 
        node->right->parent_dir = 1;

    avl_t *this = AVL_ENTRY(root, avl_t, node);

    sprintf(node->label, "%ld", this->key);
    node->lablen = strlen(node->label);

    return node;
}

static asciinode *avl_build_ascii_tree(avl_node *root) {
    asciinode *node;
    if (root == NULL) return NULL;
    node = avl_build_recursive(root);
    if (node) node->parent_dir = 0;
    return node;
}

void print_avl(avl_node *root) {
    if (NULL == root) return;
    asciinode *node = avl_build_ascii_tree(root);
    print_ascii_tree(node);
    free_ascii_tree(node);
}




/**
 * 红黑树 辅助打印
*/
static asciinode *rbt_build_recursive(rb_node *root) {
    asciinode *node;

    if (root == NULL) return NULL;

    node = (asciinode *)calloc(1, sizeof(asciinode));
    node->left = rbt_build_recursive(root->left);
    node->right = rbt_build_recursive(root->right);
    node->is_red = root->color;

    if (node->left != NULL) 
        node->left->parent_dir = -1;

    if (node->right != NULL) 
        node->right->parent_dir = 1;

    rbt_t *this = RB_TREE_ENTRY(root, rbt_t, node);

    sprintf(node->label, "%ld", this->key);
    node->lablen = strlen(node->label);

    return node;
}

static asciinode *rbt_build_ascii_tree(rb_node *root) {
    asciinode *node;
    if (root == NULL) return NULL;
    node = rbt_build_recursive(root);
    if (node) node->parent_dir = 0;
    return node;
}

void print_rbt(rb_node *root) {
    if (NULL == root) return;
    asciinode *node = rbt_build_ascii_tree(root);
    print_ascii_tree(node);
    free_ascii_tree(node);
}


/**
 * 左倾红黑树 辅助打印
*/
static asciinode *llrb_build_recursive(llrb_node *root) {
    asciinode *node;

    if (root == NULL) return NULL;

    node = (asciinode *)calloc(1, sizeof(asciinode));
    node->left = llrb_build_recursive(root->left);
    node->right = llrb_build_recursive(root->right);
    node->is_red = root->color;

    if (node->left != NULL) 
        node->left->parent_dir = -1;

    if (node->right != NULL) 
        node->right->parent_dir = 1;

    llrb_t *this = LLRB_TREE_ENTRY(root, llrb_t, node);

    sprintf(node->label, "%ld", this->key);
    node->lablen = strlen(node->label);

    return node;
}

static asciinode *llrb_build_ascii_tree(llrb_node *root) {
    asciinode *node;
    if (root == NULL) return NULL;
    node = llrb_build_recursive(root);
    if (node) node->parent_dir = 0;
    return node;
}

void print_llrb(llrb_node *root) {
    if (NULL == root) return;
    asciinode *node = llrb_build_ascii_tree(root);
    print_ascii_tree(node);
    free_ascii_tree(node);
}



/* 通过随机交换获取随机值，能保证没有重复值 */
size_t *get_rand_array1(size_t size) {
    size_t i, x, y, tmp, *nums;
    srand(time(NULL));

    nums = (size_t *)calloc(size, sizeof(size_t));
    assert(nums);

    for (i = 0; i < size; i++) 
        nums[i] = i;

    /* random swap */
    for (i = 0; i < size; i++) {
        x = rand() % size;
        y = rand() % size;
        tmp = nums[x];
        nums[x] = nums[y];
        nums[y] = tmp;
    }
    return nums;
}

size_t *get_rand_array2(size_t size) {
    size_t i, *nums;
    srand(time(NULL));

    nums = (size_t *)calloc(size, sizeof(size_t));
    assert(nums);

    for (i = 0; i < size; i++) 
        nums[i] = rand();

    return nums;
}

void drop_random_array(size_t *nums) {
    if (nums) free(nums);
}




int insert_bst(bst_node **root, bst_t *data) {
     bst_node *parent = NULL;           // insert after parent node
     bst_node **pos = root;             // insert position
     while (*pos) {
         bst_t *this = BST_ENTRY(*pos, bst_t, node);
         parent = *pos;
         if (data->key < this->key)
             pos = &((*pos)->left);
         else if (data->key > this->key) 
             pos = &((*pos)->right);
         else 
             return -1;
     }
     
     bst_link_node(parent, &data->node, pos);
     return 0;
}

bst_t *search_bst(bst_node **root, size_t key) {
     bst_node *node = *root;
     while (node) {
         bst_t *this = BST_ENTRY(node, bst_t, node);
         if (key < this->key)
             node = node->left;
         else if (key > this->key) 
             node = node->right;
         else 
             return this;
     }
     return NULL;
}



int insert_avl(avl_node **root, avl_t *data) {
     avl_node *parent = NULL;
     avl_node **pos = root;
     while (*pos) {
         avl_t *this = AVL_ENTRY(*pos, avl_t, node);
         parent = *pos;
         if (data->key < this->key)
             pos = &((*pos)->left);
         else if (data->key > this->key) 
             pos = &((*pos)->right);
         else 
             return -1;
     }
     
     avl_link_node(parent, &data->node, pos);
     avl_insert(root, &data->node);
     return 0;
}

avl_t *search_avl(avl_node **root, size_t key) {
     avl_node *node = *root;
     while (node) {
         avl_t *this = AVL_ENTRY(node, avl_t, node);
         if (key < this->key)
             node = node->left;
         else if (key > this->key) 
             node = node->right;
         else 
             return this;
     }
     return NULL;
}

int insert_rbt(rb_node **root, rbt_t *data) {
     rb_node *parent = NULL;
     rb_node **pos = root;
     while (*pos) {
         rbt_t *this = RB_TREE_ENTRY(*pos, rbt_t, node);
         parent = *pos;
         if (data->key < this->key)
             pos = &((*pos)->left);
         else if (data->key > this->key) 
             pos = &((*pos)->right);
         else 
             return -1;
     }
     
     rb_link_node(parent, &data->node, pos);
     rbt_insert(root, &data->node);
     return 0;
}

rbt_t *search_rbt(rb_node **root, size_t key) {
     rb_node *node = *root;
     while (node) {
         rbt_t *this = RB_TREE_ENTRY(node, rbt_t, node);
         if (key < this->key)
             node = node->left;
         else if (key > this->key) 
             node = node->right;
         else 
             return this;
     }
     return NULL;
}



int insert_llrb(llrb_node  **root, llrb_t *data) {
     llrb_node *parent = NULL;
     llrb_node **pos = root;
     while (*pos) {
         llrb_t *this = LLRB_TREE_ENTRY(*pos, llrb_t, node);
         parent = *pos;
         if (data->key < this->key)
             pos = &((*pos)->left);
         else if (data->key > this->key) 
             pos = &((*pos)->right);
         else 
             return -1;
     }
     
     llrb_link_node(parent, &data->node, pos);
     llrb_fix_up(root, &data->node);
     return 0;
}


llrb_t *search_llrb(llrb_node  **root, size_t key) {
     llrb_node *node = *root;
     while (node) {
         llrb_t *this = LLRB_TREE_ENTRY(node, llrb_t, node);
         if (key < this->key)
             node = node->left;
         else if (key > this->key) 
             node = node->right;
         else 
             return this;
     }
     return NULL;
}
