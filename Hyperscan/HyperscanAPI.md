# Hyperscan API

[TOC]

官网：http://intel.github.io/hyperscan/dev-reference/api_files.html#

## hs.h

## hs_common.h

### hs_free_database

**函数原型：**

```c
hs_error_t hs_free_database(hs_database_t * db)
```

**功能：**释放已编译的模式数据库。该函数使用`hs_set_database_alloctor`（或`hs_set_allocator()`）设置的回调函数。

**返回值：**成功返回`HS_SUCCESS`。失败返回其他。

**参数：**

* `db:`编译好的模式数据库。也可以是NULL，这种情况下，该函数不进行任何处理。

### hs_serialize_database

**函数原型：**

```c
hs_error_t hs_serialize_database(const hs_database_t * db
                                 char ** bytes,
                                 size_t * length)
```

**功能：**将模式数据库序列化为一个字节流。该函数使用`hs_set_database_alloctor`（或`hs_set_allocator()`）设置的回调函数。

**返回值：**成功返回`HS_SUCCESS`。如果不能分配字节数组返回`HS_NOMEM`，如果出错会返回其他值。

**参数：**

* `db:`编译好的模式数据库。
* `bytes:`成功时，会返回字节数组指针。这些字节能够后续重定位或写道硬盘上。调用者负责释放块。
* `length:`成功时，此处返回生成的字节数组的字节数。

### hs_deserialize_database

**函数原型：**

```c
hs_error_t hs_deserialize_database(const char * bytes
                                 const size_t length,
                                 hs_database_t * db)
```

**功能：**从之前由`hs_aerialize_database()`生成的字节流中重新构造模式数据库。

该函数会使用`hs_set_database_allocator()`（或`hs_set_alloctor()`）设置的分配器来为数据库分配足够的空间；为了使用预分配的内存空间，使用`hs_daserialize_database_at()`函数。

**返回值：**成功返回`HS_SUCCESS`。失败返回其他值。

**参数：**

* `bytes:`由`hs_aerialize_database()`生成的代表已编译模式数据库的字节数组。
* `length:`由`hs_serialize_database()`生成的字节数组的长度。应该与`hs_serialize_database()`的返回值相同。
* `db:`成功时，此处会返回新分配的`hs_database_t`。噶爱数据库可用于扫描和使用`hs_free_database()`进行释放。

### hs_deserialize_database_at

**函数原型：**

```c
hs_error_t hs_deserialize_database(const char * bytes
                                 const size_t length,
                                 hs_database_t * db)
```

**功能：**从之前由`hs_aerialize_database()`生成的字节流中重新构造模式数据库。

该函数（与`hs_deserialize_database()`不同）会将重新构造的数据库写到`db`参数给定的内存中。所需的空间可以使用`hs_serialized_database_size()`函数进行计算。

**返回值：**成功返回`HS_SUCCESS`。失败返回其他值。

**参数：**

* `bytes:`由`hs_aerialize_database()`生成的代表已编译模式数据库的字节数组。
* `length:`由`hs_serialize_database()`生成的字节数组的长度。应该与`hs_serialize_database()`的返回值相同。
* `db:`指向一个足够存储反序列化数据库的8字节对齐的块内存空间。成功时，重新构造的数据库会写到该内存。该数据库接下来可以用于模式匹配。使用者负责释放内存；**不应使用`hs_free_database()`调用。**

### hs_stream_size

**函数原型：**

```c
hs_error_t hs_stream_size(const hs_database_t * database,
                            size_t * stream_size)
```

**功能：**提供针对给定数据库打开的单个流分配的流状态的大小。

**返回值：**成功返回`HS_SUCCESS`。失败返回其他。

**参数：**

* `database:`已编译（流模式）模式数据库的指针。
* `stream_size:`成功该参数会存储针对给定数据库打开的单个流分配的流状态的大小字节大小。

### hs_database_size

**函数原型：**

```c
hs_error_t hs_database_size(const hs_database_t * database,
                            size_t * database_size)
```

**功能：**提供给定数据库的字节大小。

**返回值：**成功返回`HS_SUCCESS`。失败返回其他。

**参数：**

* `database:`已编译模式数据库的指针。
* `database_size:`成功该参数会存储已编译数据库的字节大小。

### hs_serialized_database_size

**函数原型：**

```c
hs_error_t hs_serialized_database_size(const char * bytes,
                                       const size_t length,
                                       size_t * deserialized_size)
```

**功能：**报告将一个给定的序列化数据库，将其反序列化所需内存空间大小的工具函数。

**返回值：**成功返回`HS_SUCCESS`。失败返回其他。

**参数：**

* `bytes:`指向由`hs_serialize_database()`生成的字节数组的指针，代表一个编译好的模式数据库。
* `length:`由`hs_serialize_database()`生成的字节数组的长度。该值应该与`hs_serialize_database()`返回值相同。
* `deserialized_size:`成功会返回由`hs_deserialize_database_at()`生成的已编译数据库大小。

### hs_database_info

**函数原型：**

```c
hs_error_t hs_database_info(const hs_database_t * database,
                            char ** info)
```

**功能：**提供数据库信息的工具函数。

**返回值：**成功返回`HS_SUCCESS`。失败返回其他。

**参数：**

* `bytes:`已编译数据库的指针。
* `info:`成功该参数会返回提供的序列化数据库版本和平台信息的字符串。该字符串使用提供的分配器`hs_set_misc_allocator()`（或`malloc()`）分配并且由调用者自己释放。

