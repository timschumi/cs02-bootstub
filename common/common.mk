# Disable built-in rules
.SUFFIXES:

COMMON_CFLAGS := -Wall -Werror -ffreestanding -I ../common
COMMON_LDFLAGS := -nostdlib

COMMON_SOURCES := \
    main.c \
    stdlib.c \
