#include <stdint.h>

#include "stdlib.h"

#define PKG_ID_LEN 8

unsigned long readul() {
    uint8_t buffer[0x20];
    memset(buffer, 0, sizeof(buffer));
    size_t len = read(buffer, sizeof(buffer) - 1);
    buffer[len] = 0;

    return strtoul(buffer, NULL, 0);
}

void main() {
    printf("Hello from bootstub!\n");

    while (1) {
        uint8_t buffer[PKG_ID_LEN];
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

            uint8_t checksum = 0;

            while (start != end) {
                uint8_t value = *start;
                uint8_t value_enc[2];

                // FIXME: Pick a better encoding (or get UART to ignore control characters).
                value_enc[0] = (value & 0xf) + 0x20;
                value_enc[1] = ((value & 0xf0) >> 4) + 0x20;

                write(value_enc, 2);
                checksum ^= value;
                start++;
            }

            uint8_t checksum_enc[2];
            checksum_enc[0] = (checksum & 0xf) + 0x20;
            checksum_enc[1] = ((checksum & 0xf0) >> 4) + 0x20;
            write(&checksum_enc, 2);

            write("ENDUPLD", 7);

            continue;
        }
    }
}
