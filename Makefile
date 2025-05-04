CC=m68k-linux-gnu-gcc
KDIR=../m68k_nolibc/linux

all:	m68kmemcpybnchmrk_os_040 \
	m68kmemcpybnchmrk_o2_040 \
	m68kmemcpybnchmrk_o3_040 \
	m68kmemcpybnchmrk_os_030 \
	m68kmemcpybnchmrk_o2_030 \
	m68kmemcpybnchmrk_o3_030 \
	m68kmemcpybnchmrk_os_000

m68kmemcpybnchmrk_os_040: main.c
	$(CC) -ffunction-sections -Wl,-gc-sections -nostdlib -static -m68040 \
		-include $(KDIR)/tools/include/nolibc/nolibc.h -Os -DVERSION="\"040, Os\"" -o $@ $< -lgcc

m68kmemcpybnchmrk_o2_040: main.c
	$(CC) -ffunction-sections -Wl,-gc-sections -nostdlib -static -m68040 \
		-include $(KDIR)/tools/include/nolibc/nolibc.h -O2 -DVERSION="\"040, O2\"" -o $@ $< -lgcc

m68kmemcpybnchmrk_o3_040: main.c
	$(CC) -ffunction-sections -Wl,-gc-sections -nostdlib -static -m68040 \
		-include $(KDIR)/tools/include/nolibc/nolibc.h -O3 -DVERSION="\"040, O3\"" -o $@ $< -lgcc

m68kmemcpybnchmrk_os_030: main.c
	$(CC) -ffunction-sections -Wl,-gc-sections -nostdlib -static -m68030 \
		-include $(KDIR)/tools/include/nolibc/nolibc.h -Os -DVERSION="\"030, Os\"" -o $@ $< -lgcc

m68kmemcpybnchmrk_o2_030: main.c
	$(CC) -ffunction-sections -Wl,-gc-sections -nostdlib -static -m68030 \
		-include $(KDIR)/tools/include/nolibc/nolibc.h -O2 -DVERSION="\"030, O2\"" -o $@ $< -lgcc

m68kmemcpybnchmrk_o3_030: main.c
	$(CC) -ffunction-sections -Wl,-gc-sections -nostdlib -static -m68030 \
		-include $(KDIR)/tools/include/nolibc/nolibc.h -O3 -DVERSION="\"030, O3\"" -o $@ $< -lgcc

m68kmemcpybnchmrk_os_000: main.c
	$(CC) -ffunction-sections -Wl,-gc-sections -nostdlib -static -m68000 \
		-include $(KDIR)/tools/include/nolibc/nolibc.h -Os -DVERSION="\"000, Os\"" -o $@ $< -lgcc
