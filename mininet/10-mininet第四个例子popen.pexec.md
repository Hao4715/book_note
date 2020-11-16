# 新的popen()/pexec()接口
除了基于shell机制的`cmd()`和`sendCmd()`，现在Mininet支持基于pipe的的接口，他们返回标准Python `Ponen()`对象。这个机制比较新并不像`cmd()`机制一样好，但是该机制能够很便利的在后台运行多个进程并监听他们的输出。`pmonitor()`方法使得监控多个`Popen()`对象更加简单。
```python
def pmonitorTest( N=3, seconds=10 ):
    "Run pings and monitor multiple hosts using pmonitor"
    topo = SingleSwitchTopo( N )
    net = Mininet( topo )
    net.start()
    hosts = net.hosts
    print "Starting test..."
    server = hosts[ 0 ]
    popens = {}
    for h in hosts:
        popens[ h ] = h.popen('ping', server.IP() )
        print "Monitoring output for", seconds, "seconds"
        endTime = time() + seconds
        for h, line in pmonitor( popens, timeoutms=500 ):
            if h:
                print '%s: %s' % ( h.name, line ),
                if time() >= endTime:
                    for p in popens.values():
                        p.send_signal( SIGINT )
        net.stop()
```
**examples/popenpoll.py 程序具有上述功能。**