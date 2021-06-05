# Linux 驱动

该部分起源于dpdk[5.Linux Drivers](http://doc.dpdk.org/guides/linux_gsg/linux_drivers.html)以及查阅其他资料。

## 词语解释

| 缩写  | 名称                         | 中文             |
| ----- | ---------------------------- | ---------------- |
| PMDs  | poll-mode drivers            | 轮询模式驱动     |
| VFIO  | Virtual Function I/O         |                  |
| DMA   | Direct Memory Access         | 直接存储器访问   |
| MMU   | memory management unit       | 内存管理单元     |
| PMMU  | paged memory management unit | 分页内存管理单元 |
| IOMMU | I/O memory management unit   |                  |
| IOVA  | IO virtual address           |                  |

## Linux 驱动

不同的PMD需要不同的内核驱动程序才能正常的工作。根据使用的不同PMD，应该正确的加载内核驱动程序，并且将网络接口绑定到该驱动程序。

### 1.IOMMU

参考：https://terenceli.github.io/%E6%8A%80%E6%9C%AF/2019/08/04/iommu-introduction

MMU是CPU使用的将虚拟地址转换为物理地址的工具，MMU的虚拟地址是CPU视角的。相对的IOMMU是驱动使用的将另一种称为IOVA（IO虚拟地址）的虚拟地址转换为物理地址的工具。**也就是说，对于CPU的寻址是通过MMU实现的，驱动的寻址是通过IOMMU实现的。**

IOMMU包含两方面的功能：DMA remapping（地址映射，可以限定用户态驱动的内存访问，提高性能的同时保证系统安全性） 和 Interrupt remapping（将硬件中断映射到用户态）。

> This includes x86 hardware with AMD-Vi and Intel VT-d,POWER systems with Partitionable Endpoints (PEs) and embedded PowerPC systems such as Freescale PAMU. 

### 2. VFIO

参考：https://www.kernel.org/doc/Documentation/vfio.txt

VFIO在安全以及IOMMU保护环境下将直接设备访问权限暴露在用户态的IOMMU框架。也就是可以实现安全、无权限限制的用户态驱动。相比UIO更加强健和安全。