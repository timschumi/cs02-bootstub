#include "stdlib.h"

void memset(void* buf, int c, size_t length) {
    while (length > 0) {
        *((char*)buf++) = c;
        length--;
    }
}

int strncmp(char* s1, char* s2, size_t length) {
    while (1) {
        if (length <= 0)
            return 0;

        if (*s1 == 0 || *s2 == 0 || *s1 != *s2)
            return *s1 - *s2;

        s1++;
        s2++;
        length--;
    }
}

unsigned long strtoul(char* buf, char** end, int base) {
    if (base != 0 && (base < 2 || base > 36))
        return 0;

    while (*buf == '\n' || *buf == '\r' || *buf == '\t' || *buf == ' ') {
        buf++;
    }

    if (!*buf) {
        if (end)
            *end = buf;

        return 0;
    }

    if (base == 0) {
        if (*buf == '0') {
            buf++;
            if (*buf == 'x' || *buf == 'X') {
                buf++;
                base = 16;
            } else {
                base = 8;
            }
        } else {
            base = 10;
        }
    }

    if (base == 16 && *buf == '0' && (*(buf + 1) == 'x' || *(buf + 1) == 'X'))
        buf += 2;

    unsigned long value = 0;

    while (*buf) {
        unsigned long digit;

        if (*buf >= '0' && *buf <= '9')
            digit = *buf - '0';
        if (*buf >= 'a' && *buf <= 'z')
            digit = 10 + *buf - 'a';
        if (*buf >= 'A' && *buf <= 'Z')
            digit = 10 + *buf - 'A';

        if (digit >= base)
            break;

        value = value * base + digit;
        buf++;
    }

    if (end)
        *end = buf;

    return value;
}
