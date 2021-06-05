# dpdk 安装与基本使用

## 环境

操作系统：centos 8.3.2011（vmware 虚拟机）

dpdk版本：dpdk-20.11

官网：https://www.dpdk.org/

## 安装步骤

### 一.工具安装

#### 1.安装常用工具

`yum install gcc vim wget git gcc-c++`

#### 2.安装Python 3.5 或更新的版本

2. 下载python 3.7 、解压并创建python程序目录。

   ```shell
   cd /usr/local
   wget https://www.python.org/ftp/python/3.7.0/Python-3.7.0.tgz
   tar -zxvf Python-3.7.0.tgz
   mkdir /usr/local/python37
   ```

3. 安装依赖：`yum install make zlib-devel libffi-devel openssl openssl-devel `

4. 安装

   ```shell
   cd /usr/local/Python-3.7.0
   ./configure --prefix=/usr/local/python37
   make && make install
   ```

5. 设置环境变量（此系统之前没有python，所以设置环境变量，若以安装过可以设置软链接）

   1. 打开环境变量文件：`vim /etc/profile`

   2. 写入环境变量：

      ```shell
      alias python3=/usr/local/python37/bin/python3.7
      alias pip3=/usr/local/python37/bin/pip3.7
      ```

   3. 生效环境变量：`source /etc/profile`

   4. 验证：执行`python3`能输出相应信息证明安装成功

#### 3.安装Meson 和 ninja

1. 安装meson：`pip3 install meson`

2. 安装ninja：

   1. 安装re2c，下载地址：http://re2c.org/index.html

      ```shell
      tar -xvJf re2c-2.0.3.tar.xz
      cd re2c-2.0.3/
      autoreconf -i -W all（没有configure可以先执行Autotools）
      ./configure
      make
      make install
      ```

   2. 安装ninja

      ```shell
      git clone git://github.com/ninja-build/ninja.git && cd ninja
      ./configure.py --bootstrap (若报错没有python，可以修改文件第一行的python为系统python环境)
      ```

   3. 添加ninja环境变量`export PATH=$PATH:/root/ninja`

#### 4.安装numactl工具以及库

`yum install numactl numactl-devel`

### 二.安装DPDK

#### 1.下载安装包

`wget http://fast.dpdk.org/rel/dpdk-20.11.tar.xz `

#### 2.解压安装包

`tar -xJf dpdk-20.11.tar.xz`

#### 3.安装DPDK

```shell
cd dpdk-20.11
meson build      #配置，build为编译输出目录,不报错既成功
cd build
ninja            #编译
ninja install    #安装
# 创建文件/etc/ld.so.conf.d/dpdk-20.11.conf，在文件中写入以下两项
# /usr/local/lib
# /usr/local/lib64
ldconfig
```

### 三.系统环境配置

#### 1.大页配置

1. 查看系统是否为NUMA架构，`dmesg | grep -i numa`，查看是否为NUMA架构。
2. 查看系统NUMA节点，`numactl -H`。
3. 查看支持2M大页还是1G大页，`cat /proc/cpuinfo`，返回结果包含"pse"表示支持2M大页，包含"pdpe1gb"表示支持1G大页。
4. 分配大页。机器重启后需要重新配置大页内存。
   * 非NUMA，` echo 1024 >/sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages`，表示配置1024个2M的大页。
   * NUMA，`echo 4 > /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages`，表示为node0配置1024个2M的大页。
5. 挂载大页。每次设置大页后需要重新挂载才能使用。
   1. 创建挂载点，`mkdir /mnt/huge`。
   2. 挂载，`mount -t hugetlbfs nodev /mnt/huge`
   3. 查看挂载情况，`cat /proc/mounts`

#### 2.网卡配置

##### 1）使用VFIO

检查系统是否支持并开启IOMMU，`dmesg | grep IOMMU`，有enable输出表示支持。若没有可以通过首先在`vim /etc/default/grub`中的`GRUB_CMDLINE_LINUX`后添加`iommu=pt intel_iommu=on`开启，再使用`grub2-mkconfig -o /boot/grub2/grub.cfg`更新配置再重启机器重新查看IOMMU即可。(若是虚拟机且不生效，需要关闭虚拟机，并在虚拟机的 “虚拟机”---->“设置”---->“处理器”---->“虚拟化引擎”开启“虚拟化IOMMU（IO内存管理单元）”)。

1. 加载vfio-pci模块。

   ```shell
   modprobe vfio-pci 
   ```

2. 加载vfio-pci模块。

   1. 查看网卡绑定状态和编号，在"usertools"目录中，通过`./dpdk-devbind.py --status`查看。
   2. 将网卡down掉，`nmcli con down ens34`
   3. 更换模块：`./dpdk-devbind.py --bind=vfio-pci 05:00.0`

#### 3.环境变量配置

`export PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig`

### 四.运行helloworld示例

进入安装目录下的examples目录中的helloworld，执行make，命令即可编译。进入编译生成的build目录，执行`./helloworld -l 0-3`即可看到输出信息。

### 其他命令

`lspci -nn -s 02:02.0`

`nmcli`

https://www.cnblogs.com/hugetong/p/5914938.html

网卡解绑与绑定可以使用dpdp的dpdk-devbind.py工具实现，也可以通过如下方式：

1. 解绑：`echo 0000:00:04.0 > /sys/bus/pci/devices/0000\:00\:04.0/driver/unbind `
2. 绑定：`echo '0000:00:04.0' > /sys/bus/pci/drivers/uio_pci_generic/bind  `