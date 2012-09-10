#include <stdio.h>

struct mystruct
{
    int e1;
    char e2;
    short e3;
};

struct mystruct retstruct()
{
    struct mystruct ms;
    ms.e1 = 1;
    ms.e2 = 2;
    ms.e3 = 3;

    return ms;
}

void passtruct(struct mystruct ms)
{
    ms.e1 = 1;
    ms.e2 = 2;
    ms.e3 = 3;
    
    printf("%d, %d, %d\n", ms.e1, ms.e2, ms.e3);
}

int main(int argc, char **argv)
{
    struct mystruct ms;

    ms = retstruct();

    passstruct(ms);

    printf("%d %d %d\n", ms.e1, ms.e2, ms.e3);
}
