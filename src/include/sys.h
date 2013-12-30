#ifndef __SYS_H
#define __SYS_H

#define TOTAL_MEM       0x2000000
#define KMALLOC_START   0x600000
#define KERNEL_END      0x1000000
#define STACK_BOTTOM    0x1000000
#define STACK_ESP       0x1400000
#define STACK_TOP       0x1600000
#define FRAME_BITMAP_NUM      (TOTAL_MEM/0x1000/0x20)  // 32bit * n
#endif
