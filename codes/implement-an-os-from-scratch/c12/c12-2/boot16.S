#define IO_PIC     0x20
#define IRQ_OFFSET 32

.text
.code16
start16:
    # 清除标志寄存器中的中断位
    cli

    # BIOS使用一个称为E820的数据结构实例表示内存地址的每一个段
    # 中断0x15的处理函数从寄存器DI中读取这个地址
    mov $e820_entry, %di
    xor %ebx, %ebx
e820_rd_entry:
    # 设置EAX寄存器的功能号
    mov $0xe820, %eax
    # 告知0x15中断处理函数，复制每条E820记录的前20个字节（第1个8字节表示内存段起始地址，第2个8字节表示段的尺寸，剩下4字节表示段的类型）
    mov $20, %ecx
    # 发起中断，执行BIOS中0x15中断处理函数
    int $0x15

    # 指向下一个E820记录的内存地址
    add $20, %di
    # 记录读取的E820的条数
    incb e820_nr_entry

    # 如果全部读取完毕，0x15中断处理函数会将寄存器EBX设置为0
    cmp $0, %ebx
    jne e820_rd_entry
 
    # 初始化8259A
    # 初始化ICW1命令字
    # D0=0：关闭IC4关联的特性
    # D1=0：单片模式
    # D3=0：KVM的虚拟8295A不支持电平触发，设置为边沿触发
    mov $0x13, %al
    # 主8259A的端口地址是0x20
    mov $(IO_PIC), %dx
    out %al,%dx
    # 初始化ICW2命令字
    # 外设的中断向量从32开始分配
    mov $(IRQ_OFFSET), %al
    mov $(IO_PIC+1), %dx
    out %al, %dx
    # 初始化ICW4命令字
    # D0=1：模式是8086及以上系统
    # D1=0：关闭自动复位ISR寄存器，由中断处理函数负责向中断芯片发送EOI（中断结束）命令
    # D2=0，D3=0：KVM忽略了缓冲模式和主从位
    # D4=0：固定优先级策略
    mov $0x1, %al
    mov $(IO_PIC+1), %dx
    out %al, %dx

    # 加载段描述符表地址到寄存器GDTR
    lgdt gdtr

    # 开启处理器的保护模式，CR0寄存器的第0位PE用于控制处理器是否开启保护模式
    mov %cr0, %eax
    # 将CR0的最后一位置为1，即开启保护模式
    or $0x1, %eax
    mov %eax, %cr0

    # 段选择子是0x8（段索引是1，使用全局段描述符表TI是0，特权级是00，即0000000000001000）
    # 保护模式的入口地址是0x20000，段基址为0，所以段内偏移地址为0x20000
    # 长跳转指令ljmpl [段选择子] [段内偏移地址]
    ljmpl $0x8, $0x20000

gdt:
    # 段描述符表的第0项保留不用
    .quad 0x0000000000000000
    # 第1项定义内核代码段
    .quad 0x00c09a00000007ff
    # 第2项定义内核数据段
    .quad 0x00c09200000007ff
gdt_end:

gdtr:
    # 低16位对应段描述符表的长度
    .word gdt_end - gdt
    # 高16位对应段描述符表的地址（0x1000<<4 + gdt）
    .word gdt, 0x1

.org 0x3000
e820_nr_entry:
    .long 0
e820_entry:
    .fill 1024, 1, 0

