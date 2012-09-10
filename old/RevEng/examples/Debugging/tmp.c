#include <stdio.h>
#include <sys/time.h>
#define time boogers
#include <time.h>
#undef time

struct mystruct 
{
    char *foo;
    struct tm *bar;
};

int time2(int *foo)
{
    if(foo)
        *foo = 2;
    return 2;
}


