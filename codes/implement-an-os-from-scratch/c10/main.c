#include "include/mm.h"
#include "include/print.h"
#include "include/sched.h"
#include "include/tss.h"
#include "include/interrupt.h"

int main() {
    mm_init();
    interrupt_init();

    // 设置标志寄存器中的IF位，开启中断
    __asm__ ("sti"); 
    sched_init();

    // 任务状态段初始化
    tss_init();

    // 使能时钟中断
    init_8254();    

    // 将寄存器CR3指向进程1的页表
    __asm__ ("mov %0, %%cr3": :"r"(current->pml4));

    // 设置栈指针指向进程1的内核栈的栈顶，并执行ret_from_kernel
    __asm__ ("movq %0, %%rsp\n\t"
        "jmp ret_from_kernel\n\t"
        :
        : "m"(current->rsp0)
        );
}