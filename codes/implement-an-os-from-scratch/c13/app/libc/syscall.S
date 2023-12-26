#define SYSCALL_SLEEP       0
#define SYSCALL_SHM         1
.globl sleep
.globl shm_open

sleep:
    # 从寄存器RAX获取具体的系统调用号，0号为syscall_table中的do_sleep
    mov $SYSCALL_SLEEP, %rax
    # 发起系统调用
    syscall
    ret

shm_open:
    # 从寄存器RAX获取具体的系统调用号，1号为syscall_table中的do_shm
    mov $SYSCALL_SHM, %rax
    syscall
    ret