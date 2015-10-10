#! /usr/bin/env/ python
# -*- coding: utf-8 -*-

import random

def gcd(n, m):
    u"""
    $B%f!<%/%j%C%I$N8_=|K!$K$h$C$F:GBg8xLs?t$r5a$a$k4X?t!%(B

    : $B%Q%i%a!<%?(B
    n: $B<+A3?t!%(B
    m: $B<+A3?t!%(B

    : $BLa$jCM(B
    $B:GBg8xLs?t!%(B
    """
    assert(n > 0 and m > 0)
    # n >= m$B$H$9$k!%(B
    if n < m:
        m, n = n, m

    while m != 0:
        r = n % m
        n = m
        m = r

    return n

def fermat_test(p):
    u"""
    $BAG?t$+$I$&$+$r(BFermat test$B$K$h$C$FH=Dj$9$k4X?t!%(BPF$B!J56M[@-!K$H$J$k3NN($,(B2^(-128)$BDxEYB8:_$9$k!%(B

    : $B%Q%i%a!<%?(B
    p: $B<+A3?t!%(B

    : $BLa$jCM(B
    $BAG?t$G$"$l$P(BTrue$B!$$=$&$G$J$1$l$P(BFalse$B!%(B
    """
    if p <= 1:
        return False
    elif p == 2:
        return True

    max_rep = 10
    for i in xrange(max_rep):
        a = random.randint(2, p-1)
        if gcd(a, p) != 1:
            return False
        if a^(p-1) % p != 1:
            return False
    else:
        return True

def main():
    n = random.randint(1, 100)
    m = random.randint(1, 100)
    print('gcd({n}, {m}) = {gcd}'.format(n=n, m=m, gcd=gcd(n, m)))

    p = random.randint(1, 10000)
    print('fermat_test({p}) = {tf}'.format(p=p, tf=fermat_test(p)))

if __name__ == '__main__':
    main()

