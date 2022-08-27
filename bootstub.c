void (*_log_message)(char*, ...) = (void*)0xae203534;

void _start() {
    _log_message("Hello from bootstub!");

    for (;;)
        asm volatile("wfi");
}
