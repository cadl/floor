#ifndef __ASM_SYS_H
#define __ASM_SYS_H
#include "type.h"


static inline void outb(u16int port, u8int value)
{
    __asm__ volatile ("outb %0, %1" :: "a"(value), "d"(port));
}


static inline u8int inb(u16int port)
{
    u8int ret;
    __asm__ volatile ("inb %1, %0": "=a"(ret): "d"(port));
    return ret;
}


static inline u16int inw(u16int port)
{
    u16int ret;
    __asm__ volatile ("inw %1, %0": "=a"(ret): "d"(port));
    return ret;
}

static inline void lidt(u32int idt_ptr)
{
    __asm__ volatile ("lidt (%0)" :: "r"(idt_ptr));
}

static inline void cli()
{
    __asm__ volatile ("cli" ::);
}

static inline void sti()
{
    __asm__ volatile ("sti" ::);
}

static inline void io_wait()
{
    __asm__ volatile ("jmp 1f\n\t"
                      "1:jmp 2f\n\t"
                      "2:");
}

static inline void hlt()
{
    __asm__ volatile ("hlt" ::);
}

#endif
