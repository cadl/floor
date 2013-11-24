BOOTLDFILE=boot.lds


boot.bin:boot.elf
	$(OBJCOPY) -R .pdr -R .comment -S -O binary boot.elf boot.bin

boot.elf:boot.o
	$(LD) boot.o -o boot.elf -T$(BOOTLDFILE) $(LDFLAGS)

boot.o:boot.s
	$(AS) boot.s -o boot.o $(ASFLAGS)
