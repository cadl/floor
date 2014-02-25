#include <page.h>
#include <type.h>
#include <memory.h>
#include <monitor.h>
#include <string.h>

page_directory_t *kernel_page_directory;
page_directory_t *current_page_directory;

void page_alloc(page_t *page, int is_kernel, int is_writeable)
{
    u32int frame_idx;
    if (!page->present)
    {
        frame_idx = get_free_frames(1);
        page->present = 1;
        page->rw = (is_writeable == 1)?1:0;
        page->user = (is_kernel == 1)?0:1;
        page->frame = frame_idx;
    }
}

page_t *addr2page(u32int address, int is_kernel, int make, page_directory_t *pd)
{
    address /= 0x1000;
    u32int table_idx = address / 1024;
    if (pd->tables[table_idx])
    {
        return &pd->tables[table_idx]->pages[address%1024];
    }
    else if(make)
    {
        u32int frame_idx;
        page_t *pg;
        pd->tables[table_idx] = (page_table_t *)kmalloc_f(sizeof(page_table_t), &frame_idx);
        memset(pd->tables[table_idx], 0, 0x1000);
        pg = &pd->tables_physical[table_idx];
        pg->frame = frame_idx;
        pg->present = 1;
        pg->rw = 1;
        pg->user = (is_kernel == 1)?0:1;
        return &pd->tables[table_idx]->pages[address%1024];
    }
    return 0;
}

void pagefault_handler(int in, registers_t *reg)
{
    u32int fault_addr;          // http://wiki.osdev.org/Paging#Page_Faults
    __asm__ volatile ("mov %%cr2, %0": "=r" (fault_addr));
    monitor_put_hex(in);
    monitor_puts("addr\n");
    monitor_put_hex(fault_addr);
    monitor_puts("err_code\n");
    monitor_put_hex(reg->err_code);
    panic("page fault\n");
}


page_directory_t *clone_directory(page_directory_t *src)
{
    u32int frame_idx;
    int i;
    page_directory_t *pd = (page_directory_t *)kmalloc_f(sizeof(page_directory_t), &frame_idx);
    memset(pd, 0, sizeof(page_directory_t));
    pd->phy_addr = (frame_idx + 1) * 0x1000;

    for (i=0; i<1024; i++)
    {
        if (!src->tables[i])
        {
            continue;
        }
        if (kernel_page_directory->tables[i] == src->tables[i])
        {
            pd->tables[i] = src->tables[i];
            pd->tables_physical[i] = src->tables_physical[i];
        }
        else
        {
            u32int pt_frame_idx;
            pd->tables[i] = clone_table(src->tables[i], &pt_frame_idx);
            pd->tables_physical[i].frame = pt_frame_idx;
            pd->tables_physical[i].present = 1;
            pd->tables_physical[i].rw = 1;
            pd->tables_physical[i].user = 1;
        }
    }
    return pd;
}

page_table_t *clone_table(page_table_t *src, u32int *frame_idx)
{
    int i;
    page_table_t *table = (page_table_t *)kmalloc_f(sizeof(page_table_t), frame_idx);
    memset(table, 0, sizeof(page_table_t));
    for (i=0; i<1024; i++)
    {
        if (!src->pages[i].present)
        {
            continue;
        }
        page_alloc(&table->pages[i], 0, 0);
        table->pages[i].present = src->pages[i].present;
        table->pages[i].rw = src->pages[i].rw;
        table->pages[i].user = src->pages[i].user;
        table->pages[i].accessed = src->pages[i].accessed;
        table->pages[i].dirty = src->pages[i].dirty;
        framecpy(table->pages[i].frame*0x1000, src->pages[i].frame*0x1000);

    }
    return table;
}

void init_paging()
{
    u32int frame_idx;
    u32int i;
    page_t *tmp_page;

    init_frame();

    kernel_page_directory = (page_directory_t *)kmalloc_f(sizeof(page_directory_t),
                                                          &frame_idx);
    memset(kernel_page_directory, 0, sizeof(page_directory_t));
    kernel_page_directory->phy_addr = (u32int)kernel_page_directory->tables_physical;

    for (i=0; i<(u32int)KERNEL_HEAP_END; i+=0x1000) // [KERNEL_SPACE_START, KERNEL_SPACE_END]
    {
        tmp_page = addr2page(i, 1, 1, kernel_page_directory);
        tmp_page->present = 1;
        tmp_page->user = 0;
        tmp_page->rw = 1;
        tmp_page->frame = i / 0x1000;
    }

    for (i=(u32int)CACHE_START; i<(u32int)CACHE_END; i+=0x1000)
    {
        tmp_page = addr2page(i, 1, 1, kernel_page_directory);
        tmp_page->present = 1;
        tmp_page->user = 0;
        tmp_page->rw = 1;
        tmp_page->frame = i / 0x1000;
    }
    switch_page_directory(kernel_page_directory);
}
