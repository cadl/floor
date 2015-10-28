#include <process.h>
#include <sys.h>
#include <string.h>
#include <usr/stdio.h>
#include <usr/cadsh.h>
#include <usr/game.h>
#include <syscall.h>


void cadsh_init()
{
    char c;
    char input[80];
    int index=0;
    char s[4] = {'$', '>', ' ', 0};

    puts(s);
    for (;;)
    {
        c = getch();
        input[index++] = c;
        putch(c);

        if (c == '\n')
        {
            if (input[0] == 's' && index == 2)
            {
                snake();
            }
            input[index] = 0;
            index = 0;
            puts(s);
        }
    }
}
