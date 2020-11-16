# 在两台主机分别运行nginx

1. 通过`sudo mn`创建最简单的网络环境（mininal）。
2. 第一步过后会进入到mininetCLI中，在其中执行`xterm h1 h2`，为主机1和主机2打开独立的操作窗口。
3. 先在主机h1执行ningx命令开启nginx，然后在主机2使用命令`wget -O - 10.0.0.1`能成功访问主机1的nginx服务器。
4. 在主机h2执行ningx命令开启nginx，然后在主机1使用命令`wget -O - 10.0.0.2`能成功访问主机2的nginx服务器。
5. 此时在宿主机会看到两个你nginx相关进程。
6. 此时在主机1执行命令`nginx -s quit`关闭主机1的nginx。

那么问题来了：
接下来会发现，主机2的nginx被停止，在宿主机下会看到主机2后开启的nginx被关闭。

此时在主机2再次执行`wget -O - 10.0.0.1`依旧能访问到主机1的nginx，并且此时在主机1执行`nginx -s quit`会报错：`nginx: [error] open() "/run/nginx.pid" failed (2: No such file or directory)`。由此可见不同的主机是共享的文件系统，主机1开启nginx以后/run/nginx.pid文件记录的是主机1中nginx的pid，当在主机2再次开启nginx时，主机2的nginx的pid会将该文件覆盖掉。

也就导致在主机1关闭nginx时，实际是将后开启的nginx关闭了。然后pid文件被删除，无法通过nginx命令关闭先开启的nginx。

然而两个主机能运行两个监听80端口的服务器，也进一步说明，在mininet中不同主机的网络环境是处于不同的Namespace而互相隔离的。但是进程以及文件还是与主机共享的。