#include <stdio.h>
#include <sys/time.h>
#include <time.h>


void fcn(time_t time)
{
    struct tm *ptime = localtime(&time);
    struct tm mytime = *ptime;

    printf("Hello\n");
    
}


int main(int argc, char **argv)
{
    fcn(time(NULL));
    
}
