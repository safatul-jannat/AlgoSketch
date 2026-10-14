#include <stdio.h>
#include <stdbool.h>
#include <string.h>

double Square(double x);

double Square(double x)
{
    double result;

    double t2;
    double t3;
    double t1;
    double t4;

    t2 = x;
    t3 = x;
    t1 = t2 * t3;
    result = t1;
    t4 = result;
    return t4;
}

int main(void)
{
    double answer;

    double t2;
    double t1;
    double t3;

    t2 = 6;
    t1 = Square(t2);
    answer = t1;
    t3 = answer;
    printf("%g\n", t3);

    return 0;
}
