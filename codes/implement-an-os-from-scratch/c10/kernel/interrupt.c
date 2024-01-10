#include "include/types.h"
#include "include/interrupt.h"
#include "include/string.h"
#include "include/segment.h"

// 产生100Hz的中断，即每秒向8259A发出100次时钟信号，该值表示每秒的中断信号次数
#define COUNTER (1193181 / 100)
// 中断类型
#define GATE_INTERRUPT 0xe 
// 异常类型
#define GATE_EXCEPTION 0xf

// 中断描述符
struct gate_desc {
    uint16_t offset_low; // 中断处理函数地址0~15位
    uint16_t segment; // 段选择子
    uint16_t ist : 3, zero : 5, type : 4, zero2 : 1, dpl : 2, p : 1; // ist：中断栈索引；type：中断类型（中断1110/异常1111）；dpl：特权级；p：存在位
    uint16_t offset_middle; // 中断处理函数地址16~31位
    uint32_t offset_high;  // 中断处理函数地址32~63位
    uint32_t reserved; // 保留
} __attribute__((packed));

// 定义256项中断和异常，中断描述符表
struct gate_desc idt_table[256];

// 初始化8254计数器芯片
void init_8254() {
    // D7D6=00：SC选择计数器0
    // D5D4=11：RW设置读取计数值的方式，11表示首先写入低8位，然后写入高8位
    // D3D2D1=011：工作模式为3，使用方波
    // D0=0：编码格式为二进制格式
    // 将控制字写入0x43端口
    __asm__ ("outb %%al, $0x43"::"a"(0x36));
    // 分别将低8位和高8位写入0x40端口
    __asm__ ("outb %%al, $0x40"::"a"(COUNTER & 0xff));
    __asm__ ("outb %%al, $0x40"::"a"(COUNTER >> 8));  
}

// 设置中断描述符
// index：中断向量，对应中断描述符表中的索引号
// addr：中断处理函数地址
// type：描述符类型
static void set_gate(unsigned char index, unsigned long addr, char type) {
    struct gate_desc* desc = &idt_table[index];

    // 中断描述符初始化为0
    memset(desc, 0, sizeof(struct gate_desc));
    // 设置为内核代码段
    desc->segment = KERNEL_CS;
    // 设置中断处理函数地址
    desc->offset_low = (uint16_t)addr;
    desc->offset_middle = (uint16_t)(addr >> 16);
    desc->offset_high = (uint32_t)(addr >> 32);
    // 特权级为0，内核空间
    desc->dpl = 0;
    desc->type = type;
    // 存在位设置为1
    desc->p = 1;
}

// 中断系统初始化
void interrupt_init() {
    // 添加缺页异常处理函数
    set_gate(14, (unsigned long)&pf_handler, GATE_EXCEPTION);
    // 设置时钟中断描述符，时钟中断的向量号是32（0x20）
    set_gate(0x20, (unsigned long)&timer_handler, GATE_INTERRUPT);

}