#include <type.h>
#include <monitor.h>
#include <init.h>
#include <sys.h>


int kernel_start(u32int a)
{
    init();
    monitor_put_dec(a);
    __asm__ volatile ("int $3");
    monitor_puts("hello\n");
    __asm__ volatile ("movl %0, %%esp":: "r"((u32int)STACK_TOP));
    monitor_puts("hi\n");
    monitor_put_hex(0x1440000);
    monitor_puts(*(int *)(0x1440000));
    

    return 0x42;
}
