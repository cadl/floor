#ifndef MM_BUDDY_H
#define MM_BUDDY_H
#include <type.h>

struct buddy_allocator
{
    u32int size;
    u32int block_size;
    s8int *powers;
};

struct buddy_allocator* buddy_new(u32int size, u32int block_size);
s32int buddy_malloc(struct buddy_allocator *allocator, u32int size);
void buddy_free(struct buddy_allocator *allocator, u32int addr);
void buddy_destroy(struct buddy_allocator *allocator);

extern struct buddy_allocator page_alloctor;

#endif
