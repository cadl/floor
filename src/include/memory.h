#ifndef __MEMORY_H
#define __MEMORY_H
#include <type.h>
#include <sys.h>

// the old one
// |       |--B---|--K------|-----KM--|---U--------|
// |xxxxxxx|xxxxxx|xxxxxxxxx|xxxxxxxxx|xxxxxxxxxxxx|
// 0     0x7c00  0x8000   0x600000  0x1000000     0x2000000

// virtual memory
// |           |           |           |           |           |           |           |           |           |           |
// 0      0x7c00      0x8000    0x100000    0x400000    0x800000    0xE00000   0x1000000   0x2000000   0x3000000   0x4000000
// a           b           c           d           e           f           @           g           h           i           j
// |-----------------------------------------------------------|-----------|           |-----------------------------------|
//         kernel space   15Mb                                                        user space  48Mb
//                                                                                     |                                   |
// a -> 0                                                                              |                                   |
// b -> boot start                                                                     |                                   |
// c -> kernel code start                                                              |                                   |
// d -> kernel code end and kernel heap start                                          |                                   |
// e -> kernel heap end and kernel stack start
// f -> kernel stack end                                                               |                                   |
// f - @ ->  cache                                                                     |                                   |
// g -> user code start                                                                |                                   |
// h -> user code end and user heap start                                              |                                   |
// i -> user heap end and user stack start                                             |                                   |
// j -> user stack end                                                                 |                                   |
//                                                                                     |                                   |
// physical memory                                                                     |                                   |
// |-----------------------------------------------------------------------------------|-----------------------------------|------------|
// 0                                                                           0x1000000                          0x4000000   0x10000000

void *kmalloc(u32int size);
void *kmalloc_f(u32int size, u32int *frame_idx);
void init_frame();

s32int get_free_frames(u32int n);

void *frame2pointer(u32int frame_idx);

#endif
