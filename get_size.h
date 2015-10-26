#ifndef GET_SIZE_H
#define GET_SIZE_H

#include <stdio.h>

typedef unsigned long long u64;
typedef unsigned int u32;
#define U64_MAX (~(u64)0)
#define U32_MAX (~(u32)0)

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

#define do_div(n,base) ({					\
			u32 __base = (base);			\
			u32 __rem;				\
			__rem = ((u64)(n)) % __base;		\
			(n) = ((u64)(n)) / __base;		\
			__rem;					\
		})

enum string_size_units {
	STRING_UNITS_10,	/* use powers of 10^3 (standard SI) */
	STRING_UNITS_2,		/* use binary powers of 2^10 */
};

void string_get_size(u64 size, u32 blk_size, const enum string_size_units units,
		char *buf, int len);


#endif /* GET_SIZE_H */
