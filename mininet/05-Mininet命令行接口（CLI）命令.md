# Mininet 命令行接口（CLI）命令
## 显示选项
执行`sudo mn`打开一个简单的虚拟网络，在CLI中输入`help`即可查看CLI 选项。
## Python解释器
如果在Mininet命令行中输入的第一个短语是`py`，表示这条命令将使用Python执行。这对扩展Mininet和探究内部执行很有用。每个主机、交换机和控制器都有一个关联的Node对象。例如：（先执行`sudo mn`命令打开一个简单的网络并进入CLI）
* `py 'hello ' + 'world'`：打印“hello world”。
* `py locals()`：打印可访问的局部变量。
* `py dir(s1)`：查看节点s1可用的方法和属性。
* `py help(h1) `：查看h1节点可用方法的在线文档（按“q”退出）。
* `py h1.IP()`：调用h1的IP()方法获取h1的IP地址。
## 链路启/停
`link s1 h1 down`：禁用交换机s1与主机h1的链路。
`link s1 h1 down`：启用交换机s1与主机h1的链路。
## Xterm显示
`xterm h1 h2`：为主机h1和主机h2打开xterm。
## 远程控制器
在远程机器（本及其他进程、其他机器、云端等）启动控制器。之后再本地执行命令`sudo mn --controller=remote,ip=[controller IP],port=[controller listening port]`即可使用远程控制器。