#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import random
import numpy as np

def deal(n, k):
    """
    0, ..., n-1をk個のビンに振り分ける関数．
    それぞれの属するビンを返す
    """
    return [random.choice(range(k)) for i in range(n)]

def make_true(m, n, r, vals):
    """
    真の行列を作る関数
    """
    table = [[random.choice(vals) for j in range(n)] for i in range(m)]
    row_cat = deal(m, r)
    col_cat = deal(n, r)
    return np.array([[table[row_cat[i]][col_cat[j]] for j in range(n)]] for i in range(m)])

def main(argv):
    pass

if __name__ == "__main__":
    main(sys.argv)
