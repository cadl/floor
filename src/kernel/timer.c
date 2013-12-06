#include "timer.h"
#include "monitor.h"
#include "type.h"
#include "asm/system.h"

u32int tick = 0;

void init_timer(u32int frequency)
{
    tick = 0;
    u32int divisor = 1193180 / frequency;
    outb(0x43, 0x36);

    u8int l = (u8int)(divisor & 0xFF);
    u8int h = (u8int)((divisor >> 8) & 0xFF);
    outb(0x40, l);
    outb(0x40, h);
}
void timer_interrupt(registers_t *preg)
{
    tick++;
    monitor_puts("timer fired\n");
    monitor_put_dec(tick);
}
