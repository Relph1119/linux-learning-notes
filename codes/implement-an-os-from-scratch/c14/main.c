#include "include/mm.h"
#include "include/print.h"
#include "include/sched.h"
#include "include/tss.h"
#include "include/interrupt.h"
#include "include/syscall.h"
#include "include/vesa.h"

int main() {
    // 内存初始化
    mm_init();
    // 中断初始化
    interrupt_init();
    // 系统调用初始化
    syscall_init(); 
    // 初始化VESA显示驱动
    vesa_init();

    // 设置标志寄存器中的IF位，开启中断    
    __asm__ ("sti"); 
    sched_init();

    // 任务状态段初始化
    tss_init();
    
    // 将寄存器CR3指向进程1的页表
    __asm__ ("mov %0, %%cr3": :"r"(current->pml4));

    // 使能时钟中断
    init_8254();    

    // 设置栈指针指向进程1的内核栈的栈顶，并执行ret_from_kernel
    __asm__ ("movq %0, %%rsp\n\t"
        "jmp ret_from_kernel\n\t"
        :
        : "m"(current->rsp0)
        );
}