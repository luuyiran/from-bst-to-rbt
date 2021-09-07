/**
 * @file test_tree.c
 * @author luuyiran (luuyiran@gmail.com)
 * @brief test bst/avl_tree/rb_tree
 * @version 0.1
 * @date 2021-09-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bst.h"
#include "avl_tree.h"
#include "rb_tree.h"
#include "llrb_tree.h"

#include "helper.h"

#define COUNTS 20

static inline void test_bst() {
    size_t i;
    bst_node *root = NULL;
    bst_node *bak = NULL;
    bst_t *data = NULL;
    bst_t *datas = (bst_t *)calloc(COUNTS, sizeof(bst_t));
    size_t *nums = get_rand_array1(COUNTS);
    assert(nums && datas);

    for (i = 0; i < COUNTS; i++) {
        datas[i].key = nums[i];
        insert_bst(&root, &datas[i]);
        printf("=================\ninsert: %ld\n", datas[i].key);
        print_bst(root);
    }

    printf("\n\n======== insertion finished! =========\n\n");

    while (root) {
        data = BST_ENTRY(root, bst_t, node);
        printf("=================\ndelete: %ld\n", data->key);

        bst_erase(&root, &data->node);

        print_bst(root);

        /* 转移到 bak */
        insert_bst(&bak, data);
        //free(data);
    }

    while (bak) {
        data = search_bst(&bak, nums[rand() % COUNTS]);
        if (data) {
            bst_erase(&bak, &data->node);
            // printf("=================\ndelete: %ld\n", data->key);
            // print_bst(bak);
        }
    }

    drop_random_array(nums);
    free(datas);
}

static inline void test_avl() {
    size_t i;
    avl_node *root = NULL;
    avl_node *bak = NULL;
    avl_t *data = NULL;
    size_t *nums = get_rand_array1(COUNTS);
    assert(nums);

    printf("======== test AVL trees ========\n");

    for (i = 0; i < COUNTS; i++) {
        data = (avl_t *)calloc(1, sizeof(avl_t));
        data->key = nums[i];
        printf("=================\ninsert: %ld\n", data->key);
        insert_avl(&root, data);
        print_avl(root);
    }

    printf("\n\n======== insertion finished! =========\n\n");

    while (root) {
        data = AVL_ENTRY(root, avl_t, node);
        printf("=================\ndelete: %ld\n", data->key);

        avl_erase(&root, &data->node);

        print_avl(root);

        /* 转移到 bak */
        insert_avl(&bak, data);
        //free(data);
    }

    printf("\n\n======== delete root finished! =========\n\n");

    while (bak) {
        data = search_avl(&bak, nums[rand() % COUNTS]);
        if (data) {
            printf("=================\ndelete: %ld\n", data->key);
            avl_erase(&bak, &data->node);
            print_avl(bak);
            free(data);
        }
    }

    printf("\n\n======== random delete finished! =========\n\n");

    drop_random_array(nums);
    printf("=========== AVL trees test OK ===========\n");
}

static inline void test_rbt() {
    size_t i;
    rb_node *root = NULL;
    rb_node *bak = NULL;
    rbt_t *data = NULL;
    size_t *nums = get_rand_array1(COUNTS);
    assert(nums);

    printf("======== test red black trees ========\n");

    for (i = 0; i < COUNTS; i++) {
        data = (rbt_t *)calloc(1, sizeof(rbt_t));
        data->key = nums[i];
        printf("=================\ninsert: %ld\n", data->key);
        insert_rbt(&root, data);
        print_rbt(root);
    }

    printf("\n\n======== insertion finished! =========\n\n");

    while (root) {
        data = BST_ENTRY(root, rbt_t, node);
        printf("=================\ndelete: %ld\n", data->key);

        rbt_erase(&root, root);

        print_rbt(root);

        /* 转移到 bak */
        insert_rbt(&bak, data);
        //free(data);
    }

    printf("\n\n======== delete root finished! =========\n\n");

    while (bak) {
        data = search_rbt(&bak, nums[rand() % COUNTS]);
        if (data) {
            printf("=================\ndelete: %ld\n", data->key);
            rbt_erase(&bak, &data->node);
            print_rbt(bak);
            free(data);
        }
    }

    printf("\n\n======== random delete finished! =========\n");

    drop_random_array(nums);
    printf("========= red black trees test OK ========\n");
}

static inline void test_llrb() {
    size_t i;
    llrb_node *root = NULL;
    llrb_node *bak = NULL;
    llrb_t *data = NULL;
    size_t *nums = get_rand_array1(COUNTS);
    assert(nums);

    printf("======== test left leaning red black trees ========\n");

    for (i = 0; i < COUNTS; i++) {
        data = (llrb_t *)calloc(1, sizeof(llrb_t));
        data->key = nums[i];
        printf("=================\ninsert: %ld\n", data->key);
        insert_llrb(&root, data);
        print_llrb(root);
    }

    printf("\n\n======== insertion finished! =========\n\n");

    while (root) {
        data = LLRB_TREE_ENTRY(root, llrb_t, node);
        printf("=================\ndelete: %ld\n", data->key);

        llrb_erase(&root, root);

        print_llrb(root);

        /* 转移到 bak */
        insert_llrb(&bak, data);
        //free(data);
    }

    printf("\n\n======== delete root finished! =========\n\n");

    while (bak) {
        data = search_llrb(&bak, nums[rand() % COUNTS]);
        if (data) {
            printf("=================\ndelete: %ld\n", data->key);
            llrb_erase(&bak, &data->node);
            print_llrb(bak);
            free(data);
        }
    }

    printf("\n\n======== random delete finished! =========\n");

    drop_random_array(nums);
    printf("========= left leaning red black trees test OK ========\n");
}


int main() {
    // test_bst();
    // test_avl();
    // test_rbt();
    test_llrb();
    return 0;

}
