#!/usr/bin/python

import fileinput
import re
from fractions import Fraction as F

suffix_value = dict()
x=F(1)
for s in ["B", "kB",  "MB",  "GB",  "TB",  "PB",  "EB",  "ZB",  "YB"]:
    suffix_value[s] = x
    x *= 1000
x=F(1)
for s in ["B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB"]:
    suffix_value[s] = x
    x *= 1024

bad = []

def check(nr, size, blk, out):
    prod = F(size)*F(blk)
    num,suffix = out.split(" ")

    if re.search("\.[0-9]$", num):
        ulp = F("0.1")
    elif re.search("\.[0-9][0-9]$", num):
        ulp = F("0.01")
    else:
        ulp = F("1")

    num = F(num)
    if num * suffix_value[suffix] <= prod < (num + ulp)*suffix_value[suffix]:
        pass
    else:
        # Assume that we do get the correct suffix...
        bad.append((nr, out, float(prod/suffix_value[suffix]), long(prod), size, blk))


for line in fileinput.input():
    nr,size,blk,out = line.strip().split("\t")
    check(nr, size, blk, out)

if bad:
    print "%s\t%-8s\t%-10s\t%-24s\t%-20s\t%-8s\n" % ("test#", "output", "true", "product", "size", "blk"),

# bad.sort(key = lambda x: x[2])
for x in bad:
    print "%s\t" % x[0],
    print "%-8s\t" % x[1],
    print "%10.4f\t" % x[2],
    print "%24s\t" % str(x[3]),
    print "%20s\t" % str(x[4]),
    print "%8s\n" % str(x[5]),
