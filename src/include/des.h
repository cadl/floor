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

#include "type.h"
#include "asm/system.h"

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

typedef struct gdt_entry_struct gdt_entry_t;
typedef struct idt_entry_struct idt_entry_t;
typedef struct gdt_ptr_struct gdt_ptr_t;
typedef struct idt_ptr_struct idt_ptr_t;


void init_gdt();
void init_idt();
void gdt_set_gate(s32int num, u32int base, u32int limit, u8int access, u8int gran);
void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags);

#endif
