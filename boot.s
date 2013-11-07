.code16
.text
    mov %cs, %ax
    mov %ax, %ds
    mov %ax, %es
    call PrintStr
    jmp .

PrintStr:
    mov $BootMessage, %ax
    mov %ax, %bp
    mov $len, %cx
    mov $0x1301, %ax
    mov $0x00c, %bx
    mov $0, %dl
    int $0x10
    ret

BootMessage: .ascii "flooooor, hi"

len = . - BootMessage

.org 510
.word 0xaa55
