.code16
.section .text

.set OS_SEG, 0x800
.set OS_OFFSET, 0x0000

.global _start

jmp start_program
nop

BS_OEMName:         .ascii  "wondwall"
BPB_BytsPerSec:     .word   512
BPB_SecPerCluster:  .byte   1
BPB_ResvdSecCnt:    .word   1
BPB_NumFATs:        .byte   2
BPB_RootEntCnt:     .word   224
BPB_TotSec16:       .word   2880
BPB_Media:          .byte   0xf0
BPB_FATSz16:        .word   9
BPB_SecPerTrk:      .word   18
BPB_NumHeads:       .word   2
BPB_HiddSec:        .long   0
BPB_TotSec32:       .long   0
BS_DrvNum:          .byte   0
BS_Reserved1:       .byte   0
BS_BootSig:         .byte   0x29
BS_VolID:           .long   0
BS_VolLab:          .ascii  "floor   0.1"
BS_FileSysType:     .ascii  "FAT12   "

start_program:
    movw %cs, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    xorw %sp, %sp
    
    cld     #clear direction flag
    sti     #set interrupt flag
    
    movw $OS_SEG, %ax
    movw %ax, %es
    movb $2, %cl

read_loop:
    movb $0x02, %ah
    movb $0x1, %al
    movb $0x0, %ch
    movb $0x0, %dh
    movb $0, %dl
    int $0x13
    jc fail

    movw %es, %ax
    addw $0x20, %ax
    movw %ax, %es
    incb %cl
    
    cmpb $5, %cl
    jbe read_loop
    
    jmp os_entry

os_entry:
    ljmp $OS_SEG, $OS_OFFSET

end:
    hlt
    jmp end

fail:
    movw 0x07C0, %ax
    movw %ax, %es
    movw $err_msg, %ax
    movw %ax, %bp
    movw $err_msg_len, %cx
    movw $0x1301, %ax
    movw $0x00c, %bx
    movb $0, %dl
    int $0x10
    jmp end

err_msg: .ascii "read sector failed" 
.set err_msg_len, . - err_msg

.org 510
.word 0xaa55
