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
    double x;
    double y;
    double sum;
    double i;
    bool done;

    double t1;
    double t2;
    double t4;
    double t5;
    double t3;
    double t6;
    bool t7;
    double t9;
    double t10;
    bool t8;
    const char * t11;
    const char * t12;
    double t14;
    double t15;
    bool t13;
    double t17;
    double t18;
    double t16;
    double t19;
    bool t21;
    bool t20;
    const char * t22;
    const char * t23;
    double t24;

    t1 = 10;
    x = t1;
    t2 = 20;
    y = t2;
    t4 = x;
    t5 = y;
    t3 = Add(t4, t5);
    sum = t3;
    t6 = 0;
    i = t6;
    t7 = false;
    done = t7;
    t9 = sum;
    t10 = 25;
    t8 = t9 > t10;
    if (!t8) goto L1;
    t11 = "Sum is large";
    printf("%s\n", t11);
    goto L2;
L1:
    t12 = "Sum is small";
    printf("%s\n", t12);
L2:
L3:
    t14 = i;
    t15 = 3;
    t13 = t14 < t15;
    if (!t13) goto L4;
    t17 = i;
    t18 = 1;
    t16 = t17 + t18;
    i = t16;
    goto L3;
L4:
    t19 = i;
    printf("%g\n", t19);
    t21 = done;
    t20 = !t21;
    if (!t20) goto L6;
    t22 = "Not done yet";
    printf("%s\n", t22);
L6:
    t23 = "The sum is:";
    printf("%s\n", t23);
    t24 = sum;
    printf("%g\n", t24);

    return 0;
}
