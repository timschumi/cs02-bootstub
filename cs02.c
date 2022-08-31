#include <stdint.h>

#include "stdlib.h"

static unsigned int (*_uart_read_character)(void) = (void*)0xae2145b0;
static uint32_t (*_uart_write_character)(unsigned int) = (void*)0xae2145f8;
static int (*_uart_ready_for_read)(void) = (void*)0xae214574;
static uint32_t (*_sleep)(uint32_t) = (void*)0xae2048e8;

size_t write(void const* buf, size_t length) {
    uint8_t const* bytes = buf;
    size_t count = 0;

    while (count < length) {
        _uart_write_character(*bytes);
        bytes++;
        count++;
    }

    return count;
}

size_t read(void* buf, size_t length) {
    while (!_uart_ready_for_read()) {
        sleep(50);
    }

    uint8_t* bytes = buf;
    size_t count = 0;

    while (count < length) {
        if (!_uart_ready_for_read())
            break;

        *bytes = _uart_read_character();
        bytes++;
        count++;
    }

    return count;
}

void sleep(size_t time) {
    _sleep(time);
}

__attribute__((naked)) void printf(char const* fmt, ...) {
    asm volatile("b 0xae203534");
}
