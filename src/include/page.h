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

typedef struct page_directory_struct
{
    page_t page_tables[1024];
} page_directory_t;

typedef struct page_table_struct
{
    page_t pages[1024];
} page_table_t;


page_directory_t *kernel_page_directory;
page_table_t *kernel_page_tables;
page_directory_t *current_page_directory;

page_t *laddr2page(u32int addr, page_directory_t *pd);

// ===================
void init_paging();

// ====================
page_table_t *frame2pt(u32int frame_idx);
page_directory_t *frame2pd(u32int frame_idx);

// ====================
void switch_page_directory(page_directory_t *pd);
void pagefault_handler(int in, registers_t *reg);

// ====================
void alloc_page(page_t *page, int user, int rw);
void free_page(page_t *page);


// ====================
page_directory_t *clone_page_directory(page_directory_t *src_pd);
void copy_pt(page_table_t *dst_pt, page_table_t *src_pt);

// ====================
void copy_frame(u32int dst_frame_idx, u32int src_frame_idx);
void copy_page(page_t *dst_page, page_t *src_page);


#endif
