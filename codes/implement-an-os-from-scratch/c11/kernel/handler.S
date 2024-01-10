# 各种中断处理函数

.text
.code64
.globl timer_handler
.globl pf_handler

# 为进程保存完整环境，调用者负责的寄存器：RDI、RSI、RDX、RCX、RAX、R8~R11
.macro SAVE_CONTEXT save_rax = 1
    pushq %rdi
    pushq %rsi
    pushq %rdx
    pushq %rcx
    .if \save_rax
    pushq %rax
    .endif
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
.endm

.macro RESTORE_CONTEXT rstore_rax = 1
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    .if \rstore_rax
    popq %rax
    .endif
    popq %rcx
    popq %rdx
    popq %rsi
    popq %rdi
.endm

timer_handler:  
    # 保护上下文  
    SAVE_CONTEXT

    # 将OCW2写入8259A的端口0x20处，D5=1表示处理器已经处理完中断了
    movb $0x20,%al
    outb %al,$0x20

    # 调用do_timer函数
    call do_timer

    # 恢复上下文
    RESTORE_CONTEXT
    # IF位自动复位，再次开启中断
    iretq

pf_handler:
    SAVE_CONTEXT

    # 发生缺页异常时，处理器会将缺页地址存储到寄存器CR2中
    # 需要从寄存器CR2中取出引起缺页的地址
    mov %cr2, %rdi
    call do_page_fault

    RESTORE_CONTEXT
    # 越过错误码，避免弹出到指令指针寄存器RIP中
    add $8, %rsp
    iretq
