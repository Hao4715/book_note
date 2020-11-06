# 容器
## 序列式容器 sequence containers
### array
array是固定大小的序列式容器。

`template < class T, size_t N > class array;`
### vector
vector是可变大小的数组。向后扩展，两倍扩展。

`template < class T, class Alloc = allocator<T> > class vector; // generic template`

vector的插入元素操作可能造成记忆体重新配置，导致原有迭代器全部失效。
### duque
### list
双向链表，插入操作以及接合操作（slpice）都不会造成原有的list迭代器失效。删除操作只会导致只想被删除元素的迭代器失效。
### forward_list
## 关联式容器 associativ containers
### set/multiset
### mao/multimap
### unordered containers
#### unordered set/multiset
#### unordered map/multimap