#include<stdio.h>
#include"myadd.h"
#include"mysub.h"

int main() {
    int a = 3;
    int b = 4;
    printf("%d+%d=%d %d-%d=%d\n", a, b, myadd(a, b), a, b, mysub(a, b));
    return 0;
}
