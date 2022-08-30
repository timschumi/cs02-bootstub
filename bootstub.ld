ENTRY(_start)

SECTIONS
{
    . = 0x81e08000;

    .entry ALIGN(4K) : AT (ADDR(.entry))
    {
        *(.entry*)
    }

    .text ALIGN(4K) : AT (ADDR(.text))
    {
        *(.text*)
    }

    .rodata ALIGN(4K) : AT (ADDR(.rodata))
    {
        *(.rodata*)
    }

    .data ALIGN(4K) : AT (ADDR(.data))
    {
        *(.data*)
    }

    .bss ALIGN(4K) : AT (ADDR(.bss))
    {
        *(.bss*)
    }

    /DISCARD/ : { *(*) }
}
