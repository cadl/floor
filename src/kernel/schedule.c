#include <process.h>
#include <schedule.h>
#include <asm/system.h>
#include <memory.h>
#include <string.h>
#include <monitor.h>


void task_list_append(proc_t **list_head, proc_t *task)
{
    proc_t *tmp_task = *list_head;
    if (tmp_task)
    {
        while (tmp_task->next != *list_head)
        {
            tmp_task = tmp_task->next;
        }
        tmp_task->next = task;
        task->next = *list_head;
    }
    else
    {
        *list_head = task;
        task->next = *list_head;
    }
}

proc_t* task_list_remove(proc_t **list_head, proc_t *task)
{
    return 0;
}

void schedule()
{
    proc_t *next_task, *prev_task;
    u32int ret;

    next_task = current_task->next;
    prev_task = current_task;

    monitor_put_hex(current_task->pid);
    if (next_task != current_task)
    {
        current_task = current_task->next;
        current_page_directory = current_task->page_directory;
        ret = context_switch(&(next_task->context), &(prev_task->context), next_task->page_directory->phy_addr);
    }
}

void init_task()
{
    cli();
    ntasks = 1;
    current_task = (proc_t *)kmalloc(sizeof(proc_t));
    memset(current_task, 0, sizeof(proc_t));
    current_task->pid = ntasks;
    current_task->page_directory = current_page_directory;
    ntasks++;
    task_list_append(&task_ready_list_head, current_task);
    sti();
}

u32int pause()
{
    if (current_task->next != current_task)
    {
        schedule();
    }
    return 0;
}
