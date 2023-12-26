#include "app/libc/std.h"
#include "include/print.h"

int main() {
    void* m = shm_open("shm-1");

    while (1) {
        print(*(char*)m);
        sleep(1000);
    }
}