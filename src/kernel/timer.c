#include <timer.h>
#include <monitor.h>
#include <type.h>
#include <asm/system.h>
#include <pic.h>
#include <int.h>
#include <schedule.h>

u32int sys_tick = 0;
u32int user_timer_count = 0;

void init_timer(u32int frequency)
{
    u32int i;

    sys_tick = 0;
    u32int divisor = 1193180 / frequency;
    outb(0x43, 0x34);

    u8int l = (u8int)(divisor & 0xFF);
    u8int h = (u8int)((divisor >> 8) & 0xFF);
    outb(0x40, l);
    outb(0x40, h);

    unmask_irq(IRQ_TIMER);

    for (i=0; i<64; i++)
    {
        user_timer[i].ticks = 0;
        user_timer[i].t = 0;
        user_timer[i].state = 0;
        user_timer[i].wait_process = 0;
    }
}


void timer_interrupt(int in, registers_t *preg)
{
    u32int i;

    sys_tick++;

    for (i=0; i<64; i++)
    {
        if (user_timer[i].state == 0)
            continue;

        user_timer[i].ticks += 1;
        if (user_timer[i].ticks >= user_timer[i].t)
        {
            user_timer[i].ticks = 0;
            user_timer[i].state = 0;
            wake_up(&(user_timer[i].wait_process));
        }
    }

    if (sys_tick == (u32int)-1)
    {
        monitor_puts("bee\n");
        sys_tick = 0;
    }


    if (sys_tick % 100000 == 0)
    {
        schedule();
    }
}


void wait_timer(timer_t *t)
{
    t->ticks = 0;
    t->state = 1;
    sleep_on(&(t->wait_process));
    return;
}


void alloc_timer(timer_t **timer, u32int interval)
{
    u32int i;
    for (i=0; i<64; i++)
    {
        if (user_timer[i].state == 0)
        {
            user_timer[i].t = interval;
            *timer = &(user_timer[i]);
            return;
        }
    }
}
