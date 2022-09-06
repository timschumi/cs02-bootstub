#include <stdint.h>

#include "stdlib.h"

#define PKG_ID_LEN 8

unsigned long readul() {
    char buffer[0x20];
    memset(buffer, 0, sizeof(buffer));
    size_t len = read(buffer, sizeof(buffer) - 1);
    buffer[len] = 0;

    return strtoul(buffer, NULL, 0);
}

void main() {
    printf("Hello from bootstub!\n");

    while (1) {
        char buffer[PKG_ID_LEN];
        memset(buffer, 0, PKG_ID_LEN);
        read(buffer, PKG_ID_LEN);

        if (strncmp("WHOISDIS", buffer, PKG_ID_LEN) == 0) {
            write("BOOTSTUB", PKG_ID_LEN);
            continue;
        }

        if (strncmp("UPLDMEM", buffer, 7) == 0) {
            uint8_t* start = (uint8_t*)readul();
            uint8_t* end = (uint8_t*)readul();

            if (start > end) {
                write("INVALID", 7);
                continue;
            }

            write("STRTUPLD", 8);

            size_t remaining = end - start;
            uint8_t checksum = 0;
            size_t bit_count = 0;
            uint16_t sliding_window = 0;

            while (remaining > 0 || checksum || bit_count > 0) {
                while (bit_count < 7) {
                    uint8_t value;
                    size_t bits_in_value = 8;

                    if (remaining > 0) {
                        value = *start;
                        remaining--;
                        start++;
                    } else if (checksum) {
                        value = checksum;
                    } else {
                        value = 0;
                        bits_in_value = 7 - bit_count;
                    }

                    checksum ^= value;

                    sliding_window = (sliding_window << bits_in_value) | value;
                    bit_count += bits_in_value;
                }

                // The most significant bit has to be set to not collide with the control characters
                // in the lower half of ASCII.
                uint8_t value = 0b10000000 | ((sliding_window >> (bit_count - 7)) & 0b1111111);
                bit_count -= 7;

                write(&value, 1);
            }

            write("ENDUPLD", 7);

            continue;
        }
    }
}
