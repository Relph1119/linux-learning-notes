#pragma once

#include "include/types.h"

#define E820MAX 32
#define E820_RAM 1

// 定义最大页面数，每个页面大小为4K，一共表示4GB物理内存
#define MAX_PAGES (1024 * 1024)
// 定义内核所需的页面，16K个页面
#define KERNEL_PAGE_NUM (1024 * 16)

#define PAGE_SIZE 4096

#define PAGE_OFFSET 0xffff888000000000
#define VA(x) ((void*)((unsigned long)(x) + PAGE_OFFSET))
#define PA(x) ((unsigned long)(x) - PAGE_OFFSET)

#define TASK0_PML4 0x30000

// 内存大小
extern unsigned long mem_size;

extern uint8_t pages[MAX_PAGES];

// 使用packed告知GCC编译器，分配结构体变量时不要进行对齐
struct e820entry {
    uint64_t addr; // 内存段的起始地址
    uint64_t size; // 内存段的尺寸
    uint32_t type; // 内存段的类型
} __attribute__((packed));

// 存储E820信息的内存区域
struct e820map {
    uint32_t nr_entry;
    struct e820entry map[E820MAX]; // 多条E820记录
};

// 内存管理初始化
void mm_init();
// 分配页面
unsigned long alloc_page();
// 释放页面
void free_page(uint64_t addr);