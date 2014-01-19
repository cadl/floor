#include <process.h>
#include <page.h>
#include <memory.h>
#include <sys.h>
#include <task.h>
#include <syscall.h>
#include <monitor.h>
#include <asm/system.h>

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
    monitor_puts("e e e \n");
    monitor_put_hex((u32int)process0_setup + (u32int)USER_CODE_START - (u32int)KERNEL_SPACE_START);
    switch_to_user_mode((u32int)process0_setup + (u32int)USER_CODE_START - (u32int)KERNEL_SPACE_START,
                        (u32int)process0_start + (u32int)USER_CODE_START - (u32int)KERNEL_SPACE_START);
}

void process0_start()
{
    syscall_monitor_puts("hello user mode\n");
    syscall_monitor_puts("aaa\n");
}
