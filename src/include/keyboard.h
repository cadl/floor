#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#define KEYBOARD_BUFFER_SIZE 0x1000

#include <type.h>

char *keyboard_buffer;
u32int keyboard_buffer_index;
void init_keyboard();
void keyboard_interrupt(int in, registers_t *preg);
char read_keyboard();
char read_keyboard_nonblock();

#endif
