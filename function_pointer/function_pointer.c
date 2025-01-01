#include <stdio.h>
#include <stdlib.h>

#define F fflush(stdout)
#define ADD 0x01
#define SUB 0x02
#define MUL 0x03
#define DIV 0x04

void division(int *res, int a, int b)
{
    *res = a / b;
}

void multiplication(int *res, int a, int b)
{
    *res = a * b;
}

void addition(int *res, int a, int b)
{
    *res = a + b;
}

void substraction(int *res, int a, int b)
{
    *res = a - b;
}

int main()
{
    int a, b, res;
    void (*function)(int *, int, int);
    int option;
    printf("Press:\n0-Quit\n1-ADD\n2-SUB\n3-MUL\n4-DIV\n");
    scanf("%d", &option);

    switch (option)
    {
    case ADD:
        function = addition;
        break;
    case SUB:
        function = substraction;
        break;
    case MUL:
        function = multiplication;
        break;
    case DIV:
        function = division;
        break;
    default:
        return 0;
        break;
    }
    printf("NUmber 1: ");
    F;
    scanf("%d", &a);
    printf("Number 2: ");
    F;
    scanf("%d", &b);
    (function)(&res, a, b);
    printf("Res: %d\n", res);

    return 0;
}