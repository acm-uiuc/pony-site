#include <stdio.h>

void intArray2D()
{
    int buf[10][10];
    int i, j;
    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 10; j++)
        {
            if(i == j)
                buf[i][j] = 1;
            else
                buf[i][j] = 0;

        }
    }
    printArray(buf);
}

