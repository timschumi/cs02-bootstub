ENTRY(_start)

SECTIONS
{
    . = 0x81e08000;

    .text ALIGN(4K) : AT (ADDR(.text))
    {
        *(.text*)
    }

    .rodata ALIGN(4K) : AT (ADDR(.rodata))
    {
        *(.rodata*)
    }

    /DISCARD/ : { *(*) }
}
