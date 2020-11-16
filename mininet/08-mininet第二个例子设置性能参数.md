# mininet第二个例子：设置性能参数
设置主机参数
```python
#!/usr/bin/python

from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import CPULimitedHost
from mininet.link import TCLink
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel

class SingleSwitchTopo( Topo ):
    "Single switch connected to n hosts."
    def build( self, n=2 ):
	switch = self.addSwitch( 's1' )
	for h in range(n):
	    # Each host gets 50%/n of system CPU，将系统CPU资源的50%分配给虚拟主机
	    host = self.addHost( 'h%s' % (h + 1),
		                 cpu=.5/n )
	    # 10 Mbps, 5ms delay（可指定为1s，100us，5ms）, 2% loss, 1000 packet queue,(最多容纳package数量)，
        # htb：HTB (Hierarchical Token Bucket)算法的流量管理功能，可有效提高带宽利用率
        # 
        # 可写为：
        #  linkopts = dict(bw=10,delay='5ms',loss=10,max_queue_size=1000,use_htb=True)
        #  (or you can use brace syntax: linkopts = {'bw':10, 'delay':'5ms', ... } )
        #  self.addLink(node1, node2, **linkopts)
	    self.addLink( host, switch, bw=10, delay='5ms', loss=2,
                          max_queue_size=1000, use_htb=True )

def perfTest():
    "Create network and run simple performance test"
    topo = SingleSwitchTopo( n=4 )
    net = Mininet( topo=topo,
	           host=CPULimitedHost, link=TCLink )
    net.start()
    print "Dumping host connections"
    dumpNodeConnections( net.hosts )
    print "Testing network connectivity"
    net.pingAll()
    print "Testing bandwidth between h1 and h4"
    # 获取到主机后，可通过命令 host.cmd() 执行命令并获取输出
    h3=net['h3']
    ping h3.cmd('ifconfig')
    
    h1, h4 = net.get( 'h1', 'h4' ) 
    net.iperf( (h1, h4) ) #在两个主机之间执行iperf命令，iperf是一个网络性能测试工具
    net.stop()

if __name__ == '__main__':
    setLogLevel( 'info' )
    perfTest()
    ```