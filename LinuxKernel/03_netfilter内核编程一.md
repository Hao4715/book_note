# netfilter内核编程一

[TOC]

## 参考

* http://onestraw.github.io/linux/netfilter-hook/

## NETFILTER

NetFilter是一个Linux下的包过滤防火墙，他集成在内核中，他提供一个标准的接口，通过该接口能够方便的进行不同的网络操作，包括包过滤、网络地址转换和端口转换。Netfilter在内核中提供一组hooks，通过这些hooks，内核模块可以向TCP/IP协议栈注册回调函数。

Iptables是一个配置IPv4包过滤和NAT的管理工具。iptables包含4个表，5个链。其中表是按照对数据包的操作区分的，链是按照不同的Hook点来区分的，表和链实际上是netfilter的两个维度。

**表：**

* filter：一般的过滤功能

- nat:用于nat功能（端口映射，地址映射等）
- mangle:用于对特定数据包的修改
- raw:优先级最高，设置raw时一般是为了不再让iptables做数据包的链接跟踪处理，提高性能

**优先级：**`raw > mangle > nat > filter`

**链：**

- PREROUTING: 数据包进入路由表之前
- INPUT: 通过路由表后目的地为本机
- FORWARDING: 通过路由表后，目的地不为本机
- OUTPUT: 由本机产生，向外转发
- POSTROUTIONG: 发送到网卡接口之前。

**Netfilter hook点：**

* **NF_INET_PRE_ROUTING：**刚刚进入网络层的数据包通过此点（刚刚进行完版本号，校验和等检测),目的地址转换在此点进行。
* **NF_INET_LOCAL_IN：**经路由查找后，送往本机的通过此检查点，INPUT包过滤在此点 。
* **NF_INET_FORWARD：**要转发的包通过此检测点，FORWARD包过滤在此点进行。
* **NF_INET_POST_ROUTING：**所有马上便要通过网络设备出去的包通过此检测点，内置的源地址转换功能（包括地址伪装）在此点进行。
* **NF_INET_LOCAL_OUT：**本机进程发出的包通过此检测点，OUTPUT包过滤在此点进行。

每个注册的hook函数经过处理后都将返回下列值之一，告知Netfilter核心代码处理结果，以便对报文采取相应的动作：

- NF_ACCEPT：继续正常的报文处理；
- NF_DROP：将报文丢弃；
- NF_STOLEN：由钩子函数处理了该报文，不要再继续传送；
- NF_QUEUE：将报文入队，通常交由用户程序处理；
- NF_REPEAT：再次调用该钩子函数。

**注：对应的定义位于内核源码中的`include/uapi/linux/netfilter.h`文件中。hook函数结构体以及hook函数定义位于内核源码中的`include/linux/netfilter.h`文件中。IP Hooks 以及优先级（优先级是有符号的32位数，值越小优先级越高）定义位于内核源码中的`include/uapi/linux/netfilter_ipv4.h`文件中。`struct sk_buff`结构体定义于内核源码中的`include/linux/skbuff.h`文件中。**

hook函数：

```c
 typedef unsigned int nf_hookfn(const struct nf_hook_ops *ops,
                    struct sk_buff *skb,
                    const struct net_device *in,
                    const struct net_device *out,
#ifndef __GENKSYMS__
                   const struct nf_hook_state *state
#else
                   int (*okfn)(struct sk_buff *)
#endif
                  );
```

hook挂载点结构体：

```c
 struct nf_hook_ops {
     struct list_head list;

     /* User fills in from here down. */
     nf_hookfn   *hook;
     struct module   *owner;
     void        *priv;
     u_int8_t    pf;
     unsigned int    hooknum;
     /* Hooks are ordered in ascending priority. */
     int     priority;

     /* Reserved for use in the future RHEL versions. Set to zero. */
     unsigned long   __rht_reserved1;
     unsigned long   __rht_reserved2;
     unsigned long   __rht_reserved3;
     unsigned long   __rht_reserved4;
     unsigned long   __rht_reserved5;
};
```

## HTTP协议的GET/POST流量分类

**功能：**GEP和POST数据包分类输出到日志文件中**。**

