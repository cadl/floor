#include <type.h>
#include <des.h>
#include <int.h>
#include <pic.h>
#include <timer.h>
#include <sys.h>
#include <asm/system.h>
#include <string.h>

gdt_entry_t gdt_entries[6];
gdt_ptr_t gdt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;


void init_gdt()
{
    gdt_ptr.limit = (sizeof(gdt_entry_t)*6) - 1;
    gdt_ptr.base = (u32int)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                    // NULL Segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);     // Code Segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);     // Data Segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);     // user mode code Segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);     // user mode data segment
    set_tss(5, 0x10, (u32int)KERNEL_STACK_TOP);
    gdt_load((u32int)&gdt_ptr);
    tss_load();
}

void init_idt()
{
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (u32int)&idt_entries;

    idt_set_gate((u8int)INT_DE, (u32int)de_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_NMI, (u32int)nmi_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_BP, (u32int)bp_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_OF, (u32int)of_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_BR, (u32int)br_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_UD, (u32int)ud_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_NM, (u32int)nm_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_DF, (u32int)df_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_CSO, (u32int)cso_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_TS, (u32int)ts_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_NP, (u32int)np_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_SS, (u32int)ss_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_GP, (u32int)gp_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_PF, (u32int)pf_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_MF, (u32int)mf_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_AC, (u32int)ac_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_MC, (u32int)mc_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_XM, (u32int)xm_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ0, (u32int)irq0_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ1, (u32int)irq1_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ2, (u32int)irq2_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ3, (u32int)irq3_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ4, (u32int)irq4_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ5, (u32int)irq5_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ6, (u32int)irq6_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ7, (u32int)irq7_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ8, (u32int)irq8_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ9, (u32int)irq9_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ10, (u32int)irq10_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ11, (u32int)irq11_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ12, (u32int)irq12_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ13, (u32int)irq13_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ14, (u32int)irq14_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_IRQ15, (u32int)irq15_entry, 0x08, 0x8E);
    idt_set_gate((u8int)INT_SYSCALL, (u32int)syscall_entry, 0x08, 0xEE);
    lidt((u32int)(&idt_ptr));
}

void gdt_set_gate(s32int num, u32int base, u32int limit, u8int access, u8int gran)
{
    gdt_entries[num].base_low       = (base & 0xFFFF);
    gdt_entries[num].base_middle    = (base >> 16) & 0xFF;
    gdt_entries[num].base_high      = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low      = (limit & 0xFFFF);
    gdt_entries[num].granularity    = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity    |= gran & 0xF0;
    gdt_entries[num].access         = access;
}

void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags)
{
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].selector = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags;
}

void set_tss(u32int num, u16int ss0, u32int esp0)
{
    u32int base = (u32int)&core_tss;
    u32int limit = base + sizeof(core_tss);
    memset(&core_tss, 0, sizeof(core_tss));
    gdt_set_gate(num, base, limit, 0xE9, 0x00);
    core_tss.ss0 = ss0;
    core_tss.esp0 = esp0;
    core_tss.cs = 0x0b;     // kernel code segment selector
    core_tss.ss = 0x13;     // kernel data segment selector
    core_tss.ds = 0x13;
    core_tss.es = 0x13;
    core_tss.fs = 0x13;
    core_tss.gs = 0x13;
}
