#include <type.h>
#include <monitor.h>
#include <init.h>
#include <sys.h>
#include <asm/system.h>
#include <task.h>


int kernel_start(u32int a)
{
    u32int pid, p;
    init();
    monitor_put_hex(a);
    monitor_puts("hello\n");
    __asm__ volatile ("movl %0, %%esp":: "r"((u32int)STACK_ESP));
    monitor_puts("hi\n");

    pid = fork();

    if (pid == 0)
    {
        p = fork();
        if (p == 0)
        {
            monitor_puts("in grandson\n");
            while (1)
            {
                hlt();
            }
        }
        else
        {
            monitor_puts("e e e\n");
            while (1)
            {
                hlt();
            }
        }
        monitor_puts("in child\n");
        while (1)
        {
            hlt();
        }
    }
    else
    {
        monitor_puts("in parent\n");
        while (1)
        {
            hlt();
        }
    }

    while (1)
    {
        monitor_puts("hlt\n");
        hlt();
    }
    return 0x42;
}
