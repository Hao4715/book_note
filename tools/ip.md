# ip

## netns

* **创建namespace：**`ip netns add xx`
* **在namespace xx 中 执行 yy命令:**`ip netns exec xx yy`

## link

* **创建 veth pair：**` ip link add type veth`

* **将 veth xx 加入到 namespace yy 中:**`ip link set veth0 netns net0`

* **给 veth pair 配上 ip 地址：**

  ```shell
  # ip netns exec net0 ip link set veth0 up
  # ip netns exec net0 ip addr
  # ip netns exec net0 ip addr add 10.1.1.1/24 dev veth0
  # ip netns exec net0 ip route
  # ip netns exec net1 ip link set veth1 up
  # ip netns exec net1 ip addr add 10.1.1.2/24 dev veth1
  ```

* **创建 bridge：**使用ip link 或者 brctl 

  ```shell
  # ip link add br0 type bridge
  # ip link set dev br0 up
  ```

## 实验

### 1、多个不同namespace之间通过bridge通信

1、创建bridge

```shell
# ip link add br0 type bridge
# ip link set dev br0 up
```

2、创建veth pair

```shell
# ip link add type veth
# ip link add type veth
# ip link add type veth
```

3、将 veth pair 的一头挂到 namespace 中，一头挂到 bridge 上，并设 IP 地址

```shell
# （1）配置第 1 个 net0
# ip link set dev veth1 netns net0
# ip netns exec net0 ip link set dev veth1 name eth0
# ip netns exec net0 ip addr add 10.0.1.1/24 dev eth0
# ip netns exec net0 ip link set dev eth0 up
#
# ip link set dev veth0 master br0
# ip link set dev veth0 up

# （2）配置第 2 个 net1
# ip link set dev veth3 netns net1
# ip netns exec net1 ip link set dev veth3 name eth0
# ip netns exec net1 ip addr add 10.0.1.2/24 dev eth0
# ip netns exec net1 ip link set dev eth0 up
#
# ip link set dev veth2 master br0
# ip link set dev veth2 up

# （3）配置第 3 个 net2
# ip link set dev veth5 netns net2
# ip netns exec net2 ip link set dev veth5 name eth0
# ip netns exec net2 ip addr add 10.0.1.3/24 dev eth0
# ip netns exec net2 ip link set dev eth0 up
# 
# ip link set dev veth4 master br0
# ip link set dev veth4 up
```

4、不通，原因是因为系统为bridge开启了iptables功能，导致所有经过br0的数据包都要受iptables里面规则的限制，而docker为了安全性（我的系统安装了 docker），将iptables里面filter表的FORWARD链的默认策略设置成了drop，于是所有不符合docker规则的数据包都不会被forward，导致你这种情况ping不通。

> 解决办法有两个，二选一：
>
> 1. 关闭系统bridge的iptables功能，这样数据包转发就不受iptables影响了：echo 0 > /proc/sys/net/bridge/bridge-nf-call-iptables
> 2. 为br0添加一条iptables规则，让经过br0的包能被forward：iptables -A FORWARD -i br0 -j ACCEPT
>
> 第一种方法不确定会不会影响docker，建议用第二种方法。

我采用以下方法解决：

```
Copyiptables -A FORWARD -i br0 -j ACCEPT
```