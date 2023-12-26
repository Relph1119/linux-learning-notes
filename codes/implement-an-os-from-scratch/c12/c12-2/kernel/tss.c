#include "include/tss.h"
#include "include/sched.h"
#include "include/string.h"
#include "include/segment.h"

extern unsigned long gdt[64];
struct tss tss;

void tss_init() {
    // 允许应用程序访问所有I/O端口
    memset(&tss, 0, sizeof(tss));
    tss.io_bitmap_offset = __builtin_offsetof(struct tss, io_bitmap);
    tss.io_bitmap[IO_BITMAP_BYTES] = ~0;
    // 设置任务状态段的RSP0指向当前应用程序的内核栈地址
    tss.rsp0 = current->kstack;    

    // 得到GDT表的第6项
    struct tss_desc* desc = (struct tss_desc*)&gdt[GDT_TSS_ENTRY];
    // 将任务状态段的段描述符清0
    memset(desc, 0, sizeof(struct tss_desc));
    // 计算tss结构体的长度的低16位
    desc->limit0 = sizeof(tss) & 0xffff;
    desc->base0 = (unsigned long)(&tss) & 0xffff;
    desc->base1 = ((unsigned long)(&tss) >> 16) & 0xff;
    // 段类型为0x9
    desc->type = 0x9;
    // 存在位为1
    desc->p = 1;
    // 段长度的第16~19位
    desc->limit1 = (sizeof(tss) >> 16) & 0xf;
    desc->base2 = ((unsigned long)(&tss) >> 24) & 0xff;
    desc->base3 = (unsigned long)(&tss) >> 32;

    // 禁止应用程序对所有端口的访问，会直接终止执行
    // memset(tss.io_bitmap, 0xff, IO_BITMAP_BYTES);

    // 装载任务寄存器TR，段索引为6，TI为0，特权级（内核级）为0
    __asm__ ("ltr %w0" : : "r"(GDT_TSS_ENTRY << 3));
}