#include <monitor.h>
#include <type.h>
#include <asm/system.h>


u16int *video_memory = (u16int *)0xb8000;
u8int cursor_x = 0;
u8int cursor_y = 0;


void move_cursor()
{
    u16int cursorLocation = cursor_y * 80 + cursor_x;
    outb(0x3d4, 14);
    outb(0x3d5, cursorLocation>>8);
    outb(0x3d4, 15);
    outb(0x3d5, cursorLocation);
}


void scroll()
{
    u8int attrByte = (0 << 4) | (15 &0x0F);
    u16int blank = 0x20 | (attrByte << 8);

    if (cursor_y >= 25)
    {
        int i;
        for (i=0; i<24*80; i++)
        {
            video_memory[i] = video_memory[i+80];
        }
        for (i=24*80; i<25*80; i++)
        {
            video_memory[i] = blank;
        }
        cursor_y = 24;
    }
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
            cursor_y += (cursor_x + 8) / 80;
            cursor_x = (cursor_x + 8) % 80;
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
    scroll();
    move_cursor();
}


void monitor_clear()
{
    u8int attrByte = (0 << 4 | (15 & 0x0F));
    u16int blank = 0x20 | (attrByte << 8);

    int i;
    for (i=0; i<80*25; i++)
    {
        video_memory[i] = blank;
    }
    cursor_x = 0;
    cursor_y = 0;

    move_cursor();
}


void monitor_puts(char *c)
{
    int i;
    for (i=0; c[i]; i++)
    {
        monitor_putc(c[i]);
    }
}


void monitor_put_hex(u32int n)
{
    int i=14, remain;
    char digit[16];

    digit[15] = '\0';
    digit[i] = '0' + n % 16;
    if (digit[i] > '9')
    {
        digit[i] = 'A' + (digit[i] - '9' - 1);
    }
    remain = n / 16;

    while (remain)
    {
        i--;
        digit[i] = '0' + remain %16;
        if (digit[i] > '9')
        {
            digit[i] = 'A' + (digit[i] - '9' - 1);
        }
        remain = remain / 16;
    }
    digit[i-1] = 'x';
    digit[i-2] = '0';
    monitor_puts(digit+i-2);
    monitor_putc('\n');
}


void monitor_put_dec(u32int n)
{
    int i=14, remain;
    char digit[16];

    digit[15] = '\0';
    digit[i] = '0' + n % 10;
    remain = n / 10;

    while(remain)
    {
        i --;
        digit[i] = '0' + remain % 10;
        remain /= 10;
    }
    monitor_puts(digit+i);
    monitor_putc('\n');
}


void panic(char *s)
{
    monitor_puts(s);
    while (1)
    {
        hlt();
    }
}
