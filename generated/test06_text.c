#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(void)
{
    const char * message;
    double x;

    const char * t1;
    double t2;
    const char * t3;
    const char * t4;
    double t5;

    t1 = "Hello World";
    message = t1;
    t2 = 42;
    x = t2;
    t3 = "Direct string";
    printf("%s\n", t3);
    t4 = message;
    printf("%s\n", t4);
    t5 = x;
    printf("%g\n", t5);

    return 0;
}
