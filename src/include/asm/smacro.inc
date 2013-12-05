/*
        macro for asm code
        
include:
    - Descriptor Base, Limit, Attr      define descriptor
    - InitDesc OffSet, GdtSegAddr       init descriptor in setup.S (pay attention to args)
    - ISR_ERRCODE Fun, Num              isr with error code
    - ISR_NOERRCODE Fun, Num            isr without error code
    - any GDT attr

                                */

.macro Descriptor Base, Limit, Attr
    .2byte  \Limit & 0xFFFF
    .2byte  \Base & 0xFFFF
    .byte   (\Base >> 16) & 0xFF
    .2byte  ((\Limit >> 8) & 0xF00) | (\Attr & 0xF0FF)
    .byte   (\Base >> 24) & 0xFF
.endm   

.macro InitDesc OffSet, GdtSegAddr
    xor %ax, %ax
    mov %cs, %ax
    shl $4, %eax
    addl $(\OffSet), %eax
    movw %ax, (\GdtSegAddr + 2)
    shr $16, %eax
    movb %al, (\GdtSegAddr + 4)
    movb %ah, (\GdtSegAddr + 7)
.endm


/* GDT Descriptor Attributes
    DA_ : Descriptor Attribute
    D: Data Segment
    C: Code Segment
    S: System Segment
    R: Read-only
    RW: Read/Write
    A: Access 
*/

.set    DA_32, 0x4000 /* 32-bit segment */

/* Descriptor privilege level */

.set    DA_DPL0, 0x00   /* DPL = 0*/
.set    DA_DPL1, 0x20   /* DPL = 1*/
.set    DA_DPL2, 0x40   /* DPL = 2*/
.set    DA_DPL3, 0x60   /* DPL = 3*/

/* GDT Code- and Data-Segment Types */

.set    DA_DR, 0x90     /* Read-Only */
.set    DA_DRW, 0x92    /* Read/Write */
.set    DA_DRWA,0x93    /* Read/Write, accessed */
.set    DA_C, 0x98      /* Execute-Only */
.set    DA_CR, 0x9A     /* Execute/Read */
.set    DA_CCO, 0x9C    /* Execute-Only, conforming */
.set    DA_CCOR, 0x9E   /* Execute/Read-Only, conforming */


/* GDT System-Segment and Gate-Descriptor Types */

.set    DA_LDT, 0x82    /* LDT */
.set    DA_TaskGate, 0x85   /* Task Gate */
.set    DA_386TSS, 0x89     /* 32-bit TSS(Available) */
.set    DA_386CGate, 0x8C   /* 32-bit Call Gate */
.set    DA_386IGate, 0x8E   /* 32-bit Interrupt Gate */
.set    DA_386TGate, 0x8F   /* 32-bit Trap Gate */

/* Selector Attributes */

.set    SA_RPL0, 0
.set    SA_RPL1, 1
.set    SA_RPL2, 2
.set    SA_RPL3, 3
.set    SA_TIG, 0
.set    SA_TIL, 4


/* about int */
.macro ISR_NOERRCODE Fun Num
.global \Fun
\Fun:
    cli
    pushl $0
    pushl $\Num
    jmp interrupt_entry
.endm

.macro ISR_ERRCODE Fun Num
.global \Fun
\Fun:
    cli
    pushl $\Num
    jmp interrupt_entry
.endm
