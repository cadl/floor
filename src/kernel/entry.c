#include <type.h>
#include <monitor.h>
#include <init.h>


int kernel_start()
{
    monitor_clear();
    init();
    __asm__ volatile ("int $3");
    monitor_puts("hello\n");
    
    return 0x42;
}
