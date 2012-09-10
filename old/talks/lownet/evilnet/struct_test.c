#include <stdio.h>
#include <netinet/if_ether.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>

struct tcppacket
{
    char eth[14];
    char ip[20];
    char tcp[20];
 /*       char buff[ETH_MTU - sizeof(eth) - sizeof(ip) -
	sizeof(tcp)]; */
};


void main()
{
    struct tcppacket t;
    
    if(sizeof(struct tcppacket) != sizeof(struct ethhdr) + 
	    sizeof(struct iphdr) + sizeof(struct tcphdr))
    {
	printf("ugly... so ugly\n");
	printf("packet %d\n\n-------\neth %d\nip %d\ntcp %d\n", 
		sizeof(struct tcppacket), sizeof(struct ethhdr), 
		sizeof(struct iphdr), sizeof(struct tcphdr));
    }
}
