# Hyperscan简介

[TOC]

## 前言

正则表达式匹配是现代网络安全应用的关键技术，但是当它在处理包有效载荷中计算密集型的字节扫描时通常会成为系统的性能瓶颈。随着网络带宽和大规则集复杂匹配模式的增加，性能要求变得更加严苛。

Hyperscan，一个高性能正则表达式匹配工具。他的高性能模式匹配采用了两种关键技术：**首先**，他利用图分解将正则表达式匹配转换为一系列字符串和有限自动机匹配，与现有的解决方案（字符串匹配转换为一部分正则表达式匹配）相比消除了重复操作，而且由于分解的正则表达式组件通常比原始模式简单也增加了DFA快速匹配的机会。**其次**，Hyperscan通过使用SIMD指令加快字符串和有限自动机匹配。

**注**

* SIMD，Single Instruction Multiple Data：一条指令操作多个数据。以加法指令为例，单指令单数据（SISD）的CPU对加法指令译码后，执行部件先访问内存，取得第一个操作数；之后再一次访问内存，取得第二个操作数；随后才能进行求和运算。而在SIMD型的CPU中，指令译码后几个执行部件同时访问内存，一次性获得所有操作数进行运算。这个特点使SIMD特别适合于多媒体应用等数据密集型运算。

## 概览

Hyperscan网站：http://intel.github.io/hyperscan/dev-reference/preface.html

Hyperscan是一个提供高性能、可同时匹配多个表达式和灵活的正则表达式引擎。

将匹配模式传递给一个编译接口以生成一个不可变的模式数据库。随后可以使用扫描接口来根据给定的模式扫描目标缓冲区并从数据缓冲区返回匹配结果。Hyperscan也提供了**流**模式，它可以检测一个流中跨多个块的匹配。

## 介绍

Hyperscan API由两个主要部分组成：Compilation和Scanning。

### Compilation

这些功能函数使用一组带有标识和操作标志的正则表达式，并将他们编译成一个不可更待的数据库提供给Hyperscan扫描API使用。Compilation过程进行大量的分析和优化工作以高效的建立与给定表达式相符的数据库。

如果一个模式因为任何可能的原因（例如不支持的表达式结构或资源限制下的溢出）而不能生成一个数据库，模式编译过程会报错。

编译好的数据库时序列化并且可重定位的，以保证他们能够被存储到磁盘或转移到其他主机。还可以针对特定平台的特征进行生成。

### Scanning

Hyperscan数据库被建立后，他就可以被用来扫描内存中的数据。Hyperscan提供了多种不同的扫描模式，模式选择取决于被扫描数据是在**一个连续块中**还是**同时存储在内存中分散的不同块中**抑或是**一个流中的一连串的块**。

所有的匹配通过用户提供的回调函数交付给应用，这些函数对每个匹配都是同步的。

## 编译待匹配的模式

http://intel.github.io/hyperscan/dev-reference/compilation.html

### 编译待匹配模式模式生成数据库

Hyperscan compiler API 接收正则表达式并且将他们转换成能够用于扫描数据的编译完成的模式数据库。

它提供了将正则表达式编译成数据库的三个函数：

1. `hs_compile()`：将**单个表达式**编译成一个模式数据库。

2. `hs_compile_multi()`：将**一组表达式**编译成一个模式数据库。在扫描时所有提供的模式会同时被扫描，当有匹配时会返回用户提供的标识。

