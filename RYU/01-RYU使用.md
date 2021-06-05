# RYU使用

[TOC]

## 安装

官方文档：https://ryu.readthedocs.io/en/latest/index.html

资源网站：

* http://osrg.github.io/ryu-book/en/html/
* https://ryu.readthedocs.io/en/latest/

使用官方提供的虚拟机镜像，用户名以及密码均为“ubuntu”。

## 官方示例

下载以及官方入门示例：http://sdnhub.org/tutorials/ryu/

1. 创建mininet拓扑：`sudo mn --topo single,3 --mac --controller remote --switch ovsk`
2. 设置交换机openflow协议：`sudo ovs-vsctl set bridge s1 protocols=OpenFlow13`
3. 打开抓包工具：`sudo wireshark &`
4. 到ryu目录并执行官方的controller示例：` cd /home/ubuntu/ryu && ./bin/ryu-manager --verbose ryu/app/simple_switch_13.py`
5. mininet主机间通信`mininet> h1 ping h3`
6. 在wireshark会看到openflow协议包。

## 官方代码示例

```python
from ryu.base import app_manager
from ryu.controller import ofp_event
from ryu.controller.handler import MAIN_DISPATCHER
from ryu.controller.handler import set_ev_cls
from ryu.ofproto import ofproto_v1_0

class L2Switch(app_manager.RyuApp):
    OFP_VERSIONS = [ofproto_v1_0.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(L2Switch, self).__init__(*args, **kwargs)
	# set_ev_cls 告诉ryu什么时机调用该方法，当ryu收到packet_in消息时会调用该方法
    # 第一个参数告诉调用时机，第二个参数表示交换机状态，“MAIN_DISPATCHER”表示协商完成后才会调用该方法。
    @set_ev_cls(ofp_event.EventOFPPacketIn, MAIN_DISPATCHER)
    def packet_in_handler(self, ev):
        msg = ev.msg       # ev.msg是表示packet_in数据类型的对象
        dp = msg.datapath  # msg。datapath表示交换机的datapath
        ofp = dp.ofproto   
        ofp_parser = dp.ofproto_parser # dp.opproto和dp.ofproto_parser表示ryu与交换机协商的openflow协议
		# OFPActionOutput指定packet_out消息的发出端口，OFPP_FLOOD表示发到所有端口。
        actions = [ofp_parser.OFPActionOutput(ofp.OFPP_FLOOD)]
        # OFPPacketOut用于构建packet_out消息
        out = ofp_parser.OFPPacketOut(
            datapath=dp, buffer_id=msg.buffer_id, in_port=msg.in_port,
            actions=actions)
        # send_msg构建并发送消息到交换机
        dp.send_msg(out)
```

## ryu连接远程ovs

* 设置ovs远程连接的目的IP以及port：`ovs-vsctl set-controller xiang tcp:192.168.103.87:6666`。
* 修改ryu监听端口的两种方式：
  1. 修改ryu/ryu/ofproto/ofproto_common.py 文件中的`OFP_TCP_PORT`项。
  2. 通过命令行指定监听端口：`ryu-manager --ofp-tcp-listen-port   6666  --verbose  simple_switch_13.py`
* 启动ryu控制器，可以通过`netstat -lnpt`查看监控端口情况。
* 连接成功后，`ovs-vsctl show`命令会看到控制器下显示`is_connect:true`。

## 常用函数方法说明

### @set_ev_cls

详情：https://ryu.readthedocs.io/en/latest/ryu_app_api.html#ryu-controller-handler-set-ev-cls

* 原型：`ryu.controller.handler.set_ev_cls`**(** *ev_cls***,** *dispatchers=None***)**

* 功能：用于为Ryu应用声明事件处理句柄的装饰器（装饰器：个人理解类似于java中的注解）。

* 参数：
  * ev_cls：该Ryu应用想接收的实例所属的事件类。使用方法：`ryu.controller.ofp_event.Event` + <OpenFlow message name>  ，<OpenFlow message name> 见ruy安装目录中的`ryu/ryu/ofproto/of_proto_v1_*_parser.py`。 
  * dispatchers：说明该为该句柄生成的事件类型。包括：
    * ryu.controller.handler.HANDSHAKE_DISPATCHER：发送和等待hello消息
    * ryu.controller.handler.CONFIG_DISPATCHER：版本协商和发送特征请求消息
    * ryu.controller.handler.MAIN_DISPATCHER：接收交换机状态消息和发送配置设置消息
    * ryu.controller.handler.DEAD_DISPATCHER：对端断开连接或因为无法恢复的错误断开连接。

* 示例：`@set_ev_cls(ofp_event.EventOFPHello,HANDSHAKE_DISPATCHER)`

### self.logger.debug | info

* 功能：调试输出
* 示例：
  * `self.logger.debug("output debug")`
  * `self.logger.info("output debug")`

### parser.OFPMatch

* 功能：定义交换机流表项匹配条件，空则表示匹配所有。具体匹配项见：https://ryu.readthedocs.io/en/latest/ofproto_v1_3_ref.html（该页面是openflow1.3版本的，其他版本见具体网页）

