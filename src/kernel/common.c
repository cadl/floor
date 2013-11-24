#include "common.h"


void outb(u16int port, u8int value)
{
    asm volatile ("outb %0, %1" :: "a"(value), "d"(port)) ;
}


u8int inb(u16int port)
{
    u8int ret;
    asm volatile ("inb %1, %0": "=a"(ret): "d"(port));
    return ret;
}


u16int inw(u16int port)
{
    u16int ret;
    asm volatile ("inw %1, %0": "=a"(ret): "d"(port));
    return ret;
}
