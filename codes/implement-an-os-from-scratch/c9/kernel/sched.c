#include "include/sched.h"
#include "include/mm.h"
#include "include/segment.h"
#include "include/string.h"

// 任务链的表头
static struct task* task_head;
// 任务链的表尾
static struct task* task_tail;
// 当前正在运行的任务
struct task* current;

// 伪造中断现场
static void fake_task_stack(unsigned long kstack) {
    uint16_t ss = USER_DS;
    // 将栈底设置在地址空间高处，128MB位置
    unsigned long rsp = 0x8000000;
    uint16_t cs = USER_CS;
    // 程序起始地址为0x100000
    unsigned long rip = 0x100000;
    unsigned long rsp_tmp;

    // kstack表示进程1的内核栈的栈底
    // 保护现场：将rsp存入临时变量中
    // 将kstack指向RSP寄存器，然后压栈
    // 恢复现场：将rsp_tmp恢复到RSP寄存器中
    __asm__ ("mov %%rsp, %5\n\t"
            "mov %4, %%rsp\n\t"
            "pushq %0\n\t"
            "pushq %1\n\t"
            "pushf\n\t"
            "pushq %2\n\t"
            "pushq %3\n\t"
            "mov %5, %%rsp\n\t"
            :
            : "m"(ss), "m"(rsp), "m"(cs), "m"(rip), "m"(kstack), "m"(rsp_tmp));
}


/* 创建任务
 * @param id 任务ID
 * @param entry 程序在虚拟地址空间的起始地址
 * @param entry_pa 程序在内存中的加载地址
*/  
static void make_task(unsigned long id, unsigned long entry, unsigned long entry_pa) {
    // 给task分配内存
    struct task* task = malloc(sizeof(struct task));
    task->id = id;

    // 分配一个页面存储当前任务的四级页表
    task->pml4 = alloc_page();
    // 将页面内容初始化为0
    memset(VA(task->pml4), 0, PAGE_SIZE);
    // 将四级页表的高256项复制到当前进程四级页表的高256项
    memcpy(VA(task->pml4 + 8 * 256), VA(TASK0_PML4 + 8 * 256), 8 * 256);
    // 将四级页表的低256项进行映射，设置页表项中的U/S位为1，允许用户进程访问映射的内存
    map_range(task->pml4, entry, entry_pa, 0x4, 1024);

    // 初始化内核栈
    task->kstack = (unsigned long)VA(alloc_page()) + PAGE_SIZE;

    fake_task_stack(task->kstack);
    // 跳回内核栈的初始地址
    task->rsp0 = task->kstack - 8 * 5;

    if (!task_head) {   
        // 第一个任务
        task_head = task;
        task_tail = task;
        task->prev = NULL;
        task->next = NULL;
    } else {
        task_tail->next = task;
        task->prev = task_tail;
        task->next = NULL;
        task_tail = task;
    }
}

// 调度任务初始化
void sched_init() {
    // 将app1加载在内存c800000处
    make_task(1, 0x100000, 0xc800000);

    current = task_head; 
}