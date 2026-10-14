#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(void)
{
    bool a;
    bool b;
    bool flag;
    bool isGreater;
    bool combined;

    bool t1;
    bool t2;
    bool t4;
    bool t5;
    bool t3;
    double t7;
    double t8;
    bool t6;
    bool t10;
    bool t11;
    bool t9;
    bool t12;
    bool t13;
    bool t14;
    bool t15;
    bool t16;

    t1 = true;
    a = t1;
    t2 = false;
    b = t2;
    t4 = a;
    if (!t4) goto L1;
    t5 = b;
    if (!t5) goto L1;
    t3 = true;
    goto L3;
L1:
    t3 = false;
L3:
    flag = t3;
    t7 = 5;
    t8 = 3;
    t6 = t7 > t8;
    isGreater = t6;
    t10 = a;
    if (t10) goto L5;
    t11 = b;
    if (t11) goto L5;
    t9 = false;
    goto L6;
L5:
    t9 = true;
L6:
    combined = t9;
    t12 = flag;
    printf("%s\n", t12 ? "true" : "false");
    t13 = a;
    printf("%s\n", t13 ? "true" : "false");
    t14 = b;
    printf("%s\n", t14 ? "true" : "false");
    t15 = isGreater;
    printf("%s\n", t15 ? "true" : "false");
    t16 = combined;
    printf("%s\n", t16 ? "true" : "false");

    return 0;
}
