#ifndef __TIMER_H
#define __TIMER_H

#include <type.h>
#include <process.h>

typedef struct timer
{
    u32int ticks;
    u32int t;
    u32int state;
    proc_t *wait_process;
} timer_t;


timer_t user_timer[64];

void wait_timer(timer_t *t);
void init_timer(u32int frequency);
void timer_interrupt(int in, registers_t *preg);
void alloc_timer(timer_t **t, u32int interval);
#endif
