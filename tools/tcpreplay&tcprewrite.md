[TOC]

## tcprewrite

* 生成中间文件：`tcpprep -p --pcap=test.pcap --cachefile=test_cache.pcap`
* 修改ip（需要使用中间缓存文件）：`tcprewrite -i eth.pcap -o output.pcap --cachefile=test_cache.pcap --endpoints=10.0.0.1:10.0.0.2`（目的ip:源ip）
* 修改mac（不需要缓存文件）：`tcprewrite --enet-dmac=00:55:22:AF:C6:37 --enet-smac=00:44:66:FC:29:AF --infile=input.pcap --outfile=output.pcap`

## tcpreplay

* 放包：`tcpreplay -i enp5s0f1 -p 1000 pos_rewrite.pcap`

## grep

* 在目录中所有文件中匹配串：`grep "test" ./ -nr`