#ifndef __PIC_H
#define __PIC_H

#define PIC1            0x20        // IO base address of master PIC
#define PIC2            0xA0        // IO base address of slave PIC
#define PIC1_CMD        PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_CMD        PIC2
#define PIC2_DATA       (PIC2+1)

#define PIC_EOI         0x20        // End of interrupt command code

#define ICW1_ICW4       0x01
#define ICW1_SINGLE     0x02
#define ICW1_INTERVAL4  0x04
#define ICW1_LEVEL      0x08
#define ICW1_INIT       0x10

#define ICW4_8086       0x01
#define ICW4_AUTO       0x02
#define ICW4_BUF_SLAVE  0x08
#define ICW4_BUF_MASTER 0x0c
#define ICW4_SFNM       0x10


void PIC_remap();
void PIC_sendEOI(int irq);
void mask_irq(int irq);
void unmask_irq(int irq);

#endif
