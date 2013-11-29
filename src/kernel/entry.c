#include <common.h>
#include <monitor.h>


int kernel_start()
{
    char a[] = "aaabb\n";
    monitor_clear();
    monitor_puts(a);
    monitor_put_dec(11234);
    monitor_putc('\n');
    monitor_put_dec(0);
    monitor_putc('\n');
    monitor_put_dec(013);
    monitor_putc('\n');
    monitor_put_dec(-32);
    return 0x42;
}
