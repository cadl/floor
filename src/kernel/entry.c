#include <common.h>
#include <monitor.h>


int kernel_start()
{
    monitor_clear();
    monitor_puts("aaaaaabbbbbb\n");
    return 0x42;
}
