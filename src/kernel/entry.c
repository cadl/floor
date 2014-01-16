#include <type.h>
#include <monitor.h>
#include <init.h>
#include <sys.h>
#include <asm/system.h>
#include <task.h>
#include <syscall.h>


int kernel_start(u32int a)
{
    init();
    monitor_put_hex(a);
    monitor_puts("hello\n");
    __asm__ volatile ("movl %0, %%esp":: "r"((u32int)(KERNEL_STACK_TOP)));
    monitor_puts("hi\n");

    syscall_monitor_puts("aaa\n");
    while (1)
    {
        hlt();
    }
    return 0x42;
}