### hs_serialized_database_info

**函数原型：**

```c
hs_error_t hs_serialized_database_info(const char  * bytes,
                                       size_t length,
                                       char ** info)
```

**功能：**提供序列化数据库信息的工具函数。

**返回值：**成功返回`HS_SUCCESS`。失败返回其他。

**参数：**

* `bytes:`序列化数据库的指针。
* `length:`序列化数据库的长度。
* `info:`成功该参数会返回提供的序列化数据库版本和平台信息的字符串。该字符串使用提供的分配器`hs_set_misc_allocator()`（或`malloc()`）分配并且由调用者自己释放。

### hs_set_allocator

**函数原型：**

```c
hs_error_t hs_set_allocator(hs_alloc_t alloc_func,hs_free_t free_func)
```

**功能：**设置Hyperscan为运行时流状态，暂存空间，数据库字节码和其他各种Hyperscan API返回的数据结构体分配内存空间的分配函数和释放函数。该函数与`hs_steam_allocator()` ,`hs_set_scratch_allocator()` ,`hs_set_database_allocator()` 和`hs_set_misc_allocator()` 功能相同。**该调用会重写之前已设置的任何数据库分配器。** 

**Note: there is no way to change how temporary objects created during the various compile calls (hs_compile(), hs_compile_multi(), hs_compile_ext_multi()) are allocated.**

**返回值：**成功返回`HS_SUCCESS`。失败返回其他。

**参数：**

* `alloc_func:`分配内存的回调函数指针。
* `free_func:`释放内存的回调函数指针。

### hs_set_database_allocator

**函数原型：**

```c
hs_error_t hs_set_database_allocator(hs_alloc_t alloc_func,hs_free_t free_func)
```

**功能：**设置Hyperscan为编译调用（`hs_compile()`，`hs_compile_multi()`，`hs_compile_ext_multi()`）和数据库序列化（`hs_daserialize_database()`）生成的数据库字节码分配内存的分配和释放函数。如果没有设置流分配函数或者两个参数都设为NULL，那么将使用默认的方法（例如系统`malloc()`和`free()`调用）。**该调用会重写之前已设置的任何数据库分配器。** 

**注意：数据库分配器也可能通过`hs_set_allocator()`调用设置。**

**Note: there is no way to change how temporary objects created during the various compile calls (hs_compile(), hs_compile_multi(), hs_compile_ext_multi()) are allocated.**

**返回值：**成功返回`HS_SUCCESS`。失败返回其他。

**参数：**

* `alloc_func:`分配内存的回调函数指针。
* `free_func:`释放内存的回调函数指针。

### hs_set_misc_allocator

**函数原型：**

```c
hs_error_t hs_set_misc_allocator(hs_alloc_t alloc_func,hs_free_t free_func)
```

**功能：**设置Hyperscan为Hyperscan API 返回项分配内存的分配和释放函数，例如`hs_compile_error_t`，`hs_expr_info_t`和序列化的数据库。如果没有设置流分配函数或者两个参数都设为NULL，那么将使用默认的方法（例如系统`malloc()`和`free()`调用）。**该调用会重写之前已设置的任何misc分配器。** **注意：misc分配器也可能通过`hs_set_allocator()`调用设置。**

**返回值：**成功返回`HS_SUCCESS`。失败返回其他。

**参数：**

* `alloc_func:`分配内存的回调函数指针。
* `free_func:`释放内存的回调函数指针。

### hs_set_scratch_allocator

**函数原型：**

```c
hs_error_t hs_set_scratch_allocator(hs_alloc_t alloc_func,hs_free_t free_func)
```

**功能：**设置`hs_alloc_stream()`和`hs_clone_scratch()`分配暂存空间时Hyperscan分配内存的分配和释放函数。如果没有设置流分配函数或者两个参数都设为NULL，那么将使用默认的方法（例如系统`malloc()`和`free()`调用）。**该调用会重写之前已设置的任何暂存分配器。** **注意：暂存分配器也可能通过`hs_set_allocator()`调用设置。**

**返回值：**成功返回`HS_SUCCESS`。失败返回其他。

**参数：**

* `alloc_func:`分配内存的回调函数指针。
* `free_func:`释放内存的回调函数指针。

### hs_set_stream_allocator

**函数原型：**

```c
hs_error_t hs_set_stream_allocator(hs_alloc_t alloc_func,hs_free_t free_func)
```

**功能：**设置`hs_open_stream()`为流状态下Hyperscan分配内存的分配和释放函数。如果没有设置流分配函数或者两个参数都设为NULL，那么将使用默认的方法（例如系统`malloc()`和`free()`调用）。**该调用会重写之前已设置的任何流分配器。** **注意：流分配器也可能通过`hs_set_allocator()`调用设置。**

**返回值：**成功返回`HS_SUCCESS`。失败返回其他。

**参数：**

* `alloc_func:`分配内存的回调函数指针。
* `free_func:`释放内存的回调函数指针。

### hs_version

**函数原型：**

```c
hs_error_t hs_version(void)
```

**功能：**识别当前版本的工具函数。

**返回值：**包含此发行的版本号和版本日期的字符串，静态分配资源不需要调用者释放。

**参数：无**

### hs_valid_platform

**函数原型：**

```c
hs_error_t hs_valid_platform(void)
```

**功能：**测试当前系统架构的工具函数。Hyperscan需要补充流式SIMD扩展3指令集。该函数能够在任何x86平台被调用来测试系统是否提供需要的指令集。

