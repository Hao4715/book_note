# mininet第一个例子：创建网络拓扑
代码：
```python
#!/usr/bin python
from mininet.topo import Topo    #Mininet拓扑的基础类
from mininet.net import Mininet
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel

class SingleSwitchTopo(Topo): 
    "Single switch connected to n hosts."
    def build(self, n=2):        #重写build函数
        switch = self.addSwitch('s1') #添加交换机
        # Python's range(N) generates 0..N-1
        for h in range(n):      #循环添加四台主机并将主机分别于交换机建立链路
            host = self.addHost('h%s' % (h + 1)) #向网络中添加主机
            self.addLink(host, switch) #向网络中两个节点之间添加链路

def simpleTest():
    "Create and test a simple network"
    topo = SingleSwitchTopo(n=4)
    net = Mininet(topo)    #初始化网络，返回值为创建和管理网络类的对象
    net.start()    #开启网络
    print "Dumping host connections"
    dumpNodeConnections(net.hosts)  #获取节点连接信息，net.hosts表示网络中所有节点
    print "Testing network connectivity"
    net.pingAll()  #通过每个节点ping其他所有节点测试网络连通性
    net.stop()     #关停网络

if __name__ == '__main__':
    # Tell mininet to print useful information
    setLogLevel('info')      #设置Mininet的输出级别
    simpleTest()             #创建网络环境
```