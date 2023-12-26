#include<stdio.h>

int main() {
    int x = 2;
    int y = 3;
    int sum = 0;

    // 内联汇编
    asm ( "mov %1, %%eax\n\t"
        "mov %[addend], %%ebx\n\t"
        "add %%ebx, %%eax\n\t"
        "mov %%eax, %0"
        : "=m"(sum)
        : "m"(x), [addend]"m"(y)
        : "eax", "ebx", "r11", "r12"
    );
}