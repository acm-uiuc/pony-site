#include <stdio.h>

int ***allocate3D(int dim3, int dim2, int dim1)
{
    int ***array;
    int i, j;

    if((array = (int ***)malloc(dim3*sizeof(int **))) == NULL)
    {
        printf("Not enough space\n");
        exit(1);
    }

    for(i = 0; i < dim3; i++)
    {
        if((array[i] = (int **)malloc(dim2*sizeof(int*))) == NULL)
        {
            printf("Not enough space\n");
            exit(1);
        }
        for(j = 0; j < dim2; j++)
        {
            if((array[i][j] = (int *)malloc(dim1*sizeof(int))) == NULL)
            {
                printf("Not enough space\n");
                exit(1);
            }
        }
    }

    return array;
}

void intArray3D()
{
    int ***buf;
    int h, i, j;

    buf = allocate3D(3,10,10);
    
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
