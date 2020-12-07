## tcprewrite

* 生成中间文件：`tcpprep -p --pcap=test.pcap --cachefile=test_cache.pcap`
* 修改ip：`tcprewrite -i eth.pcap -o output.pcap --cachefile=test_cache.pcap --endpoints=10.0.0.1:10.0.0.2`（目的ip:源ip）
* 修改mac：``

## tcpreplay

* 放包：`tcpreplay -i enp5s0f1 -p 1000 pos_rewrite.pcap`