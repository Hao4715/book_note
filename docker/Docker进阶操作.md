# Docker 进阶操作

[TOC]

## 修改docker0默认网段

1、创建/etc/docker/daemon.json（该文件的配置项可以通过“man dockerd”查看），写入如下内容：

```shell
{
	"bip":"*.*.*.1/*",
	"mtu":*
}
```

2、重启docker。（需保证没有容器在运行）