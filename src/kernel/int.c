#include <type.h>
#include <int.h>
#include <monitor.h>
#include <pic.h>
#include <timer.h>
#include <page.h>
#include <asm/system.h>
#include <syscall.h>


void interrupt_handler(int in, registers_t *reg)
{
    if (in < 0 || in > INT_MAXN)
    {
        monitor_puts("unrecognized interruptnumber\n");
        return;
    }

    switch (in)
    {
        case INT_PF:
            pagefault_handler(in, reg);
            break;
        case INT_IRQ0:
            timer_interrupt(in, reg);
            PIC_sendEOI(in-INT_IRQ0);
            break;
        case INT_IRQ7:
            spurious_irq_interrupt(in, reg);
            break;
        case INT_IRQ15:
            spurious_irq_interrupt(in, reg);
            break;
        case INT_SYSCALL:
            syscall_handler(in, reg);
            break;
        default:
            monitor_puts("recieved interrupt: ");
            monitor_put_dec(in);
            monitor_puts("\n");
            monitor_put_hex(reg->err_code);
            hlt();
            break;
    }
}
