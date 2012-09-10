#include <stdio.h>

int **allocate2D(int dim2, int dim1)
{
    int **array;
    int i;
    
    if((array = (int **)malloc(dim2*sizeof(int *))) == NULL)
    {
        printf("Not enough space\n");
        exit(1);
    }
    
    for(i = 0; i < dim2; i++)
        if((array[i] = (int *)malloc(dim1*sizeof(int))) == NULL)
        {
            printf("Not enough space\n");
            exit(1);
        }

    return array;
}

void intArray2D()
{
    int **buf;
    int i, j;
    buf = allocate2D(10,10);
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

