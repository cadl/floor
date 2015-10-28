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
    proc_t *tmp_task = *list_head;
    if (tmp_task)
    {
        while (tmp_task->next != task)
        {
            tmp_task = tmp_task->next;
            if (tmp_task == *list_head)
            {
                // don't have this task
                return 0;
            }
        }
        if (task == *list_head)
        {
            *list_head = task->next==*list_head? 0: tmp_task;
        }
        tmp_task->next = task->next;

        return task;
    }
    else
    {
        // the list is null
        return 0;
    }
}

void schedule()
{
    proc_t *next_task, *prev_task, *tmp_task;

    // monitor_puts("schedule\n");
    tmp_task = current_task->next;

    next_task = 0;
    prev_task = 0;

    while (tmp_task != current_task)
    {
        if (tmp_task->state == 0)
        {
            next_task = tmp_task;
            break;
        }
        tmp_task = tmp_task->next;
    }

    if (next_task)
    {
        prev_task = current_task;
        current_task = next_task;
        current_page_directory = current_task->page_directory;
        context_switch(&(next_task->context), &(prev_task->context), next_task->page_directory->phy_addr);
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
    schedule();
    return 0;
}


void sleep_on(proc_t **p)
{
    proc_t *tmp;

    if (!p)
        return;

    tmp = *p;
    *p = current_task;
    current_task->state = 1;
    schedule();
    if (tmp)
        tmp->state = 0;
}


void wake_up(proc_t **p)
{
    if (p && *p)
    {
        (**p).state = 0;
        *p = 0;
    }
}