3. `hs_compile_ext_multi()`：与上一个函数一样允许将**一组表达式**编译成一个模式数据库，但是允许为每个表达式指定[额外的参数](http://intel.github.io/hyperscan/dev-reference/compilation.html#extparam)

Hyperscan库通过编译来分析给定的模式并且预决定如何以优化的方式来执行模式的扫描，这运行时进行这些工作时消耗巨大的。

在编译模式是，需要确定编译模式的结果是用于流、块还是向量模式：

* **流模式**：被扫描的目标数据是一个持续的流，数据并不能一次全部获取；数据中的块是按顺序扫描并且匹配或许会跨越流中的多个块。在流模式下，每个流需要一块内存来存储两次扫描调用之间的状态。
* **块模式**：被扫描的目标数据是离散的、连续的、能够在一次调用扫描完成并且不需要保存状态的块。
* **向量模式**：被扫描的目标数据由能一次获取的非连续块列表组成。与块模式相同，不需要保存状态。

为了编译流模式下使用的匹配模式，`hs_compile`的`mode`参数必须设置为`HS_MODE_STREAM`；同样的块模式需要设置为`HS_MODE_BLOCK`以及向量模式的`HS_MODE_VECTORED`。为某一模式（流模式，块模式和向量模式）生成的模式数据库只能在该模式下使用。用来生成编译完成的模式数据库的Hyperscan版本必须与用来扫描的Hyperscan版本一致。

### 编译纯文字

纯文字是一种特殊类型的正则表达式。当且仅当一个字符序列中的字符被独立的读和解释时认为该字符序列是纯文字。也就是说在任何相邻的字符之间不存在任何的句法联系。

例如，形如`/bc?/`的表达式，可以说是一个表示单个字符b或者两个字符bc的正则表达式，另一方面也可以说是一个长度为3包含字符b，c和？的字符序列。在正则表达式情况下，`?`表示名为“0-1量词”的特定语法角色，他与位于它前面的一个字符有特定的句法联系，与他相似的字符还有`[`,`]`,`(`,`)`,`{`,`}`,`-`,`*`,`+`,`\`,`|`,`/`,`:`,`^`,`.`,`$`。当这些**元符号（正则表达式中有特殊意义的字符）**出现在单纯的文字情况下他们就失去其他额外的意义只是单纯的ASCII码。

Hyperscan最初就是设计为处理通用正则表达式。因此内嵌了一个复杂的解释器来完成全面的正则表语法解释工作。对于上文中纯字符（不带任何意义的字符）的识别是解释更复杂正则语法的基本工作。

然而在实际情况中，匹配模式不一定是正则表达式。可能只是纯文字。这是如果纯文字中包含元字符就会产生问题。如果直接交给传统的Hyperscan编译API，那么所有这些元字符会按照预设的方式进行解释，这不仅是不必要的而且产生的结果也是不符合实际预期的。为了避免这种情况，用户需要通过将元字符转换成其他格式的方式对这些纯文字的模式进行预处理：在某些元字符前添加反斜杠`\`或将所有字符转换成十六进制的表示。

在`v5.2.0`版本中，Hyperscan为纯文字匹配模式提供了两个新的编译API：

1. `hs_compile_lit()`：将**单个纯文字**匹配模式编译成一个模式数据库。
2. `hs_compile_lit_multi()`：将**一组纯文字**匹配模式编译成一个模式数据库。在扫描时所有提供的模式会同时被扫描，当有匹配时会返回用户提供的标识。

这两个API是为匹配模式是纯文字的情况下设计的。用户可以将原始数据直接传入这两个API而不用担心匹配模式中包含正则元数据。不需要任何预处理工作。

**对于新的API，为每一个文字匹配模式增加了一个`length`参数。Hyperscan需要通过明确的知道文字的长度来定位输入表达式的结尾而不是通过简单的识别字符串中的`\0`字符。**

支持的标记（flags）：`HS_FLAG_CASELESS`,`HS_FLAG_SINGLEMATCH`,`HS_FLAG_SOM_LEFTMOST`。

**注：**纯文字API不支持[额外的参数](http://intel.github.io/hyperscan/dev-reference/compilation.html#extparam)。对于runtime，传统的runtimeAPI仍然可以用于纯文字匹配。

**注：**如果目标规则中含有至少一条正则表达式，应该使用传统编译API（ `hs_compile()`, `hs_compile_multi()`和 `hs_compile_ext_multi()`），新的纯文本API是为单纯的纯文本表达式设计的。

### 支持的匹配模式

**Hyperscan支持PCRE库（“libpcre”，在http://www.pcre.org/有详细描述）使用的模式句法。但不是所有libpcre结构都支持。使用不支持的结构会产生编译错误。**

#### 支持的结构

http://intel.github.io/hyperscan/dev-reference/compilation.html#supported-constructs

#### 不支持的结构

http://intel.github.io/hyperscan/dev-reference/compilation.html#unsupported-constructs

### 语义

尽管Hyperscan遵从libpcre句法，但是它提供不同的语义。与libpcre语义的主要区别在于流和多个同时模式匹配的需求。

与libpcre语义的主要区别：

* **多模式匹配：**Hyperscan可以同时报告多个模式的匹配。这与libpcre中多个模式使用`|`分割的方式不同，这种方式是从所至右交替评估。
* **缺乏次序：**尽管Hyperscan始终位于当前扫描的范围内，但是并不保证多匹配是有序的。
* **仅支持尾偏移：**Hyperscan默认行为是只报告匹配的尾偏移。可以通过在模式编译时使用`pre-expression`标记的方式开启首偏移报告。
* **报告“所有匹配”：**在`fooxyzbarbar`上执行`/foo.*bar/`匹配规则在Hyperscan中会返回两个匹配--`fooxyzbar`和`fooxyzbarbar`。相反，libpcre语义默认只报告一个匹配--`fooxyzbarbar`（贪婪语义：greedy semantics），如果开启非贪婪语义会报告一个匹配--`fooxyzbar`。**这意味着，Hyperscan中贪婪语义和非贪婪语义之间的切换操作实际上是无操作。**

### 开始匹配（Start of Match）

标准操作下，当匹配回调函数被调用后Hyperscan只会报告匹配的尾偏移。如果某一特定匹配模式`HS_FLAG_SOM_LEFTMOST`的标记被指定，会返回相同的匹配集合，但是每个匹配还会提供与尾偏移相符的可能的最左端偏移。

使用SOM标记需要有一些权衡和限制：

* 减少模式支持（会导致Hyperscan对一些复杂匹配模式的支持出错）：对于许多匹配模式，追踪SOM是非常复杂的并且可能导致Hyperscan编译该模式失败而报错“Pattern too large”，即使该模式在正常情况下是可以被支持的。
* 增加流状态：扫描时状态空间用于追踪潜在的SOM偏移，并且在流模式下这下状态必须存储在持久的流状态中。因此，SOM会逐渐增加匹配一个模式所需的流状态。
* 性能开销：追踪SOM通常会造成性能开销。
  * 特征不相容：一些Hyperscan模式标记（例如`HS_FLAG_SINGLEMATCH`和`HS_FLAG_PREFILTER`）不能与SOM组合使用。将他们与`HS_FLAG_SOM_LEFTMOST`一起指定会导致编译错误。

### 扩展参数

在某些情况下，需要比使用正则表达式语法轻松指定的方式更多地控制模式的匹配行为。此时，Hyperscan提供了`hs_compile_ext_multi()`方法可以指定一系列“额外的参数”。

额外参数通过使用`hs_expr_ext_t`结构体指定，它提供如下值域：

* `flags`：管理该结构体中哪些参数被使用。
* `min_offset`：数据流中尾偏移的最小值，在该范围内表达式应该匹配成功。
* `max_offset`：数据流中尾偏移的最大值，在该范围内表达式应该匹配成功。
* `min_length`：匹配表达式的最小匹配长度（从头到尾）
* `edit_diatance`：在给定的编辑距离（Levenshtein distance，量测方式是看至少需要多少次的处理才能将一个字符串变成另一个字符串。）中匹配表达式。
* `hamming_diatance`：在给定的汉明距离（Hamming distance，表示两个等长字符串在对应位置上不同字符的数目）中匹配表达式。

这些参数或者允许在编译时限制由模式产生的匹配集（而不是依赖应用程序在运行时处理不需要的匹配），或者允许近似地匹配模式（在给定的编辑距离内）以产生更多的匹配 。

### 预过滤模式

### 指令集专业化

### 近似匹配

Hyperscan提供了实验性的近似匹配模式，它会根据给定的编辑距离进行模式匹配。匹配行为定义如下：

* **编辑距离**
* **汉明距离**
* **近似匹配**

### 逻辑组合



## 扫描待匹配的模式

Hyperscan提供三种不同的扫描模式，没种都有各自以`HS_SCAN`开头的函数。此外，流模式有一些其他的API函数来管理流状态。

### Handling Matches

所有此类函数在匹配成功时都会调用一个用户提供的回调函数。函数定义如下：

`*typedef* int`**( * match_event_handler)**`(unsigned int *id*, unsigned long long *from*, unsigned long long *to*, unsigned int *flags*, void **context*)`

`id`参数会设置到编译时提供的匹配表达式的标识中，`to`参数设置为匹配的尾偏移。如果未匹配模式设置了SOM，`from`参数会设置为匹配最左端可能的首偏移。

匹配回调函数能够通过返回一个非零值以停止扫描。参考[match_event_handler](http://intel.github.io/hyperscan/dev-reference/api_files.html#c.match_event_handler)获取更多信息。

### 流模式

Hyperscan流运行时API核心包括开启、扫描和关闭Hyperscan数据流函数：

* `hs_open_stream()`：为扫描分配和初始化一个新的流。
* `hs_scan_stream()`：扫描给定流中的数据块，并在检测到匹配时引发匹配。
* `hs_close_stream()`：完成给定流的匹配并且释放流状态。在调用`hs_close_stream()`后，流句柄就会失效并且无论如何不应该再使用它。

随着扫描任何检测到的匹配都通过一个函数指针回调返回给调用应用。

匹配回调函数能够通过返回一个非零值来停止扫描当前数据流。在流模式下，会导致该流停留在一个状态，在该状态下其余数据不会被扫描，并且随后任何对该流调用`hs_scan_stream()`的行为都会立即返回`HS_SCAN_TERMINATED`。调用者仍然必须调用`hs_close_stream()`来完成对该流的清理工作。

流存留在Hyperscan库中，这样目标流的跨多个块的模式匹配状态能够保存--不保存状态将不可能实现数据中跨块的匹配。因而，每个流需要更多的存储消耗，管理这些状态也会带来性能消耗。

#### 流管理

除`hs_open_stream()`,`hs_scan_stream()`和`hs_cloae_stream()`之外，Hyperscan API还提供了其他的函数来管理流：

* `hs_reset_stream()`：将流重置到初始状态；与`hs_close_stream()`相同，但不会释放流状态使用的内存。
* `hs_copy_stream()`：构造流的副本（重新分配）。
* `hs_reset_and_copy_stream()`：先重置目标流，再将待复制的流创建副本到目标流。避免了`hs_copy_stream()`的重新分配。

#### 流压缩

一个流对象被分配了固定大小的空间，固定大小的空间来保证在执行扫描操作时不需要进行内存分配工作。当系统内存不够用时，可以通过减少近期不会使用的流的空间使用会减小内存压力。为此，Hyperscan API提供了用于将流与压缩表示形式进行转换的调用。压缩的表示形式与完整的流对象不同，因为它不为给定当前流状态的组件保留空间。Hyperscan API函数如下：

* `hs_compress_stream()`：使用流压缩后表示形式填充提供的缓冲区并返回压缩形式占用的字节数。如果缓冲区不够大来存储压缩形式，`HS_INSUFFICIENT_SPACE`会随需要的大小一起返回。该调用不会以任何形式修改原始流：他仍然可以被写进函数`hs_scan_stream()`、用于各种重置函数来重新初始化流的状态或者使用`hs_close_stream()`调用来释放流资源。
* `hs_expand_stream()`：基于存储压缩形式的缓冲区创建一个新的流。
* `hs_reset_and_expand_stream()`：先重置已存在的流，然后在已存在的流的基础上根据包含压缩形式的缓冲区构造一个新的流。该调用避免了`hs_expand_stream()`的重新分配。

**注：不推荐在每个调用之间都是用流压缩，因为性能原因，在标准流和压缩形式之间转换会花费时间。**

### 块模式

块模式的运行时API只包含一个函数：`hs_scan()`。该函数使用编译的匹配模式来匹配目标数据中的匹配，使用一个指针回调函数与应用通信。

单个的`hs_scan()`函数本质上与调用`hs_open_stream()`，单独调用`hs_scan_stream()`随后调用`hs_close_stream()`是一样的，除此之外块模式不会引发流模式的其他消耗等问题。

### 向量模式

向量模式的运行时API与块模式API相同，只包含一个函数：`hs_scan_vector()`。该函数接受一组数据指针和长度，按照次序扫描一个在内存中不连续的数据块集合。

从调用来看，这个模式与流扫描模式下按一定次序扫描数据块或将数据块按次序拷贝到一个块中进行块匹配产生相同的匹配。

### 暂存空间（Scratch Space）

在扫描数据时，Hyperscan需要少量的临时内存来存储即使内部数据。不幸的是这个数量对于栈来说过于庞大，尤其对于嵌入式系统，并且动态分配内存太过于消耗，因此必须为扫描函数提供一个预分配的“暂存”空间。

函数`hs_alloc_scrach()`为提供的数据库分配一个足够大的内存区域作为暂存区。如果应用使用多个数据库，是需要一个暂存区：这种情况下，为每个数据库调用`hs_alloc_scrach()`（使用同一个`scrach`指针）会保证足够大的暂存区以支持给定的数据库中的任何一个数据库。

尽管Hyperscan是可重入的，但是暂存区的使用并不是可重入的。例如，如果设计一个认为是递归或嵌套的扫描，那么这个上下文需要一个额外的暂存区。

在**递归扫描以外的情况下**，每个线程只需要一个暂存空间并且可以（确实需要）在扫描开始之前进行空间分配。

在**一系列表达式由“主”线程编译并且数据由多个“工作”线程扫描的情况下**，`hs_clone_scratch`函数能够方便的将已有的暂存空间进行拷贝供每个线程使用（而不是强迫调用者将编译后的数据库通过`hs_alloc_scrach()`传递多次）。

例：

```c
hs_error_t err;
hs_scratch_t *scratch_prototype = NULL;
err = hs_alloc_scratch(db, &scratch_prototype);
if (err != HS_SUCCESS) {
    printf("hs_alloc_scratch failed!");
    exit(1);
}

hs_scratch_t *scratch_thread1 = NULL;
hs_scratch_t *scratch_thread2 = NULL;

err = hs_clone_scratch(scratch_prototype, &scratch_thread1);
if (err != HS_SUCCESS) {
    printf("hs_clone_scratch failed!");
    exit(1);
}
err = hs_clone_scratch(scratch_prototype, &scratch_thread2);
if (err != HS_SUCCESS) {
    printf("hs_clone_scratch failed!");
    exit(1);
}

hs_free_scratch(scratch_prototype);

/* 现在每个线程都能够通过自身的暂存空间扫描数据库 db */
```

### 定制分配器（Custom Allocators）

默认情况下，Hyperscan在运行时使用的结构体（暂存空间，流状态等）都是使用的默认系统调用，通常是`malloc()`和`free()`.

Hyperscan API 提供一些函数使用户可以使用定制的分配器：

* `hs_set_database_allocator()`：将分配器和释放器为所有编译的模式数据库设置。
* `hs_set_scratch_allocator()`：将分配器和释放器为暂存空间设置。
* `hs_set_stream_allocator()`：将分配器和释放器为流模式下的流状态设置。
* `hs_set_misc_allocator`：将分配器和释放器为冗余数据设置，例如编译出错结构体和提示信息字符串。

`hs_set_allocator()`函数可用于将所有的自定义分配器设置为 allocate/free 对。













