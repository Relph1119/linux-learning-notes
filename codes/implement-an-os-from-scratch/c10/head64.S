#include "include/segment.h"

.text
.code64
.globl gdt
.globl ret_from_kernel

    lgdt gdtr
    # 将中断描述符表地址写入IDTR寄存器中
    lidt idtr

    # 初始化寄存器
    mov $KERNEL_DS, %ax
    mov %ax, %ds
    mov %ax, %ss
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    # 使用RSP寄存器指向栈底
    mov $task0_stack, %rsp
    # 跳转到main方法
    push $main
    ret

# 所有段的TI为0，内核段的特权级为0，用户段的特权级为3
.align 64
gdt:
    # 空描述符（保留不用）
    .quad 0x0000000000000000
    # 内核代码段描述符
    .quad 0x00209a0000000000
    # 内核数据段描述符
    .quad 0x0000920000000000
    # 32位用户代码段描述符
    .quad 0x0000000000000000
    # 用户数据段描述符
    .quad 0x0000f20000000000
    # 64位用户代码段描述符
    .quad 0x0020fa0000000000
    .fill 128, 8, 0
gdt_end:

gdtr:
    .word gdt_end - gdt
    .quad gdt

idtr:
    # 中断描述符表长度，256项中断描述符，每个是16字节
    .word 16 * 256
    # 中断描述符表地址
    .quad idt_table

# 4KB大小的栈空间
.fill 4096, 1, 0
task0_stack:

# 在执行iret前，初始化其他段寄存器，之后再从栈中弹出断点信息，返回用户空间
ret_from_kernel:
    mov $USER_DS, %rax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    iretq