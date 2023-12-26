#define SYSCALL_SLEEP       0

.globl sleep

sleep:
    # 从寄存器RAX获取具体的系统调用号，0号为syscall_table中的do_sleep
    mov $SYSCALL_SLEEP, %rax
    # 发起系统调用
    syscall
    ret