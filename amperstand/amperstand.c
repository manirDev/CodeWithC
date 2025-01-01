#include <stdio.h>

void addition(int *target, int a, int b)
{
    *target = a + b;
}

int main()
{
    int x, y, ans;
    x = 5;
    y = 15;
    addition(&ans, x, y);
    printf("%d\n", ans);
    return 0;
}