#ifndef SCHEDULE_H
#define SCHEDULE_H
#define TASKNUM 0x20

#include <process.h>

extern u32int ntasks;

proc_t *task_ready_list_head;
proc_t *task_sleep_list_head;
proc_t *current_task;

void task_list_append(proc_t **list_head, proc_t *task);
proc_t* task_list_remove(proc_t **list_head, proc_t *task);

void schedule();
void init_task();
u32int pause();

extern u32int context_switch(context_t *next_context, context_t *prev_context, u32int pd_phyaddr);
extern u32int save_context(context_t *cxt);
void sleep_on(proc_t **p);
void wake_up(proc_t **p);
#endif
