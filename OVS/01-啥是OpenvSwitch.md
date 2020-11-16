# 啥是Open vSwitch
Open vSwitch是一个**多层软件交换机**。Open vSwitch适合在VM环境下用作虚拟交换机。除了向虚拟网络层提供标准控制和可视化接口之外，他还被设计成支持跨多个物理服务器的分发。Open vSwitch支持多个基于Linux虚拟化技术包括Xen/XenServer，KVM和VirtualBox。

代码主体是C编写因而易于移植。目前版本支持以下功能：
* Standard 802.1Q VLAN model with trunk and access ports
* NIC bonding with or without LACP on upstream switch
* NetFlow, sFlow(R), and mirroring for increased visibility
* QoS (Quality of Service) configuration, plus policing
* Geneve, GRE, VXLAN, STT, and LISP tunneling
* 802.1ag connectivity fault management
* OpenFlow 1.0 plus numerous extensions
* Transactional configuration database with C and Python bindings
* High-performance forwarding using a Linux kernel module
