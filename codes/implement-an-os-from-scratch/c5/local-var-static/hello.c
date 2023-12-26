#include<stdio.h>

void f() {
    // 静态局部变量
    static int count = 0;

    count++;
    printf("%d\n", count);
}

int main() {
    f();
    f();
}