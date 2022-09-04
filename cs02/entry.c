void main();
__attribute__((section(".entry"))) void _start() {
    main();

    for (;;)
        asm volatile("wfi");
}
