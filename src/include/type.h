#ifndef __TYPE_H
#define __TYPE_H


typedef unsigned int    u32int;
typedef int             s32int;
typedef unsigned short  u16int;
typedef int             s16int;
typedef unsigned char   u8int;
typedef char            s8int;


struct registers_struct
{
    u32int ds, es, fs, gs;
    u32int ebp, esi, edi, edx, ecx, ebx, eax;
    u32int int_no, err_code;
    u32int eip, cs, eflags;
    u32int esp, ss;
} __attribute__((packed));

typedef struct registers_struct registers_t;
#endif
