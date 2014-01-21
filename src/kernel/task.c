#include <task.h>
#include <type.h>
#include <memory.h>
#include <monitor.h>
#include <string.h>
#include <page.h>
#include <asm/system.h>
#include <int.h>
#include <pic.h>


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

void task_switch()
{
    u32int esp, ebp, eip;

    //monitor_puts("in task ");
    //monitor_put_dec(current_task->id);
    if (current_task && (current_task->next != current_task))
    {
        __asm__ volatile ("mov %%esp, %0": "=r"(esp));
        __asm__ volatile ("mov %%ebp, %0": "=r"(ebp));
        eip = get_eip();
        if (eip == 0x12345)
        {
            return;
        }
        current_task->esp = esp;
        current_task->ebp = ebp;
        current_task->eip = eip;
        current_task = current_task->next;
        switch_page_directory(current_task->page_directory);

        __asm__ volatile("              \
                cli;                    \
                mov %0, %%ecx;          \
                mov %1, %%esp;          \
                mov %2, %%ebp;          \
                mov $0x12345, %%eax;    \
                sti;                    \
                jmp *%%ecx;"
                : : "r"(current_task->eip),
                "r"(current_task->esp),
                "r"(current_task->ebp): "%ecx");
    }
}