**返回值：**`HS_SUCCESS`表示编译成功。`HS_COMPILER_ERROR`表示编译失败，详析失败原因见 `error` 参数。

**参数：无**

## hs_compile.h

### hs_compile

**函数原型：**

```c
hs_error_t hs_compile(const char * expression,
                      unsigned int flags,
                      unsigned int mode,
                      const hs_platform_info_t * platform,
                      hs_database_t ** db,
                      hs_compile_error_t ** error)
```

**功能：**基础的正则表达式编译器，编译一条正则表达式生成一个能够传递给运行时函数（例如`hs_scan()`，`hs_open_stream()`）的Hyperscan数据库。

**返回值：**`HS_SUCCESS`表示编译成功。`HS_COMPILER_ERROR`表示编译失败，详析失败原因见 `error` 参数。

**参数：**

* `expression:`待编译的以NULL结尾的表达式。注意：该字符串参数仅代表待匹配的模式，不带定界符和标志。任何全局标志都应使用flags参数指定。例如：正则表达式`/abc?def/i`在编译时应该提供字符串`abc?def`作为`experssion`，并且指定`flags`参数为`HS_FLAG_CASELESS`。
* `flags:`修改表达式的行为。可以通过“或”运算使用多个标志。有效值参照官网。
* `mode:`影响整个数据库编译模式的标志。必须指定`HS_MODE_STREAM`或`HS_MODE_BLOCK`或`HS_MODE_VECTORED`其中之一，来选择使用生成流、块或向量数据库。其他以“HS_MODE”为首标记也可用来指定使用具体特点，详见编译模式标志。
* `platform:`如果非空，platform结构体会被用来决定数据库的目标格式。如果为空，会生成一个适于在当前机器的数据库。
* `db:`函数执行成功，该参数会返回生成的数据库指针；执行失败返回NULL。**调用者需要负责使用`hs_free_database()来释放分配的空间。`**
* `error:`若编译失败，会返回指向`hs_compile_error_t`的指针，来提供错误原因的详细信息。**调用者需要负责调用`hs_free_compile_error()`函数来释放分配的空间。**

### hs_compile_multi

**函数原型：**

```c
hs_error_t hs_compile_multi(const char *const * expressions,
                      const unsigned int * flags,
                      const unsigned int * ids,
                      unsigned int elements,
                      unsigned int mode,
                      const hs_platform_info_t * platform,
                      hs_database_t ** db,
                      hs_compile_error_t ** error)
```

**功能：**多正则表达式编译器，编译多条正则表达式生成一个能够传递给运行时函数（例如`hs_scan()`，`hs_open_stream()`）的Hyperscan数据库，每个表达式都可以用唯一的整数标记，该整数将传递到match回调中以标识已匹配的模式。

**返回值：**`HS_SUCCESS`表示编译成功。`HS_COMPILER_ERROR`表示编译失败，详析失败原因见 `error` 参数。

**参数：**

* `expression:`待编译的以NULL结尾的表达式。注意：该字符串参数仅代表待匹配的模式，不带定界符和标志。任何全局标志都应使用flags参数指定。例如：正则表达式`/abc?def/i`在编译时应该提供字符串`abc?def`作为`experssion`，并且指定`flags`参数为`HS_FLAG_CASELESS`。
* `flags:`修改表达式的行为的数组。可以通过“或”运算使用多个标志。指定NULL指针会将所有的标志设为0。有效值参照官网。
* `ids:`一个整数数组，该整数数组指定要与表达式数组中的相应模式相关联的ID号。空指针将会设置所有的模式ID为零。
* `elements:`输入数组中的元素个数。
* `mode:`影响整个数据库编译模式的标志。必须指定`HS_MODE_STREAM`或`HS_MODE_BLOCK`或`HS_MODE_VECTORED`其中之一，来选择使用生成流、块或向量数据库。其他以“HS_MODE”为首标记也可用来指定使用具体特点，详见编译模式标志。
* `platform:`如果非空，platform结构体会被用来决定数据库的目标格式。如果为空，会生成一个适于在当前机器的数据库。
* `db:`函数执行成功，该参数会返回生成的数据库指针；执行失败返回NULL。**调用者需要负责使用`hs_free_database()来释放分配的空间。`**
* `error:`若编译失败，会返回指向`hs_compile_error_t`的指针，来提供错误原因的详细信息。**调用者需要负责调用`hs_free_compile_error()`函数来释放分配的空间。**

### hs_compile_ext_multi

**函数原型：**

```c
hs_error_t hs_compile_ext_multi(const char *const * expressions,
                      const unsigned int * flags,
                      const unsigned int * ids,
                      const hs_expr_ext_t * const * ext,
                      unsigned int elements,
                      unsigned int mode,
                      const hs_platform_info_t * platform,
                      hs_database_t ** db,
                      hs_compile_error_t ** error)
```

**功能：**支持额外参数的多正则表达式编译器。该函数调用将一组正则表达式编译成与`hs_compile_multi()`一样的数据库，但是允许通过`hs_expr_ext_t`结构体为每个表达式指定额外的参数。

**返回值：**`HS_SUCCESS`表示编译成功。`HS_COMPILER_ERROR`表示编译失败，详析失败原因见 `error` 参数。

**参数：**

