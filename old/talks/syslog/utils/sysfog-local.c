/* When all else fails, blame somebody else :) */

#include <stdio.h>
#include <syslog.h>

void usage(char *prog)
{
    fprintf(stderr, 
	    "Usage %s [<options>] \"process[pid]\" \"message\"\n\n"
	    "Where options are:\n"
	    "\tLog type options:\n"
	    "\t  -p\tLog to AUTHPRIV\t   -d\tLog to DAEMON\n"
	    "\t  -m\tLog to MAIL\t   -u\tLog to NEWS\n"
	    "\t  -c\tLog to CRON\t   -l\tLog to LPR\n"
	    "\t  -s\tLog to SYSLOG\t   -k\tLog to KERNEL\n"
	    "\tLog Priority options:\n"
	    "\t  -e\tError message\t   -w\tWarning Message\n"
	    "\t  -n\tNotice message\t   -i\tInfo Message\n"
	    "\tOther options:\n"
	    "\t  -f n\tFill with 'n' messages (ie to fill /var)\n"
	    "\t  -r n\tGenerate a 'n' byte message of random crap\n"
	    "\t  -R n\tRandomize the process too\n"
	    "\t  -L\tRandomize Log file\n\n"
	    "Note that procces[pid] doesn't have to be a real process and pid, but should\n"
	    "be believable. It might do you good to have <process> be the name of this\n"
	    "executable as well... \n"
	    ,prog);
    
}

int main(int argc, char **argv)
{
#ifdef LOG_AUTHPRIV
	int logtype = LOG_AUTHPRIV;
#else
	int logtype = LOG_AUTH;
#endif
	int priority = LOG_NOTICE;
	char *process = NULL, *message = NULL;
	int argat = 1, fill = 1;
	int randmessage = 0, randproc = 0, randlog = 0;
	int randcrap, i;

	if(argc < 2)
	{
		usage(argv[0]);
		exit(0);
	}

	for(argat = 1; argat < argc; argat++)
	{
		if(argv[argat][0] == '-')
		{
			switch(argv[argat][1])
			{
				case 'p':
#ifdef LOG_AUTHPRIV
					logtype = LOG_AUTHPRIV;
#else
					logtype = LOG_AUTH;
#endif
					break;
				case 'd':
					logtype = LOG_DAEMON;
					break;
				case 'm':
					logtype = LOG_MAIL;
					break;
				case 'u':
					logtype = LOG_NEWS;
					break;
				case 'c':
					logtype = LOG_CRON;
					break;
				case 'l':
					logtype = LOG_LPR;
					break;
				case 's':
					logtype = LOG_SYSLOG;
					break;
				case 'k':
					logtype = LOG_KERN;
					break;
				case 'e':
					priority = LOG_EMERG;
					break;
				case 'w':
					priority = LOG_WARNING;
					break;
				case 'n':
					priority = LOG_NOTICE;
					break;
				case 'i':
					priority = LOG_INFO;
					break;
				case 'L':
					randlog = 1;
					break;
				case 'f':
					if(isdigit(argv[argat+1][0]))
						fill = atoi(argv[++argat]);
					break;
				case 'r':
					if(isdigit(argv[argat+1][0]))
						randmessage = atoi(argv[++argat]);
					break;
				case 'R':
					if(isdigit(argv[argat+1][0]))
						randproc = atoi(argv[++argat]);
					break;
			}
		}
		else if(process == NULL)
		{
			process = argv[argat];
			if(strncmp(process, argv[0], strlen(argv[0])))
			{
				fprintf(stderr, "You probably should rename this executable to be some"
						" form of %s...\n", process);
				exit(1);
			}
		}
		else
		{
			message = argv[argat];
		}
	}

	srand(time(NULL));

	if(randproc)
	{
		process = (char *)calloc(randproc, sizeof(char));
	}
	else if(process == NULL)
	{
		fprintf(stderr, "You need a process, even in fill mode\n");
		exit(1);
	}

	if(randmessage)
	{
		message = (char *)calloc(randmessage, sizeof(char));
	} 
	else if(message == NULL)
	{
		fprintf(stderr, "You need SOME kind of message!\n");
		exit(1);
	}

	if(!randproc && !randlog)
		openlog(process, 0, logtype);

	while(fill-- > 0)
	{
		if(randmessage)
		{	    
			i = 4;
			for(; i < randmessage; i += 4)
			{	    
				randcrap = rand();
				// Eliminate nulls
				randcrap |= 0x01010101;
				memcpy(message+i-4, &randcrap, sizeof(int));
			}
		}
		if(randlog) // Make 'em think that syslogd went nuts :)
		{
			switch(rand() % 8)
			{
				case 0:
#ifdef LOG_AUTHPRIV
					logtype = LOG_AUTHPRIV;
#else
					logtype = LOG_AUTH;
#endif
					break;
				case 1:
					logtype = LOG_DAEMON;
					break;
				case 2:
					logtype = LOG_MAIL;
					break;
				case 3:
					logtype = LOG_NEWS;
					break;
				case 4:
					logtype = LOG_CRON;
					break;
				case 5:
					logtype = LOG_LPR;
					break;
				case 6:
					logtype = LOG_SYSLOG;
					break;
				case 7:
					logtype = LOG_KERN;
					break;
			}
			if(!randproc)
			{
				closelog();
				openlog(process, 0, logtype);
			}
		}
		if(randproc)
		{
			i = 4;
			for(; i < randproc; i += 4)
			{	    
				randcrap = rand();
				// Eliminate nulls
				randcrap |= 0x01010101;
				memcpy(process+i-4, &randcrap, sizeof(int));
			}
			closelog();
			openlog(process, 0, logtype);	
		}
		syslog(priority, message);
	}
}
