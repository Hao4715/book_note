# VxLAN

一种网络虚拟化技术，对VLAN的扩展。VxLAN技术是一种大二层的虚拟网络技术，主要原理是引入一个UDP格式的外层隧道作为数据链路层，而原有数据报文内容作为隧道净荷加以传输。由于外层采用了UDP作为传输手段，净荷数据可以轻松地在二三层网络中传送。VxLAN已成为业界主流的虚拟网络技术之一。

VXLAN头部包含有一个VXLAN标识（即VNI，VXLAN Network Identifier），只有在同一个VXLAN上的虚拟机之间才能相互通信。[VNI](https://baike.baidu.com/item/VNI/7297818)在数据包之中占24比特，故可支持1600万个VXLAN的同时存在，远多于VLAN的4094个，因此可适应大规模租户的部署。

https://support.huawei.com/enterprise/zh/doc/EDOC1100087027?sendFrom=mobile#ZH-CN_TOPIC_0254803584

## 术语

* **VTEP**：VXLAN Tunnel Endpoint (VTEP)。VXLAN使用VTEP设备对VXLAN报文进行封装与解封装，包括ARP请求报文和正常的VXLAN数据报文，VTEP将原始以太网帧通过VXLAN封装后发送至对端 VTEP设备，对端VTEP接收到 VXLAN报文后解封装然后根据原始 MAC进行转发，VTEP可以是物理交换机、物理服务器或者其他支持 VXLAN的硬件设备或软件来实现。
* **VNI**：Virtual Network ID ( VNI)， VNI封装在 VXLAN头部，共 24-bit ，***支持16,000,000 逻辑网络。VNI是一种类似于VLAN ID的用户标识，一个VNI代表了一个租户，属于不同VNI的虚拟机之间不能直接进行二层通信。
* **VXLAN网关**：VXLAN网关用于连接 VXLAN网络和传统 VLAN网络，VXLAN网关实现 VNI和VLAN ID 之间的映射， VXLAN 网关实际上也是一台 VTEP设备。

## VXLAN格式

- VXLAN Header

  增加VXLAN头（8字节），其中包含24比特的**VNI**字段，用来定义VXLAN网络中不同的租户。此外，还包含**VXLAN Flags**（8比特，取值为00001000）和两个保留字段（分别为24比特和8比特）。

- UDP Header

  VXLAN头和原始以太帧一起作为UDP的数据。UDP头中，目的端口号（VXLAN Port）固定为4789，源端口号（UDP Src. Port）是原始以太帧通过哈希算法计算后的值。

- Outer IP Header

  封装外层IP头。其中，源IP地址（Outer Src. IP）为源VM所属VTEP的IP地址，目的IP地址（Outer Dst. IP）为目的VM所属VTEP的IP地址。

- Outer MAC Header

  封装外层以太头。其中，源MAC地址（Src. MAC Addr.）为源VM所属VTEP的MAC地址，目的MAC地址（Dst. MAC Addr.）为到达目的VTEP的路径中下一跳设备的MAC地址。

