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
    if (eip == 0x12345)
    {
        PIC_sendEOI(32-INT_IRQ0);
    }
    if (current_task == parent_task)
    {
        cli();
        __asm__ volatile ("mov %%esp, %0": "=r"(esp));
        __asm__ volatile ("mov %%ebp, %0": "=r"(ebp));
        new_task->esp = esp;
        new_task->ebp = ebp;
        new_task->eip = eip;
        new_task->id = ntasks;
        new_task->page_directory = clone_directory(current_page_directory);
        ntasks++;
        sti();
        return new_task->id;
    }
    else
    {
        // in child process
        sti();
        return 0;
    }
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

void switch_to_user_mode(u32int addr, u32int p0_addr)
{
    __asm__ volatile("          \
            cli;                \
            mov $0x23, %%ax;    \
            mov %%ax, %%ds;     \
            mov %%ax, %%es;     \
            mov %%ax, %%fs;     \
            mov %%ax, %%gs;     \
            mov %%esp, %%eax;   \
            pushl $0x23;        \
            pushl %%eax;        \
            pushf;              \
            pop %%eax;          \
            orl %%eax, 0x200;   \
            push %%eax;          \
            pushl $0x1b;        \
            pushl %0;           \
            movl %0, %%eax;       \
            iret;":: "r"(addr), "r"(p0_addr): "%eax");
}
