CC=m68k-linux-gnu-gcc
KDIR=../m68k_nolibc/linux

m68kmemcpybnchmrk: main.c
	$(CC) -ffunction-sections -Wl,-gc-sections -nostdlib -static -m68040 \
		-include $(KDIR)/tools/include/nolibc/nolibc.h -O3 -o $@ $< -lgcc
