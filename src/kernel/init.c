#include <init.h>
#include <type.h>
#include <des.h>
#include <timer.h>
#include <keyboard.h>
#include <pic.h>
#include <page.h>
#include <monitor.h>
#include <syscall.h>
#include <schedule.h>
#include <process.h>
#include <asm/system.h>

void init()
{
    monitor_clear();
    welcome();
    monitor_puts("INIT GDT... \n");
    init_gdt();
    monitor_puts("INIT IDT...\n");
    init_idt();
    monitor_puts("INIT PAGING...\n");
    init_paging();
    PIC_remap();
    monitor_puts("INIT TIMER...\n");
    init_timer(100);
    monitor_puts("INIT KEYBOARD...\n");
    init_keyboard();
    monitor_puts("INIT SYSTEM CALL...\n");
    init_syscall();
    monitor_puts("INIT PROCESS...\n");
    init_process0();
}

void welcome()
{
    monitor_puts("+==========================================================+\n");
    monitor_puts("|                     _____       _____                    |\n");
    monitor_puts("|                    |  _  |     |  _  |                   |\n");
    monitor_puts("|                    |_| |_|     |_| |_|                   |\n");
    monitor_puts("|                                                          |\n");
    monitor_puts("|                           _____                          |\n");
    monitor_puts("|                          |_____|                         |\n");
    monitor_puts("|                                                          |\n");
    monitor_puts("+==========================================================+\n");
    monitor_puts("\n\n\n");
}
