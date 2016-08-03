#!/usr/bin/env python
# coding: utf-8

import sys

def main():

    # カテゴリIDを列番号に対応させる
    catid_dict = {}
    with open('cnames', 'r') as fin:
        # 参照用
        for line in fin:
            col, catid = line.rstrip().split('\t')
            catid_dict[catid] = int(col)

    # 第一カテゴリをキー，属するカテゴリIDの列番号集合を値とする辞書を作る
    first_catid_dict = {}
    for line in sys.stdin:
        elms = line.rstrip().split('\t')
        catid = elms[0]
        if len(elms) < 3:
            continue
        first_catid = elms[2]
        if first_catid not in first_catid_dict:
            first_catid_dict[first_catid] = set()
        first_catid_dict[first_catid].add(catid_dict[catid])

    # 列グラフラプラシアンを出力する
    for first_catid in first_catid_dict:
        size = len(first_catid_dict[first_catid])
        if size <= 1:
            continue
        for col1 in first_catid_dict[first_catid]:
            for col2 in first_catid_dict[first_catid]:
                if col1 == col2:
                    # 対角成分
                    print(str(col1)+','+str(col2)+','+str(size-1))
                else:
                    # 非対角成分
                    print(str(col1)+','+str(col2)+','+'-1')

if __name__ == '__main__':
    main()
