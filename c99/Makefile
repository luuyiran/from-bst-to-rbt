ALL:test_tree benchmark

TREE_SRC=bst.c avl_tree.c rb_tree.c llrb_tree.c

HELPER=helper.c

CFLAGS:=-W -Wall -pedantic -std=c99 -O3 #-g -fstack-protector-all -fsanitize=address -fno-omit-frame-pointer -fsanitize=leak

test_tree : $(TREE_SRC) $(HELPER) test_tree.c 
	gcc -o $@ $^ $(CFLAGS)

benchmark : $(TREE_SRC) $(HELPER) benchmark.c 
	gcc -o $@ $^ $(CFLAGS)

.PHONY:clean
clean:
	@rm -rvf test_tree benchmark
