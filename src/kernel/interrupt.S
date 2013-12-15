#include "int.h"
.include "asm/smacro.inc"


.section .text

ISR_NOERRCODE       de_entry,       INT_DE
ISR_NOERRCODE       nmi_entry,      INT_NMI
ISR_NOERRCODE       bp_entry,       INT_BP
ISR_NOERRCODE       of_entry,       INT_OF
ISR_NOERRCODE       br_entry,       INT_BR
ISR_NOERRCODE       ud_entry,       INT_UD
ISR_NOERRCODE       nm_entry,       INT_NM
ISR_ERRCODE         df_entry,       INT_DF
ISR_ERRCODE         cso_entry,      INT_CSO
ISR_ERRCODE         ts_entry,       INT_TS
ISR_ERRCODE         np_entry,       INT_NP
ISR_ERRCODE         ss_entry,       INT_SS
ISR_ERRCODE         gp_entry,       INT_GP
ISR_ERRCODE         pf_entry,       INT_PF
ISR_NOERRCODE       mf_entry,       INT_MF
ISR_ERRCODE         ac_entry,       INT_AC
ISR_NOERRCODE       mc_entry,       INT_MC
ISR_NOERRCODE       xm_entry,       INT_XM

ISR_NOERRCODE       irq0_entry,     INT_IRQ0
ISR_NOERRCODE       irq1_entry,     INT_IRQ1
ISR_NOERRCODE       irq2_entry,     INT_IRQ2
ISR_NOERRCODE       irq3_entry,     INT_IRQ3
ISR_NOERRCODE       irq4_entry,     INT_IRQ4
ISR_NOERRCODE       irq5_entry,     INT_IRQ5
ISR_NOERRCODE       irq6_entry,     INT_IRQ6
ISR_NOERRCODE       irq7_entry,     INT_IRQ7
ISR_NOERRCODE       irq8_entry,     INT_IRQ8
ISR_NOERRCODE       irq9_entry,     INT_IRQ9
ISR_NOERRCODE       irq10_entry,    INT_IRQ10
ISR_NOERRCODE       irq11_entry,    INT_IRQ11
ISR_NOERRCODE       irq12_entry,    INT_IRQ12
ISR_NOERRCODE       irq13_entry,    INT_IRQ13
ISR_NOERRCODE       irq14_entry,    INT_IRQ14
ISR_NOERRCODE       irq15_entry,    INT_IRQ15

ISR_NOERRCODE       syscall_entry,  INT_SYSCALL

.global interrupt_entry
.type interrupt_entry, @function

interrupt_entry:
    #xchgl %eax, (%esp)        # pushl %eax movl (%esp), %eax
    pushl %eax
    mov 4(%esp), %eax
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %edi
    pushl %esi
    pushl %ebp
    pushl %gs
    pushl %fs
    pushl %es
    pushl %ds

    /* Reset kernel data segments */
    movl $0x10, %ecx
    movw %cx, %ds
    movw %cx, %es
    movw %cx, %fs
    movw %cx, %gs

    pushl %esp        /* Poiner of regs */
    pushl %eax        /* First argument of interrupt_handler */
    call interrupt_handler
    addl $8, %esp


interrupt_return:
    popl %ds
    popl %es
    popl %fs
    popl %gs 
    popl %ebp
    popl %esi
    popl %edi
    popl %edx
    popl %ecx
    popl %ebx
    popl %eax
    addl $8, %esp
    sti
    iret
