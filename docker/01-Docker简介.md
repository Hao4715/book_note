# Docker简介

https://docs.docker.com/get-started/overview/

Docker是一个用来开发、发布和运行程序的平台。Docker可以将应用与基础环境分离从而实现快速的软件交付。通过Docker可以按照管理应用的方式管理基础环境。利用Dokcer发布、测试和能够快捷部署代码的机制，能够显著地减少产品代码编写与部属之间的延迟。
## Docker平台
Docker提供了打包和运行应用在一个松散隔离环境的能力--容器（Container）。这种隔离性和安全性允许用户同时在一个主机运行多个容器（Container）。容器轻量化的原因是不需要额外加载管理程序，而是直接在主机的内核中运行。这意味着与虚拟机相比，在一台主机上可以运行更多的Docker容器，甚至可以在虚拟机的虚拟主机上运行Docker容器。

Docker提供了管理容器生命周期的工具和容器：
* 使用容器开发应应用和应用支持的组件
* 容器作为了发布和测试应用的单元
* 准备就绪后，以容器或协调服务的形式将应用发布到生产环境（无论生产环境是本地数据中心或云提供商或者两者混合）。
## Docker引擎
Docker引擎是一个具有以下主要组件的客户端-服务器应用：
* 一个名为 daemon process（守护进程）的持续运行的Server程序（`dockerd`命令）。
* REST API，他指定程序可以用来与守护进程进行通信并下发指示的接口。
* 命令行接口（CLI）客户端（`docker`）命令。

注：server（docker守护进程）位于最中心，次中心是REST API，最外层是CLI（直接管理network，container，image，data volumes）。CLI通过脚本或者直接CLI命令使用Docker Rest API来控制或者与Docker守护进程交互。守护进程创建和管理Docker对象。例如images，containers，networks和volums。
## 用途
### 快速、一致的交付应用程序
Docker允许开发人员使用包含了应用和服务的本地化标准环境而简化了开发周期。容器非常适合持续集成和持续交互的工作流程。
### 响应式部署和扩展
### 在相同硬件环境运行更多工作负载
## Docker架构
Docker使用客户端-服务器架构。Docker客户端与Docker daemon通信，Docker daemon完成构建、运行和分发Docker容器的繁重工作。客户吨与daemon可以运行在同一个系统中也可以使用客户端连接到一个远程的daemon。Docker客户端通过REAT API、UNIX socket或网络接口与daemon进行通信。
## Dokcer daemon
Docker daemon（dockerd）监听Docker API请求和管理Docker对象，比如：images、containers、networks和volums。daemon也可以与其他daemon通信来管理Docker服务。
## Docker 客户端
Docker客户端（docker）通过Docker API将命令发送到daemon（dockerd）。一个客户端可以与多个daemon通信。
## Docker 仓库
Docker仓库存储Docker镜像。Docker Hub是一个公用仓库并且是Docker默认使用的仓库（用户可以自定义使用哪个仓库）。

执行`docker pull`和`docker run`命令从指定的仓库拉取镜像。`docker push`命令回将镜像推送到指定的仓库。
## Docker 对象
Docker对象包括images（镜像）、 containers（容器）、networks（网络）、 volumes（数据卷）、 plugins（插件）、 and other objects（其他对象）。
## IMAGES

一个image是一个包含创建Docker容器指令的只读模板。通常一个镜像都是基于其他镜像并有一些自定义。例如，基于ubuntu镜像和其他环境的Apache Web服务器镜像或者其他应用镜像。

可以使用自己创建的镜像或者他人发布在仓库中的镜像。创建自己的镜像需要创建具有简单句法的Dockerfile来定义创建和运行的步骤。Dockerfile中每个指令都创建一个层。当修改Dockerfile和重建镜像时，只对更改的层进行重建。这是镜像比其他虚拟化技术更轻量化，小和快的原因之一。

## 容器

容器时镜像的可执行实例。可以使用Docker API 或 CLI创建、启动、停止、移动或删除容器。可以将容器连接到一个或多个网络、为他连接存储器或甚至基于容器当前状态创建新的镜像。

默认一个容器与其他容器和宿主机有较好的隔离性，但是用户可以控制容器与其他容器和宿主机在网络、存储或其他底层系统的隔离性。

容器由他的镜像和用户创建或启动时给予的选项所定义。当一个容器被删除，他的任何未被永久存储的状态将消失。

### `docker run`命令示例

`docker run -i -t ubuntu /bin/bash`命令创建一个ubuntu容器，交互式的连接到本地的命令行绘画并且运行/bin/bash。

命令过程：

1. 如果本地没有ubuntu镜像，Docker从指定的曾库拉取镜像（相当于手动执行`docker pull ubuntu`）。
2. Docker创建一个新的容器，相当于手动执行`docker container create`命令。
3. Docker为容器创建一个可写可读的文件系统作为容器最后一层。使得运行的容器可以在容器的件系统中创建或修改文件和目录。
4. Docker创建一个网络接口来将容器连接到默认网络，即使用户没有指定任何网络操作。该步包含为容器分配IP地址。默认情况下，容器能够使用宿主机的网络连接连接到外部网络。
5. Docker开启容器并执行`/bin/bash`。因为容器是交互式启动并附属于用户终端（因为使用了`-i`和`-t`选项），用户可以在提示连接成功后使用键盘输入到容器。
6. 使用`exit`命令来关闭`/bin/bash`，该容器停止运行但不会被删除。可以撤哦那个其或移除它。



















