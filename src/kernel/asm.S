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
