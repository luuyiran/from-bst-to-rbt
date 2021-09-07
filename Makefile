ALL:test benchmark

HEADER_FILES:=helper.hpp bst.hpp avl_tree.hpp rb_tree.hpp llrb_tree.hpp

CFLAGS:=-W -Wall -pedantic -O3 #-g -fstack-protector-all -fsanitize=address -fno-omit-frame-pointer -fsanitize=leak

LIBS:=-pthread -lbenchmark

test : main.cpp $(HEADER_FILES)
	g++ -o $@ $< $(CFLAGS) 

benchmark : benchmark.cpp $(HEADER_FILES)
	g++ -o $@ $< $(CFLAGS) $(LIBS)


.PHONY:clean
clean:
	@rm -rvf test benchmark
