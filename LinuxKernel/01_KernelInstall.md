# 内核安装

## 下载内核

1. 使用命令`cat /etc/redhat-release`查看centos系统版本
2. 在https://vault.centos.org/7.6.1810/os/Source/SPackages/下载所需内核版本（本次使用：https://vault.centos.org/7.6.1810/os/Source/SPackages/kernel-3.10.0-957.el7.src.rpm）

## 安装内核

1. 使用命令`rpm -i kernel-3.10.0-957.el7.src.rpm`在安装内核

   **说明：报错`warning: user mockbuild does not exist - using root`**

   **解决方法：**

   * `yum install mock`:安装mock
* `useradd -s /sbin/nologin mockbuild`：根据报错中提示的user创建。
2. 再重新执行rpm命令。

3. 安装依赖包：`yum install bison gcc rpm-build redhat-rpm-config asciidoc hmaccalc perl-ExtUtils-Embed pesign xmlto audit-libs-devel binutils-devel elfutils-devel elfutils-libelf-devel java-devel ncurses-devel newt-devel numactl-devel pciutils-devel python-devel zlib-devel`

4. 回到根目录，进入rpmbuild目录中的SPECS目录中，执行`rpmbuild`命令。

   ```shell
   cd ~/rpmbuild/SPECS
   rpmbuild -bp --target=$(uname -m) kernel.spec
   ```

   报错缺乏依赖包，安装依赖即可：`yum install net-tools bc python-docutils`

   重新执行`rpmbuild -bp --target=$(uname -m) kernel.spec`命令。完成后在../BUILD目录即可看见对应版本的源码包。
   
5. 创建符号链接。
   
   ```shell
   cd /lib/modules/3.10.0-957.el7.x86_64/
   rm ./build
   ln -s /root/rpmbuild/BUILD/kernel-3.10.0-957.el7/linux-3.10.0-957.el7.x86_64/ build
   ```
   
6. 编写hello测试

   ```c
   //hello.c
   //必要的头文件
   #include <linux/module.h>
   #include <linux/kernel.h>
   #include <linux/init.h>
   //模块许可证声明（必须）
   MODULE_LICENSE("Dual BSD/GPL");
   //模块加载函数（必须）
   static int hello_init(void)
   {
       printk(KERN_ALERT "Hello World enter/n");
       return 0;
   }
   //模块卸载函数（必须）
   static void hello_exit(void)
   {
       printk(KERN_ALERT "Hello World exit/n");
   }
   //模块的注册
   module_init(hello_init);
   module_exit(hello_exit);
   //声明模块的作者（可选）
   MODULE_AUTHOR("uno");
   //声明模块的描述（可选）
   MODULE_DESCRIPTION("This is a simple example!/n");
   //声明模块的别名（可选）
   MODULE_ALIAS("A simplest example");
   ```

   ```makefile
   obj-m += hello.o
   #generate the path
   CURRENT_PATH:=$(shell pwd)
   #the current kernel version number
   LINUX_KERNEL:=$(shell uname -r)
   #the absolute path
   LINUX_KERNEL_PATH:=/lib/modules/$(LINUX_KERNEL)/build
   #complie object
   all:
       make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules
   #clean
   clean:
       make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean
   ```

   make编译时会报错如下：

   fatal error: generated/autoconf.h: No such file or directory。

   解决办法： 重新编译内核源码。

   安装编译依赖：

   ```shell
   yum install ncurses-devel
   yum install elfutils-libelf-devel
   ```

   进入源码目录编译：

   ```shell
   cd /root/rpmbuild/BUILD/kernel-3.10.0-957.el7/linux-3.10.0-957.el7.x86_64
   make
   ```

7. 修改内核源码，修改内核源码后需要重新编译源码生成新的内核。

   1. `cd /root/rpmbuild/BUILD/kernel-3.10.0-957.el7/linux-3.10.0-957.el7.x86_64`
   2. `make menuconfig`,没特殊要求，直接退出即可
   3. `make`编译，耗时久
   4. 安装：`make modules_install`，`make install`
   5. 更新引导文件：`grub2-mkconfig -o /boot/grub2/grub.cfg`

## 模块简单使用

* 加载模块：`insmod hello.ko`
* 卸载模块：`rmmod hello.ko`
* 查看系统加载的所有模块以及依赖关系：`lsmod`,lsmod命令实际上读取并分析了/proc/modules文件。
* `mkdinfo hello.ko`可获得模块的信息，包括作者，说明，参数等信息。