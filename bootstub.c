__attribute__((naked)) void _start() {
    ((void (*)(char*))0xae2027a4)((char*)0xae2644ec);

    for (;;)
        asm volatile("wfi");
}
