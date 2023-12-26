#include "include/types.h"
#include "include/mm.h"

// 分配页面
unsigned long alloc_page() {
    unsigned long addr = 0;

    // 从内存映像占用的物理页面之后的页面开始
    for (long i = KERNEL_PAGE_NUM; i < mem_size / PAGE_SIZE; i++) {
        // 直到找到空页面
        if (pages[i] == 0) {
            // 将新分配的页面标记为已占用
            pages[i] = 1;
            // 计算页面的物理地址
            addr = PAGE_SIZE * i;
            break;
        }
    }

    // 返回新分配页面的物理地址
    return addr;
}

// 释放页面
void free_page(uint64_t addr) {
    // 计算归还页面在数组page的索引
    uint32_t index = addr / PAGE_SIZE;

    // 标记页面为空闲
    pages[index] = 0;  
}