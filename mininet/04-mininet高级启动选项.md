# mininet高级启动选项
## 运行回归测试
通过`--test`选项，不需要每次都进入CLI执行操作,而是会自动执行选项对应的操作并将结果输出。
* `sudo mn --test=pingpair`：创建一个mininal拓扑的网络，使用OpenFlow reference控制器，并且执行ping操作，最后关闭网络和控制器。
* `sudo mn --test=iperf`：创建一个mininal拓扑的网络，在其中一台主机运行一个iperf服务器，在另一台主机运行iperf客户端，并解析网络带宽。
## 修改拓扑大小和类型
默认拓扑是一个交换机连接两台主机。可以使用`--topo`以及创建拓扑的参数。例如：
* `sudo mn --test pingall --topo single,3`：创建一个拓扑有一个交换机连接到三台主机上，并执行pingall，返回执行结果并关闭网络。
* `sudo mn --test pingall --topo linear,4`：创建一个线性拓扑，四台交换机都连接到一台主机，并且四台交换机连接到一条线上。
## 链路改变
Mininet 2.0之后的版本可以设置链路参数。
```
 $ sudo mn --link tc,bw=10,delay=10ms //在linux命令行执行
 mininet> iperf           //以下在CLI中执行
 ...
 mininet> h1 ping -c10 h2
```
每条链路的delay是10ms，那么RTT会是40ms，因为ICMP请求穿过两条链路（一条到交换机，一条到目标）并且ICMP响应也经过两条链路。
## 可调输出详细度
默认的输出信息等级是`info`，该等级会打印Mininet启动和停止时做了什么。完整的`debug`输出可以使用`-v`参数指定。例如：
```
$ sudo mn -v debug
...
mininet> exit
```
除此之外还有`output`（）和`warning`（）选项。
## 自定义网络拓扑
可以通过Python API实现方便的自定义网络拓扑，在mininet安装目录中`custom/topo-2sw-2host.py`中是一个示例，它包含两台相连的交换机以及每一台交换机连接一台主机。
```
"""Custom topology example

Two directly connected switches plus a host for each switch:

   host --- switch --- switch --- host

Adding the 'topos' dict with a key/value pair to generate our newly defined
topology enables one to pass in '--topo=mytopo' from the command line.
"""

from mininet.topo import Topo

class MyTopo( Topo ):
    "Simple topology example."

    def __init__( self ):
        "Create custom topo."

        # Initialize topology
        Topo.__init__( self )

        # Add hosts and switches
        leftHost = self.addHost( 'h1' )
        rightHost = self.addHost( 'h2' )
        leftSwitch = self.addSwitch( 's3' )
        rightSwitch = self.addSwitch( 's4' )

        # Add links
        self.addLink( leftHost, leftSwitch )
        self.addLink( leftSwitch, rightSwitch )
        self.addLink( rightSwitch, rightHost )


topos = { 'mytopo': ( lambda: MyTopo() ) }
```
当提供了自定义的mininet文件，它可以增添新的拓扑逻辑、交换机类型并且可以在命令行进行测试，例如：
`sudo mn --custom ~/mininet/custom/topo-2sw-2host.py --topo mytopo --test pingall`。
## 设置ID = MAC
默认情况下，主机启动时会随机分配一个MAC地址。这对调试很不友好。此时`--mac`选项就很有用，它可以将主机的MAC和IP地址设置为更短、唯一且易读的ID。
注：交换机由Linux指定的MAC数据口地址依然是随机的。因为可以通过OpenFlow对交换机的MAC进行指定。
## Xterm显示
为了复杂的测试，可以在启动mininet时使用参数`-x`打开多个窗口，每个窗口对应一台主机或交换机。在对应的窗口执行命令即可。例如：`sudo mn -x`会打开四个窗口，分别对应控制器，交换机和两台主机。

也可以在mininet CLI中使用“名字 xterm &”的方式打开一个xterm窗口。例如`h1 xterm &`或`xterm h1`会为主机h1打开一个xterm窗口（可为一个机器打开多个xterm窗口）。
## 其他交换机类型
使用`--switch`选项可以指定交换机类型。例如：
* `sudo mn --switch user --test iperf`指定交换机为用户态交换机。
注：用户态交换机使用iperf报告的带宽会比前面的内核交换机更低，如果执行前面的ping操作会看到更高的延迟，因为现在需要额外将数据包从内核传到用户态。
* `sudo mn --switch ovsk --test iperf`：这是另一种交换机--Open vSwitch（OVS），他已被提前安装到Mininet VM中，他的iperf TCP带宽应该与OpenFlow内核交换机相似甚至更快。

## Mininet Benchmark
使用`--test`选项的`none`操作，可以记录拓扑的设置和关闭总时间。`sudo mn --test none`。
## 所有东西均处于自己的Namespace（仅限用户态交换机）
默认情况下，只有主机位于各自的NameSpace，而交换机和控制器都在 root NameSpace下。可以使用`--innamespace`将交换机放到自己的NameSpace。例：`sudo mn --innamespace --switch user`。与之前使用loopback与控制器进行通信不同，现在交换机会通过与控制器通信。孤立的讲，这个选项没什么用处，但是确实为如何隔离不同的交换机提供了一个例子。
**注意：该选项对Open vSwitch不起作用。**
