#include <stdio.h>

void intArray3D()
{
    int buf[3][10][10];
    int h, i, j;
    
    for(h = 0; h < 3; h++)
    {
        for(i = 0; i < 10; i++)
        {
            for(j = 0; j < 10; j++)
            {
                if(i == j)
                    buf[h][i][j] = h;
                else
                    buf[h][i][j] = 0;

            }
        }
    }
    printArray(buf);
}
