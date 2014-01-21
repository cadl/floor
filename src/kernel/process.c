#include <process.h>
#include <page.h>
#include <memory.h>
#include <sys.h>
#include <task.h>
#include <syscall.h>
#include <string.h>
#include <pic.h>
#include <int.h>
#include <asm/system.h>


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
            or $0x200, %%eax;   \
            push %%eax;          \
            pushl $0x1b;        \
            pushl %0;           \
            movl %0, %%eax;       \
            iret;":: "r"(addr), "r"(p0_addr): "%eax");
}

void init_process0()
{
    // process0
    // After some basic system initialization, system create the No.0 process, and switch to user mode from kernel mode. Every proccess fork from No.0.
    // This function aims to create No.0 process.

    u32int i, j;
    page_t *kernel_code_page, *user_code_page, *tmp_page;

    current_page_directory = clone_directory(kernel_page_directory);

    for (i=(u32int)KERNEL_SPACE_START, j=(u32int)USER_CODE_START; i<(u32int)KERNEL_HEAP_START; i+=0x1000, j+=0x1000)
    {
        kernel_code_page = get_page(i, 1, 0, kernel_page_directory);
        user_code_page = get_page(j, 0, 1, current_page_directory);
        user_code_page->present = 1;
        user_code_page->frame = kernel_code_page->frame;
        user_code_page->rw = 1;
        user_code_page->user = 1;
    }
    for (i=(u32int)USER_STACK_BOTTOM; i<(u32int)USER_STACK_TOP; i+=0x1000)
    {
        tmp_page = get_page(i, 0, 1, current_page_directory);
        alloc_frame(tmp_page, 0, 1);
    }
    switch_page_directory(current_page_directory);
    switch_to_user_mode((u32int)process0_setup + (u32int)USER_CODE_START - (u32int)KERNEL_SPACE_START,
                        (u32int)process0_start + (u32int)USER_CODE_START - (u32int)KERNEL_SPACE_START);
}

void process0_start()
{
    syscall_monitor_puts("hello user mode\n");
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
