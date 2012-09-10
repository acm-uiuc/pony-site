#include <stdio.h>

void charArray()
{
    char buf[512];
    
    strncpy(buf, "hello there, govna!", sizeof(buf)-1);
    
    buf[strlen("hello there, govna!")] = 0;
    
    printArray(buf);
}

void intArray()
{
    int buf[512];
    int i;
    for(i = 0; i < 512; i++)
        buf[i] = i;
    

    printArray(buf);
    
}


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
int main(int argc, char **argv)
{
}
