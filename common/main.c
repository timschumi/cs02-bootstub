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

            while (remaining > 0) {
                checksum ^= *start;
                write(start, 1);
                start++;
                remaining--;
            }

            write(&checksum, 1);

            write("ENDUPLD", 7);

            continue;
        }
    }
}
