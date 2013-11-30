AS=as
LD=ld
OBJCOPY=objcopy
CFLAGS=-nostdlib -nostdinc -fno-builtin -I../include -m32 -g -Wall
ASFLAGS=-I ../include --32
LDFLAGS=-m elf_i386
CC=gcc

export AS CC LD OBJCOPY CFLAGS ASFLAGS LDFLAGS


all: sys.img

kernel/kernel.bin:kernel/*.c kernel/*.s
	make -C kernel/

boot/boot.bin: boot/boot.o
	make -C boot/

sys.img:boot/boot.bin kernel/kernel.bin
	cat kernel/kernel.bin >> boot/boot.bin
	dd if=boot/boot.bin of=sys.img bs=512

clean:
	rm -f kernel/*.o kernel/kernel.bin kernel/kernel.elf
	rm -f boot/boot.bin boot/boot.elf boot/boot.o
