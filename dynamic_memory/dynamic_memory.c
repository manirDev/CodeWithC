#include <stdio.h>
#include <stdlib.h>

int main()
{
    char* name;
    name = malloc(32);

    printf("Enter your name: \n");
    scanf("%s", name);
    printf("Hello %s\n", name);

    free(name);
    return 0;
}