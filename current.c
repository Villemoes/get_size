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
	int i, j;
	u32 remainder = 0, sf_cap, exp;
	char tmp[8];
	const char *unit;

	tmp[0] = '\0';
	i = 0;
	if (!size)
		goto out;

	while (blk_size >= divisor[units]) {
		remainder = do_div(blk_size, divisor[units]);
		i++;
	}

	exp = divisor[units] / (u32)blk_size;
	/*
	 * size must be strictly greater than exp here to ensure that remainder
	 * is greater than divisor[units] coming out of the if below.
	 */
	if (size > exp) {
		remainder = do_div(size, divisor[units]);
		remainder *= blk_size;
		i++;
	} else {
		remainder *= size;
	}

	size *= blk_size;
	size += remainder / divisor[units];
	remainder %= divisor[units];

	while (size >= divisor[units]) {
		remainder = do_div(size, divisor[units]);
		i++;
	}

	sf_cap = size;
	for (j = 0; sf_cap*10 < 1000; j++)
		sf_cap *= 10;

	if (j) {
		remainder *= 1000;
		remainder /= divisor[units];
		snprintf(tmp, sizeof(tmp), ".%03u", remainder);
		tmp[j+1] = '\0';
	}

 out:
	if (i >= ARRAY_SIZE(units_2))
		unit = "UNK";
	else
		unit = units_str[units][i];

	snprintf(buf, len, "%u%s %s", (u32)size,
		 tmp, unit);
}
