#include <syscall.h>
#include <timer.h>
#include <usr/stdio.h>


void show_clock()
{
    u32int ticks = 0;
    char hour = 0, minute = 0, sec = 0;
    timer_t *t;
    char s[9];
    u8int i;

    syscall_alloc_timer(&t, 1000);

    for (;;)
    {
        syscall_wait_timer(t);
        ticks += 1;
        hour = ticks / 3600 % 24;
        minute = ticks / 60 % 60;
        sec = ticks % 60;
        s[0] = '0' + (hour / 10);
        s[1] = '0' + (hour % 10);
        s[2] = ':';
        s[3] = '0' + (minute / 10);
        s[4] = '0' + (minute % 10);
        s[5] = ':';
        s[6] = '0' + (sec / 10);
        s[7] = '0' + (sec % 10);
        s[8] = 0;

        for (i=0; s[i]; i++)
            syscall_monitor_putc_at(s[i], 70+i, 3, 0, 0xa);
    }
}
