# Disable built-in rules
.SUFFIXES:

COMMON_CFLAGS := -ffreestanding -I ../common
COMMON_LDFLAGS := -nostdlib

COMMON_SOURCES := \
    main.c \
    stdlib.c \
