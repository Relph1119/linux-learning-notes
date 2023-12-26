#include "include/mm.h"
#include "include/print.h"

int main() {
    mm_init();

    unsigned long *x = VA(200 * 1024 * 1024);

    *x = 5;
    print(*x);

    __asm__("hlt");
}