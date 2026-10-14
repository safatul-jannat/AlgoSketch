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
    double t5;
    double t6;
    double t8;
    double t9;
    bool t7;
    double t10;

    t1 = 15;
    x = t1;
    t3 = x;
    t4 = 10;
    t2 = t3 > t4;
    if (!t2) goto L1;
    t5 = x;
    printf("%g\n", t5);
    goto L2;
L1:
    t6 = 0;
    printf("%g\n", t6);
L2:
    t8 = x;
    t9 = 100;
    t7 = t8 > t9;
    if (!t7) goto L4;
    t10 = 999;
    printf("%g\n", t10);
L4:

    return 0;
}
