# Disable built-in rules
.SUFFIXES:

COMMON_CFLAGS := -std=gnu99 -ffreestanding -I ../common
COMMON_LDFLAGS := -nostartfiles -nostdlib

COMMON_SOURCES := \
    main.c \
    stdlib.c \
