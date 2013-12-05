#include "type.h"
#include "int.h"
#include "monitor.h"


void interrupt_handler(int in, registers_t *reg)
{
    if (in < 0 || in > INT_MAXN)
    {
        monitor_puts("unrecognized interruptnumber\n");
        return;
    }
    monitor_puts("recieved interrupt: ");
    monitor_put_dec(in);
    monitor_puts("\n");
}
