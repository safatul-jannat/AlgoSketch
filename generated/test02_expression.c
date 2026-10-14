#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(void)
{
    double x;

    double t2;
    double t4;
    double t5;
    double t3;
    double t1;
    double t6;

    t2 = 2;
    t4 = 3;
    t5 = 4;
    t3 = t4 * t5;
    t1 = t2 + t3;
    x = t1;
    t6 = x;
    printf("%g\n", t6);

    return 0;
}
