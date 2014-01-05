//  header file about descriptor
//  struct:
//      - gdt_entry_struct      gdt_entry_t
//      - idt_entry_struct      idt_entry_t
//      - gdt_ptr_struct        gdt_ptr_t
//      - idt_ptr_struct        idt_ptr_t
//
//  func:
//      - init_gdt
//      - init_idt
//      - gdt_set_gate
//      - idt_set_gate

#ifndef __DES_H
#define __DES_H

#include <type.h>
#include <asm/system.h>

struct gdt_entry_struct
{
    u16int  limit_low;      //  limit;  lower 16bits
    u16int  base_low;       //  base;   lower 16bits
    u8int   base_middle;    //  base;   middle 16bits
    u8int   access;         //  access flags; DPL etc.
    u8int   granularity;    //  granularity;
    u8int   base_high;      //  base;   high 8bits 
} __attribute__((packed));

struct gdt_ptr_struct
{
    u16int  limit;
    u32int  base;
} __attribute__((packed));

struct idt_entry_struct
{
    u16int  base_low;           // the lower 16 bit of address
    u16int  selector;           // kernel segment selector 
    u8int   always0;            // this always be zero 
    u8int   flags;              //  
    u16int  base_high;          // the high 16 bit of address 
} __attribute__((packed));  

struct idt_ptr_struct
{
    u16int limit;
    u32int base;
} __attribute__((packed));

struct tss_entry_struct         // http://www.jamesmolloy.co.uk/tutorial_html/10.-User%20Mode.html
{
    u32int prev_tss;   // The previous TSS - if we used hardware task switching this would form a linked list.
    u32int esp0;       // The stack pointer to load when we change to kernel mode.
    u32int ss0;        // The stack segment to load when we change to kernel mode.
    u32int esp1;       // Unused...
    u32int ss1;
    u32int esp2;
    u32int ss2;
    u32int cr3;
    u32int eip;
    u32int eflags;
    u32int eax;
    u32int ecx;
    u32int edx;
    u32int ebx;
    u32int esp;
    u32int ebp;
    u32int esi;
    u32int edi;
    u32int es;         // The value to load into ES when we change to kernel mode.
    u32int cs;         // The value to load into CS when we change to kernel mode.
    u32int ss;         // The value to load into SS when we change to kernel mode.
    u32int ds;         // The value to load into DS when we change to kernel mode.
    u32int fs;         // The value to load into FS when we change to kernel mode.
    u32int gs;         // The value to load into GS when we change to kernel mode.
    u32int ldt;
    u16int trap;
    u16int iomap_base;
} __attribute__((packed));

typedef struct gdt_entry_struct gdt_entry_t;
typedef struct idt_entry_struct idt_entry_t;
typedef struct gdt_ptr_struct gdt_ptr_t;
typedef struct idt_ptr_struct idt_ptr_t;
typedef struct tss_entry_struct tss_entry_t;

tss_entry_t core_tss;

void init_gdt();
void init_idt();
void gdt_set_gate(s32int num, u32int base, u32int limit, u8int access, u8int gran);
void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags);
void set_tss(u32int num, u16int ss0, u32int esp0);

extern void gdt_load(u32int ptr);
extern void tss_load(u32int sel);

#endif
