#ifndef __MEMORY_H
#define __MEMORY_H
#include <type.h>
#include <sys.h>

#define BITMAP_INDEX(a) ((a) / (8*4))
#define BITMAP_OFFSET(a) ((a) % (8*4))

// |       |--B---|--K------|-----KM--|---U--------|
// |xxxxxxx|xxxxxx|xxxxxxxxx|xxxxxxxxx|xxxxxxxxxxxx|
// 0     0x7c00  0x8000   0x600000  0x1000000     0x2000000  

void* kmalloc(u32int size);
void init_frame();

u32int frames[FRAMES_NUM];

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
    for (i=0; i<FRAMES_NUM; i++)
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

void *frame2pointer(u32int frame_idx);

#endif
