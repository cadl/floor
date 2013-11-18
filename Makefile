AS=as
CC=gcc
LD=ld
LDFILE=floor_x86.lds
OBJCOPY=objcopy


all: boot.img

boot.o: boot.s
	$(AS) boot.s -o boot.o

boot.elf: boot.o
	$(LD) boot.o -o boot.elf -T$(LDFILE)

os.o: os.s
	$(CC) -c os.s -o os.o

os.bin: os.o
	$(LD) -Ttext=0x0 -s os.o -o os.bin --oformat binary
	
boot.bin: boot.elf
	$(OBJCOPY) -R .pdr -R .comment -R.note -S -O binary boot.elf boot.bin

boot.img: boot.bin os.bin
	dd if=boot.bin of=boot.img bs=512 count=1
	dd if=/dev/zero of=boot.img skip=1 seek=1 bs=512 count=2879
	mount boot.img /tmp/fd
	cp os.bin /tmp/fd
	umount /tmp/fd

clean:
	rm -rf boot.o boot.elf boot.bin boot.img
