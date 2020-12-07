# 底层技术

Docker使用Go语言编写并利用许多Linux内核特性来实现自身的一些功能。

## Namespaces

Docker使用Namespaces技术来提供隔离的工作空间--容器。当容器被创建时会为容器创建一系列*namespaces*。

这些*namespaces*提供了隔离的层。容器的每一个部分运行在各自独立的*namespace*并且圈子安备局限在自己的namespace。

Docker引擎使用了Linux中以下的namespace：

* `pid` namespace：进程隔离（PID：进程ID）
* `net` namespace：管理网络接口（NET：Networking）
* `ipc` namespace：管理IPC资源的权限（IPC：InterProcess Communication）
* `mnt` namespace：管理文件系统挂载点（MNT：Mount）
* `uts` namespace：隔离内核和版本识别（UTS：Unix Timesharing System）

## Control groups

Linux下的Docker Engine也依赖于control groups技术（cgroups）。一个cgroup将应用先知道一系列指定的资源集中。Control groups允许Docker Engine 将可用的硬件资源共享给容器并有选择地进行约束和限制。例如，可以限制特定容器可用的内存。

## Unin file systems

Union file systems（联合文件系统）或 UnionFS 是通过创建层进行操作的文件系统，使得他们很轻量和快速。Docker Engine使用UnionFS来为容器提供构建模块。Docker Engine可以使用多种UnionFS变体，包括AUFS, btrfs, vfs, and DeviceMapper。

## Container format

Docker Engine 将namespaces、control groups 和 UnionFS组合成一个封装叫做--container format。默认containe format 是`libcontainer`。未来，Docker可能会结合BSD jails或Solaris Zones技术提供更多container format。



