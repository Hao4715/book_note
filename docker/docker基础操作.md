# docker基础操作
## 帮助命令
* `docker version`
* `docker info`
* `docker --help`
## 镜像命令
* `docker images [OPTIONS]` ： 列出本地主机上的镜像
  * OPTIONS说明（可叠加使用）：
  * `docker images -a` ： 列出本地所有的镜像（含中间映像层）
  * `docker images -q` ： 只显示镜像ID
  * `docker images --digests` ： 只显示镜像的摘要信息
  * `docker images --no-trunc` ： 显示完整的镜像信息
* `docker search [OPTIONS] 镜像名称` ：在 http://hub.docker.com 查询镜像信息。
  * OPTIONS说明 （可叠加使用）： 
  * `docker search -s 指定数量 镜像名称` ： 列出收藏数不小于指定值的镜像。
  * `docker search --no-trunc 镜像名称` ： 显示完整的镜像信息。
   * `docker search --automated 镜像名称` ： 只列出 automated build 类型的镜像。
* `docker pull 镜像名称` ：下载镜像（默认为下载最新版相当于`docker pull 镜像名称:latest`，可通过`docker pull 镜像名称:版本号`下载指定版本）。
* `docker rmi 镜像名称|镜像ID` ：删除镜像（默认为删除最新版相当于`docker rmi 镜像名称:latest`，可通过`docker rmi 镜像名称:版本号`删除指定版本）。
  * `docker rmi -f 镜像名:版本号 镜像名:版本号` ： 删除多个（`-f`表示强制删除，不指定版本号默认删除latest）。
  * `docker rmi -f $(docker images -qa)` ： 删除全部。
## 容器命令
### 新建并启动容器
`docker run [OPTIONS] IMAGE [COMMAND] [ARG...]`

**OPTIONS:**
* `--name="容器新名字"` ： 为容器指定一个名称。
* `-d` ： 后台运行容器，并返回容器ID，也即启动守护式容器。
* `-i` ： 以交互模式运行容器，通常与 -t 同时使用。
* `-t` ： 为容器重新分配一个伪输入终端，通常与 -i 同时使用。
* `-P` ： 随机端口映射。
* `-p` ： 指定端口映射，有以下四种格式：
  * ip:hostPort:containerPort
  * ip::containerPort
  * hostPort:containerPort
  * containerPort
  * 例子：
    1. `docker run -it -p 8080:8080 nginx` ： 指定端口映射。
    2. `docker run -it -P nginx` ： 随机端口映射。
### 列出当前所有正在运行的容器
`docker ps [OPTIONS]`

**OPTIONS:**
* `-a` ： 列出当前所有正在运行的容器+历史上运行过的。
* `-l` ： 显示最近创建的容器。
* `-n [num]` ： 显示最近num个创建的容器。
* `-q` ： 静默模式，只显示容器编号。
* `--no-trunc` ： 不做截断输出。
### 退出容器
* `exit` ： 容器停止并退出。
* `ctrl+p+q`（键盘按键）：容器不停止，退出容器。
### 启动容器
`docker start 容器ID或名字`
### 重启容器
`docker start 容器ID或名字`
### 停止容器
`docker stop 容器ID或名字`
### 强制停止容器
`docker kill 容器ID或名字`
### 删除已停止的容器
`docker rm 容器ID`

`docker rm -f 容器ID` ： 强制删除。

`docker rm -f $(docker ps -a -q)` ： 一次性删除多个容器。

`docker ps -a -q | xargs docker rm` ： 一次性删除多个容器。
### 启动守护式容器
`docker run -d 容器名`

**说明：**执行后再使用`docker ps -a`进行查看，会发现容器已经退出。因为**docker容器后台运行就必须有一个前台进程。**容器运行的命令如果不是那些**一直挂起的命令（如top，tail），**就会自动退出。
### 查看容器日志
`docker logs -f -t --tail 数量n 容器ID`
* `-t` ： 加入时间戳。
* `-f` ： 跟随最新的日志打印。
* `--tail` ： 数字显示最后n条日志。
### 查看容器内运行的进程
`docker top 容器ID`
### 查看容器内部细节
`docker inspect 容器ID`
### 进入正在运行的容器并以命令行交互
* `docker attach 容器ID` ： 进入容器。
* `docker exec -it 容器ID 待执行命令` ： 不进入容器，执行命令并获得执行结果（`docker exec -t 容器ID /bin/bash` == `docker attach 容器ID`）。

**区别：**
* attach ： 直接进入容器启动命令终端，不会启动新的进程。
* exec ： 在容器中打开新的终端，并且可以启动新的进程。
### 从容器内拷贝文件到主机上
`docker cp 容器ID:容器内路径 目的主机路径`
### 制作镜像
`docker commit -a="author" -m="message" 容器ID uno/newContainer:2.2` ： 会在本地生成一个新版本的镜像。