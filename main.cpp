/**
 * @file main.cpp
 * @author luuyiran (luuyiran@gmail.com)
 * @brief test maps
 * @version 0.1
 * @date 2021-09-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "avl_tree.hpp"
#include "rb_tree.hpp"
#include "llrb_tree.hpp"

#define COUNTS 20

typedef bst_map<size_t, size_t>  base_type;
typedef avl_map<size_t, size_t>  avl_type;
typedef rbt_map<size_t, size_t>  rbt_type;
typedef llrb_map<size_t, size_t> llrb_type;

// test for trees
template <typename Tree>
static void test_map(Tree) {
    base_type *root, *bak;
    root = new Tree();
    bak = new Tree();

    size_t *nums = get_rand_array1(COUNTS);

    for (int i = 0; i < COUNTS; i++) {
        printf("=================\ninsert: %ld\n", nums[i]);
        root->insert(nums[i], i);
        print(*root);
    }

    printf("\n\n======== insertion finished! =========\n\n");

    while (!root->empty()) {
        size_t key = root->root->key;
        printf("=================\ndelete: %ld\n", key);

        root->remove(key);
        //bak->insert(key, key);

        print(*root);
    }

    printf("\n\n======== root empty! =========\n\n");

    //while (!bak->empty()) {
    //    if (bak->end() != bak->remove(nums[rand() % COUNTS]))
    //        print(*bak);
    //}

    printf("\n\n======== bak empty! =========\n\n");

    drop_random_array(nums);

    delete root;
    delete bak;
}

// test for insert operator=   itorator 
static void test1() {
    llrb_map<char, size_t> *x = new llrb_map<char, size_t>();
    x->insert('o', 0);
    x->insert('i', 1);
    x->insert('t', 22);
    x->insert('d', 2);
    x->insert('x', 33);
    x->insert('a', 3);
    x->insert('p', 66);
    x->insert('c', 4);

    print(*x);

    printf("insert b - 996\n");
    (*x)['b'] = 996;

    print(*x);

    printf("x[p]=%lu  x[b]=%lu\n", (*x)['p'], (*x)['b']);

    (*x)['p'] = 88;
    (*x)['b'] = 111;

    print(*x);
    printf("x[p]=%lu  x[b]=%lu\n", (*x)['p'], (*x)['b']);

    for (auto i = x->begin(); i != x->end(); i++) 
         printf("%lu  ", *i);

    printf("\n");
    delete x;
}

#define TEST_COUNTS  2000000ul  
#define TYPE_COUNTS 4           

static void benchmark() {
    size_t i = 0;
    
    base_type *base[TYPE_COUNTS] = {nullptr};
    base[i++] = new base_type(); 
    base[i++] = new avl_type(); 
    base[i++] = new rbt_type(); 
    base[i++] = new llrb_type();

    size_t *nums = get_rand_array1(TEST_COUNTS);
    assert(nums);

    for (auto ptr : base) {
        clock_t tic, toc;
        tic = clock();
        for (i = 0; i < TEST_COUNTS; i++)
            ptr->insert(nums[i], nums[i]);
        toc = clock();
        printf("insert\t%s\t%ld\n", ptr->name(), toc - tic);
    }

    randomed(nums, TEST_COUNTS);

    for (auto ptr : base) {
        clock_t tic, toc;
        tic = clock();
        for (i = 0; i < TEST_COUNTS; i++) 
            ptr->find(nums[i]);
        toc = clock();
        printf("find  \t%s\t%ld\n", ptr->name(), toc - tic);
    }

    for (auto ptr : base) {
        clock_t tic, toc;
        tic = clock();
        for (i = 0; i < TEST_COUNTS; i++)
            ptr->remove(nums[i]);
        toc = clock();
        printf("delete \t%s\t%ld\n", ptr->name(), toc - tic);
    }

    for (auto ptr : base) assert(ptr->empty());

    for (auto ptr : base) delete (ptr);
    drop_random_array(nums);
}


int main() {

    #define TEST_ALL        0
    #define TEST_ITERM      6

    if (TEST_ALL || 1 == TEST_ITERM)
        test1();

    if (TEST_ALL || 2 == TEST_ITERM)
        test_map(base_type());

    if (TEST_ALL || 3 == TEST_ITERM)
        test_map(avl_type());

    if (TEST_ALL || 4 == TEST_ITERM)
        test_map(rbt_type());

    if (TEST_ALL || 5 == TEST_ITERM)
        test_map(llrb_type());


    if (TEST_ALL || 6 == TEST_ITERM)
        benchmark();

    return 0;
}