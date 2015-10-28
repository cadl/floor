#include <syscall.h>
#include <timer.h>
#include <usr/game.h>
#include <usr/stdio.h>


int snake()
{
    timer_t *timer;
    int direct = 1;
    int max_x = 70, min_x = 10, max_y = 20, min_y = 2;
    int x = 11, y = 19;
    int i, j, k;
    u8int c;

    clear();

    for (k=0; k < 60 * 23; k++)
    {
        i = k % 60;
        j = k / 60;

        if (i == 0 && min_y+j<max_y)
        {
            syscall_monitor_putc_at('|', min_x, min_y+j, 0, 15);
            syscall_monitor_putc_at('|', max_x, min_y+j, 0, 15);
        }
        if (j == 0 && min_x+i<max_x)
        {
            syscall_monitor_putc_at('-', min_x+i, min_y, 0, 15);
            syscall_monitor_putc_at('-', min_x+i, max_y, 0, 15);
        }
    }


    syscall_alloc_timer(&timer, 500);

    for(;;)
    {
        syscall_wait_timer(timer);

        c = getch_nonblock();

        if (c == 'w')
        {
            if (direct % 2 == 0)
                direct = 1;
        }
        else if (c == 'd')
        {
            if (direct % 2 != 0)
                direct = 2;
        }
        else if (c == 's')
        {
            if (direct % 2 == 0)
                direct = 3;
        }
        else if (c == 'a')
        {
            if (direct % 2 != 0)
                direct = 4;
        }
        else if (c == 'q')
        {
            clear();
            return 0;
        }

        syscall_monitor_putc_at(' ', x, y, 0, 15);

        switch (direct)
        {
        case 1:
            y -= 1;
            break;
        case 2:
            x += 1;
            break;
        case 3:
            y += 1;
            break;
        case 4:
            x -= 1;
            break;
        }

        if (x>=max_x || x <= min_x || y >= max_y || y <= min_y)
        {
            clear();
            putch('e');
            putch('n');
            putch('d');
            putch('\n');
            getch();
            return 0;
        }

        syscall_monitor_putc_at('@', x, y, 0, 15);
    }

}
