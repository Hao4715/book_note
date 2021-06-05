# STL面试知识

[TOC]

## 容器

《STL源码剖析》中介绍的容器包括：

* array：数组
* vector：向量（数组）
* list：双向环状链表
* deque：双端队列
* stack：栈
* queue：队列。
* heap：堆
* priority_queue：优先队列
* slist：单向链表
* set
* map
* multiset
* multimap
* hash_set
* hash_map
* hash_multiset
* hash_multimap

**array**

数组，大小固定。

**vector**

单向扩展的数组，大小随容器扩充增加，遍历过程中增加删除元素回导致原有迭代器失效。

**list**

链表。插入操作会插入在指定元素前面。

**deque**

双向可扩展的链表，逻辑上连续，实际上是由一段一段的定量连续空间组成。

**stack**

栈。默认基于deque为底部结构来实现的，因此是adapter（配接器）。也可使用list作为底层结构来实现，使用：`stack<int,list<int> liststack;`。**没有迭代器**

**queue**

队列。**没有迭代器。**默认基于deque为底部结构来实现的，因此是adapter（配接器）。也可使用list作为底层结构来实现，使用：`queue<int,list<int> listqueue;`

**heap**

STL默认提供的是大顶堆。**没有遍历功能，因此没有迭代器。**

**priority_queue**

优先队列。缺省情况下是大顶堆（以vector表现得完全二叉树）实现。没有迭代器。

小顶堆：

```c
#include <queue>
priority_queue<int,vector<int>,greater<int>> pq;
```

**slist**

双向链表。插入操作会插入在指定元素前面。不提供push_back只提供push_front，因此元素次序和元素插入次序相反。

**set**

二叉搜索树具有对数平均时间的表现，前提是输入的数据有足够的随机性，否则会导致O(n)的复杂度。因此在STL中使用平衡二叉搜索树。

根据元素键值自动排序，由于RB-tree自动排序效果不错，因此使用RB-tree实现。

**map**

根据元素键值自动排序，由于RB-tree自动排序效果不错，因此使用RB-tree实现。

**multiset**

与set完全相同，唯一差别在于它允许键值重复。

**multimap**

与map完全相同，唯一差别在于它允许键值重复。

**hash_set**

STL的hash table使用的是开链的方式解决冲突。

hash_set基于hash table，元素没有顺序。

**hash_map**

hash_map基于hash table，元素没有顺序。

**hash_multiset**

与multiset完全相同，唯一差别在于它基于hash table 实现，元素没有顺序。

**hash_multimap**

与multimap完全相同，唯一差别在于它基于hash table 实现，元素没有顺序。

**unordered_map**

hash_table

**unordere_set**

insert：

```
1、m[n]++;
2、it->second++;
```

## 算法

* sort

**sort：**

默认升序排序，

降序：`sort(begin, end, greater<T>())`

cmp函数：`bool cmp(T &x, T &y){ return x > y; }`，返回true时表示将 x放到 y 前面。