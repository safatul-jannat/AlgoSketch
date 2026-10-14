#include <stdio.h>
#include <stdbool.h>
#include <string.h>

double Add(double a, double b);

double Add(double a, double b)
{
    double t2;
    double t3;
    double t1;

    t2 = a;
    t3 = b;
    t1 = t2 + t3;
    return t1;
}

int main(void)
{
    double sum;
    double big;

    double t2;
    double t3;
    double t1;
    double t5;
    double t6;
    double t4;
    double t7;
    double t8;

    t2 = 10;
    t3 = 20;
    t1 = Add(t2, t3);
    sum = t1;
    t5 = sum;
    t6 = 5;
    t4 = Add(t5, t6);
    big = t4;
    t7 = sum;
    printf("%g\n", t7);
    t8 = big;
    printf("%g\n", t8);

    return 0;
}
