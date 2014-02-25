#ifndef __PAGE_H
#define __PAGE_H

#include <type.h>
#include <sys.h>

#define enable_paging() \
{\
    u32int cr0;\
    __asm__ volatile ("mov %%cr0, %0": "=r"(cr0)::"%eax");\
    cr0 |= 0x80000000;\
    __asm__ volatile ("mov %0, %%cr0":: "r"(cr0):"%eax");\
}

#define disable_paging() \
{\
    u32int cr0;\
    __asm__ volatile ("mov %%cr0, %0": "=r"(cr0));\
    cr0 &= 0x7fffffff;\
    __asm__ volatile ("mov %0, %%cr0":: "r"(cr0));\
}

#define switch_page_directory(pd) \
{\
    current_page_directory = pd;\
    __asm__ volatile ("mov %0, %%cr3":: "r"(pd->phy_addr));\
    u32int cr0;\
    __asm__ volatile ("mov %%cr0, %0": "=r"(cr0):);\
    cr0 |= 0x80000000;\
    __asm__ volatile ("mov %0, %%cr0":: "r"(cr0));\
}

typedef struct page_struct
{
    u32int present:1;
    u32int rw:1;
    u32int user:1;
    u32int accessed:1;
    u32int dirty:1;
    u32int unused:7;
    u32int frame:20;
} page_t;

typedef struct page_table_struct
{
    page_t pages[1024];
} page_table_t;

typedef struct page_directory_struct
{
    page_table_t *tables[1024];
    page_t tables_physical[1024];
    u32int phy_addr;
} page_directory_t;

extern page_directory_t *kernel_page_directory;
extern page_directory_t *current_page_directory;

void init_paging();
void page_alloc(page_t *page, int is_kernel, int is_writeable);

void pagefault_handler(int in, registers_t *reg);

page_t *addr2page(u32int address, int is_kernel, int make, page_directory_t *pd);
page_directory_t *clone_directory(page_directory_t *src);
page_table_t *clone_table(page_table_t *src, u32int *frame_idx);

extern void framecpy(int dst, int src);

#endif
