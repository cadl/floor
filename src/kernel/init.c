#include <init.h>
#include <type.h>
#include <des.h>
#include <timer.h>
#include <pic.h>
#include <page.h>
#include <monitor.h>
#include <syscall.h>
#include <schedule.h>
#include <asm/system.h>
#include <process.h>

void init()
{
    cli();
    monitor_clear();
    init_gdt();
    monitor_puts("init gdt over\n");
    init_idt();
    monitor_puts("init idt over\n");
    init_paging();
    monitor_puts("init paging over\n");
    PIC_remap();
    init_timer(500);
    monitor_puts("init timer over\n");
    init_syscall();
    monitor_puts("init syscall over\n");
    init_process0();
    monitor_puts("init process0 over\n");
    sti();
}
