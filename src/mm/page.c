#include <page.h>
#include <type.h>
#include <memory.h>
#include <monitor.h>
#include <asm/system.h>
#include <string.h>

// switch pd, pf_handler
void switch_page_directory(page_directory_t *pd)
{
    current_page_directory = pd;
    __asm__ volatile ("mov %0, %%cr3":: "r"(pd));
    enable_paging();
}

void pagefault_handler(int in, registers_t *reg)
{
    u32int fault_addr;
    int p, rw, us;          // http://wiki.osdev.org/Paging#Page_Faults
    __asm__ volatile ("mov %%cr2, %0": "=r" (fault_addr));
    monitor_puts("addr\n");
    monitor_put_hex(fault_addr);
    monitor_puts("err_code\n");
    monitor_put_hex(reg->err_code);
    p = reg->err_code & 0x01;
    rw = reg->err_code & 0x02;
    us = reg->err_code & 0x04;
    panic("page fault\n");
}

//  alloc_page, free_page 
void alloc_page(page_t *page, int user, int rw)
{
    if (!(page->present))
    {
        u32int frame_idx = get_free_frame_idx();
        if (frame_idx == (u32int)-1)
        {
            panic("no free frame\n") ;
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

// laddr2page: line address to page_t*
page_t *laddr2page(u32int line_addr, page_directory_t *pd)
{
    u32int pt_idx, pg_idx; 
    pt_idx = line_addr / 0x1000 / 1024;
    pg_idx = line_addr / 0x1000 % 1024;
    if (pd->page_tables[pt_idx].present)
    {
        return &(((page_table_t *)frame2pointer(pd->page_tables[pt_idx].frame))->pages[pg_idx]);
    }
    else
    {
        monitor_put_hex(line_addr);
        monitor_putc('\n');
        alloc_page(&(pd->page_tables[pt_idx]), 1, 1);
        return &(((page_table_t *)frame2pointer(pd->page_tables[pt_idx].frame))->pages[pg_idx]);
    }
}

// init_paging
void init_paging()
{
    int i, j;
    u32int start=0;
    init_frame(); 
    kernel_page_directory = (page_directory_t *)kmalloc(sizeof(page_directory_t));
    memset(kernel_page_directory, 0, sizeof(page_directory_t));
    current_page_directory = kernel_page_directory;
    kernel_page_tables = (page_table_t *)kmalloc(sizeof(page_table_t)*6);
    memset(kernel_page_tables, 0, sizeof(page_table_t)*6);
    for (i=0; i<6; i++)
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
        monitor_put_hex(start);
    }
    switch_page_directory(kernel_page_directory);
}

// clone page directory, copy_page table
page_directory_t *clone_page_directory(page_directory_t *src_pd)
{
    int i;
    page_directory_t *dst_pd;
    dst_pd = (page_directory_t *)kmalloc(sizeof(page_directory_t));
    memset(dst_pd, 0, sizeof(page_directory_t));
    for (i=0; i<4; i++)
    {
        if (!src_pd->page_tables[i].present)
        {
            continue;
        }
        dst_pd->page_tables[i] = src_pd->page_tables[i];
    }
    for (i=4; i<6; i++)
    {
         if (!src_pd->page_tables[i].present)
        {
            continue;
            copy_page(&(dst_pd->page_tables[i]), &(src_pd->page_tables[i]));
            copy_pt(frame2pt(dst_pd->page_tables[i].frame), frame2pt(src_pd->page_tables[i].frame));
        }
    }
    return dst_pd;
}

void copy_pt(page_table_t *dst_pt, page_table_t *src_pt)
{
    int i;
    for (i=0; i<1024; i++)    
    {
        if (!src_pt->pages[i].present)
        {
            continue;
        }
        copy_page(&(dst_pt->pages[i]), &(src_pt->pages[i]));
    }
}

// copy page, copy frame
void copy_page(page_t *dst_page, page_t *src_page)
{
    u32int frame_idx = get_free_frame_idx();
    if (frame_idx == (u32int)-1)
    {
        panic("no free frame\n");
    }
    copy_frame(frame_idx, src_page->frame);
    dst_page->rw = src_page->rw;
    dst_page->user = src_page->user;
    dst_page->accessed = src_page->accessed;
    dst_page->dirty = src_page->dirty;
    dst_page->unused = src_page->unused;
    dst_page->frame = frame_idx;
}

void copy_frame(u32int dst_frame_idx, u32int src_frame_idx)
{
    u32int src, dst;
    src = src_frame_idx * 0x1000;
    dst = dst_frame_idx * 0x1000;
    cli();
    disable_paging();
    memcpy((void *)dst, (void *)src, 512);  // 512 byte per frame
    enable_paging();
    sti();
}

// frame to page_table *, frame to page_directory *
page_table_t *frame2pt(u32int frame_idx)
{
    return (page_table_t *)(frame_idx * 0x1000);
}

page_directory_t *frame2pd(u32int frame_idx)
{
    return (page_directory_t *)(frame_idx * 0x1000);
}
