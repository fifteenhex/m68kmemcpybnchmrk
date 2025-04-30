CC=m68k-linux-gnu-gcc
KDIR=../m68k_nolibc/linux/

m68kmemcpybnchmrk: main.c
	$(CC) -m68040 -nostdlib -static -include $(KDIR)/tools/include/nolibc/nolibc.h -Os -o $@ $< -lgcc
