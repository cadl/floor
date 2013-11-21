.include "pm.inc"

.code16
.text
jmp LABEL_BEGIN

# segment descritors for GDT
LABEL_GDT:          Descriptor 0, 0, 0
LABEL_DESC_CODE32:  Descriptor 0, (SegCode32Len-1), (DA_C + DA_32)
LABEL_DESC_VIDEO:   Descriptor 0xB8000, 0xffff, DA_DRW 

.set GdtLen, . - LABEL_GDT

GdtPtr:
    .2byte (GdtLen - 1)  # Limit field
    .4byte 0             # Base field

.set SelectorCode32, (LABEL_DESC_CODE32 - LABEL_GDT)
.set SelectorVideo, (LABEL_DESC_VIDEO - LABEL_GDT)


/* begin */
LABEL_BEGIN:
    mov %cs, %ax
    mov %ax, %ds
    mov %ax, %ss
    mov %ax, %es
    
    InitDesc LABEL_SEG_CODE32, LABEL_DESC_CODE32
    
    xor %eax, %eax
    mov %ds, %ax
    shl $4, %eax
    add $(LABEL_GDT), %eax
    movl %eax, (GdtPtr + 2)

    lgdtw GdtPtr

    cli
    
    inb $0x92, %al
    orb $2, %al
    outb %al, $0x92

    movl %cr0, %eax
    orl $1, %eax
    movl %eax, %cr0

    ljmp $SelectorCode32, $0

LABEL_SEG_CODE32:
    .code32
    movw $(SelectorVideo), %ax
    movw %ax, %gs
    
    movl $((80 * 10 + 0) * 2), %edi
    movb $0xC, %ah
    movb $'F', %al
    mov %ax, %gs:(%edi)
    jmp .

.set SegCode32Len, . - LABEL_SEG_CODE32

.org 0x200, 0x41
