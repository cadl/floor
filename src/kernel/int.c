#include "type.h"
#include "int.h"
#include "monitor.h"
#include "pic.h"
#include "timer.h"
#include "asm/system.h"


void PIC_remap()
{
    //u8int pd1, pd2;

    //pd1 = inb(PIC1_DATA);
    //pd2 = inb(PIC2_DATA);
    outb(PIC1_CMD, ICW1_INIT|ICW1_ICW4);
    outb(PIC2_CMD, ICW1_INIT|ICW1_ICW4);
    outb(PIC1_DATA, INT_IRQ0);
    outb(PIC2_DATA, INT_IRQ8);
    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);
    outb(PIC1_DATA, 0x05);
    outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA, 0xf8);
    outb(PIC2_DATA, 0xff);
    //outb(PIC1_DATA, pd1);
    //outb(PIC2_DATA, pd2);
}

void mask_irq(int irq)
{
    u16int port;
    u8int v;

    if (irq < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        irq -= 8;
    }
    v = inb(port) | (1 << irq);
    outb(port, v);
}

void unmask_irq(int irq)
{
    u16int port;
    u8int v;
    
    if (irq < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        irq -= 8;
    }
    v = inb(port) & ~(1 << irq);
    outb(port, v);
}

void interrupt_handler(int in, registers_t *reg)
{
    if (in < 0 || in > INT_MAXN)
    {
        monitor_puts("unrecognized interruptnumber\n");
        return;
    }

    switch (in)
    {
        case INT_IRQ0:
            timer_interrupt(reg);
            break;
        default:
            monitor_puts("recieved interrupt: ");
            monitor_put_dec(in);
            monitor_puts("\n");
            break;
    }
    if (INT_IRQ0 <= in <= INT_IRQ15)
    {
        PIC_sendEOI(in-INT_IRQ0);
    }
}

void PIC_sendEOI(int irq)
{
    if (irq >= 8)
    {
        outb(PIC2_CMD, PIC_EOI);
    }
    else
    {
        outb(PIC2_CMD, PIC_EOI);
    }
}
