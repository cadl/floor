#ifndef __PAGE_H
#define __PAGE_H

#include <type.h>
#include <sys.h>

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


page_directory_t *kernel_page_directory;
page_directory_t *current_page_directory;

void init_paging();
void alloc_frame(page_t *page, int is_kernel, int is_writeable);

void switch_page_directory(page_directory_t *pd);
void pagefault_handler(int in, registers_t *reg);

page_t *get_page(u32int address, int make, page_directory_t *pd);
page_directory_t *clone_directory(page_directory_t *src);
page_table_t *clone_table(page_table_t *src, u32int *frame_idx);

#endif
