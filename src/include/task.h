#ifndef __TASK_H
#define __TASK_H

#include <type.h>
#include <page.h>

typedef struct task_struct
{
    int id;
    u32int esp, ebp;
    u32int eip;
    page_directory_t *page_directory;
    struct task_struct *next;
} task_t;

typedef struct tss_entry_struct tss_entry_t;

extern u32int get_eip();
void init_task();
void task_switch();
void switch_to_user_mode(u32int addr, u32int p0_addr);


u32int fork();

task_t *current_task;
task_t *tasks_head;
u32int ntasks;

#endif
