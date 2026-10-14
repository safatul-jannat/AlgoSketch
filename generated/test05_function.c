#include <stdio.h>
#include <stdbool.h>
#include <string.h>

double Add(double a, double b);
double Multiply(double a, double b);

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

double Multiply(double a, double b)
{
    double t2;
    double t3;
    double t1;

    t2 = a;
    t3 = b;
    t1 = t2 * t3;
    return t1;
}

int main(void)
{
    double result;
    double product;

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
    result = t1;
    t5 = 3;
    t6 = 4;
    t4 = Multiply(t5, t6);
    product = t4;
    t7 = result;
    printf("%g\n", t7);
    t8 = product;
    printf("%g\n", t8);

    return 0;
}
