#include <stdio.h>

void function3args(int a, int b, int c)
{
    printf("%d, %d, %d\n", a,b,c);
}

int function3argsRet(int a, int b, int c)
{
    return a*b*c;
}

void functionPtrArg(int a, int b, int *c)
{
    printf("%d, %d, %d\n", a,b,*c);
}

int *functionPtrRet(int a, int b, int *c)
{
    return c + a + b;
}

int functionLocalVars(int a, int b)
{
    int localA;
    int localB;
    int localC;

    localA = a ^ b;
    localB = a / b;
    localC = a % b;

    localC = localA | localB;

    return a & localC;
}


int main(int argc, char **argv)
{
    int a;
    int *ptr;
    
    asm("nop");

    function3args(1,2,3);

    asm("nop");
    
    a = function3argsRet(1,2,3);
    
    asm("nop");

    functionPtrArg(1,3, &a);
    
    asm("nop");

    ptr = functionPtrRet(1,3, &a);
    
    asm("nop");
   
    a = functionLocalVars(1,2);

    asm("nop");
}

