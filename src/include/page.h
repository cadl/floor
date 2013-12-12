#ifndef __PAGE_H
#define __PAGE_H
#include "type.h"
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
page_directory_t *current_page_directory;

page_t *laddr2page(u32int addr, page_directory_t *pd);
void alloc_page(page_t *page, int user, int rw);
void free_page(page_t *page);
void init_paging();
void switch_page_directory(page_directory_t *pd);

#endif
