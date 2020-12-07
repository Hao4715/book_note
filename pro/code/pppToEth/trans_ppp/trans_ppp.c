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
#include <net/ethernet.h>

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

void receive(int skfd,struct sockaddr_ll *target);
int trans(char *ppp_buf,int ppp_len,char *eth_buf,int *eth_buf_len);
//unsigned short check_sum(unsigned short *addr,int len);

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
	while(1)
	{
    	receive(skfd,&target_ll);
	}
}
void receive(int skfd,struct sockaddr_ll *target)
{
	
    char buf[2048]={0};
    /*struct ip *ip;
    struct tcphdr *tcp;
	struct ppp_head *ppp_h;
	struct ppp_tail *ppp_t;
    */
	int ip_len;
	/*
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
	*/
	int len;
    errno = 0;	
	int eth_buf_len = 0;
	char eth_buf[2048] = {0};
    int res = recvfrom(skfd,buf,2048,0,(struct sockaddr*)target,&len);
    if(res == -1) 
	{
		printf(strerror(errno));
		printf("\n");
		exit(1);
	}
	//printf("recv _len:%d    len:%d\n",res,len);    
	res = trans(buf,res,eth_buf,&eth_buf_len);
	errno = 0;
	res = sendto(skfd,eth_buf,eth_buf_len,0,(struct sockaddr*)target,sizeof(struct sockaddr_ll));
	printf(strerror(errno));
	printf("send_len:%d\n",res);
}

int trans(char *ppp_buf, int ppp_len,char *eth_buf, int *eth_buf_len)
{
	printf("recv _len:%d\n",ppp_len);    
	struct ether_header *eth_head;
	
	eth_head = (struct ether_header*)eth_buf;

	eth_head->ether_dhost[0] = 0x36;
	eth_head->ether_dhost[1] = 0xba;
	eth_head->ether_dhost[2] = 0x9d;
	eth_head->ether_dhost[3] = 0x80;
	eth_head->ether_dhost[4] = 0x1a;
	eth_head->ether_dhost[5] = 0x36;
	
	eth_head->ether_shost[0] = 0x1a;
	eth_head->ether_shost[1] = 0xe0;
	eth_head->ether_shost[2] = 0x12;
	eth_head->ether_shost[3] = 0xd0;
	eth_head->ether_shost[4] = 0x86;
	eth_head->ether_shost[5] = 0x2e;

	eth_head->ether_type = htons(0x0800);
	
	int ppp_head_len = sizeof(struct ppp_head);
	
	char *con = (char *)eth_buf + sizeof(struct ether_header);
	char *start = ppp_buf + sizeof(struct ppp_head);
	int len =  ppp_len - ppp_head_len; 
	
	memcpy(con,start,len*sizeof(char));
	//strncpy(con,start,len);
	printf("eth_len:%d\n",strlen(eth_buf));
	*eth_buf_len = ppp_len - sizeof(struct ppp_head) + sizeof(struct ether_header);
	return 1;
}


/*
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
}*/
