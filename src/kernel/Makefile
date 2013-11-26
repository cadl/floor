SUBDIR=kernel/
OBJS=entry.o kernel.o monitor.o common.o
KERNELLDS=kernel.lds

kernel.bin:kernel.elf kernel.sym
	$(OBJCOPY) -O binary kernel.elf kernel.bin

kernel.elf:$(OBJS)
	$(LD) $(OBJS) -o kernel.elf -T $(KERNELLDS) $(LDFLAGS)

kernel.sym:kernel.elf
	$(OBJCOPY) --only-keep-debug kernel.elf kernel.sym

%.o:%.c
	$(CC) $(CFLAGS) $< -c -o $@

%.o:%.s
	$(AS) $< -o $@ $(ASFLAGS)
