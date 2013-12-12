#ifndef __MEMORY_H
#define __MEMORY_H
#include <type.h>
#include <sys.h>

#define BITMAP_INDEX(a) ((a) / (8*4))
#define BITMAP_OFFSET(a) ((a) % (8*4))

// |       |--boot---|-----kernel--------|-malloc-memory-|
// |xxxxxxx|xxxxxxxxx|xxxxxxxxx|xxxxxxxxx|xxxxxxxxxxxxxxx|
// 0     0x7c00    0x8000             0x600000  

void* kmalloc(u32int size);
void memset(void* ptr, u8int val, u32int size);

u32int nframes;        // byte of bitmap
u32int *frames;

static void mark_frame(u32int frame_idx)
{
    u32int b_idx = BITMAP_INDEX(frame_idx);
    u32int b_offset = BITMAP_OFFSET(frame_idx);
    frames[b_idx] |= (0x1 << b_offset);
}

static void clear_frame(u32int frame_idx)
{
    u32int b_idx = BITMAP_INDEX(frame_idx);
    u32int b_offset = BITMAP_OFFSET(frame_idx);
    frames[b_idx] &= ~(0x1 << b_offset);
}

static u32int test_frame(u32int frame_idx)
{
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
    return (u32int)-1;
}

void init_frame();
#endif
