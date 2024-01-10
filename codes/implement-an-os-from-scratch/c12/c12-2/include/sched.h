#pragma once

#include "include/types.h"

// 进程状态
enum task_state {
    // 任务正在运行或就绪态
    TASK_RUNNING = 0,
    // 任务处于可中断的睡眠态
    TASK_INTERRUPTIBLE
};

// 记录进程的信息和状态
struct task {
    unsigned long id; 
    enum task_state state; // 任务状态
    unsigned long rip;     // 任务调度切换时，指令的地址
    unsigned long rsp0;    // 任务调度切换时，内核栈的栈顶
    unsigned long kstack;  // 内核栈的栈底，用于在内核时，任务状态段中特权级为0的栈指针指向当前任务的内核栈栈底
    unsigned long pml4;    // 根页表的物理地址，用于更新寄存器CR3指向当前任务的页表

    // 支撑任务链的链表
    struct task* next;     
    struct task* prev;
};

struct timer {
    unsigned long alarm;   // 记录时钟到期时间
    struct task* task;     
    struct timer* next;
    struct timer* prev;
};

extern unsigned long ret_from_kernel;
extern unsigned long idle_task_entry;
extern unsigned long task0_stack;
extern struct task* current;

void sched_init();