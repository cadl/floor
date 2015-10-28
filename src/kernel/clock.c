#include <syscall.h>
#include <timer.h>
#include <usr/stdio.h>


void show_clock()
{
    timer_t *t;
    syscall_alloc_timer(&t, 1000);

    for (;;)
    {
        syscall_wait_timer(t);
        putch('t');
    }
}
