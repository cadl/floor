#ifndef __INT_H
#define __INT_H


#define INT_DE      0   //Divide by 0
//                  1   Reserved
#define INT_NMI     2   //NMI Interrupt
#define INT_BP      3   //Breakpoint (INT3)
#define INT_OF      4   //Overflow (INTO)
#define INT_BR      5   //Bounds range exceeded (BOUND)
#define INT_UD      6   //Invalid opcode (UD2)
#define INT_NM      7   //Device not available (WAIT/FWAIT)
#define INT_DF      8   //Double falut
#define INT_CSO     9   //Coprocessor segment overrun
#define INT_TS      10  //Invalid TSS
#define INT_NP      11  //Segment not present
#define INT_SS      12  //Stack-segment fault
#define INT_GP      13  //General protection fault
#define INT_PF      14  //Page fault
//                  15  Reserved
#define INT_MF      16  //x87 FPU error
#define INT_AC      17  //Alignment check
#define INT_MC      18  //Machine check
#define INT_XM      19  //SIMD Floating-Point Exception
//               20-31  Reserved
#define INT_IRQ0    32
#define INT_IRQ1    33
#define INT_IRQ2    34
#define INT_IRQ3    35
#define INT_IRQ4    36
#define INT_IRQ5    37
#define INT_IRQ6    38
#define INT_IRQ7    39
#define INT_IRQ8    40
#define INT_IRQ9    41
#define INT_IRQ10   42
#define INT_IRQ11   43
#define INT_IRQ12   44
#define INT_IRQ13   45
#define INT_IRQ14   46
#define INT_IRQ15   47
#define INT_SYSCALL 48
#define INT_MAXN    48
#define IDT_SIZE    256

#define IRQ_TIMER       0
#define IRQ_KEYBOARD    1
#define IRQ_SLAVECARD   2       // 8259A slave controller
#define IRQ_COM2        3
#define IRQ_COM1        4
#define IRW_LPT2        5
#define IRQ_FLOPPY      6
#define IRQ_LPT1        7
#define IRQ_RTC         8       //cmos real time controller
//      Unassigned      9-11
#define IRQ_MOUSE       12
#define IRQ_MATHP       13
#define IRQ_HD1         14
#define IRQ_HD2         15

#ifndef __ASM__
#include <type.h>

void interrupt_handler(int in, registers_t *reg);

extern void de_entry(void);
extern void nmi_entry(void);
extern void bp_entry(void);
extern void of_entry(void);
extern void br_entry(void);
extern void ud_entry(void);
extern void nm_entry(void);
extern void df_entry(void);
extern void cso_entry(void);
extern void ts_entry(void);
extern void np_entry(void);
extern void ss_entry(void);
extern void gp_entry(void);
extern void pf_entry(void);
extern void mf_entry(void);
extern void ac_entry(void);
extern void mc_entry(void);
extern void xm_entry(void);
extern void irq0_entry(void);
extern void irq1_entry(void);
extern void irq2_entry(void);
extern void irq3_entry(void);
extern void irq4_entry(void);
extern void irq5_entry(void);
extern void irq6_entry(void);
extern void irq7_entry(void);
extern void irq8_entry(void);
extern void irq9_entry(void);
extern void irq10_entry(void);
extern void irq11_entry(void);
extern void irq12_entry(void);
extern void irq13_entry(void);
extern void irq14_entry(void);
extern void irq15_entry(void);
extern void syscall_entry(void);
#endif
#endif
