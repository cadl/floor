#ifndef __TIMER_H
#define __TIMER_H

#include "type.h"

void init_timer(u32int frequency);
void timer_interrupt(int in, registers_t *preg);
#endif
