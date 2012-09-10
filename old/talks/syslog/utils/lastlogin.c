/* 
 * This is a lastlogin editor for ultrix's and suns 
 * maybe bsd too.
 */
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <lastlog.h>
#include <pwd.h>
char *cbp;
int gpair()
{   /* taken from touch, thanks UCB :) */
        register int c, d;
        register char *cp;
        cp = cbp;
    if (*cp == 0) return (-1);
        c = (*cp++ - '0') * 10;
    if (c<0 || c>100) return (-1);
    if (*cp == 0) return (-1);
    if ((d = *cp++ - '0') < 0 || d > 9) return (-1);
        cbp = cp;
        return (c+d);
}
main(argc,argv)
int argc;
char *argv[];
{
    static struct tm time;
    time_t timbuf;
    int f;
    struct lastlog newll;
    struct passwd *pw;
    if (argc==6) {
        if ((pw=getpwnam(argv[1])) == NULL) {
        fprintf(stderr,"no such account\n\r"); exit(2); }
        cbp=(char *)argv[2];
        time.tm_mon=gpair()-1; time.tm_mday=gpair(); time.tm_hour=gpair();
        if (time.tm_hour == 24) { time.tm_hour = 0; time.tm_mday++; }
        time.tm_min=gpair();   time.tm_sec=gpair();  time.tm_year=100+gpair();
		fprintf(stderr, "%d\n", time.tm_year); 
        timbuf=timelocal(&time);
        newll.ll_time=timbuf;
        strcpy(newll.ll_line,argv[3]);
        strcpy(newll.ll_host,argv[4]);
        if ((f = open(argv[5], O_RDWR)) >= 0) {
            lseek(f, (long)pw->pw_uid * sizeof (struct lastlog), 0);
            write(f, (char *) &newll, sizeof newll);
            close(f);
        } else printf("error opening lastlog\n\r");
    } else printf("usage: %s user mmddhhmmssyy tty site\n\r",argv[0]);
}
