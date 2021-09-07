### 二叉平衡树测试

使用C++实现了类似STL中map容器，底层分别使用普通二叉查找树、AVL树、经典红黑树、左倾红黑树. 

```lua
luuyiran@ubuntu:~$ make -j2
luuyiran@ubuntu:~$ ./benchmark
2021-09-07T22:28:56+08:00
Running ./benchmark
Run on (8 X 1992 MHz CPU s)
Load Average: 0.52, 0.58, 0.59
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
insert/bst:105759274696720   2569620900 ns   2546875000 ns            1
insert/avl:105759274696768   2666974200 ns   2656250000 ns            1
insert/rbt:105759274696816   2530156800 ns   2484375000 ns            1
insert/llrb:105759274696864  3463220200 ns   3390625000 ns            1
find/bst:105759274696720     2205500700 ns   2203125000 ns            1
find/avl:105759274696768     1999516100 ns   1937500000 ns            1
find/rbt:105759274696816     1748858200 ns   1734375000 ns            1
find/llrb:105759274696864    1712089800 ns   1703125000 ns            1
_delete/bst:105759274696720  2498882000 ns   2500000000 ns            1
_delete/avl:105759274696768  2359427200 ns   2359375000 ns            1
_delete/rbt:105759274696816  2333987500 ns   2328125000 ns            1
_delete/llrb:105759274696864 7227784300 ns   7218750000 ns            1
luuyiran@ubuntu:~$ ./test
insert  bst     2109375
insert  avl     2109375
insert  rbt     1875000
insert  llrb    2078125
find    bst     2312500
find    avl     1687500
find    rbt     1750000
find    llrb    1703125
delete  bst     2593750
delete  avl     2046875
delete  rbt     2000000
delete  llrb    3093750
```

200万随机数据测试`./test`：
![image](200.png)


2000万随机数据测试`./test`：
![image](2000.png)

测试表明，经典红黑树综合表现最好。

