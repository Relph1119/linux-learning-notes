# 扩展阅读：什么是中断

整个操作系统就是一个中断驱动的死循环，其他所有事情都是由操作系统提前注册的中断机制和其对应的中断处理函数完成的。

## 1 中断和异常

CPU提供了两种中断程序执行的机制，分为中断和异常：
- 中断（interrupt）：是一个异步事件，通常由IO设备触发。
- 异常（exception）：是一个同步事件，是CPU在执行指令时检测到的反常条件。

中断的发出过程：可编程中断控制器有很多`IRQ`引脚，这些引脚接入了一堆能发出中断请求的硬件设备，当硬件设备给引脚发送一个信号时，通过中断器转换为中断号并存储到一个端口上，CPU通过接收到`INTR`引脚的信号，读取端口的中断号。

给CPU一个中断号的3种方式：
1. 通过中断控制器给CPU的`INTR`引脚发送信号。
2. CPU执行某条指令发现了异常。
3. 执行`INT n`指令。

## 2 CPU收到中断号之后的事

CPU收到中断号之后的处理过程：
1. CPU收到一个中断号`n`后，会去中断描述符表中寻找第n个中断描述符。
2. 从中断描述符中找到中断处理程序的地址：从中断描述符中找到程序的段选择子和段内偏移地址，段选择子从全局描述符表（GDT）中寻找段描述符取出段基址，段基址+段内偏移地址=处理程序的入口地址。
3. 然后跳转到处理程序的入口地址，执行处理程序。

## 3 中断描述符和中断描述符表

- 中断描述符表（IDT）：本质是一个在内存中的数组。

```c
struct desc_struct idt_table[256] = {{0, 0}};
```

- 中断描述符：是中断描述符表这个数组里存储的数据结构。

```c
struct desc_struct {
    unsigned long a, b
};
```

中断描述符的类别：
- 任务门描述符（Task Gate）
- 中断门描述符（Interrupt Gate）：当CPU收到中断号，修改`IF`标志位，防止中断嵌套。
- 陷阱门描述符（Trap Gate）：允许中断嵌套。

## 4 相关问题

### 4.1 CPU如何找到中断描述符表？

在IDTR寄存器中，前16位表示中断描述符表大小（字节数），后32位表示中断描述符表的起始内存地址。

```nasm
idt_descr:
    .word 256 * 8 - 1
    .long idt_table
    
lidt idt_descr  ! move to IDTR
```

### 4.2 谁把这个中断描述符表写在内存中的？

操作系统将各种处理函数都写入了中断描述符表中，代码位置：`kernel/traps.c`

```c
void trap_init(void)
{
	int i;
	set_trap_gate(0,&divide_error);
	set_trap_gate(1,&debug);
	set_trap_gate(2,&nmi);
	set_system_gate(3,&int3);	/* int3-5 can be called from all */
	set_system_gate(4,&overflow);
	set_system_gate(5,&bounds);
	set_trap_gate(6,&invalid_op);
	set_trap_gate(7,&device_not_available);
	set_trap_gate(8,&double_fault);
	set_trap_gate(9,&coprocessor_segment_overrun);
	set_trap_gate(10,&invalid_TSS);
	set_trap_gate(11,&segment_not_present);
	set_trap_gate(12,&stack_segment);
	set_trap_gate(13,&general_protection);
	set_trap_gate(14,&page_fault);
	set_trap_gate(15,&reserved);
	set_trap_gate(16,&coprocessor_error);
	for (i=17;i<48;i++)
		set_trap_gate(i,&reserved);
	set_trap_gate(45,&irq13);
	outb_p(inb_p(0x21)&0xfb,0x21);
	outb(inb_p(0xA1)&0xdf,0xA1);
	set_trap_gate(39,&parallel_interrupt);
}
```

### 4.3 找到中断描述符之后的事

CPU在收到一个中断号之后，直接把中断描述符里的中断程序地址取出来，放到`CS:IP`寄存器中，该寄存器存放着下一条指令的地址，到了下一个CPU指令周期，CPU就会去执行中断处理程序。

具体操作：
1. 如果发生了特权级转移，压入之前的栈段寄存器SS及栈顶指针ESP被保存到栈中，并将栈切换为TSS中的栈。
2. 压入标志寄存器`EFLAGS`。
3. 压入之前的代码段寄存器CS和指令寄存器ESP，相当于压入返回地址。
4. 如果中断有错误码，则压入错误码`ERROR_CODE`。
5. 结束（之后跳转到中断程序）。

## 5 总结

- 中断的来源：外设通过INTR引脚发送信号，或CPU在执行指令过程中自身触发中断，或由软件通过`INT n`指令触发。
- 中断程序的入口地址：通过`IDTR`寄存器找到中断描述符表，通过中断描述符表和中断号定位到中断描述符，取出程序入口的地址。
- 中断描述符表的写入：操作系统代码写到内存中。
- 找到程序入口地址之后：压栈（保护现场和传递错误码），并跳转到入口地址处执行代码。
