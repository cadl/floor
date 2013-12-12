#include <page.h>
#include <type.h>
#include <memory.h>
#include <monitor.h>

void alloc_page(page_t *page, int user, int rw)
{
    if (!(page->frame))
    {
        u32int frame_idx = get_free_frame_idx();
        if (frame_idx == (u32int)-1)
        {
            monitor_puts("no free frame\n") ;
            return;
        }
        mark_frame(frame_idx);
        page->frame = frame_idx;
        page->user = user;
        page->rw = rw;
        page->present = 1;
    }
}

void free_page(page_t *page)
{
    u32int frame_idx;
    frame_idx = page->frame;
    if (frame_idx)
    {
        clear_frame(frame_idx);
    }
}

page_t *laddr2page(u32int line_addr, page_directory_t *pd)
{
    u32int pt_idx, pg_idx; 
    pt_idx = line_addr / 0x1000 / 1024;
    pg_idx = line_addr / 0x1000 % 1024;
    if (pd->page_tables[pt_idx].frame)
    {
        return &((page_table_t *)((pd->page_tables[pt_idx]).frame*0x1000))->pages[pg_idx];
    }
    else
    {
        alloc_page(&(pd->page_tables[pt_idx]), 1, 1);
        memset(&pd->page_tables[pt_idx], 0, sizeof(page_table_t));
        pd->page_tables[pt_idx].present = 1;
        return &((page_table_t *)((pd->page_tables[pt_idx]).frame*0x1000))->pages[pg_idx];

    }
}

void init_paging()
{
    int i;
    page_t *tmp;
    init_frame(); 
    kernel_page_directory = (page_directory_t *)kmalloc(sizeof(page_directory_t));
    memset(kernel_page_directory, 0, sizeof(page_directory_t));
    current_page_directory = kernel_page_directory;
    for (i=0; i<(u32int)KERNEL_END; i+=0x1000)
    {
        tmp = laddr2page(i, kernel_page_directory);
        alloc_page(tmp, 1, 1);
    }
    switch_page_directory(kernel_page_directory);
}

void switch_page_directory(page_directory_t *pd)
{
    u32int cr0;
    current_page_directory = pd;
    __asm__ volatile ("mov %0, %%cr3":: "r"(pd));
    __asm__ volatile ("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile ("mov %0, %%cr0":: "r"(cr0));

}
