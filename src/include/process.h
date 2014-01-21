#ifndef PROCESS_H
#define PROCESS_H
#include <type.h>

void switch_to_user_mode(u32int addr, u32int p0_addr);
void init_process0();
void process0_start();
extern void process0_setup();
u32int fork();

#endif
