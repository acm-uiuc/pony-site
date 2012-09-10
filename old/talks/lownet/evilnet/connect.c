/* CS323 MP5
 * Mike Perry
 * mperry1
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>

#define TIMEPORT 37 // port of the timeserver
#define JEFFPORT 4598 // port to post netid

#define EPOCH_OFFSET ((unsigned int)2208988800) // The start of the UNIX era
#define NETID_PADDED    "mperry1     "
#define NETID_LEN       12

/* Make a connectio to host host on port port. Return the socket */
int connect_to_host(char *host, int port)
{
    int sockfd;
    struct hostent *he;
    struct sockaddr_in their_addr; // connector's address information 

    printf("Connecting to %s...\n", host);
    if ((he=gethostbyname(host)) == NULL) {  // get the host info 
        perror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    their_addr.sin_family = AF_INET;    // host byte order 
    their_addr.sin_port = htons(port);  // short, network byte order 
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(their_addr.sin_zero), '\0', 8);  // zero the rest of the struct 

    if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }

    return sockfd;
}


int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char *chrtime;
    int i;
    time_t thetime;

    if(argc != 3) {
        fprintf(stderr,"usage: %s timeserver postserver\n", argv[0]);
        exit(1);
    }

    /* Make connection */
    sockfd = connect_to_host(argv[1], TIMEPORT);

    /* read the time data in */
    if((numbytes=recv(sockfd, &thetime, sizeof(thetime), MSG_WAITALL)) != sizeof(thetime)) 
    {
        perror("recv");
        exit(1);
    }
    
    close(sockfd);
    printf("Closed connection.\n");

    /* Calculate UNIX time from read value */    
    thetime = ntohl(thetime);
    thetime -= EPOCH_OFFSET;
    chrtime = ctime(&thetime);
    printf("Time from remote host: %s", chrtime);     
    
    /* Print localtime */
    time(&thetime);
    chrtime = ctime(&thetime);
    printf("Time at local machine: %s", chrtime);     
  
    /* Connect to post id */
    sockfd = connect_to_host(argv[2], JEFFPORT);

    /* send id */
    if((numbytes=write(sockfd, NETID_PADDED, NETID_LEN) != NETID_LEN))
    {
        perror("write");
        exit(1);
    }
    printf("Posted NetID.\n");

    /* send time */
    thetime = htonl(time(NULL));
    if((numbytes=write(sockfd, &thetime, sizeof(thetime)) != sizeof(thetime)))
    {
        perror("write");
        exit(1);
    }
    printf("Posted time.\n");

    close(sockfd);
    printf("Closed connection.\n");
    
    return 0;
}
