#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "bst.h"
#include "avl_tree.h"
#include "rb_tree.h"

#include "helper.h"

/* 200 万 */
#define COUNTS 2000000ul


/* 根据时间消耗排序用 */
typedef struct _TIME_INFO_ {
    char name[16];
    clock_t insert;
    clock_t search;
    clock_t delet_root;
    clock_t search_delete;
    size_t  rotates;
} TIME_INFO;



static TIME_INFO test_bst() {
    size_t i, cnt = 0;
    TIME_INFO cpu_times;
    clock_t tic, toc;
    bst_node *root = NULL;
    bst_t *data = NULL;
    bst_t *datas = (bst_t *)calloc(COUNTS, sizeof(bst_t));
    size_t *nums = get_rand_array1(COUNTS);
    assert(nums && datas);

    memcpy(cpu_times.name, "bst", 4);

    tic = clock();
    for (i = 0; i < COUNTS; i++) {
        datas[i].key = nums[i];
        insert_bst(&root, &datas[i]);
    }
    toc = clock();
    cpu_times.insert = toc - tic;

    tic = clock();
    while (root)
        bst_erase(&root, root);
    toc = clock();
    cpu_times.delet_root = toc - tic;

    memset(datas, 0, COUNTS * sizeof(bst_t));
    tic = clock();
    for (i = 0; i < COUNTS; i++) {
        datas[i].key = nums[i];
        insert_bst(&root, &datas[i]);
    }
    toc = clock();
    cpu_times.insert += toc - tic;

    tic = clock();
    while (cnt < 2 * COUNTS) {
        if (search_bst(&root, nums[rand() % COUNTS]))
            cnt++;
    }
    toc = clock();
    cpu_times.search = toc - tic;

    tic = clock();
    while (root) {
        data = search_bst(&root, nums[rand() % COUNTS]);
        if (data)
            bst_erase(&root, &data->node);
    }
    toc = clock();
    cpu_times.search_delete = toc - tic;

    drop_random_array(nums);
    free(datas);
    cpu_times.rotates = 0;
    return cpu_times;
}

static TIME_INFO test_avl() {
    size_t i, cnt = 0;
    TIME_INFO cpu_times;
    clock_t tic, toc;
    avl_node *root = NULL;
    avl_t *data = NULL;
    avl_t *datas = (avl_t *)calloc(COUNTS, sizeof(avl_t));
    size_t *nums = get_rand_array1(COUNTS);
    assert(nums && datas);

    memcpy(cpu_times.name, "avl", 4);

    tic = clock();
    for (i = 0; i < COUNTS; i++) {
        datas[i].key = nums[i];
        insert_avl(&root, &datas[i]);
    }
    toc = clock();
    cpu_times.insert = toc - tic;

    tic = clock();
    while (root)
        avl_erase(&root, root);
    toc = clock();
    cpu_times.delet_root = toc - tic;

    memset(datas, 0, COUNTS * sizeof(avl_t));
    tic = clock();
    for (i = 0; i < COUNTS; i++) {
        datas[i].key = nums[i];
        insert_avl(&root, &datas[i]);
    }
    toc = clock();
    cpu_times.insert += toc - tic;


    tic = clock();
    while (cnt < 2 * COUNTS) {
        if (search_avl(&root, nums[rand() % COUNTS]))
            cnt++;
    }
    toc = clock();
    cpu_times.search = toc - tic;

    tic = clock();
    while (root) {
        data = search_avl(&root, nums[rand() % COUNTS]);
        if (data)
            avl_erase(&root, &data->node);
    }
    toc = clock();
    cpu_times.search_delete = toc - tic;

    drop_random_array(nums);
    free(datas);

    cpu_times.rotates = avl_rotate_times();

    return cpu_times;
}

