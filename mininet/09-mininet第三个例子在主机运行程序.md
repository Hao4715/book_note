# mininet第三个例子：在主机运行程序
每个Mininet主机本质上是一个bash shell进程连接到一个或多个网络接口，所以与它交互的最简单方式是使用```(cmd)```方法发送命令到shell。


* 使用```cmd()```命令。在主机执行命令并获得输出。
```python
    h1 = net.get('h1')	
    result = h1.cmd('ifconfig')
    print result
```

* 很多时候主机需要在后台运行一个程序，关闭程序，并将输出保存到文件。
```python
from time import sleep
...
print "Starting test..."
h1.cmd('while true; do date; sleep 1; done > /tmp/date.out &')
sleep(10)
print "Stopping test"
h1.cmd('kill %while')
print "Reading output"
f = open('/tmp/date.out')
lineno = 1
for line in f.readlines():
    print "%d: %s" % ( lineno, line.strip() )
    lineno += 1
f.close()
```
**注意：Mininet退出（故意退出或错误退出）时，并不保证所有运行在后台的程序被关闭。因此需要保证手动关闭任务。因此可能需要定期使用`ps`命令来保证没有僵尸进程拖慢系统速度。**
* 为了利用shell的wait机制，Mininet本身允许通过`sendCmd()`一个前台命令并通过`waitOutput()`等待命令完成。
```python
for h in hosts:
    h.sendCmd('sleep 20')
…
results = {}
for h in hosts:
    results[h.name] = h.waitOutput()
```
* `examples/multipoll.py`提供了一个`monitorFiles()`函数提供了一种监控多个输出文件的机制。这简化了监测多个主机输出的流程。
```python
def monitorTest( N=3, seconds=3 ):
    "Run pings and monitor multiple hosts"
    topo = SingleSwitchTopo( N )
    net = Mininet( topo )
    net.start()
    hosts = net.hosts
    print "Starting test..."
    server = hosts[ 0 ]
    outfiles, errfiles = {}, {}
    for h in hosts:
        # Create and/or erase output files
        outfiles[ h ] = '/tmp/%s.out' % h.name
        errfiles[ h ] = '/tmp/%s.err' % h.name
        h.cmd( 'echo >', outfiles[ h ] )
        h.cmd( 'echo >', errfiles[ h ] )
        # Start pings
        h.cmdPrint('ping', server.IP(),
                   '>', outfiles[ h ],
                   '2>', errfiles[ h ],
                   '&' )
    print "Monitoring output for", seconds, "seconds"
    for h, line in monitorFiles( outfiles, seconds, timeoutms=500 ):
        if h:
            print '%s: %s' % ( h.name, line )
    for h in hosts:
        h.cmd('kill %ping')
    net.stop()
```
**注参考examples/multipoll.py 和 examples/multiping.py**
