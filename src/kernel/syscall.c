#include <syscall.h>
#include <monitor.h>
#include <process.h>
#include <schedule.h>

void *syscalls[SYSCALL_NUM];

void init_syscall()
{
    syscalls[0] = monitor_puts;
    syscalls[1] = process_fork;
    syscalls[2] = pause;
}

void syscall_handler(u32int in, registers_t *reg)
{
    if (reg->eax >= SYSCALL_NUM)
    {
        return;
    }

    void *func = syscalls[reg->eax];
    u32int ret;
    __asm__ volatile("          \
            pushl %1;           \
            pushl %2;           \
            pushl %3;           \
            pushl %4;           \
            pushl %5;           \
            call *%6;           \
            addl $20, %%esp;    \
            " : "=a"(ret): "r"(reg->edi), "r"(reg->esi),
           "r"(reg->edx), "r"(reg->ecx), "r"(reg->ebx), "r"(func));
    reg->eax = ret;
}


DEF_SYSCALL1(monitor_puts, 0, char*);
DEF_SYSCALL0(fork, 1);
DEF_SYSCALL0(pause, 2);
