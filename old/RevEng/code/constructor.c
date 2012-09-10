#include <stdio.h>

int _my_begin() __attribute__((constructor));

int _my_begin()
{
    printf("Hi. I live at 0x%x Computer St.\n", &_my_begin);
}
