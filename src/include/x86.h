#include "common.h"


extern void gdt_load(u32int ptr);
void init_descriptor_tables();
void init_gdt();
void gdt_set_des(s32int num, u32int base, u32int limit, u8int access, u8int gran);


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
