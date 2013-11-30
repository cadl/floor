#include <common.h>
#include <monitor.h>


int kernel_start()
{
    int i;
    char a[] = "aaabb\n";
    monitor_clear();
    monitor_puts(a);
    monitor_put_dec(11234);
    monitor_putc('\n');
    monitor_put_hex(0x1234a);
    monitor_putc('\n');
    for (i=0; i<100; i++)
    {
        monitor_puts("sandan is sb\n");
    }
    
    return 0x42;
}
