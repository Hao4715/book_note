# 容器
## 序列式容器 sequence containers
### array
array是固定大小的序列式容器。

`template < class T, size_t N > class array;`
### vector
vector是可变大小的数组。向后扩展，两倍扩展。

`template < class T, class Alloc = allocator<T> > class vector; // generic template`

vector的插入元素操作可能造成记忆体重新配置，导致原有迭代器全部失效。
### list
双向链表（实际是双向环状链表），插入操作以及接合操作（slpice）都不会造成原有的list迭代器失效。删除操作只会导致只想被删除元素的迭代器失效。
### duque
双向开口的连续线性空间，双向开口是指可在头尾两端分别做元素的插入和删除操作。

对deque进行排序操作，为了最高效率，可将deque先复制到一个vector，将vector排序后再复制回deque。

deque在逻辑上是连续的空间，实际上是由一段一段的定量连续空间构成。deque最大的任务便是在这些分段的定量连续空间上维护其整体连续的假象，并提供随机存取的接口，因而迭代器架构相对复杂。
### stack
没有遍历行为。不属于contiainer归类为container adapter，因为是以其他容器（deque，list）为底层实现的。
### queue
没有遍历行为。不属于contiainer归类为container adapter，因为是以其他容器（deque，list）为底层实现的。
### heap

### forward_list
## 关联式容器 associativ containers
### set/multiset
### mao/multimap
### unordered containers
#### unordered set/multiset
#### unordered map/multimap