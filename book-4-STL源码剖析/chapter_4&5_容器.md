# 容器
## 序列式容器 sequence containers
### array
array是固定大小的序列式容器。

`template < class T, size_t N > class array;`
### vector
vector是可变大小的数组。向后扩展，两倍扩展。

`template < class T, class Alloc = allocator<T> > class vector; // generic template`
### duque
### list
### forward_list
## 关联式容器 associativ containers
### set/multiset
### mao/multimap
### unordered containers
#### unordered set/multiset
#### unordered map/multimap