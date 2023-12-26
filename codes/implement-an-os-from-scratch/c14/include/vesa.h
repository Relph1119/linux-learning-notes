#pragma once

#include "include/types.h"

struct vesa_mode_info {
    uint8_t pad0[18]; 
    uint16_t hres;     // 以像素为单位的水平分辨率
    uint16_t vres;     // 以像素为单位的垂直分辨率

    uint8_t pad1[3];
    uint8_t bpp;       // 每个像素使用的字节数
    uint8_t pad2[14];

    uint32_t fbbase;   // framebuffer占据的地址空间的起始地址

    uint8_t pad3[212];
} __attribute__ ((packed));

// 精简的结构体，用于应用程序获取模式信息
struct mode_info {
    uint32_t fbbase;
    uint16_t hres;
    uint16_t vres;
    uint8_t bpp;
};

void vesa_init();