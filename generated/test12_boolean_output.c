#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(void)
{
    bool t;
    bool f;
    bool same;

    bool t1;
    bool t2;
    bool t4;
    bool t5;
    bool t3;
    bool t6;
    bool t7;

    t1 = true;
    t = t1;
    t2 = false;
    f = t2;
    t4 = t;
    t5 = f;
    t3 = t4 == t5;
    same = t3;
    t6 = t;
    printf("%s\n", t6 ? "true" : "false");
    t7 = same;
    printf("%s\n", t7 ? "true" : "false");

    return 0;
}
