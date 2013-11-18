.include "pm.inc"

.code16
.section .text
jmp os_main

# segment descritors for GDT
GDT_DESC_NULL: SEG_DESC 0, 0, 0
GDT_DESC_CODE32: SEG_DESC 0, (PECode32Len-1), (DESC_ATTR_TYPE_CD_ER | DESC_ATTR_D)
GDT_DESC_VIDEO: SEG_DESC 0xB8000, 0xFFFF, (DESC_ATTR_TYPE_CD_RW)

.set GdtLen, (. - GDT_DESC_NULL)

.set SegSelectorCode32, (GDT_DESC_CODE32 - GDT_DESC_NULL)
.set SefSelectorVideo, (GDT_DESC_VIDEO - GDT_DESC_NULL)

GDTPtr:
    .word (GdtLen - 1)  # Limit field
    .long 0             # Base field

os_main:
    movw %cs, %ax
    movw %ax, %ds
    movw %ax, %ss
    movw %ax, %es

InitSegDescriptor PE_CODE32, GDT_DESC_CODE32

    xorw %ax, %ax
    movw %cs, %ax
    shl $4, %eax
    addl $GDT_DESC_NULL, %eax
    movl %eax, (GDTPtr + 2)

    xorw %ax, %ax
    in $0x92, %al
    or $2, %al
    out %al, $0x92
    
    cli

    lgdt GDTPtr

    movl %cr0, %eax
    orl $1, %eax
    movl %eax, %cr0
    ljmp $SegSelectorCode32, $0

PE_CODE32:
    .code32
    movl $((80 * 10 + 0) * 2), %edi
    movb $0xC, %ah
    movb $'C', %al
    mov %ax, %gs:(%edi)
    jmp .

.set PECode32Len, (. - PE_CODE32)

.org 0x200, 0x41
