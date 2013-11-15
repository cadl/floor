SECTIONS {
    . = 0x7c00;
    .text : {
        _ftext = .;
        *(.text)
    } = 0
}
