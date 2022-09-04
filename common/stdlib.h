#include <stddef.h>

// Implemented by stdlib.c
int strncmp(char* s1, char* s2, size_t length);
unsigned long strtoul(char* buf, char** end, int base);
void memset(void* buf, int c, size_t length);

// Implemented by device
void sleep(size_t);
size_t read(void* buf, size_t length);
size_t write(void const* buf, size_t length);
void printf(char const* fmt, ...);
