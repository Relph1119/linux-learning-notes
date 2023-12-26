#include "include/string.h"
#include "include/mm.h"
#include "include/sched.h"

// 共享内存块
struct shm {
    char* name;           // 共享内存块的名字，用于区分不同的共享内存
    unsigned long page;   // 指向一个用于共享内存的物理页面
        
    struct shm* next;     // 指向下一个共享内存块，用于链接共享内存
};

struct shm* shm_head;
struct shm* shm_tail;

// 实现共享内存系统调用
// name：应用传递进来的共享内存名字
int do_shm(char* name) {
    struct shm* shm = NULL;
    // 使用一个固定地址作为共享内存映射的虚拟地址
    unsigned long va = 0x4000000;

    // 遍历并寻找名字与应用请求相同的共享内存
    for (struct shm* s = shm_head; s; s = s->next) {
        if (!strcmp(s->name, name)) {
            shm = s;
            break;
        }
    }

    if (!shm) {
        // 如果没有找到，则创建一个新的共享内存
        shm = malloc(sizeof(struct shm));
        // 申请一个内存块
        int len = strlen(name);
        shm->name = malloc(len + 1);
        memcpy(shm->name, name, len);
        shm->name[len] = '\0';  
        // 分配一个页面作为共享内存页
        shm->page = alloc_page();
        shm->next = NULL;

        // 构建共享内存链表
        if (shm_head == NULL) {
            shm_head = shm;
            shm_tail = shm;
        } else {
            shm_tail->next = shm;
            shm_tail = shm;
        }
    }

    // 将共享内存映射到应用程序的地址空间
    // current->pml4 是当前进程页表的根页面
    // va 是固定虚拟地址 0x4000000
    // shm->page 是共享的物理页面
    // us位设置为1，允许允许在特权级为3的用户程序访问这块内存
    // 1表示建立1个页面的映射关系
    map_range(current->pml4, va, shm->page, 0x4, 1);

    return va;
}
