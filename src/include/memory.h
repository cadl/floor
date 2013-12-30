#ifndef __MEMORY_H
#define __MEMORY_H
#include <type.h>
#include <sys.h>

#define BITMAP_INDEX(a) ((a) / (8*4))
#define BITMAP_OFFSET(a) ((a) % (8*4))

// |       |--B---|--K------|-----KM--|---U--------|
// |xxxxxxx|xxxxxx|xxxxxxxxx|xxxxxxxxx|xxxxxxxxxxxx|
// 0     0x7c00  0x8000   0x600000  0x1000000     0x2000000  

u32int frames[FRAME_BITMAP_NUM];

void *kmalloc(u32int size);
void *kmalloc_f(u32int size, u32int *frame_idx);
void init_frame();

void mark_frame(u32int frame_idx);
void clear_frame(u32int frame_idx);
u32int test_frame(u32int frame_idx);
u32int get_free_frame_idx();

void *frame2pointer(u32int frame_idx);
void framecpy(u32int dst_frame_idx, u32int src_frame_idx);

#endif
