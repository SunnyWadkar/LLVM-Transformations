#include <stdio.h>

int main(){
    int x1, x2, x3;

    x1 = 5;
    x2 = 9;
    x3 = 2;

    if(x2)
    {
        x1 = x1 + 5; // has to be removed
    }
    else{
        x1 = x1 + 6; // has to be removed
    }

    x1 = x2 + 1; // has to be removed
    x2 = x1 + 2; // has to be removed
    x3 = 2 + x3;

    return x3;
}