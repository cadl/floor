#include <type.h>
#include <monitor.h>
#include <init.h>


int kernel_start()
{
    init();
    char a[] = "hello\n";
    monitor_puts(a);
    __asm__ volatile ("int $3");
    monitor_puts("hello\n");
    //monitor_put_hex(0x1f00000);
    //monitor_puts(*(int *)(0x1f00000));
    
    return 0x42;
}
