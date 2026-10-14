#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(void)
{
    const char * a;
    const char * b;
    const char * c;

    const char * t1;
    const char * t2;
    const char * t3;
    const char * t5;
    const char * t6;
    bool t4;
    const char * t7;
    const char * t8;
    const char * t10;
    const char * t11;
    bool t9;
    const char * t12;
    const char * t13;

    t1 = "hello";
    a = t1;
    t2 = "hello";
    b = t2;
    t3 = "world";
    c = t3;
    t5 = a;
    t6 = b;
    t4 = strcmp(t5, t6) == 0;
    if (!t4) goto L1;
    t7 = "equal";
    printf("%s\n", t7);
    goto L2;
L1:
    t8 = "not equal";
    printf("%s\n", t8);
L2:
    t10 = a;
    t11 = c;
    t9 = strcmp(t10, t11) == 0;
    if (!t9) goto L3;
    t12 = "same";
    printf("%s\n", t12);
    goto L4;
L3:
    t13 = "different";
    printf("%s\n", t13);
L4:

    return 0;
}