* `expression:`待编译的以NULL结尾的表达式。注意：该字符串参数仅代表待匹配的模式，不带定界符和标志。任何全局标志都应使用flags参数指定。例如：正则表达式`/abc?def/i`在编译时应该提供字符串`abc?def`作为`experssion`，并且指定`flags`参数为`HS_FLAG_CASELESS`。
* `flags:`修改表达式的行为。可以通过“或”运算使用多个标志。有效值参照官网。
* `ids:`一个整数数组，该整数数组指定要与表达式数组中的相应模式相关联的ID号。空指针将会设置所有的模式ID为零。
* `ext:`为每个模式指定额外参数的`hs_expr_ext_t`结构体指针数组。如果单个模式不需要扩展行为，则可以指定NULL；如果任何表达式都不需要扩展行为，则可以指定整个数组为NULL。**这些结构体时用的内存需要调用者进行申请和释放。**
* `elements:`输入数组中的元素个数。
* `mode:`影响整个数据库编译模式的标志。必须指定`HS_MODE_STREAM`或`HS_MODE_BLOCK`或`HS_MODE_VECTORED`其中之一，来选择使用生成流、块或向量数据库。其他以“HS_MODE”为首标记也可用来指定使用具体特点，详见编译模式标志。
* `platform:`如果非空，platform结构体会被用来决定数据库的目标格式。如果为空，会生成一个适于在当前机器的数据库。
* `db:`函数执行成功，该参数会返回生成的数据库指针；执行失败返回NULL。**调用者需要负责使用`hs_free_database()来释放分配的空间。`**
* `error:`若编译失败，会返回指向`hs_compile_error_t`的指针，来提供错误原因的详细信息。**调用者需要负责调用`hs_free_compile_error()`函数来释放分配的空间。**

### hs_compile_lit

**函数原型：**

```c
hs_error_t hs_compile_lit(const char * expressions,
                      unsigned flags,
                      const size_t len,
                      const hs_platform_info_t * platform,
                      hs_database_t ** db,
                      hs_compile_error_t ** error)
```

**功能：**基础的纯文字表达式编译器。该函数调用将一条**纯文字（不是正则表达式）**生成一个能够传递给运行时函数（例如`hs_scan()`，`hs_open_stream()`）的Hyperscan数据库。

**返回值：**`HS_SUCCESS`表示编译成功。`HS_COMPILER_ERROR`表示编译失败，详析失败原因见 `error` 参数。

**参数：**

* `expression:`待编译的以NULL结尾的表达式。注意：该字符串参数仅代表待匹配的模式，不带定界符和标志。任何全局标志都应使用flags参数指定。例如：正则表达式`/abc?def/i`在编译时应该提供字符串`abc?def`作为`experssion`，并且指定`flags`参数为`HS_FLAG_CASELESS`。同时，该字符串内容会被解释成不包含任何正则语法的纯字符串。
* `flags:`修改表达式的行为。可以通过“或”运算使用多个标志。有效值参照官网。
* `len:`一个纯字符串表达式的长度。由于`expression`知识的字符串内容是被视为单个字符逐个处理的，特殊的终止符`\0`应该被允许出现在表达式中，并且不被视为终止符来处理。因此，纯文字表达式不能通过识别`\0`来判断结束，而是应该通过表达式长度。
* `mode:`影响整个数据库编译模式的标志。必须指定`HS_MODE_STREAM`或`HS_MODE_BLOCK`或`HS_MODE_VECTORED`其中之一，来选择使用生成流、块或向量数据库。其他以“HS_MODE”为首标记也可用来指定使用具体特点，详见编译模式标志。
* `platform:`如果非空，platform结构体会被用来决定数据库的目标格式。如果为空，会生成一个适于在当前机器的数据库。
* `db:`函数执行成功，该参数会返回生成的数据库指针；执行失败返回NULL。**调用者需要负责使用`hs_free_database()来释放分配的空间。`**
* `error:`若编译失败，会返回指向`hs_compile_error_t`的指针，来提供错误原因的详细信息。**调用者需要负责调用`hs_free_compile_error()`函数来释放分配的空间。**

### hs_compile_lit_multi

**函数原型：**

```c
hs_error_t hs_compile_lit_multi(const char * const * expressions,
                      const unsigned* flags,
                      const unsigned *ids,
                      const size_t * lens,
                      const hs_platform_info_t * platform,
                      hs_database_t ** db,
                      hs_compile_error_t ** error)
```

**功能：**基础的纯文字表达式编译器。该函数调用将一条**纯文字（不是正则表达式）**生成一个能够传递给运行时函数（例如`hs_scan()`，`hs_open_stream()`）的Hyperscan数据库。

**返回值：**`HS_SUCCESS`表示编译成功。`HS_COMPILER_ERROR`表示编译失败，详析失败原因见 `error` 参数。

**参数：**

