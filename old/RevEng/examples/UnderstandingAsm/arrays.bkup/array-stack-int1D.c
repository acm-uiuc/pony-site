#include <stdio.h>

void intArray()
{
    int buf[512];
    int i;
    for(i = 0; i < 512; i++)
        buf[i] = i;
    

    printArray(buf);
    
}

