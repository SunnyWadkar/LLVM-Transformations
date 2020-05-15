#include <stdio.h>

int test(int x)
{ 
    int a = 1;
    int p=0;
    int q = 0;
    for(int i=0;i<100;i++)
    {
        p = a + 4; // This code needs to be hoisted
        q = x + p; // This code needs to be hoisted
    }
    return p;
}

int main()
{
    int r = test(3);
    printf("result is: %d\n",r);
    return 0;
}