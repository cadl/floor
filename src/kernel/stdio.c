#include <syscall.h>
#include <usr/stdio.h>


void puts(char *s)
{
    int i;
    for(i=0; s[i]; i++)
    {
        syscall_monitor_putc(s[i]);
    }
}


void putch(char c)
{
    syscall_monitor_putc(c);
}


char getch()
{
    return syscall_read_keyboard();
}
