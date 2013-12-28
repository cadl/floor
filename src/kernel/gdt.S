.section .text

.global gdt_load
.type gdt_load, @function
gdt_load:
    movl 4(%esp), %eax
    lgdtw (%eax)
    movw $0x10, %ax     # selector of data segment
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ljmp $0x08, $load

load:
    ret
