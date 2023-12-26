#include "app/libc/std.h"
#include "include/print.h"
#include "app/libdraw/draw.h"

int main() {
    void* m = shm_open("shm-1");
    *(char*)m = 'S';

    struct mode_info mode_info;
    get_mode_info(&mode_info);

    unsigned long fbbase = fbmap();

    // 每次间隔1秒，变换矩形的颜色
    while (1) {
        draw_rect(10, 100, 150, 100, RED, fbbase, &mode_info);
        sleep(1000);
        draw_rect(10, 100, 150, 100, GREEN, fbbase, &mode_info);
        sleep(1000);
        draw_rect(10, 100, 150, 100, BLUE, fbbase, &mode_info);
        sleep(1000);
    }
}