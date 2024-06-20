#include <stdio.h>

extern "C" {
void printInt(int a) {
    printf("%d\n", a);
}

void printDouble(double a) {
    printf("%lf\n", a);
}
}