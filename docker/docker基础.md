# docker基础
## docker容器数据卷
需求：希望数据持久化或者容器之间共享数据。

docker产生的数据如果不通过`docker commit`生成新的镜像，使得数据作为镜像的一部分被保存下来，那么当容器删除后，数据也就消失了。为了保存数据在docker中使用**卷**。

**特点：**
1. 数据卷可在容器之间共享或重用数据。
2. 卷中的更改可以直接生效。
3. 数据卷中的更改不会包含在镜像的更新中。
4. 数据卷的生命周期一直持续到没有容器使用它为止。

### 容器内添加
* 直接命令命令添加 `docker run -it -v 宿主机绝对路径目录:容器内目录 镜像名`。
  * 查看是否挂载成功：`docker inspect 容器ID`。
  * 带权限添加卷：`docker run -it -v 宿主机绝对路径目录:容器内目录:ro 镜像名`（ro：容器内 read only）。
* DockerFile添加

**步骤：**
1. 根目录新建mydocker（或其他）文件夹并进入。
2. 可在Dockerfile中使用`VOLUME`指令来给镜像添加一个或多个数据卷。
3. Dockerfile构建（创建一个文件，文件名任意，内容如下）。
```
# volume test
FROM centos   #从本地centos镜像构建
VOLUME ["/dataVolumeContainer1","/dataVolumeContainer1"]
CMD echo "finished ----- succes "
CMD /bin/bash
```
4. build后生成镜像：`docker build -f /mydocker/dockerfile -t uno/centos .`。
5. run容器。

**说明：**
* 宿主机路径：通过`docker inspect 容器ID`查看宿主机路径。
## docker容器数据卷容器
命名的容器挂载数据卷，其他容器通过挂载这个（父容器）实现数据共享，挂载数据卷的容器称之为数据卷容器。