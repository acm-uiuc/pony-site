#include <stdio.h>

int main()
{
    char *buf = malloc(10);
    buf[-1] = 0;
    buf[11] = 0;
    fprintf(stderr, "yo\n");
    free(buf);
    return 0;	
}
