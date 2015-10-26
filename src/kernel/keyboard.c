#include <keyboard.h>
#include <type.h>
#include <int.h>
#include <asm/system.h>
#include <monitor.h>
#include <memory.h>
#include <schedule.h>
#include <process.h>


static proc_t *wait_keyboard = 0;


unsigned char kbdus[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
    '9', '0', '-', '=', '\b',	/* Backspace */
    '\t',			/* Tab */
    'q', 'w', 'e', 'r',	/* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
    '\'', '`',   0,		/* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
    'm', ',', '.', '/',   0,				/* Right shift */
    '*',
    0,	/* Alt */
    ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
    '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
    '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};


void init_keyboard()
{
    keyboard_buffer = kmalloc(KEYBOARD_BUFFER_SIZE);
    keyboard_buffer_index = 0;
    unmask_irq(IRQ_KEYBOARD);
}


void keyboard_interrupt(int in, registers_t *preg)
{
    u8int scancode;
    scancode = inb(0x60);

    if (!(scancode & 0x80))
    {
        keyboard_buffer[keyboard_buffer_index++] = kbdus[scancode];
        wake_up(&wait_keyboard);
    }
}


char read_keyboard()
{
    char c;
    u32int i;

    if (keyboard_buffer_index == 0)
    {
        sleep_on(&wait_keyboard);
    }

    c = keyboard_buffer[0];
    for (i=0; i<keyboard_buffer_index-1; i++) {
        keyboard_buffer[i] = keyboard_buffer[i+1];
    }
    keyboard_buffer_index -= 1;
    return c;
}
