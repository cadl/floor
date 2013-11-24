OUTPUT_FORMAT("binary")
STARTUP("os.o")

phys = 0x0;

SECTIONS {
    .text phys : AT(phys) {
        code = .;
        *(.text)
        *(.rodata)
        . = ALIGN(0);
    }
    .data : AT(phys + (data - code)) {
        data = .;
        *(.data)
        . = ALIGN(0);
    }
    .bss : AT(phys + (bss - code)) {
        bss = .;
        *(.bss)
        . = ALIGN(0);
    }
    end = .;
}
