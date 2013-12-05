#include "common.h"
#include "monitor.h"
#include "x86.h"


int kernel_start()
{
    monitor_clear();
    init_gdt();
    init_idt();
    __asm__ volatile ("int $3");
    __asm__ volatile ("int $4");
    __asm__ volatile ("cli");
    __asm__ volatile ("int $4");
    __asm__ volatile ("int $5");
    monitor_puts("hello\n");
    
    return 0x42;
}