* `expression:`待编译的以NULL结尾的表达式。注意：该字符串参数仅代表待匹配的模式，不带定界符和标志。任何全局标志都应使用flags参数指定。例如：正则表达式`/abc?def/i`在编译时应该提供字符串`abc?def`作为`experssion`，并且指定`flags`参数为`HS_FLAG_CASELESS`。同时，该字符串内容会被解释成不包含任何正则语法的纯字符串。
* `flags:`修改表达式的行为的数组。可以通过“或”运算使用多个标志。指定空指针会将所有的标志设为0。有效值参照官网。
* `ids:`一个整数数组，该整数数组指定要与表达式数组中的相应模式相关联的ID号。空指针将会设置所有的模式ID为零。
* `lens:`纯字符串表达式的**长度数组**。由于`expression`知识的字符串内容是被视为单个字符逐个处理的，特殊的终止符`\0`应该被允许出现在表达式中，并且不被视为终止符来处理。因此，纯文字表达式不能通过识别`\0`来判断结束，而是应该通过表达式长度。
* `mode:`影响整个数据库编译模式的标志。必须指定`HS_MODE_STREAM`或`HS_MODE_BLOCK`或`HS_MODE_VECTORED`其中之一，来选择使用生成流、块或向量数据库。其他以“HS_MODE”为首标记也可用来指定使用具体特点，详见编译模式标志。
* `platform:`如果非空，platform结构体会被用来决定数据库的目标格式。如果为空，会生成一个适于在当前机器的数据库。
* `db:`函数执行成功，该参数会返回生成的数据库指针；执行失败返回NULL。**调用者需要负责使用`hs_free_database()来释放分配的空间。`**
* `error:`若编译失败，会返回指向`hs_compile_error_t`的指针，来提供错误原因的详细信息。**调用者需要负责调用`hs_free_compile_error()`函数来释放分配的空间。**

### hs_free_compile_error

**函数原型：**

```c
hs_error_t hs_free_compile_error(hs_compile_error_t * error)
```

**功能：**释放由函数`hs_compile()`，`hs_compile_multi()`或`hs_compile_ext_multi()`产生的错误信息结构体。

**返回值：**释放成功返回`HS_SUCCESS`。失败返回其他。

**参数：**

* `error:`待释放错误信息，传入 NULL 也是安全的。

### hs_expression_info

**函数原型：**

```c
hs_error_t hs_expression_info(const char * expression,
                      unsigned int flags,
                      hs_expr_info_t **info, 
                      hs_compile_error_t ** error)
```

**功能：**提供正则表达式信息的实用函数。`hs_expr_info_t`提供的信息包含模式匹配的最小和最大宽度。

**注意：该函数能够正确分析的正则表达式并不意味着对它的编译一定能成功（通过`hs_compile()`，`hs_compile_multi()`和`hs_compile_ext_multi()`编译）。该函数可能对Hyperscan不能编译的正则表达式返回`HS_SUCCESS`。**

**注意：一些单模式标志（例如`HS_FLAG_ALLOWEMPTY`，`HS_FLAG_SOM_LEFTMOST`）能被该调用接受，但是由于他们不影响`hs_expr_info_t`结构体的属性值，因此也不会影响该函数的输出。**

**返回值：**编译成功返回`HS_SUCCESS`。编译失败返回`HS_COMPILER_ERROR`，详析失败原因见 `error` 参数。

**参数：**

* `expression:`待编译的以NULL结尾的表达式。注意：该字符串参数仅代表待匹配的模式，不带定界符和标志。任何全局标志都应使用flags参数指定。例如：正则表达式`/abc?def/i`在编译时应该提供字符串`abc?def`作为`experssion`，并且指定`flags`参数为`HS_FLAG_CASELESS`。同时，该字符串内容会被解释成不包含任何正则语法的纯字符串。
* `flags:`修改表达式的行为。可以通过“或”运算使用多个标志。指定空指针会将所有的标志设为0。有效值参照官网。
* `info:`成功，该参数会返回指向模式信息的指针，失败返回NULL。**该结构体使用分配器提供的`hs_set_allocator()`（或没有设置分配器情况下的`malloc()`）进行内存分配，并且需要调用者自己释放。**
* `error:`若编译失败，会返回指向`hs_compile_error_t`的指针，来提供错误原因的详细信息。**调用者需要负责调用`hs_free_compile_error()`函数来释放分配的空间。**

### hs_expression_ext_info

**函数原型：**

```c
hs_error_t hs_expression_ext_info(const char * expression,
                      unsigned int flags,
                      const hs_ext_t *ext,
                      hs_expr_info_t **info, 
                      hs_compile_error_t ** error)
```

**功能：**提供支持额外参数的正则表达式信息的实用函数。`hs_expr_info_t`提供的信息包含模式匹配的最小和最大宽度。

**注意：该函数能够正确分析的正则表达式并不意味着对它的编译一定能成功（通过`hs_compile()`，`hs_compile_multi()`和`hs_compile_ext_multi()`编译）。该函数可能对Hyperscan不能编译的正则表达式返回`HS_SUCCESS`。**

**注意：一些单模式标志（例如`HS_FLAG_ALLOWEMPTY`，`HS_FLAG_SOM_LEFTMOST`）能被该调用接受，但是由于他们不影响`hs_expr_info_t`结构体的属性值，因此也不会影响该函数的输出。**

**返回值：**编译成功返回`HS_SUCCESS`。编译失败返回`HS_COMPILER_ERROR`，详析失败原因见 `error` 参数。

**参数：**

* `expression:`待编译的以NULL结尾的表达式。注意：该字符串参数仅代表待匹配的模式，不带定界符和标志。任何全局标志都应使用flags参数指定。例如：正则表达式`/abc?def/i`在编译时应该提供字符串`abc?def`作为`experssion`，并且指定`flags`参数为`HS_FLAG_CASELESS`。同时，该字符串内容会被解释成不包含任何正则语法的纯字符串。
* `flags:`修改表达式的行为。可以通过“或”运算使用多个标志。指定空指针会将所有的标志设为0。有效值参照官网。
* `ext:`指向为该模式定义了额外参数的`hs_expr_ext_t`结构体的指针。如果没有需要的额外参数，可设置为NULL。
* `info:`成功，该参数会返回指向模式信息的指针，失败返回NULL。**该结构体使用分配器提供的`hs_set_allocator()`（或没有设置分配器情况下的`malloc()`）进行内存分配，并且需要调用者自己释放。**
* `error:`若编译失败，会返回指向`hs_compile_error_t`的指针，来提供错误原因的详细信息。**调用者需要负责调用`hs_free_compile_error()`函数来释放分配的空间。**

