.text
.code64
# 无限循环，每次循环向串口输出一个字符
1:  
    mov $0x3f8, %dx
    mov $'A', %ax
    out %ax, %dx

    # 请求内核将应用挂起1000ms
    mov $1000, %rdi
    # 从寄存器RAX获取具体的系统调用号，0号为syscall_table中的do_sleep
    mov $0, %rax
    # 发起系统调用
    syscall

    jmp 1b