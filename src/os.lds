SECTIONS {
    . = 0x0;
    .text : {
        _ftext = .;
        *(.text)
    }
}
