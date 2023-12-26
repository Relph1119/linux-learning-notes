#include "app/libc/std.h"
#include "include/print.h"

int main() {
    void* m = shm_open("shm-1");
  *(char*)m = 'S';

    while (1) {
        print('A');
        sleep(1000);
    }
}