#include <sys.h>

.global get_eip
get_eip:
    mov 0(%esp), %eax
    ret

.global framecpy
framecpy:
    push %ebx
    pushf
    cli
    mov 16(%esp), %ebx
    mov 12(%esp), %ecx
    mov %cr0, %edx
    and $0x7fffffff, %edx
    mov %edx, %cr0

    mov $1024, %edx
loop:
    mov (%ebx), %eax
    mov %eax, (%ecx)
    addl $4, %ebx
    addl $4, %ecx
    dec %edx
    jnz loop

    mov %cr0, %edx
    orl $0x80000000, %edx
    mov %edx, %cr0

    popf
    pop %ebx
    ret

.global gdt_load
gdt_load:
    movl 4(%esp), %eax
    lgdtw (%eax)
    movw $0x10, %ax     # selector of data segment
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ret

.global tss_load
tss_load:
    mov $(CORE_TSS), %ax
    ltr %ax
    ret

.global process0_setup
process0_setup:
    movl $(USER_STACK_TOP-0x100), %eax
    movl %eax, %esp
    jmp process0_start
