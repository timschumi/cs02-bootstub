void (*_log_message)(char*, ...) = (void*)0xae203534;

__attribute__((naked)) void _start() {
    _log_message((char*)0xae25ec74, "blahblah");

    for (;;)
        asm volatile("wfi");
}
