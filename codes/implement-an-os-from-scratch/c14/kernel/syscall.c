#include "include/types.h"
#include "include/segment.h"
#include "include/syscall.h"
#include "include/sched.h"
#include "include/vesa.h"

#define MSR_STAR 0xc0000081
#define MSR_LSTAR 0xc0000082
#define MSR_SYSCALL_MASK 0xc0000084
#define RF_IF 0x00000200

typedef unsigned long (*fn_ptr)();

unsigned long do_sleep(long ms);
unsigned long do_shm(const char* name);
unsigned long do_fbmap();
unsigned long do_get_mode_info(struct mode_info *mode_info);

fn_ptr syscall_table[] = { do_sleep, do_shm, do_fbmap, do_get_mode_info };

void syscall_init() {
    // 设置用户代码段和内核代码段
    uint64_t star_val = (uint64_t)USER32_CS << 48  | (uint64_t)KERNEL_CS << 32;
    uint64_t syscall_entry = (uint64_t)system_call;
    // 初始化标志寄存器的第9位（中断使能位）
    uint64_t syscall_mask = RF_IF;

    // 将值写入STAR寄存器
    // 指令wrmsr从寄存器ECX读取MSR的ID，分别将寄存器EAX和EDX的值写入MSR寄存器的低32位和高32位
    // 编译器将各个值预先装载到ECX、EAX、EDX中
    __asm__("wrmsr": : "c" (MSR_STAR), "a" ((uint32_t)star_val), "d" (star_val >> 32));
    // 将值写入LSTAR寄存器
    __asm__("wrmsr": : "c" (MSR_LSTAR), "a" ((uint32_t)syscall_entry), "d" (syscall_entry >> 32));
    // 将值写入SFMASK寄存器
    __asm__("wrmsr": : "c" (MSR_SYSCALL_MASK), "a" ((uint32_t)syscall_mask), "d" (syscall_mask >> 32));
}