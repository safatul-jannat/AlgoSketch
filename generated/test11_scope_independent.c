#include <stdio.h>
#include <stdbool.h>
#include <string.h>

double Make(double c);

double Make(double c)
{
    double n;

    double t2;
    double t3;
    double t1;
    double t4;

    t2 = c;
    t3 = 2;
    t1 = t2 * t3;
    n = t1;
    t4 = n;
    return t4;
}

int main(void)
{
    double a;
    double b;

    double t2;
    double t1;
    double t4;
    double t3;
    double t5;
    double t6;

    t2 = 3;
    t1 = Make(t2);
    a = t1;
    t4 = 5;
    t3 = Make(t4);
    b = t3;
    t5 = a;
    printf("%g\n", t5);
    t6 = b;
    printf("%g\n", t6);

    return 0;
}
