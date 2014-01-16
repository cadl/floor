#ifndef __SYS_H
#define __SYS_H

#define TOTAL_MEM             0x10000000
#define KMALLOC_SPACE_START   0x0

#define KERNEL_HEAP_START     0x100000
#define KERNEL_HEAP_END       0x400000

#define KERNEL_STACK_TOP      0x800000
#define KERNEL_STACK_BOTTOM   0x400000

#define KERNEL_SPACE_END      0x800000

#define CACHE_START           0x900000
#define CACHE_END             0xE00000

#define USER_SPACE_START      0x1000000

#define USER_CODE_START       0x1000000
#define USER_CODE_END         0x2000000

#define USER_HEAP_START       0x2000000
#define USER_HEAP_END         0x3000000

#define USER_STACK_TOP        0x4000000
#define USER_STACK_BOTTOM     0x3000000

#define USER_SPACE_END        0x4000000

#define STACK_BOTTOM    0x1000000
#define STACK_ESP       0x1010000
#define STACK_TOP       0x1020000
#define FRAME_BITMAP_NUM      (TOTAL_MEM/0x1000/0x20)  // 32bit * n

#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10
#define USER_CODE_SELECTOR   0x1B
#define USER_DATA_SELECTOR   0x23
#define CORE_TSS             0x2B

#endif
