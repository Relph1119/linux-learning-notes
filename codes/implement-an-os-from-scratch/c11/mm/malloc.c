#include "include/types.h"
#include "include/mm.h"

// 记录页面中内存块的使用情况，即桶链表
struct bucket_desc {
    void* freeptr; // 空闲块链表第一个节点的地址
    struct bucket_desc* next; // 指向下一个页面
    short refcnt; // 记录页面中使用的块数
};

struct bucket_dir {
    uint16_t size; // 块大小
    struct bucket_desc* bdesc; // 对应的桶链表
};

// 分为不同字节大小的内存块
static struct bucket_dir bucket_dir[] = {
    { 16, NULL }, 
    { 32, NULL },
    { 64, NULL },
    { 128, NULL },
    { 256, NULL },
    { 512, NULL },
    { 1024, NULL },
    { 2048, NULL },
    { 0, NULL }
};

// 分配内存块
void* malloc(int size) {
    void* va = NULL;
    struct bucket_dir* bdir = NULL;
    struct bucket_desc* bdesc = NULL;

    // 找到合适内存块大小的桶链表
    for (bdir = bucket_dir; bdir->size != 0; bdir++) {
        if (bdir->size >= size) {
            break;
        }
    }

    if (bdir->size == 0) {
        return NULL;
    }

    // 找到空闲块
    for (bdesc = bdir->bdesc; bdesc; bdesc = bdesc->next) {
        if (bdesc->freeptr) {
            break;
        }
    }

    // 如果找不到
    if (bdesc == NULL) {
        // 新申请一个页面
        bdesc = VA(alloc_page());
        bdesc->freeptr = (void*)((unsigned long)bdesc + sizeof(struct bucket_desc));
        bdesc->next = NULL;
        bdesc->refcnt = 0;

        unsigned long* p = bdesc->freeptr;
        int i = 0;
        // 将页面中的内存块链接为一个链表
        while (++i < ((PAGE_SIZE - sizeof(struct bucket_desc)) / bdir->size)) {
            *p = (unsigned long)p + bdir->size;
            p = (unsigned long*)((unsigned long)p + bdir->size);
        }
        *p = 0;

        // 将新分配的桶链接到表头
        bdesc->next = bdir->bdesc;
        bdir->bdesc = bdesc;
    }

    // 从空闲块链表的头部分配一个内存块
    va = bdesc->freeptr;
    // 将原头部空闲块从空闲链表中删除
    bdesc->freeptr = (void*)(*((unsigned long*)va));
    bdesc->refcnt++;

    // 返回分配到的内存块地址
    return va;
}

// 释放内存块
void free(void* obj) {
    // 根据内存块的地址，计算所在页面的页面地址
    unsigned long page = (unsigned long)obj & ~0xfffUL;
    struct bucket_dir* bdir = bucket_dir;;
    struct bucket_desc* bdesc = NULL;

    // 遍历桶链表
    for (; bdir->size != 0; bdir++) {
        bdesc = bdir->bdesc;

        for (; bdesc; bdesc = bdesc->next) {
            // 找到桶后，将内存块插到桶中的空闲链表的表头
            if ((unsigned long)bdesc == page) {
                *((unsigned long*)obj) = (unsigned long)bdesc->freeptr;
                bdesc->freeptr = obj;
                bdesc->refcnt--;
                goto free_page;
            }
        }
    }

    // 如果桶的引用计数减为0，释放页面
free_page:
    if (bdesc && bdesc->refcnt == 0) {
        struct bucket_desc* tmp = bdir->bdesc;
        struct bucket_desc* prev = NULL;
        for (; tmp; tmp = tmp->next) {
            if ((unsigned long)tmp == (unsigned long)bdesc) {
                break;
            }
            prev = tmp;
        }

        if (!prev) {
            bdir->bdesc = tmp->next;
        } else {
            prev->next = tmp->next;
        }

        free_page(PA(bdesc));
    }
}