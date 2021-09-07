/**
 * @file benchmark.cpp
 * @author luuyiran (luuyiran@gmail.com)
 * @brief test maps 
 * @version 0.1
 * @date 2021-09-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <benchmark/benchmark.h>


#include "avl_tree.hpp"
#include "rb_tree.hpp"
#include "llrb_tree.hpp"

#define TEST_COUNTS  2000000UL

typedef bst_map<size_t, size_t>  base_type;
typedef avl_map<size_t, size_t>  avl_type;
typedef rbt_map<size_t, size_t>  rbt_type;
typedef llrb_map<size_t, size_t> llrb_type;

base_type *bst = new base_type(); 
base_type *avl = new avl_type(); 
base_type *rbt = new rbt_type(); 
base_type *llrb = new llrb_type();

size_t *nums = get_rand_array1(TEST_COUNTS);

static void insert(benchmark::State& state) {
    base_type *ptr = (base_type *)state.range(0);
    for (auto _ : state)
    for (size_t i = 0; i < TEST_COUNTS; i++)
        ptr->insert(nums[i], nums[i]);
}

BENCHMARK(insert)->ArgName("bst")->Arg((int64_t)bst);
BENCHMARK(insert)->ArgName("avl")->Arg((int64_t)avl);
BENCHMARK(insert)->ArgName("rbt")->Arg((int64_t)rbt);
BENCHMARK(insert)->ArgName("llrb")->Arg((int64_t)llrb);


static void find(benchmark::State& state) {
    base_type *ptr = (base_type *)state.range(0);
    for (auto _ : state)
    for (size_t i = 0; i < TEST_COUNTS; i++)
        ptr->find(nums[i]);
}

BENCHMARK(find)->ArgName("bst")->Arg((int64_t)bst);
BENCHMARK(find)->ArgName("avl")->Arg((int64_t)avl);
BENCHMARK(find)->ArgName("rbt")->Arg((int64_t)rbt);
BENCHMARK(find)->ArgName("llrb")->Arg((int64_t)llrb);



static void _delete(benchmark::State& state) {
    base_type *ptr = (base_type *)state.range(0);
    for (auto _ : state)
    for (size_t i = 0; i < TEST_COUNTS; i++)
        ptr->remove(nums[i]);
}

BENCHMARK(_delete)->ArgName("bst")->Arg((int64_t)bst);
BENCHMARK(_delete)->ArgName("avl")->Arg((int64_t)avl);
BENCHMARK(_delete)->ArgName("rbt")->Arg((int64_t)rbt);
BENCHMARK(_delete)->ArgName("llrb")->Arg((int64_t)llrb);



BENCHMARK_MAIN();