static TIME_INFO test_rbt() {
    size_t i, cnt = 0;
    TIME_INFO cpu_times;
    clock_t tic, toc;
    rb_node *root = NULL;
    rbt_t *data = NULL;
    rbt_t *datas = (rbt_t *)calloc(COUNTS, sizeof(rbt_t));
    size_t *nums = get_rand_array2(COUNTS);
    assert(nums);

    memcpy(cpu_times.name, "rbt", 4);

    tic = clock();
    for (i = 0; i < COUNTS; i++) {
        datas[i].key = nums[i];
        insert_rbt(&root, &datas[i]);
    }
    toc = clock();
    cpu_times.insert = toc - tic;

    tic = clock();
    while (root)
        rbt_erase(&root, root);
    toc = clock();
    cpu_times.delet_root = toc - tic;

    memset(datas, 0, COUNTS * sizeof(rbt_t));
    tic = clock();
    for (i = 0; i < COUNTS; i++) {
        datas[i].key = nums[i];
        insert_rbt(&root, &datas[i]);
    }
    toc = clock();
    cpu_times.insert += toc - tic;

    tic = clock();
    while (cnt < 2 * COUNTS) {
        if (search_rbt(&root, nums[rand() % COUNTS]))
            cnt++;
    }
    toc = clock();
    cpu_times.search = toc - tic;


    tic = clock();
    while (root) {
        data = search_rbt(&root, nums[rand() % COUNTS]);
        if (data)
            rbt_erase(&root, &data->node);
    }
    toc = clock();
    cpu_times.search_delete = toc - tic;

    drop_random_array(nums);
    free(datas);
    cpu_times.rotates = rbt_rotate_times();
    return cpu_times;
}

static TIME_INFO test_llrb() {
    size_t i, cnt = 0;
    TIME_INFO cpu_times;
    clock_t tic, toc;
    llrb_node *root = NULL;
    llrb_t *data = NULL;
    llrb_t *datas = (llrb_t *)calloc(COUNTS, sizeof(llrb_t));
    size_t *nums = get_rand_array2(COUNTS);
    assert(nums);

    memcpy(cpu_times.name, "llrb", 5);

    tic = clock();
    for (i = 0; i < COUNTS; i++) {
        datas[i].key = nums[i];
        insert_llrb(&root, &datas[i]);
    }
    toc = clock();
    cpu_times.insert = toc - tic;

    tic = clock();
    while (root)
        llrb_erase(&root, root);
    toc = clock();
    cpu_times.delet_root = toc - tic;

    memset(datas, 0, COUNTS * sizeof(rbt_t));
    tic = clock();
    for (i = 0; i < COUNTS; i++) {
        datas[i].key = nums[i];
        insert_llrb(&root, &datas[i]);
    }
    toc = clock();
    cpu_times.insert += toc - tic;

    tic = clock();
    while (cnt < 2 * COUNTS) {
        if (search_llrb(&root, nums[rand() % COUNTS]))
            cnt++;
    }
    toc = clock();
    cpu_times.search = toc - tic;


    tic = clock();
    while (root) {
        data = search_llrb(&root, nums[rand() % COUNTS]);
        if (data)
            llrb_erase(&root, &data->node);
    }
    toc = clock();
    cpu_times.search_delete = toc - tic;

    drop_random_array(nums);
    free(datas);
    cpu_times.rotates = llrb_rotate_times();
    return cpu_times;
}

static void benchmark() {
    int i;
    TIME_INFO  cpu_times[4] = {0};
    cpu_times[0] = test_bst();
    cpu_times[1] = test_avl();
    cpu_times[2] = test_rbt();
    cpu_times[3] = test_llrb();
    printf("#\tinsert\tsearch\tdel_root\tsearch_del\trotates\n");

    for (i = 0; i < 4; i++) {
        printf("%s\t%ld\t%ld\t%ld\t\t%ld\t\t%ld\n", 
        cpu_times[i].name, 
        cpu_times[i].insert,
        cpu_times[i].search, 
        cpu_times[i].delet_root, 
        cpu_times[i].search_delete,
        cpu_times[i].rotates);
    }

}




int main() {
    benchmark();
    return 0;
}
