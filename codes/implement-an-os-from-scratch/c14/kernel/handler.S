# 各种中断处理函数

.text
.code64
.globl timer_handler
.globl pf_handler
.globl kb_handler
.globl system_call

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

system_call:
    # 将当前寄存器RSP中的用户栈顶保存到任务状态段的rsp2，rsp2的基址偏移为20字节
    mov %rsp, tss + 20
    # 将任务状态段的rsp0保存的进程内核栈加载到寄存器RSP中，rsp0的基址偏移为4字节
    mov tss + 4, %rsp

    # 将任务状态段的rsp2的用户栈压入内核栈
    pushq tss + 20
    # 无需保存RAX寄存器
    SAVE_CONTEXT 0

    # 由于位于同一个特权级，使用call调用具体的系统调用
    # 间接跳转
    call *syscall_table(, %rax, 8)

    RESTORE_CONTEXT 0
    # 从内核栈弹出用户栈顶指针RSP，恢复用户栈
    pop %rsp

    sysretq

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

kb_handler:
    SAVE_CONTEXT
    
    # 调用键盘中断处理函数
    call process_kb

    # 将OCW2写入8259A的端口0x20处，D5=1表示处理器已经处理完中断了
    movb $0x20, %al
    outb %al, $0x20

    RESTORE_CONTEXT
    iretq