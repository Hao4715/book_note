# 第七章

类的构造函数，拷贝，赋值，析构。

使用class与struct定义类的唯一区别在于默认的访问权限，class是privat，struct是public。

友元：允许其他类或者函数访问类的非公有成员，友元不是类的成员，不受所在区域访问控制级别的约束。友元不具有传递性。

可变数据成员：mutable关键字，即使是const对象中的该变量任然可以修改。