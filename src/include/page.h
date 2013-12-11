#ifndef __PAGE_H
#define __PAGE_H
#include "type.h"

#define BITMAP_INDEX(a) ((a)/(8*4))
#define BITMAP_OFFSET(a) ((a)%(8*4))


typedef struct pd_entry_struct // page directory entry
{
    u32int present:1;
    u32int rw:1;
    u32int user:1;
    u32int write_through:1;
    u32int cache_disabled:1;
    u32int accessed:1;
    u32int always0:1;
    u32int page_size:1;
    u32int ignored:1;
    u32int avail:3;
    u32int frame:20;
} pd_entry_t;

typedef struct pt_entry_struct // page table entry
{
    u32int present:1;
    u32int rw:1;
    u32int user:1;
    u32int write_through:1;
    u32int cache_disabled:1;
    u32int accessed:1;
    u32int dirty:1;
    u32int always0:1;
    u32int global:1;
    u32int avail:3;
    u32int frame:20;
    
} pt_entry_t;

typedef struct page_directory_struct
{
    pd_entry_t pt[1024];
} page_directory_t;

typedef struct page_table_struct
{
    pt_entry_t page[1024];
} page_table_t;

// mark frame status use bitmap
//
//
u32int nframes;
u32int *frames;

void alloc_frame(pt_entry_t *page, int user, int rw);
void free_frame(pt_entry_t *page);
void init_paging();
void switch_page_directory(page_directory_t *pd);

static void set_frame(u32int addr)
{
    u32int frame_idx = addr / 0x1000; // 4kb per frame 
    u32int b_idx = BITMAP_INDEX(frame_idx);
    u32int b_offset = BITMAP_OFFSET(frame_idx);
    frames[b_idx] |= (0x1 << b_offset);
}

static void clear_frame(u32int addr)
{
    u32int frame_idx = addr / 0x1000;
    u32int b_idx = BITMAP_INDEX(frame_idx);
    u32int b_offset = BITMAP_OFFSET(frame_idx);
    frames[b_idx] &= ~(0x1 << b_offset);
}

static u32int test_frame(u32int addr)
{
    u32int frame_idx = addr / 0x1000;
    u32int b_idx = BITMAP_INDEX(frame_idx);
    u32int b_offset = BITMAP_OFFSET(frame_idx);
    return (frames[b_idx] & (0x1 << b_offset));
}

static u32int get_free_frame_idx()
{
    u32int i, j;
    for (i=0; i<BITMAP_INDEX(nframes); i++)
    {
        if (frames[i] != 0xffffffff)
        {
            for (j=0; j<32; j++)
            {
                u32int b = 0x1 << j;
                if (!(frames[i]&b))
                {
                    return i*32+j;  
                }
            }
        }
    }
}
#endif