* 示例：更多示例见：https://ryu.readthedocs.io/en/latest/ofproto_v1_3_ref.html

  * 匹配IP地址：

    ```python
    kwargs = dict(eth_type=ether_types.ETH_TYPE_IP,ipv4_src='172.16.18.135')
    match = parser.OFPMatch(**kwargs)
    ```

  * 匹配带掩码的mac地址（所有以“08:00:45:00”结尾的IP地址）：

    ```py
    kwargs = dict(eth_dst=('00:00:08:00:45:00','00:00:ff:ff:ff:ff'))
    match = parser.OFPMatch(**kwargs)
    ```

### help()  [python查看函数使用]

* 功能：python语言中使用该函数查看函数的使用
* 示例：help(parser.OFPActionOutput)

## 示例代码说明

自己写的测试代码，参考官方代码：http://osrg.github.io/ryu-book/en/html/switching_hub.html#

```python
from ryu.base import app_manager
from ryu.controller import ofp_event
from ryu.controller.handler import MAIN_DISPATCHER
from ryu.controller.handler import HANDSHAKE_DISPATCHER
from ryu.controller.handler import DEAD_DISPATCHER
from ryu.controller.handler import set_ev_cls
from ryu.ofproto import ofproto_v1_3

class L2Switch(app_manager.RyuApp):
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(L2Switch, self).__init__(*args, **kwargs)

    @set_ev_cls(ofp_event.EventOFPHello,HANDSHAKE_DISPATCHER)
    def packet_hello_handler(self, ev):
        self.logger.debug("handshake_dispatcher_my")

    @set_ev_cls(ofp_event.EventOFPSwitchFeatures, CONFIG_DIAPATCHER)
    def switch_features_handler(self, ev):
        self.logger.debug("config_dispatcher_my")
        # ev.msg 是与存储的openflow消息匹配的实例，此处是ryu.ofproto.ofproto_v1_3_parser.OPFSwitchFeature。msg详见 ryu/ryu/ofproto/ofproto_parser.py 中的 MsgBase
        # msg.datapath ryu.controller.controller.Datapath 类实例，Datapath类实现很多关键过程，例如与OpenFlow交换机的实际通信和保证event与收到的消息相对应。Ryu应用用到的Datapath的主要属性有：详见 ryu/ryu/controler/controller.py中的 Datapath
        # 1.id：与OpenFlow交换机连接的ID(data path ID)
        # 2.ofproto：表示支持使用中的OpenFlow版本的ofproto模块。本例中OpenFlow 1.3 格式使用ryu.ofproto.ofproto_v1_3模块
        # 3.ofproto_parser：与ofproto相同，指示ofproto_parser模块。本例使用ryu.ofproto.ofproto_v1_3_parser模块。
        datapath = ev.msg.datapath
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser
        # install the table_miss flow entry
        # 空的match匹配所有的包，见ryu/ryu/ofproto/ofproto_v1_*_parser.py中的OFPMatch
        math = parser.OFPMatch() 
        # 将controler设为消息输出端口，将消息转发到controller，OFPCML_NO_BUFFER是为了将所有包发到controller而设的最大值，此处可直接写入数字或者定义的变量
        actions = [parser.OFPActionOutput(ofproto.OFPP_CONTROLLER,ofproto.OFPCML_NO_BUFFER)]
        # 自定功能函数，将Flow Mod 消息下发到交换机，并且priority设为0
        self.add_flow(datapath, 0, match, actions)

    def add_flow(self, datapath, priority, match, actions):
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser
        # construct flow_mod message and send it.
        inst = [parser.OFPInstructionActions(ofproto.OFPIT_APPLY_ACTIONS,actions)]
        mod = parser.OFPFlowMod(datapath=datapath,priority=priority,match=match,instructions=inst)
        datapath.send_msg(mod)

    # PacketIn messege：创建Packet-In event句柄来接收未知目的的包。
    # 常用的属性见 ryu/ryu/ofproto/ofproto_v1_*_parser.py中的OFPPacketIn说明
    @set_ev_cls(ofp_event.EventOFPPacketIn,MAIN_DISPATCHER)
    def packet_in_handler(self, ev):
        self.logger.debug("main_dispatcher_my")
        msg = ev.msg
        datapath = msg.datapath
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser

        dpip = datapath.id
        self.mac_to_port.setdefault(dpip, {})

        pkt = packet.Packet(msg.data)
        eth_pkt = pkt.get_protocol(ethernet.ethernet)
        dst = eth_pkt.dst
        src = eth_pkt.src

        in_port = msg.match['in_port']

        self.logger.info("packet in %s %s %s %s",dpid, src, dst, in_port)

        self.mac_to_port[dpid][src] = in_port

        if dst in self.mac_to_port[dpid]:
            out_port = self.mac_to_port[dpid][dst]
        else:
            out_port = ofproto.OFPP_FLOOD

        actions = [parser.OFPActionOutput(out_port)]
        if out_port != ofproto.OFPP_FLOOD:
            match = parser.OFPMatch(in_port=inport, eth_dst=dst)
            self.add_flow(datapath, 1, match, actions)
        out = parser.OFPPacketOut(datapath=datapath,buffer_id=ofproto.OFP_NO_BUFFER,in_port=in_port,actions=actions,data=msg.data)
        datapathp.send_msg(out)


    @set_ev_cls(ofp_event.EventOFPStateChange,DEAD_DISPATCHER)
    def connection_dead_controller(self, ev):
        self.logger.debug("dead_dispatcher_my")
```



