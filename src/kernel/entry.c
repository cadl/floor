#include <type.h>
#include <monitor.h>
#include <init.h>
#include <sys.h>
#include <asm/system.h>
#include <task.h>


int kernel_start(u32int a)
{
    u32int pid;
    init();
    monitor_put_hex(a);
    monitor_puts("hello\n");
    __asm__ volatile ("movl %0, %%esp":: "r"((u32int)STACK_ESP));
    monitor_puts("hi\n");

    pid = fork();

    if (pid == 0)
    {
        while (1)
        {
            monitor_puts("in child\n");
        }
    }
    else
    {
        while (1)
        {
            monitor_puts("in parent\n");
        }
    }

    while (1)
    {
        monitor_puts("hlt\n");
        hlt();
    }
    return 0x42;
}
