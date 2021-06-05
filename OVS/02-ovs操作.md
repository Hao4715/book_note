# OVS 操作命令

* 启动ovs：`ovs-ctl start`
* 查看网桥br0配置，检查网桥是否创建成功     `ovs-vsctl show`
* 删除端口 `ovs-vsctl del-port ${name}`
* 删除网桥`ovs-vsctl del-br ${name}`
* 无网络创建docker容器`docker run -it --name centnginx1 --net=none --privileged centosnginx`
* 创建网桥`ovs-vsctl add-br br0`
* 为docker添加网卡`ovs-docker add-port br0 eth0 centnginx1 --ipaddress=172.20.0.3/24`
* ovs查看端口信息`ovs-vsctl list interface ${name}`
* 宿主机增加虚拟网卡`ovs-vsctl add-port br0 ens44 -- set interface ens44 type="internal"`
* 启动网卡`ifconfig ens44 up`
* 设置IP`ifconfig ens44 172.20.0.4/24`
* 查看流表项`ovs-ofctl dump-flows br0`
* 增加流表项 `ovs-ofctl add-flow br0 priority=50000,ip,in_port="11f2ff1bb6b14_l",actions=mod_dl_dst:ee:e0:0f:4e:40:e1,mod_nw_src:172.20.0.1,output:vxlan1`
* 掩码匹配：` ovs-ofctl add-flow br0 priority=1000,dl_dst=7e:00:00:00:00:00/ff:00:00:00:00:00,actions=drop`
* 删除流表项`ovs-ofctl del-flows br0 nw_dst=172.20.0.5`
* 设置MAC老化时间：
  * 查看MAC老化时间：`ovs-appctl fdb/show xiang`
  * 开启：`ovs-vsctl set bridge xiang stp_enable=true`
  * 查看：`ovs-vsctl get bridge xiang stp_enable`
  * 关闭：`ovs-vsctl set bridge xiang stp_enable=false`
  * 设置时间：`ovs−vsctl set Bridge br0 other_config:rstp-ageing-time=1000`
* 为网桥设置设置IP：`ifconfig xhw 172.25.0.254/24 up`
* 为容器设置网关：`route add default gw 172.25.0.254`
* 设置ovs vxlan 端口：` ovs-vsctl add-port br0 vxlan1 -- set interface vxlan1 type=vxlan options:remote_ip=10.0.0.1 options:local_ip=10.0.0.2 option:key=671745`
* 修改ovs vxlan端口vni：`ovs-vsctl set interface "port0" option:key=456704`

dl_dst=7e:ff:00:03:08:00

## 实验规则

* `ovs-ofctl add-flow xiang priority=1000,dl_dst=00:00:08:00:45:00/00:00:ff:ff:ff:ff,actions=output:`
* `ovs-ofctl add-flow xiang priority=100,ip,nw_src=172.16.18.135,actions=mod_dl_dst:d6:ba:25:dd:64:a8,mod_nw_dst:172.20.0.3,normal`
* `ovs-ofctl add-flow xiang priority=0,actions=NORMAL`

http://www.openvswitch.org/support/dist-docs-2.5/

### ovs-ofctl

http://www.openvswitch.org/support/dist-docs-2.5/ovs-ofctl.8.pdf

* Q: I can't seem to add my PPP interface to an Open vSwitch bridge.

* A: PPP most commonly carries IP packets, but Open vSwitch works only with Ethernet frames. The correct way to interface PPP to an Ethernet network is usually to use routing instead of switching.(http://www.openvswitch.org/support/dist-docs-2.5/FAQ.md.html)

#### Group Table

Group Table给OpenFlow Switch提供了更加高级的数据包转发特性（比如select或者all），其由多个Group Entries组成，而每个Group Entry结构如下所示：

![](./img/grouptable.png)

TLVs ：Type, Length, Value

OpenFlow Extensible Match (OXM)

metadata:Table metadata. Used to pass information between tables

pkt_mark

ct_state

−−flow−format 

