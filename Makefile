CC=gcc
CFLAGS := -O2 -g -Wall -Wextra -Wno-sign-compare
depssuffix := deps

all: test-rasmus test-rasmus2 test-current test-vitaly

-include .*.$(depssuffix)

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MF $(dir $*).$(notdir $*).$(depssuffix) -c -o $@ $<

test-%: test.o %.o
	$(CC) -o $@ $^

%.err: test-%
	./$< | ./check_get_size.py > $@
