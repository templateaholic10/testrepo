#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import subprocess
import json

from_dirname = "lab"
from_full_dirname = os.path.join(os.path.dirname(os.path.abspath(__file__)), from_dirname)
to_dirname = "/usr/local/include"
utilize_filenamefilename = "utilize.json"

def copy(dirname, filenames=None):
    u"""
    ディレクトリ内の特定のファイルをコピーする関数
    """
    # ファイルコピー
    if filenames is None:
        filenames = subprocess.check_output(["ls", os.path.join(from_full_dirname, dirname)]).split()
    for filename in filenames:
        full_filename = os.path.join(os.path.join(from_full_dirname, dirname), filename)
        to_full_filename = os.path.join(to_dirname, os.path.join(dirname, filename.split(".")[0]))
        subprocess.call('sh -c "sudo cp {from_filename} {to_filename}"'.format(from_filename=full_filename, to_filename=to_full_filename), shell=True)

def main():
    with open(utilize_filenamefilename) as fin:
        utilize_filename = json.load(fin)
    for k, v in utilize_filename.items():
        copy(k, v)

if __name__ == "__main__":
    main()
