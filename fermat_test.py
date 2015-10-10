#! /usr/bin/env/ python
# -*- coding: utf-8 -*-

import random

def gcd(n, m):
    u"""
    ユークリッドの互除法によって最大公約数を求める関数．

    : パラメータ
    n: 自然数．
    m: 自然数．

    : 戻り値
    最大公約数．
    """
    assert(n > 0 and m > 0)
    # n >= mとする．
    if n < m:
        m, n = n, m

    while m != 0:
        r = n % m
        n = m
        m = r

    return n

def fermat_test(p):
    u"""
    素数かどうかをFermat testによって判定する関数．PF（偽陽性）となる確率が2^(-128)程度存在する．

    : パラメータ
    p: 自然数．

    : 戻り値
    素数であればTrue，そうでなければFalse．
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

