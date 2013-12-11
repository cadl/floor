#ifndef __MEMORY_H
#define __MEMORY_H
#include <type.h>

// |       |--boot---|-----kernel--------|-malloc-memory-|
// |xxxxxxx|xxxxxxxxx|xxxxxxxxx|xxxxxxxxx|xxxxxxxxxxxxxxx|
// 0     0x7c00    0x8000             0x600000  

u32int placement_addr;
u32int kmalloc(u32int size);

#endif
