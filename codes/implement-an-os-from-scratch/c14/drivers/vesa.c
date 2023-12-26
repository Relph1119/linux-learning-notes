#include "include/vesa.h"
#include "include/string.h"
#include "include/mm.h"
#include "include/sched.h"

struct vesa_mode_info* vesa_mode_info;

void vesa_init() {
    // 分配内存
    vesa_mode_info = malloc(sizeof(struct vesa_mode_info));
    // 由于kvmtool加载在内核0x10000处，实模式中读取的模式信息偏移为0x4000处，故从0x14000处复制模式信息
    memcpy(vesa_mode_info, (char*)0x14000, sizeof(struct vesa_mode_info));
} 

// 为应用传递模式信息
unsigned long do_get_mode_info(struct mode_info *mode_info) {
    mode_info->fbbase = vesa_mode_info->fbbase;
    mode_info->hres = vesa_mode_info->hres;
    mode_info->vres = vesa_mode_info->vres;
    mode_info->bpp = vesa_mode_info->bpp;  

    return 0;
}

// 将framebuffer映射到用户空间
unsigned long do_fbmap() {
    // 固定一个地址作为framebuffer映射在进程的用户空间的起始地址
    unsigned long va = 0xe000000;
    // 物理地址
    unsigned long pa = vesa_mode_info->fbbase;
    // 计算映射长度
    int size = vesa_mode_info->hres * vesa_mode_info->vres * vesa_mode_info->bpp / 8;
    int npage = 0;
    // 进行地址映射，US位设置为1，表示允许运行在特权级为3的用户访问这块内存
    map_range(current->pml4, va, pa, 0x4, (size + PAGE_SIZE - 1) / PAGE_SIZE);

    return va;
}
