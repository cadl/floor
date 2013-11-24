#include "monitor.h"


u16int *video_memory = (u16int *)0xB8000;

u8int cursor_x = 0;
u8int cursor_y = 0;

static void move_cursor()
{
    
}

void monitor_putc(char c)
{
    u8int backColor = 0;
    u8int foreColor = 15;

    u8int attrByte = (backColor << 4) | (foreColor & 0x0F);
    u16int attr = attrByte << 8;
    u16int *location;

    switch (c)
    {
        case '\b':
            if (cursor_x)
            {
                cursor_x -= 1;
            }
            break;
        case '\t':
            cursor_x = (cursor_x + 8) & (8 - 1);
            break;
        case '\r':
            cursor_x = 0;
            break;
        case '\n':
            cursor_x = 0;
            cursor_y += 1;
            break;
        default:
            if (c >= ' ')
            {
                location = video_memory + (cursor_y*80 + cursor_x);
                *location = (c | attr);
                cursor_x += 1;
            }
            break;
    }
    if (cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y += 1;
    }
}


void monitor_clear()
{
    u8int attrByte = (0 << 4 | (15 & 0x0F));
    u16int blank = 0x20;

    int i;
    for (i=0; i<80*25; i++)
    {
        *(video_memory + i) = blank;
    }
    cursor_x = 0;
    cursor_y = 0;
}


void monitor_puts(char *c)
{
    int i;
    for (i=0; c[i]; i++)
    {
        monitor_putc(c[i]);
    }
}
