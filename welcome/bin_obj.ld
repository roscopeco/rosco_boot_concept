OUTPUT_FORMAT("elf32-m68k")

SECTIONS {
    .rodata.binary :
    {        
        _image_start = .;
        *(.data)
        _image_end = .;
        . = ALIGN(2);
        _binary_end = .;
    }
}
