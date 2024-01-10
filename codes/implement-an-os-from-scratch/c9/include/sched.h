#pragma once

#include "include/types.h"

// 记录进程的信息和状态
struct task {
    unsigned long id; 
    unsigned long rip;     // 任务调度切换时，指令的地址
    unsigned long rsp0;    // 任务调度切换时，内核栈的栈顶
    unsigned long kstack;  // 内核栈的栈底，用于在内核时，任务状态段中特权级为0的栈指针指向当前任务的内核栈栈底
    unsigned long pml4;    // 根页表的物理地址，用于更新寄存器CR3指向当前任务的页表

    // 支撑任务链的链表
    struct task* next;     
    struct task* prev;
};

extern struct task* current;
void sched_init();
