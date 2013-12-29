#include <task.h>
#include <type.h>
#include <page.h>
#include <memory.h>
#include <monitor.h>
#include <asm/system.h>


void init_task()
{
    cli();
    ntasks = 1;
    current_task = (task_t *)kmalloc(sizeof(task_t));
    current_task->id = ntasks;
    current_task->next = 0;
    current_task->eip = 0;
    current_task->esp = 0;
    current_task->ebp = 0;
    current_task->page_directory = current_page_directory;
    tasks_head = current_task;
    current_task->next = current_task;
    ntasks++;
    sti();
}

u32int fork()
{
    u32int eip, ebp, esp;
    task_t *new_task, *tmp_task, *parent_task;
    cli();
    parent_task = current_task;
    new_task = (task_t *)kmalloc(sizeof(task_t));
    memset(new_task, 0, sizeof(task_t));
    tmp_task = tasks_head;
    while (tmp_task->next != tasks_head)
    {
        tmp_task = tmp_task->next;
    }
    tmp_task->next = new_task;
    new_task->next = tasks_head;
    eip = get_eip();
    if (current_task == parent_task)
    {
        __asm__ volatile ("mov %%esp, %0": "=r"(esp));
        __asm__ volatile ("mov %%ebp, %0": "=r"(ebp));
        new_task->esp = esp;
        new_task->ebp = ebp;
        new_task->eip = eip;
        new_task->id = ntasks;
        new_task->page_directory = clone_page_directory(current_page_directory);
        ntasks++;
        sti();
        return new_task->id;
    }
    else
    {
        // in child process
        return 0;
    }
}


void task_switch()
{
    u32int esp, ebp, eip;          
    page_directory_t *pd;

    cli();
    if (current_task && (current_task->next/*k = current_task*/))
    { 
        __asm__ volatile ("mov %%esp, %0": "=r"(esp));
        __asm__ volatile ("mov %%ebp, %0": "=r"(ebp));
        eip = get_eip();
        if (eip == 0x12345)
        {
            sti();
            return;
        }
        current_task->esp = esp;
        current_task->ebp = ebp;
        current_task->eip = eip;
        current_task = current_task->next;
        eip = current_task->eip;
        esp = current_task->esp;
        ebp = current_task->ebp;
        pd = current_task->page_directory;
        
        switch_page_directory(pd);
        __asm__ volatile("         \
                cli;                 \
                mov %0, %%ecx;       \
                mov %1, %%esp;       \
                mov %2, %%ebp;       \
                mov $0x12345, %%eax; \
                sti;                 \
                jmp *%%ecx           "
                : : "r"(eip), "r"(esp), "r"(ebp): "%ecx");
    }
    sti();
}
