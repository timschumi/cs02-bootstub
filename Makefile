CROSS_COMPILE = /opt/toolchains/arm-eabi-4.8/bin/arm-eabi-
CFLAGS = -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=softfp -mfpu=neon

bootstub.img: bootstub.bin ramdisk.bin
	mkbootimg \
		--kernel bootstub.bin \
		--ramdisk ramdisk.bin \
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
		-o bootstub.img

bootstub.bin: bootstub.o
	$(CROSS_COMPILE)objcopy -O binary -j .text bootstub.o bootstub.bin
	truncate -s 3633224 bootstub.bin

ramdisk.bin:
	truncate -s 1359384 ramdisk.bin

bootstub.o: bootstub.c
	$(CROSS_COMPILE)gcc $(CFLAGS) -o bootstub.o -c bootstub.c
