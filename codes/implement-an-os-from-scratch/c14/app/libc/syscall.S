#define SYSCALL_SLEEP       0
#define SYSCALL_SHM         1
#define SYSCALL_FBMAP       2
#define SYSCALL_GETMODEINFO 3

.globl sleep
.globl shm_open
.globl fbmap
.globl get_mode_info

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

fbmap:
    # 从寄存器RAX获取具体的系统调用号，2号为syscall_table中的do_fbmap
    mov $SYSCALL_FBMAP, %rax
    syscall
    ret

get_mode_info:
     # 从寄存器RAX获取具体的系统调用号，3号为syscall_table中的do_get_mode_info
    mov $SYSCALL_GETMODEINFO, %rax
    syscall
    ret