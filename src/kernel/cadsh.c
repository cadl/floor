#include <process.h>
#include <sys.h>
#include <usr/stdio.h>
#include <usr/cadsh.h>
#include <syscall.h>


void cadsh_init()
{
    char c;
    char s[4] = {'$', '>', ' ', 0};

    puts(s);
    for (;;)
    {
        c = getch();

        putch(c);
        if (c == '\n')
        {
            puts(s);
        }
    }
}
