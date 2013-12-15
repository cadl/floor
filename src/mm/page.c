#include <page.h>
#include <type.h>
#include <memory.h>
#include <monitor.h>

void alloc_page(page_t *page, int user, int rw)
{
    if (!(page->present))
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
    if (pd->page_tables[pt_idx].present)
    {
        return &((page_table_t *)((pd->page_tables[pt_idx]).frame*0x1000))->pages[pg_idx];
    }
    else
    {
        monitor_put_hex(line_addr);
        monitor_putc('\n');
        alloc_page(&(pd->page_tables[pt_idx]), 1, 1);
        monitor_puts("2page\n");
        return &((page_table_t *)((pd->page_tables[pt_idx]).frame*0x1000))->pages[pg_idx];

    }
}

void init_paging()
{
    int i, j;
    u32int start=0;
    init_frame(); 
    kernel_page_directory = (page_directory_t *)kmalloc(sizeof(page_directory_t));
    memset(kernel_page_directory, 0, sizeof(page_directory_t));
    current_page_directory = kernel_page_directory;
    kernel_page_tables = (page_table_t *)kmalloc(sizeof(page_table_t)*4);
    memset(kernel_page_tables, 0, sizeof(page_table_t)*4);
    for (i=0; i<4; i++)
    {
        for(j=0; j<1024; j++)
        {
            kernel_page_tables[i].pages[j].frame = start;
            kernel_page_tables[i].pages[j].present = 1;
            kernel_page_tables[i].pages[j].user = 1;
            kernel_page_tables[i].pages[j].rw = 1;
            start++;
        }
        kernel_page_directory->page_tables[i].frame = ((u32int)&(kernel_page_tables[i]))/0x1000;
        kernel_page_directory->page_tables[i].present = 1;
        kernel_page_directory->page_tables[i].user = 1;
        kernel_page_directory->page_tables[i].rw = 1;
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
    monitor_puts("cr\n");
}

void pagefault_handler(int in, registers_t *reg)
{
    u32int fault_addr;
    int bit0, bit1, bit2, bit4;
    __asm__ volatile ("mov %%cr2, %0": "=r" (fault_addr));
    monitor_put_hex(fault_addr);
    monitor_puts(" ---- addr\n");
    monitor_put_hex(reg->err_code);
    monitor_puts(" ---- err_code\n");
    bit0 = reg->err_code & 0x1;
    bit1 = reg->err_code & 0x2;
    bit2 = reg->err_code & 0x4;
    bit4 = reg->err_code & 0x10; 
}

