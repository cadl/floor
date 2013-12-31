#include <init.h>
#include <type.h>
#include <des.h>
#include <timer.h>
#include <pic.h>
#include <page.h>
#include <monitor.h>
#include <task.h>
#include <asm/system.h>

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
    init_task();
    monitor_puts("init task over\n");
    sti();
}
