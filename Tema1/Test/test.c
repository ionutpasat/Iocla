#include <stdio.h>
#include <stdlib.h>

int main()
{
    unsigned char n = -128 -1;
    char m = 127;
    if(n == m)
        printf("%d\n", 1);

    return 0;
}