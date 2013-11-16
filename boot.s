.code16
    .section .text
    
jmp _start
nop

BS_OEMName:         .ascii  "wondwall"
BPB_BytsPerSec:     .2byte  512
BPB_SecPerCluster:  .byte   1
BPB_ResvdSecCnt:    .2byte  1
BPB_NumFATs:        .byte   2
BPB_RootEntCnt:     .2byte  224
BPB_TotSec16:       .2byte  2880
BPB_Media:          .byte   0xf0
BPB_FATSz16:        .2byte  9
BPB_SecPerTrk:      .2byte  18
BPB_NumHeads:       .2byte  2
BPB_HiddSec:        .4byte  0
BPB_TotSec32:       .4byte  0
BS_DrvNum:          .byte   0
BS_Reserved1:       .byte   0
BS_BootSig:         .byte   0x29
BS_VolID:           .4byte  0
BS_VolLab:          .ascii  "floor   0.1"
BS_FileSysType:     .ascii  "FAT12   "

_start:
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

BootMessage: 
    .ascii "flooooor, hi"
    .equ len, . - BootMessage

.org 510
.word 0xaa55
