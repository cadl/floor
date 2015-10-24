AS=as
LD=ld
OBJCOPY=objcopy
CFLAGS=-nostdlib -nostdinc -fno-builtin -I../include -m32 -g -Wall -O0
ASFLAGS= -I../include --32
LDFLAGS=-m elf_i386
CC=gcc
OBJS=kernel/kernel.o mm/mm.o
KERNELLDS=kernel.lds

export CC LD OBJCOPY CFLAGS LDFLAGS AS ASFLAGS


all: sys.img

kernel/kernel.o:kernel/*.c kernel/*.S
	make -C kernel/

mm/mm.o:mm/*.c
	make -C mm/

boot/boot.bin: boot/boot.o
	make -C boot/

kernel.bin:kernel.elf
	$(OBJCOPY) -O binary kernel.elf kernel.bin

kernel.elf:$(OBJS)
	$(LD) $(OBJS) -o kernel.elf -T $(KERNELLDS) $(LDFLAGS)

sys.img:boot/boot.bin kernel.bin
	cat kernel.bin >> boot/boot.bin
	dd if=boot/boot.bin of=sys.img bs=512

bochs-run: all
	bochs -f bochsrc

qemu-run: all
	qemu-system-i386 -fda sys.img -s -S

clean:
	rm -f kernel/*.o mm/*.o
	rm -f sys.img kernel.bin kernel.elf
	rm -f boot/boot.bin boot/boot.elf boot/boot.o