### hs_populate_platform

**函数原型：**

```c
hs_error_t hs_populate_platform(hs_platform_info_t * platform)
```

**功能：**根据当前主机填充平台信息。

**返回值：**成功返回`HS_SUCCESS`。失败返回其他值。

**参数：**

* `expression:`成功时，指向基于当前主机的平台信息结构体。

## hs_runtime.h

### match_event_handler

**原型：**

```c
typedef int( * match_event_handler)(unsigned int id, 
                                    unsigned long long from, 
                                    unsigned long long to, 
                                    unsigned int flags, 
                                    void *context)
```

**功能：**匹配事件回调函数类型的定义。由`hs_scan()`,`hs_scan_vector`或`hs_scan_stream()`指定的匹配回调函数。当匹配发生时会调用该函数，匹配的细节会通过参数的方式传入该函数，并且该函数应该返回一个值来表示在目标数据上的匹配是否应该继续。如果依次扫描不需要回调，则可以使用NULL来压制匹配产生。

该回调函数不应该尝试对同一条流调用Hyperscan API函数并且不应该重用触发他的API调用分配的暂存空间。

**返回值：**匹配需要停止返回一个非零值，否则返会0。如果是在流模式下进行匹配并且返回非零值，任何对该流随后的`hs_scan_stream()`都会立即返回`HS_SCAN_TERMINATED`。

**参数：**

* `id:`匹配的正则表达式的ID。如果该表达式是通过`hs_compile()`编译的单个表达式，该值为0。
* `from:`
* `to:`匹配当前表达式的最后一个字节。
* `flags:`当前版本未使用。
* `context:`用户提供的指向`hs_scan()`，`hs_scan_vector()`或`hs_scan_stream()`函数的指针。

### hs_open_stream

**原型：**

```c
hs_error_t hs_open_stream(const hs_database_t * db, 
                          unsigned int flags, 
                          hs_stream_t ** stream)
```

**功能：**打开并初始化一条流。

**返回值：**成功返回`HS_SUCCESS`。失败返回其他值。

**参数：**

* `db:`一个编译好的模式数据库。
* `flags:`修改流行为的标志。当前未使用。
* `stream:`成功时，会返回指向生成的`hs_stream_t`的指针。失败返回NULL。

### hs_scan_stream

**原型：**

```c
hs_error_t hs_scan_stream(hs_stream_t * id, 
                          const char * data, 
                          unsigned int length, 
                          unsigned int flags, 
                          hs_scratch_t * scratch, 
                          match_event_handler onEvent, 
                          void * ctxt)
```

**功能：**将要扫描的数据写入打开的流。

**返回值：**成功返回`HS_SUCCESS`。如果匹配回调指明扫描需要停止返回`HS_SCAN_TERMINATED`。出错返回其他值。

**参数：**

* `id:`数据将被写入的流ID（由`hs_open_stream()`返回的）。
* `data:`指向将被扫描的数据的指针。
* `length:`要扫描的字节数。
* `flags:`修改流行为的标志。当前未使用。
* `scratch:`由`hs_alloc_acratch()`分配的单线程暂存空间。
* `onEvent:`指向匹配事件回调函数的指针。如果指定为NULL，不会返回匹配。
* `ctxt:`用户定义的函数指针，会在匹配发生时传递给回调函数。

### hs_close_stream

**原型：**

```c
hs_error_t hs_close_stream(hs_stream_t * id, 
                           hs_scratch_t * scratch, m
                           atch_event_handler onEvent, 
                           void * ctxt)
```

**功能：**关闭流。

该函数完成给定流的匹配并且释放流状态相关的内存。该调用完成后由`id`指定的流会失效并且不能再被使用。为了在完成后重用该流，可以使用`hs_reset_stream()`函数重用它而不是关闭。

