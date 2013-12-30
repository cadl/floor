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
    __asm__ volatile ("int $3");
    monitor_puts("hello\n");
    monitor_put_hex(*((u32int *)1300000));
    __asm__ volatile ("movl %0, %%esp":: "r"((u32int)STACK_ESP));
    monitor_puts("hi\n");
    
    /*pid = fork();
    if (pid == 0)
    {
        monitor_puts("in child process\n");
    }
    else
    {
        monitor_puts("in parent process\n");
    }*/

    while (1)
    {
        hlt();
    }

    return 0x42;
}
