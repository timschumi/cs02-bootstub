#include <stdint.h>

// Note: This has to be the very first.
void main();
void _start() {
    main();

    for (;;)
        asm volatile("wfi");
}

void (*_log_message)(char*, ...) = (void*)0xae203534;
uint8_t (*__microusb_register_read)(uint16_t reg, uint8_t* value) = (void*)0xae230200;
uint8_t (*__microusb_register_write)(uint16_t reg, uint8_t value) = (void*)0xae23027c;

uint16_t microusb_registers[] = {
    0x1,
    0x2, // INT2
    0x3,
    0x4,
    0x5,
    0x6,
    0x7, // ADC
    0xa, // DATA_TYPE1
    0xb, // DATA_TYPE2
    0x13, // INT1
};

void microusb_dump_registers() {
    _log_message("Register:");
    for (int i = 0; i < sizeof(microusb_registers)/sizeof(microusb_registers[0]); i++) {
        _log_message(" 0x%02x", microusb_registers[i]);
    }
    _log_message("\n");

    _log_message("Value:   ");
    for (int i = 0; i < sizeof(microusb_registers)/sizeof(microusb_registers[0]); i++) {
        uint8_t value = 0;
        uint8_t rc = __microusb_register_read(microusb_registers[i], &value);
	    if (rc == 0)
            _log_message(" 0x%02x", value, rc);
        else
            _log_message(" FAIL");
    }
    _log_message("\n");
}

void main() {
    _log_message("Hello from bootstub!\n");

    _log_message("Here is an USB register dump:\n");
    microusb_dump_registers();
}
