#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(void)
{
    double x;

    double t1;
    double t3;
    double t4;
    bool t2;
    double t6;
    double t7;
    double t5;
    double t8;

    t1 = 0;
    x = t1;
L1:
    t3 = x;
    t4 = 5;
    t2 = t3 < t4;
    if (!t2) goto L2;
    t6 = x;
    t7 = 1;
    t5 = t6 + t7;
    x = t5;
    goto L1;
L2:
    t8 = x;
    printf("%g\n", t8);

    return 0;
}
