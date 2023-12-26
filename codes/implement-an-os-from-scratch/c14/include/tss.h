#pragma once

#include "include/types.h"

// 可以寻址64K（65536）个端口
#define IO_BITMAP_BYTES (65536 / 8)

struct tss {
    uint32_t reserved1;                       // 保留
    uint64_t rsp0;                            // 特权级为0的栈指针
    uint64_t rsp1;                            // 特权级为1的栈指针
    uint64_t rsp2;                            // 特权级为2的栈指针
    uint64_t reserved2;                       // 保留
    uint64_t ist[7];                          // IST的7个专用栈
    uint32_t reserved3;                       // 保留
    uint32_t reserved4;                       // 保留
    uint16_t reserved5;                       // 保留
    uint16_t io_bitmap_offset;                // 程序I/O权限位图相对于任务状态段基址的16位偏移
    uint8_t io_bitmap[IO_BITMAP_BYTES + 1];   // I/O权限位图
} __attribute__((packed));

struct tss_desc {
    uint16_t limit0;    // 段长度
    uint16_t base0;     // 段基址（0~15）
    uint16_t base1 : 8, type : 4, desc_type : 1, dpl : 2, p : 1;
    uint16_t limit1 : 4, avl : 1, zero0 : 2, g : 1, base2 : 8;
    uint32_t base3;     // 段基址（32~63）
    uint32_t zero1;     // 保留
} __attribute__((packed));

extern struct tss tss;
// TSS任务状态段初始化
void tss_init();