#include "get_size.h"

void string_get_size(u64 size, u32 blk_size, const enum string_size_units units,
		     char *buf, int len)
{
	static const char *const units_10[] = {
		"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"
	};
	static const char *const units_2[] = {
		"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB"
	};
	static const char *const *const units_str[] = {
		[STRING_UNITS_10] = units_10,
		[STRING_UNITS_2] = units_2,
	};
	static const unsigned int divisor[] = {
		[STRING_UNITS_10] = 1000,
		[STRING_UNITS_2] = 1024,
	};
	int order = 0, j;
	u64 remainder = 0;
	u32 sf_cap;
	char tmp[8];
	const char *unit;

	tmp[0] = '\0';
	if (!size)
		goto out;

	/*
	 * size can be huge and doing size * blk_size right away can overflow.
	 * As a first step reduce huge size to something less than
	 * blk_size * divisor[units].
	 */
	while (size > (u64)blk_size * divisor[units]) {
		remainder = do_div(size, divisor[units]);
		order++;
	}

	/* Now we're OK with doing size * blk_size, it won't overflow. */
	size *= blk_size;
	remainder *= blk_size;
	/*
	 * We were doing partial multiplication by blk_size.
	 * remainder >= divisor[units] here means size should be increased.
	 */
	size += remainder / divisor[units];
	remainder -= (remainder / divisor[units]) * divisor[units];

	/*
	 * Normalize. size >= divisor[units] means we still have enough
	 * precision and dropping remainder is fine.
	 */
	while (size >= divisor[units]) {
		remainder = do_div(size, divisor[units]);
		order++;
	}

	sf_cap = size;
	for (j = 0; sf_cap*10 < 1000; j++)
		sf_cap *= 10;

	if (j) {
		remainder *= 1000;
		remainder /= divisor[units];
		/* remainder is < divisor[units] here, (u32) is legit */
		snprintf(tmp, sizeof(tmp), ".%03u", (u32)remainder);
		tmp[j+1] = '\0';
	}

 out:
	if (order >= ARRAY_SIZE(units_2))
		unit = "UNK";
	else
		unit = units_str[units][order];

	/* size is < divisor[units] here, (u32) is legit */
	snprintf(buf, len, "%u%s %s", (u32)size,
		 tmp, unit);
}
