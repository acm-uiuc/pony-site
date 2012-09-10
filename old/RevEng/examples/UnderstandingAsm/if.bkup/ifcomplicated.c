#include <stdio.h>

int main(int argc, char **argv)
{
    int a;
    int b;
    int c;

    printf("Enter some values:\n");
    scanf("%d\n%d\n%d\n", &a,&b,&c);
    
    if(a<b && b < c)
    {
        printf("a < b < c\n");   
    }
    else if(a == b || b == c || a == c)
    {
        printf("One pair of a,b,c is equal\n");
    }

    printf("End test 1\n");

    if((a == b && b == c) || atoi(argv[1]) < 0)
    {
        printf("SOme stuff happened\n");
    }
    else
    {
        printf("Some stuff didn't happen\n");
    }
}
