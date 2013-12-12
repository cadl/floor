#include <init.h>
#include <type.h>
#include <des.h>
#include <timer.h>
#include <pic.h>
#include <page.h>

void init()
{
    init_gdt();
    init_idt();
    PIC_remap();
    init_timer(500);
    init_paging();
}
