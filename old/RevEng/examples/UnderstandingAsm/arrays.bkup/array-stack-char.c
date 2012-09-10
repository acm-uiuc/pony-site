#include <stdio.h>

void charArray()
{
    char buf[512];
    
    strncpy(buf, "hello there, govna!", sizeof(buf)-1);
    
    buf[strlen("hello there, govna!")] = 0;
    
    printArray(buf);
}

