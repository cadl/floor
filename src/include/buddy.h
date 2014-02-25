#ifndef MM_BUDDY_H
#define MM_BUDDY_H
#include <type.h>

struct buddy_alloctor
{
    u32int size;
    u32int block_size;
    s8int *powers;
};

struct buddy_alloctor* buddy_new(u32int size, u32int block_size);
u32int buddy_malloc(struct buddy_alloctor *alloctor, u32int size);
void buddy_free(struct buddy_alloctor *alloctor, u32int addr);
void buddy_destroy(struct buddy_alloctor *alloctor);

void init_sys_alloctor();
extern struct buddy_alloctor page_alloctor;

#endif
