#include "app/libc/std.h"
#include "include/print.h"

int main() {
    while (1) {
        print('A');
        sleep(1000);
    }
}