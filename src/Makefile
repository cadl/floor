AS=as
LD=ld
BOOTLDFILE=boot.lds
OSLDFILE=os.lds
OBJCOPY=objcopy

KERNELINC = kernel/include/
BOOTINC = boot/include/



all: boot.img

boot.o: boot/boot.s
	$(AS) boot/boot.s -o boot.o -I $(BOOTINC)

os.o: kernel/os.s
	$(AS) kernel/os.s -o os.o -I $(KERNELINC)

boot.elf: boot.o
	$(LD) boot.o -o boot.elf -T$(BOOTLDFILE)

boot.bin: boot.elf
	$(OBJCOPY) -R .pdr -R .comment -S -O binary boot.elf boot.bin

os.bin: os.o
	$(LD) -Ttext=0x0 -s os.o -o os.bin --oformat binary

boot.img: boot.bin os.bin
	cat os.bin >> boot.bin
	dd if=boot.bin of=boot.img bs=512

clean:
	rm -f boot.o boot.bin boot.img os.bin os.o boot.elf
