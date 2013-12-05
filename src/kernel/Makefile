SUBDIR=kernel/
OBJS=entry.o setup.o monitor.o des.o gdt.o int.o interrupt.o
KERNELLDS=kernel.lds

kernel.bin:kernel.elf
	$(OBJCOPY) -O binary kernel.elf kernel.bin

kernel.elf:$(OBJS)
	$(LD) $(OBJS) -o kernel.elf -T $(KERNELLDS) $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) $< -c -o $@

%.o:%.S
	$(CC) $(CFLAGS) -D__ASM__ $< -c -o $@ 
