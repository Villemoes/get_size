#include <stdio.h>

#include "get_size.h"

static u32 testnr;

static void
do_one(u64 size, u32 blk_size)
{
	char buf_si[16];
	char buf_iec[16];
	
	string_get_size(size, blk_size, STRING_UNITS_10, buf_si, sizeof(buf_si));
	string_get_size(size, blk_size, STRING_UNITS_2, buf_iec, sizeof(buf_iec));

	printf("%06u\t%llu\t%u\t%s\n", ++testnr, size, blk_size, buf_si);
	printf("%06u\t%llu\t%u\t%s\n", ++testnr, size, blk_size, buf_iec);
}

static void
do_powers(u32 base, u32 blk_size)
{
	u64 p;

	for (p = 1; p < U64_MAX/base && (p *= base); ) {
		do_one(p-1, blk_size);
		do_one(p, blk_size);
		do_one(p+1, blk_size);
	}
}

int main(int argc, char *argv[])
{
	static const u64 sizes[] = {0, 1, 42, 500, 501, 1234, 1440, 5000, 987654321, ~0ULL};
	static const u32 blk_sizes[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
	unsigned i, j;

	for (j = 0; j < ARRAY_SIZE(blk_sizes); ++j) {
		/*
		 * Doing all powers of a few small numbers get
		 * reasonable coverage across the u64 space. Also try
		 * them plus/minus 1 to catch some corner cases
		 * (that's especially important when doing powers of
		 * 2 or 10).
		 */

		do_powers(2, blk_sizes[j]);
		do_powers(3, blk_sizes[j]);
		do_powers(5, blk_sizes[j]);
		do_powers(7, blk_sizes[j]);
		do_powers(10, blk_sizes[j]);

		/* Also check a pseudo-random collection of sizes. */
		for (i = 0; i < ARRAY_SIZE(sizes); ++i)
			do_one(sizes[i], blk_sizes[j]);
	}

	return 0;
}
