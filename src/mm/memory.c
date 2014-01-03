#include <memory.h>
#include <type.h>
#include <sys.h>
#include <string.h>
#include <monitor.h>
#include <asm/system.h>
#include <page.h>


u32int placement_addr = (u32int)KMALLOC_START;

void *kmalloc(u32int size)
{
    void *m_start;
    if (placement_addr & (0xfff))
    {
        placement_addr &= 0xfffff000;
        placement_addr += 0x1000;
    }
    m_start = (void *)placement_addr;
    placement_addr += size;
    return m_start;
}

void *kmalloc_f(u32int size, u32int *frame_idx)
{
    void *m_start;
    if (placement_addr & 0xfff)
    {
        placement_addr &= 0xfffff000;
        placement_addr += 0x1000;
    }
    *frame_idx = placement_addr / 0x1000;
    m_start = (void *)placement_addr;
    placement_addr += size;
    return m_start;
}
void init_frame()
{
    memset(frames, 0x00, sizeof(u32int) * FRAME_BITMAP_NUM);
}

void *frame2pointer(u32int frame_idx)
{
    u32int addr;
    addr = frame_idx * 0x1000;
    return (void *)addr;
}

void mark_frame(u32int frame_idx)
{
    u32int b_idx = BITMAP_INDEX(frame_idx);
    u32int b_offset = BITMAP_OFFSET(frame_idx);
    frames[b_idx] |= (0x1 << b_offset);
}

void clear_frame(u32int frame_idx)
{
    u32int b_idx = BITMAP_INDEX(frame_idx);
    u32int b_offset = BITMAP_OFFSET(frame_idx);
    frames[b_idx] &= ~(0x1 << b_offset);
}

u32int test_frame(u32int frame_idx)
{
    u32int b_idx = BITMAP_INDEX(frame_idx);
    u32int b_offset = BITMAP_OFFSET(frame_idx);
    return (frames[b_idx] & (0x1 << b_offset));
}

u32int get_free_frame_idx()
{
    u32int i, j;
    for (i=0; i<FRAME_BITMAP_NUM; i++)
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
    panic("no free frame\n");
    return (u32int)-1;
}
