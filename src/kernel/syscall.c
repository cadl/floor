#include <syscall.h>
#include <monitor.h>
#include <process.h>
#include <schedule.h>
#include <keyboard.h>
#include <timer.h>


void *syscalls[SYSCALL_NUM];
u32int ntasks;


void init_syscall()
{
    syscalls[0] = monitor_putc;
    syscalls[1] = process_fork;
    syscalls[2] = pause;
    syscalls[3] = read_keyboard;
    syscalls[4] = alloc_timer;
    syscalls[5] = wait_timer;
    syscalls[6] = monitor_putc_at;
    syscalls[7] = read_keyboard_nonblock;
    syscalls[8] = monitor_clear;
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

DEF_SYSCALL1(monitor_putc, 0, char);
DEF_SYSCALL0(fork, 1);
DEF_SYSCALL0(pause, 2);
DEF_SYSCALL0(read_keyboard, 3);
DEF_SYSCALL2(alloc_timer, 4, timer_t**, u32int);
DEF_SYSCALL1(wait_timer, 5, timer_t*);
DEF_SYSCALL5(monitor_putc_at, 6, char, u32int, u32int, u8int, u8int);
DEF_SYSCALL0(read_keyboard_nonblock, 7);
DEF_SYSCALL0(monitor_clear, 8);
