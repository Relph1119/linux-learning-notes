#include "include/vesa.h"
#include "app/libdraw/draw.h"

extern const char fonts_zh[2][32];
extern const char fonts_en[6][16];

// 绘制英文字符
void draw_en(int origin_x, int origin_y, int index, int color, unsigned long fbbase, struct mode_info* mode_info) {
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 1; x++) {
            int pos = y * 1 + x;

            char byte = fonts_en[index][pos];
            int n = 0;
            while (n++ < 8) {
                char bit = (byte >> (8 - n)) & 0x1;
                if (bit == 1) {
                    *((uint32_t*)fbbase + (origin_y + y) * mode_info->hres + (origin_x + x * 8 + (n - 1))) = color;
                }
            }
        }
    }
}

// 绘制中文字符
void draw_zh(int origin_x, int origin_y, int index, int color, unsigned long fbbase, struct mode_info* mode_info) {
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 2; x++) {
            int pos = y * 2 + x;

            char byte = fonts_zh[index][pos];
            int n = 0;
            while (n++ < 8) {
                char bit = (byte >> (8 - n)) & 0x1;
                if (bit == 1) {
                    *((uint32_t*)fbbase + (origin_y + y) * mode_info->hres + (origin_x + x * 8 + (n - 1))) = color;
                }
            }
        }
    }
}

// 绘制矩形，每条线段的像素用前景色绘制
void draw_rect(int origin_x, int origin_y, int l, int w, int color, unsigned long fbbase, 
               struct mode_info* mode_info) {
    // 绘制上下两条边            
    for (int i = origin_x; i < origin_x + l; i++) {
        *((uint32_t*)fbbase + origin_y * mode_info->hres + i) = color;
        *((uint32_t*)fbbase + (origin_y + w) * mode_info->hres + i) = color;
    }

    // 绘制左右两条边
    for (int i = origin_y; i < origin_y + w; i++) {
        *((uint32_t*)fbbase + i * mode_info->hres + origin_x) = color;
        *((uint32_t*)fbbase + i * mode_info->hres + origin_x + l) = color;
    }
}

// 清空屏幕
void clear(unsigned long fbbase, struct mode_info* mode_info) {
    for (int y = 0; y < mode_info->vres; y++) {
        for (int x = 0; x < mode_info->hres; x++) {
            *((uint32_t*)fbbase + y * mode_info->hres + x) = 0;
        }
    }
}