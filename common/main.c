#include <stdint.h>

#include "device.h"
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

        if (strncmp("BOOTFILE", buffer, 8) == 0) {
            uint8_t* buffer = NULL;
            size_t buffer_size = 0;

            boot_get_area(&buffer, &buffer_size);

            uint8_t* buffer_writeptr = buffer;

            size_t file_size = readul();
            size_t remaining_size = file_size;

            if (file_size > buffer_size) {
                write("INVALID", 7);
                continue;
            }

            write("STRTUPLD", 8);

            while (remaining_size > 0) {
                read(buffer_writeptr, 1);
                if (remaining_size % 256 == 0) {
                    write(buffer_writeptr, 1);
                }
                buffer_writeptr++;
                remaining_size--;
            }

            write("ENDUPLD", 7);

            printf("Booting the binary at 0x%x (sized 0x%x)...\n", buffer, file_size);

            boot_jump();

            continue;
        }
    }
}
