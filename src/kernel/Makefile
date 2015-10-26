SUBDIR=kernel/
OBJS=entry.o setup.o monitor.o des.o int.o interrupt.o timer.o pic.o init.o string.o schedule.o asm.o syscall.o process.o switch.o keyboard.o cadsh.o stdio.o

kernel.o:$(OBJS)
	$(LD) -r $^ $(LDFLAGS) -o kernel.o

%.o:%.c
	$(CC) $(CFLAGS) $< -c -o $@

%.o:%.S
	$(CC) $(CFLAGS) -D__ASM__ $< -c -o $@
