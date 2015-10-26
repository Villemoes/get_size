#include "get_size.h"

void string_get_size(u64 size, u32 blk_size, const enum string_size_units units,
		     char *buf, int len)
{
	static const char units_str[][9][4] = {
		[STRING_UNITS_10] = {"B", "kB",  "MB",  "GB",  "TB",  "PB",  "EB",  "ZB",  "YB"},
		[STRING_UNITS_2]  = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB"},
	};
	static const unsigned int divisor[] = {
		[STRING_UNITS_10] = 1000,
		[STRING_UNITS_2] = 1024,
	};
	int i, j;
	u32 remainder = 0, sf_cap;
	char tmp[8];
	const char *unit;

	tmp[0] = '\0';
	i = 0;

	while (size > U64_MAX/blk_size) {
		do_div(size, divisor[units]);
		i++;
	}
	size *= blk_size;
	while (size > divisor[units]) {
		remainder = do_div(size, divisor[units]);
		i++;
	}

	sf_cap = size;
	for (j = 0; sf_cap && sf_cap*10 < 1000; j++)
		sf_cap *= 10;

	if (i && j) {
		remainder *= 1000;
		remainder /= divisor[units];
		snprintf(tmp, sizeof(tmp), ".%03u", remainder);
		tmp[j+1] = '\0';
	}

	if (i >= ARRAY_SIZE(units_str[0]))
		unit = "UNK";
	else
		unit = units_str[units][i];

	snprintf(buf, len, "%u%s %s", (u32)size, tmp, unit);
}
