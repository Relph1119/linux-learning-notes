#include "include/mm.h"
#include "include/print.h"
#include "include/string.h"

uint8_t pages[MAX_PAGES];
unsigned long mem_size = 0;

// 物理映射的第2阶段
// pml4_pa：根页表的物理地址
// from_va：需要映射的起始虚拟地址
// to_pa：from_va对应的物理地址
// us：访问权限，如果页表项映射的内存允许应用程序访问，传递0x4，如果只允许内核访问，则传递0
// npage：映射的页面数 
void map_range(unsigned long pml4_pa, unsigned long from_va, unsigned long to_pa, char us, long npage) {
    long n = 0;
    
    while (n < npage) {
  	    // pml4
  	    unsigned long* page_va = VA(pml4_pa);
        // 第39~47位用于四级页表的索引
  	    short index = (from_va >> 39) & 0x1ff;
        // 计算页表项的地址
  	    unsigned long entry = *(page_va + index);
        // 判断页表项中存在位
  	    if (!(entry & 0x1)) {
            // 如果不存在，分配页面，并设置存在位和可读写位，设置访问权限
            *(page_va + index) = alloc_page() | 0x3 | us;
            // 更新页表项
            entry = *(page_va + index);
  	    }
 
  	    // pml3
  	    page_va = VA(entry & 0xffffffffff000);
  	    index = (from_va >> 30) & 0x1ff;
  	    entry = *(page_va + index);
  	    if (!(entry & 0x1)) {
            *(page_va + index) = alloc_page() | 0x3 | us;
            entry = *(page_va + index);
  	    }

  	    // pml2
  	    page_va = VA(entry & 0xffffffffff000);
  	    index = (from_va >> 21) & 0x1ff;
  	    entry = *(page_va + index);
  	    if (!(entry & 0x1)) {
            *(page_va + index) = alloc_page() | 0x3 | us;
            entry = *(page_va + index);
  	    }

  	    // pml1
  	    page_va = VA(entry & 0xffffffffff000);
  	    index = (from_va >> 12) & 0x1ff;
  	    if (!((*(page_va + index)) & 0x1)) {
            *(page_va + index) = (to_pa + PAGE_SIZE * n) | 0x3 | us;
  	    }
		
        n++;
        from_va += PAGE_SIZE;
    }
}

void mm_init() {
    // 指向E820记录区域
    struct e820map* e820 = (struct e820map*) 0x13000;

    // 计算物理内存区域
    for (int i = 0; i < e820->nr_entry; i++) {
        if (e820->map[i].type == E820_RAM) {
            // 将内存区域的起始地址与长度相加，得到内存地址
            unsigned long tmp = e820->map[i].addr + e820->map[i].size;
            // 最大值即可用物理内存的大小
            if (tmp > mem_size) {
                mem_size = tmp;
            }
        }
    }

    print(mem_size);     

    // 将数组pages的各项全部设置为0
    memset(pages, 0, MAX_PAGES);

    // 将内核映像占用的页面标记为1，表示已用
    for (int i = 0; i < KERNEL_PAGE_NUM; i++) {
        pages[i] = 1;
    }

    map_range(TASK0_PML4, (unsigned long)VA(0), 0, 0, 
              (mem_size + PAGE_SIZE - 1) / PAGE_SIZE);
}