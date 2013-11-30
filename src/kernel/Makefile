SUBDIR=kernel/
OBJS=entry.o setup.o monitor.o common.o x86.o gdt.o
KERNELLDS=kernel.lds

kernel.bin:kernel.elf
	$(OBJCOPY) -O binary kernel.elf kernel.bin

kernel.elf:$(OBJS)
	$(LD) $(OBJS) -o kernel.elf -T $(KERNELLDS) $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) $< -c -o $@

%.o:%.s
	$(AS) $< -o $@ $(ASFLAGS)
