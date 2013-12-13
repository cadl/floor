#include <type.h>
#include <pic.h>
#include <asm/system.h>
#include <int.h>


void PIC_remap()
{
    outb(PIC1_CMD, ICW1_INIT+ICW1_ICW4);
    outb(PIC2_CMD, ICW1_INIT+ICW1_ICW4);
    outb(PIC1_DATA, INT_IRQ0);
    outb(PIC2_DATA, INT_IRQ8);
    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);
    outb(PIC1_DATA, 0xff);          // mask all irq at start
    outb(PIC2_DATA, 0xff);
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

void PIC_sendEOI(int irq)
{
    if (irq >= 8)
    {
        outb(PIC2_CMD, PIC_EOI);
    }
    outb(PIC1_CMD, PIC_EOI);
}

u16int PIC_get_irq_reg(int ocw3)
{
    outb(PIC1_CMD, ocw3);
    outb(PIC2_CMD, ocw3);
    return (inb(PIC2_CMD) << 8) | inb(PIC1_CMD);
}

u16int PIC_get_irr(void)
{
    return PIC_get_irq_reg(PIC_READ_IRR);
}

u16int PIC_get_isr(void)
{
    return PIC_get_irq_reg(PIC_READ_ISR);
}

void spurious_irq_interrupt(int in, registers_t *reg)
{
    int isr = PIC_get_isr();
    if (isr & (0x01 << in))
    {
        // real irq interrupt
        // pass to handler. later
    }
    else
    {
        if (in == 15)
        {
            PIC_sendEOI(in);
        }
    }
}
