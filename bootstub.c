#include <stdint.h>

void (*_log_message)(char*, ...) = (void*)0xae203534;
void (*_uart_print_character)(int) = (void*)0xae2026e8;
uint32_t (*_sleep)(uint32_t) = (void*)0xae2048e8;
uint8_t (*__microusb_register_read)(uint16_t reg, uint8_t* value) = (void*)0xae230200;
uint8_t (*__microusb_register_write)(uint16_t reg, uint8_t value) = (void*)0xae23027c;

uint32_t get_input_key_set(int set) {
    return *(uint32_t*)(0x350038a0 + set * 4);
}

void wait_for_key_press() {
    while (get_input_key_set(0))
        _sleep(100);

    while (!get_input_key_set(0))
        _sleep(100);
}

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

int last_buffer_offset = 0;

void microusb_force_usb() {
    uint8_t value = 0;
    __microusb_register_read(0x2, &value);
    __microusb_register_write(0x2, value & 0b11111011);
    __microusb_register_write(0x13, 0x24);

    last_buffer_offset = (*(int*)0xae8ffffc) & 0x6ffff;
}

void microusb_unforce_usb() {
    uint8_t value = 0;
    __microusb_register_read(0x2, &value);
    __microusb_register_write(0x2, value | 0b00000100);
    __microusb_register_write(0x13, 0x00);

    int current_buffer_offset = (*(int*)0xae8ffffc) & 0x6ffff;

    while (last_buffer_offset != current_buffer_offset) {
        _uart_print_character(*((char*)0xae900000 + last_buffer_offset));
        last_buffer_offset = (last_buffer_offset + 1) & 0x6ffff;
    }
}

void main() {
    _log_message("Hello from bootstub!\n");

    _log_message("Here is an USB register dump:\n");
    microusb_dump_registers();

    microusb_force_usb();

    _log_message("USB registers while force-enabling USB:\n");
    microusb_dump_registers();

    microusb_unforce_usb();
}
