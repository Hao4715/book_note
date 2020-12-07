# Namespaces简介

[TOC]

## 参考资料

https://lwn.net/Articles/531114/

https://www.cnblogs.com/linhaifeng/p/6657119.html

https://www.cnblogs.com/sparkdev/p/9365405.html

目前，Linux实现了六种不同类型的Namespaces。每个名称空间的目的都是以一种抽象的形式包装特定的全局系统资源，以使其在名称空间中的进程中看起来像它们具有自己的隔离的全局资源实例。命名空间的总体目标之一是支持容器的实现，这是一种用于轻量级虚拟化（以及其他目的）的工具，它为一组进程提供了错觉，即它们是系统上唯一的进程。

## 三个主要系统调用

* clone()：创建一个新进程同时创建namespace并通过参数实现隔离。
* unshare()：使某个进程脱离某个namespace。
* setns()：将某个进程加入某个namespace。

## 创建一个名称空间


```c
#define _GNU_SOURCE 
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

/* 定义一个给 clone 用的栈，栈大小1M */
#define STACK_SIZE (1024 * 1024) 
static char container_stack[STACK_SIZE];

char* const container_args[] = {
    "/bin/bash",
    NULL
};

int container_main(void* arg)
{
    printf("Container - inside the container!\n");
    /* 直接执行一个shell，以便我们观察这个进程空间里的资源是否被隔离了 */
    execv(container_args[0], container_args);
    printf("Something's wrong!\n");
    return 1;
}

int main()
{
    printf("Parent - start a container!\n");
    /* 调用clone函数，其中传出一个函数，还有一个栈空间的（为什么传尾指针，因为栈是反着的） */
    int container_pid = clone(container_main, container_stack+STACK_SIZE, SIGCHLD, NULL);
    /* 等待子进程结束 */
    waitpid(container_pid, NULL, 0);
    printf("Parent - container stopped!\n");
    return 0;
}
```

编译：`gcc clone.c -o clone`

编译后执行可执行文件会进入隔离空间，使用exit退出。

## 七种namespace

### UTS namespace（CLONE_NEWUTS）

主要目的是独立出主机名和网络信息服务（**NIS**）。

```c
#define _GNU_SOURCE 
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

/* 定义一个给 clone 用的栈，栈大小1M */
#define STACK_SIZE (1024 * 1024) 
static char container_stack[STACK_SIZE];

char* const container_args[] = {
    "/bin/bash",
    NULL
};


/* 与uts有关的代码:此处只演示主机名的隔离 */
int container_main(void* arg) 
{ 
    printf("Container - inside the container!\n"); 
    sethostname("container",10); /* 设置hostname */ 
    execv(container_args[0], container_args); 
    printf("Something's wrong!\n"); 
    return 1; 
} 
 
int main() 
{ 
    printf("Parent - start a container!\n"); 
    int container_pid = clone(container_main, container_stack+STACK_SIZE,  
            CLONE_NEWUTS | SIGCHLD, NULL); /*启用CLONE_NEWUTS Namespace隔离 */ 
    waitpid(container_pid, NULL, 0); 
    printf("Parent - container stopped!\n"); 
    return 0; 
} 
```



### IPC namespace（CLONE_NEWIPC）

IPC全称 Inter-Process Communication，是Unix/Linux下进程间通信的一种方式，IPC有共享内存、信号量、消息队列等方法。所以，为了隔离，我们也需要把IPC给隔离开来，这样，只有在同一个Namespace下的进程才能相互通信。如果你熟悉IPC的原理的话，你会知道，IPC需要有一个全局的ID，即然是全局的，那么就意味着我们的Namespace需要对这个ID隔离，不能让别的Namespace的进程看到。

```c
#define _GNU_SOURCE 
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

/* 定义一个给 clone 用的栈，栈大小1M */
#define STACK_SIZE (1024 * 1024) 
static char container_stack[STACK_SIZE];

char* const container_args[] = {
    "/bin/bash",
    NULL
};


/* 与uts有关的代码:此处只演示主机名的隔离 */
int container_main(void* arg) 
{ 
    printf("Container - inside the container!\n"); 
    sethostname("container",10); /* 设置hostname */ 
    execv(container_args[0], container_args); 
    printf("Something's wrong!\n"); 
    return 1; 
} 
 
int main() 
{ 
    printf("Parent - start a container!\n"); 
    int container_pid = clone(container_main, container_stack+STACK_SIZE,  
            CLONE_NEWUTS | CLONE_NEWIPC | SIGCHLD, NULL); /*新增CLONE_NEWIPC就可以了 */ 
    waitpid(container_pid, NULL, 0); 
    printf("Parent - container stopped!\n"); 
    return 0; 
} 
```

ipc相关命令：

* ipcmk：创建IPC队列
* ipcrm：删除IPC队列
* ipcs：查看队列

编译运行：首先在全局创建一个ipc队列，在全局中`ipcs -q`能够查看队列信息，执行 `./uts`程序后，在容器中也能看到，但是执行`./ipc`后，在容器中看不到，证明实现了IPC隔离。

### PID namespace（CLONE_NEWPID）

