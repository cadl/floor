SUBDIR=kernel/
OBJS=entry.o os.o monitor.o common.o
KERNELLDS=kernel.lds

kernel.bin:$(OBJS)
	$(LD) -s $(OBJS) -o kernel.bin -T $(KERNELLDS) $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) $< -c -o $@

%.o:%.s
	$(AS) $< -o $@ $(ASFLAGS)
