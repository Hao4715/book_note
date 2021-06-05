# shell

一个简单的shell脚本，实现通过参数获取ovs网桥名，通过配置文件向网桥添加vxlan端口的功能。

* shell程序

  ```shell
  #!/bin/bash
  if [ $# == 1 ]
  then
          #检查输入参数的网桥是否存在
          br_name=$1
          grep_name="Bridge "$1
      res=$(ovs-vsctl show | grep "$grep_name")
          #返回为空表示用户输入的网桥不存在，退出程序
          if [ -z "$res" ]
          then
                  echo "brige not exist"
                  exit 0
          fi
          echo $res
          #获取配置文件行数，即配置项数目
          num=$(awk 'END{print NR}' vxlan_port_info.cfg)
          echo $num
  
          local_vxlan_ip=()   #存储Vxlan本端IP
          remote_vxlan_ip=()  #存储Vxlan远端IP
          vxlan_vni=()        #Vxlan对应的vni
          i=0
  
          #按行读取文件内容
          while read line
          do
                  local_vxlan_ip[i]=$(echo $line | awk '{print $1}')
                  remote_vxlan_ip[i]=$(echo $line | awk '{print $2}')
                  vxlan_vni[i]=$(echo $line | awk '{print $3}')
                  i=$(($i+1))
          done < vxlan_port_info.cfg
  
          i=0
          while(($i<$num))
          do
                  echo ${local_vxlan_ip[i]}
                  echo ${remote_vxlan_ip[i]}
                  echo ${vxlan_vni[i]}
          port_name="port"$i
              #根据脚本参数以及配置文件信息，为ovs对应的网桥添加如下格式的端口
              #ovs-vsctl add-port xhw vxlantest -- set interface vxlantest type=vxlan options:local_ip=10.0.0.1 options:remote_ip=10.0.0.2 option:key=454656
                  ovs_command="ovs-vsctl add-port "$br_name" "$port_name" -- set interface "$port_name" type=vxlan options:local_ip="${local_vxlan_ip[i]}" options:remote_ip="${remote_vxlan_ip[i]}" option:key="${vxlan_vni[i]}
          echo $ovs_command
                  ${ovs_command}
          i=$(($i+1))
          done
  
  else
          echo "please input one bridge name"
  fi
  ```

  

* 配置文件

  ```
  10.0.0.1 10.0.0.2 454656
  10.0.0.3 10.0.0.4 454657
  ```

  