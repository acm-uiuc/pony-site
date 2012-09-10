#include <stdio.h>

int *allocate1D(int size)
{
    int *array;
    if((array = (int*)malloc(size*sizeof(int))) == NULL)
    {
        printf("Out of memory\n");
        exit(1);
    }
    return array;
}

void intArray()
{
    int *buf;
    int i;

    buf = allocate1D(512);
    for(i = 0; i < 512; i++)
        buf[i] = i;
    

    printArray(buf);
    
}

