#include "common.h"
#include "x86.h"

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

void init_gdt()
{
    gdt_ptr.limit = (sizeof(gdt_entry_t)*5) - 1;
    gdt_ptr.base = (u32int)&gdt_entries;

    gdt_set_des(0, 0, 0, 0, 0);                    // NULL Segment
    gdt_set_des(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);     // Code Segment
    gdt_set_des(2, 0, 0xFFFFFFFF, 0x9A, 0xCF);     // Data Segment

    gdt_load((u32int)&gdt_ptr);
}


void init_descriptor_tables()
{
    init_gdt();
}


void gdt_set_des(s32int num, u32int base, u32int limit, u8int access, u8int gran)
{
    gdt_entries[num].base_low       = (base & 0xFFFF);
    gdt_entries[num].base_middle    = (base >> 16) & 0xFF;
    gdt_entries[num].base_high      = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low      = (limit & 0xFFFF);
    gdt_entries[num].granularity    = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity    |= gran & 0xF0;
    gdt_entries[num].access         = access;
}
