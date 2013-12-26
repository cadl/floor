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

extern void get_eip();
void init_task();
void task_switch();
 

task_t *current_task;

#endif