使用`CLONE_NEWPID`即可实现PID隔离，这里使用`CLONE_NEWUTS`、`CLONE_NEWIPC`和`CLONE_NEWPID`实现三种隔离，接近于在Linux上新建一个隔离的操作系统了。

```c
#define _GNU_SOURCE 
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

/* 定义一个给 clone 用的栈，栈大小1M */
#define STACK_SIZE (1024 * 1024) 
static char container_stack[STACK_SIZE];

char* const container_args[] = {
    "/bin/bash",
    NULL
};


int container_main(void* arg) 
{ 
    printf("Container [%5d] - inside the container!\n",getpid()); /* 此处的getpid()是为了获取容器的初始进程(init)的pid */
    sethostname("container",10); /* 设置hostname */ 
    execv(container_args[0], container_args); 
    printf("Something's wrong!\n"); 
    return 1; 
} 
 
int main() 
{ 
    printf("Parent [%5d] - start a container!\n",getpid()); /* 此处的getpid()则是为了获取父进程的pid */ 
    int container_pid = clone(container_main, container_stack+STACK_SIZE,  
            CLONE_NEWUTS | CLONE_NEWIPC | CLONE_NEWPID | SIGCHLD, NULL); /*新增CLONE_NEWPID即可,此处代表在UTS和IPC隔离的基础之上再进行PID的隔离，其实我们完全可以只加CLONE_NEWPID自己:这样的话就只代表隔离PID了 */ 
    waitpid(container_pid, NULL, 0); 
    printf("Parent - container stopped!\n"); 
    return 0; 
}
```

在unix操作系统中，PID为1的进程init（centos7 之后改为systemd），功能特殊，因此为了实现进程隔离，需要创建出PID为1的进程。

但是，在子进程的shell里输入ps,top等命令，我们还是可以看得到所有进程。说明并没有完全隔离。这是因为，像ps, top这些命令会去读/proc文件系统，所以，因为/proc文件系统在父进程和子进程都是一样的，所以这些命令显示的东西都是一样的。所以，我们还需要对文件系统进行隔离，这就需要用到**mount**命名空间了

### Mount namespaces（CLONE_NEWNS）

进程运行时可以将挂载点与系统分离，使用这个功能时，我们可以达到 chroot 的功能，而在安全性方面比 chroot 更高。

```c
#define _GNU_SOURCE 
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

/* 定义一个给 clone 用的栈，栈大小1M */
#define STACK_SIZE (1024 * 1024) 
static char container_stack[STACK_SIZE];

char* const container_args[] = {
    "/bin/bash",
    NULL
};

int container_main(void* arg) 
{ 
    printf("Container [%5d] - inside the container!\n", getpid()); 
    sethostname("container",10); 
    /* 重新mount proc文件系统到 /proc下 */ 
    system("mount -t proc proc /proc"); 
    execv(container_args[0], container_args); 
    printf("Something's wrong!\n"); 
    return 1; 
} 
 
int main() 
{ 
    printf("Parent [%5d] - start a container!\n", getpid()); 
    /* 启用Mount Namespace - 增加CLONE_NEWNS参数 */ 
    int container_pid = clone(container_main, container_stack+STACK_SIZE,  
            CLONE_NEWUTS | CLONE_NEWIPC | CLONE_NEWPID | CLONE_NEWNS | SIGCHLD, NULL); 
    waitpid(container_pid, NULL, 0); 
    printf("Parent - container stopped!\n"); 
    return 0; 
} 
```

这个程序执行后，会实现mount的隔离。在通过CLONE_NEWNS创建mount namespace后，父进程会把自己的文件结构复制给子进程中。而子进程中新的namespace中的所有mount操作都只影响自身的文件系统，而不对外界产生任何影响。

### Network namespace（CLONE_NEWNET）

用于隔离网络资源（/proc/net，IP地址，网卡，路由等）。后台进程可以运行在不同命名空间内相同的端口上，还可以虚拟出一块网卡。每个网络命名空间都有自己的路由表，它自己的iptables设置提供nat和过滤。Linux网络命名空间还提供了在网络命名空间内运行进程的功能。

### User namespace（CLONE_USER）

同进程 ID 一样，用户 ID 和组 ID 在命名空间内外是不一样的，并且在不同命名空间内可以存在相同的 ID。

### Cgroup namespace（CLONE_NEWCGROUP）

内核版本4.6添加。

## Linux namespace操作

`/proc/$[pid]/ns`目录存储pid进程所属的Namespace信息。可以通过`ll /proc/$$/ns`查看当前进程的Namespace进程信息，其中的文件都是链接文件，格式为`xxx:[inode number]`。xxx为namespace类型，inode number则用来标识一个namespace，如果两个进程的namespace文件指向同一个链接文件，说明相关资源在同一个namespace中。

其次，在 /proc/[pid]/ns 里放置这些链接文件的另外一个作用是，一旦这些链接文件被打开，只要打开的文件描述符(fd)存在，那么就算该 namespace 下的所有进程都已结束，这个 namespace 也会一直存在，后续的进程还可以再加入进来。
除了打开文件的方式，我们还可以通过文件挂载的方式阻止 namespace 被删除。比如我们可以把当前进程中的 uts 挂载到 ~/uts 文件：

```shell
touch `/uts
sudo mount --bind /proc/$$ns/uts ~/uts
```







