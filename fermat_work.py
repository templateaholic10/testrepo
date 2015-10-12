#! /usr/bin/env/ python
# -*- coding: utf-8 -*-

from fermat_test import *
import numpy as np

def main():
    max_num = 10000
    result = np.zeros(max_num)
    for p in xrange(1, max_num+1):
        # 1-max_numまでのmax_num個の数について
        counter = 0
        for i in xrange(1, max_num+1):
            # max_num回Fermat testを行う．
            if unit_fermat_test(p):
                counter += 1
        result[p-1] = counter
    filename = 'fermat_work.txt'
    np.savetxt(filename, result, fmt='%d', delimiter=',')

if __name__ == '__main__':
    main()
