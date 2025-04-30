#define SZ_1K 1024
#define SZ_1M (SZ_1K * 1024)
#define TEST_SZ (SZ_1M * 256)
#define COPY_SZ (128 * SZ_1K)
#define BUFF_SZ (COPY_SZ * 2)

#define OUTTER_LOOPS (TEST_SZ/COPY_SZ)

#define ARRAY_SZ(a) (sizeof(a) / sizeof(a[0]))
#define ALIGN_PAD 16

static long get_us(void)
{
	struct timeval tv;

	sys_gettimeofday(&tv, NULL);

	return (tv.tv_sec * (1000000l)) + tv.tv_usec;
}

/* Dumb */

static void memcpy_dumb_bytes(void *to, const void *from)
{
	for (int j = 0; j < OUTTER_LOOPS; j++) {
		for (int i = 0; i < COPY_SZ; i++) {
			const char *cfrom = (from + i);
			char *cto = (to + i);

			*cto = *cfrom;
		}
	}
}

static void memcpy_dumb_words(void *to, const void *from)
{
	for (int j = 0; j < OUTTER_LOOPS; j++) {
		for (int i = 0; i < COPY_SZ; i += 2) {
			const uint16_t *wfrom = (from + i);
			uint16_t *wto = (to + i);

			*wto = *wfrom;
		}
	}
}

static void memcpy_dumb_longs(void *to, const void *from)
{
	for (int j = 0; j < OUTTER_LOOPS; j++) {
		for (int i = 0; i < COPY_SZ; i += 4) {
			const uint32_t *lfrom = (from + i);
			uint32_t *lto = (to + i);

			*lto = *lfrom;
		}
	}
}

/* movem */

static inline void movem_copy32(void *dest, const void *src)
{
	asm volatile ("movem.l (%[from]), %%d0-%%d7\n"
				  "movem.l %%d0-%%d7, (%[to])\n"
				  :
				  : [from] "a" (src), [to] "a" (dest)
				  : "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
}

static void memcpy_movem_32(void *to, const void *from)
{
	for (int j = 0; j < OUTTER_LOOPS; j++) {
		for (int i = 0; i < COPY_SZ; i += 32) {
			const char *cfrom = (from + i);
			char *cto = (to + i);

			movem_copy32(cto, cfrom);
		}
	}
}

/* move16 */
static inline void move16_copy32(void *dest, const void *src)
{
	asm volatile ("move16 (%[from])+, (%[to])+\n"
				  "move16 (%[from])+, (%[to])+\n"
				  :
				  : [from] "a" (src), [to] "a" (dest));
}

static inline void move16_copy64(void *dest, const void *src)
{
	asm volatile ("move16 (%[from])+, (%[to])+\n"
				  "move16 (%[from])+, (%[to])+\n"
				  "move16 (%[from])+, (%[to])+\n"
				  "move16 (%[from])+, (%[to])+\n"
				  :
				  : [from] "a" (src), [to] "a" (dest));
}

static inline void move16_copy128(void *dest, const void *src)
{
	asm volatile ("move16 (%[from])+, (%[to])+\n"
				  "move16 (%[from])+, (%[to])+\n"
				  "move16 (%[from])+, (%[to])+\n"
				  "move16 (%[from])+, (%[to])+\n"
				  "move16 (%[from])+, (%[to])+\n"
				  "move16 (%[from])+, (%[to])+\n"
				  "move16 (%[from])+, (%[to])+\n"
				  "move16 (%[from])+, (%[to])+\n"
				  :
				  : [from] "a" (src), [to] "a" (dest));
}

static void memcpy_move16_32(void *to, const void *from)
{
	for (int j = 0; j < OUTTER_LOOPS; j++) {
		for (int i = 0; i < COPY_SZ; i += 32) {
			const char *cfrom = (from + i);
			char *cto = (to + i);

			move16_copy32(cto, cfrom);
		}
	}
}

static void memcpy_move16_64(void *to, const void *from)
{
	for (int j = 0; j < OUTTER_LOOPS; j++) {
		for (int i = 0; i < COPY_SZ; i += 64) {
			const char *cfrom = (from + i);
			char *cto = (to + i);

			move16_copy64(cto, cfrom);
		}
	}
}

static void memcpy_move16_128(void *to, const void *from)
{
	for (int j = 0; j < OUTTER_LOOPS; j++) {
		for (int i = 0; i < COPY_SZ; i += 128) {
			const char *cfrom = (from + i);
			char *cto = (to + i);

			move16_copy128(cto, cfrom);
		}
	}
}

struct memcpy_impl {
	const char* name;
	void (*func)(void *to, const void *from);
};

static const struct memcpy_impl impls[] = {
	{ .name = "dumb_bytes", .func = memcpy_dumb_bytes },
	{ .name = "dumb_words", .func = memcpy_dumb_words },
	{ .name = "dumb_longs", .func = memcpy_dumb_longs },
	{ .name = "movem_32", .func = memcpy_movem_32 },
	{ .name = "move16_32", .func = memcpy_move16_32 },
	{ .name = "move16_64", .func = memcpy_move16_64 },
	{ .name = "move16_128", .func = memcpy_move16_128 },
};

int main(int argc, char **argv, char **envp)
{
	printf("m68k memcpy benchmark\n");

	void *buffer = malloc(BUFF_SZ + ALIGN_PAD);
	if (!buffer) {
		printf("Failed to allocate buffer\n");
		return -1;
	}

	buffer = (void*) ((((uint32_t) buffer) + ALIGN_PAD) & ~(ALIGN_PAD - 1));

	const void *from = buffer;
	void *to = buffer + COPY_SZ;

	printf("Will do %d copies of %d bytes from 0x%x to 0x%x\n",
		   OUTTER_LOOPS, COPY_SZ, (unsigned int) from, (unsigned int) to);

	for (int i = 0; i < ARRAY_SZ(impls); i++) {
		const struct memcpy_impl *impl = &impls[i];

		long start = get_us();
		impls->func(to, from);
		long end = get_us();
		long duration = end - start;

		//float speed = TEST_SZ / duration;
		printf("%s\t%ld uS (xx MiB/s)\n", impl->name, duration);
	}

	return 0;
}
