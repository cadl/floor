#include "type.h"
#include "monitor.h"
#include "timer.h"
#include "des.h"


int kernel_start()
{
    monitor_clear();
    init_gdt();
    init_idt();
    init_timer(50);
    __asm__ volatile ("int $3");
    monitor_puts("hello\n");
    
    return 0x42;
}
