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
#include <netpacket/packet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/if_ether.h>

struct ppp_head
{
	uint8_t flag;
	uint8_t addr;
	uint16_t control;
	uint16_t protocol;
};
struct ppp_tail
{
	uint16_t FCS;
	uint8_t flag;
};

void attack(int skfd,struct sockaddr_ll *target);
unsigned short check_sum(unsigned short *addr,int len);

int main(int argc,char** argv){
    int skfd;
    struct sockaddr_ll target_ll;//ll
	struct ifreq target_req;//ll
	struct hostent *host;
    const int on=1;
    unsigned short srcport;
	
	bzero(&target_ll,sizeof(target_ll));//ll
	target_ll.sll_family = PF_PACKET;//ll
	target_ll.sll_protocol = htons(ETH_P_ALL);
	strcpy(target_req.ifr_name,"eth0");
    if(0>(skfd=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL))))
    {
        perror("Create Error");
        exit(1);
    }
	errno = 0;
	if(0>ioctl(skfd,SIOCGIFINDEX,&target_req))
	{
		printf("ioctl 1 %s\n",strerror(errno));
		return 0;
	}
	target_ll.sll_ifindex = target_req.ifr_ifindex;	
	/*errno = 0;
	if(0>ioctl(skfd,SIOCGIFADDR,&target_req))
	{
		printf("ioctl 2 %s\n",strerror(errno));
		return 0;
	}
	errno = 0;
	if(0>ioctl(skfd,SIOCGIFHWADDR,&target_req))
	{
		printf("ioctl 3 %s\n",strerror(errno));
		return 0;
	}
	unsigned char macsrc[ETH_ALEN];
	memcpy(macsrc,target_req.ifr_hwaddr.sa_data,ETH_ALEN);
	printf("mac address ==== :");
	int i;
	for(i = 0;i<ETH_ALEN;++i)
	{
		printf(":%02x",macsrc[i]);
	}
	printf("\n");
	*/
	/*
	if(0>ioctl(skfd,SIOCGIFADDR,&target_req))
	{
		printf(strerror(errno));
		printf("ioctl error\n");
		exit(1);
	}*/
    int t = 0;
	while(t < 20)
	{
    	attack(skfd,&target_ll);
		++t;
	}
}
void attack(int skfd,struct sockaddr_ll *target)
{
    char buf[128]={0};
    struct ip *ip;
    struct tcphdr *tcp;
	struct ppp_head *ppp_h;
	struct ppp_tail *ppp_t;
    int ip_len;
    char hello[100] = "hellogffd";
	printf("hello strlen:%d\n",strlen(hello));
	ip_len = sizeof(struct ppp_head) + sizeof(struct ppp_tail) + sizeof(struct ip)+sizeof(struct tcphdr) + strlen(hello);
    
	ppp_h = (struct ppp_head*)buf;
	ppp_h->flag = 0x7E;
	ppp_h->addr = 0xFF;
	ppp_h->control = htons(0x0003);
	ppp_h->protocol = htons(0x0800);

	ip=(struct ip*)(buf + sizeof(struct ppp_head));
    ip->ip_v = IPVERSION;
	printf("struct ip:%d\n",sizeof(struct ip));
	ip->ip_hl = sizeof(struct ip)>>2;
    ip->ip_tos = 0;
    ip->ip_len = htons(ip_len);
    ip->ip_id=0;
    ip->ip_off=0;
    ip->ip_ttl=MAXTTL;
    ip->ip_p=IPPROTO_TCP;
    ip->ip_sum=0;
    inet_aton("172.20.0.4",&ip->ip_dst);
    tcp = (struct tcphdr*)(buf+sizeof(struct ip) + sizeof(struct ppp_head));
    tcp->source = htons(8887);
    tcp->dest = atoi("8888");
    tcp->seq = random();
    tcp->doff = 5;
    tcp->syn = 1;
    tcp->check = 0;
    
    inet_aton("172.20.0.3",&(ip->ip_src));
    tcp->check=check_sum((unsigned short*)tcp,sizeof(struct tcphdr));
	char *ch = (char *)(buf + sizeof(struct ip) + sizeof(struct tcphdr) + sizeof(struct ppp_head));
	strncpy(ch,hello,strlen(hello));
    ppp_t = (struct ppp_tail*)(buf+sizeof(struct ip) + sizeof(struct ppp_head)+sizeof(struct tcphdr) + strlen(hello));
    ppp_t->flag = 0x7E;
    errno = 0;	
    int res = sendto(skfd,buf,ip_len,0,(struct sockaddr*)target,sizeof(struct sockaddr_ll));
    if(res == -1) 
	{
		printf(strerror(errno));
		printf("\n");
		exit(1);
	}
	printf("send _len:%d\n",ip_len);    

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