Note: This operation may result in matches being returned (via calls to the match event callback) for expressions anchored to the end of the data stream (for example, via the use of the `$` meta-character). If these matches are not desired, NULL may be provided as the [match_event_handler](http://intel.github.io/hyperscan/dev-reference/api_files.html#hs__runtime_8h_1a08d3ace9d76281e92d0bab63eef9a98b) callback.

**返回值：**成功返回`HS_SUCCESS`。失败返回其他值。

**参数：**

* `id:`由`hs_open_stream()`返回的流ID。
* `scratch:`由`hs_alloc_acratch()`分配的单线程暂存空间。当`onEvent`回调为NULL时，该值可指定为NULL。
* `onEvent:`指向匹配事件回调函数的指针。如果指定为NULL，不会返回匹配。
* `ctxt:`用户定义的函数指针，会在匹配发生时传递给回调函数。

### hs_reset_stream

**原型：**

```c
hs_error_t hs_reset_stream(hs_stream_t * id, 
                           unsigned int flags, 
                           hs_scratch_t * scratch, 
                           match_event_handler onEvent, 
                           void * context)
```

**功能：**将流重置为初始化状态。

概念上讲，等同于在给定流上执行`hs_close_stream()`再调用`hs_open_stream()`。新的流替代原始流的内存位置，避免了释放和分配新空间的开销。

Note: This operation may result in matches being returned (via calls to the match event callback) for expressions anchored to the end of the original data stream (for example, via the use of the `$` meta-character). If these matches are not desired, NULL may be provided as the [match_event_handler](http://intel.github.io/hyperscan/dev-reference/api_files.html#hs__runtime_8h_1a08d3ace9d76281e92d0bab63eef9a98b) callback.

Note: the stream will also be tied to the same database.

**返回值：**成功返回`HS_SUCCESS`。失败返回其他值。

**参数：**

* `id:`需要被重置的流（由`hs_open_stream()`创建）。
* `flags:`修改流行为的标志。暂未使用。
* `scratch:`由`hs_alloc_acratch()`分配的单线程暂存空间。当`onEvent`回调为NULL时，该值可指定为NULL。
* `onEvent:`指向匹配事件回调函数的指针。如果指定为NULL，不会返回匹配。
* `context:`用户定义的函数指针，会在匹配发生时传递给回调函数。

### hs_copy_stream

**原型：**

```c
hs_error_t hs_copy_stream(hs_stream_t ** to_id, 
                          const hs_stream_t * from_id)
```

**功能：**复制给定的流。新的流与原始流有相同的状态，包括相同的当前流偏移。

**返回值：**成功返回`HS_SUCCESS`。失败返回其他值。

**参数：**

* `to_id:`成功时会返回指向新的`gs_stream_t`的流指针。失败返回NULL。

* `from_id:`执行被拷贝的流（由`hs_open_stream()`创建）。

### hs_reset_and_copy_stream

**原型：**

```c
hs_error_t hs_reset_and_copy_stream(hs_stream_t * to_id, 
                                    const hs_stream_t * from_id, 
                                    hs_scratch_t * scratch, 
                                    match_event_handler onEvent, 
                                    void * context)
```

**功能：**将`from`流状态复制给`to`流。`to`流会先被重置（如果`onEvent`回调函数被指定为非空，会报告EOD匹配）。

**注意：`to`流和`from`流应该时基于同一个数据库创建。**

**返回值：**成功返回`HS_SUCCESS`。失败返回其他值。

**参数：**

* `to_id:`成功时会返回指向新的`gs_stream_t`的流指针。失败返回NULL。
* `from_id:`执行被拷贝的流（由`hs_open_stream()`创建）。
* `scratch:`由`hs_alloc_acratch()`分配的单线程暂存空间。当`onEvent`回调为NULL时，该值可指定为NULL。
* `onEvent:`指向匹配事件回调函数的指针。如果指定为NULL，不会返回匹配。
* `context:`用户定义的函数指针，会在匹配发生时传递给回调函数。

### hs_compress_stream

**原型：**

```c
hs_error_t hs_compress_stream(const hs_stream_t * stream, 
                              char * buf, 
                              size_t buf_space, 
                              size_t * used_space)
```

**功能：**在提供的缓冲区中创建提供的流的压缩表示形式。该压缩表示形式可以通过`hs_expand_stream()`或`hs_reet_and_expand_stream()`转换回流状态。压缩表示形式占用的空间会写到`used_space`中。如果没有足够的空间存储压缩形式，会返回`HS_INSUFFICIENT_SPACE`并且`used_space`回返回所需的空间大小。

**注意：该函数不会关闭提供的流，人可以继续使用该流回通过`hs_close_stream()`来释放它。**

**注意：`to`流和`from`流应该时基于同一个数据库创建。**

**返回值：**成功返回`HS_SUCCESS`。如果给定的空间不足会返回`HS_INSUFFICIENT_SPACE`。

**参数：**

* `stream:`要被压缩的流（由`hs_open_stream()`创建）。

* `buf:`将压缩形式写入的空间。**注意：如果该调用只是用来计算需要的空间大小，那么这里可以填入NULL，并且指定`buf_space`为0。**

* `buf_space:` `buf`的字节数。如果`buf`空间不足，该调用会因为`HS_INSUFFICIENT_SPACE`儿失败。

* `used_space:`指向使用空间大小的指针。所用空间应该小于或等于`buf_space`。如果调用因为`HS_INSUFFICIENT_SPACE`儿失败，该参数会返回所需空间大小。

### hs_expand_stream

**原型：**

```c
hs_error_t hs_expand_stream(const hs_database_t * db, 
                            hs_stream_t ** stream, 
                            const char * buf, 
                            size_t buf_size)
```

**功能：**将由`hs_compress_stream()`创建的压缩表示解压为一个新的流。

**注意：`buf`必须对应于针对`db`打开的流的`hs_compress_stream()`创建的完整压缩表示形式。如果不满足这些属性，则并非总是能检测到此API的滥用，并且行为未定义。**

**注意：`to`流和`from`流应该时基于同一个数据库创建。**

**返回值：**成功返回`HS_SUCCESS`。如果给定的空间不足会返回`HS_INSUFFICIENT_SPACE`。

**参数：**

* `db:`打开压缩流的已编译模式数据库。
* `stream:`成功时会返回扩展的`hs_stream_t`指针。失败返回NULL。
* `buf:` 一个流的压缩表示形式。该压缩形式由`hs_compress_stream()`创建。
* `buf_size:`压缩表示形式的字节大小。

### hs_reset_and_expand_stream

**原型：**

```c
hs_error_t hs_expand_stream(hs_stream_t * to_stream, 
                            const char * buf, 
                            size_t buf_size, 
                            hs_scratch_t * scratch, 
                            match_event_handler onEvent, 
                            void * context)
```

**功能：**将由`hs_compress_stream()`创建的压缩解压到`to`流顶部。`to`流会先被重置。

**注意：`to`流必须与压缩形式时针对同一数据库打开的。**

**注意：`buf`必须对应于针对`db`打开的流的`hs_compress_stream()`创建的完整压缩表示形式。如果不满足这些属性，则并非总是能检测到此API的滥用，并且行为未定义。**

**返回值：**成功返回`HS_SUCCESS`。如果给定的空间不足会返回`HS_INSUFFICIENT_SPACE`。

**参数：**

* `to_stream:`指向有效流状态的指针。成功时会返回指向`hs_stream_t`的指针，失败返回NULL。
* `buf:`流的压缩表示形式。由`hs_compress_stream()`创建。
* `buf_size:` 压缩表示形式的字节大小。
* `scratch:`由`hs_alloc_scratch()`创建的单线程暂存空间。当`onEvent`为NULL时，该参数也可为NULL。
* `onEvent:`回调函数的的匹配指针。若为空，不会返回匹配。
* `context:`用户定义的当匹配发生时传给回调函数的指针。

### hs_scan

**原型：**

```c
hs_error_t hs_expand_stream(const hs_database_t * db, 
                            const char * data, 
                            unsigned int length, 
                            unsigned int flags, 
                            hs_scratch_t * scratch, 
                            match_event_handler onEvent, 
                            void * context)
```

**功能：**块正则表达式扫描器（非流式）。

**返回值：**成功返回`HS_SUCCESS`。若匹配回调指示停止扫描返回`HS_SCAN_TERMINATED`。出错返回其他值。

**参数：**

* `db:`已编译的模式数据库。
* `data:`待扫描数据指针。
* `length:` 待扫描长度。
* `flags:`修改函数行为的标志。暂时未用。
* `scratch:`由`hs_alloc_scratch()`为该数据库创建的暂存空间。
* `onEvent:`调函数的的匹配指针。若为空，不会返回匹配。
* `context:`用户定义的当匹配发生时传给回调函数的指针。

### hs_scan_vector

**原型：**

```c
hs_error_t hs_scan_vector(const hs_database_t * db, 
                          const char *const * data, 
                          const unsigned int * length, 
                          unsigned int count, 
                          unsigned int flags, 
                          hs_scratch_t * scratch, 
                          match_event_handler onEvent, 
                          void * context)
```

**功能：**向量正则表达式扫描器（非流式）。

**返回值：**成功返回`HS_SUCCESS`。若匹配回调指示停止扫描返回`HS_SCAN_TERMINATED`。出错返回其他值。

**参数：**

* `db:`已编译的模式数据库。
* `data:`待扫描数据指针数组。
* `length:` 待扫描长度数组。
* `count:`需扫描的数据块数量。应该与`data`以及`length`数量一致。
* `flags:`修改函数行为的标志。暂时未用。
* `scratch:`由`hs_alloc_scratch()`为该数据库创建的暂存空间。
* `onEvent:`调函数的的匹配指针。若为空，不会返回匹配。
* `context:`用户定义的当匹配发生时传给回调函数的指针。

### hs_alloc_scratch

**原型：**

```c
hs_error_t hs_alloc_scratch(const hs_database_t * db, hs_scratch_t ** scratch)
```

**功能：**为Hyperscan的使用分配一个暂存空间。这对于运行时使用是必需的，并且每个线程或并发调用者都需要一个临时空间。此函数将使用hs_set_scratch_allocator（）或hs_set_allocator（）设置的任何分配器回调。

**返回值：**分配成功返回`HS_SUCCESS`。分配失败返回`HS_NOMEM`。出错返回其他错误。

**参数：**

* `db:`由`hs_compile()`产生的数据库。

* `scratch:`见官网。

### hs_clone_scratch

**原型：**

```c
hs_error_t hs_clone_scratch(const hs_scratch_t * src, hs_scratch_t ** dest)
```

**功能：**从已有的暂存空间拷贝并分配一个新的暂存空间。

**返回值：**成功返回`HS_SUCCESS`。出错返回其他错误。

**参数：**

* `src:`已存在的待拷贝暂存空间。

* `dest:`指向新的暂存空间的指针。

### hs_scratch_size

**原型：**

```c
hs_error_t hs_scratch_size(const hs_scratch_t * scratch, size_t * scratch_size)
```

**功能：**返回暂存空间的大小。

**返回值：**成功返回`HS_SUCCESS`。出错返回其他错误。

**参数：**

* `scratch:`由`hs_alloc_scratch()`或`hs_clone_scratch()`分配的单线程暂存空间。

* `scratch_size:`成功时返回暂存空间的字节大小。

### hs_free_scratch

  **原型：**

```c
hs_error_t hs_free_scratch(hs_scratch_t * scratch)
```

**功能：**释放之前由`hs_alloc_scratch()`或`hs_clone_scratch()`分配的暂存空间。该函数使用`hs_set_scratch_allocator()`或`hs_set_allocator()`设置的释放回调函数。

**返回值：**成功返回`HS_SUCCESS`。出错返回其他错误。

**参数：**

* `scratch:`待释放暂存空间。NULL也是在安全的参数。




  

  

  


