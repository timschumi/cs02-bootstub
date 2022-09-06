include ../common/common.mk

CROSS_COMPILE = arm-none-eabi-
CFLAGS = -mcpu=cortex-a9 -mfloat-abi=softfp -mfpu=neon
LDFLAGS = -T linker.ld

DEVICE_SOURCES = \
    board.c \
    entry.c \

out/bootstub.img: out/bootstub.bin out/ramdisk.bin
	mkbootimg \
		--kernel out/bootstub.bin \
		--ramdisk out/ramdisk.bin \
		--cmdline "buildvariant=userdebug" \
		--base 0x82000000 \
		--kernel_offset 0x00008000 \
		--ramdisk_offset 0x01000000 \
		--second_offset 0x00f00000 \
		--tags_offset 0x00000100 \
		--os_version 7.1.2 \
		--os_patch_level 2021-06 \
		--pagesize 4096 \
		--header_version 0 \
		--hashtype sha1 \
		-o out/bootstub.img

out/bootstub.bin: out/bootstub.elf
	$(CROSS_COMPILE)objcopy -O binary out/bootstub.elf out/bootstub.bin
	truncate -s 3633224 out/bootstub.bin

out/ramdisk.bin:
	truncate -s 1359384 out/ramdisk.bin

out/bootstub.elf: $(COMMON_SOURCES:%.c=out/common/%.o) $(DEVICE_SOURCES:%.c=out/cs02/%.o)
	@mkdir -p out
	$(CROSS_COMPILE)ld $(COMMON_LDFLAGS) $(LDFLAGS) -o $@ $^

out/%.o: ../%.c
	@mkdir -p $(dir $@)
	$(CROSS_COMPILE)gcc -c $(COMMON_CFLAGS) $(CFLAGS) -o $@ $^

.PHONY: flash

flash: out/bootstub.img
	heimdall flash --KERNEL out/bootstub.img
