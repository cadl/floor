#ifndef PROCESS_H
#define PROCESS_H
#include <type.h>
#include <page.h>


void switch_to_user_mode(u32int addr);
void init_process0();
void process0_start();
extern void process0_setup();
u32int process_fork();


typedef struct context_struct
{
    u32int eip;
    u32int esp;
    u32int ebx;
    u32int ecx;
    u32int edx;
    u32int esi;
    u32int edi;
    u32int ebp;
} context_t;

typedef struct proc_struct
{
    u32int pid;
    u32int state;
    context_t context;
    page_directory_t *page_directory;
    struct proc_struct *next;
} proc_t;

#endif
