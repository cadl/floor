#include <type.h>
#include <monitor.h>
#include <init.h>


int kernel_start()
{
    char a[] = "hello\n";
    monitor_clear();
    monitor_puts(a);
    init();
    __asm__ volatile ("int $3");
    monitor_puts("hello\n");
    monitor_put_hex(0x1f00000);
    monitor_puts(*(int *)(0x1f00000));
    
    return 0x42;
}
