// 内核代码段选择子
#define KERNEL_CS 0x8
// 内核数据段选择子
#define KERNEL_DS 0x10
// 32位用户代码段选择子
#define USER32_CS 0x1b
// 用户数据段选择子
#define USER_DS 0x23
// 64位用户代码段选择子
#define USER_CS 0x2b

// 段描述符表的第6项作为任务状态段的段描述符
#define GDT_TSS_ENTRY 6