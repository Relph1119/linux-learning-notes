#include "include/sched.h"
#include "include/mm.h"
#include "include/segment.h"
#include "include/string.h"
#include "include/print.h"
#include "include/tss.h"

// 任务链的表头
static struct task* task_head;
// 任务链的表尾
static struct task* task_tail;
// 当前正在运行的任务
struct task* current;
struct task* idle_task;

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
    task->state = TASK_RUNNING;

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
    task->rip = (unsigned long)&ret_from_kernel;

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

// 创建空闲任务
static void make_idle_task() {
    idle_task = malloc(sizeof(struct task));
    idle_task->id = 0;
    idle_task->state = TASK_RUNNING;
    idle_task->pml4 = TASK0_PML4;
    idle_task->kstack = (unsigned long)&task0_stack;
    idle_task->rsp0 = (unsigned long)&task0_stack;
    idle_task->rip = (unsigned long)&idle_task_entry;
}

// 调度任务初始化
void sched_init() {
    // 将app1加载在内存c800000处
    make_task(1, 0x100000, 0xc800000);
    // 将app1加载在内存d000000处
    make_task(2, 0x100000, 0xd000000);
    // 创建空闲任务
    make_idle_task();

    current = task_head; 
}

void schedule() {
    struct task* next = NULL;

    // 找到可运行的任务
    for (struct task* t = task_head; t; t = t->next) {
        if (t->state == TASK_RUNNING) {
            next = t;
            break;
        }
    }

    // 如果找不到，则设置为空闲任务
    if (!next) {
        next = idle_task;
    }

    // 执行任务切换
    if (next != current) {
        __asm__ ("mov %%rsp, %0\n\t" /* 保存当前进程的内核栈顶 */           \
                "movq $1f, %1\n\t"   /* 保存当前进程恢复执行的指令地址 */    \
                "mov %2, %%rsp\n\t"  /* 恢复下一个任务的内核栈顶 */         \
                "push %3\n\t"        /* 恢复下一个任务恢复执行的指令地址 */  \
                : "=m"(current->rsp0), "=m"(current->rip)                 \
                : "m"(next->rsp0), "m"(next->rip)                         \
        );

        // 任务状态段中内核栈的切换
        tss.rsp0 = (unsigned long)next->kstack;
        // 更新指向下一个运行的任务
        current = next;

        // 根页表的切换
        __asm__ ("mov %0, %%cr3" : : "a" (next->pml4));
        __asm__ ("ret");
        __asm__ ("1:");
    }
}

// 将当前任务从任务链表的当前位置摘除，链接到任务队列的末尾重新排队
void do_timer() {
    // 如果当前正在执行的任务不是空闲任务，当前任务的时间片就用完了
    if (current != idle_task) {
        // 不是队尾任务
        if (current != task_tail) {
            if (current->prev) {
                current->prev->next = current->next;
            }
            current->next->prev = current->prev;

            current->prev = task_tail;
            task_tail->next = current;

            if (current == task_head) {
                task_head = current->next;
            }
            task_tail = current;

            current->next = NULL;
        }
    }

    // 从任务队列头部开始，选择下一个可以执行的任务
    schedule();
}