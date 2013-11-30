#include "common.h"
#include "monitor.h"
#include "x86.h"


int kernel_start()
{
    monitor_clear();
    init_gdt();
    monitor_puts("hello\n");
    
    return 0x42;
}
