#include <process.h>
#include <page.h>
#include <memory.h>
#include <sys.h>
#include <syscall.h>
#include <string.h>
#include <pic.h>
#include <int.h>
#include <schedule.h>
#include <process.h>
#include <asm/system.h>


void switch_to_user_mode(u32int addr)
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
            iret;":: "r"(addr): "%eax");
}

void init_process0()
{
    // process0
    // After some basic system initialization, system create the No.0 process, and switch to user mode from kernel mode. Every proccess fork from No.0.
    // This function aims to create No.0 process.

    u32int i, j;
    page_t *kernel_code_page, *user_code_page, *tmp_page;
    cli();
    current_page_directory = clone_directory(kernel_page_directory);

    for (i=(u32int)KERNEL_SPACE_START, j=(u32int)USER_CODE_START; i<(u32int)KERNEL_HEAP_END; i+=0x1000, j+=0x1000)
    {
        kernel_code_page = get_page(i, 1, 0, kernel_page_directory);
        user_code_page = get_page(j, 0, 1, current_page_directory);
        user_code_page->present = 1;
        user_code_page->frame = kernel_code_page->frame;
        user_code_page->rw = 1;
        user_code_page->user = 1;
    }
    for (i=(u32int)KERNEL_STACK_BOTTOM; i<(u32int)KERNEL_STACK_TOP; i+=0x1000)
    {
        tmp_page = get_page(i, 1, 1, current_page_directory);
        alloc_frame(tmp_page, 1, 1);
    }
    for (i=(u32int)USER_STACK_BOTTOM; i<(u32int)USER_STACK_TOP; i+=0x1000)
    {
        tmp_page = get_page(i, 0, 1, current_page_directory);
        alloc_frame(tmp_page, 0, 1);
    }
    init_task();
    switch_page_directory(current_page_directory);
    switch_to_user_mode((u32int)process0_setup + (u32int)USER_CODE_START - (u32int)KERNEL_SPACE_START);
}

void process0_start()
{
    u32int pid;
    syscall_monitor_puts("hello user mode\n");
    pid = syscall_fork();
    if (pid == 0)
    {
        u32int ppid = syscall_fork();
        if (ppid == 0)
        {
            for (;;)
                syscall_monitor_puts("grand child\n");
        }
        else
        {
            for (;;)
            {
                syscall_monitor_puts("child\n");
//                syscall_pause();
            }
        }
    }
    else
    {
        for (;;)
        {
            syscall_monitor_puts("parent\n");
//            syscall_pause();
        }
    }
    for (;;)
    {
        syscall_pause();
    }
}

u32int process_fork()
{
    u32int ret;
    proc_t *new_task;
    context_t *cxt;
    cli();
    new_task = (proc_t *)kmalloc(sizeof(proc_t));
    memset(new_task, 0, sizeof(proc_t));

    task_list_append(&task_ready_list_head, new_task);

    cxt = &(new_task->context);

    ret = save_context(cxt);

    if (ret != 0x19910611)
    {
        new_task->page_directory = clone_directory(current_page_directory);
        new_task->pid = ntasks;
        ntasks++;
        return new_task->pid;
    }
    else
    {
        PIC_sendEOI(32-INT_IRQ0);
        return 0;
    }
}
